String nom = "Arduino";
String msg;


struct color_t{
  unsigned int ID;

  int valores[3];
};

color_t prueba = {3,{255,120,120}};


void LeerPuertoSerial() {
  msg = "";
  if (Serial.available()) {
    delay(10);
    while (Serial.available() > 0) {
      msg += (char)Serial.read();
    }
    Serial.flush();
  }
}


void enviarDatos(){
  Serial.print(prueba.ID);
  Serial.print(",");
  for (int i= 0; i < 3;i++) {
    Serial.print(prueba.valores[i]);
    Serial.print(",");
  }

  
}



void setup() {
  Serial.begin(9600);

  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);  

}

void loop() {
  LeerPuertoSerial();


  if (msg != "") {
    enviarDatos();

  }
  
  delay(300);
}
