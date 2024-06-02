#include "Arduino_BMI270_BMM150.h"
#include "Arduino_APDS9960.h"
#define SND_T 0.017

static const float RedCorrection = 227.0 / 48;
static const float GreenCorrection = 182.0 / 47;
static const float BlueCorrection = 245.0 / 40;


void SerialEnviarDist(int s1,int s2,int s3,int s4);
void SerialEnviarFloat(float f);
void SerialEnviarInt(long v);
void IntToBytes(byte* bytes, long lng);
void FloatToBytes(byte* bytes, float f);


class snd_sensor_t
{
  private:
  int trigger, echo;
  public:


  
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
    delayMicroseconds(1);
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
    
    Serial.println("Started Color");
    
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


  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
  

  // Read the color.
  APDS.readColor(r, g, b);

  r = min(255,int(r * RedCorrection));
  g = min(255,int(g * GreenCorrection));
  b = min(255,int(b * BlueCorrection));



  }
};


snd_sensor_t u1(5,6);
snd_sensor_t u2(7,8);
snd_sensor_t u3(9,10);
snd_sensor_t u4(11,12);


sensor_t Sensor(50);

void setup() {
  Serial.begin(9600);


  
  Sensor.Init();

  

  
}

void loop() {

  
  SerialEnviarDatos(&Sensor, &u1, &u2, &u3, &u4);
  
  

  
  
 
}

void SerialEnviarDatos(sensor_t *s, snd_sensor_t *snd1,snd_sensor_t *snd2,snd_sensor_t *snd3,snd_sensor_t *snd4) {



  //lee los datos del giroscopio


  float _x,_y,_z;
  if (IMU.gyroscopeAvailable()) {
  IMU.readGyroscope(_x, _y, _z);
  }
  
  unsigned char* sz = (unsigned char*)&_z;

  s->ReadColor();

  byte Data[34];

  byte byter[4];
  byte byteg[4];
  byte byteb[4];
  
  FloatToBytes(byter, s->r);
  FloatToBytes(byteg, s->g);
  FloatToBytes(byteb, s->b);

  //colores r g b

  for (int i = 0; i < 4; i++) {
    Data[i] = byter[i];
  }
  for (int i = 4; i < 8; i++) {
    Data[i] = byteg[i-4];
  }
  for (int i = 8; i < 12; i++) {
    Data[i] = byteb[i-8];
  }

  
  ///

  

  byte dist1[4];
  byte dist2[4];
  byte dist3[4];
  byte dist4[4];

  IntToBytes(dist1, snd1->GetDistance());
  IntToBytes(dist2, snd2->GetDistance());
  IntToBytes(dist3, snd3->GetDistance());
  IntToBytes(dist4, snd4->GetDistance());

  Data[13] = dist1[0];
  Data[14] = dist1[1];
  Data[15] = dist1[2];
  Data[16] = dist1[3];

  Data[17] = dist2[0];
  Data[18] = dist2[1];
  Data[19] = dist2[2];
  Data[20] = dist2[3];

  Data[21] = dist3[0];
  Data[22] = dist3[1];
  Data[23] = dist3[2];
  Data[24] = dist3[3];

  Data[25] = dist4[0];
  Data[26] = dist4[1];
  Data[27] = dist4[2];
  Data[28] = dist4[3];

  Data[29] = sz[0];
  Data[30] = sz[1];
  Data[31] = sz[2];
  Data[32] = sz[3];


  Data[33] = '\n';

  Serial.write(Data, 34);
  
 }






void IntToBytes(byte* bytes, long lng) {
  if (lng > 0) {
   bytes[0] = (byte) ((lng & 0xFF000000) >> 24);
  bytes[1] = (byte) ((lng & 0x00FF0000) >> 16);
  bytes[2] = (byte) ((lng & 0x0000FF00) >> 8);
  bytes[3] = (byte) ((lng & 0x000000FF));
  }
}

void FloatToBytes(byte* bytes, float f) {
  int length = sizeof(float);

  for(int i = 0;i < length;i++) {
    bytes[i] = ((byte*)&f)[i];
  }
}
