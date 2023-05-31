#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h> 

//WiFi
#define WIFI_SSID "WiFi_Pancho"
#define WIFI_PASSWORD "123456789"

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

bool bVal, Salida;

String baseData = "/Soil_Humidity";
unsigned long sendDataPrevMillis = 0;
// Relay pins
const int relay1Pin = D1;  // GPIO pin for relay 1
const int relay2Pin = D2;  // GPIO pin for relay 2
String Value;

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
 
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);

}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis(); //Hora en Milisegundos 

    
    Serial.printf("Get Outputs ref... %s\n", Firebase.RTDB.getBool(&fbdo, "/Control_Out", &bVal) ? bVal ? "true" : "false" : fbdo.errorReason().c_str());
    Serial.println(" ");
    Value = Firebase.RTDB.getBool(&fbdo, "/Control_Out", &bVal)? bVal ? "true" : "false" : fbdo.errorReason().c_str();
    if(Value=="true"){
      Salida = 1;
    }
    else{
      Salida = 0;
    }
    Serial.println(Salida);
  }
  digitalWrite(relay1Pin, !Salida);
  digitalWrite(relay2Pin, !Salida);
}