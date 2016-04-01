#define CAR_LEFTDRIVE   0x04
#define CAR_RIGHTDRIVE  0x05

#define CAR_STEERING    0x01
#define CAR_PAN         0x0E
#define CAR_TILT        0x0F

#define CAR_CONFIGFILE  "car.settings"

typedef struct
{
  unsigned address;        // address for PCA9685
  unsigned frequency;      // frequency for PCA9685
  unsigned minDrive;       // minimum PWM cycle value for drive wheels (where rotation still occurs)
  unsigned maxDrive;       // maximum PWM cycle value for drive wheels (likely 4095)
  int      reverseLeft;    // True to reverse forward and reverse directions on left drive wheel
  unsigned reverseRight;   // True to reverse forward and reverse directions on right drive wheel

  unsigned minSteering;    // minimum PWM cycle value for steering servo
  unsigned maxSteering;    // maximum PWM cycle value for steering servo
  unsigned homeSteering;   // PWM cycle value for home (centered) position on steering servo

  unsigned minPan;         // minimum PWM cycle value for pan servo
  unsigned maxPan;         // maximum PWM cycle value for pan servo
  unsigned homePan;        // PWM cycle value for home position on pan servo

  unsigned minTilt;        // minimum PWM cycle value for tilt servo
  unsigned maxTilt;        // maximum PWM cycle value for tilt servo
  unsigned homeTilt;       // PWM cycle value for home position on tilt servo

} CarInfoStruct;

