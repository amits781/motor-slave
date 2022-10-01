#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 4 //define green LED pin
#define LED_R 5 //define red LED

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

long milisUpdated = 0;

int ledBlue = 7;
int ledGreen = 6;
int ledRed = 5;

int e_red=0;
int e_green=0;
int e_blue=255;

bool firstStart = true;
bool currStatus = false;

void setup() {
  Serial.begin(9600);   // Initiate a serial communication
  while (!Serial) {
      ; // wait for serial port to connect.
   }
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
 
  Serial.println("Put your card to the reader...");
  Serial.println();
  
}


void setLedColor(int initialValue, int finalValue, int ledValue){
  if(initialValue > finalValue){
    for(int i = initialValue; i >= finalValue; i--){
      analogWrite(ledValue, (255-i));
      delay(30);
    }
  } else {
    for(int i = initialValue; i <= finalValue; i++){
      analogWrite(ledValue, (255-i));
      delay(30);
    }
  }
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 { 
  setLedColor(e_green, green_light_value, ledGreen);
  setLedColor(e_blue, blue_light_value, ledBlue);
  setLedColor(e_red, red_light_value, ledRed);
  e_red = red_light_value;
  e_green = green_light_value;
  e_blue = blue_light_value;
  delay(1000);
}

void firstStartGlitch(){
  analogWrite(ledBlue, 255-200);
  delay(1000);
  
  for(int i = 0; i <= 5; i++){
  blinkLedWithDelay(ledBlue, 200);
  }
  analogWrite(ledBlue, 255-200);
  delay(200);
  for(int i = 0; i <= 1; i++){
    blinkLed(ledBlue, 200);
  }
  analogWrite(ledBlue, 255-200);
  delay(1000);
  analogWrite(ledBlue, 0);
  delay(5000);
  firstStart = false;
}

void blinkLed(int ledPin, int maxValue){
  analogWrite(ledPin, 255);
  delay(200);
  analogWrite(ledPin, (255-maxValue));
  delay(200);
}

void blinkLedWithDelay(int ledPin, int maxValue){
  analogWrite(ledPin, 255);
  for(int i = maxValue; i>= 0; i-=3){
    analogWrite(ledPin, (255-i));
    delay(1);
  }
}

void auraBackLight(){
  RGB_color(255, 0, 200);
  readRfid();
  if(currStatus == false){
      return;
  }
  RGB_color(18, 123, 153);
  readRfid();
  if(currStatus == false){
      return;
  }
  RGB_color(133, 7, 148);
  readRfid(); 
  if(currStatus == false){
      return;
  }
  RGB_color(198, 33, 214);
  readRfid(); 
  if(currStatus == false){
      return;
  }
  RGB_color(204, 5, 195);
  readRfid();
  if(currStatus == false){
      return;
  }
  RGB_color(178, 4, 159);
  readRfid();
  if(currStatus == false){
      return;
  }
  RGB_color(205, 4, 184);
  readRfid();
  if(currStatus == false){
      return;
  }
  RGB_color(18, 123, 153);
}

void readRfid(){
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "BC 72 AC 16") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println(millis());
    Serial.println();
    if((millis() - milisUpdated) > 10000){
      if(currStatus == false){
      currStatus = true;
    } else {
      currStatus = false;
    }
    delay(1000);
    Serial.println("Current Status:"+String(currStatus));
    milisUpdated = millis();
    }
    
  }
 
 else   {
    Serial.println(" Access denied");
    
  
  }
}

void loop() { 
  readRfid();
  if(currStatus == true){
    if(firstStart){
      firstStartGlitch();
    } else {
      auraBackLight();
    }
  }
}
