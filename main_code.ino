//lcd + keypad

#include <LiquidCrystal_I2C.h>
#include <Keypad.h>  //install keypad library by Mark Stanley and Alexander Brevig

/*circuit: pin 2 corresopinds to f2 side on keypad. pin 10 is connected to extreme left of the keypad. other pins in between follow each other*/
LiquidCrystal_I2C lcd(0x27,20,4);
const byte ROWS = 5; //5 rows
const byte COLS = 4; //4 columns
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


byte rowPins[ROWS] = {2,3,4,5,6}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {10,9,8,7}; //connect to the column pinouts of the kpd


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
float lm35Reading = 0;
float dhtTmpReading = 5;
float dhtHumidityReading = 6;
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
int counterCountdown = 10;
String NoOfKgsStr = "";
String MoisContentStr = "";
int NoOfKgsCounter = 0;
int MoistureContentCounter = 0;
String distrAdvice = "Distribution: ";
String ReadyToStartEnqPt1 = "Ready?";
String ReadyToStartEnqPt2 = "Press Enter to Start";


//pins
int valve1_pin = 1;
int valve2_pin =11;
int airHeater_pin=13;
int fan1Relay_pin=12;
int fan2Relay_pin = 12;







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
  Serial.begin(9600);  
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on


  pinMode(valve1_pin, OUTPUT);
  pinMode(valve2_pin, OUTPUT);
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



  

//  char key = customKeypad.getKey();
//      
//   if (key){
//    Serial.print("Key: "); 
//    char* pressed_key = getKey(key);
//    Serial.println(pressed_key);
//    print_to_Lcd("maize", 3, 0);
    //return pressed_key;
   }
 
  
 //char* validKey = check_Key_for_crop_selection_and_validity();
  
  
  



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
  
  



  

    fan1RelayOn = true;
    fan2RelayOn = true;

    
    
  
   if (lm35Reading<requiredTemp){
    airHeaterRelayOn = true;
   }else{
    airHeaterRelayOn = false;
   }

   if(dhtHumidityReading>requiredHumidity){
    valve1Open = true;
    valve2Open = true;
    }else {
    valve1Open = false;
    valve2Open = false;
    }

    
valve1_pin = 1;
valve2_pin =11;
airHeater_pin=13;
fan1Relay_pin=12;
fan2Relay_pin = 12;
   
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
  
if(fan2RelayOn == true){
  digitalWrite(fan2RelayOn,HIGH);
}

if(airHeaterRelayOn==true){
   
    digitalWrite(airHeater_pin,HIGH);
  } else {
    digitalWrite(airHeater_pin,LOW);
  }

  if (valve1Open=true)
  {
    digitalWrite(valve1_pin,HIGH);
  } else {
     digitalWrite(valve1_pin,LOW);
    }

     if (valve2Open=true)
  {
    digitalWrite(valve2_pin,HIGH);
  } else {
     digitalWrite(valve2_pin,LOW);
    }

    //countdown begins
    counterCountdown = counterCountdown-1;
    String CounterCountdownStr = String(counterCountdown);

    centerTxt(CounterCountdownStr + " Seconds",row1); 
    centerTxt("Remaining.....",row2);
    //print_to_Lcd(CounterCountdownStr,2,0);
    Serial.println(CounterCountdownStr);
    
    delay(1000);
    lcd.clear();
    

}

 
//print_to_Lcd("Time is over!!",3,0);
 centerTxt("Drying time over!!",row1); 
 centerTxt("Owner notified.",row2); 
delay(2000);

  

  
} // End loop


//getKey function to change print "string type characters"



