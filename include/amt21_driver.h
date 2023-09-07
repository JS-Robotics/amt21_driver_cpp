//
// Created by ICraveSleep on 05-Feb-23.
//

#ifndef AMT21_DRIVER_CPP_AMT21_DRIVER_H
#define AMT21_DRIVER_CPP_AMT21_DRIVER_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>


/*! \brief Selection list of supported encoder baud rates
 *
 * The encoder default to a baud rate of 115200bps.
 * In order to apply other baud rates, the device has to be configured
 *
 * \k9600 9600 baud rate
 * \k19200 12900 baud rate
 * \k38400 3840 baud rate
 * \k115200 115200 baud rate
 * \k2000000 2000000 baud rate
 *
 */
enum class AMT21BaudRate {
  k9600 = B9600,  // NOT TESTED
  k19200 = B19200,  // NOT TESTED
  k38400 = B38400,  // NOT TESTED
  k115200 = B115200,
  k2000000 = B2000000
};


/*! \brief Selection list of encoder resolution
 *
 * \k12bit 12 bit resolution encoder
 * \k14Bit 14 bit resolution encoder
 */
enum class AMT21Resolution {
  k12Bit = 12,
  k14Bit = 14
};

/*! \brief Selection list of encoder turn types
 * \kSingleTurn For AMT21 encoders that are single-turn
 * \kMultiTurn For AMT21 encoders that are multi-turn
 */
enum class AMT21TurnType {
  kSingleTurn = 5210,
  kMultiTurn = 5211
};


/*! \brief Error code list
 * \kNoError = 0x00, No error
 * \kEvenChecksumFailed 0x01,  The Even parity bit checksum evaluation failed
 * \kOddChecksumFailed 0x02, The Odd parity bit checksum evaluation failed
 * \kErrorReadingBytes 0x03,  Error reading bytes from serial
 * \kGetTurnNotAvailable 0x04, Not possible to get number of turns when encoder turn type is set to single-turn
 * \kSetZeroNotAvailable 0x05, Not possible to set zero position when encoder turn type is set to multi-turn
 * \kErrorOpeningSerialPort 0x06, Error Opening the serial port
 */
enum class EncoderErrorCodes {
  kNoError = 0x00, /** No error **/
  kEvenChecksumFailed = 0x01,  /** The Even parity bit checksum evaluation failed **/
  kOddChecksumFailed = 0x02,  /** The Odd parity bit checksum evaluation failed **/
  kErrorReadingBytes = 0x03,  /** Error reading bytes from serial **/
  kGetTurnNotAvailable = 0x04,  /** Not possible to get number of turns when encoder turn type is set to single-turn **/
  kSetZeroNotAvailable = 0x05,  /** Not possible to set zero position when encoder turn type is set to multi-turn **/
  kErrorOpeningSerialPort = 0x06  /** Error Opening the serial port **/

};

class Amt21Driver {
 public:
  Amt21Driver(const std::string &port, AMT21Resolution encoder_12bit, AMT21BaudRate baud_rate, AMT21TurnType turn_type);

  ~Amt21Driver();

  /*! \brief Opens the constructor specified USB port */
  bool Open();

  /*! \brief Closes the open USB port */
  void Close();

  /*! \breif Get the encoder capacitive disc position
   *
   * This function returns the current position of the capacitive disc.
   * The value is from 0-16382 if the encoder has 14bit resolution.
   * The value is from 0-4095 if the encoder has 12bit resolution.
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

  /*! \brief Get the number of encoder turns
   *
   * Multi-turn encodes only
   *
   * @return (int32_t) Number of encoder turns
   */
  int32_t GetTurns();

  /*! \brief Set the current encoder position as zero position. I.e. make current position 0 value
   *
   * @return (bool) Returns true if the encoder position was successfully zeroed. Else returns true
   */
  bool SetZeroPosition();

  /*! \brief Resets the encoder.
   *
   * @return True if encoder reset was successful.
   */
  bool ResetEncoder();

  /*! \brief Validate if the checksum failed on read position request
   *
   * If the checksum fails it means that the read position response data from the encoder is not valid.
   * In order to check if the current read response after getting the response data is valid.
   * This function serves this purpose.
   *
   * @return True if the checksum failed. False if the read position response data was received without errors
   */
  [[nodiscard]] bool ChecksumFailed() const;

  /*! \brief Get the current encoder error
   *
   * See enum class EncoderErrors for error code information.
   *
   * @return (uint8_t) Current encoder error
   */
  [[nodiscard]] uint8_t GetEncoderError() const;

  /*! \brief Check if the port is currently open or closed
   *
   * @return (bool) True if the UART port is open, False if it is closed
   */
  [[nodiscard]] bool PortOpen() const;

  /*! \breif Set the node ID to be utilized
   *
   * Utilize this function in order to change from the default node id 0x54 to another.
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
  [[maybe_unused]] [[nodiscard]] uint8_t GetNodeId() const;

 private:

  /*! \brief Perform checksum validation of the returned read response
  *
  * @param checksum The return response from the encoder to be checksum validated
  * @return False if checksum validation fails, and True if it passes
  */
  bool ChecksumValidation(uint16_t &checksum);

  static constexpr float kPi = 3.14159265358979323846;
  static constexpr uint16_t kCheckBitMask = 0b0011111111111111;
  static constexpr uint16_t k14BitMaxValue = 16383; // 0-16383 (16384)
  static constexpr uint16_t k12BitMaxValue = 4095; // 0-4095 (4096)
  static constexpr uint8_t kMinimumDebounceTime = 50; // μs
  static constexpr uint32_t kResetSleepTime = 10000000; // μs (10s)

  bool checksum_failed_;
  uint8_t node_id_;
  int fd_port_;
  bool encoder_12bit_;
  bool encoder_single_turn_;
  AMT21BaudRate baud_rate_;
  std::string uart_port_;
  bool port_open_;
  EncoderErrorCodes encoder_error_;

  uint8_t read_position_request;
  uint8_t extended_command;
  uint8_t read_turns_request; // Multi-turn encoders only
  static constexpr uint8_t set_zero_position_request = 0x5E;  // Single turn encoders only
  static constexpr uint8_t reset_encoder_request = 0x75;
};

#endif //AMT21_DRIVER_CPP_AMT21_DRIVER_H
