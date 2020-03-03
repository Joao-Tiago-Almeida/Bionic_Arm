#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define NUM_MOV 6 //número de movimentos conhecidos
#define ANGULO_MAX 60 //ângulo máximo de compressão do fio
#define ANGULO_MIN 0  //ângulo de à vontade
#define ANGULO_MAX_P 180 //ângulo máximo de compressão do fio
#define SERVO_POS_UM 12  //pin do polegar, os outros são a decrementar
#define DELAY 0 //tempo de espera para rotacao dos servos

#define NUM_MOV 5
#define REST 0
#define ROCK 1
#define PAPER 2
#define SCISSORS 3

String player_name = "", option = "";
int game_over, comp_score, player_score, comp_choice;
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  lcd.begin (16,2);
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  attach_servos();
  init_mao();
}

/*---------- MOVIMENTOS MÃO ----------*/
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
vect_funcoes_void vect_movimentos_mao[NUM_MOV] = {&descanso, &pedra, &papel, &tesoura};  //vetor com funções que representão os movimentos conhecidos
Servo s_polegar, s_indicador, s_medio, s_anelar, s_mindinho, s_pulso;
int movimento = 0;

// movimentos conhecidos
void pedra() {
  angulos_mao(ANGULO_MAX, ANGULO_MAX, ANGULO_MAX, ANGULO_MAX, ANGULO_MAX, ANGULO_MAX_P / 2);
  Serial.println("*Arm chooses rock*");
}
void papel() {
  angulos_mao(ANGULO_MIN, ANGULO_MIN, ANGULO_MIN, ANGULO_MIN, ANGULO_MIN, ANGULO_MAX_P / 2);
  Serial.println("*Arm chooses paper*");
}
void tesoura() {
  angulos_mao(ANGULO_MAX, ANGULO_MIN, ANGULO_MIN, ANGULO_MAX, ANGULO_MAX, ANGULO_MAX_P / 2);
  Serial.println("*Arm chooses scissors*");
}
void descanso() {
  angulos_mao(ANGULO_MAX / 2 -10, ANGULO_MAX / 2 -10, ANGULO_MAX / 2 -10, ANGULO_MAX / 2 -10, ANGULO_MAX / 2 -10, ANGULO_MAX_P / 2);
  Serial.println("*Arm at rest*");
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
  vect_movimentos_mao[0]();
}

//funções relacionadas com os servos
void attach_servos() {
  s_polegar.attach(SERVO_POS_UM);
  s_indicador.attach(SERVO_POS_UM - 1);
  s_medio.attach(SERVO_POS_UM - 2);
  s_anelar.attach(SERVO_POS_UM - 3);
  s_mindinho.attach(SERVO_POS_UM - 4);
  s_pulso.attach(SERVO_POS_UM - 5);
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

void move_hand(int mov){
  if (mov >= 0 && mov < NUM_MOV)
    vect_movimentos_mao[mov]();
  }

void loop() {
  menu();
}

/*---------- MENU PRINCIPAL ----------*/
void menu() {
  Serial.println("----- Welcome to the Bionic Arm! -----\nWhat's your name?");
  lcd.setCursor(0, 0);
  lcd.print("Welcome to the");
  lcd.setCursor(0, 1);
  lcd.print("Bionic Arm!");
  while (!Serial.available() && player_name == "") {
    player_name = Serial.readString();
  }
  int len = player_name.length();
  player_name[len - 1] = '\0';
  Serial.print("Hello, ");
  Serial.print(player_name);
  lcd.setCursor(0, 0);
  lcd.print("Hello,");
  lcd.setCursor(0, 1);
  lcd.print(player_name);

  Serial.println("\nWhat would you like to do?\n1- Play Rock, Paper, Scissors\n2- Option unavailable at the moment");

  while (!Serial.available() && (option.charAt(0) != '1' || option.charAt(0) != '2') && option == "") {
    option = Serial.readString();
    if (option.charAt(0) != '1' && option.charAt(0) != '2' && option != "") {
      Serial.println("Sorry, that's not a valid option! Try again.");
      option = "";
    }
  }

  if (option.charAt(0) == '1') {
    game();
  }
  else if (option.charAt(0) == '2') {
  
  }
  option = "";
  player_name = "";
}

/*---------- PEDRA, PAPEL OU TESOURA ----------*/
void game() {
  game_over = 0, comp_score = 0, player_score = 0;
  Serial.println("Let the games begin!");

  while (!game_over) {
    option = "";
    comp_choice = random(1, 4);

    for (int i = 3; i != 0;  i--) {
      Serial.println(i);
      lcd.setCursor(0, 0);
      lcd.print("Ready?");
      lcd.setCursor(4, 1);
      lcd.print(i);
      delay(1500);
    }
    move_hand(comp_choice);
    delay(1500);

    if (comp_choice == ROCK) {
      lcd.setCursor(0, 0);
      lcd.print("Computer chose");
      lcd.setCursor(0, 1);
      lcd.print("Rock");
    }
    if (comp_choice == PAPER) {
      lcd.setCursor(0, 0);
      lcd.print("Computer chose");
      lcd.setCursor(0, 1);
      lcd.print("Paper");
    }
    if (comp_choice == SCISSORS) {
      lcd.setCursor(0, 0);
      lcd.print("Computer chose");
      lcd.setCursor(0, 1);
      lcd.print("Scissors");
    }

    Serial.print("Who won?\n1- ");
    Serial.print(player_name);
    Serial.print("\n2- Computer\n3- Draw");
    while (!Serial.available() && (option.charAt(0) != '1' || option.charAt(0) != '2' || option.charAt(0) != '3') && option == "") {
      option = Serial.readString();
      if (option.charAt(0) != '1' && option.charAt(0) != '2' && option.charAt(0) != '3' && option != "") {
        Serial.println("Sorry, that's not a valid option! Try again.");
      }
    }
    if (option.charAt(0) == '1') {
      player_score ++;
    }
    else if (option.charAt(0) == '2') {
      comp_score ++;
    }

    Serial.print("\n");
    Serial.print(player_name);
    Serial.print(" Score: ");
    Serial.print(player_score);
    Serial.print("\n");
    Serial.print("Robotic Arm's Score: ");
    Serial.print(comp_score);
    Serial.print("\n");

    if (comp_score == 3) {
      Serial.println("\n----- Looks like the Robotic Arm is the winner! Better luck next time! -----");
      lcd.setCursor(0, 0);
      lcd.print("Sorry, you lost!");
      lcd.setCursor(8, 1);
      lcd.print(":(");

      game_over = 1;
    }
    else if (player_score == 3) {
      Serial.println("\n----- Congratulations! You win! -----");
      lcd.setCursor(0, 0);
      lcd.print("Congratulations!");
      lcd.setCursor(2, 1);
      lcd.print("You win!");

      game_over = 1;
    }
    move_hand(REST);
    delay(2500);
  }
  Serial.println("Thank you for playing!\n");
  lcd.setCursor(0, 0);
  lcd.print("Thank you for");
  lcd.setCursor(0, 1);
  lcd.print("playing!");
  delay(5000);
}
