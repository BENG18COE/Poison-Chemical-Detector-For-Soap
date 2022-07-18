#include <common.h>
#include <Firebase.h>
#include <FirebaseESP8266.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 
FirebaseData firebaseData;

// defines pins numbers for HC04
const int trigPin = D4;  //D?
const int echoPin = D5;  //D? 

#define FIREBASE_HOST "smartpark-5faeb-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "uVOqjtL8bPeihMn10SO9ttGNsZARhcMPMM8YF3OH"
#define WIFI_SSID "Mugaya"
#define WIFI_PASSWORD "12345670"

// defines variables for HC04 Signal
long duration;
float distance;

// defines variables for temperature and gas Signal
float gas;
float temp;
float sensor;

//define LED
  const int GRE = D7;
  const int YEL = D7;

  //define BUZ and PUMP
  const int BUZ = D0;
  const int MQ = A0;
  const int PUMP = D6;
  String smstext;

float alcohol=0;
float benzene=0;
float formal=0;
unsigned long last_time, now_time;
const long interval = 10000;
unsigned long previousMillis = 0;
void setup() {

  //iniate LCD
  //Wire.begin(2,0);
  lcd.init();                 //Init the LCD
  lcd.backlight();            //Activate backlight     
  lcd.home();
 

  //initiate HC04
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(GRE, OUTPUT);
  pinMode(YEL, OUTPUT);
  pinMode(BUZ, OUTPUT);
  pinMode(MQ, INPUT);
  pinMode(PUMP, OUTPUT);
  Serial.begin(9600); // Starts the serial communication

  lcd.clear();
  lcd.print("COSMETICS QC");
  delay(1000);
  lcd.clear();


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  connecting  ");
  while (WiFi.status() != WL_CONNECTED)
  {
    lcd.setCursor(0, 1);
    lcd.print(".");
    delay(30);
    lcd.print("..");
    delay(30);
    lcd.print("...");
    delay(30);
    lcd.print("....");
    delay(30);
    lcd.print(".....");
    delay(30);
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Connected with IP:  ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  smstext=Firebase.setString(firebaseData, "/sms/send", "Drainage Cleaner");
  delay(1000);
  smstext=Firebase.getString(firebaseData, "/sms/send")? firebaseData.to<const char *>() : firebaseData.errorReason().c_str();

  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();
  alcohol=analogRead(A0);
  lcd.setCursor(1,0);
  lcd.print("COMP");
  lcd.setCursor(7,0);
  lcd.print("F");
  lcd.setCursor(9,0);
  lcd.print(formal);
  lcd.setCursor(0,1);
  lcd.print("A");
  lcd.setCursor(8,1);
  lcd.print("B");
  lcd.setCursor(11,1);
  lcd.print("F");
  lcd.setCursor(10,1);
  lcd.print(benzene);
  lcd.setCursor(2,1);
  lcd.print(alcohol);
  lcd.setCursor(15,0);
  if(alcohol<60||formal<30||benzene<40){
    lcd.print("G");
    }
    else{
      lcd.print("F");
      }
  
  digitalWrite(D3,HIGH);
  digitalWrite(D0,HIGH);
  if(digitalRead(D3)==HIGH)
   {
    alcohol=analogRead(A0);
    Serial.print("Alcohol value: ");
    Serial.println(alcohol);
    benzene=(0.75*alcohol);
    formal=(0.23*alcohol)+(0.24*benzene);
    Firebase.setString(firebaseData, "/cosmetics/user/100/benzene", String(benzene));
    Firebase.setString(firebaseData, "/cosmetics/user/100/alcohol", String(alcohol));
    Firebase.setString(firebaseData, "/cosmetics/user/100/formal", String(formal));
   }
   delay(1000);
   digitalWrite(D3,LOW);
   digitalWrite(D0,LOW);
   Serial.print("alcohl value: ");
   Serial.println(alcohol);
   if(digitalRead(D3)==LOW){
    benzene=analogRead(A0);
    Serial.print("benzene value: ");
    Serial.println(benzene);
    lcd.setCursor(3,1);
    lcd.print("no any call...!");
    formal=(0.23*alcohol)+(0.24*benzene);
    Firebase.setString(firebaseData, "/cosmetics/user/100/benzene", String(benzene));
    Firebase.setString(firebaseData, "/cosmetics/user/100/formal", String(formal));

   }
   Serial.print("benzene value: ");
   Serial.println(benzene);
   delay(1000);
    

}
