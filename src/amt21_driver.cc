//
// Created by Sondre on 05-Feb-23.
//

#include "amt21_driver.h"

Amt21Driver::Amt21Driver() {
  fd_port = 0;
  node_id = 'T'; // AMT21 default node id
  struct termios options{};
  options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(fd_port, TCIFLUSH);
  tcsetattr(fd_port, TCSANOW, &options);
}

uint16_t Amt21Driver::GetEncoderPositionRaw() {
  char package[] = {'T'};
  return 0;
}
void Amt21Driver::Open() {
  fd_port = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  std::cout << "Opened fd_port linked to: " << fd_port << std::endl;
  if(fd_port == -1){
    std::cout << "Error opening the serial port" << std::endl;
//    return -1;
  }

}
void Amt21Driver::Close() {
  if(fd_port){
    close(fd_port);
    std::cout << "Closed fd_port linked to: " << fd_port << std::endl;
  }
}
void Amt21Driver::SetNodeId() {

}
uint8_t Amt21Driver::GetNodeId() {
  return 0;
}

