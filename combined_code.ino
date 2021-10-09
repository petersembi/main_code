//lcd + keypad

#include <LiquidCrystal_I2C.h>
#include <Keypad.h>  //install keypad library by Mark Stanley and Alexander Brevig
#include <AccelStepper.h> //Install the AccelStepper Library
#include <SoftwareSerial.h> //For GSM
#include <Stepper.h>
#include <Adafruit_Sensor.h> //FOR DHT SENSOR
#include <DHT.h> //for dht sensor
#include <DHT_U.h> //for dht sensor
/*=======SENSOR PINS==========*/
int dhtPin = 5;
int lm35Pin = 0; //AnalogPin

/*=========DHT SENSOR SETUP============*/
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
DHT_Unified dht(dhtPin, DHTTYPE);
 uint32_t delayMS;

/*circuit: pin 2 corresopinds to f2 side on keypad. pin 10 is connected to extreme left of the keypad. other pins in between follow each other*/
LiquidCrystal_I2C lcd(0x27,20,4); 
//arduino mega: connect sda to pin 20, scl to pin 21;
//arduino uno: connect sda to A4, scl to pin a5; 

/*=========RELAY PINS============*/
int airHeater_pin=2;
int fan1Relay_pin=3;
int fan2Relay_pin = 4;





/*=========STEPPER MOTOR CONFIG USING STEPPER.H LIBRARY============*/
//motor config using stepper.h library
const float STEPS_PER_REV = 32; 
const float GEAR_RED = 64;
const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;
int StepsRequired;

//#define motorPin11  23     // Blue   - 28BYJ48 pin 1
//#define motorPin22  25     // Pink   - 28BYJ48 pin 2
//#define motorPin33 27    // Yellow - 28BYJ48 pin 3
//#define motorPin44  29    // Orange - 28BYJ48 pin 4

int motorPin11 = 23;     // Blue   - 28BYJ48 pin 1
int motorPin22 = 25;     // Pink   - 28BYJ48 pin 2
int motorPin33 = 27;    // Yellow - 28BYJ48 pin 3
int motorPin44 = 29;    // Orange - 28BYJ48 pin 4

Stepper steppermotor(STEPS_PER_REV, motorPin11, motorPin33, motorPin22, motorPin44);

/***=========MOTOR CONFIGURATION=========*******/
// Define step constants
#define FULLSTEP 4
#define HALFSTEP 8




const byte ROWS = 5; //5 rows
const byte COLS = 4; //4 columns

// Define step constants
#define FULLSTEP 4
#define HALFSTEP 8

// Define Motor Pins (2 Motors used)

#define motorPin1  23     // Blue   - 28BYJ48 pin 1
#define motorPin2  25     // Pink   - 28BYJ48 pin 2
#define motorPin3  27    // Yellow - 28BYJ48 pin 3
#define motorPin4  29    // Orange - 28BYJ48 pin 4
                        
                        
#define motorPin5  31     // Blue   - 28BYJ48 pin 1
#define motorPin6  33     // Pink   - 28BYJ48 pin 2
#define motorPin7  35     // Yellow - 28BYJ48 pin 3
#define motorPin8  37     // Orange - 28BYJ48 pin 4

// Define two motor objects
// The sequence 1-3-2-4 is required for proper sequencing of 28BYJ48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
AccelStepper stepper2(FULLSTEP, motorPin5, motorPin7, motorPin6, motorPin8);

/*===========END OF MOTOR CONFIGURATION============*/

//==================GSM SET UP====================//

