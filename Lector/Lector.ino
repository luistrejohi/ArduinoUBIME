#include <avr/wdt.h>
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 9 // Configurable, permite ver el diseño y configuracion del pin
#define SS_PIN 10 // Configurable, permite ver el diseño y configuracion del pin
String option;
int x;
MFRC522 mfrc522(SS_PIN, RST_PIN); // Cre MFRC522 

MFRC522::MIFARE_Key key;

//Inicio


void softwareReset( uint8_t prescaller) {
  // start watchdog with the provided prescaller
  wdt_enable( prescaller);
  // wait for the prescaller time to expire
  // without sending the reset signal by using
  // the wdt_reset() method
  while(1) {}
}


void setup() {
Serial.begin(9600); //Inicia la cominicacion con la computadora
while (!Serial); // condicion
SPI.begin(); //  SPI bus
mfrc522.PCD_Init(); // MFRC522 card

// Usando FFFFFFFFFFFFh Predeterminado por la placa o llavero
for (byte i = 0; i < 6; i++) {
key.keyByte[i] = 0xFF;
}



Serial.println(F("Ingrese Tarjeta"));
}


void loop(){
  
// Look for new cards
if ( ! mfrc522.PICC_IsNewCardPresent())
return;

// Select one of the cards
if ( ! mfrc522.PICC_ReadCardSerial())
return;

byte sector = 1;
byte blockAddr = 6; //Se especifica que bloque se leera _____------______-----______
byte dataBlock[] = {
0x01, 0x02, 0x03, 0x04, // 1, 2, 3, 4,
0x05, 0x06, 0x07, 0x08, // 5, 6, 7, 8,
0x08, 0x09, 0xff, 0x0b, // 9, 10, 255, 12,
0x0c, 0x0d, 0x0e, 0x0f // 13, 14, 15, 16
};
byte trailerBlock = 7;
MFRC522::StatusCode status;
byte buffer[18];
byte size = sizeof(buffer);

mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);

// lee la data del bloque
Serial.print(F("Cargando dato del bloque "));
Serial.println(F(" ..."));
status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);

dump_byte_array(buffer, 16);

delay(3000);
softwareReset( WDTO_60MS);

   }



/**
* 
*/
void dump_byte_array(byte *buffer, byte bufferSize) {
  Serial.write('|');
  
for (byte i = 0; i < bufferSize; i++) {
// Serial.print(buffer[i] < 0x10 ? " 0" : " ");
// Serial.print(buffer[i], HEX);

Serial.write(buffer[i]);
}
}
