#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h> 

//WiFi
#define WIFI_SSID "CETYS_Universidad"
#define WIFI_PASSWORD ""

//FireBase
#define API_KEY "AIzaSyDoY2Gis3VJPMoKTYS8iqPS6nqIkPRy0x8"
#define DATABASE_URL "https://proyecto-final-8bdcf-default-rtdb.firebaseio.com/"

#define USER_EMAIL "espuser@mail.com"
#define USER_PASSWORD "esppassword"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

//Declaring Initial Variables
String DataH = "/Soil_Humidity"; 
String sensorValueH; //numero de sensor 
unsigned long sendDataPrevMillis = 0;
const int sensor_pin = 0;
float moisture_percentage;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0))
  {
    moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023) * 100.00 ) );
    sendDataPrevMillis = millis(); //Hora en Milisegundos 
    
    sensorValueH = DataH; //concatenando el número de lectura que es
    Serial.printf("Set Soil Hums... %s",Firebase.setFloat(fbdo, sensorValueH, moisture_percentage)?"ok": fbdo.errorReason().c_str());//%s -> Agregando string
    Serial.println(analogRead(0));
    Serial.println(moisture_percentage);
    Serial.println(" ");
  }
}