int rxPin = 3; //CONNECT TO TX OF GSM MODULE
int txPin = 4; //CONNECT TO RX OF GSM MODULE
SoftwareSerial sim(rxPin, txPin);
int _timeout;
String _buffer;
String number = "+254791084796"; //-> change with your number
String smsRaining = "Rainfall Started. Dehydrator Processes stopped! Consider Moving Dehydrator inhouse";
String smsProcessOver = "Drying Process is over. Offload Dehydrator after 5 minutes";
/////=========END OF GSM SET UP=========//////
//character array for string (more than one character)
char* specialKeys[] ={
            "F1",  "F2", "#", "*",
            "1",  "2", "3", "UP",
            "4",  "5", "6", "DOWN",
            "7",  "8", "9", "ESC",
            "LEFT",  "0", "RIGHT", "ENTER"  
                    };

//character array for chars                    
char specialKeysID[] = {
        'A',  'B', '#', '*',
        '1',  '2', '3', 'C',
        '4',  '5', '6', 'D',
        '7',  '8', '9', 'E',
        'F',  '0', 'G', 'H'
      };                    

//populating the keys multidimensional array
char keys[ROWS][COLS] = {
{specialKeysID[0],  specialKeysID[1], specialKeysID[2], specialKeysID[3]},
{specialKeysID[4],  specialKeysID[5], specialKeysID[6], specialKeysID[7]},
{specialKeysID[8],  specialKeysID[9], specialKeysID[10], specialKeysID[11]},
{specialKeysID[12],  specialKeysID[13], specialKeysID[14], specialKeysID[15]},
{specialKeysID[16],  specialKeysID[17], specialKeysID[18], specialKeysID[19]}
};

//keypad_pins
int rowpin1 = 22;
int rowpin2 = 24;
int rowpin3 = 26;
int rowpin4 = 28;
int rowpin5 = 30;
int colpin1 = 32;
int colpin2 = 34;
int colpin3 = 36;
int colpin4 = 38;
/*circuit: rowpin1 corresopinds to f2 side on keypad. colpin4 is connected to extreme left of the keypad. other pins in between follow each other*/

byte rowPins[ROWS] = {rowpin1,rowpin2,rowpin3, rowpin4,rowpin5}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {colpin4,colpin3,colpin2,colpin1}; //connect to the column pinouts of the kpd
//note that for col pins we start with pin 4

Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// Variables

//String testString = "Technologies";  projectNmePt2 = "", innovationNamePt1 = "", innovationNamePt2 = "";
String projectNmePt1 = "AUTOMATED  SMALL ";
String StringprojectNmePt2 = "SCALE GRAIN";
String StringprojectNmePt3 = "DEHYDRATOR!";
String StringprojectTitle = "<< PROJECT TITLE >>";
String innovationNamePt1 = "BPV DRYING";
String innovationNamePt2 = "TECHNOLOGIES LTD!!!";
String cropPrompt = "CHOOSE TYPE OF CROP";
String crop1 = " 1. << MAIZE >>";
String crop2 = " 2. << RICE >>";
String selectedCrop = "CROP SELECTED: ";
String moistureContent = "MOISTURE CONTENT: ";
char key_pressed;
float requiredTemp = 20.0;
float requiredHumidity = 20.0;
float riceTemp = 20;
float maizeTemp = 30;
float riceHumidity = 50;
float maizeHumidity = 60;
float lm35TempReading = 0;
float dhtTmpReading = 0;
float dhtHumidityReading = 0;
boolean valve1Open = false;
boolean valve2Open = false;
boolean airHeaterRelayOn = false;
boolean fan1RelayOn = false;
boolean fan2RelayOn = false;
char* pressed_key = "";
String kgsToLoad = "Enter No. of Kgs: ";
int x = 0; //for checking validity of crop selected 
int y = 0; //for checking validity of no of kgs
int z = 0; //for checking validity of moisture content
int p = 0; //for checking system parameters and doing count down
int r = 0;//for checking whether user is ready to start drying process
int counterCountdown = 48;
int stepperCounter = 5000;
String NoOfKgsStr = "";
String MoisContentStr = "";
int NoOfKgsCounter = 0;
int MoistureContentCounter = 0;
String distrAdvice = "Distribution: ";
String ReadyToStartEnqPt1 = "Ready?";
String ReadyToStartEnqPt2 = "Press Enter to Start";
int val;//for storing analog pin reading connected to lm35 temp sensor


