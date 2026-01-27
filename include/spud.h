/** @file spud.h
    @brief Spud definitions.

  Definitions for pins on Spud v1.0 

  - **AI**: Analog input
  - **SW**: Switches
  - **DO** or **DIO**: Digital input/output
  - **SDI**: Serial Data In (SPI bus for CAN bus)
  - **SDO**: Serial Data Out (SPI bus for CAN bus)
  - **CS**: Chip select
  - **INT**: Interrupt

*/

#ifndef SPUD_H
#define SPUD_H

#if SPUD_VERSION > 1

#else

#define AI0 GPIO_NUM_33 
#define AI1 GPIO_NUM_32 
#define AI2 GPIO_NUM_35 
#define AI3 GPIO_NUM_34 
#define AI4 GPIO_NUM_39 
#define AI5 GPIO_NUM_36

#define SW0 GPIO_NUM_23 
#define SW1 GPIO_NUM_22 
#define SW2 GPIO_NUM_21 
#define SW3 GPIO_NUM_27 
#define SW4 GPIO_NUM_26 
#define SW5 GPIO_NUM_25 

#define DO0 GPIO_NUM_4
#define DO1 GPIO_NUM_16 
#define DO2 GPIO_NUM_18 
#define DO3 GPIO_NUM_19 

#define DIO0 DO0
#define DIO1 DO1
#define DIO2 DO2
#define DIO3 DO3

#define SDO GPIO_NUM_13
#define SDI GPIO_NUM_12
#define SCLK  GPIO_NUM_14

#define CS0  GPIO_NUM_15
#define INT0  GPIO_NUM_17

#define CS1  GPIO_NUM_2
#define INT1  GPIO_NUM_5

#define CAN0_CS  GPIO_NUM_15
#define CAN0_INT  GPIO_NUM_17

#define CAN1_CS  GPIO_NUM_2
#define CAN1_INT  GPIO_NUM_5

#endif /* SPUD_VERSION == 1.0 */

#endif /* SPUD_H include guard */
