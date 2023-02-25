//
// Created by ICraveSleep on 05-Feb-23.
//

#include "amt21_driver.h"
#include "bitset"

int main() {
  uint16_t encoder_value;
  float angle;
  Amt21Driver driver = Amt21Driver("/dev/ttyUSB0",
                                   AMT21Resolution::k14Bit,
                                   AMT21BaudRate::k115200,
                                   AMT21TurnType::kSingleTurn);

  bool opened = driver.Open();
  if(!opened){
    std::cout << "No access to usb port. Please give read and write access" << std::endl;
    return 0;
  }

  encoder_value = driver.GetEncoderPosition();
  std::cout << "Encoder Position: " << encoder_value << std::endl;
//  driver.SetZeroPosition();
//  encoder_value = driver.GetEncoderPosition();
//  std::cout << "Encoder Position: " << encoder_value << std::endl;
  int counter = 0;
  while (counter < 100) {
    angle = driver.GetEncoderPosition();
    std::cout << "Angle: " << angle << std::endl;
    counter++;
  }

//  angle = driver.GetEncoderAngle();
//  std::cout << "Angle: " << angle << std::endl;
  driver.Close();

  return 0;
}
