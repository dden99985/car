#include <stdio.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <time.h>
#include <PCA9685.h>
#include <logging.h>

extern int LOGGING;

// Drive Motors
#define DRIVE0_A	0
#define DRIVE0_B	1
#define DRIVE1_A	2
#define DRIVE1_B	3


main(int argc, char *argv[])
{
  LOGGING = LOG_ERROR | LOG_NORMAL | LOG_FUNCTION;

  int status;

  if(wiringPiSetup() == -1)
  {
    fprintf(stderr, "WiringPi failed to initialize!");
    return -1;
  }

  CarInit();

/*
  pinMode(DRIVE0_A, OUTPUT);
  pinMode(DRIVE0_B, OUTPUT);
  pinMode(DRIVE1_A, OUTPUT);
  pinMode(DRIVE1_B, OUTPUT);

  int hPCA9685;
  if ((hPCA9685 = PCA9685Init(0x40, 200)) == -1)
  {
    fprintf(stderr, "Error: %d\n       %s\n", errno, strerror(errno));
    return 1;
  }

  if((status = PCA9685WriteLedDuty(hPCA9685, 4, 100)) != 0)
    fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
  if((status = PCA9685WriteLedDuty(hPCA9685, 5, 100)) != 0)
    fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));

  digitalWrite(DRIVE0_A, HIGH);
  digitalWrite(DRIVE0_B, LOW);
  digitalWrite(DRIVE1_A, HIGH);
  digitalWrite(DRIVE1_B, LOW);

  int q;
  for(q = 4096; q > 0; q--)
  {
    if((status = PCA9685WriteLedDuty(hPCA9685, 4, q)) != 0)
      fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
    if((status = PCA9685WriteLedDuty(hPCA9685, 5, q)) != 0)
      fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
    delay(1);
  }

  delay(1000);

  digitalWrite(DRIVE0_A, LOW);
  digitalWrite(DRIVE0_B, HIGH);
  digitalWrite(DRIVE1_A, LOW);
  digitalWrite(DRIVE1_B, HIGH);

  for(q = 0; q <= 4096; q++)
  {
    if((status = PCA9685WriteLedDuty(hPCA9685, 4, q)) != 0)
      fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
    if((status = PCA9685WriteLedDuty(hPCA9685, 5, q)) != 0)
      fprintf(stderr, "I2C Error: %d - %s\n", errno, strerror(errno));
    delay(1);
  }

  delay(1000);

  digitalWrite(DRIVE0_A, LOW);
  digitalWrite(DRIVE0_B, LOW);
  digitalWrite(DRIVE1_A, LOW);
  digitalWrite(DRIVE1_B, LOW);
*/
}

