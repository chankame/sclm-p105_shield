#include "sclm-p105_shield.h"

SclmP105Shield *sclmP105Shield;

void setup() {
	// put your setup code here, to run once:
	sclmP105Shield = new SclmP105Shield();
}

void loop() {
	// put your main code here, to run repeatedly:
	sclmP105Shield->Reset();
	delay(1000);

	// Back Light
	sclmP105Shield->String("Back Light");
	sclmP105Shield->Update();
	delay(1000);
	sclmP105Shield->String("Off", Color::White, Line::Upper);
	sclmP105Shield->Update();
	delay(1000);
	sclmP105Shield->BackLight(BackLight::Off);
	sclmP105Shield->Update();
	delay(1000);
	sclmP105Shield->String("Low", Color::White, Line::Upper);
	sclmP105Shield->BackLight(BackLight::Low);
	sclmP105Shield->Update();
	delay(1000);
	sclmP105Shield->String("High", Color::White, Line::Upper);
	sclmP105Shield->BackLight(BackLight::High);
	sclmP105Shield->Update();
	delay(1000);
	// Color
	for(uint8_t i=0; i<0x11; i++) sclmP105Shield->Color(i, Color::White);
	sclmP105Shield->String("ColorWhite", Color::White);
	sclmP105Shield->Update();
	delay(1000);
	for(uint8_t i=0; i<0x11; i++) sclmP105Shield->Color(i, Color::Yellow);
	sclmP105Shield->String("Yellow", Color::Yellow);
	sclmP105Shield->Update();
	delay(1000);
	for(uint8_t i=0; i<0x11; i++) sclmP105Shield->Color(i, Color::Magenta);
	sclmP105Shield->String("Magenta", Color::Magenta);
	sclmP105Shield->Update();
	delay(1000);
	for(uint8_t i=0; i<0x11; i++) sclmP105Shield->Color(i, Color::Red);
	sclmP105Shield->String("Red", Color::Red);
	sclmP105Shield->Update();
	delay(1000);
	for(uint8_t i=0; i<0x11; i++) sclmP105Shield->Color(i, Color::Cyan);
	sclmP105Shield->String("Cyan", Color::Cyan);
	sclmP105Shield->Update();
	delay(1000);
	for(uint8_t i=0; i<0x11; i++) sclmP105Shield->Color(i, Color::Green);
	sclmP105Shield->String("Green", Color::Green);
	sclmP105Shield->Update();
	delay(1000);
	for(uint8_t i=0; i<0x11; i++) sclmP105Shield->Color(i, Color::Blue);
	sclmP105Shield->String("Blue", Color::Blue);
	sclmP105Shield->Update();
	delay(1000);
	sclmP105Shield->String("0123456789");
	for(uint8_t i=0; i<32; i++){
		for(uint8_t j=0; j<0x1b; j++){
			sclmP105Shield->Color(j, static_cast<Color>((i+j)%8));
		}
		sclmP105Shield->Update();
		delay(100);
	}
	// integer
	sclmP105Shield->Cls();
	for(int i=0; i<1000; i++){
		if((i/500)%2){
			sclmP105Shield->Number(i, Color::Red, Line::Upper);
			sclmP105Shield->Number(-i, Color::Blue, Line::Lower);
		} else {
			sclmP105Shield->Number(i);
		}
		sclmP105Shield->Update();
	}
	// float
	for(float i=0; i<100; i+=0.1){
		if(((int)i/50)%2){
			sclmP105Shield->Number(i, Color::Magenta, Line::Upper);
			sclmP105Shield->Number(-i, Color::Cyan, Line::Lower);
		} else {
			sclmP105Shield->Number(i, Color::Green);
		}
		sclmP105Shield->Update();
	}
	// Hex
	for(uint32_t i=0x00f0; i<0x010f; i++){
		sclmP105Shield->Number(static_cast<int32_t>(i), Color::Yellow, Line::Upper);
		sclmP105Shield->Hex(i, Color::Magenta, Line::Lower);
		sclmP105Shield->Update();
		delay(100);
	}
	// Time
	for(uint32_t ms=millis(); millis()<ms+10000; ){
		sclmP105Shield->Time(millis(), Color::Cyan);
		sclmP105Shield->Update();
	}
	// Mix
	sclmP105Shield->Cls();
	sclmP105Shield->String("COUNT", Color::Magenta, Line::Upper);
	for(int i=-500; i<0; i++){
		sclmP105Shield->Number(i, Color::Yellow, Line::Lower);
		sclmP105Shield->Update();
	}
	sclmP105Shield->Cls();
	sclmP105Shield->String("COUNT", Color::Magenta, Line::Lower);
	for(int i=0; i<500; i++){
		sclmP105Shield->Number(i, Color::Yellow, Line::Upper);
		sclmP105Shield->Update();
	}
	// String
	sclmP105Shield->Cls();
	sclmP105Shield->String("HelloWorld");
	sclmP105Shield->Update();
	delay(1000);
	sclmP105Shield->Cls();
	sclmP105Shield->String("Upper", Color::Magenta, Line::Upper);
	sclmP105Shield->Update();
	delay(1000);
	sclmP105Shield->Cls();
	sclmP105Shield->String("Lower", Color::Cyan, Line::Lower);
	sclmP105Shield->Update();
	delay(1000);
	sclmP105Shield->String("Upper", Color::Magenta, Line::Upper);
	sclmP105Shield->Update();
	delay(1000);
	String lipsum = "          The quick brown fox jumps over the lazy dog";
	for(int i=0; i<lipsum.length(); i++){
		sclmP105Shield->Cls();
		sclmP105Shield->String(&lipsum[i]);
		sclmP105Shield->Update();
		delay(200);
	}
	// Custom Glyph
	sclmP105Shield->Cls();
	uint8_t fall[] = {
		0b10000000,
		0b10100000,
		0b10101000,
		0b00101000,
		0b00001000,
	};
	uint8_t guru[] = {
		0b10000000,
		0b00000010,
		0b00000100,
		0b00001000,
		0b00010000,
		0b01000000,
	};
	for(int i=0; i<40; i++){
		for(uint8_t j=0; j<5; j++){
			sclmP105Shield->Glyph(j, fall[(i+j)%5]);
			sclmP105Shield->Write(0x11+j, j | (7+(i+j)/5) << 5);
			sclmP105Shield->Glyph((5+j), guru[(i+j)%6]);
			sclmP105Shield->Write(0x16+j, (5+j) | (j%6) << 5);
		}
		sclmP105Shield->Update();
		delay(100);
	}
}
