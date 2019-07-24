#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define NUM_MOV 5
#define SS_PIN 10
#define RST_PIN 9
#define REST 0
#define ROCK 1
#define PAPER 2
#define SCISSORS 3
#define JOAO 4
#define CARMEN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);
bool get_card();

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Wire.begin();         //Transmissão
  Serial.begin(9600);
  SPI.begin();          //Leitura cartão
  mfrc522.PCD_Init();
  randomSeed(analogRead(0));
  lcd.begin(16, 2);
}

String carmen_card = "8D 29 91 B9", joao_card = "15 55 89 92";
byte movimento;
String player_name = "", option = "";
int game_over, comp_score, player_score, comp_choice;

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
    read_card();
  }
  option = "";
  player_name = "";
}

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
    Serial.print("\n2- Computer");
    while (!Serial.available() && (option.charAt(0) != '1' || option.charAt(0) != '2') && option == "") {
      option = Serial.readString();
      if (option.charAt(0) != '1' && option.charAt(0) != '2' && option != "") {
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
    delay(2500);
    seleciona_movimento(REST);
  }
  Serial.println("Thank you for playing!\n");
  lcd.setCursor(0, 0);
  lcd.print("Thank you for");
  lcd.setCursor(0, 1);
  lcd.print("playing!");
  delay(5000);
}

bool get_card() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) { // Look for new cards
    return false;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {   // Select one of the cards
    return false;
  }
  return true;
}

void read_card() {
  Serial.println("Put your card to the reader, please...");
  while (!get_card());
  Serial.print("UID tag :");
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));

    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();
    if (content.substring(1) == carmen_card || content.substring(1) == joao_card) {
      Serial.println("Authorized access");
      String person = content.substring(1) == carmen_card ? "Carmen" : "Joao" ;
      Serial.print("Hi ");
      Serial.println(person);
      Serial.println();
      lcd.setCursor(0, 0);
      lcd.print("Hello,");
      lcd.setCursor(0, 1);
      lcd.print(person);

      if (person == "Carmen") {
        seleciona_movimento(CARMEN);
      }
      else if (person == "Joao") {
        seleciona_movimento(JOAO);
      }

      delay(500);

    }

    Serial.println(" Access denied");
  }
}

void seleciona_movimento(byte num) {
  Wire.beginTransmission(8);
  Wire.write(num);
  Wire.endTransmission();
}

void loop() {
  menu();
}
