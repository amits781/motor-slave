#include<LiquidCrystal.h> // including the lcd library
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // sets the interfacing pins

int startLed = 10;
int stopLed = 9;
bool isStart = false;
bool isStop = true;
int digitalPinBlue = A1;
int current = 0;
void setup()
{
 Serial.begin(9600);
 pinMode(startLed, OUTPUT);
 pinMode(stopLed, OUTPUT);
 digitalWrite(startLed, HIGH);
 lcd.clear();
 lcd.begin(16, 2);  // initializes the 16x2 LCD
 while (!Serial) {
    ; // wait for serial port to connect.
  }

  lcd.setCursor(0, 0); // set cursor to first row
  lcd.print("Init done"); // print to lcd
}


void handleAction(String action){
  if(strcmp("start",action.c_str()) == 0){
    startMotor();
  } else if(strcmp("stop",action.c_str()) == 0){
    stopMotor();
  } 
}

void startMotor(){
  digitalWrite(startLed, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(startLed, HIGH);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  isStart = true;
  isStop = false;
}

void stopMotor(){
  digitalWrite(stopLed, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(6000);                       // wait for a second
  digitalWrite(stopLed, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  isStart = false;
  isStop = true;
}


void loop()
{
  String input = "";
  String action = "";
  String value = "";
  if (Serial.available() > 0) { 
      input = Serial.readString();
      int index = input.indexOf(':'); 
      action = input.substring(0,index);
      value = input.substring(index+1,input.length());
      if(strcmp("action",action.c_str()) == 0){
        lcd.clear();
        lcd.setCursor(0, 0); // set cursor to first row
        lcd.print("Action: "+action); // print out to LCD
        lcd.setCursor(0, 1); // set cursor to secon row
        lcd.print("Value: "+String(value)); // print out the retrieved value to the second row
        handleAction(value);
      } else if (strcmp("display",action.c_str()) == 0){
        int indexTime = value.indexOf('|');
        int elapse = value.substring(0,indexTime).toInt();
        int rem = value.substring(indexTime+1,value.length()).toInt();
        rem = rem - elapse;
        int eMin = elapse/60;
        int eSec = elapse%60;
        int remMin = rem/60;
        int remSec = rem%60;
        lcd.clear();
        lcd.setCursor(0, 0); // set cursor to first row
        lcd.print("TRan: " + String(eMin)+":"+String(eSec)); // print out to LCD
        lcd.setCursor(0, 1); // set cursor to secon row
        lcd.print("TRem: " + String(remMin)+":"+String(remSec)); // print out to LCD
      } else if (strcmp("query",action.c_str()) == 0){
        lcd.clear();
        if(strcmp("0",value.c_str()) == 0 ){
          lcd.setCursor(0, 0); // set cursor to first row
          lcd.print("Sync");
          lcd.setCursor(0, 1); // set cursor to secon row
          lcd.print("Completed");
        } else{
          lcd.setCursor(0, 0); // set cursor to first row
          lcd.print("Syncing");
          lcd.setCursor(0, 1); // set cursor to secon row
          lcd.print("Data");
          if( current == 1 ){
            Serial.write("   IP1   "); 
          } else {
            Serial.write("   IP0   ");
          }
        }
        delay(1000);
      }
    }
    int analogOutput = analogRead(digitalPinBlue);
    if(analogOutput > 1000){
      if(current != 1 ){
        current = 1;
        Serial.write("   P1   ");
        delay(1000);
      }
    } else {
      if(current != 0 ){
        current = 0;
        Serial.write("   P0   ");
        delay(1000);
      }
    }
  }
