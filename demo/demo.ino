#include "sclm-p105_shield.h"

SclmP105Shield *sclmP105Shield;

void setup() {
  // put your setup code here, to run once:
  sclmP105Shield = new SclmP105Shield(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  update();
  sclmP105Shield->Cls();
  for(uint8_t i=0; ; i++){
    for(uint8_t j=0; j<10; j++){
      sclmP105Shield->Digit(static_cast<Segment>(0x11 + j), i & 0x1f);
    }
    sclmP105Shield->Update();
    delay(1000);
    for(uint8_t j=0; j<8; j++){
      sclmP105Shield->Write(static_cast<Segment>(0x1b + (i & 0x1f)), 1 << j);
      sclmP105Shield->Digit(static_cast<Segment>(0x1a), j);
      sclmP105Shield->Update();
      delay(1000);
    }
    sclmP105Shield->Color(Segment::InfusionVolume, Color::White);
    sclmP105Shield->ResetGlyph(0x1b + (i & 0x1f));
    sclmP105Shield->Update();
    delay(1000);
    sclmP105Shield->Cls();
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