#include "./Common/tdFile.hpp"
#include "./Common/tdImage.h"
#include <windows.h>
#include "BitFontGen.h"


#pragma pack(push, 1)

struct MzHeader
{
    uint16_t signature;
    uint16_t extraBytes;
    uint16_t pages;
    uint16_t relocationItems;
    uint16_t headerSize;
    uint16_t minimumAllocation;
    uint16_t maximumAllocation;
    uint16_t initialSS;
    uint16_t initialSP;
    uint16_t checkSum;
    uint16_t initialIP;
    uint16_t initialCS;
    uint16_t relocationTable;
    uint16_t overlay;
    uint16_t res1;
    uint16_t res2;
    uint16_t res3;
    uint16_t res4;
    uint16_t res5;
    uint16_t res6;
    uint16_t res7;
    uint16_t res8;
    uint16_t res9;
    uint16_t res10;
    uint16_t res11;
    uint16_t res12;
    uint16_t res13;
    uint16_t res14;
    uint16_t res15;
    uint16_t res16;
    uint16_t offsetNEHeader;
};

typedef struct NeHeader
{
    uint8_t  magic_n;
    uint8_t  magic_e;
    uint8_t  linker_version;
    uint8_t  linker_revision;
    uint16_t entry_table_offset;
    uint16_t entry_table_size;
    uint32_t crc32;
    uint16_t module_flags;
    uint16_t auto_data_segment;
    uint16_t dynamic_heap_size;
    uint16_t stack_size;
    uint16_t reg_ip;
    uint16_t reg_cs;
    uint16_t reg_sp;
    uint16_t reg_ss;
    uint16_t num_segment_table_entries;
    uint16_t num_module_ref_table_entries;
    uint16_t non_resident_name_table_size;
    uint16_t segment_table_offset;
    uint16_t resource_table_offset;
    uint16_t resident_name_table_offset;
    uint16_t module_reference_table_offset;
    uint16_t imported_names_table_offset;
    uint32_t non_resident_name_table_offset;
    uint16_t num_movable_entries;
    uint16_t sector_alignment_shift_count;
    uint16_t num_resource_entries;
    uint8_t  exe_type;
    uint8_t  os2_exe_flags;
    uint8_t  reserved[8];
} NeHeader;

typedef struct NeSegmentTableEntry
{
    uint16_t offset;
    uint16_t size;
    uint16_t segment_flags;
    uint16_t minimum_allocation;
} NeSegmentTableEntry;

struct RESOURCE_ENTRY
{
    uint16_t offset;
    uint16_t length;
    uint16_t flags;
    uint16_t id;
    uint16_t handle;
    uint16_t usage;
};

struct NE_NAMEINFO
{
    uint16_t offset;
    uint16_t length;
    uint16_t flags;
    uint16_t id;
    uint16_t handle;
    uint16_t usage;
};

struct NE_TYPEINFO
{
    uint16_t type_id;
    uint16_t count;
    uint32_t resloader;
    NE_NAMEINFO nameinfo[1];
};

struct NE_RESTABLE
{
    uint16_t  size_shift;
    NE_TYPEINFO typeinfo[1];
};

struct FONTINFO_V2
{
    uint16_t dfType;
    uint16_t dfPoints;
    uint16_t dfVertRes;
    uint16_t dfHorizRes;
    uint16_t dfAscent;
    uint16_t dfInternalLeading;
    uint16_t dfExternalLeading;
    uint8_t  dfItalic;
    uint8_t  dfUnderline;
    uint8_t  dfStrikeOut;
    uint16_t dfWeight;
    uint8_t  dfCharSet;
    uint16_t dfPixWidth;
    uint16_t dfPixHeight;
    uint8_t  dfPitchAndFamily;
    uint16_t dfAvgWidth;
    uint16_t dfMaxWidth;
    uint8_t  dfFirstChar;
    uint8_t  dfLastChar;
    uint8_t  dfDefaultChar;
    uint8_t  dfBreakChar;
    uint16_t dfWidthBytes;
    uint32_t dfDevice;
    uint32_t dfFace;
    uint32_t dfBitsPointer;
    uint32_t dfBitsOffset;
    uint8_t  dfReserved;
    //dfVersion == 0x300 +18
//     uint32_t dfFlags;
//     uint16_t dfAspace;
//     uint16_t dfBspace;
//     uint16_t dfCspace;
//     uint32_t dfColorPointer;
//     uint32_t dfReserved1[4];
};

struct FNT_HEADER
{
    uint16_t dfVersion;
    uint32_t dfSize;
    char     dfCopyright[60];
    FONTINFO_V2 dffi;
} ;

struct CHAR_TABLE_ENTRY
{
    uint16_t width;
    uint16_t offset;
};

#pragma pack(pop)