//pins








//variables used for populating center text parameters
int row0 = 0; 
int row1 = 1;
int row2 = 2;
int row3 = 3;

//variables for populating print_to_Lcd() function
int row_to_print = 0 ;
int column_to_print = 0; 

// ***********IMPORTANT FUNCTIONS*********** 
// centerTxt(String txtToCenter, int rowToWrite): for centering text on lcd
//keyPressed(): for getting key pressed. return a char variable;
//getKey(char *k): for converting from single character declaration  to the other keys description described above. 
//                : used inside keyPressed function

/*======================GSM FUNCTIONS====================*/
void SendMessage(String SMS)
{
  //TOTAL DELAY  700milliseconds
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  //String SMS = "Hello, how are you? greetings from miliohm.com admin";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
}
void RecieveMessage()
{
  Serial.println ("SIM800L Read an SMS");
  sim.println("AT+CMGF=1");
  delay (200);
  sim.println("AT+CNMI=1,2,0,0,0"); // AT Command to receive a live SMS
  delay(200);
  Serial.write ("Unread Message done");
}
String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}

/*======DHT SENSOR FUNCTIONS========*/
 // Get temperature event and print its value.

float dhtTemp (){
   sensors_event_t event;
        dht.temperature().getEvent(&event);
        if (isnan(event.temperature)) {
          Serial.println(F("Error reading temperature!"));
        }
        else {
          Serial.print(F("Temperature: "));
          Serial.print(event.temperature);
          Serial.println(F("°C"));
        }
        return event.temperature;
  }

 float dhtHumidity (){
   // Get humidity event and print its value.
   sensors_event_t event;
        dht.humidity().getEvent(&event);
        if (isnan(event.relative_humidity)) {
          Serial.println(F("Error reading humidity!"));
        }
        else {
          Serial.print(F("Humidity: "));
          Serial.print(event.relative_humidity);
          Serial.println(F("%"));
        }
        return event.relative_humidity;
 }

  float lm35TempR(){
    
    val = analogRead(lm35Pin);
    float mv = (val/1024.0)*5000;
    float cel = mv/10;
    Serial.print("Temp from LM35: ");
    Serial.println(cel);
    Serial.print(("°C"));
    
    return cel;
  }
       
      
/*==========LCD AND KEYPAD FUNCTIONS*===============*/
//function to center text
void centerTxt(String txtToCenter, int rowToWrite){
  int stringLength = 0, cursorPstn = 0;  
  stringLength = txtToCenter.length();
  cursorPstn = (20-stringLength)/2;
  lcd.setCursor(cursorPstn,rowToWrite);
  lcd.print(txtToCenter);
}

char* getKey(char *k)
{
//Robojax.com 5x4 keypad detecting string value
for(int i=0; i<20; i++)
 {
    if(specialKeysID[i] ==k) return specialKeys[i];
  }
 
}//getKey

char* keyPressed(){

  char key = customKeypad.getKey();
      
   if (key){
    Serial.print("Key: "); 
    char* pressed_key = getKey(key);
    Serial.println(pressed_key);
    return pressed_key;
       
    //To print single characters
    //Serial.println(key);
  } 
 
}

//to check whether key pressed is valid. takes program back to crop enquiry. Returns valid key pressed
char* check_Key_for_crop_selection_and_validity(){
  key_pressed = keyPressed();
//    while (key_pressed != "1"||"2"){
//      lcd.clear();
//      centerTxt("Invalid choice",row0);
//      print_to_Lcd("Please Try Again",1,0);
//    
//     delay(3000);
//     lcd.clear();
//      centerTxt(cropPrompt,row0); 
//      print_to_Lcd(crop1, 1, 0);
//      print_to_Lcd(crop2, 2, 0);     
//     key_pressed = keyPressed();
//    }
//    return key_pressed;
  }


