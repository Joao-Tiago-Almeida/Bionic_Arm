#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define NUM_MOV 5 //numero de movimentos conhecidos
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
bool get_card();

void setup() {
  Wire.begin();         //Transmissão
  Serial.begin(9600);
  SPI.begin();          //Leitura cartão
  mfrc522.PCD_Init();
}

String carmen_card = "94 92 AE CB";
String joao_card = "15 55 89 92";
byte movimento;
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
  Serial.println("Game");
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
