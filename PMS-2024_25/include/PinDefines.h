/**
 * @brief PIN defines for the STM32H7
 * @version 0.1.0
 */




 // fix this part
 #include "stm32l452xx.h"
 #include "stm32l4xx_hal.h"
 #include <Arduino.h>
 #pragma once
 

 /******************** DEBUGGING ********************/
 
 /* Serial print header using UART9 */
 #define DEBUG_MCU_TX_PIN                 PC4
 #define DEBUG_MCU_RX_PIN                 PC5
 // if you're looking for DEBUG_BAUD_RATE, its found in platformio.ini
 

 /******************** GPIO ********************/

 #define Buck_Converter_Shutdown        PA9
 #define Battery_Monitor_Enable         PA10

 #define Temp_Chip_Select               PC13
 
 /******************** SENSORS ********************/

             /**** ADC Sensors ****/

/* Current Sensors */
#define Current_Sense_Vref              PA0
#define Current_Sense_Vout              PA1
#define RPI_Current_Sense_Vref          PA2
#define RPI_Current_Sense_Vout          PA3


             /**** I2C Bus ****/

/* Battery Monitor */
#define SDA                             PB7
#define SCL                             PB8
// I would recommend including the addresses you will be using here


             /**** SPI Sensor ****/
 
/* Temperature Sensor */
#define Temp_SPI_SCLK                   PC_10
#define Temp_SPI_MISO                   PC_11
#define Temp_SPI_MOSI                   PC_12
// MISO is Master In Slave Out, its how the sensor sends data

             /**** CAN Transceiver ****/

#define CAN_TX                          PB12
#define CAN_RX                          PB13

 

 