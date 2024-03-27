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
	static constexpr inline uint8_t DB0 = 0;
	static constexpr inline uint8_t DB1 = 1;
	static constexpr inline uint8_t DB2 = 2;
	static constexpr inline uint8_t DB3 = 3;
	static constexpr inline uint8_t DB4 = 4;
	static constexpr inline uint8_t DB5 = 5;
	static constexpr inline uint8_t DB6 = 6;
	static constexpr inline uint8_t DB7 = 7;
#ifdef PROTOTYPE
	static constexpr inline uint8_t BUSY = 10;
	static constexpr inline uint8_t CLK = 9;
	static constexpr inline uint8_t _CS = 8;
	static constexpr inline uint8_t _SW1 = 12;
	static constexpr inline uint8_t _SW2 = 11;
#else
	static constexpr inline uint8_t BUSY = 8;
	static constexpr inline uint8_t CLK = 9;
	static constexpr inline uint8_t _CS = 10;
	static constexpr inline uint8_t _SW1 = 11;
	static constexpr inline uint8_t _SW2 = 12;
#endif
	static constexpr inline uint8_t bufferLength = 0x3b;
	static constexpr inline uint8_t backBufferLength = 10;
	static constexpr inline uint8_t glyph[] = {
		// T LU M LD B RD RU ?
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
		0b11011100, // G
		0b01110100, // H
		0b01010000, // I
		0b00011110, // J
		0b11110100, // K
		0b01011000, // L
		0b11010110, // M
		0b00110100, // N
		0b00111100, // O
		0b11110010, // P
		0b11100110, // Q
		0b00110000, // R
		0b01101100, // S
		0b01111000, // T
		0b00011100, // U
		0b01011110, // V
		0b01001010, // W
		0b01110110, // X
		0b01101110, // Y
		0b10011010, // Z
		0b00000000, // 
	};
	uint8_t buffer[bufferLength];
	uint8_t backBuffer[backBufferLength];
	uint8_t dataLength;
	uint16_t bitString;

#ifdef PROTOTYPE
	uint8_t bitrev(uint8_t bits);
#endif
public:
	SclmP105Shield();
	void Update();
	uint8_t Read(uint8_t address);
	uint8_t Read(Segment segment);
	void Write(uint8_t address, uint8_t data);
	void Write(Segment segment, uint8_t data);
	void BackLight(BackLight backLight);
	void Cls();
	void Color(uint8_t address, ::Color color);
	void Color(Segment segment, ::Color color);
	void Digit(uint8_t address, uint8_t number, ::Color color = Color::White);
	void Digit(Segment segment, uint8_t number, ::Color color = Color::White);
	void Number(int32_t number, ::Color color = Color::White, Line line = Line::None, bool decimalPoint = false);
	void Number(int16_t number, ::Color color = Color::White, Line line = Line::None);
	void Number(float number, ::Color color = Color::White, Line line = Line::None);
	void Hex(uint32_t number, ::Color color = Color::White, Line line = Line::None);
	void Time();
	void String(::String string, ::Color color = Color::White, Line line = Line::None);
	void Glyph(uint8_t id, uint8_t glyph);
	void GlyphChar(uint8_t id, char c);
	void ResetGlyph(uint8_t address, uint8_t glyphID = 0);
	void Reset();
};