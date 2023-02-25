# amt21_driver_cpp
amt21_driver_cpp is a UART to RS485 driver for CUI Devices' AMT21 absolute rotary encoders.

## Build and Install ##
**Note: The library will, by default, be installed at `/usr` instead of `/usr/local` in order to be plug and play with ROS2**

    cd amt21_driver_cpp
    mkdir build
    cd build
    cmake ..
    make
    sudo make install

## Usage ##

Simply find and link the package `amt21_driver` in the `CMakelists.txt`

    find_package(amt21_driver REQUIRED)

    target_link_libraries(mylib/myxecutable amt21_driver)


## Giving the UART to RS485 read and write access ##

Do not forget to give the necessary access to the UART port: `sudo chmod /dev/<device>`. This
change will not be persistent through reboot or device connection/reconnection.
To make it persistent, creat an udev-rule. This will require the vendor and product id of the device.
Use `lsusb` in order to determine the `<vendorid>` and `<productid>`. 

    echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="<vendorid>", ATTR{idProduct}=="<productid>", MODE="0666"' | sudo tee /etc/udev/rules.d/91-amt-encoder.rules
Then, in order to reload udev-rules run the following commands, or simply reboot. 
    
    sudo udevadm control --reload-rules && sudo udevadm trigger

    
### Example udev-rule ###

In the example a USB to RS485 adapter using the FT232 Serial IC is used.

In a terminal window, `lsusb` returns the following.
    
    Bus 004 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
    Bus 003 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
    Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
    Bus 001 Device 008: ID 0bda:c811 Realtek Semiconductor Corp. 802.11ac NIC
    Bus 001 Device 007: ID 05e3:0743 Genesys Logic, Inc. SDXC and microSDXC CardReader
    Bus 001 Device 005: ID 05e3:0610 Genesys Logic, Inc. Hub
    Bus 001 Device 004: ID 046d:c52b Logitech, Inc. Unifying Receiver
    Bus 001 Device 003: ID 31b2:0011 DCMT Technology USB Condenser Microphone
    Bus 001 Device 002: ID 1b1c:1b3c Corsair Corsair Gaming HARPOON RGB Mouse
    Bus 001 Device 006: ID 0b05:1939 ASUSTek Computer, Inc. AURA LED Controller
    Bus 001 Device 009: ID 0403:6001 Future Technology Devices International, Ltd FT232 Serial (UART) IC
    Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub

From the above information it can be determined that `<vendorid> = 0403` and `<productid> = 6001`.
Thus, the following udev-rule is applied:

    echo 'SUBSYSTEM=="tty", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001" MODE="0666"' | sudo tee /etc/udev/rules.d/99-usb-serial.rules

If you share your linux system with other users, or just don't like the
idea of write permission for everybody, you can replace `MODE:="0666"` with
`OWNER:="<yourusername>"` to create the device owned by you, or with
`GROUP:="<somegroupname>"` and mange access using standard unix groups.
