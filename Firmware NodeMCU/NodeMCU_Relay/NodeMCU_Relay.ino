#include <ESP8266WiFi.h>
#include "DHTesp.h"
#include <FirebaseESP8266.h>

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h> 

//WiFi
#define WIFI_SSID "WiFi_Pancho"
#define WIFI_PASSWORD "123456789"

//FireBase
#define API_KEY "AIzaSyBOafPFq7K0anVty-puKq5QSZ7hwGMlBJQ"
#define DATABASE_URL "https://proyectoprueba-8e29a-default-rtdb.firebaseio.com"

#define USER_EMAIL "espuser@mail.com"
#define USER_PASSWORD "esppassword"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

//Declaring Initial Variables
float TempSense = 0;
float HumSense = 0;
int RandTemp = 0;
int count2 = 0;
uint8_t k = 0; 
bool bVal, Salida;
DHTesp dht;
String baseDataT = "/lectura/temperatura/temp";
String baseDataH = "/lectura/humedad/hum";
String baseData = "/random/lectura";
String sensorValueT; //numero de sensor
String sensorValueH; //numero de sensor 
String sensorValue; //numero de sensor 
unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;

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

  dht.setup(D5,DHTesp::DHT11);
}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0))
  {
    RandTemp = random(20,30);
    TempSense = dht.getTemperature();
    HumSense = dht.getHumidity();

    sendDataPrevMillis = millis(); //Hora en Milisegundos 
    
    sensorValueT = baseDataT + count; //concatenando el número de lectura que es
    sensorValueH = baseDataH + count; //concatenando el número de lectura que es
    sensorValue = baseData + count2; //concatenando el número de lectura que es

    
    Serial.printf("Set Temps... %s",Firebase.setFloat(fbdo, sensorValueT, TempSense)?"ok": fbdo.errorReason().c_str());//%s -> Agregando string
    Serial.println(" ");
    Serial.printf("Set Hums... %s",Firebase.setFloat(fbdo, sensorValueH, HumSense)?"ok": fbdo.errorReason().c_str());//%s -> Agregando string
    Serial.println(" ");
    
    /*Serial.printf("Get Outputs ref... %s\n", Firebase.RTDB.getBool(&fbdo, "salidas/relay1", &bVal) ? bVal ? "true" : "false" : fbdo.errorReason().c_str());
    Serial.println(" ");
    Salida = Firebase.RTDB.getBool(&fbdo, "salidas/relay1", &bVal);
    //Serial.println(Salida);*/
  }
}