
# Sensor Cache

The main objective of this software is for testing sensors or for low latency data collection. By using USART commands the user will be able to retreive the sensor data form the device.

## SetUp
- Search for the COM port.
- Initialize cool terminal. 
- Set the COM port and change Baudrate to 38400.

## Commands
A command should be send inside '\<CMD>' followed by '\r' and '\n' This can be achieved by using typing the comand and hit ENTER or by sending the command by script. 
- \<AN0> | Get Analog 0 (Pot1) value. 
- \<AN1> | Get Analog 1 (Pot2) value.
- \<PAT> | Get GPIO Pattern (Button&Leds) value.
- \<ACC> | Get Accelerometer values.

### AN0
Analogue value for potentiometer 1. \
Analogue values int volts. \
The values are updated every 1000 ms.\
Responce Example:\
```<AN10>-Value: 1.83 V```

### AN1
Analogue value for potentiometer 2.\
Analogue values int volts.\
The values are updated every 1000 ms\
Responce Example:\
```<AN1>-Value: 1.99 V```

### PAT
Digital Hexadecimal Value for the gpio patterns (Default LED1 On).\
Button 2 to 8 save a hex value.\
LEDs 2 to 8 alternate the hex value making them on and off.\
Button 1 is an interrupt to reset the value to default or 0x01.\
LED1 stay on if the default value is set.\
 Responce Example:\
```<PAT>-Hex: 1```

### ACC
G values for the accelerometer.\
Values are presentes as X Y and Z, in this particular order.\
Only work for the LIS3DH sensor (STM32F4Discovery board Revision C01). Otherwise a no value message will be set (see errors).\
Responce Example:\
```<ACC>-XYZ: -0.00 -0.02 1.10```

## Errors

- ```<ERR|CMD>-Invalid:%s``` | Not a valid command. Followed by the user input

- ```<ERR|CMD>-Size Exceed:%s``` | Size of incomming message exceed the max size. Follow the user input.

- ```<ERR|ACC>-No Value``` | No sensor avaliable, therefore, no value received. Check Board revision.

# Development

**Hardware:** STM32F407VGX

**Baord:** STM32F4 Discovery Board Revision C01

**Software:** Keil uVision, Cool Terminal

**Firmware:** STM32F4 peripherals and RTOS 

# More

Licence annd authors information

## License
[MIT](https://choosealicense.com/licenses/mit/)

## Authors
- [@Anon](Broke)

