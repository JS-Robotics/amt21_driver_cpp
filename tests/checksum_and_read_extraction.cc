//
// Created by Sondre on 07-Feb-23.
//
#include "iostream"
#include "bitset"

int main() {
  /** Examples provided from AMT21 datasheet: https://www.cuidevices.com/product/resource/amt21.pdf  **/

  // Checking how to get values out of encoder
  uint8_t buffer[2] = {0xE4, 0xF9};
  uint16_t test = ((buffer[1] << 8) | (buffer[0]));
  test = (test & 0b0011111111111111);  // Remove checksum to get 0x39E4
  std::cout << test << std::endl;  // 14bit encoder value
  std::cout << (test >> 2) << std::endl; // 12bit encoder value

  /** Checkbit Formula
   Odd: K1 = !(H5^H3^H1^L7^L5^L3^L1)
   Even: K0 = !(H4^H2^H0^L6^L4^L2^L0)
   From the above response 0x61AB:
   Odd: 0 = !(1^0^0^1^1^1^1) = correct
   Even: 1 = !(0^0^1^0^0^0^1) = correct

   Response comes in as:
   K1-K0-H5-H4-H3-H2-H1 L7-L6-L5-L4-L3-L2-L1-L0
  **/

  // Checking how to perform checksum evaluation
  uint16_t response = 0x61AB;
  uint8_t k1 = (response >> 15); // Is going to be 0
  uint8_t k0 = (response >> 14); // Is going to be 1
  std::cout << "k1: " << +k1 << std::endl;
  std::cout << "k0: " << +k0 << std::endl;
  std::bitset<16> response_bits(response);
  std::cout << response_bits << std::endl;
  uint8_t h5 = (response >> 13) & 0b00000001;
  std::bitset<8> h5_bits(h5);
  std::cout << "H5 : " << +h5 << std::endl;
  std::cout << "H5 bits: " << h5_bits << std::endl;

  uint8_t odd = !(response >> 13 & 0b00000001) ^  // H5
      (response >> 11 & 0b00000001) ^             // H3
      (response >> 9 & 0b00000001) ^              // H1
      (response >> 7 & 0b00000001) ^              // L7
      (response >> 5 & 0b00000001) ^              // L5
      (response >> 3 & 0b00000001) ^              // L3
      (response >> 1 & 0b00000001);               // L1
  std::cout << "Odd: " << +odd << std::endl;  // Is going to be 0

  uint8_t even = !(response >> 12 & 0b00000001) ^  // H4
      (response >> 10 & 0b00000001) ^              // H2
      (response >> 8 & 0b00000001) ^               // H0
      (response >> 6 & 0b00000001) ^               // L6
      (response >> 4 & 0b00000001) ^               // L4
      (response >> 2 & 0b00000001) ^               // L2
      (response & 0b00000001);                     // L0
  std::cout << "Even: " << +even << std::endl;  // Is going to be 1
}