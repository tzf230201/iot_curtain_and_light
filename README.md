# IoT Device for Curtain, Main Lighting, and RGB Ambient Light Control  

This project demonstrates an IoT device that controls a curtain motor, main lights, and an ambient light (which can also function as a sleep lamp). The device is powered by an ESP8266 microcontroller and integrates the Universal Telegram Bot library for remote control. The system offers four different modes of operation for flexible and convenient usage.


## Features

- **ESP8266-Based Control**: Enables wireless communication and control using the ESP8266 microcontroller.  
- **Telegram Bot Integration**: Allows real-time remote control via a dedicated Telegram bot.  
- **Four Operation Modes**:  
  1. **Button Control**: Manually control the curtain motor, room lighting, and RGB ambient light using physical buttons.  
  2. **LDR (Light Dependent Resistor) Control**: Automatically adjusts the lighting system based on ambient light levels.  
  3. **Time-Based Control**: Automates curtain and light operations using a Real-Time Clock (RTC) module.  
  4. **Direct Telegram Control**: Provides instant control through commands sent via Telegram.  



## Hardware Specifications  

![Diagram](picture/diagram.png)  
*Figure 1: Electronics Diagram*  



## Device Images  

- **Board Design**  
![Design](picture/design.png)  
*Figure 2: 3D Board Design*  

- **Real Hardware**  
![Real](picture/real.png)  
*Figure 3: Physical Board*  



## Usage  

![Usage](picture/usage.png)  
*Figure 4: Device in Action*  

1. **Physical Button Control**: Simply press the physical buttons to manage the curtain motor, room lights, and RGB ambient light.  
2. **Automatic Mode**: The system automatically adjusts the main and ambient lighting based on the surrounding light detected by the LDR sensor.  
3. **Time-Based Mode**: Set schedules for automated curtain and light management.  
4. **Telegram Control**: Send specific commands through Telegram to remotely operate the curtain, room lighting, and ambient light.  

All control methods can be configured through the Telegram bot by sending the `/start` command. 
