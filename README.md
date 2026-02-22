# Gudu VCU
Documentation repository: [GuduVCU-docs](https://github.com/DexterCarpenter/GuduVCU-docs)

This repository hosts the control software for the 'Gudu' EV conversion project.


# INIT PROCEDURE
-- 1. signal in from Ignition switch A2
-- 2. on in from A2 to power out for SW0 (ign switch)

https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/gpio.html


#### Analog Pins

| Name | GPIO # | Function            | 24-Pin ID |
| ---- | ------ | ------------------- | --------- |
| A0   | 33     | Throttle Position 1 | 22        |
| A1   | 32     | Throttle Position 2 | 23        |
| A2   | 35     | Ignition Switch     | 24        |
| A3   | 34     |                     | 12        |
| A4   | 39     | Brake Switch        | 11        |
| A5   | 36     |                     | 10        |
#### Switch Pins

| Name | GPIO # | Function                 | 24-Pin ID |
| ---- | ------ | ------------------------ | --------- |
| SW0  | 23     | Ignition Signal          | 13        |
| SW1  | 22     | Brake Signal             | 14        |
| SW2  | 21     | F/S Charge Relay         | 15        |
| SW3  | 27     | Negative Battery Relay   | 16        |
| SW4  | 26     | Battery Pre-Charge Relay | 17        |
| SW5  | 25     | Positive Battery Relay   | 18        |
#### Digital Pins

| Name | GPIO # | Function                 | 24-Pin ID |
| ---- | ------ | ------------------------ | --------- |
| DIO0 |        |                          | 6         |
| DIO1 |        | Water Pump Speed Control | 5         |
| DIO2 |        |                          | 4         |
| DIO3 |        |                          | 3         |
#### CAN Bus

| Name     | GPIO # | Function          | 24-Pin ID |
| -------- | ------ | ----------------- | --------- |
| CAN0_CS  | 2      | Car CAN Bus       |           |
| CAN0_INT | 5      | Car CAN Bus       |           |
| CAN1_CS  | 15     | Accessory CAN Bus |           |
| CAN1_INT | 17     | Accessory CAN Bus |           |



#### Contributors:
- Colin Hale-Brown
- Dexter Carpenter
- Milan Donhowe