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


int v_max = 120;
int atras = 50;

int LFSensor[2];

int setPoint = 1000; // Punto Medio
float Kp = 0.1; //Probar primero con este
int Kd = 0; //Segundo con este kp/2
int Ki = 0;
int error = 0; // posicion 0 1000 2000
int errorAnterior = 0; // posicion anterior
float valorPID = 0; 

bool buttonState = false;

bool stopState = true;

void setup(){
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
  
  if(buttonState){
  
    stopState = true;
    //Lógica del codigo
    sensRead();
    calcularPID();
    motoresPID();
    
  } else if (stopState){
    deadPoint();  
  }
    
}

void state(){
  buttonState = !buttonState;
}

void sensRead()
  {
    LFSensor[0] = digitalRead(sensIzq_Pin); // Izq
    LFSensor[1] = digitalRead(sensDer_Pin);
        
    if((     LFSensor[0]==  1)&&(LFSensor[1]== 0 ))  {error = 0;} //Izquierda
    else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 ))  {error = 1000;} //Medio
    else if((LFSensor[0]== 0 )&&(LFSensor[1]== 1 ))  {error = 2000;} //Derecha
    else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 ))  {goBackward();}
    
  }

  void calcularPID()
  {
    int P, I, D; 
    P = error - setPoint;
    I = error + errorAnterior;
    D = error - errorAnterior;
    valorPID = (Kp*P) + (Ki*I) + (Kd*D);
    errorAnterior = error;

    //Valor PID
    Serial.println("VALOR PID: ");
    Serial.print(valorPID);
    Serial.print(" ");
    Serial.print(P);
    Serial.print(" ");
    Serial.print(I);
    Serial.print(" ");
    Serial.print(D);
  }

  void motoresPID(){

    if(valorPID > v_max){valorPID=v_max;}
    if(valorPID < -v_max){valorPID= -v_max;}
    
    //Acción del motor A
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, v_max + valorPID);
  
    //Acción del motor B
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, v_max - valorPID);
  }

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
    //Serial.println("Nopea se mueve hacia atras");
  }
