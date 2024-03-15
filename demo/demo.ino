#include "sclm-p105_shield.h"

SclmP105Shield *sclmP105Shield;

void setup() {
  // put your setup code here, to run once:
  sclmP105Shield = new SclmP105Shield();
  delay(1000);
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
  return;

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

  return;

  String lipsum = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  for(int i=0; i<lipsum.length(); i++){
    sclmP105Shield->Cls();
    sclmP105Shield->String(&lipsum[i]);
    sclmP105Shield->Update();
    delay(200);
  }
  return;

  for(uint8_t i=0; i<10; i++){
    sclmP105Shield->Digit(static_cast<Segment>(0x11 + i), 0);
  }
  for(uint8_t i=0; i<36; i++){
    sclmP105Shield->ResetGlyph(0x1b, i);
    sclmP105Shield->Update();
    delay(1000);
  }
  for(float i=0; ; i+=0.1){
    if(((int)i/50)%2){
      sclmP105Shield->Number(i, Color::Red, Line::Upper);
      sclmP105Shield->Number(-i, Color::Blue, Line::Lower);
    } else {
      sclmP105Shield->Number(i);
    }
    sclmP105Shield->Update();
  }

  // バックライト
  BackLight backlights[] = {BackLight::High, BackLight::Low, BackLight::Off, BackLight::Low, BackLight::High};
  for(auto &&bl : backlights){
    sclmP105Shield->BackLight(bl);
    sclmP105Shield->Update();
    delay(1000);
  }

  // 色
  for(uint8_t i=0; i<8; i++){
    for(uint8_t address=1; address<0x1b; address++){
      sclmP105Shield->Color(static_cast<Segment>(address), static_cast<Color>((i+1)%8));
      sclmP105Shield->Update();
      delay(100);
    }
  }

  // 数字
  for(uint8_t i=0; i<0x10; i++){
    for(uint8_t address=0x11; address<0x1b; address++){
      sclmP105Shield->Digit(static_cast<Segment>(address), i);
    }
    sclmP105Shield->Update();
    delay(1000);
  }
}

#if false
void update()
{
  static uint8_t frame;
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
#if true
  if(digitalRead(12) == LOW){
    mode = (mode + 1) % 3;
  }
  if(digitalRead(11) == LOW){
    switch(mode){
      case 0: address = (address + 1) % 0x2b; break;
      case 1: 
      {
        auto data = sclmP105Shield->Read(address);
        data = (data >> 5 & 0x07) + 1 << 5 | data & 0x1f;
        sclmP105Shield->Write(address, data);
      }
      break;
      case 2: 
      {
        auto data = sclmP105Shield->Read(address);
        data = data & 0xe0 | data + 1 & 0x1f;
        sclmP105Shield->Write(address, data);
      }
      break;
    }
  }

  auto select = (mode == 0 && frame % 2) ? 0x20 : 0x00;
  sclmP105Shield->Write(18+0, select | address/16);
  sclmP105Shield->Write(18+1, select | address%16);
  auto data = sclmP105Shield->Read(address);
  select = (mode == 1 && frame % 2) ? 0x20  : 0x00;
  sclmP105Shield->Write(18+2, select | data/16);
  select = (mode == 2 && frame % 2) ? 0x20 : 0x00;
  sclmP105Shield->Write(18+3, select | data%16);
#endif
  sclmP105Shield->Update();
  frame++;
}
#endif