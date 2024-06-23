
// --------------------libraries------------------------
// Gyroscope lib
#include "Arduino_BMI270_BMM150.h"
// Color sensor lib
#include "Arduino_APDS9960.h"
// Servomotor lib
#include <Servo.h>
// --------------------libraries------------------------

// --------------------Constants------------------------
// Constant of the sound speed (check the readme of source on github)
#define SND_T 0.017
// Constant of the car's speed 
#define VEL 128
// --------------------Constants------------------------

// Servo instance
Servo servomotor;
int servopin = 2;

// Line counters
int cornerl = 0;
int cornerr = 0;

// Motors pins
int IN3 = A0;
int IN4 = A1;
int speed = A3;
int lowspeed = 500;

//Start button pin
int button_pin = A2;


class distanceSensor
{
  private:
  int trigger, echo;
  public:
  // Constructor
  distanceSensor(int _trigger, int _echo) {
    trigger = _trigger;
    echo = _echo;
  }
  // Pin configuration
  void init() {
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trigger, LOW);
  }

  // Distance method
  int getDistance() {

    float time,dist;
    
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    time = pulseIn(echo, HIGH);

    dist = time * SND_T;

    return int(dist);
    
  }
  
};

class imuAndApds {
  private:
  // Sensibility of z axis desviation
  int threshold;

  public:
  float x, y, z;
  // Color values in RGB (8bit)
  int r, g, b;
  
  imuAndApds(int _threshold) {
    this->threshold = _threshold;
  }
  
  // Pinmode of the sensors and Sensor configuration
  void init() {
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);

    
    if (!APDS.begin()) {
      Serial.println("Failed to initialize APDS!");
    }
    if (!IMU.begin()) {
      Serial.println("Failed to initialize IMU!");
    }
  }

  void ReadColor() {
    
 while (!APDS.colorAvailable()) {
    //delay(5);
  }
  //turn on LED light
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  
  APDS.readColor(r, g, b);

  //converts colors to a 0-255 range
  r = min(255,int(r * RedCorrection));
  g = min(255,int(g * GreenCorrection));
  b = min(255,int(b * BlueCorrection));
  }
};

void initMotor() {
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}


void goForward() {
  analogWrite(IN4, VEL);
  analogWrite(IN3, 0);
  analogWrite(speed, lowspeed);
}

void goBackward() {
  analogWrite(IN4, 0);
  analogWrite(IN3, VEL);
  analogWrite(speed, lowspeed);
}

void stop() {
  analogWrite(IN4, 0);
  analogWrite(IN3, 0);
}

void goLeft() {
  servomotor.write(45);
  delay(100);
  digitalWrite(servopin,0);
  
} 

void goRight() {
  servomotor.write(135);
  delay(100);
  digitalWrite(servopin,0);
}
void goFront() {
  servomotor.write(90);
  delay(100);
  digitalWrite(servopin,0);
}


// Distance sensor instancies
distanceSensor front(4,3);
distanceSensor left(6,5);
distanceSensor right(10,9);
distanceSensor back(12,11);

imuAndApds imuColor(50);

// States for the first challenge
bool run = false;
bool checkColor = false;
void setup() {
  Serial.begin(9600);
  front.init();
  right.init();
  left.init();
  back.init();
  imuColor.init();
  pinMode(button_pin, INPUT);
  pinMode(speed, OUTPUT);
  servomotor.attach(servopin);

}


void loop() {


  // Start statement
  if (analogRead(button_pin) > 140){run = true;}
  // When the start button is pressed
  
    Serial.print("Front: ");
    Serial.print(front.getDistance());
    Serial.print(" Left: ");
    Serial.print(left.getDistance());
    Serial.print(" Right: ");
    Serial.print(right.getDistance());
    Serial.println(" ");
    
    // While the car is on
    if (run) {
      
      goForward();

    
    //measuring the difference from both distances 
    int leftd = left.getDistance();
    int rightd = right.getDistance();
    int diference = abs(leftd -rightd);
    
    
    //
    if (front.getDistance() >= 100) {
        goFront();

        //difference between walls
      if (diference >= 36){
        
        if (leftd > rightd){
          goLeft();
          delay(500);

        }else if (leftd < rightd) {
          goRight();
          delay(500);
          }
        }
        }
        //large section detections
        else if (leftd >= 100 && front.getDistance() < 100 && cornerr == 0){
        goLeft();
        cornerl += 1;
        delay(500);
      } 
      else if (rightd >= 100 && front.getDistance() < 100 && cornerl == 0){
        goRight();
        cornerr += 1;
        delay(500);
      }
      //stops at last lap
      if (cornerr >= 12 || cornerl >= 12) {
        delay(600);
        run = false;
        stop();
      }
    } 
    delay(500);
  }

  
