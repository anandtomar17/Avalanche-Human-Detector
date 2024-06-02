#include <MicroNMEA.h>
#include <DHTesp.h>
#include <WiFi.h>
#include <ThingSpeak.h>
char buffer[85];
MicroNMEA nmea(buffer, sizeof(buffer));

char rmc[] = "$GNRMC,034035.000,A,3150.8617,N,11711.9038,E,3.02,183.45,240516,,,A*75\r\n";
char rmc2[] = "$GNRMC,034035.000,A,3150.8617,N,11711.9038,E,3.02,183.45,240516,,,A*75\r";
char gga[] = "$GNGGA,034035.000,3150.8617,N,11711.9038,E,1,4,1.50,40.9,M,0.0,M,,*44\r";

void fill(const char *sentence) {
  for (int i = 0; i < strlen(sentence); i++) {
    if (nmea.process(sentence[i])) {
      return;
    }
  }
}
void imprimir() {
  Serial.print("Valid fix: ");
  Serial.println(nmea.isValid() ? "yes" : "no");

  Serial.print("Nav. system: ");
  if (nmea.getNavSystem())
    Serial.println(nmea.getNavSystem());
  else
    Serial.println("none");

  Serial.print("Num. satellites: ");
  Serial.println(nmea.getNumSatellites());

  //Serial.print("HDOP: ");
  //Serial.println(nmea.getHDOP() / 10., 1);

  //Serial.print("Date/time: ");
  //Serial.print(nmea.getYear());
  //serial.print('-');
  //Serial.print(int(nmea.getMonth()));
  //Serial.print('-');
  //Serial.print(int(nmea.getDay()));
  //Serial.print('T');
  //Serial.print(int(nmea.getHour()));
  //Serial.print(':');
  //Serial.print(int(nmea.getMinute()));
  //Serial.print(':');
  //Serial.println(int(nmea.getSecond()));

  //long latitude_mdeg = nmea.getLatitude();
  //long longitude_mdeg = nmea.getLongitude();
 // Serial.print("Latitude (deg): ");
  //Serial.println(latitude_mdeg / 1000000., 6);

  //Serial.print("Longitude (deg): ");
  //Serial.println(longitude_mdeg / 1000000., 6);

  //long alt;
  //Serial.print("Altitude (m): ");
  //if (nmea.getAltitude(alt))
   // Serial.println(alt / 1000., 3);
  //else
    //Serial.println("not available");

  //Serial.print("Speed: ");
  //Serial.println(nmea.getSpeed() / 1000., 3);
  //Serial.print("Course: ");
  //Serial.println(nmea.getCourse() / 1000., 3);
  //Serial.println("- - - - - - - - - - - -  -");
}
// DHT declaration
DHTesp dhtSensor;
TempAndHumidity data;
const int dhtpin = 15;

// Ultrasonic declaration
const int echo = 4;
const int trig = 2;
float distance;
float duration;

const int ledPin = 13; // Pin to which the LED is connected

const int pirpin = 14;
// WiFi declaration
char ssid[] = "Wokwi-GUEST";
char pass[] = "";
WiFiClient client;

// Thingspeak declaration
unsigned long myChannelNumber = 2264423;
const char *myWriteAPIKey = "L3UJDG459I78V3RX";
int statusCode;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  fill(rmc2);
  imprimir();
  fill(gga);
  imprimir();
  // Ultrasonic setup
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // DHT setup
  dhtSensor.setup(dhtpin, DHTesp::DHT22);
  pinMode(pirpin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  // WiFi setup
  WiFi.mode(WIFI_STA);

  // Thingspeak setup
  ThingSpeak.begin(client);
  delay(10);
  }
  float getdistance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  return duration * 0.034 / 2;
}

void loop() {
  int a=digitalRead(pirpin);
  Serial.println(a);
digitalWrite(ledPin, a);
 if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect...");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected");
  }
   // Read DHT sensor
  data = dhtSensor.getTempAndHumidity();

  // Read distance
  distance = getdistance();
  long latitude_mdeg = nmea.getLatitude();
  long longitude_mdeg = nmea.getLongitude();
  int randomSpO2 = random(90, 100);  // Random value between 90 and 99
  int randomHeartRate = random(60, 100);  // Random value between 60 and 99
 if(a==HIGH){
 
  Serial.println("Motion Detected");

  Serial.print("Latitude (deg): ");
  Serial.println(latitude_mdeg / 1000000., 6);

  Serial.print("Longitude (deg): ");
  Serial.println(longitude_mdeg / 1000000., 6);

  Serial.println("Distance:" + String(distance));
  Serial.println("Temp:" + String(data.temperature));
  Serial.println("Humi:" + String(data.humidity));
   Serial.print("Heart Rate: ");
  Serial.print(randomHeartRate);
  Serial.print(" bpm, ");

  Serial.print("Oxygen Saturation: ");
  Serial.print(randomSpO2);
  Serial.println(" %");


  Serial.println("<-------------------->");}
 
 else if (a==LOW){

   Serial.println("Motion Not Detected");
   Serial.println("Distance:NIL");
  Serial.println("Temp:NIL");
  Serial.println("Humi:NIL");
  Serial.println("Heart Rate:NIL");
  Serial.println("Oxygen Saturation:NIL");

}
}
