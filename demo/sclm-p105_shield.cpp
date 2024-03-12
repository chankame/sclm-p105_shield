#include "sclm-p105_shield.h"

#ifdef PROTOTYPE
#define CLK_WAIT 0
#else
#define CLK_WAIT 5
#endif

SclmP105Shield::SclmP105Shield(
    uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3, uint8_t db4, uint8_t db5, uint8_t db6, uint8_t db7,
    uint8_t busy, uint8_t clk, uint8_t _cs, uint8_t _sw1, uint8_t _sw2)
    : DB0(db0), DB1(db1), DB2(db2), DB3(db3), DB4(db4), DB5(db5), DB6(db6), DB7(db7), BUSY(busy), CLK(clk), _CS(_cs), _SW1(_sw1), _SW2(_sw2), buffer{}, dataLength(0x1b)
{
#ifdef PROTOTYPE
    BUSY = 10;
    CLK = 9;
    _CS = 8;
    _SW1 = 12;
    _SW2 = 11;
#endif
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

    // T LU M LD B RD RU ?
    Write(0x1b, 0b11011110);
    Write(0x1c, 0b00000110);
    Write(0x1d, 0b10111010);
    Write(0x1e, 0b10101110);
    Write(0x1f, 0b01100110);
    Write(0x20, 0b11101100);
    Write(0x21, 0b11111100);
    Write(0x22, 0b10000110);
    Write(0x23, 0b11111110);
    Write(0x24, 0b11101110);
    Write(0x25, 0b11110110);
    Write(0x26, 0b01111100);
    Write(0x27, 0b11011000);
    Write(0x28, 0b00111110);
    Write(0x29, 0b11111000);
    Write(0x2a, 0b11110000);
    
    Update();
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
#if true
    while(digitalRead(BUSY));
    while(digitalRead(BUSY));
    digitalWrite(_CS, LOW);
//  delayMicroseconds(CLK_WAIT);
    for(int i=0; i<dataLength; i++){
        while(digitalRead(BUSY));
        digitalWrite(CLK, HIGH);
        auto data = buffer[i];
#ifdef PROTOTYPE
        PORTD = bitrev(data);
#else
        PORTD = data;
#endif
        delayMicroseconds(CLK_WAIT);
        while(digitalRead(BUSY));
        digitalWrite(CLK, LOW);
        delayMicroseconds(CLK_WAIT);
    }
//  while(digitalRead(BUSY));
    digitalWrite(_CS, HIGH);
#else
    while(digitalRead(BUSY));
    while(digitalRead(BUSY));
    digitalWrite(_CS, LOW);
    delayMicroseconds(6);
    for(int i=0; i<27; i++){
        auto data = buffer[i];
        digitalWrite(CLK, HIGH);
#ifdef PROTOTYPE
        PORTD = bitrev(data);
#else
        PORTD = data;
#endif
        delayMicroseconds(6);
        while(digitalRead(BUSY));
        digitalWrite(CLK, LOW);
        delayMicroseconds(6);
    }
    delayMicroseconds(4);
    digitalWrite(_CS, HIGH);
#endif
    if(dataLength > 0x1b){
        dataLength = 0x1b;
        while(digitalRead(BUSY));
        Update();
    }
}

uint8_t SclmP105Shield::Read(uint8_t address)
{
    if(address >= bufferLength) return 0;
    return buffer[address];
}

void SclmP105Shield::Write(uint8_t address, uint8_t data)
{
    if(address >= bufferLength) return;
    buffer[address] = data;
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
}

void SclmP105Shield::Color(Segment segment, ::Color color)
{
    Write(segment, static_cast<uint8_t>(color) << 5);
}

void SclmP105Shield::Digit(Segment segment, uint8_t number, ::Color color)
{
    Write(segment, number & 0x1f | static_cast<uint8_t>(color) << 5);
}

void SclmP105Shield::Number(int32_t number, ::Color color, Line line, bool decimalPoint)
{
    Color(Segment::Colon, Color::Black);
    if(decimalPoint){
        switch(line){
        case Line::None:
            Color(Segment::DecimalPointUpper, Color::Black);
            Color(Segment::DecimalPointLower, color);
            break;
        case Line::Upper:
            Color(Segment::DecimalPointUpper, color);
            break;
        case Line::Lower:
            Color(Segment::DecimalPointLower, color);
            break;
        }
    } else {
        switch(line){
        case Line::None:
            Color(Segment::DecimalPointUpper, Color::Black);
            Color(Segment::DecimalPointLower, Color::Black);
            break;
        case Line::Upper:
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
        segment--;
        Digit(Segment::Digit9, 0, Color::Black);
    }
    if(number < 0){
        isMinus = true;
        number *= -1;
    }
    for(; segment>=segmentTop; segment--){
        Digit(static_cast<Segment>(segment), number%10, color);
        number /= 10;
        if(number == 0 && color != Color::Black){
            if(isMinus && segment>segmentTop) Digit(static_cast<Segment>(--segment), 0x12, color);
            color = Color::Black;
        }
    }
}

void SclmP105Shield::Number(float number, ::Color color, Line line)
{
    Number(static_cast<int32_t>(number * 10), color, line, true);
}

void SclmP105Shield::ResetGlyph(uint8_t address)
{
    Write(address, glyph[(address-0x1b)&0x0f]);
}