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

  // Mix
  sclmP105Shield->Cls();
  sclmP105Shield->String("SCORE", Color::Magenta, Line::Upper);
  for(int i=0; i<2000; i++){
    sclmP105Shield->Number(-i, Color::Yellow, Line::Lower);
    sclmP105Shield->Update();
  }
//return;

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
//String lipsum = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  String lipsum = "Lorem ipsum dolor sit amet";
  for(int i=0; i<lipsum.length(); i++){
    sclmP105Shield->Cls();
    sclmP105Shield->String(&lipsum[i]);
    sclmP105Shield->Update();
    delay(256-i/2);
  }
}