//to print text to lcd
void print_to_Lcd( String stringToPrint, int rowToPrint, int columnToPrint){
  lcd.setCursor(columnToPrint, rowToPrint);
  lcd.print(stringToPrint);  
  }
  
void setup() { 

/*PINS SET UP -  MODES*/
pinMode(airHeater_pin, OUTPUT);
pinMode(fan1Relay_pin, OUTPUT);
pinMode(fan2Relay_pin, OUTPUT);

//INITIALIZE DHT SENSOR
 // Initialize device.
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  

//========MOTOR SET UP==========//
//The motors are actually set into motion using the AccelStepper library “run” function in void loop. 
// 1 revolution Motor 1 CW. NB: One revolution is 2048 steps. use moveTo function to set that. 
  stepper1.setMaxSpeed(5000.0);
  stepper1.setAcceleration(50.0);
  stepper1.setSpeed(2000);
  stepper1.moveTo(4000);  
  
// 1 revolution Motor 2 CCW
  stepper2.setMaxSpeed(10000.0);
  stepper2.setAcceleration(500.0);
  stepper2.setSpeed(200);
  stepper2.moveTo(-2048); 

//GSM SET UP//
  
  
  _buffer.reserve(50);
  Serial.println("Sistem Started...");
  sim.begin(9600);
  delay(1000);



  
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on


 
  pinMode(airHeater_pin, OUTPUT);
  pinMode(fan1Relay_pin, OUTPUT);
  pinMode(fan2Relay_pin, OUTPUT);

  centerTxt(innovationNamePt1,row0);
  centerTxt(innovationNamePt2,row1);
 delay(4000);
  
  lcd.clear();
  
  centerTxt(StringprojectTitle,row0);
  centerTxt(projectNmePt1,row1);
  centerTxt(StringprojectNmePt2,row2);
   centerTxt(StringprojectNmePt3,row3);  
delay(4000);

 lcd.clear();
centerTxt(cropPrompt,row0); 
 print_to_Lcd(crop1, 1, 0);
  print_to_Lcd(crop2, 2, 0);
delay(4000);



  


   }
 
  

  
  
  



