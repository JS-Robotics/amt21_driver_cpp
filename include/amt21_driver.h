//
// Created by Sondre on 05-Feb-23.
//

#ifndef AMT21_DRIVER_CPP_AMT21_DRIVER_H
#define AMT21_DRIVER_CPP_AMT21_DRIVER_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

class Amt21Driver {
 public:
  Amt21Driver();
  ~Amt21Driver() = default;
  void Open();
  void Close();
  uint16_t GetEncoderPositionRaw();
  void SetNodeId();
  uint8_t GetNodeId();

 private:
  static constexpr uint16_t kCheckBitMask = 0b0011111111111111;
  uint8_t node_id;
  int fd_port;
};

#endif //AMT21_DRIVER_CPP_AMT21_DRIVER_H
