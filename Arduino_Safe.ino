#include <Wire.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9
#define I2C_ADDR 0x27
#define MAX_KEY_LEN 4
#define BUZZER 8

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte colPins[COLS] = {4, 3, 2, 1};
byte rowPins[ROWS] = {8, 7, 6, 5};

String uidString;

unsigned long startTime;
const int duration = 10;


Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
String validCardUid = "13e16ea9"; // The UID of the valid card
String pinCode = "1234"; // The valid pin code
String enteredPin = "";

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.init();
  lcd.setBacklight(50);
  lcd.setCursor(0, 0);
  Serial.println("RFID door lock system ready.");
  lcd.print("Scanner Ready");
  lcd.setCursor(0,1);
  lcd.print("TAP CARD");   
}

void loop() {
  char key = keypad.getKey();
  
      if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) 
      {
        String cardUid = "";
        uidString = "";
        for (byte i = 0; i < mfrc522.uid.size; i++) 
        {
          uidString += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
          uidString += String(mfrc522.uid.uidByte[i], HEX);
        }
        
        Serial.print("Card UID: ");
        Serial.println(uidString);
        
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          cardUid += String(mfrc522.uid.uidByte[i], HEX);
        }
        if(cardUid == validCardUid)
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Scanning...");
            delay(3000);
            Serial.println("Access card recognized");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Card Valid");
            delay(1000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Enter 4-Pin:");
            enteredPin = "";
            while (enteredPin.length() < 4) {
              char key = keypad.getKey();
              if (key) 
              {
                enteredPin += key;
                lcd.setCursor(enteredPin.length(), 1);
                lcd.print("*");
              }
            }
            if(enteredPin == pinCode)
            {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Access granted");
              Serial.println("Access granted");
              delay(2000);
              lcd.clear();   
              lcd.setCursor(0,0);
              lcd.print("Door Unlocked!");
              delay(2000);
              lcd.clear();   
              lcd.setCursor(0,0);
              lcd.print("Closing in:");
              int seconds = 0;
              int remaining = 10;
              while(remaining>0)
              {
                remaining = duration - seconds;
                lcd.setCursor(0, 1);
                lcd.print(remaining);
                lcd.print(" seconds");
                seconds++;                
                delay(1000);   
              }
              delay(3000);
              setup();
            }
            else
            {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Incorrect Pin");
              Serial.println("Incorrect Pin");
              delay(2000);
              lcd.clear();   
              setup();  
            }
        }
        else
        {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Scanning...");
              delay(3000);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Access Denied");
              Serial.println("Access Denied");
              delay(2000);
              lcd.clear();   
              setup();           
        }
      }
  delay(1000);      
}


