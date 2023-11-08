#include "./Common/tdFile.hpp"
#include "./Common/tdImage.h"
#include <string.h>
#include <stdlib.h>
#include "BitImage.h"
#include "BitFontGen.h"


static void ScanDir_FON(const char* dir, bool mini)
{
    BitFontGen bitFont;
    tdFileFind find;
    char fileIn[128];
    char file_name_cpp[128];
    char file_name_png[128];

    size_t dirLen = strlen(dir);
    memcpy(fileIn, dir, dirLen);
    if(dirLen && (fileIn[dirLen-1] != '/' || fileIn[dirLen-1] != '\\'))
        fileIn[dirLen++] = '/';
    memcpy(file_name_cpp, fileIn, dirLen);
    memcpy(file_name_png, fileIn, dirLen);
    size_t lenBase_cpp = dirLen;
    size_t lenBase_png = dirLen;
    if(mini)
    {
        memcpy(file_name_cpp+dirLen, "font95/", 8);
        memcpy(file_name_png+dirLen, "font95/png/", 12);
        lenBase_cpp += 7;
        lenBase_png += 11;
    }else{
        memcpy(file_name_cpp+dirLen, "font/", 6);
        memcpy(file_name_png+dirLen, "font/png/", 10);
        lenBase_cpp += 5;
        lenBase_png += 9;
    }
    tdFile_CreateFolder(file_name_cpp);
    tdFile_CreateFolder(file_name_png);

    memcpy(file_name_cpp+lenBase_cpp, "BitFont_", 8);
    memcpy(file_name_png+lenBase_png, "BitFont_", 8);
    lenBase_cpp += 8;
    lenBase_png += 8;

    memcpy(&fileIn[dirLen], "*.fon", 6);
    if(!find.Start(fileIn, false))
        return;
    while(find.Find(&fileIn[dirLen]))
    {
        if(!bitFont.CreateFromFON(fileIn, mini))
            continue;
        size_t len = strlen(&fileIn[dirLen]);
        size_t prefix_len = len-6-4;
        while(prefix_len > 3)
        {
            if(fileIn[dirLen+6+prefix_len-1] >= '0' && fileIn[dirLen+6+prefix_len-1] <= '9')
            {
                bool isX = false;
                if(fileIn[dirLen+6+prefix_len-2] == 'x')
                {
                    isX = true;
                    prefix_len -=2;
                }else if(fileIn[dirLen+6+prefix_len-3] == 'x')
                {
                    isX = true;
                    prefix_len -=3;
                }
                if(isX)
                {
                    if(fileIn[dirLen+6+prefix_len-1] >= '0' && fileIn[dirLen+6+prefix_len-1] <= '9')
                        prefix_len--;
                    if(fileIn[dirLen+6+prefix_len-1] >= '0' && fileIn[dirLen+6+prefix_len-1] <= '9')
                        prefix_len--;
                }
                if(fileIn[dirLen+6+prefix_len-1] == '_')
                {
                    prefix_len--;
                    break;
                }
            }
            if(fileIn[dirLen+6+prefix_len-3] == '-' && fileIn[dirLen+6+prefix_len-2] == '2'&& fileIn[dirLen+6+prefix_len-1] == 'y')
            {
                prefix_len -= 3;
                continue;
            }
            if(fileIn[dirLen+6+prefix_len-3] == '-' && fileIn[dirLen+6+prefix_len-2] == '2'&& fileIn[dirLen+6+prefix_len-1] == 'x')
            {
                prefix_len -= 3;
                continue;
            }
            break;
        }
        for(size_t i = 0; i < prefix_len; i++)
        {
            if(fileIn[dirLen+6+i] == '-')
                fileIn[dirLen+6+i] = '_';
        }

        prefix_len += sprintf(&fileIn[dirLen+6+prefix_len], "_%ux%u", bitFont.m_width, bitFont.m_height);
        fileIn[dirLen+6+prefix_len] = 0;

        memcpy(file_name_cpp+lenBase_cpp, &fileIn[dirLen+6], prefix_len);
       // sprintf(file_name_cpp+lenBase_cpp, "_%ux%u", gBitFont.m_width, gBitFont.m_height);
        memcpy(file_name_cpp+lenBase_cpp + prefix_len, ".cpp", 5);
        memcpy(file_name_png+lenBase_png, file_name_cpp+lenBase_cpp, prefix_len);
        memcpy(file_name_png+lenBase_png + prefix_len, ".png", 5);

        bitFont.SavePNG(file_name_png);
        bitFont.SaveCPP(file_name_cpp, &fileIn[dirLen+6]);
    }
}


