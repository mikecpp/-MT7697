#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
	lcd.begin();

	lcd.backlight();
}

void loop()
{
    char msg[16];
    static int count = 0;
    
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Hello, Mike");
    lcd.setCursor(0, 1);
    sprintf(msg, "Count = %d", count++);
    lcd.print(msg);
    delay(1000);
}

