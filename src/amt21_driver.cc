//
// Created by ICraveSleep on 05-Feb-23.
//

#include "amt21_driver.h"

Amt21Driver::Amt21Driver(const std::string &port,
                         AMT21Resolution encoder_12bit,
                         AMT21BaudRate baud_rate,
                         AMT21TurnType turn_type)
    : port_(port), baud_rate_(baud_rate) {

  port_open_ = false;

  if (encoder_12bit
      == AMT21Resolution::k12Bit) {  // https://stackoverflow.com/questions/24297992/is-it-possible-to-make-a-scoped-enumeration-enum-class-contextually-converti  --- value of type enum is not contexaullty convertible to bool  TODO find better solution
    encoder_12bit_ = true;
  } else {
    encoder_12bit_ = false;
  }

  if (turn_type == AMT21TurnType::kSingleTurn) {
    encoder_single_turn_ = true;
  } else {
    encoder_single_turn_ = false;
  }

  node_id_ = 0x54; // AMT21 default node id
  read_position_request = node_id_;
  extended_command = node_id_ + 0x02;
  read_turns_request = node_id_ + 0x01;
}

Amt21Driver::~Amt21Driver() {
  if (port_open_) {
    Close();
  }
}

uint16_t Amt21Driver::GetEncoderPosition() {
  checksum_failed_ = false;

  uint8_t request_package[] = {read_position_request};
  int32_t reqeust_package_size = sizeof(request_package) / sizeof(request_package[0]);
  int64_t bytes_written = write(fd_port_, request_package, reqeust_package_size);

  uint8_t receive_buffer[2];
  int64_t bytes_read;
  usleep(kMinimumDebounceTime);
  bool do_read = true;
  uint16_t counter = 0;

  while (do_read && counter < 10000) {
    bytes_read = read(fd_port_, receive_buffer, sizeof(receive_buffer));
    if (bytes_read >= 1) {
      do_read = false;
    }
    usleep(kMinimumDebounceTime);
    counter++;
  }

  if (bytes_read <= 0) {
    std::cout << "Error reading bytes" << std::endl;
  }

  uint16_t response = ((receive_buffer[1] << 8) | (receive_buffer[0]));

  if (!ChecksumValidation(response)) {
    checksum_failed_ = true;
  }

  response = (response & kCheckBitMask);

  if (encoder_12bit_) {
    response = (response >> 2);
  }

  return response;
}

