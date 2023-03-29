
# Sensor Cache

The main objective of this software is for testing sensors or for low latency data collection. By using USART commands the user will be able to retreive the sensor data form the device.

## SetUp
- Search for the COM port.
- Initialize cool terminal. 
- Set the COM port and change Baudrate to 38400.

## Commands

- \<AN0> | Get Analog 0 (Pot1) value. 
- \<AN1> | Get Analog 1 (Pot2) value.
- \<PAT> | Get GPIO Pattern (Button&Leds) value.
- \<ACC> | Get Accelerometer values.

### AN0

### AN1

### PAT

### ACC

## Errors

- Invalid CMD | Not a valid command. Followed by the user input

- Size Exceed!| Size of incomming message exceed the max size. Follow the user input.



# Development

**Hardware:** STM32F407VGX

**Baord:** STM32F4 Discovery Board

**Software:** Keil uVision, Cool Terminal

**Firmware:** STM32F4 peripherals and RTOS 

# More

Licence annd authors information

## License
[MIT](https://choosealicense.com/licenses/mit/)

## Authors
- [@Anon](Broke)

