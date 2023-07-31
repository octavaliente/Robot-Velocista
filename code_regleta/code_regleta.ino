/*
        Robot Amateur
  bfd-1000 sensor ir 5 canales
  tb6612 puente doble h
  arduino nano
  motor amarillo
*/

// Pines de conexión

// Pines de Conexión Puente Doble H
// Motor A Izquierdo
const int PWMA = 5; //Velocidad a ingresar
const int AIN1 = 8; //High o low // Movimiento Antihorario (Avance)
const int AIN2 = 7; //High o low // Horario (Retroceso)

const int STBY = 9;

// Motor B Derecho
const int PWMB = 6;
const int BIN1 = 10; //Movimiento Horario (Retroceso) en HIGH
const int BIN2 = 11; //Movimiento antiHorario (Avance) en HIGH

// Pulsador Pull Down
const int PUDO = 2;

// Pines de Conexión Sensores
const int s1 = A4;
const int s2 = A3;
const int s3 = A2;
const int s4 = A1;
const int s5 = A0;

// Variable de Sensor

int s1_value; // Izquierda del todo
int s2_value; // Izquierda - Medio
int s3_value; // Medio
int s4_value; // Derecha - Medio
int s5_value; // Derecha del todo

// Velocidades

const int atras = 0;
const int giro = 70;
const int vel = 100;
const int brake = 20;

bool buttonState = false;

bool stopState = true;

void setup() {
  // Setup de Puente doble H
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);

  // Setup Sensores
  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(s4, INPUT);
  pinMode(s5, INPUT);

  // Interrupción
  pinMode(PUDO, INPUT);
  attachInterrupt(digitalPinToInterrupt(PUDO), state, RISING); //Interrupción para el pulsador
  
  Serial.begin(9600);
  Serial.println("Void setup realizado");

}

void loop() {

  if (buttonState) {

    stopState = true;

    sensRead();

    if(s2_value == 0 && s3_value == 1 && s4_value == 1){
        
        goLeft();
      
      } else if (s2_value == 1 && s3_value == 0 && s4_value == 1) {
       
        goForward();
        
      } else if (s2_value == 1 && s3_value == 1 && s4_value == 0){
        
        goRight();
         
      } else if (s2_value == 0 && s3_value == 0 && s4_value == 1){
        //izquierdaPoquito
        goLeftM();  
      } else if (s2_value == 1 && s3_value == 0 && s4_value == 0){
        //derechaPoquito
        goRightM();
      }
  } else if (stopState) {

    deadPoint(); //Punto muerto para que los motores no se muevan
  }

}

//Función state(). Cuando se acciona el pulsador cambia el estado hasta que se vuelva a accionar.
void state() {
  buttonState = !buttonState;
}

void sensRead() {

  s2_value = digitalRead(s2);
  s3_value = digitalRead(s3);
  s4_value = digitalRead(s4);

  //Comprobar valores
  Serial.println("El valor del sensor izquierdo es: ");
  Serial.println(s2_value);
  Serial.println("El valor del sensor medio es: ");
  Serial.println(s3_value);
  Serial.println("El valor del sensor derecho es: ");
  Serial.println(s4_value);
}

//Función goForward(). El robot se mueve hacia adelante
void goForward() {
  //Acción del motor A
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, vel);

  //Acción del motor B
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, vel);

  //Comprobar valores (BORRAR)
  Serial.println("Nopea Turbo se mueve hacia adelante");
}

//Función goLeft(). El robot se mueve hacia la izquierda
void goLeft() {

  //Acción del motor A
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, vel);

  //Acción del motor B
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, brake);

  //Comprobar valores (BORRAR)
  Serial.println("Nopea Turbo se mueve hacia la izquierda");
}

//Función goRight(). El robot se mueve hacia la derecha
void goRight() {

  //Acción del motor A
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, brake);

  //Acción del motor B
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, vel);

  //Comprobar valores (BORRAR)
  Serial.println("Nopea Turbo se mueve hacia la derecha");
}

void goBackward() {
  //Acción del motor A
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, atras);

  //Acción del motor B
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, atras);

  //Comprobar valores (BORRAR)
  Serial.println("Nopea se mueve hacia atras");
}

// Función deadPoint(). Si no se acciona el pulsador los motores no se accionan. Esta función también evita que una vez que se usó y se volvió a pulsar el pulsador, los motores no queden con algun valor definido (los resetea)
void deadPoint() {

  stopState = false;

  //Acción del motor A
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(PWMA, LOW);

  //Acción del motor B
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  digitalWrite(PWMB, LOW);

  //Comprobar valores (BORRAR)
  Serial.println("Nopea está esperando el pulsador");
}

void goLeftM(){
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, vel);

  //Acción del motor B
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, giro);
}

void goRightM(){
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, giro);

  //Acción del motor B
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, vel);
}
