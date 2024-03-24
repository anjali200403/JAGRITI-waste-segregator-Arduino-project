#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo myservo;

int irsensor = 2; // IR sensor connected to digital pin 2
int moistureSensorPin = A0; // Moisture sensor connected to analog pin A0
int dryWasteTrig = 6; // Trigger pin for dry waste ultrasonic sensor
int dryWasteEcho = 7; // Echo pin for dry waste ultrasonic sensor
int wetWasteTrig = 8; // Trigger pin for wet waste ultrasonic sensor
int wetWasteEcho = 11; // Echo pin for wet waste ultrasonic sensor
int buzzerPin = 10; // Buzzer connected to digital pin 10
int buzzDuration = 1000; // Duration to sound the buzzer (in milliseconds)
int distanceDry;
long durationDry;
int distanceWet;
long durationWet;
int distance;
long duration;
int currentAngle = 35; // Store the current angle of the servo

int inches = 0;

int cm = 0;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}



void setup() {
  pinMode(buzzerPin, OUTPUT);
  myservo.attach(9);
  //myservo.write(currentAngle); // Set the initial position to 90 degrees
  pinMode(irsensor, INPUT);
  
  // For Dry Garbage Ultrasonic Sensor
  pinMode(dryWasteTrig, OUTPUT);
  pinMode(dryWasteEcho, INPUT);
  
  // For Wet Garbage Ultrasonic Sensor
  pinMode(wetWasteTrig, OUTPUT);
  pinMode(wetWasteEcho, INPUT);
  lcd.begin();
  
  
}

void loop() {
  lcd.clear();
  int moistureValue = analogRead(moistureSensorPin); // Read moisture sensor value
  
  // Ultrasonic Sensor readings
  
  // measure the ping time in cm
  distanceDry = 0.01723 * readUltrasonicDistance(6, 6);
  // convert to inches by dividing by 2.54
 distanceWet = 0.01723 * readUltrasonicDistance(8, 8);
  delay(100); // Wait for 100 millisecond(s)
  Serial.println("Wet"); // Print "Wet" in the serial monitor
    Serial.print("Wet Waste Distance: ");
    Serial.println(distanceWet);
  Serial.println("Dry"); // Print "Dry" in the serial monitor
    Serial.print("Dry Waste Distance: ");
    Serial.print(distanceDry);
   delay(1000); // Wait for 100 millisecond(s)
  

  if (moistureValue > 550 && digitalRead(irsensor) == LOW) {
    myservo.write(75); // Set the servo position to 30 degrees
    lcd.setCursor(0,0); // set our LCD cursor to the correct position
    lcd.print("Wet Waste"); // push our output string to the LCD
    lcd.setCursor(0,1);
    lcd.print("e");
 
    Serial.println(" cm");
    delay(1000);
  } else if (digitalRead(irsensor) == LOW) {
    myservo.write(95); // Set the servo position to 150 degrees for dry condition
    lcd.setCursor(0,0); // set our LCD cursor to the correct position
    lcd.print("Dry Waste"); // push our output string to the LCD
    lcd.setCursor(0,1);
    lcd.print("e");
    Serial.println(" cm");
    delay(1000);
  }

  // Check if either compartment is full
  if (distanceDry < 10 || distanceWet < 10) {
    digitalWrite(buzzerPin, HIGH);
    if(distanceDry<10){
      lcd.setCursor(0,0); // set our LCD cursor to the correct position
      lcd.print("Dry Dustbin is"); // push our output string to the LCD
      lcd.setCursor(0,1);
      lcd.print("bin full");
    }else{
      lcd.setCursor(0,0); // set our LCD cursor to the correct position
      lcd.print("Wet Dustbin is"); // push our output string to the LCD
      lcd.setCursor(0,1);
      lcd.print("bin full");
    }
    delay(buzzDuration);
    digitalWrite(buzzerPin, LOW);
  }
  lcd.clear();
  // Return the servo to the 90-degree position
  myservo.write(currentAngle);
  int moisturevalue2=analogRead(moistureSensorPin);
  while(moisturevalue2==moistureValue){
    moisturevalue2=analogRead(moistureSensorPin);
  }
}