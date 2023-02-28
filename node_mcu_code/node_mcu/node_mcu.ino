hup// Library Declaration
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

// NUDE_MCU Pins Declaration
#define D0    16 // high at boot (no PWMm or I2C support)
#define D1    05 // SCL
#define D2    04 // SDA
#define D3    00 // boot failure if pulled LOW
#define D4    02 // high at boot and boot failure if pulled LOW (on-board led)
#define D5    14
#define D6    12
#define D7    13
#define D8    15 // boot failure if pulled HIGH
#define RX    03 // rx pin high at boot
#define TX    01 // tx pin high at boot and boot failure if pulled LOW
#define ADC0  A0 // analogic


// Defining pins 
#define echoPin D2 // attach pin D2 ESP8266 to pin Echo of HC-SR04
#define trigPin D3 //attach pin D3 ESP8266 to pin Trig of HC-SR04

#define smoke ADC0

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
float GAS; //variable for the gas value

//Fill in appropriate on Firebase
// link youtube describe the instruction 

//https://www.youtube.com/watch?v=80tECQYzwEEhttps://www.youtube.com/watch?v=80tECQYzwEE

#define FIREBASE_HOST "nude-mcu-default-rtdb.firebaseio.com"             // put the link from the firebase servise 
#define FIREBASE_AUTH "h8bI4rYGxCdbONOT4s6Y9r1T5UeDwCrthadB8ht0"         // firebae also



//Nama Wifi
#define WIFI_SSID "user_name_WIFI"
#define WIFI_PASSWORD "PASSWORD"  

// declare data objects from FirebaseESP8266
FirebaseData firebaseData;

void setup() {
  
 // define the output-input pins  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(smoke,INPUT);//Sets the Gas_Sensor pin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 & Gas Test "); // print some text in Serial Monitor
  Serial.println("with ESP8266");
  
  //   Connection to Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
}

void loop(){
  //chang from here to ss

  ///////////////////// ULTRASONEC///////////////
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

//////////// GAS_SENSOR////////////////////////
  GAS=analogRead(smoke);
  
  Serial.println(GAS); 
  

//********************************************************
//********************SS**********************************
//********************************************************

  // Provides distance status to firebase
  if (Firebase.setFloat(firebaseData, "/Project_reading/Ultrasonic", distance)){
      Serial.println("Distance delivered");
    } else{
      Serial.println("Distance not delivered");
      Serial.println("Because: " + firebaseData.errorReason());
    } 
    
  //if you want to add miltiple sensor remove the comment 
  /* 
   *  
   *  
   *  
   if (Firebase.setFloat(firebaseData, "/Project_reading/Ultrasonic", distance)){
      Serial.println("Distance delivered");
    } else{
      Serial.println("Distance not delivered");
      Serial.println("Because: " + firebaseData.errorReason());
    }   
   *  
   * 
    */
    
  
  delay(1000);
}
