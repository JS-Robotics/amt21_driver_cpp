//
// Created by Sondre on 05-Feb-23.
//

#include "include/amt21_driver.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "amt21_driver.h"

int main() {
    char t = 1;
    char a = 1;
    int val = ((t << 8 ) | (a)) >> 2; // 0000 0001 0000 0001
    std::cout << val << std::endl;
    int c = (a & 0b0011111111111111);

    uint8_t buffer[2] = {0xE4, 0xF9};
    uint16_t test = ((buffer[1] << 8) | (buffer[0]));
    test = (test & 0b0011111111111111);  // Remove checksum to get 0x39E4
    std::cout << test << std::endl;  // 14bit encoder value
    std::cout << (test >> 2) << std::endl; // 12bit encoder value

    Amt21Driver driver = Amt21Driver();
    driver.Open();

    driver.Close();
    return 0;
}
