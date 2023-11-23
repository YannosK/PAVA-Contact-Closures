# PAVA-Contact-Closures

Arduino Nano works in conjunction with a Public-Address Voice-Alarm system in order to output different evacuation messages in different speaker zones, in consecutive time intervals.

The PAVA system has control inputs with dry contact closures. We are using the arduino digital pins to control high trigger 5V relays, that will sequentially drive the control inputs of the PAVA system.

## Arduino Nano

The arduino will be driven by contact closures of a fire-detection SCADA system. The digital inputs of the arduino are defined in the source code as IN1 and IN2. Pins D2 and D3 are specifically chosen because they support external interrupts.
