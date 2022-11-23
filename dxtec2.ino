  /* DXTEC2 v01 */ 
  
  
  ///////////////////////////////////////////////////////// motor
  int IN1 = 9; // pin digital in1
  int IN2 = 8; // pin digital in2
  int ENB = 10; // pin digital con pwm para enbv
  ///////////////////////////////////////////////////////// pot vel
  int velocidad = 0;
  int potenciometro = 0;
  ///////////////////////////////////////////////////////// pot ang
  int angulo = 0;
  int potenciometro1 = 0;
  ///////////////////////////////////////////////////////// lcd
  #include <Wire.h> 
  #include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(0x27,16,2);  
  ///////////////////////////////////////////////////////// limite carrera 
  const int limiteCar1 = 7;
  ///////////////////////////////////////////////////////// encoder para medir pulsos de recorrido
  #include "Rotary.h";
  #define ROTARY_PIN1	4
  #define ROTARY_PIN2	5
  #define CLICKS_PER_STEP   2 
  Rotary r;
  ///////////////////////////////////////////////////////// operacion velocidad-angulo  
  int multVel;
  int multAng;
  int multRec;

  void setup()
  {
   Serial.begin(9600);
   ///////////////////////////////////////////////////////// motor setup
   pinMode (ENB, OUTPUT); 
   pinMode (IN1, OUTPUT); 
   pinMode (IN2, OUTPUT); 
   ///////////////////////////////////////////////////////// lcd setup
   lcd.init();
   lcd.backlight();
   ///////////////////////////////////////////////////////// limite carrera setup
   pinMode(limiteCar1, INPUT_PULLUP);
   ///////////////////////////////////////////////////////// encoder setup
   r.begin(ROTARY_PIN1, ROTARY_PIN2,CLICKS_PER_STEP);
   r.setChangedHandler(rotate);
   ///////////////////////////////////////////////////////// punto de partida
   puntoPartida();
  }
 
  void loop()
  {
    ///////////////////////////////////////////////////////// funcion encoder
    r.loop();
    ///////////////////////////////////////////////////////// leemos el valor del potenciómetro (0-1023)
    potenciometro = analogRead(A0);
    potenciometro1 = analogRead(A1);
    ///////////////////////////////////////////////////////// lo transformamos a velocidad (0-255)  
    velocidad =  potenciometro/4;
    ///////////////////////////////////////////////////////// aplicamos la velocidad al pin ENB del motor
    analogWrite(ENB,velocidad);
    ///////////////////////////////////////////////////////// calculos para el print
    multVel = potenciometro * 0.098;
    multAng = potenciometro1 * 0.132;
    multRec = r.getPosition() * 0.411;
    
 /*/ESTAS SON LAS LINEAS QUE MAREAN EL ENCODER

    lcd.setCursor(0, 0);
    lcd.print(String("Velocidad: ") + multVel + String("%      "));
    lcd.setCursor(0, 2);
    lcd.print(String("Angulo: ") + multAng + String(" grados     "));

/*/

    ///////////////////////////////////////////////////////// funcionamiento del carro

    if (digitalRead(limiteCar1) == LOW) {
      avance();
    }
    if (multAng < multRec) {
      retroceso();
    }
    if (r.getPosition() <= 5) {
      avance();
    }
    if (digitalRead(limiteCar1) == LOW) {
      r.resetPosition();
      r.resetPosition();
    }
  }

  void rotate(Rotary& r) {
   Serial.println(r.getPosition());
  } 

  void puntoPartida(){
    while (digitalRead(limiteCar1) != LOW) {
      analogWrite(ENB, 150);
      digitalWrite (IN1, LOW);
      digitalWrite (IN2, HIGH); 
    }
  }

  void avance() {
    analogWrite(ENB,velocidad);
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW); 
  }

  void retroceso() {
    analogWrite(ENB,velocidad);
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH); 
  }

  void stop() {
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, LOW);     
  }
