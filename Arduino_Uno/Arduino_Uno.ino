#include <LiquidCrystal_I2C.h>

#define NUM_MOV 6 //número de movimentos conhecidos
#define ANGULO_MAX 60 //ângulo máximo de compressão do fio
#define ANGULO_MIN 0  //ângulo de à vontade
#define ANGULO_MAX_P 180 //ângulo máximo de compressão do fio
#define SERVO_POS_UM 6  //pin do primeiro servo, os outros vêm de seguida, logo, SERVO_POS_UM = [1, 9]
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
  lcd.init();
  lcd.backlight();
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

  Serial.println("\nWhat would you like to do?\n1- Play Rock, Paper, Scissors\n2- Read my school card for a special move");

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
    seleciona_movimento(comp_choice);
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
    seleciona_movimento(REST);
    delay(2500);
  }
  Serial.println("Thank you for playing!\n");
  lcd.setCursor(0, 0);
  lcd.print("Thank you for");
  lcd.setCursor(0, 1);
  lcd.print("playing!");
  delay(5000);
}
