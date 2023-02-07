//
// Created by ICraveSleep on 05-Feb-23.
//

#include "amt21_driver.h"
#include "bitset"

int main() {
  uint16_t encoder_value;
  float angle;

  Amt21Driver driver = Amt21Driver("/dev/ttyUSB0", false, AMT21BaudRate::k115200);
  driver.Open();
  encoder_value = driver.GetEncoderPosition();
  std::cout << "Encoder Position: " << encoder_value << std::endl;
  int counter = 0;
  while(counter < 100){
    angle = driver.GetEncoderAngle();
    std::cout << "Angle: " << angle << std::endl;
    counter ++;
  }

//  angle = driver.GetEncoderAngle();
//  std::cout << "Angle: " << angle << std::endl;
  driver.Close();

  return 0;
}
