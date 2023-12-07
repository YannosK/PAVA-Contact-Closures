# PAVA-Contact-Closures

Arduino Nano works in conjunction with a Public-Address Voice-Alarm system in order to output different evacuation messages in different speaker zones, in consecutive time intervals.

The PAVA system has control inputs with dry contact closures. We are using the arduino digital pins to control high trigger 5V relays, that will sequentially drive the control inputs of the PAVA system.

## Arduino Nano Connections & Working Principle

The arduino will be driven by contact closures of a fire-detection SCADA system. The digital inputs of the arduino are defined in the source code as IN1 and IN2. Pins D2 and D3 are specifically chosen because they support external interrupts.

The arduino will drive three high-trigger normally-open 5V relays via digital output pins defined in the source code as ROUT1, ROUT2, ROUT3. In the case that IN1 is driven the relays will close one after the other with idle intervals of 1 second in between. While a releay is closed it drives the control inputs of the PAVA system, that outputs an evacuation message in the corresponding speaker zone. Obviously this example assumes three speaker zones, driven by the three relays, and three different messages to be played on each zone, with fixed time durations.

Whenever IN1 drives the arduino the outputs will drive the relays in the following order:

_ROUT1 -> wait 1sec -> ROUT 2 -> wait 1 sec -> ROUT3 ->wait 1 sec -> start again_

Whenever IN2 drives the arduino the outputs will drive the relays in the following order:

_ROUT3 -> wait 1sec -> ROUT 2 -> wait 1 sec -> ROUT1 ->wait 1 sec -> start again_

## Duration of Messages

The duration of the messages will must be inputed by the user, that has also loaded the messages on the PAVA system. The user needs to connect via a Serial Monitor (eg. PuTTY) with the Arduino, via its USB port. There they will press any button to enter the menu providing the instructions on how to enter the duration of the messages.

The function that handles the User Interface is SerialCom() in the source code.

The duration of the messages will be safed inside the EEPROM memory, in order to survive in the case that the Arduino unexpectadly reboots. The function EEPROMwrite() handles the writing of the durations in EEPROM memory, in multiple locations, for redundancy purposes. EEPROMread() reads the values using a voting system from the various memory locations.
