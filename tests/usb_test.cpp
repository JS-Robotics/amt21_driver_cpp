#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

int main() {
//  char t = 1;
//  char a = 1;
//  int val = ((t << 8) | (a)) >> 2; // 0000 0001 0000 0001
//  std::cout << val << std::endl;

  int fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  std::cout << fd << std::endl;
  if (fd == -1) {
    std::cout << "Error opening the serial port" << std::endl;
    close(fd);
    return -1;
  }

  struct termios options{};
  //  options.c_cflag = 0010002 | CS8 | CLOCAL | CREAD;
//  options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
  options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &options);
  int runner = 0;
  while (runner < 1) {
    char package[] = {'T'};
    int32_t package_size = sizeof(package) / sizeof(package[0]);
    int64_t bytes_written = write(fd, package, package_size);
//    std::cout << "Bytes written: " << bytes_written << std::endl;

    uint8_t buffer[256];
    int64_t bytes_read;
    bool do_read = true;
    int counter = 0;
    while (do_read) {
      usleep(30);
      bytes_read = read(fd, buffer, sizeof(buffer));
      if (bytes_read >= 1 || counter >= 50000) {
        std::cout << "Bytes read: " << bytes_read << std::endl;
        int value = ((buffer[1] << 8) | (buffer[0]));
        value = (value & 0b0011111111111111);
//        value = (value >> 2);
//        std::cout << +buffer[0] << ":" << +buffer[1] << std::endl;
//        std::cout << value << std::endl;
        std::cout << "value: " << value << ", runner: " << runner << ", counter: " << counter << std::endl;
        do_read = false;
      }
      counter++;
//      usleep(10000);
    }
    runner++;
  }
  close(fd);
  return 0;
}