#define NE_RSCTYPE_CURSOR		0x8001
#define NE_RSCTYPE_BITMAP		0x8002
#define NE_RSCTYPE_ICON			0x8003
#define NE_RSCTYPE_MENU			0x8004
#define NE_RSCTYPE_DIALOG		0x8005
#define NE_RSCTYPE_STRING		0x8006
#define NE_RSCTYPE_FONTDIR		0x8007
#define NE_RSCTYPE_FONT			0x8008
#define NE_RSCTYPE_ACCELERATOR	0x8009
#define NE_RSCTYPE_RCDATA		0x800a
#define NE_RSMESSAGETABLE       0x800b
#define NE_RSCTYPE_GROUP_CURSOR	0x800c
#define NE_RSCTYPE_GROUP_ICON	0x800e


static void ParseFNT(BitFontGen* bitFont, FNT_HEADER* font, bool mini)
{
    CHAR_TABLE_ENTRY* charTbl;
    charTbl = (CHAR_TABLE_ENTRY*)&font[1];
    if(font->dfVersion == 0x300)
    charTbl = (CHAR_TABLE_ENTRY*)((char*)charTbl + 18);

    bitFont->Init((uint8_t)font->dffi.dfPixWidth, (uint8_t)font->dffi.dfPixHeight, mini?BitFontGen::BIT_FONT_FLAG_95_CHARS:0);
    size_t c_count = 256;
    if(mini)
        c_count = 95;
    for(size_t ch = 0; ch < c_count; ch++)
    {
        CHAR_TABLE_ENTRY* charInf = &charTbl[ch];
        if(mini)
            charInf = &charTbl[ch+' '];
        uint8_t* charData = (uint8_t*)font + charInf->offset;
        size_t  outPos = bitFont->m_step*ch;
        size_t  mask_bit = 0x80;
        uint8_t outPix8  = 0;

        for(size_t h = 0; h < bitFont->m_height; h++)
        {
            size_t inPix8 = 0;
            size_t inPix8Count = 0;
            for(size_t w = 0; w < bitFont->m_width; w++)
            {
                if(inPix8Count == 0)
                {
                    if(w == 0)
                    {
                        inPix8 = charData[h];
                    }else{
                        inPix8 = charData[(w>>3)*bitFont->m_height+h];
                    }
                    inPix8Count = 8;
                }
                inPix8Count--;
                bool val = (inPix8 & (1<<inPix8Count) ) != 0;
                if(val)
                    outPix8 |= mask_bit;
                mask_bit >>= 1;
                if(!mask_bit)
                {
                    bitFont->m_data[outPos++] = outPix8;
                    outPix8  = 0;
                    mask_bit = 0x80;
                }
            }
        }
        if(mask_bit != 0x80)
            bitFont->m_data[outPos] = outPix8;
    }
}

bool BitFontGen::CreateFromFON(const char* fontName, bool mini)
{
    tdFile file;
    if(!file.Open(fontName, tdFile::r))
        return false;
    size_t fon_file_size = (size_t)file.Size();
    if(fon_file_size > 0x100000)
        return false;

    uint8_t* fon_data = (uint8_t*)malloc(fon_file_size);
    file.Read(fon_data, fon_file_size);

    MzHeader* mz = (MzHeader*)fon_data;
    if(mz->signature != ('M' | ('Z' << 8)))
    {
        free(fon_data);
        return false;
    }
    NeHeader* ne = (NeHeader*)&fon_data[mz->offsetNEHeader];
    if(ne->magic_e != 'E' || ne->magic_n  != 'N')
    {
        free(fon_data);
        return false;
    }
    NE_RESTABLE* resTable = (NE_RESTABLE*)&fon_data[mz->offsetNEHeader + ne->resource_table_offset];

    uint16_t font_typeinfo_num = ~0;
    uint16_t font_offset = 0;
    for(size_t i = 0; resTable->typeinfo[i].type_id;i++)
    {
        if(resTable->typeinfo[i].type_id == NE_RSCTYPE_FONT)
        {
            font_typeinfo_num = i;
            break;
        }
    }
    if(font_typeinfo_num == ~0)
    {
        free(fon_data);
        return false;
    }
    NE_NAMEINFO* nameinfo = resTable->typeinfo[font_typeinfo_num].nameinfo;
    bool ret = false;
    for(size_t i = 0; i < resTable->typeinfo[font_typeinfo_num].count; i++)
    {
        uint32_t fontOffset = nameinfo[i].offset << resTable->size_shift;
        uint32_t fontLength = nameinfo[i].length << resTable->size_shift;
        if(fontOffset+fontLength > fon_file_size)
            break;
        FNT_HEADER* pFont = (FNT_HEADER*)&fon_data[fontOffset];
        ParseFNT(this, pFont, mini);
        ret = true;
        break;
    }
    free(fon_data);
    return ret;
}

