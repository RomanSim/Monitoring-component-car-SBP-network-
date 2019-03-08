


/**

each message consists of:

uint32 id_message
uint32 timestmap
uint32 ECU_id
uint32 value_1
uint32 value_2

Timing anomaly:
 * each 50 ms i should get only one message regarding the SPEEDOMETER
 * each 5 ms i should get only one message regarding the PEDALS
 * each 10 ms i should get only one message regarding the ABS
 * each 100 ms i should get only one message regarding the TIRE_PRESSURE


 example:

-> 2 0 0x100 0 0
   3 0 0x400 0 0
   4 0 0x800 0 100
   5 5 0x200 0 0
   6 10 0x200 0 0
   7 10 0x400 0 0
   8 15 0x200 0 0
   9 20 0x200 1 0
   10 20 0x400 0 0
   11 25 0x200 2 0
   12 30 0x200 3 0
   13 30 0x400 0 0
   14 35 0x200 4 0
   15 40 0x200 5 0
   16 40 0x400 0 0
   17 45 0x200 6 0
   18 50 0x200 7 0
   19 50 0x100 0 0 -> after 50 ms. the next round will start from timestamp 5 , 10 ,15 etc..
   20 50 0x400 0 0

   we'll have to check that ECU_id 0x100 have one apperance each 50 ms.


   Behavioral anomaly:

   * check range values (speed_value 0-300 , acceleration 0-100 , break 0-100 , abs 0/1 , pressure_percentage 0-100)
     can be done in the same timestamp. no need to compare diffrent timestamps.


   * if value of pedal gas or break bigger then 0, its cant go down back to 0 in less then 10 ms.
     need to check the next pedal message and compare.

   * gass and brake pedal cannot be bigger then 0 simultaneously, can be checked in the same timestamp.

   * the carspeed cannot changed faster the 5kmh within 50 ms. needed tocheck evrey 50 ms the speedometer.

   * exeption to this is a car crash, in wich the speed will change to 0. is dhould stay at 0. if it goes up we repport


  Correlation anomaly:

   * when accelration bigger then 0 speed should not be decreasing. if gas pedal is on (bigger then 0) the
    speed in the speedometer will not decrease. need to compare between prdals and speedometer.

   * when brake is active bigger then 0, speed should not be increasing.  compare pedals to speedometer.

   * when brake pressed hard (80+) the ABS will recive 1

   * when tire_pressure bellow 30, speed cant be above 50kmh  if it does report

   * the tire_pressure cant increase while the car is moving.
