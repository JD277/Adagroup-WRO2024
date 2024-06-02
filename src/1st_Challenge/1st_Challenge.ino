#include "Arduino_BMI270_BMM150.h"
#include "Arduino_APDS9960.h"
#include <Servo.h>
#define SND_T 0.017
#define VEL 128
Servo servomotor;

static const float RedCorrection = 227.0 / 48;
static const float GreenCorrection = 182.0 / 47;
static const float BlueCorrection = 245.0 / 48;

int orange_count = 0;
int blue_count = 0;

int IN3 = A0;
int IN4 = A1;

class snd_sensor_t
{
  private:
  int trigger, echo;
  public:

  snd_sensor_t() {
    trigger = 0;
    echo = 0;
  }
  
  snd_sensor_t(int _trigger, int _echo) {
    trigger = _trigger;
    echo = _echo;
  }

  void Init() {
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trigger, LOW);
  }

  int GetDistance() {

    float tim,dist;
    
    digitalWrite(trigger, HIGH);
    digitalWrite(trigger, LOW);

    tim = pulseIn(echo, HIGH);

    dist = tim * SND_T;

    return dist;
    
  }
  
};

class sensor_t {
  private:
  
  int threshold;
  public:
  float x, y, z;
  int r, g, b;
  
  sensor_t(int _threshold) {
    this->threshold = _threshold;
  }
  
  void Init() {
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    
    Serial.println("Started");
    
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
  //enciende el LED
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
  
  //leer color
  APDS.readColor(r, g, b);

  //convierte los colores en un rango de 0-255
  r = min(255,int(r * RedCorrection));
  g = min(255,int(g * GreenCorrection));
  b = min(255,int(b * BlueCorrection));
  }
};

void InitMotor() {
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void GoForward() {
  analogWrite(IN3, VEL);
  analogWrite(IN4, 0);
}

void GoBackward() {
  analogWrite(IN3, 0);
  analogWrite(IN4, VEL);
}

void Stop() {
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

void GoLeft() {
  servomotor.write(135);
} 

void GoRight() {
  servomotor.write(45);
}

snd_sensor_t u1(5,6);
snd_sensor_t u2(7,8);
snd_sensor_t u3(9,10);
snd_sensor_t u4(11,12);

sensor_t Sensor(50);

void setup() {
  Serial.begin(9600);

  Sensor.Init();

  servomotor.attach(4);

}

//pista_azul = [13,77,161]
//pista_naranja = [245,130,32]
void loop() {
  Sensor.ReadColor();

  int r = Sensor.r;
  int g = Sensor.g;
  int b = Sensor.b;

  //4
  GoForward();

  if (u2.GetDistance() > 40) {
    GoRight();
  }
  else if (u4.GetDistance() > 40) {
    GoLeft();
  }
  
  //5
  if ((r-g) > 40 && (r-b) > 40) {
    //Serial.println("naranja detectado");
    GoRight();
    orange_count ++;
  }
  else if ((b-r) > 34 && (b-g) > 34) {
    //Serial.println("azul detectado");
    GoLeft();
    blue_count ++;
  }

  if (orange_count >= 12 || blue_count >= 12) {
    delay(1000);
    Stop();
  }







  
  };