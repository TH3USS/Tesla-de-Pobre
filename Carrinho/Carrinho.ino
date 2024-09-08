#include <AFMotor.h>
#include <Servo.h> //importando a biblioteca do Servo
#include <Ultrasonic.h> //importando a biblioteca do Sonar

// definindo variáveis

int servo = 10; // servomotor ligado em uma porta digital PWM
int trig = 13;   //pino TRIG
int echo = 2;   //pino ECHO
int distancia;  //variável pra guradar a distancia
AF_DCMotor motor1(2);
AF_DCMotor motor2(4);
int pot = 80;  // potência do motor de 0 a 255
int tempo;      // duração do sinal enviado do sensor ultrassônico
int F, E, D;    // distâncias medidas da frente (90°), da direita (30°) e esquerda (150°)
Ultrasonic sonar(trig, echo);//objeto do tipo Ultrasonic
Servo motorzinho; //criando um Servo chamado motorzinho

void setup() {

  Serial.begin(9600);
  motor1.setSpeed(0); // 0  a 255
  motor2.setSpeed(0); // 0  a 255
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);

  motorzinho.attach(servo); //Servo motor no pino 10
  motorzinho.write(90);  // inicia olhando para frente (ângulo de 90°)
  delay(2000);
}

void loop() {
  // inicia com os motores parados
  motor_parar();
  motorzinho.write(0);          // gira para direita (ângulo de 30°)
  delay(1000);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  tempo = pulseIn(echo, HIGH);
  // calcula a distância a partir do tempo de ida e volta do ultrassom com v = 345 m/s
  D = tempo/58;          
  motorzinho.write(180);       // gira para esquerda (ângulo de 150°)
  delay(1500);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  tempo = pulseIn(echo, HIGH);
  E = tempo/58;  
  motorzinho.write(90);       // gira para frente (ângulo de 90°)
  delay(1000);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  tempo = pulseIn(echo, HIGH);
  F = tempo/58;  

  if (F > 30) {                                  
    if ((D > 20) && (E > 20)) {   // o carrinho vai para frente desde que a 
      motor_frente();             // distância ao obstáculo for maior que 30 cm
    }
    else {
      if (D > E) {                              
        // o carrinho gira para a direita
        motor_giraD();
      }
      else {                                       
        // o carrinho gira para a esquerda
        motor_giraE();
      }
    }
  }
  else {                               
    // se a distância for menor do que 30 cm, então ele dá ré e gira 
    motor_re();
  } 
}

void motor_frente() {
  motor1.setSpeed(pot);
  motor1.run(BACKWARD); // sentido horário
  motor2.setSpeed(pot);
  motor2.run(BACKWARD); // sentido horário
  delay(2000);
}

void motor_parar() {
  motor1.setSpeed(0);
  motor1.run(RELEASE);
  motor2.setSpeed(0);
  motor2.run(RELEASE);
  delay(1000);
}

void motor_re() {
  motor1.setSpeed(pot);
  motor1.run(FORWARD); // sentido anti-horário
  motor2.setSpeed(pot);
  motor2.run(FORWARD); // sentido anti-horário
  delay(1000);
}

void motor_giraD() {
  motor1.setSpeed(pot);
  motor1.run(FORWARD); // sentido anti-horário
  motor2.setSpeed(0);
  motor2.run(RELEASE);
  delay(1000);
}

void motor_giraE() {
  motor1.setSpeed(0);
  motor1.run(RELEASE);
  motor2.setSpeed(pot);
  motor2.run(FORWARD); // sentido anti-horário
  delay(1000);
}
