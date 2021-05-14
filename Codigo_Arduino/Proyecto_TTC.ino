#include <DHT.h>

#define LDR_PIN A0
#define RV1_PIN A2 
#define RV2_PIN A3
#define RV3_PIN A7
#define DHT_PIN 2
#define DHT_TYPE DHT11
#define MPX_PIN A1

#define MOTOR_PIN 13

#define LED_TEMP_PIN 11
#define LED_HUMIDITY_PIN 10
#define LED_STORM_PIN 9

// Inicializamos el sensor DHT11
DHT dht(DHT_PIN, DHT_TYPE);
bool motorState = false;
bool previusMotor = false;

void setup() 
{
  Serial.begin(9600);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_TEMP_PIN, OUTPUT);
  pinMode(LED_HUMIDITY_PIN, OUTPUT);
  pinMode(LED_STORM_PIN, OUTPUT);
  dht.begin();
}

void loop() 
{
 
  int humidity = dht.readHumidity();
  int temp = dht.readTemperature();
  int brightness = analogRead(LDR_PIN);
  int pressure= analogRead(MPX_PIN);
  int wind = analogRead(RV1_PIN);
  int rain = analogRead(RV2_PIN);

  if(temp >= 26 || temp <= 6) digitalWrite(LED_TEMP_PIN, HIGH);
  else digitalWrite(LED_TEMP_PIN, LOW);

  if(humidity >= 79)
  {
    motorState = false;
    previusMotor = true;
    digitalWrite(LED_HUMIDITY_PIN, HIGH);
  }
  else if(humidity <= 30)
  {
    motorState = true;
    previusMotor = false; 
    digitalWrite(MOTOR_PIN, HIGH);
    digitalWrite(LED_HUMIDITY_PIN, HIGH); 
  }
  else
  {
    motorState = false;
    previusMotor = true;
    digitalWrite(MOTOR_PIN, LOW);
    digitalWrite(LED_HUMIDITY_PIN, LOW);
  }
  
  if(wind >= 700 && rain >= 700 && pressure >= 200) digitalWrite(LED_STORM_PIN, HIGH);
  else digitalWrite(LED_STORM_PIN, LOW);


  Serial.println("luminosidad " + String(brightness));
  Serial.println("humedaddesuelo " + String(humidity)); 
  Serial.println("temperatura " + String(temp)); 
  
  if (Serial.available() > 0)
  {
      char c = Serial.read();
      if(c = '1')previusMotor = !previusMotor;
      if(motorState && previusMotor) digitalWrite(MOTOR_PIN, HIGH);
      else digitalWrite(MOTOR_PIN, LOW);
   }
  
  delay(30000); 
}
