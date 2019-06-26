#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String carmen_card = "94 92 AE CB";
String joao_card = "15 55 89 92";

bool get_card();

void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Put your card to the reader...");
  Serial.println();

}

void loop()
{
  while(! get_card());
  
  //-------------------------------------------------------------------
  //         TODO retirar
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  //-------------------------------------------------------------------
  
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == carmen_card || content.substring(1) == joao_card) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    String person = content.substring(1) == carmen_card? "Carmen" : "Joao" ;
    Serial.print("Hi ");
    Serial.println(person);
    Serial.println();
    delay(500);
  }

  else   {
    Serial.println(" Access denied");
  }
}
bool get_card() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return false;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return false;
  }
  return true;
}
