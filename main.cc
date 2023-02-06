//
// Created by ICraveSleep on 05-Feb-23.
//

#include "amt21_driver.h"

int main() {
//    uint8_t buffer[2] = {0xE4, 0xF9};
//    uint16_t test = ((buffer[1] << 8) | (buffer[0]));
//    test = (test & 0b0011111111111111);  // Remove checksum to get 0x39E4
//    std::cout << test << std::endl;  // 14bit encoder value
//    std::cout << (test >> 2) << std::endl; // 12bit encoder value

  uint16_t encoder_value;
  float angle;
  Amt21Driver driver = Amt21Driver("/dev/ttyUSB0", false, 120000);
  driver.Open();

  encoder_value = driver.GetEncoderPosition();
  std::cout << "Encoder Position: " << encoder_value << std::endl;
  angle = driver.GetEncoderAngle();
  std::cout << "Angle: " << angle << std::endl;
  driver.Close();

  return 0;
}
