#include <Servo.h>
#include <Wire.h>

#define NUM_MOV 5 //número de movimentos conhecidos
#define ANGULO_MAX 60 //ângulo máximo de compressão do fio
#define ANGULO_MIN 0  //ângulo de à vontade
#define ANGULO_MAX_P 180 //ângulo máximo de compressão do fio
#define SERVO_POS_UM 6  //pin do primeiro servo, os outros vêm de seguida, logo, SERVO_POS_UM = [1, 9]
#define DELAY 0 //tempo de espera para rotacao dos servos
#define ENDERECO 8  //linha de informação de master-slave

//vetor de funções void
typedef void (*vect_funcoes_void)();

// estrutura dos servos da mão
typedef struct {
  int polegar;
  int indicador;
  int medio;
  int anelar;
  int mindinho;
  int pulso;
} MAO;

// declarações de variáveis globais
MAO mao;  //estrura que contem os dedos da mao
vect_funcoes_void vect_movimentos_mao[NUM_MOV] = {&relax_mao, &pedra, &papel, &tesoura, &descanso};  //vetor com funções que representão os movimentos conhecidos
Servo s_polegar, s_indicador, s_medio, s_anelar, s_mindinho, s_pulso;
int *movimento = 0;

// movimentos conhecidos
void pedra() {
  angulos_mao(ANGULO_MAX, ANGULO_MAX, ANGULO_MAX, ANGULO_MAX, ANGULO_MAX, ANGULO_MAX_P / 2);
}
void papel() {
  angulos_mao(ANGULO_MIN, ANGULO_MIN, ANGULO_MIN, ANGULO_MIN, ANGULO_MIN, ANGULO_MAX_P / 2);
}
void tesoura() {
  angulos_mao(ANGULO_MAX, ANGULO_MIN, ANGULO_MIN, ANGULO_MAX, ANGULO_MIN, ANGULO_MAX_P / 2);
}
void descanso() {
  angulos_mao(ANGULO_MAX / 2, ANGULO_MAX / 2, ANGULO_MAX / 2, ANGULO_MAX / 2, ANGULO_MAX / 2, ANGULO_MAX_P / 2);
}

//funções auxiliares
void angulos_mao(int p, int i, int M, int a, int m , int P) {
  m += 10;
  int Maximo = max_value(p, i, M, a, m, P);
  for (int count = ANGULO_MIN; count < Maximo || count == ANGULO_MIN; count ++) {
    //recebe o movimento dependente se o angulo aumenta ou diminui, de cada sevo
    //o ângulo aumenta
    if (mao.polegar < p) mao.polegar++;
    //o ângulo diminui
    else if (mao.polegar > p)  mao.polegar--;
    if (mao.indicador < i) mao.indicador++;
    else if (mao.indicador > i)  mao.indicador--;
    if (mao.medio < M) mao.medio++;
    else if (mao.medio > M) mao.medio--;
    if (mao.anelar < a) mao.anelar++;
    else if (mao.anelar > a) mao.anelar--;
    if (mao.mindinho < m) mao.mindinho++;
    else if (mao.mindinho > m) mao.mindinho--;
    //if(mao.pulso < P) mao.pulso++;
    //else if(mao.pulso > P) mao.pulso--;
    mao.pulso = P;
    //info_mao();
    servos_mao();
  }
}
int max_value(int p, int i, int M, int a, int m, int P) {
  p = abs( mao.polegar - p);
  i = abs(mao.indicador - i);
  M = abs(mao.medio - M);
  a = abs(mao.anelar - a);
  m = abs(mao.mindinho - m);
  int aa = max(p, i);
  int bb = max(M, a);
  int cc = max(aa, bb);
  return max(cc, m);

}
void init_mao() {

  vect_movimentos_mao[2]();
  vect_movimentos_mao[0]();
  vect_movimentos_mao[2]();

}
void relax_mao() {
  int times = 1;
  for (int i = 0; i <  times * 5; i++) {
    angulos_mao(ANGULO_MAX * (((i + 5) % 5) == 0), ANGULO_MAX * (((i + 4) % 5) == 0), ANGULO_MAX * (((i + 3) % 5) == 0), ANGULO_MAX * (((i + 2) % 5) == 0), ANGULO_MAX * (((i + 1) % 5) == 0), ANGULO_MAX_P / 2);

  }
}
void info_mao() {
  Serial.print("Polegar: ");
  Serial.println(mao.polegar);
  Serial.print("Indicador: ");
  Serial.println(mao.indicador);
  Serial.print("Médio: ");
  Serial.println(mao.medio);
  Serial.print("Anelar: ");
  Serial.println(mao.anelar);
  Serial.print("Mindinho: ");
  Serial.println(mao.mindinho);
  Serial.print("Pulso: ");
  Serial.println(mao.pulso);
  Serial.println();
}

//atua quando a mão está pronta a ser utilizada
void servo_ready(){
  pinMode(13, OUTPUT);
  for(int i = 1; i <= NUM_MOV; i++){
    digitalWrite(13, HIGH);
    delay(NUM_MOV/i);
    digitalWrite(13, LOW);
    delay(NUM_MOV/i);
  }
}


//funções relacionadas com os servos
void attach_servos() {
  /*s_polegar.attach(SERVO_POS_UM, ANGULO_MIN, ANGULO_MAX);
    s_indicador.attach(SERVO_POS_UM + 1, ANGULO_MIN, ANGULO_MAX);
    s_medio.attach(SERVO_POS_UM + 2, ANGULO_MIN, ANGULO_MAX);*/

  s_polegar.attach(SERVO_POS_UM);
  s_indicador.attach(SERVO_POS_UM + 1);
  s_medio.attach(SERVO_POS_UM + 2);
  s_anelar.attach(SERVO_POS_UM + 3);
  s_mindinho.attach(SERVO_POS_UM + 4);
  s_pulso.attach(SERVO_POS_UM + 5);
}
void detach_servos() {
  s_polegar.detach();
  s_indicador.detach();
  s_medio.detach();
  s_anelar.detach();
  s_mindinho.detach();
  s_pulso.detach();
}
void servos_mao() {
  if (s_polegar.read() != mao.polegar) {
    s_polegar.write(mao.polegar);
  }
  if (s_indicador.read() != mao.indicador) {
    s_indicador.write(mao.indicador);

  }
  if (s_medio.read() != mao.medio) {
    s_medio.write(mao.medio);
  }
  if (s_anelar.read() != mao.anelar) {
    s_anelar.write(mao.anelar);
  }
  if (s_mindinho.read() != mao.mindinho) {
    s_mindinho.write(mao.mindinho);
  }
  if (s_pulso.read() != mao.pulso) {
    s_pulso.write(mao.pulso);
  }
}

//interrupção para receber informação do master
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read();        // receive byte as a character
  }
  *movimento = (int)Wire.read();

  if (*movimento >= 0 && *movimento < NUM_MOV)
    vect_movimentos_mao[*movimento]();
}

void setup() {
  Wire.begin(ENDERECO);

  Wire.onReceive(receiveEvent);
  Serial.begin(9600);

  attach_servos();
  init_mao();
  servo_ready();
  //detach_servos();


}

void loop() {
  

}