void loop() {



while(x!=5){

  if(x==1)
  {

  
centerTxt(cropPrompt,row0); 
 print_to_Lcd(crop1, 1, 0);
  print_to_Lcd(crop2, 2, 0);
//Serial.println("costing");


  }

char key = customKeypad.getKey();


   if (key){
    
   
   // print_to_Lcd("maize", 3, 0);
   
  
   
     Serial.print("Key: "); 
     pressed_key = getKey(key);
    Serial.println(pressed_key);
   
    if (pressed_key == "1" )
    { 
      lcd.clear();
      centerTxt(selectedCrop,row0); 
           
      print_to_Lcd("Maize",1,0);
      
      requiredTemp = maizeTemp;
      requiredHumidity = maizeHumidity;
      Serial.println("Required Temp is: ");
      Serial.print(requiredTemp);
      Serial.println("Required Humidity is: ");
      Serial.print(requiredHumidity);
      delay(2000);
      lcd.clear();
      x = 5;
      
      }else if(pressed_key == "2" ) {
      lcd.clear();
      centerTxt(selectedCrop,row0);
      print_to_Lcd("Rice",1 , 0);  
      requiredTemp = riceTemp;  
      requiredHumidity = riceHumidity;
      Serial.print("Required Temp is: ");
      Serial.println(requiredTemp);
      Serial.print("Required Humidity is: ");
      Serial.println(requiredHumidity);
      x=5;
      delay(2000);
      lcd.clear();
     
      
   }else {
         lcd.clear();
      centerTxt("Invalid Choice",row0);
      print_to_Lcd("Please Try Again",1 , 0);
      delay(2000);
      lcd.clear();
      x =1;
   }       

   Serial.println(x);
   }
}

 while(z!=5){
  centerTxt(moistureContent,row0);

  if(z==1)
  {
  
centerTxt(moistureContent,row0);

  }
  
char key = customKeypad.getKey();


   if (key){
      
     
     pressed_key = getKey(key);
   
   

   

    if(MoistureContentCounter <= 2 && pressed_key != "ENTER"){
        MoisContentStr.concat(pressed_key);
        MoistureContentCounter++;
         print_to_Lcd(pressed_key,1,MoistureContentCounter);
         delay(1000);
    

    if(MoistureContentCounter == 2 && pressed_key != "ENTER"){
      Serial.println(MoisContentStr);
      lcd.clear();
      centerTxt(moistureContent,row0);
      print_to_Lcd(MoisContentStr,1,0);
      print_to_Lcd("%",1,2);
      delay(2000);
      lcd.clear();
      z=5;
      
      
      
//      if(noOfKgsFloat <= 30){
//        lcd.clear();
//        centerTxt(distrAdvice,row0);         
//
//        float kgsPerLayer = noOfKgsFloat/3;
//        String kgsPerLayerString = String(kgsPerLayer);
//        print_to_Lcd("Layer 1: ",1,0);
//        print_to_Lcd(kgsPerLayerString,1,10);
//        print_to_Lcd("Layer 2",2,0);
//        print_to_Lcd(kgsPerLayerString,2,10);
//        print_to_Lcd("Layer 3",3,0);
//        print_to_Lcd(kgsPerLayerString,3,10);
//        y=5;
//        delay(10000);
//        lcd.clear();
//      }
//      else{
//        lcd.clear();
//        print_to_Lcd("A max of 30 allowed",0,1);
//        delay(4000);
//        y=1;
//        NoOfKgsCounter = 0;
//        NoOfKgsStr ="";
//        lcd.clear();
//        
//      
//      }
   }
     
      
   }
     }
}


      

    while(y!=5){
  centerTxt(kgsToLoad,row0);

  if(y==1)
  {
  
centerTxt(kgsToLoad,row0);

  }
  
char key = customKeypad.getKey();


   if (key){
      
     
     pressed_key = getKey(key);
   
   

   

    if(NoOfKgsCounter <= 2 && pressed_key != "ENTER"){
        NoOfKgsStr.concat(pressed_key);
        NoOfKgsCounter++;
         print_to_Lcd(pressed_key,1,NoOfKgsCounter);
         delay(1000);
    

    if(NoOfKgsCounter == 2 && pressed_key != "ENTER"){
      Serial.println(NoOfKgsStr);
      float noOfKgsFloat = 0;
      noOfKgsFloat = NoOfKgsStr.toFloat();
      if(noOfKgsFloat <= 30){
        lcd.clear();
        centerTxt(distrAdvice,row0);         

        float kgsPerLayer = noOfKgsFloat/3;
        String kgsPerLayerString = String(kgsPerLayer);
        print_to_Lcd("Layer 1: ",1,0);
        print_to_Lcd(kgsPerLayerString,1,10);
        print_to_Lcd("kgs",1,14);
        print_to_Lcd("Layer 2",2,0);
        print_to_Lcd(kgsPerLayerString,2,10);
        print_to_Lcd("kgs",2,14);
        print_to_Lcd("Layer 3",3,0);
        print_to_Lcd(kgsPerLayerString,3,10);
        print_to_Lcd("kgs",3,14);
        y=5;
        delay(5000);
        lcd.clear();
      }
      else{
        lcd.clear();
        print_to_Lcd("A max of 30 allowed",0,1);
        delay(4000);
        y=1;
        NoOfKgsCounter = 0;
        NoOfKgsStr ="";
        lcd.clear();
        
      
      }
   }
     
      
   }
     }
}

while (r!=5)
{
  


  centerTxt(ReadyToStartEnqPt1,row0);    

  print_to_Lcd(ReadyToStartEnqPt2,1,0);
  
  
  char key = customKeypad.getKey();


   if (key){
      
     
     pressed_key = getKey(key);
   
   

   

    if( pressed_key == "ENTER"){
       lcd.clear();
        r = 5;
        Serial.println("Drying begins.");
        //print_to_Lcd("Drying begins!",1,0);
        centerTxt("Drying begins!",row0); 
        delay(800);
        lcd.clear();
        delay(800);
        centerTxt("Drying begins!",row0);
        delay(800);
        lcd.clear();
        delay(800);
        centerTxt("Drying begins!",row0);
        
        delay (2000);
        lcd.clear();
        
        
    } else {
      lcd.clear();
      Serial.println("Invalid Key");
        centerTxt("Invalid Key!!",row1);
      r=1;
      delay (1000);
      lcd.clear();
      }
    

   
     
      
   }
     }
  
  
    
    


     while (counterCountdown>1)
{
/*
 * pending: Rainfall sensor; closing and opening of entrance 
 * 
 * 
 * 
 */


  
      fan1RelayOn = true;
    fan2RelayOn = true;

       while(stepperCounter>=1){
      //stepper1.run();
      //stepperCounter = stepperCounter-1;
      //Serial.println(stepperCounter);
       StepsRequired=10000;
        steppermotor.setSpeed(1000);   
        steppermotor.step(StepsRequired);
        stepperCounter = stepperCounter-1;
        Serial.println(stepperCounter);
       }
      
          


/*=======FUNCTION CALLS FOR CONTROLS AND COMMUNICATION=======*/
dhtTmpReading = dhtTemp ();
dhtHumidityReading = dhtHumidity ();
lm35TempReading = lm35TempR();
//SendMessage(smsRaining);
//SendMessage(smsProcessOver);

/*====END=====*/

   if (lm35TempReading<requiredTemp){
    airHeaterRelayOn = true;    
   }else{
    airHeaterRelayOn = false;
   }

   if(dhtHumidityReading>requiredHumidity){
    //open valve/rotate motor 1: 
    
     while(stepperCounter>=1){
      stepper1.run();
      stepperCounter = stepperCounter-1;
      Serial.println(stepperCounter);
      
       }
       stepper1.moveTo(-4048);
       stepperCounter = 5000;

        //allow 2s for air to be expelled. 
       delay(2000);
       //close valve
       while(stepperCounter>=1){
      stepper1.run();
      stepperCounter = stepperCounter-1;
      Serial.println(stepperCounter);
      
       }
       stepperCounter = 5000;

       
    }else {
    
   Serial.println ("Humidity is within acceptable range");
    //SendMessage(sms1ToSend);
    }
//figure out how to check current motor position inorder to close as an alternative to opening for 2s
    

   
if(fan1RelayOn == true){
  digitalWrite(fan1Relay_pin,HIGH);
}else {
  digitalWrite(fan1Relay_pin,LOW);
}


if(fan2RelayOn == true){
  digitalWrite(fan2Relay_pin,HIGH);
} else {
  digitalWrite(fan2Relay_pin,LOW);
  }
  


if(airHeaterRelayOn==true){
   
    digitalWrite(airHeater_pin,HIGH);
  } else {
    digitalWrite(airHeater_pin,LOW);
  }

 

    //countdown begins
    counterCountdown = counterCountdown-1;
    String CounterCountdownStr = String(counterCountdown);

    centerTxt(CounterCountdownStr + " Seconds",row1); 
    centerTxt("Remaining.....",row2);
    print_to_Lcd(CounterCountdownStr,2,0);
    Serial.println(CounterCountdownStr);
    
    delay(1000);
    lcd.clear();
    

}

 
//print_to_Lcd("Time is over!!",3,0);
 centerTxt("Drying time over!!",row1); 
 SendMessage(smsProcessOver);
 delay(2000);
 centerTxt("Owner notified.",row2);  
delay(2000);


  

  
} // End loop


//getKey function to change print "string type characters"



