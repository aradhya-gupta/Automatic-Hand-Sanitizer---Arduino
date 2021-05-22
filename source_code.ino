//include libraries required for LCD display
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//define arduino pins to be used 
#define echoPin 2
#define trigPin 3
#define transistorPin 5
#define ledPin 6
#define buzzerPin 8

//global variables
long duration;            // time taken by ultrasonic pulse to come back to ultrasonic sensor
int distance;             // distance of hand from ultrasonic sensor
long number = 0;          // number of times used
const int threshold = 3;  // maximum number of uses allowed before refill (set to 3 for ease of demonstration)

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//set the modes(input or output) of used pins
void setup() {
  lcd.begin();
  pinMode(buzzerPin, OUTPUT); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(transistorPin, OUTPUT);
  pinMode(ledPin, OUTPUT); 
  Serial.begin(9600);
}
void loop() {
  lcd.clear();
  //calculate distance of hand from ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); //in micro secs
  
  distance = duration * 0.034 / 2; //speed of sound=340 m/sec
  Serial.print(" Dist: ");
  Serial.print(distance);
  Serial.print(" cm Cntr: ");
  Serial.print(number); 
  Serial.print(" "); 
  if(number<threshold){
    lcd.print("Number: "); 
    lcd.print(number);      
  }
  if(number>=threshold){
    digitalWrite(ledPin, HIGH); //turn the LED on
    lcd.clear(); 
    lcd.print("REFILL");  
    tone(buzzerPin, 2000);      //buzzer emits sound of 2kHz frequency
    delay(2000);
    noTone(buzzerPin);     
    Serial.println("LED on means 3 uses completed!");
  }
  else if (distance<=3){ //3 cm
    number++; 
    digitalWrite(transistorPin, HIGH);  //turn the DC pump on by switching the trasistor on 
    Serial.print("Pump On--");
    delay(150);
    digitalWrite(transistorPin, LOW);
    Serial.println("--Pump OFF");
    delay(2000);
  }
  else{
    digitalWrite(transistorPin, LOW);  
    Serial.println("--Pump OFF");
  }
  delay(50);
}
