/**
* @author:  Egorov Sergey <listrid@yandex.ru>
**/
#pragma once

#ifndef _TDIMAGE_H_
#define _TDIMAGE_H_
#include <stdint.h>

//#define _TD_IMAGE_PPM //подключить ppm сжатие
// #ifdef _WIN32
   #include <windows.h>
   #define _TD_IMAGE_WIN32
//   #ifdef TDIMAGE_EXPORTS
//      #define TDIMAGE_API __declspec(dllexport)
//   #else
//      #define TDIMAGE_API __declspec(dllimport)
//   #endif
// #else
  #define TDIMAGE_API
//#endif


#pragma pack(push, 1)
struct tdBGR
{
    uint8_t b,g,r;
    static tdBGR BGR(uint8_t b, uint8_t g, uint8_t r){tdBGR ret;ret.b=b;ret.g=g;ret.r=r;return ret;};
    inline operator size_t() const { return (r << 16) | (g << 8) | b; }
};

struct tdBGRA
{
    uint8_t b, g, r, a;
    static tdBGRA BGRA(uint8_t b, uint8_t g, uint8_t r, uint8_t a){tdBGRA ret;ret.b=b;ret.g=g;ret.r=r; ret.a=a;return ret;};
    inline operator size_t() const { return (a << 24) | (r << 16) | (g << 8) | b; }
};

struct tdRECT
{
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;

    inline uint32_t x() const {return right;};
    inline uint32_t y() const {return top;};
    inline uint32_t width() const {return right - left;};
    inline uint32_t height()const {return bottom - top;};
};

struct tdBOX
{
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
};

struct tdPOINT
{
    uint32_t x;
    uint32_t y;
};

#pragma pack(pop)

#ifdef _TD_IMAGE_PPM
class tdPPM_In;
class tdPPM_Out;
#endif

#ifndef _WINDEF_
    typedef struct HFONT__ *HFONT;
#endif

#pragma pack(push, 4)
typedef struct TDIMAGE_API tdImage
{
    enum TYPE
    {
        FREE = 0, GREY, BGR, BGRA,
        //не обрабатывается (для просмотра)
        C3
    };
    int32_t  m_width;
    int32_t  m_height;
    uint32_t m_step;
    uint8_t* m_data;
#ifdef _TD_IMAGE_WIN32
    bool Print(HFONT font, const char* text, size_t color, uint32_t DT_format);
    static tdPOINT PrintSize(HFONT font, const char* text, uint32_t DT_format);
#endif
    size_t Print(int32_t x, int32_t y, const char* txt, size_t color, bool origin);
    bool DrawLine(size_t x0, size_t y0, size_t x1, size_t y1, size_t color);
    bool DrawTriangleFill(size_t Ax, size_t Ay, size_t Bx, size_t By, size_t Cx, size_t Cy, size_t color);
    bool DrawCircle(size_t x, size_t y, size_t r, size_t color);
    bool DrawCircleFill(size_t Cx, size_t Cy, size_t R, size_t color);
    bool DrawEllipseFill(size_t Cx, size_t Cy, size_t A, size_t B, size_t color);
    bool Draw(tdImage* img, tdRECT* r = (tdRECT*)0);//только для BGR или BGRA
    void Flip();

    void Create(tdImage::TYPE type, size_t width, size_t height, size_t step = 0);
    void Set(tdImage::TYPE t, size_t w, size_t h, size_t step, void* d){Free();m_type=t; m_width=int32_t(w);m_height=int32_t(h);m_step=uint32_t(step);m_data=(uint8_t*)d;m_isFree = false;};
    void Set(tdImage* img){Free();m_type=img->m_type; m_width=img->m_width;m_height=img->m_height;m_step=img->m_step;m_data=(uint8_t*)img->m_data;m_isFree = false;};
    void Copy(const tdImage* src);
    bool ResizeNN(const tdImage* src);//NearestNeighbor
    bool ResizeSS(const tdImage* src);//SuperSampling


    TYPE Type() const {return m_type;};
    void Free();
    tdImage(){m_width=m_height=m_step=0;m_data = (uint8_t*)0;m_type = FREE;};
    tdImage(tdImage::TYPE t, size_t w, size_t h, size_t step, void* d){ m_data = (uint8_t*)0; this->Set(t, w, h, step, d);};
    tdImage(tdImage::TYPE t, size_t w, size_t h, size_t step = 0)     { m_data = (uint8_t*)0; this->Create(t, w, h, step);};
    tdImage(const tdImage *img){m_data = (uint8_t*)0; this->Copy(img);}
    ~tdImage(){Free();};
#ifdef _TD_IMAGE_WIN32
    bool SaveBMP(const char* nameFile) const;
    bool SaveJPG(const char* nameFile, size_t Quality) const;
    bool SavePNG(const char* nameFile) const;
#endif
#ifdef _TD_IMAGE_PPM
    bool SaveIPP(const char* nameFile) const;
    bool Save(tdPPM_Out* outPPM) const;
    bool Load(tdPPM_In*  inPPM);
#endif
#ifdef _TD_IMAGE_WIN32
    bool Load(const char* nameFile);
    bool Load(const void* data, size_t size);
#endif
    void operator=(tdImage& img)
    {
        this->Set(&img);
    };
private:
    TYPE m_type;//тип пикселя в картинке
    bool m_isFree;
}tdImage; 
#pragma pack(pop)

#endif //_TDIMAGE_H_