bool Amt21Driver::Open() {
  fd_port_ = open(port_.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
  std::cout << "Opened fd_port_: " << fd_port_ << std::endl;
  if (fd_port_ == -1) {
    std::cout << "Error opening the serial port" << std::endl;
    return false;
  }

  // Configure port if opened
  port_open_ = true;
  struct termios options{};
  options.c_cflag = static_cast<int>(baud_rate_) | CS8 | CLOCAL | CREAD;
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(fd_port_, TCIFLUSH);
  tcsetattr(fd_port_, TCSANOW, &options);
  return true;
}

void Amt21Driver::Close() {
  if (fd_port_) {
    close(fd_port_);
    std::cout << "Closed fd_port: " << fd_port_ << std::endl;
  }
  port_open_ = false;
}

[[maybe_unused]] void Amt21Driver::SetNodeId(uint8_t node_id) {
  node_id_ = node_id;

  // Updated request value after changing node ID
  extended_command = node_id_ + 0x02;
  read_position_request = node_id_ + 0x01;
}

[[maybe_unused]] uint8_t Amt21Driver::GetNodeId() const {
  return node_id_;
}

float Amt21Driver::GetEncoderAngle() {
  uint16_t encoder_position = GetEncoderPosition();
  if (!encoder_12bit_) {
    return static_cast<float>(encoder_position) * (2.f * kPi) / static_cast<float>((k14BitMaxValue));
  } else {
    return static_cast<float>(encoder_position) * (2.f * kPi) / static_cast<float>((k12BitMaxValue));
  }
}

float Amt21Driver::GetEncoderAngleDeg() {
  uint16_t encoder_position = GetEncoderPosition();
  if (!encoder_12bit_) {
    return static_cast<float>(encoder_position) * (360.f) / static_cast<float>((k14BitMaxValue));
  } else {
    return static_cast<float>(encoder_position) * (360.f) / static_cast<float>((k12BitMaxValue));
  }
}

bool Amt21Driver::SetZeroPosition() {
  if (!encoder_single_turn_) {
    std::cout << "Not possible to set zero position when encoder turn type is set to multi-turn" << std::endl;
    return false;
  }
  uint8_t request_package[] = {extended_command, set_zero_position_request};
  int32_t reqeust_package_size = sizeof(request_package) / sizeof(request_package[0]);
  int64_t bytes_written = write(fd_port_, request_package, reqeust_package_size);
  usleep(10000000);
  return true;
}

bool Amt21Driver::ResetEncoder() {
  uint8_t request_package[] = {extended_command,};
  int32_t reqeust_package_size = sizeof(request_package) / sizeof(request_package[0]);
  int64_t bytes_written = write(fd_port_, request_package, reqeust_package_size);
  usleep(10000000);
  return true;
}

int32_t Amt21Driver::GetTurns() {  //TODO has to be tested
  if (encoder_single_turn_) {
    std::cout << "Not possible to read turns when encoder turn type is set to single-turn" << std::endl;
    return 0;
  }
  uint8_t request_package[] = {read_turns_request};
  int32_t reqeust_package_size = sizeof(request_package) / sizeof(request_package[0]);
  int64_t bytes_written = write(fd_port_, request_package, reqeust_package_size);

  //TODO this is a duplicate of ReadEncoderPosition(). figure out how to remove duplication
  uint8_t receive_buffer[2];
  int64_t bytes_read;
  usleep(kMinimumDebounceTime);
  bool do_read = true;
  uint16_t counter = 0;

  while (do_read && counter < 10000) {
    bytes_read = read(fd_port_, receive_buffer, sizeof(receive_buffer));
    if (bytes_read >= 1) {
      do_read = false;
    }
    usleep(kMinimumDebounceTime);
    counter++;
  }

  if (bytes_read <= 0) {
    std::cout << "Error reading bytes" << std::endl;
  }

  uint16_t response = ((receive_buffer[1] << 8) | (receive_buffer[0]));

  if (!ChecksumValidation(response)) {
    checksum_failed_ = true;
  }

  response = (response & kCheckBitMask);

  if (encoder_12bit_) {
    response = (response >> 2);
  }

  return response;
}

bool Amt21Driver::ChecksumValidation(uint16_t &checksum) {
  uint8_t k1 = (checksum >> 15);
  uint8_t k0 = (checksum >> 14 & 0b00000001);

  uint8_t odd = !(((checksum >> 13) & 0b00000001) ^
      ((checksum >> 11) & 0b00000001) ^
      ((checksum >> 9) & 0b00000001) ^
      ((checksum >> 7) & 0b00000001) ^
      ((checksum >> 5) & 0b00000001) ^
      ((checksum >> 3) & 0b00000001) ^
      ((checksum >> 1) & 0b00000001));

  if (k1 != odd) {
    return false;
  }

  uint8_t even = !(((checksum >> 12) & 0b00000001) ^
      ((checksum >> 10) & 0b00000001) ^
      ((checksum >> 8) & 0b00000001) ^
      ((checksum >> 6) & 0b00000001) ^
      ((checksum >> 4) & 0b00000001) ^
      ((checksum >> 2) & 0b00000001) ^
      ((checksum) & 0b00000001));

  if (k0 != even) {
    return false;
  }
  return true;
}

bool Amt21Driver::ChecksumFailed() const {
  return checksum_failed_;
}
