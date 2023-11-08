#pragma once

class BitFontGen
{
public:
    const static uint8_t BIT_FONT_FLAG_95_CHARS = 1;
    uint8_t  m_height;
    uint8_t  m_width;
    uint8_t  m_step;
    uint8_t  m_flags;
    uint8_t* m_data;

    BitFontGen();
    ~BitFontGen();
    void Init(uint8_t width, uint8_t height, uint8_t flags);
    bool Create(const char* fontName, uint8_t defH = 0, uint8_t defW = 0, bool mini = true);
    bool CreateFromFON(const char* fontName, bool mini);

    void DrawChar(tdImage& img, uint16_t x, uint16_t y, uint8_t chr);

    void SavePNG(const char* fileName);
    bool SaveCPP(const char* fileName, const char* prefix);
};