bool BitImageToFileC(const BitImage* img, const char* fileName, const char* prefix)
{
    tdFile file;
    if(!file.Open(fileName, tdFile::crw))
        return false;
    char txt[128];
    file.Write("\r\n\r\nexern const unsigned char bitImage_", 39);
    file.Write(prefix, strlen(prefix));
    uint32_t data_size = ((img->m_width+7)>>3)*img->m_height;
    file.Write(txt, sprintf(txt, "[%u] = {\r\n    ", data_size+4));
    file.Write(txt, sprintf(txt, "0x%.2X,0x%.2X, 0x%.2X,0x%.2X, // width, height",
        img->m_width&0xFF, (img->m_width>>8)&0xFF, img->m_height&0xFF, (img->m_height>>8)&0xFF));
    for(size_t i = 0; i < data_size; i++)
    {
        if(i)
            file.Write(",", 1);
        if((i&0x0F) == 0)
            file.Write("\r\n    ", 6);
        file.Write(txt, sprintf(txt, "0x%.2X", img->m_data[i]));
    }
    file.Write("\r\n};\r\n", 6);
    file.Close();
    return true;
}


bool GrayImageToBitImageC(const char* ImagFileName, const char* outFileC, uint8_t grayTreshold, const char* prefix)
{
    tdImage inImg;
    if(!inImg.Load(ImagFileName))
        return false;
    if(inImg.m_width > 0x7FFF || inImg.m_height > 0x7FFF)
        return false;
    tdImage::TYPE pixFm = inImg.Type();
    BitImage bitImage(inImg.m_width, inImg.m_height);
    bitImage.Clear(false);

    if(pixFm == tdImage::TYPE::GREY)
    {
        const uint8_t* src = inImg.m_data;
        for(int32_t h = 0; h < inImg.m_height; h++)
        {
            for(int32_t w = 0; w < inImg.m_width; w++)
            {
                bitImage.SetPixel(w, h, src[w] < grayTreshold);
            }
            src += inImg.m_step;
        }
    }
    if(pixFm == tdImage::TYPE::BGR)
    {
        const uint8_t* src = inImg.m_data;
        for(int32_t h = 0; h < inImg.m_height; h++)
        {
            for(int32_t w = 0; w < inImg.m_width; w++)
            {
                size_t oY = (19595*src[w*3 + 2] + 38469*src[w*3 + 1] + 7471*src[w*3 + 0])>>16;
                bitImage.SetPixel(w, h, oY < grayTreshold);
            }
            src += inImg.m_step;
        }
    }
    if(pixFm == tdImage::TYPE::BGRA)
    {
        const uint8_t* src = inImg.m_data;
        for(int32_t h = 0; h < inImg.m_height; h++)
        {
            for(int32_t w = 0; w < inImg.m_width; w++)
            {
                size_t oY = (19595*src[w*4 + 2] + 38469*src[w*4 + 1] + 7471*src[w*4 + 0])>>16;
                bitImage.SetPixel(w, h, oY < grayTreshold);
            }
            src += inImg.m_step;
        }
    }
    return BitImageToFileC(&bitImage, outFileC, prefix);
}



int main(int argc, char* argv[])
{

 //   https://int10h.org/oldschool-pc-fonts/download/
    // <zip>\fon - Bm (windows bitmap)\*.FON


    ScanDir_FON(".\\data\\font", true); return 0;



//     for(size_t i = 1; i < 17; i++)
//     {
//         char fileIn[64], fileOut[64], prefix[32];
//         sprintf(fileIn, ".\\data\\human\\%.3u.png", i);
//         sprintf(fileOut, ".\\data\\out\\human%.2u.h", i);
//         sprintf(prefix, "human%u", i);
//         GrayImageToBitImageC(fileIn, fileOut, 30, prefix);
//     }
    return 0;
}





