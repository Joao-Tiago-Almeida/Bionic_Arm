#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define NUM_MOV 4 //numero de movimentos conhecidos
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
bool get_card();

void setup() {
  Wire.begin();         //Transmissão
  Serial.begin(9600);
  SPI.begin();          //Leitura cartão
  mfrc522.PCD_Init();
  Serial.println("Put your card to the reader...");
  Serial.println();
}

String carmen_card = "94 92 AE CB";
String joao_card = "15 55 89 92";
byte movimento;

bool get_card() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) { // Look for new cards
    return false;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {   // Select one of the cards
    return false;
  }
  return true;
}

void seleciona_movimento(byte num) {
  Wire.beginTransmission(8);
  Wire.write(num);
  Wire.endTransmission();
}

void loop() {
  movimento = 0;

  if (get_card()) {
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

  while (movimento < NUM_MOV) {
    seleciona_movimento(movimento);
    Serial.println(movimento);
    movimento ++;
    delay(1000);
  }
  delay(1000);
}
