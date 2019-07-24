#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define NUM_MOV 5 //numero de movimentos conhecidos
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
bool get_card();

/* LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)*/
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Wire.begin();         //Transmissão
  Serial.begin(9600);
  SPI.begin();          //Leitura cartão
  mfrc522.PCD_Init();
<<<<<<< HEAD
  randomSeed(analogRead(0));
  lcd.begin(16, 2);
=======
>>>>>>> b686bf82888a4c7c5dbcad2199e4fdf38164df48
}

String carmen_card = "94 92 AE CB", joao_card = "15 55 89 92";
byte movimento;
<<<<<<< HEAD
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
=======
String player_name = "";
String option = "";

void menu() {
  Serial.println("----- Welcome to the Bionic Arm! -----\n What's your name?");

  while (!Serial.available() && player_name == "") {
    player_name = Serial.readString();
  }
  Serial.print("Hello, ");
  Serial.print(player_name);

  Serial.println("What would you like to do?\n1- Play Rock, Paper, Scissors\n2- Read my school card for a special move");
>>>>>>> b686bf82888a4c7c5dbcad2199e4fdf38164df48

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
<<<<<<< HEAD
  game_over = 0, comp_score = 0, player_score = 0;
  Serial.println("Let the games begin!");

  while (!game_over) {
    option = "";
    comp_choice = random(3);

    for (int i = 3; i != 0;  i--) {
      Serial.println(i);
      lcd.setCursor(0, 0);
      lcd.print("Ready?");
      lcd.setCursor(4, 1);
      lcd.print(i);
      delay(1500);
    }
    seleciona_movimento(comp_choice);

    if (comp_choice == 0) {
      lcd.setCursor(0, 0);
      lcd.print("Computer chose");
      lcd.setCursor(0, 1);
      lcd.print("Rock");
    }
    if (comp_choice == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Computer chose");
      lcd.setCursor(0, 1);
      lcd.print("Paper");
    }
    if (comp_choice == 2) {
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
      delay(5000);
    }
    else if (player_score == 3) {
      Serial.println("\n----- Congratulations! You win! -----");
      lcd.setCursor(0, 0);
      lcd.print("Congratulations!");
      lcd.setCursor(2, 1);
      lcd.print("You win!");
      
      game_over = 1;
      delay(5000);
    }
  }
  Serial.println("Thank you for playing!\n");
  lcd.setCursor(0, 0);
  lcd.print("Thank you for");
  lcd.setCursor(0, 1);
  lcd.print("playing!");
  delay(5000);
=======
  Serial.println("Game");
>>>>>>> b686bf82888a4c7c5dbcad2199e4fdf38164df48
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
  while (!get_card()) {
    Serial.print("UID tag :");
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
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
      delay(500);
    }
<<<<<<< HEAD

=======
    
>>>>>>> b686bf82888a4c7c5dbcad2199e4fdf38164df48
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
