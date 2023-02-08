//
// Created by ICraveSleep on 05-Feb-23.
//

#ifndef AMT21_DRIVER_CPP_AMT21_DRIVER_H
#define AMT21_DRIVER_CPP_AMT21_DRIVER_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

enum class AMT21BaudRate{
  k115200 = 12,
  k38400 = 123123,
  k19200 = 3214,
  k9600 = 12315
};

enum class AMT21Resolution{
  k12Bit = 12,
  k14Bit = 14
};

class Amt21Driver {
 public:
  Amt21Driver(const std::string &port, AMT21Resolution encoder_12bit, AMT21BaudRate baud_rate);

  ~Amt21Driver() = default;

  /*! \brief Opens the constructor specified USB port */
  void Open();

  /*! \brief Closes the open USB port */
  void Close();

  /*! \breif Get the encoder capacitive disc position
   *
   * This function returns the current position of the capacitive disc.
   * The value is from 0-16382 if the encoder has 14bit resolution.
   * The value is from 0-4095 if the encoder has 12bit resolution
   * @return uint16_t (0 - encoder resolution)
   */
  uint16_t GetEncoderPosition();

  /*! \breif Get the encoder shaft angle in radians
   *
   * @return float (Encoder angle in rad)
   */
  float GetEncoderAngle();

  /*! \breif Get the encoder shaft angle in degrees
   *
   * @return float (Encoder angle in deg)
   */
  float GetEncoderAngleDeg();

  /*! \brief Validate if the checksum failed on read position request
   *
   * If the checksum fails it means that the read position response data from the encoder is not valid.
   * In order to check if the current read response after getting the response data is valid.
   * This function serves this purpose.
   *
   * @return True if the checksum failed. False if the read position response data was received without errors.
   */
  bool ChecksumFailed();

  /*! \breif Set the node ID to be utilized
   *
   * Utilize this function in order to change from the default node id 0x54 to another
   *
   * Available node addresses are:
   * 00 20 40 60 80 A0 C0 E0
   * 04 24 44 64 84 A4 C4 E4
   * 08 28 48 68 88 A8 C8 E8
   * 0C 2C 4C 6C 8C AC CC EC
   * 10 30 50 70 90 B0 D0 F0
   * 14 34 54(default) 74 94 B4 D4 F4
   * 18 38 58 78 98 B8 D8 F8
   * 1C 3C 5C 7C 9C BC DC FC
   *
   * @param node_id - The node ID on the RS485 bus to request different responses from
   */
  [[maybe_unused]] void SetNodeId(uint8_t node_id);

  /*! \brief Get the current node ID
   *
   * @return uint8_t (node ID)
   */
  [[maybe_unused]] uint8_t GetNodeId();

 private:

  /*! \brief Perform checksum validation of the returned read response.
  *
  * @param checksum
  * @return False if checksum validation fails, and True if it passes.
  */
  bool ChecksumValidation(uint16_t &checksum);

  static constexpr float kPi = 3.14159265358979323846;
  static constexpr uint16_t kCheckBitMask = 0b0011111111111111;
  static constexpr uint16_t k14BitMaxValue = 16382; // 0-16383 (16382)
  static constexpr uint16_t k12BitMaxValue = 4095; // 0-4095 (4096)
  static constexpr uint8_t kMinimumDebounceTime = 50; // μs

  bool checksum_failed_;
  uint8_t node_id_;
  int fd_port_;
  bool encoder_12bit_;
  AMT21BaudRate baud_rate_;
  std::string port_;
};


#endif //AMT21_DRIVER_CPP_AMT21_DRIVER_H
