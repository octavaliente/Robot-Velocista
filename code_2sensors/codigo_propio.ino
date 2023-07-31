/*
Logica del código:
Una vez prendido el interruptor ON/OFF,
El pulsador determina el inicio del código y hasta no volver a apretarlo no se apaga
El Robot inicia con un deadpoint que hecho como un estado para que no se repita en loop
En el momento que se presiona el pulsador el robot entra en estado de lectura (se activa)
En caso de que se desee parar solamente debe volver a presionarse el pulsador para reactivar el deadpoint
Este evita que el robot funcione innecesariamente
*/

// Pines de Conexión Puente Doble H
//Motor A Izquierdo
const int PWMA = 6; //Velocidad a ingresar 
const int AIN1 = 8; //High o low // Movimiento Antihorario (Avance)
const int AIN2 = 7; //High o low // Horario (Retroceso)

const int STBY = 9;

//Motor B Derecho
const int PWMB = 5; 
const int BIN1 = 10; //Movimiento Horario (Retroceso) en HIGH
const int BIN2 = 11; //Movimiento antiHorario (Avance) en HIGH

//Pulsador Pull Down
const int PUDO = 2;

//Sensores
int sensIzq_Value; // Valor del sensor IZQ, un 1 es negro, un 0 blanco
int sensDer_Value; // Valor del sensor DER, un 1 es negro, un 0 blanco

const int sensDer_Pin = A1;
const int sensIzq_Pin = A0;

const int atras = 50; // Minimo 50
const int giro = 70; // 100
const int vel = 90; // Velocidad de recta
const int brake = 30; // Velocidad de frenado
//tiempo 7,87

/*
const int atras = 50; // Minimo 50
const int giro = 90; // 100
const int vel = 110; // Velocidad de recta
const int brake = 10; // Velocidad de frenado
*/
/*
const int atras = 50; // Minimo 50
const int giro = 90; // 100
const int vel = 100; // Velocidad de recta
const int brake = 10; // Velocidad de frenado
*/
/*PONER ESTE PARA SEGUNDA VUELTA*/


bool buttonState = false;

bool stopState = true;

int cont = 0;

void setup (){
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
  
  pinMode(sensIzq_Pin, INPUT);
  pinMode(sensDer_Pin, INPUT);

  pinMode(PUDO, INPUT);
  attachInterrupt(digitalPinToInterrupt(PUDO), state, RISING); //Interrupción para el pulsador 

  Serial.begin(9600);
  Serial.println("Void setup realizado");
}

void loop(){
  
  if (buttonState){   
    if (cont==0){cont++; delay(2000);}
    stopState = true;
    
    sensRead();
  
    if(sensIzq_Value == 0 && sensDer_Value == 0){ // Detecta zona blanca de ambos lados
      //Avance
      goForward();
    } else if (sensIzq_Value == 1 && sensDer_Value == 0){ // Detecta zona negra del lado izquierdo
      //Giro a la izquierda
      goLeft();
    } else if (sensIzq_Value == 0 && sensDer_Value == 1){ // Detecta zona negra del lado derecho
      //Gira a la derecha
      goRight();
    } else if (sensIzq_Value == 1 && sensDer_Value == 1){
      //Retroceso
      goBackward();  
    }  
  } else if(stopState) {
      
      deadPoint(); //Punto muerto para que los motores no se muevan
    }

}
//Función state(). Cuando se acciona el pulsador cambia el estado hasta que se vuelva a accionar.
void state(){
  buttonState = !buttonState;
}

//Función sensRead(). Actualiza las variables globales respecto de los valores del sensor
void sensRead(){

  Serial.println("Lectura realizada");
  
  sensIzq_Value = digitalRead(sensIzq_Pin); //Guarda el valor del sensor izquierdo
  sensDer_Value = digitalRead(sensDer_Pin); //Guarda el valor del sensor derecho

  //Corroborar Valores (BORRAR)
  Serial.println("El valor del sensor izquierdo es ");
  Serial.println(sensIzq_Value);
 
  Serial.println("El valor del sensor derecho es ");
  Serial.println(sensDer_Value);
}

//Función goForward(). El robot se mueve hacia adelante
void goForward() {
  //Acción del motor A
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, vel);
  
  //Acción del motor B
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, vel);

  //Comprobar valores (BORRAR)
  Serial.println("Nopea se mueve hacia adelante");
}

//Función goLeft(). El robot se mueve hacia la izquierda
void goLeft() {
  
  //Acción del motor A
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, giro);
  
  //Acción del motor B
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, brake);
  
  //Comprobar valores (BORRAR)
  Serial.println("Nopea se mueve hacia la izquierda");
}

//Función goRight(). El robot se mueve hacia la derecha
void goRight() {
  
  //Acción del motor A
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, brake);
  
  //Acción del motor B
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, giro);
  
  //Comprobar valores (BORRAR)
  Serial.println("Nopea se mueve hacia la derecha");
}

void goBackward(){
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
  
  cont = 0;
  //Comprobar valores (BORRAR)
  Serial.println("Nopea está esperando el pulsador");
}
