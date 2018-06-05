# *IPot*

## Overview
The project of a simple watering system with humidity measurement based on STM32F4-DISCOVERY board.  [STMIPot] (https://github.com/PUT-PTM/2018_plant_watering). [The movies] (https://drive.google.com/file/d/1kGmeSqZCRVhO3qLcpNGLJXfvGC6VG8cg/view?usp=sharing) show the project of a simple watering system with humidity measurement.

## Description
The project consists of:
- STM32F4-Discovery board,
- moisture sensor,
- Electric Liquid Pump 12V,
- Relay Module 5V,
- Switched-mode power supply 12V.

Moisture level when pump is set on can be set via IDE, after compiling
 
## Tools
System Workbench for STM32 IDE

## How to run
1. Connect STM32F4-DISCOVERY board with the Relay: 
  *  VDD  -  5V
  *  GND  -  GND
  *  IN   -  PD12
2. Connect STM32F4-DISCOVERY board with the LED screen: 
 *  VDD  -  3V
 *  a  -  PE0
 *  b  -  PE1
 *  c  -  PE2
 *  d  -  PE3
 *  e  -  PE4
 *  f  -  PE5
 *  g  -  PE6
 *  h  -  PE7
 *  3  -  PB14
 *  2  -  PB13
 *  1  -  PB12
3. Connect STM32F4-DISCOVERY board with moisture sensor by ADC(Analog Digital Converter:
 *  VDD  -  3V
 *  GND  -  GND
 *  A0   -  PA1
4. Connect the Relay with Switched-mode power supply:

 
 
## How to compile
Download the project and compile it with System Workbench for STM32 IDE.

## Future improvements
Posibility of extending for many plants

## Attributions
* http://www.st.com/en/embedded-software/stm32-standard-peripheral-libraries.html?querycriteria=productId=LN1939


## License
MIT

## Credits
* Cezary Czekalski

The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology.

Supervisor: dr. Marek Kraft
