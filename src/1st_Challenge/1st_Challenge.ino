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
// color corrections
static const float RedCorrection = 227.0 / 48;
static const float GreenCorrection = 182.0 / 47;
static const float BlueCorrection = 245.0 / 48;
// --------------------Constants------------------------

// Servo instance
Servo servomotor;

// Line counters
int orange_count = 0;
int blue_count = 0;

// Motors pins
int IN3 = A0;
int IN4 = A1;

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
    digitalWrite(trigger, LOW);

    time = pulseIn(echo, HIGH);

    dist = time * SND_T;

    return dist;
    
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
    
    //Serial.println("Started");
    
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
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
  
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
  analogWrite(IN3, VEL);
  analogWrite(IN4, 0);
}

void goBackward() {
  analogWrite(IN3, 0);
  analogWrite(IN4, VEL);
}

void stop() {
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

void goLeft() {
  servomotor.write(135);
} 

void goRight() {
  servomotor.write(45);
}
void goFront() {
  servomotor.write(0);
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

  imuColor.init();
  pinMode(button_pin, INPUT);
  servomotor.attach(4);

}

//pista_azul = [13,77,161]
//pista_naranja = [245,130,32]
void loop() {
  imuColor.ReadColor();
  // Getting the RGB code from nano's the lecture 
  int r = imuColor.r;
  int g = imuColor.g;
  int b = imuColor.b;

  // Start statement
  (digitalRead(button_pin) == 0)? run = true;
  
  // When the start button is pressed

  if (run){
    goForward();
    // measuring the difference from both distances 
    int left = left.GetDistance();
    int right = right.GetDistance();
    int diference;
    (left - right < 0)? diference = (left - right) * (-1) : diference = left - right;
    // While the car is on
    if (front.GetDistance() > 80) {
      if (diference >= 36){
        if (left > right){
          goLeft();
          delay(500);
          goFront();

        } else if (left < right) {
          goRight();
          delay(500);
          goFront();
        }
      }
    } else if (front.GetDistance() < 80 && checkColor == true) {
        goFront();
        if ((r-g) > 40 && (r-b) > 40) {
          //Serial.println("naranja detectado");
          goRight();
          orange_count ++;
          checkColor = false;
        } else if ((b-r) > 34 && (b-g) > 34) {
          //Serial.println("azul detectado");
          goLeft();
          blue_count ++;
          checkColor = false;

    }
  }
  
  if (orange_count >= 12 || blue_count >= 12) {
    delay(1000);
    stop();
  }
  };
  }

  
