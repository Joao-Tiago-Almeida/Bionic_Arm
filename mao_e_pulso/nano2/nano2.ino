#include <Servo.h>
#include <Wire.h>

#define NUM_MOV 4 //número de movimentos conhecidos
#define ANGULO_MAX 90 //ângulo máximo de compressão do fio
#define ANGULO_MAX_P 180 //ângulo máximo de compressão do fio
#define ANGULO_MIN 0  //ângulo de à vontade
#define SERVO_POS_UM 6  //pin do primeiro servo, os outros vêm de seguida, logo, SERVO_POS_UM = [1, 9]
#define DELAY 500 //tempo de espera para rotacao dos servos
#define ENDERECO 8  //linha de informação de master-slave

//vetor de funções void
typedef void (*vect_funcoes_void)();

// estrutura dos servos da mão
typedef struct {
  int anelar;
  int mindinho;
  int pulso;
} MAO;

// declarações de variáveis globais
MAO mao;  //estrura que contem os dedos da mao
vect_funcoes_void vect_movimentos_mao[NUM_MOV] = {&relax_mao, &pedra, &papel, &tesoura};  //vetor com funções que representão os movimentos conhecidos
Servo s_medio, s_anelar, s_mindinho, s_pulso;
int *movimento = 0;

// movimentos conhecidos
void pedra() {
  angulos_mao(ANGULO_MAX, ANGULO_MAX, ANGULO_MAX_P / 2);
}
void papel() {
  angulos_mao(ANGULO_MIN, ANGULO_MIN, ANGULO_MAX_P / 2);
}
void tesoura() {
  angulos_mao(ANGULO_MAX, ANGULO_MIN, ANGULO_MAX_P / 2);
}

//funções auxiliares
void angulos_mao(int p, int i, int M) {
  mao.anelar = p;
  mao.mindinho = i;
  mao.pulso = M;
  info_mao();
  //servos_mao();
}
void init_mao() {

  vect_movimentos_mao[2]();
  vect_movimentos_mao[0]();
  vect_movimentos_mao[2]();

}
void relax_mao() {
  int times = 1;
  for (int i = 0; i <  times * 5; i++) {
    angulos_mao(ANGULO_MAX * (((i + 2) % 5) == 0), ANGULO_MAX * (((i + 1) % 5) == 0), ANGULO_MAX_P / 2);
    delay(DELAY);
    if (i != times * 3 || i != times * 4) {
      delay(DELAY);
    }
  }
}
void info_mao() {
  Serial.print("Anelar: ");
  Serial.println(mao.anelar);
  Serial.print("Mindinho: ");
  Serial.println(mao.mindinho);
  Serial.print("Pulso: ");
  Serial.println(mao.pulso);
  Serial.println();
}


//funções relacionadas com os servos
void attach_servos() {
  /*s_polegar.attach(SERVO_POS_UM, ANGULO_MIN, ANGULO_MAX);
    s_indicador.attach(SERVO_POS_UM + 1, ANGULO_MIN, ANGULO_MAX);
    s_medio.attach(SERVO_POS_UM + 2, ANGULO_MIN, ANGULO_MAX);*/

  s_anelar.attach(SERVO_POS_UM);
  s_mindinho.attach(SERVO_POS_UM + 1);
  s_pulso.attach(SERVO_POS_UM + 2);
}
void detach_servos() {
  s_anelar.detach();
  s_mindinho.detach();
  s_pulso.detach();
}
void servos_mao() {
  if (s_anelar.read() != mao.anelar) {
    s_anelar.write(mao.anelar);
    delay(DELAY);
  }
  if (s_mindinho.read() != mao.mindinho) {
    s_mindinho.write(mao.mindinho);
    delay(DELAY);
  }
  if (s_pulso.read() != mao.pulso) {
    s_pulso.write(mao.pulso);
    delay(DELAY);
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
  //init_mao();
  //detach_servos();
}

void loop() {

}
