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
int lowspeed = 90;

//Start button pin
int button_pin = A2;


class distanceSensor {
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

      float time, dist;

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
      //  r = min(255,int(r * RedCorrection));
      //  g = min(255,int(g * GreenCorrection));
      //  b = min(255,int(b * BlueCorrection));
    }
};

void initMotor() {
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void goForward() {
  analogWrite(speed, lowspeed);
  analogWrite(IN4, VEL);
  analogWrite(IN3, 0);
}

void goBackward() {
  analogWrite(speed, lowspeed);
  analogWrite(IN4, 0);
  analogWrite(IN3, VEL);
}

void stop() {
  analogWrite(IN4, 0);
  analogWrite(IN3, 0);
}

void goLeft() {
  servomotor.write(30);
  delay(20);
  digitalWrite(servopin, 0);
}

void goRight() {
  servomotor.write(120);
  delay(20);
  digitalWrite(servopin, 0);
}
void goFront() {
  servomotor.write(90);
  delay(20);
  digitalWrite(servopin, 0);
}

// Distance sensor instancies
distanceSensor front(4, 3);
distanceSensor left(6, 5);
distanceSensor right(10, 9);
distanceSensor back(12, 11);

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
  if (analogRead(button_pin) > 140) { run = true; }
  // When the start button is pressed
  // ----------------------------Debugging code---------------------------------
    /*Serial.print("Front: ");
    Serial.print(front.getDistance());
    Serial.print(" Left: ");
    Serial.print(left.getDistance());
    Serial.print(" Right: ");
    Serial.print(right.getDistance());
    Serial.println(" ");*/
  // ----------------------------Debugging code---------------------------------

  // While the car is on
  if (run) {
    goForward();

    //measuring the difference from both distances
    int leftd = left.getDistance();
    int rightd = right.getDistance();
    int diference = abs(leftd - rightd);

    //straight paths
    if (front.getDistance() >= 120 && (left.getDistance() < 80 || right.getDistance() < 80)) {
      goFront();

      //difference between walls
        if (left.getDistance() <= 25 && (cornerr != 0 || cornerl !=0)) {
          lowspeed = 120;
          goForward();
          goRight();
          goFront();

        } 
        else if (right.getDistance() <= 25 && (cornerr != 0 || cornerl !=0)) {
          lowspeed = 120;
          goForward();
          goLeft();
          goFront();

        }
    }
    //large section detections
    else if (left.getDistance() >= 100 && front.getDistance() < 115 && cornerr == 0) {
      lowspeed = 150;
      goForward();
      goLeft();
      cornerl += 1;

    }
    else if (right.getDistance() >= 100 && front.getDistance() < 115 && cornerl == 0) {
      lowspeed = 255;
      goForward();
      goRight();
      cornerr += 1;
    }
    //stops at last lap

    if (cornerr >= 12 || cornerl >= 12) {
        delay(600);
        run = false;
        stop();
      }
    lowspeed = 90;
    goForward();
  }
