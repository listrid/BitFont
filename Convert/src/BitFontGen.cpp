#include "./Common/tdFile.hpp"
#include "./Common/tdImage.h"
#include "BitFontGen.h"


BitFontGen::BitFontGen()
{
    m_data   = NULL;
    m_height = 0;
    m_width  = 0;
}

BitFontGen::~BitFontGen()
{
    if(m_data)
        free(m_data);
}

void BitFontGen::Init(uint8_t width, uint8_t height, uint8_t flags)
{
    m_height = height;
    m_width  = width;
    m_step   = (m_height*m_width + 7) >> 3;
    m_flags  = flags;
    if(m_data)
        free(m_data);
    size_t ch_count = 256;
    if(flags&BIT_FONT_FLAG_95_CHARS)
        ch_count = 95;

    m_data = (uint8_t*)malloc(m_step*ch_count);
    memset(m_data, 0xFF, m_step*ch_count);
}

void BitFontGen::DrawChar(tdImage& img, uint16_t x, uint16_t y, uint8_t ch)
{
    if(m_flags&BIT_FONT_FLAG_95_CHARS)
    {
        ch -= ' ';
        if(ch >= 95)
            ch = '?' - ' ';
    }
    if(!m_data)
        return;
    uint8_t* pos = &m_data[m_step*ch];
    size_t mask_bit = 0;
    uint8_t pix8;
    for(size_t h = 0; h < m_height; h++)
    {
        for(uint16_t w = 0; w < m_width; w++)
        {
            if(mask_bit == 0)
            {
                mask_bit = 0x80;
                pix8 = pos[0];
                pos++;
            }
            if (pix8 & mask_bit)
                img.m_data[img.m_step*y + x + w] = 0;
            else
                img.m_data[img.m_step*y + x + w] = 0xFF;
            mask_bit >>= 1;
        }
        y++;
    }
}

void BitFontGen::SavePNG(const char* fileName)
{
    tdImage img;
    size_t ch_count;
    if(m_flags&BIT_FONT_FLAG_95_CHARS)
    {
        ch_count = 95;
        img.Create(tdImage::GREY, (m_width+1)*16+1, (m_height+1)*6+1);
    }else{
        ch_count = 256;
        img.Create(tdImage::GREY, (m_width+1)*16+1, (m_height+1)*16+1);

    }
    memset(img.m_data, 0xF0, img.m_step*img.m_height);

    size_t n = 0, x = 1, y = 1;
    for(size_t c = 0; c < ch_count; c++)
    {
        if(n == 16)
        {
            n = 0;
            x = 1;
            y += m_height+1;
        }
        n++;
        if(m_flags&BIT_FONT_FLAG_95_CHARS)
        {
            DrawChar(img, x, y, ' ' + c);
        }else{
            DrawChar(img, x, y, c);
        }
        x += m_width + 1;
    }
    img.SavePNG(fileName);
}

bool BitFontGen::Create(const char* fontName, uint8_t defH, uint8_t defW, bool mini)
{
    HBITMAP    hBitmap;
    uint8_t*   pixels;
    BITMAPINFO hg;
    size_t width = 300, height = 300;
    size_t step = (width*3+3)&(-4);

    hg.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
    hg.bmiHeader.biPlanes   = 1;
    hg.bmiHeader.biBitCount = 24;
    hg.bmiHeader.biCompression = BI_RGB;
    hg.bmiHeader.biClrUsed     = 0;
    hg.bmiHeader.biClrImportant  = 0;
    hg.bmiHeader.biXPelsPerMeter = 0;
    hg.bmiHeader.biYPelsPerMeter = 0;
    hg.bmiHeader.biWidth  = width;
    hg.bmiHeader.biHeight = height;

    HFONT hFont = CreateFontA(int(defH), int(defW), 0, 0, 0, false, false, false, 
        ANSI_CHARSET , OUT_DEVICE_PRECIS , CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH, fontName);
    if(!hFont)
        return false;
    hBitmap = CreateDIBSection(0, &hg, DIB_RGB_COLORS, (void**)&pixels, 0, 0);

    HDC hDC = CreateCompatibleDC(NULL);
    SetTextColor(hDC, RGB(0,0,0));
    SelectObject(hDC, hFont);
    SelectObject(hDC, hBitmap);
    RECT rc = { 0, 0, 0, 0 };

    DrawTextA(hDC, "W", 1, &rc, DT_CALCRECT | DT_SINGLELINE | DT_NOCLIP | DT_LEFT);
    Init(uint8_t(rc.right), uint8_t(rc.bottom), mini?BIT_FONT_FLAG_95_CHARS:0);
    rc.bottom = height;
    rc.right  = width;

 //   tdImage img;
 //   img.Set(tdImage::BGR, width, height, step, pixels);
    memset(pixels, 0xFF, step*height);
    size_t c_count = 256;
    if(mini)
        c_count = 95;
    for(size_t c = 0; c < c_count; c++)
    {
        char ch = c;
        if(mini)
            ch += char(' ');

        DrawTextA(hDC, &ch, 1, &rc, DT_NOCLIP | DT_LEFT | DT_SINGLELINE);
        size_t pos = m_step*c;
        size_t  mask_bit = 0x80;
        uint8_t pix8 = 0;
        uint8_t* lineBGR = pixels +rc.left*3 + (height - rc.top - 1)*step;
        for(size_t h = 0; h < m_height; h++)
        {
            for(size_t w = 0; w < m_width; w ++)
            {
                if(lineBGR[w*3] < 0x50)
                    pix8 |= mask_bit;
                mask_bit >>= 1;
                if(!mask_bit)
                {
                    m_data[pos++] = pix8;
                    pix8 = 0;
                    mask_bit = 0x80;
                }
            }
            lineBGR -= step;
        }
        if(mask_bit != 0x80)
            m_data[pos] = pix8;
        rc.left += m_width;
        if(rc.left + m_width > rc.right)
        {
            rc.left = 0;
            rc.top += m_height;
        }
    }
//    img.SavePNG("font.png");
    DeleteObject(hBitmap);
    DeleteObject(hFont);
    DeleteDC(hDC);
    return true;
}

bool BitFontGen::SaveCPP(const char* fileName, const char* prefix)
{
    tdFile file;
    if(!m_data)
        return false;
    if(!file.Open(fileName, tdFile::crw))
        return false;
    size_t prefix_len = strlen(prefix);
    size_t data_size  = m_step*255;
    if(m_flags&BIT_FONT_FLAG_95_CHARS)
        data_size  = m_step*95;
    char txt[128];
    file.Write("\r\n\r\nextern const unsigned char BitFont_", 39);
    file.Write(prefix, prefix_len);
    file.Write(txt, sprintf(txt, "[%u] = {\r\n    ", (uint32_t)data_size+4));
    file.Write(txt, sprintf(txt, "%u, %u, %u, %u, // width, height, step, flags", m_width, m_height, m_step, m_flags));
    for(size_t i = 0; i < data_size; i++)
    {
        if(i)
            file.Write(",", 1);
        if((i&0x0F) == 0)
            file.Write("\r\n    ", 6);
        file.Write(txt, sprintf(txt, "0x%.2X", m_data[i]));
    }
    file.Write("\r\n};\r\n", 6);
    return true;
}
