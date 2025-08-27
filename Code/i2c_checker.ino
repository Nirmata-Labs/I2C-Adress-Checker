#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height (for 0.91" OLED, usually 128x32)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1   // Reset pin (not used on Wemos D1 mini)

// OLED I2C address (ignore this in scan results)
#define OLED_ADDR 0x3C

// Create display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  Wire.begin();  // SDA = D2 (GPIO4), SCL = D1 (GPIO5) on Wemos D1 mini

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  delay(1000);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("I2C Scanner"));
  display.display();

  delay(1000);
}

void loop() {
  byte error, address;
  int nDevices = 0;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Scanning I2C..."));

  for (address = 1; address < 127; address++) {
    if (address == OLED_ADDR) continue;  // Skip OLED

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print(F("I2C device found at 0x"));
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);

      display.setCursor(0, 16);
      display.print(F("Found: 0x"));
      if (address < 16) display.print("0");
      display.println(address, HEX);
      nDevices++;
    }
  }

  if (nDevices == 0) {
    Serial.println(F("No I2C devices found"));
    display.setCursor(0, 16);
    display.println(F("No I2C device"));
  }

  display.display();
  delay(2000); // Wait before rescanning
}
