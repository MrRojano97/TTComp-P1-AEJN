#include <LiquidCrystal.h>
#include <DHT.h>

//Contain info send by bluetooth device
String info;

//Declare fan for regulate temp or humedity.
int tempHigh = 50;
int tempLow = 51;
int humLow = 53;
int humHigh = 52;

//Declare light of House
int lamp1 = 22;
int lamp2 = 23;
int lamp3 = 24;
int lamp4 = 25;
int lamp5 = 26;
int lamp6 = 27;
int lamp7 = 28;
int lamp8 = 29;

//Declare data from DHT11 sensor
int sensor = 3;
int temperature;
int humidity;
DHT dhtSensor(sensor, DHT11); //Init Sensor
LiquidCrystal lcd(4,5,6,7,8,9); //Initial pin LCD  

boolean sendAlertHumidity = true;
boolean sendCorrectHumidity = true;
boolean sendAlertTemperature = true;
boolean sendCorrectTemperature = true;

boolean automaticControl = true;

void setup() {
  dhtSensor.begin();
  lcd.begin(16,2);
  Serial.begin(9600);
  
  pinMode(tempHigh, OUTPUT);
  pinMode(tempLow, OUTPUT);
  pinMode(humLow, OUTPUT);
  pinMode(humHigh, OUTPUT);

  pinMode(lamp1, OUTPUT);
  pinMode(lamp2, OUTPUT);
  pinMode(lamp3, OUTPUT);
  pinMode(lamp4, OUTPUT);
  pinMode(lamp5, OUTPUT);
  pinMode(lamp6, OUTPUT);
  pinMode(lamp7, OUTPUT);
  pinMode(lamp8, OUTPUT);
}

void loop() {
  verifyRemoteData(); //Verify if there are data to process and execute
  controlHumidityandTemperature(); //Check data for Humidity and Temperature

  delay(200); //Wait for repeat loop
}


void verifyRemoteData(){
  if(Serial.available() > 0){
    info = Serial.readString();
    // Length (with one extra character for the null terminator)
    int str_len = info.length() + 1; 
    // Prepare the character array (the buffer) 
    char char_array[str_len];
    // Copy it over 
    info.toCharArray(char_array, str_len);
      
    String aux = "";
    for(int i=0; char_array[i] != NULL; i++){
      if(char_array[i] == ';'){
        executeCommand(aux);
        aux="";
      }
      else{
        aux+=char_array[i];
      }
    }
  }
}

//Activate o desactivate manual control for temperature and humidity
void controlManual(String command){
  if(command == "c: on"){
    automaticControl = false;
  }
  else{
    automaticControl = true;
    //Restart alarm
    sendAlertHumidity = true;
    sendCorrectHumidity = true;
    sendAlertTemperature = true;
    sendCorrectTemperature = true;
  }
  //restart all control
  digitalWrite(humLow, LOW);
  digitalWrite(humHigh, LOW);
  digitalWrite(tempLow, LOW);
  digitalWrite(tempHigh, LOW);
}

//Control manual humidity
void manualHumidityControl(String command){
  if(command == "h up on"){
    digitalWrite(humHigh, HIGH);
  }
  else if(command == "h up off"){
    digitalWrite(humHigh, LOW);
  }
  else if(command == "h down on"){
    digitalWrite(humLow, HIGH);
  }
  else{
    digitalWrite(humLow, LOW);
  }
}

//Control manual temperature
void manualTemperatureControl(String command){
  if(command == "t up on"){
    digitalWrite(tempHigh, HIGH);
  }
  else if(command == "t up off"){
    digitalWrite(tempHigh, LOW);
  }
  else if(command == "t down on"){
    digitalWrite(tempLow, HIGH);
  }
  else{
    digitalWrite(tempLow, LOW);
  }
}

void executeCommand(String command){
  //manual control
  if(command.startsWith("c")){
    controlManual(command);
  }
  //humidity by manual control
  else if(command.startsWith("h")){
    manualHumidityControl(command);
  }
  //temperature by manual control
  else if(command.startsWith("t")){
    manualTemperatureControl(command);
  }
  //light control
  else if(command.startsWith("l")){
    
  }
  //voice control
  else if(command.startsWith("v")){
    
  }
  else{
    
  }
}

void controlHumidityandTemperature(){
  //Get data from sensor.
  humidity=dhtSensor.readHumidity();
  temperature=dhtSensor.readTemperature();
  updateLcd(humidity, temperature); 

  if(automaticControl){
    controlHumidity(humidity);
    controlTemperature(temperature);
  }
  //readSerial();
}

//Update panel LCD that contain info about status temperature and humidity
void updateLcd(int humidity, int temperature){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String("Temperatura: ")+String(temperature)+String("C°"));
  lcd.setCursor(0,1);
  lcd.print(String("Humedad: ")+String(humidity)+String("%"));
}

//Function for control the humidity.
void controlHumidity(int h){
  if(h >= 40 && h <= 60){
    digitalWrite(humLow, LOW);
    digitalWrite(humHigh, LOW);
    if(sendCorrectHumidity){
      sendAlertHumidity = true;
      sendCorrectHumidity = false;
      Serial.println("hok");
    }
  }
  else{
    if(h > 60){
      digitalWrite(humLow, HIGH);
      if(sendAlertHumidity){
        sendAlertHumidity = false;
        sendCorrectHumidity = true;
        Serial.println("hup");
      }
    }
    else{
      digitalWrite(humHigh, HIGH);
      if(sendAlertHumidity){
        sendAlertHumidity = false;
        sendCorrectHumidity = true;
        Serial.println("hdown");
      }
    }
  }
}

void controlTemperature(int t){
  if(t >= 19 && t <= 25){
    digitalWrite(tempLow, LOW);
    digitalWrite(tempHigh, LOW);
    if(sendCorrectTemperature){
      sendCorrectTemperature = false;
      sendAlertTemperature = true;
      Serial.println("tok");
    }
  }
  else{
    if(t > 25){
      digitalWrite(tempLow, HIGH);
      if(sendAlertTemperature){
        sendAlertTemperature = false;
        sendCorrectTemperature = true;
        Serial.println("tup");
      }
    }
    else{
      digitalWrite(tempHigh, HIGH);
      if(sendAlertTemperature){
        sendCorrectTemperature = true;
        sendAlertTemperature = false;
        Serial.println("tdown");
      }
    }
  }  
}
