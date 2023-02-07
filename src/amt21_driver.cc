//
// Created by ICraveSleep on 05-Feb-23.
//

#include "amt21_driver.h"

Amt21Driver::Amt21Driver(const std::string &port, bool encoder_12bit, uint32_t baud_rate)
    : port_(port), encoder_12bit_(encoder_12bit), baud_rate_(baud_rate) {
  node_id_ = 0x54; // AMT21 default node id
  struct termios options{};
  options.c_cflag = 0010002 | CS8 | CLOCAL | CREAD;
//  options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(fd_port_, TCIFLUSH);
  tcsetattr(fd_port_, TCSANOW, &options);
}

uint16_t Amt21Driver::GetEncoderPosition() {
  uint8_t request_package[] = {node_id_};
  int32_t reqeust_package_size = sizeof(request_package) / sizeof(request_package[0]);
  int64_t bytes_written = write(fd_port_, request_package, reqeust_package_size);

  uint8_t receive_buffer[2];
  int64_t bytes_read;
  usleep(1000000); // TODO Make some sort of continuous read at higher frequencies
  bytes_read = read(fd_port_, receive_buffer, sizeof(receive_buffer));

  if (bytes_read <= 0) {
    std::cout << "Error reading bytes" << std::endl;
  }

  uint16_t value = ((receive_buffer[1] << 8) | (receive_buffer[0]));
  value = (value & kCheckBitMask);

  if (encoder_12bit_) {
    value = (value >> 2);
  }

  //std::cout << +receive_buffer[1] << ":" << +receive_buffer[0] << std::endl;
  //std::cout << value << std::endl;

  return value;
}

void Amt21Driver::Open() {
  fd_port_ = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  std::cout << "Opened fd_port_ linked to: " << fd_port_ << std::endl;
  if (fd_port_ == -1) {
    std::cout << "Error opening the serial port" << std::endl;
  }

}

void Amt21Driver::Close() {
  if (fd_port_) {
    close(fd_port_);
    std::cout << "Closed fd_port_ linked to: " << fd_port_ << std::endl;
  }
}

[[maybe_unused]] void Amt21Driver::SetNodeId(uint8_t node_id) {
  node_id_ = node_id;
}

[[maybe_unused]] uint8_t Amt21Driver::GetNodeId() {
  return node_id_;
}

float Amt21Driver::GetEncoderAngle() {
  uint16_t encoder_position = GetEncoderPosition();
  if (!encoder_12bit_) {
    return static_cast<float>(encoder_position) * (360.f) / static_cast<float>((k14BitMaxValue));
  } else {
    return static_cast<float>(encoder_position) * (360.f) / static_cast<float>((k12BitMaxValue));
  }
}
bool Amt21Driver::ChecksumValidation(uint16_t &checksum) {

  std::cout << checksum << std::endl;
  uint8_t k1 = (checksum >> 15); // Is going to be 0
  uint8_t k0 = (checksum >> 14); // Is going to be 1
//    std::cout << "k1: " << +k1 << std::endl;
//    std::cout << "k0: " << +k0 << std::endl;
  uint8_t odd = !(checksum >> 13 & 0b00000001) ^
      (checksum >> 11 & 0b00000001) ^
      (checksum >> 9 & 0b00000001) ^
      (checksum >> 7 & 0b00000001) ^
      (checksum >> 5 & 0b00000001) ^
      (checksum >> 3 & 0b00000001) ^
      (checksum >> 1 & 0b00000001);

  if (k1 != odd) {
    return false;
  }

//  std::cout << "Odd: " << +odd << std::endl;  // Is going to be 0

  uint8_t even = !(checksum >> 12 & 0b00000001) ^
      (checksum >> 10 & 0b00000001) ^
      (checksum >> 8 & 0b00000001) ^
      (checksum >> 6 & 0b00000001) ^
      (checksum >> 4 & 0b00000001) ^
      (checksum >> 2 & 0b00000001) ^
      (checksum & 0b00000001);
//  std::cout << "Even: " << +even << std::endl;  // Is going to be 1

  if (k0 != even) {
    return false;
  }

  return true;
}

