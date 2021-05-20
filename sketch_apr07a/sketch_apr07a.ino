#include <LiquidCrystal.h>
#include <DHT.h>

//Contain info send by bluetooth device
String info;

//Declare fan for regulate temp or humedity.
int tempHigh = 50;
int tempLow = 51;
int humLow = 53;
int humHigh = 52;

//Declare data from DHT11 sensor
int sensor = 3;
int temperature;
int humidity;
DHT dhtSensor(sensor, DHT11); //Init Sensor
LiquidCrystal lcd(4,5,6,7,8,9); //Initial pin LCD  

//To send info at user about status temperature and humidity
boolean sendAlertHumidity = true;
boolean sendCorrectHumidity = true;
boolean sendAlertTemperature = true;
boolean sendCorrectTemperature = true;

//enable or disable automatic control for temperature and humidity
boolean automaticControl = true;

//Declare light of House
int lamp1 = 22;
int lamp2 = 23;
int lamp3 = 24;
int lamp4 = 25;
int lamp5 = 26;
int lamp6 = 27;
int lamp7 = 28;
int lamp8 = 29;

//flags ligth
boolean l1 = false;
boolean l2 = false;
boolean l3 = false;
boolean l4 = false;
boolean l5 = false;
boolean l6 = false;
boolean l7 = false;
boolean l8 = false;
boolean allHouseFlag = false;

//Switch for lamps
int switch1 = A0;
int switch2 = A1;
int switch3 = A2;
int switch4 = A3;
int switch5 = A7;
int switch6 = A6;
int switch7 = A5;
int switch8 = A4;

int anochecer = A8;

//Control door
int abierto = A9;
int cerrado = A10;
int alarm = A11;

//flag securityControl
boolean securityControl = false;

//Control move sensor
int moveSensor = 10;

//Flag alertSensor
boolean alertMoveSensor = true;

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

  pinMode(switch1, INPUT);
  pinMode(switch2, INPUT);
  pinMode(switch3, INPUT);
  pinMode(switch4, INPUT);
  pinMode(switch5, INPUT);
  pinMode(switch6, INPUT);
  pinMode(switch7, INPUT);
  pinMode(switch8, INPUT);

  pinMode(anochecer, INPUT);

  pinMode(abierto, OUTPUT);
  pinMode(cerrado, OUTPUT);
  pinMode(alarm, OUTPUT);

  pinMode(moveSensor, INPUT);
}

