#define CAR_CONFIGFILE  "car.settings"

typedef struct
{
  unsigned address;        // address for PCA9685
  unsigned frequency;      // frequency for PCA9685

  unsigned piLeftA;        // wiringPi pin for left drive A
  unsigned piLeftB;        // wiringPi pin for left drive B
  unsigned piRightA;       // wiringPi pin for right drive A
  unsigned piRightB;       // wiringPi pin for right drive B
  unsigned pwmLeft;        // address of left drive PWM
  unsigned pwmRight;       // address of right drive PWM
  unsigned minDrive;       // minimum PWM cycle value for drive wheels (where rotation still occurs)
  unsigned maxDrive;       // maximum PWM cycle value for drive wheels (likely 4095)
  int      reverseLeft;    // True to reverse forward and reverse directions on left drive wheel
  unsigned reverseRight;   // True to reverse forward and reverse directions on right drive wheel

  unsigned pwmSteering;    // address of steering PWM 
  unsigned minSteering;    // minimum PWM cycle value for steering servo
  unsigned maxSteering;    // maximum PWM cycle value for steering servo
  unsigned homeSteering;   // PWM cycle value for home (centered) position on steering servo

  unsigned pwmPan;         // address of pan PWM 
  unsigned minPan;         // minimum PWM cycle value for pan servo
  unsigned maxPan;         // maximum PWM cycle value for pan servo
  unsigned homePan;        // PWM cycle value for home position on pan servo

  unsigned pwmTilt;        // address of tilt PWM 
  unsigned minTilt;        // minimum PWM cycle value for tilt servo
  unsigned maxTilt;        // maximum PWM cycle value for tilt servo
  unsigned homeTilt;       // PWM cycle value for home position on tilt servo

} CarInfoStruct;

