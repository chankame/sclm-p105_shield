#include "sclm-p105_shield.h"

#define CLK_WAIT 5

SclmP105Shield::SclmP105Shield(bool prototype) : prototype(prototype), buffer{}, backBuffer{}, dataLength(0x1b), bitString(0)
{
	if(prototype){
		BUSY = 10;
		CLK = 9;
		_CS = 8;
		_SW1 = 12;
		_SW2 = 11;
	} else {
		BUSY = 8;
		CLK = 9;
		_CS = 10;
		_SW1 = 11;
		_SW2 = 12;
	}
	pinMode(DB0, OUTPUT);
	pinMode(DB1, OUTPUT);
	pinMode(DB2, OUTPUT);
	pinMode(DB3, OUTPUT);
	pinMode(DB4, OUTPUT);
	pinMode(DB5, OUTPUT);
	pinMode(DB6, OUTPUT);
	pinMode(DB7, OUTPUT);
	pinMode(BUSY, INPUT);
	pinMode(CLK, OUTPUT);
	pinMode(_CS, OUTPUT);
	pinMode(_SW1, INPUT_PULLUP);
	pinMode(_SW2, INPUT_PULLUP);
	PORTD = 0;
	digitalWrite(CLK, LOW);
	digitalWrite(_CS, HIGH);

	Reset();
}

uint8_t SclmP105Shield::bitrev(uint8_t bits)
{
	return bits >> 7 & 0b00000001
		 | bits >> 5 & 0b00000010
		 | bits >> 3 & 0b00000100
		 | bits >> 1 & 0b00001000
		 | bits << 1 & 0b00010000
		 | bits << 3 & 0b00100000
		 | bits << 5 & 0b01000000
		 | bits << 7 & 0b10000000;
}

void SclmP105Shield::Update()
{
	if(dataLength == 0x1b){
		for(uint8_t i=0; i<backBufferLength; i++){
			buffer[0x11+i] = backBuffer[i];
		}
	}
	while(digitalRead(BUSY));
	while(digitalRead(BUSY));
	digitalWrite(_CS, LOW);
	if(prototype){
		for(uint8_t i=0; i<dataLength; i++){
			while(digitalRead(BUSY));
			digitalWrite(CLK, HIGH);
			auto data = buffer[i];
			PORTD = bitrev(data);
			delayMicroseconds(CLK_WAIT);
			while(digitalRead(BUSY));
			digitalWrite(CLK, LOW);
			delayMicroseconds(CLK_WAIT);
		}
	} else {
		for(uint8_t i=0; i<dataLength; i++){
			while(digitalRead(BUSY));
			digitalWrite(CLK, HIGH);
			auto data = buffer[i];
			PORTD = data;
			while(digitalRead(BUSY));
			digitalWrite(CLK, LOW);
		}
	}
	digitalWrite(_CS, HIGH);
	if(dataLength > 0x1b){
		dataLength = 0x1b;
		while(digitalRead(BUSY));
		Update();
	}
}

uint8_t SclmP105Shield::Read(uint8_t address)
{
	if(address >= bufferLength) return 0;
	if(address >= 0x11 && address < 0x1b) return backBuffer[address - 0x11];
	return buffer[address];
}

uint8_t SclmP105Shield::Read(Segment segment)
{
	return Read(static_cast<uint8_t>(segment));
}

void SclmP105Shield::Write(uint8_t address, uint8_t data)
{
	if(address >= bufferLength) return;
	if(address >= 0x11 && address < 0x1b) backBuffer[address - 0x11] = data;
	else buffer[address] = data;
	if(dataLength < address + 1) dataLength = address + 1;
}

void SclmP105Shield::Write(Segment segment, uint8_t data)
{
	Write(static_cast<uint8_t>(segment), data);
}

void SclmP105Shield::BackLight(::BackLight backLight)
{
	Write(Segment::Backlight, static_cast<uint8_t>(backLight));
}

void SclmP105Shield::Cls()
{
	for(uint8_t i=0x01; i<0x1b; i++){
		Write(i, static_cast<uint8_t>(Color::Black) << 5);
	}
	for(uint8_t i=0; i<0x10; i++){
		Glyph(i, glyph[i]);
	}
	bitString=0;
}

void SclmP105Shield::Color(uint8_t address, ::Color color)
{
	Write(address, Read(address) & 0x1f | static_cast<uint8_t>(color) << 5);
}

void SclmP105Shield::Color(Segment segment, ::Color color)
{
	Color(static_cast<uint8_t>(segment), color);
}

void SclmP105Shield::Digit(uint8_t address, uint8_t number, ::Color color = Color::White)
{
	if(bitString == 0){
		Write(address, number & 0x1f | static_cast<uint8_t>(color) << 5);
		return;
	}
	if(number < 0x10){
		Write(address, address-0x11 | static_cast<uint8_t>(color) << 5);
		GlyphChar(address-0x11, number+'0');
	} else {
		Write(address, number & 0x1f | static_cast<uint8_t>(color) << 5);
	}
}

void SclmP105Shield::Digit(Segment segment, uint8_t number, ::Color color)
{
	Digit(static_cast<uint8_t>(segment), number, color);
}

