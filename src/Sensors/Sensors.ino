#include "Arduino_BMI270_BMM150.h"
#include "Arduino_APDS9960.h"

class Sensors {
  private:
  float x, y, z;
  int threshold;
  public:
  int r, g, b;

  Sensors(int _threshold) {
    threshold = _threshold;
  }

  bool inicializar() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("Started");

    if (!IMU.begin() && !APDS.begin()) {
      Serial.println("Failed to initialize IMU or APDS!");
      return false;
    }
    return true;
  }

  void leer_giroscopio() {
    if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
  
    if(z < -threshold)
    {
    Serial.println("Collision left");
    delay(500);
    }
    if(z > threshold)
    {
    Serial.println("Collision right");
    delay(500);
    }
    }
    Serial.println();
  }

  void leer_color() {
    // check if a color reading is available
    while (! APDS.colorAvailable()) {
      delay(5);
    }

    // read the color
    APDS.readColor(r, g, b);

    // print the values
    Serial.print("Red light = ");
    Serial.println(r);
    Serial.print("Green light = ");
    Serial.println(g);
    Serial.print("Blue light = ");
    Serial.println(b);
    Serial.println();

    // wait a bit before reading again
    delay(500);
  }
};

Sensors Sensor(50);

void setup() {
  Sensor.inicializar();
}

void loop() {
  Sensor.leer_giroscopio();
  Sensor.leer_color();
}