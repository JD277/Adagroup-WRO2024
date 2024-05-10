String nom = "Arduino";
String msg;

float temp = 48.4;
float pi = 3.1418;


void SerialEnviarFloat(float f);

void IntToBytes(byte* bytes, long lng);

void FloatToBytes(byte* bytes, float f);



void setup() {
  
  Serial.begin(9600);
  
}


void loop() {


  SerialEnviarFloat(temp);
  
  delay(1000);
}





void SerialEnviarFloat(float f) {
  byte bytef[4];
  
  FloatToBytes(bytef, f);

  byte Data[5];

  for (int i = 0; i < 4; i++) {
    Data[i] = bytef[i];
  }
  Data[4] = '\n';

  Serial.write(Data, 5);
}


void IntToBytes(byte* bytes, long lng) {

  bytes[0] = (byte) ((lng & 0xFF000000) >> 24);
  bytes[1] = (byte) ((lng & 0x00FF0000) >> 16);
  bytes[2] = (byte) ((lng & 0x0000FF00) >> 8);
  bytes[3] = (byte) ((lng & 0x000000FF));
}

void FloatToBytes(byte* bytes, float f) {
  int length = sizeof(float);

  for(int i = 0;i < length;i++) {
    bytes[i] = ((byte*)&f)[i];
  }
}
