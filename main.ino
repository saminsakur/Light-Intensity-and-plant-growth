// https://www.livelyroot.com/blogs/plant-care/snake-plant-light-requirements

#include <RtcDS1302.h>
#include <ThreeWire.h>
#include <BH1750.h>
#include <RtcDateTime.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// DS1302 CLK/SCLK --> 5
// DS1302 DAT/IO --> 4
// DS1302 RST/CE --> 2

BH1750 lightMeter;
LiquidCrystal_I2C lcd(0x27, 16, 2);
ThreeWire samwire (4,5,2); // IO,SCL,CE
RtcDS1302<ThreeWire> rtc(samwire);

int r1 = 6;
int r2 = 7;

const int OnHour = 9;
const int OffHour = 17;
const int offMin = 0;

void setup() {
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  Serial.begin(57600);
  lcd.init();
  lcd.backlight();

  rtc.Begin();
  RtcDateTime currentTime = RtcDateTime(__DATE__, __TIME__);

    if (!rtc.IsDateTimeValid()) 
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        lcd.setCursor(0, 0);
        lcd.println("RTC lost confidence in the DateTime!");
        delay(1000);
        rtc.SetDateTime(currentTime);
    }

    RtcDateTime now = rtc.GetDateTime();
    if (now < currentTime) {
      Serial.println("RTC is older than compile time!  (Updating DateTime)");
      rtc.SetDateTime(currentTime);
    } else if (now > currentTime) {
      Serial.println("RTC is newer than compile time. (this is expected)");
    } else if (now == currentTime) {
      Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  lightMeter.begin();

  lcd.setCursor(2, 0);
  lcd.print("Siuuu!");
  delay(500);
}

void loop() {
  RtcDateTime now = rtc.GetDateTime();
  Serial.print("TIME: ");
  Serial.print(now.Hour());
  Serial.print(":");
  Serial.print(now.Minute());
  Serial.print(":");
  Serial.print(now.Second());
  Serial.println(" ");

  if (now.Hour()>=OnHour && now.Hour()<OffHour && now.Minute()>=offMin){
    lights_on();
  }
  else{
    lights_off();
  }
  
  lcd.setCursor(0,0);
  lcd.print("Light intensity: ");
  float lux = lightMeter.readLightLevel();
  lcd.setCursor(4,1);
  lcd.print(lux);
  lcd.print(" lx \n");
  lcd.println();
  delay(1000);
}

void lights_off(){
  digitalWrite(r1, HIGH);
  digitalWrite(r2, HIGH);
  Serial.println("LIGHTS OFF!");
}

void lights_on(){
  digitalWrite(r1, LOW);
  digitalWrite(r2, LOW);
  Serial.println("LIGHTS ON!");
}