void loop() {
  verifyRemoteData(); //Verify if there are data to process and execute
  controlHumidityandTemperature(); //Check data for Humidity and Temperature
  controlLight();
  checkEntrance();

  delay(100);
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

//Control unitary for lamps
void checkchangeOnlight(String command){
  if(!allHouseFlag){
    if(command.indexOf("1") > 0){
      if(command.indexOf("on") > 0){
        digitalWrite(lamp1, HIGH);
        l1 = true;
      }
      else{
        digitalWrite(lamp1, LOW);
        l1 = false;
      }
    }
    else if(command.indexOf("2") > 0){
      if(command.indexOf("on") > 0){
        digitalWrite(lamp2, HIGH);
        l2 = true;
      }
      else{
        digitalWrite(lamp2, LOW);
        l2 = false;
      }
    }
    else if(command.indexOf("3") > 0){
      if(command.indexOf("on") > 0){
        digitalWrite(lamp3, HIGH);
        l3 = true;
      }
      else{
        digitalWrite(lamp3, LOW);
        l3 = false;
      }
    }
    else if(command.indexOf("4") > 0){
      if(command.indexOf("on") > 0){
        digitalWrite(lamp4, HIGH);
        l4 = true;
      }
      else{
        digitalWrite(lamp4, LOW);
        l4 = false;
      }
    }
    else if(command.indexOf("5") > 0){
      if(command.indexOf("on") > 0){
        digitalWrite(lamp5, HIGH);
        l5 = true;
      }
      else{
        digitalWrite(lamp5, LOW);
        l5 = false;
      }
    }
    else if(command.indexOf("6") > 0){
      if(command.indexOf("on") > 0){
        digitalWrite(lamp6, HIGH);
        l6 = true;
      }
      else{
        digitalWrite(lamp6, LOW);
        l6 = false;
      }
    }
    else if(command.indexOf("7") > 0){
      if(command.indexOf("on") > 0){
        digitalWrite(lamp7, HIGH);
        l7 = true;
      }
      else{
        digitalWrite(lamp7, LOW);
        l7 = false;
      }
    }
    else if(command.indexOf("8") > 0){
      if(command.indexOf("on") > 0){
        digitalWrite(lamp8, HIGH);
        l8 = true;
      }
      else{
        digitalWrite(lamp8, LOW);
        l8 = false;
      }
    }
    else{
      //nothing to do
    }
  }
}

//on/off all lamps 
void allHouse(String command){
  if(command == "a on"){
    allHouseFlag = true;
    digitalWrite(lamp1, HIGH);
    digitalWrite(lamp2, HIGH);
    digitalWrite(lamp3, HIGH);
    digitalWrite(lamp4, HIGH);
    digitalWrite(lamp5, HIGH);
    digitalWrite(lamp6, HIGH);
    digitalWrite(lamp7, HIGH);
    digitalWrite(lamp8, HIGH);
  }
  else{
    allHouseFlag = false;
    digitalWrite(lamp1, LOW);
    digitalWrite(lamp2, LOW);
    digitalWrite(lamp3, LOW);
    digitalWrite(lamp4, LOW);
    digitalWrite(lamp5, LOW);
    digitalWrite(lamp6, LOW);
    digitalWrite(lamp7, LOW);
    digitalWrite(lamp8, LOW);
  }
}

void checksecurityControl(String command){
  if(command == "o on"){
    securityControl = true;
    digitalWrite(abierto, HIGH);
    digitalWrite(cerrado, LOW);
  }
  else{
    securityControl = false;
  }
}

//Analyze command for send to specific function
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
    checkchangeOnlight(command);
  }
  else if(command.startsWith("a")){
    allHouse(command);
  }
  else{
    checksecurityControl(command);
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
}

//Update panel LCD that contain info about status temperature and humidity
void updateLcd(int humidity, int temperature){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String("Temperatura: ")+String(temperature)+String("C°"));
  lcd.setCursor(0,1);
  lcd.print(String("Humedad: ")+String(humidity)+String("%"));
}

//Function for control the humidity in the range.
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

//Function for control the temperature in the range.
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

//Check the light flags to apply status from switch to the lamp
void controlLight(){
  if(!allHouseFlag){
    if(!l1){
      digitalWrite(lamp1, digitalRead(switch1));
    }
    if(!l2){
      digitalWrite(lamp2, digitalRead(switch2));
    }
    if(!l3){
      digitalWrite(lamp3, digitalRead(switch3));
    }
    if(!l4){
      digitalWrite(lamp4, digitalRead(switch4));
    }
    if(!l5){
      digitalWrite(lamp5, digitalRead(switch5));
    }
    if(!l6){
      digitalWrite(lamp6, digitalRead(switch6));
    }
    if(!l7){
      digitalWrite(lamp7, digitalRead(switch7));
    }
    if(!l8){
      digitalWrite(lamp8, digitalRead(switch8));
    }
    //Check if it's night
    if(digitalRead(anochecer) > 0){
      digitalWrite(lamp8, HIGH);
    }
  }
}

//Check the entrance of move Sensor
void checkEntrance(){
  if(digitalRead(anochecer) > 0){
    if(!securityControl){
      if(digitalRead(moveSensor) == HIGH){
        digitalWrite(alarm, HIGH);
        delay(100);
        digitalWrite(alarm, LOW);
        digitalWrite(cerrado, HIGH);
        digitalWrite(abierto, LOW);
        
        if(alertMoveSensor){
          Serial.println("pon");
          alertMoveSensor = false;
        }
      }
      else{
        alertMoveSensor = true;
        digitalWrite(alarm, LOW);
        digitalWrite(cerrado, LOW);
        digitalWrite(abierto, HIGH);
      }
    }
  }
  else{
    digitalWrite(abierto, HIGH);
    digitalWrite(cerrado, LOW);
  }
}
