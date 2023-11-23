# PAVA-Contact-Closures

Arduino Nano works in conjunction with a Public-Address Voice-Alarm system in order to output different evacuation messages in different speaker zones, in consecutive time intervals.

The PAVA system has control inputs with dry contact closures. We are using the arduino digital pins to control high trigger 5V relays, that will sequentially drive the control inputs of the PAVA system.

## Arduino Nano

The arduino will be driven by contact closures of a fire-detection SCADA system. The digital inputs of the arduino are defined in the source code as IN1 and IN2. Pins D2 and D3 are specifically chosen because they support external interrupts.

The arduino will drive three high-trigger normally-open 5V relays via digital output pins defined in the source code as ROUT1, ROUT2, ROUT3. In the case that IN1 is driven the relays will close one after the other with idle intervals of 1 second in between. While a releay is closed it drives the control inputs of the PAVA system, that outputs an evacuation message in the corresponding speaker zone. Obviously this example assumes three speaker zones, driven by the three relays, and three different messages to be played on each zone, with fixed time durations.
