#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// OLED-skärmens inställningar
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Frekvenser för färger
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Totala summan av färger
int totalSum = 0;

// Tröskelvärden för färgigenkänning
int redThresholdMin = 21;
int redThresholdMax = 23;
int greenThresholdMin = 32;
int greenThresholdMax = 34;
int blueThresholdMin = 25;
int blueThresholdMax = 28;

// Variabel för att hålla koll på om en giltig färg har upptäckts
bool validColorDetected = false;

void setup() {
  // Starta seriell kommunikation
  Serial.begin(9600);

  // Starta OLED-skärmen
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // Rensa skärmen
  display.clearDisplay();
  display.display();

  // Inställningar för färgsensorn
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // Sätta frekvensskalningen till 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
}

void loop() {
  // Läs röd färg
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  redFrequency = pulseIn(sensorOut, LOW);
  if(redFrequency >= redThresholdMin && redFrequency <= redThresholdMax) {
    totalSum += 2;
    validColorDetected = true;
  }

  // Läs grön färg
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  greenFrequency = pulseIn(sensorOut, LOW);
  if(greenFrequency >= greenThresholdMin && greenFrequency <= greenThresholdMax) {
    totalSum += 5;
    validColorDetected = true;
  }

  // Läs blå färg
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  blueFrequency = pulseIn(sensorOut, LOW);
  if(blueFrequency >= blueThresholdMin && blueFrequency <= blueThresholdMax) {
    totalSum += 10;
    validColorDetected = true;
  }

  // Uppdatera OLED-skärmen om en giltig färg har upptäckts
  if(validColorDetected) {
    updateOLED();
    validColorDetected = false; // Återställ variabeln
  }
  
  delay(100); // Fördröjning innan nästa avläsning
}

void updateOLED() {
  // Rensa skärmen
  display.clearDisplay();
  
  // Skriv ut den nya totala summan
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Du har: ");
  display.print(totalSum);
  display.println(" kr");
  
  // Uppdatera skärmen
  display.display();
}
