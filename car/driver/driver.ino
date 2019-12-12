#include <RH_ASK.h>
#include <SPI.h>
#include <AFMotor.h>

AF_DCMotor motor_right(2);
AF_DCMotor motor_left(3);
RH_ASK rf_driver(300, 2, A1);

void motor_cntrl(char xx, char yy)
{
  Serial.println("Input char");
  Serial.print(xx);
  Serial.println(yy);
  int x = xx - 'm';
  int y = yy - 'm';

  int r8, lf;
  float spd = 255 * (((float)abs(y)) / 12);
  Serial.println("r8 lf spd");
  Serial.print(x);
  Serial.print(y);
  Serial.println(spd);
  r8 = lf = spd;
  if (x >= 0)
  {
    r8 = r8 - ((spd / 12) * abs(x));
  }
  else
  {
    lf = lf - ((spd / 12) * abs(x));
  }
  Serial.print("r8=");
  Serial.print(r8);
  Serial.print(" lf=");
  Serial.println(lf);
  Serial.println("-------");
  motor_right.setSpeed(r8);
  motor_left.setSpeed(lf);
  if (y >= 0)
  {
    motor_right.run(FORWARD);
    motor_left.run(FORWARD);
  }
  else
  {
    motor_right.run(BACKWARD);
    motor_left.run(BACKWARD);
  }
}

void setup()
{
  rf_driver.init();
  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  uint8_t buf[2];
  uint8_t len = sizeof(buf);
  if (rf_driver.recv(buf, &len))
  {
    char x = (char)buf[0];
    char y = (char)buf[1];
    if ((x >= 'a' && x <= 'y') && (y >= 'a' && y <= 'y'))
      motor_cntrl(x, y);
  }
}
