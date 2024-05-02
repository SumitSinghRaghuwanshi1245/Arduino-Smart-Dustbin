#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Constants
const int trigPin = 9;        // Lid control ultrasonic sensor
const int echoPin = 10;
const int trigPin2 = 12;      // Garbage level ultrasonic sensor
const int echoPin2 = 13;
const int servoPin = 6;

// Create an LCD display object at address 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables
long duration, duration2;
int distance, distance2;
Servo servo;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  servo.attach(servoPin);
  servo.write(0); // Lid closed
  Serial.begin(9600);

  lcd.init();                      // Initialize the LCD
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Dustbin");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // First sensor for lid control
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Second sensor for garbage level
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  // Check if someone is trying to use the bin
  if (distance < 20) {
    servo.write(90);
    lcd.setCursor(0, 0);
    lcd.print("Lid Opening...");
    lcd.setCursor(0, 1);
  } else {
    servo.write(0);
    lcd.setCursor(0, 0);
    lcd.print("Lid Closed      ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }

  // Display garbage level
  lcd.setCursor(0, 1);
  lcd.print("Fill Level: ");
  lcd.print(map(distance2, 0, 22, 100, 0)); // Assuming max depth of 30 cm
  lcd.print("%");

  delay(300); // Delay to reduce display flickering
}