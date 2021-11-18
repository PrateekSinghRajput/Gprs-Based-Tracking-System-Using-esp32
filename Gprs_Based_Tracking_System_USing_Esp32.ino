//Prateek
//wwww.prateeks.in
//https://www.youtube.com/c/JustDoElectronics/videos

#define BLYNK_PRINT Serial
#define TINY_GSM_MODEM_SIM800

//#define BLYNK_HEARTBEAT 30

#include <TinyGPS++.h> //https://github.com/mikalhart/TinyGPSPlus
#include <TinyGsmClient.h> //https://github.com/vshymanskyy/TinyGSM
#include <BlynkSimpleTinyGSM.h> //https://github.com/blynkkk/blynk-library

char auth[] = "GL7svcQiOVqFwTZS43svvI7ah33i06p4";
char apn[]  = "internet";
char user[] = "";
char pass[] = "";

//const String PHONE = "Enter_Your_Phone_Number";

#define rxPin 4
#define txPin 2
HardwareSerial sim800(1);
TinyGsm modem(sim800);

#define RXD2 16
#define TXD2 17
HardwareSerial neogps(2);
TinyGPSPlus gps;

WidgetMap myMap(V0);
BlynkTimer timer;
int pointIndex = 1;

void setup() {

  Serial.begin(115200);
  Serial.println("esp32 serial initialize");
  delay(10);
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("neogps serial initialize");
  delay(10);
  sim800.begin(9600, SERIAL_8N1, rxPin, txPin);
  Serial.println("SIM800L serial initialize");
  delay(3000);
  Serial.println("Initializing modem...");
  modem.restart();
  Blynk.begin(auth, modem, apn, user, pass);
  //timer.setInterval(5000L, sendToBlynk);
}

void loop() {

  while (neogps.available())
  {
    if (gps.encode(neogps.read()))
    {
      sendToBlynk();
    }
  }

  Blynk.run();
  //timer.run();

}

void sendToBlynk()
{

  if (gps.location.isValid() )
  {
    //get latitude and longitude
    float latitude = (gps.location.lat());
    float longitude = (gps.location.lng());
    float speed = gps.speed.kmph();
    float satellites = gps.satellites.value();

    Serial.print("Latitude:  ");
    Serial.println(latitude, 6);
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);
    Serial.print("Speed: ");
    Serial.println(speed, 6);

    Blynk.virtualWrite(V1, String(latitude, 6));
    Blynk.virtualWrite(V2, String(longitude, 6));
    myMap.location(pointIndex, latitude, longitude, "GPS_Location");
    Blynk.virtualWrite(V3, speed);
  }
}
