//
// Created by ICraveSleep on 05-Feb-23.
//

#include "amt21_driver.h"
#include "bitset"

int main() {

//    uint8_t buffer[2] = {0xE4, 0xF9};
//    uint16_t test = ((buffer[1] << 8) | (buffer[0]));
//    test = (test & 0b0011111111111111);  // Remove checksum to get 0x39E4
//    std::cout << test << std::endl;  // 14bit encoder value
//    std::cout << (test >> 2) << std::endl; // 12bit encoder value


  /** Checkbit Formula
  Odd: K1 = !(H5^H3^H1^L7^L5^L3^L1)
  Even: K0 = !(H4^H2^H0^L6^L4^L2^L0)
  From the above response 0x61AB:
  Odd: 0 = !(1^0^0^1^1^1^1) = correct
  Even: 1 = !(0^0^1^0^0^0^1) = correct
  **/

  uint16_t response = 0x61AB;
//  uint8_t k1 = (response >> 15); // Is going to be 0
//  uint8_t k0 = (response >> 14); // Is going to be 1
//  std::cout << "k1: " << +k1 << std::endl;
//  std::cout << "k0: " << +k0 << std::endl;
//  std::bitset<16> response_bits(response);
//  std::cout << response_bits << std::endl;
//  uint8_t h5 = (response >> 13) & 0b00000001;
//  std::bitset<8> h5_bits(h5);
//  std::cout << "H5 : " << +h5 << std::endl;
//  std::cout << "H5 bits: " << h5_bits << std::endl;
//
//  uint8_t odd = !(response >> 13 & 0b00000001) ^
//      (response >> 11 & 0b00000001) ^
//      (response >> 9 & 0b00000001) ^
//      (response >> 7 & 0b00000001) ^
//      (response >> 5 & 0b00000001) ^
//      (response >> 3 & 0b00000001) ^
//      (response >> 1 & 0b00000001);
//  std::cout << "Odd: " << +odd << std::endl;  // Is going to be 0
//
//  uint8_t even = !(response >> 12 & 0b00000001) ^
//      (response >> 10 & 0b00000001) ^
//      (response >> 8 & 0b00000001) ^
//      (response >> 6 & 0b00000001) ^
//      (response >> 4 & 0b00000001) ^
//      (response >> 2 & 0b00000001) ^
//      (response & 0b00000001);
//  std::cout << "Even: " << +even << std::endl;  // Is going to be 1


  uint16_t encoder_value;
  float angle;

  Amt21Driver driver = Amt21Driver("/dev/ttyUSB0", false, 120500);
  driver.Open();
  encoder_value = driver.GetEncoderPosition();
  std::cout << "Encoder Position: " << encoder_value << std::endl;
  angle = driver.GetEncoderAngle();
  std::cout << "Angle: " << angle << std::endl;
  driver.Close();

  return 0;
}
