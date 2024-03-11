#define PROTOTYPE
#include <Arduino.h>

enum class BackLight
{
    High,
    Low,
    Off,
};

enum class Color
{
    White,
    Yellow,
    Magenta,
    Red,
    Cyan,
    Green,
    Blue,
    Black,
};

enum class Segment
{
    Backlight,
    Colon,
    DecimalPointUpper,
    DecimalPointLower,
    System,
    Unknown,
    Time,
    MilliLiter,
    MilliLiterPerHour,
    InfusionVolume,
    Cumulative,
    FastForward,
    Schedule,
    End,
    Occlusion,
    Plunger,
    Battery,
    Digit0,
    Digit1,
    Digit2,
    Digit3,
    Digit4,
    Digit5,
    Digit6,
    Digit7,
    Digit8,
    Digit9,
};

enum class Line
{
    None,
    Upper,
    Lower,
};


class SclmP105Shield
{
    static constexpr uint8_t bufferLength = 0x3b;
    static constexpr uint8_t glyph[] = {
        0b11011110, // 0 
        0b00000110, // 1
        0b10111010, // 2
        0b10101110, // 3
        0b01100110, // 4
        0b11101100, // 5
        0b11111100, // 6
        0b10000110, // 7
        0b11111110, // 8
        0b11101110, // 9
        0b11110110, // A
        0b01111100, // B
        0b11011000, // C
        0b00111110, // D
        0b11111000, // E
        0b11110000, // F
    };
    uint8_t DB0;
    uint8_t DB1;
    uint8_t DB2;
    uint8_t DB3;
    uint8_t DB4;
    uint8_t DB5;
    uint8_t DB6;
    uint8_t DB7;
    uint8_t BUSY;
    uint8_t CLK;
    uint8_t _CS;
    uint8_t _SW1;
    uint8_t _SW2;
    uint8_t buffer[bufferLength];
    uint8_t dataLength;

    uint8_t bitrev(uint8_t bits);
public:
    SclmP105Shield(uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3, uint8_t db4, uint8_t db5, uint8_t db6, uint8_t db7, uint8_t busy, uint8_t clk, uint8_t _cs, uint8_t _sw1, uint8_t _sw2);
    void Update();
    uint8_t Read(uint8_t address);
    void Write(uint8_t address, uint8_t data);
    void Write(Segment segment, uint8_t data);
    void BackLight(BackLight backLight);
    void Cls();
    void Color(Segment segment, Color color);
    void Digit(Segment segment, uint8_t number, ::Color color = Color::White);
    void Number(int32_t number, ::Color color = Color::White, Line line = Line::None, bool decimalPoint = false);
    void Number(float number, ::Color color = Color::White, Line line = Line::None);
};