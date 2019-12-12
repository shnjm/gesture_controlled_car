//Written by Ahmet Burkay KIRNIK
//TR_CapaFenLisesi
//Measure Angle with a MPU-6050(GY-521)

#include <Wire.h>
#include <RH_ASK.h>
#include <SPI.h>

const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

double x;
double y;
double z;

RH_ASK rf_driver(300, 8, 9);

void setup()
{
  rf_driver.init();
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}
void loop()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  // lets go, do the job
  Serial.print("AngleX= ");
  Serial.println(x);

  Serial.print("AngleY= ");
  Serial.println(y);

  Serial.print("AngleZ= ");
  Serial.println(z);

  int xx = x < 200 ? x : x - 360;
  int yy = y < 200 ? y : y - 360;
  xx = xx > 30 ? 30 : xx;
  xx = xx < -30 ? -30 : xx;
  yy = yy > 30 ? 30 : yy;
  yy = yy < -30 ? -30 : yy;
  uint8_t buf[2];
  xx = (float)xx / 2.5;
  yy = (float)yy / 2.5;
  buf[0] = 'm' + xx;
  buf[1] = 'm' - yy;
  Serial.print((char)buf[0]);
  Serial.println((char)buf[1]);
  const char *msg = "Welcome to the Workshop!";
  rf_driver.send(buf, sizeof(buf));
  rf_driver.waitPacketSent();
  Serial.println("-----------------------------------------");
  delay(400);
}
