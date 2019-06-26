#include <Servo.h>

#define NUM_MOV 3 //número de movimentos conhecidos
#define ANGULO_MAX 90 //ângulo máximo de compressão do fio
#define ANGULO_MIN 0  //ângulo de à vontade
#define SERVO_POS_UM 6  //pin do primeiro servo, os outros vêm de seguida, logo, SERVO_POS_UM = [1, 9]
#define DELAY 500 //tempo de espera para rotacao dos servos

//vetor de funções void
typedef void (*vect_funcoes_void)();

// estrutura dos servos da mão
typedef struct {
  int polegar;
  int indicador;
  int medio;
  int anelar;
  int mindinho;
} MAO;

// declarações de variáveis globais
MAO mao;  //estrura que contem os dedos da mao
vect_funcoes_void vect_movimentos_mao[NUM_MOV] = {&pedra, &papel, &tesoura};  //vetor com funções que representão os movimentos conhecidos
Servo s_polegar, s_indicador, s_medio, s_anelar, s_mindinho;

// movimentos conhecidos
void pedra() {
  angulos_mao(ANGULO_MAX, ANGULO_MAX, ANGULO_MAX, ANGULO_MAX, ANGULO_MAX);
}
void papel() {
  angulos_mao(ANGULO_MIN, ANGULO_MIN, ANGULO_MIN, ANGULO_MIN, ANGULO_MIN);
}
void tesoura() {
  angulos_mao(ANGULO_MAX, ANGULO_MIN, ANGULO_MIN, ANGULO_MAX, ANGULO_MAX);
}

//funções auxiliares
void angulos_mao(int p, int i, int M, int a, int m) {
  mao.polegar = p;
  mao.indicador = i;
  mao.medio = M;
  mao.anelar = a;
  mao.mindinho = m;
}
void init_mao() {
  papel();
  relax_mao();
  papel();
  
}
void relax_mao() {
  for (int i = 0; i < 1 * 5; i++) {
    angulos_mao(ANGULO_MAX * (((i + 5) % 5) == 0), ANGULO_MAX * (((i + 4) % 5) == 0), ANGULO_MAX * (((i + 3) % 5) == 0), ANGULO_MAX * (((i + 2) % 5) == 0), ANGULO_MAX * (((i + 1) % 5) == 0));
    delay(200);
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
  Serial.println();
}

//funções relacionadas com os servos
void attach_servos() {
  /*s_polegar.attach(SERVO_POS_UM, ANGULO_MIN, ANGULO_MAX);
  s_indicador.attach(SERVO_POS_UM + 1, ANGULO_MIN, ANGULO_MAX);
  s_medio.attach(SERVO_POS_UM + 2, ANGULO_MIN, ANGULO_MAX);
  s_anelar.attach(SERVO_POS_UM + 3, ANGULO_MIN, ANGULO_MAX);
  s_mindinho.attach(SERVO_POS_UM + 4, ANGULO_MIN, ANGULO_MAX);*/

  s_polegar.attach(SERVO_POS_UM);
  s_indicador.attach(SERVO_POS_UM + 1);
  s_medio.attach(SERVO_POS_UM + 2);
  s_anelar.attach(SERVO_POS_UM + 3);
  s_mindinho.attach(SERVO_POS_UM + 4);
}
void detach_servos() {
  s_polegar.detach();
  s_indicador.detach();
  s_medio.detach();
  s_anelar.detach();
  s_mindinho.detach();
}
void servos_mao() {
  if (s_polegar.read() != mao.polegar) {
    s_polegar.write(mao.polegar);
    delay(DELAY);
  }
  if (s_indicador.read() != mao.indicador) {
    s_indicador.write(mao.indicador);
    delay(DELAY);
  }
  if (s_medio.read() != mao.medio) {
    s_medio.write(mao.medio);
    delay(DELAY);
  }
  if (s_anelar.read() != mao.anelar) {
    s_anelar.write(mao.anelar);
    delay(DELAY);
  }
  if (s_mindinho.read() != mao.mindinho) {
    s_mindinho.write(mao.mindinho);
    delay(DELAY);
  }
}


void setup() {
  Serial.begin(9600);

  attach_servos();
  init_mao();
  detach_servos();
}

void loop() {

}
