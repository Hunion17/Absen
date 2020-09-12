#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_PN532.h>
#define PN532_IRQ   (2)
#define PN532_RESET (3) 

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Alamat Mac Address
char server[] = "192.168.10.10"; //IP server
IPAddress ip(192,168,10,5); //IP arduino
EthernetClient client; //kabel utp
int lampu_merah = 8;
int lampu_hijau = 9;
String UID_CARD;
String attendance_status;
String user_card;
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3 ,POSITIVE);
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
uint8_t data;
void setup() {
  pinMode(lampu_merah,OUTPUT);
  pinMode(lampu_hijau,OUTPUT);
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("    ABSEN UG    ");
  lcd.setCursor(0,1);
  lcd.print(" SELAMAT DATANG ");
  delay(5000);
  lcd.clear();
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  nfc.SAMConfig();
  
  Serial.println("Mulai deteksi kartu ISO14443A...");
}


void loop(void) {
uint8_t terbaca;
uint8_t uid[] = { 0, 0, 0, 0 };  
uint8_t uidLength;      
lcd.setCursor(0,0);
lcd.print(" TEMPELKAN KARTU ");
terbaca = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (terbaca) {
    Serial.println("Kartu ISO14443A terdeteksi ");
    Serial.print("  Panjang byte U-ID : ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  Nilai U-ID: ");
    nfc.PrintHex(uid, uidLength);
    uint32_t szUid;
    String uidData;
    for (szUid=0; szUid < uidLength; szUid++){
          uidData += String(uid[szUid]&0xFF, HEX);
    }
    uidData.toUpperCase(); //UI-D di jadikan string dan di jadiin huruf besar
    Serial.println(uidData);
    if (uidData == "BC4AA4CE" ){//pake database di sini
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("   BERHASIL   ");
    digitalWrite(lampu_hijau,HIGH);
    delay(3000);
    digitalWrite(lampu_hijau,LOW);
    lcd.clear();
 }
 else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("      GAGAL   ");
    lcd.setCursor(0,1);
    lcd.print("    COBA LAGI  ");
    digitalWrite(lampu_merah,HIGH);
    delay(3000);
    digitalWrite(lampu_merah,LOW);
    lcd.clear();
  }
      }
   else{
    Serial.println("not found card");
   }
}

void inputDB(){
 
}
