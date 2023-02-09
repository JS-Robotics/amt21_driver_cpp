//
// Created by ICraveSleep on 09.02.23.
//

#include <iostream>

enum class EncoderErrorCodes{
  kNoError = 0x00,
  kEvenChecksumFailed = 0x01,
  kOddChecksumFailed = 0x02,
  kErrorReadingBytes = 0x03,
  kGetTurnNotAvailable = 0x04,
  kSetZeroNotAvailable = 0x05,
  kErrorOpeningSerialPort = 0x06

};

uint8_t GetErrorCode(EncoderErrorCodes error_code){
  return static_cast<uint8_t>(error_code);
}

int main(){
  EncoderErrorCodes error_code = EncoderErrorCodes::kNoError;

  uint8_t error = GetErrorCode(error_code);

  if(error){
    std::cout << "Not suppose to print this" << std::endl;
  } else {
    std::cout << "No error: 0x00, is handled as false" << std::endl;
  }

  error_code = EncoderErrorCodes::kErrorReadingBytes;
  error = GetErrorCode(error_code);

  if(error){
    std::cout << "Error is true, and should print" << std::endl;
  }

}