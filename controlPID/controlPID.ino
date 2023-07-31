  /*
          Robot Amateur
    bfd-1000 sensor ir 5 canales
    tb6612 puente doble h
    arduino nano
    motor amarillo
  */
  
  // Pines de conexión
  int cont = 0;
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
  int v_max = 180;
  int atras = 50;

  bool buttonState = false;
  
  bool stopState = true;

  // Control P I D
  int Kp = 30; //Probar primero con este
  int Kd = 7; //Segundo con este kp/2
  int Ki = 0;
  int error = 0; // posicion -4 -3 -2 -1 0 1 2 3 4 
  int errorAnterior = 0; // posicion anterior
  int valorPID = 0;
  // int Setpoint = 4000; // medio

  // int posicion = 0; // 0 1000 2000 3000 4000 5000 6000 7000 8000

  int LFSensor[5];
  
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

  void loop (){


     //testRead();
      //sensRead();
      //calcularPID();
      //motoresPID();
    if(buttonState){
      // Código
      if (cont==0){cont++; delay(2000);}
      stopState = true;
      
      sensRead();
      calcularPID();
      motoresPID();
      
      //Probar 
      //testRead();
    } else if(stopState){
      deadPoint();
    }
  }

  void state() {
    buttonState = !buttonState;
  }

  void sensRead()
  {
    LFSensor[0] = digitalRead(s1); // Izq
    LFSensor[1] = digitalRead(s2);
    LFSensor[2] = digitalRead(s3); // Med
    LFSensor[3] = digitalRead(s4);
    LFSensor[4] = digitalRead(s5); // Der
    
    if((     LFSensor[0]==  1)&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 0 ))  {error = 4;}
    else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 ))  {error = 3;}
    else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 1 ))  {error = 2;}
    else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 1 ))  {error = 1;}
    else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 ))  {error = 0;}
    else if((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 ))  {error = -1;}
    else if((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 ))  {error = -2;}
    else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 ))  {error = -3;}
    else if((LFSensor[0]== 0 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 ))  {error = -4;}
    //else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 ))  {mode = STOPPED; error = 0;} //revisar
    else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 ))  {goBackward();} //revisar
  }

  void calcularPID()
  {
    int P, I, D; 
    P = error;
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
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
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

    cont = 0;
    //Comprobar valores (BORRAR)
    Serial.println("Nopea está esperando el pulsador");
  }

  void goBackward() {
    //Acción del motor A
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, atras);
  
    //Acción del motor B
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, atras);
  
    //Comprobar valores (BORRAR)
    //Serial.println("Nopea se mueve hacia atras");
  }

  void testRead()
  {
       int LFS0 = digitalRead(s1);
       int LFS1 = digitalRead(s2);
       int LFS2 = digitalRead(s3);
       int LFS3 = digitalRead(s4);
       int LFS4 = digitalRead(s5);
       
       Serial.println ("Sensores: L  1 2 3 4 5  R ==> "); 
       Serial.print (LFS0); 
       Serial.print (" ");
       Serial.print (LFS1); 
       Serial.print (" ");
       Serial.print (LFS2); 
       Serial.print (" ");
       Serial.print (LFS3); 
       Serial.print (" ");
       Serial.print (LFS4); 
       Serial.print (" ");
  }