void SclmP105Shield::Number(int32_t number, ::Color color, Line line, bool decimalPoint)
{
	if(decimalPoint){
		switch(line){
		case Line::None:
			Color(Segment::Colon, Color::Black);
			Color(Segment::DecimalPointUpper, Color::Black);
			Color(Segment::DecimalPointLower, color);
			break;
		case Line::Upper:
			Color(Segment::Colon, Color::Black);
			Color(Segment::DecimalPointUpper, color);
			break;
		case Line::Lower:
			Color(Segment::DecimalPointLower, color);
			break;
		}
	} else {
		switch(line){
		case Line::None:
			Color(Segment::Colon, Color::Black);
			Color(Segment::DecimalPointUpper, Color::Black);
			Color(Segment::DecimalPointLower, Color::Black);
			break;
		case Line::Upper:
			Color(Segment::Colon, Color::Black);
			Color(Segment::DecimalPointUpper, Color::Black);
			break;
		case Line::Lower:
			Color(Segment::DecimalPointLower, Color::Black);
			break;
		}
	}
	uint8_t segment = 0x1a;
	uint8_t segmentTop = 0x11;
	if(line == Line::Upper) segment = 0x15;
	if(line == Line::Lower) segmentTop = 0x16;
	bool isMinus = false;
	if(!decimalPoint && number > -1000 && number < 10000){
		Digit(segment--, 0, Color::Black);
	}
	if(number < 0){
		isMinus = true;
		number *= -1;
	}
	for(; segment>=segmentTop; segment--){
		Digit(segment, number%10, color);
		number /= 10;
		if(number == 0 && color != Color::Black){
			if(isMinus && segment>segmentTop) Digit(--segment, 0x12, color);
			color = Color::Black;
		}
	}
}

void SclmP105Shield::Number(int16_t number, ::Color color, Line line)
{
	Number(static_cast<int32_t>(number), color, line);
}

void SclmP105Shield::Number(float number, ::Color color, Line line)
{
	Number(static_cast<int32_t>(number * 10), color, line, true);
}

void SclmP105Shield::Hex(uint32_t number, ::Color color, Line line)
{
	switch(line){
	case Line::None:
		Color(Segment::Colon, Color::Black);
		Color(Segment::DecimalPointUpper, Color::Black);
		Color(Segment::DecimalPointLower, Color::Black);
		break;
	case Line::Upper:
		Color(Segment::Colon, Color::Black);
		Color(Segment::DecimalPointUpper, Color::Black);
		break;
	case Line::Lower:
		Color(Segment::DecimalPointLower, Color::Black);
		break;
	}
	uint8_t segment = 0x1a;
	uint8_t segmentTop = 0x11;
	if(line == Line::Upper) segment = 0x15;
	if(line == Line::Lower) segmentTop = 0x16;
	if(number < 0x10000){
		Digit(segment--, 0, Color::Black);
	}
	for(; segment>=segmentTop; segment--){
		Digit(segment, number&0xf, color);
		number >>= 4;
		if(number == 0 && color != Color::Black){
			color = Color::Black;
		}
	}
}

void SclmP105Shield::Time(uint32_t ms, ::Color color)
{
	Color(Segment::Time, color);
	Color(Segment::Colon, ms/500%2 ? Color::Black : color);
	Color(Segment::DecimalPointUpper, Color::Black);
	Color(Segment::DecimalPointLower, color);
	Digit(Segment::Digit9, ms/100%10, color);	
	Digit(Segment::Digit8, ms/1000%10, color);	
	Digit(Segment::Digit7, ms/10000%6, color);	
	Color(Segment::Digit6, Color::Black);	
	Color(Segment::Digit5, Color::Black);	
	Digit(Segment::Digit4, ms/60000%10, color);	
	Digit(Segment::Digit3, ms/600000%6, color);	
	Digit(Segment::Digit2, ms/3600000%10, color);
	if(ms/36000000){
		Digit(Segment::Digit1, ms/36000000%10, color);	
		if(ms/360000000){
			Digit(Segment::Digit0, ms/360000000%10, color);	
		} else {
			Color(Segment::Digit0, Color::Black);	
		}
	} else {
		Color(Segment::Digit1, Color::Black);	
		Color(Segment::Digit0, Color::Black);	
	}
}

void SclmP105Shield::String(::String string, ::Color color, Line line)
{
	switch(line){
	case Line::None:
		Color(Segment::Colon, Color::Black);
		Color(Segment::DecimalPointUpper, Color::Black);
		Color(Segment::DecimalPointLower, Color::Black);
		break;
	case Line::Upper:
		Color(Segment::Colon, Color::Black);
		Color(Segment::DecimalPointUpper, Color::Black);
		break;
	case Line::Lower:
		Color(Segment::DecimalPointLower, Color::Black);
		break;
	}
	uint8_t segment = 0;
	uint8_t segmentBottom = 10;
	if(line == Line::Upper) segmentBottom = 5;
	if(line == Line::Lower) segment = 5;
	for(uint8_t i=0; segment<segmentBottom; i++,segment++){
		auto c = string[i];
	//	if(c == '\0') break;
	//	Digit(static_cast<Segment>(0x11+segment), segment, color);
		Write(0x11+segment, segment | static_cast<uint8_t>(color) << 5);
		GlyphChar(segment, c);
	}
}

void SclmP105Shield::Glyph(uint8_t id, uint8_t glyph_)
{
	if(id >= 0x10) return;
	Write(0x1b + id, glyph_);
	if(glyph_ == glyph[id]) bitString &= ~(0x0001 << id);
	else                                   bitString |=   0x0001 << id;
}

void SclmP105Shield::GlyphChar(uint8_t id, char c)
{
	uint8_t code = 36;
	if(c >= '0' && c <= '9')      code = c - '0';
	else if(c >= 'A' && c <= 'Z') code = c - 'A' + 10;
	else if(c >= 'a' && c <= 'z') code = c - 'a' + 10;
	Glyph(id, glyph[code]);
}

void SclmP105Shield::Reset()
{
	for(uint8_t i=0; i<0x1b; i++){
		Write(i, 0);
	}
	for(uint8_t i=0; i<0x10; i++){
		Glyph(i, glyph[i]);
	}
	Update();
}
