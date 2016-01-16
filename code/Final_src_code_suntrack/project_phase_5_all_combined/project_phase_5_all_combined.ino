// __@ Author: Mayur Talole
// project on sun tracking solar panel with local trouble shooter

 #include <Servo.h> 
 #include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <Temboo.h>
#include "TembooAccount.h"


Servo myservo1;
Servo myservo2;  
 
 const int buttonPin = PUSH2;     // the number of the pushbutton pin
const int ledPin =  GREEN_LED;
int buttonState = 0;

 int val= 30;

// These constants won't change.  They're used to give names
// to the pins used:
//const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
//const int analogOutPin = 9; // Analog output pin that the LED is attached to

const int xpin = PB_4;                  // x-axis of the accelerometer
const int ypin = PB_5;                  // y-axis
const int zpin = PK_0;                  // z-axis (only on 3-axis models)

const int analogInPin1= PE_1;
const int analogInPin2= PE_2;
int sensor1Value = 0;        // value read from the pot
int output1Value = 0;        // value output to the PWM (analog out)
int sensor2Value = 0;        // value read from the pot
int output2Value = 0; 

int xangle=0;
int yangle=0;
int zangle=0;

int xangle_val=0;
int yangle_val=0;
int zangle_val=0;

#define RGB_BLUE_LED PM_0 
#define RGB_GREEN_LED PK_5
#define RGB_RED_LED PK_4

String conditiontex = "";
String content = "";
// the address for which a weather forecast will be retrieved
// you can change this to be whatever you like
String ADDRESS_FOR_FORECAST = "Dallas, Texas";
String address = "";
int temp= 0;
int numRuns = 1;   // execution count, so that this doesn't run forever
int maxRuns = 10;  // max number of times the Yahoo WeatherByAddress Choreo should be run

 
int potpinx = PD_2;  // analog pin used to connect the potentiometer
int potpiny = PK_1;  // analog pin used to connect the potentiometer

int centrepin =PD_4;
int valx;    // variable to read the value from the analog pin 
int valy; 


WiFiClient client;
WiFiClient client1;

int i=0;
/************************************************************************************************/

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  // myservo1.attach(PM_7);
  // myservo2.attach(PM_3);

      myservo1.attach(PM_7);  // attaches the servo on pin 9 to the servo object 
      myservo2.attach(PF_1);


pinMode(buttonPin, INPUT_PULLUP);   
 pinMode(ledPin, OUTPUT);
 
 

    pinMode(RGB_BLUE_LED, OUTPUT);
    pinMode(RGB_GREEN_LED, OUTPUT);
    pinMode(RGB_RED_LED, OUTPUT);
    
    int wifiStatus = WL_IDLE_STATUS;

  // determine if the WiFi Shield is present.
  Serial.print("\n\WIFI Booster :");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("No connection established");
    
    // if there's no WiFi shield, stop here.
    while(true);
  }
 
  while(wifiStatus != WL_CONNECTED) {
    Serial.print("WiFi to MTASUS:");
    wifiStatus = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    if (wifiStatus == WL_CONNECTED) {
      Serial.println("connected ");
    } else {
      Serial.println("connection failed");
    }
    delay(500);
  }
  
}
/***************************************************************************************/
void loop() {
  // read the analog in value:
  sensor1Value = analogRead(analogInPin1);            
  // map it to the range of the analog out:
  output1Value = map(sensor1Value, 0, 1023, 0, 255);  
  // change the analog out value:
  sensor2Value = analogRead(analogInPin2);            
  // map it to the range of the analog out:
  output2Value = map(sensor2Value, 0, 1023, 0, 255);  
  
//  analogWrite(analogOutPin, outputValue);           
//analogWrite(analogOutPin, outputValue);           

for (int i = 0; i < 4; i++)
  {
    // blink the BLUE LED 10x with the help of the for-loop! 
    digitalWrite(RGB_BLUE_LED, HIGH); 
    delay(5); 
    digitalWrite(RGB_BLUE_LED, LOW); 
    delay(5);
  }


  // print the results to the serial monitor:
  Serial.print("sensor1 = " );                       
//  Serial.print(sensor1Value);      
  Serial.print("\t output1 = ");      
  Serial.println(output1Value); 
 Serial.print("sensor2 = " );                       
 // Serial.print(sensor2Value);      
  Serial.print("\t output2 = ");      
  Serial.println(output2Value);  

  // wait 10 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
//  delay(1000); 
 
 
 /*myservo1.write(val); 
 delay(1000);
  myservo2.write(val); 
delay(1000);

  delay(1000); 
 myservo1.write(0); 
 delay(1000);
  myservo2.write(0); 
delay(1000);
*/


/**********************************************************************************************/

playwithjoy();
  
  /*******************************************************************************************/

for (int i = 0; i < 4; i++)
  {
    // blink the BLUE LED 10x with the help of the for-loop! 
    digitalWrite(RGB_RED_LED, HIGH); 
    delay(5); 
    digitalWrite(RGB_RED_LED, LOW); 
    delay(5);
  }
  
  
  
  for (int i = 0; i < 4; i++)
  {
    // blink the BLUE LED 10x with the help of the for-loop! 
    digitalWrite(RGB_GREEN_LED, HIGH); 
    delay(5); 
    digitalWrite(RGB_GREEN_LED, LOW); 
    delay(5);
  }
  
  acclr_tm4c();
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW); 
    cellphonenotification();  
}
  
  
  
  
}

void acclr_tm4c()
{

 // print the sensor values:
 xangle_val = (((int) analogRead(xpin)) );
  //Serial.print(xangle_val);
  // print a tab between values:
  Serial.print("\t");
 yangle_val=(((int) analogRead(ypin)));
  // print a tab between values:
  Serial.print("\t");
  zangle_val = (((int) analogRead(zpin)) );
  Serial.println();
  // delay before next reading:
  
  xangle =map(xangle_val, 0, 2048, 0, 180);
  yangle =map(yangle_val, 0, 2048, 0, 180);
  zangle =map(zangle_val, 0, 2048, 0, 180);
 
  Serial.print("xangle>>>>  ");
  Serial.println(xangle);
  
  yangle =map(yangle_val, 0, 2048, 0, 180);
  Serial.print("yangle>>>>  ");
  Serial.println(yangle);
  
  zangle =map(zangle_val, 0, 2048, 0, 180);
  Serial.print("zangle>>>>  ");
  Serial.println(zangle);
  delay(2);
 // Serial.println("mamuuuuu");

}

void cellphonenotification(){


if (numRuns <= maxRuns) {
      
    // print status
    Serial.println("Running GetWeatherByAddress - Run #" + String(numRuns++) + "...");

    // create a TembooChoreo object to send a Choreo request to Temboo
    TembooChoreo GetWeatherByAddressChoreo(client);
    
    // add your Temboo account info
    GetWeatherByAddressChoreo.setAccountName(TEMBOO_ACCOUNT);
    GetWeatherByAddressChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    GetWeatherByAddressChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // set the name of the Choreo we want to run
    GetWeatherByAddressChoreo.setChoreo("/Library/Yahoo/Weather/GetWeatherByAddress");
    
    // set Choreo inputs; in this case, the address for which to retrieve weather data
    // the Temboo client provides standardized calls to 100+ cloud APIs
    GetWeatherByAddressChoreo.addInput("Address", ADDRESS_FOR_FORECAST);

    // add an output filter to extract the name of the city
    GetWeatherByAddressChoreo.addOutputFilter("city", "/rss/channel/yweather:location/@city", "Response");
    
    // add an output filter to extract the current temperature
    GetWeatherByAddressChoreo.addOutputFilter("temperature", "/rss/channel/item/yweather:condition/@temp", "Response");
    //address = readString();
    // add an output filter to extract the date and time of the last report
    GetWeatherByAddressChoreo.addOutputFilter("date", "/rss/channel/item/yweather:condition/@date", "Response");

     GetWeatherByAddressChoreo.addOutputFilter("text", "/rss/channel/item/yweather:condition/@text", "Response");
  
    // GetWeatherByAddressChoreo.addOutputFilter("humidity", "/rss/channel/item/yweather:atmosphere/@humidity", "Response");
  
    // run the Choreo 
    GetWeatherByAddressChoreo.run();
        
    // when the Choreo results are available, print them to the serial monitor
    while(GetWeatherByAddressChoreo.available()) {
      char c = GetWeatherByAddressChoreo.read();    
      Serial.print(c);
      content.concat(c);  
  }
    
  

  if (content != "") {
    Serial.println("contents are");
  address.concat(content[65]);
   address.concat(content[66]);
 
// Serial.println(content[65]);
 //Serial.println(content[66]);
 for(i=75; i<88; i++)
  conditiontex.concat(content[i]);
  
  /*
  conditiontex.concat(content[74]);
  conditiontex.concat(content[75]);
  conditiontex.concat(content[76]);
  conditiontex.concat(content[77]);
  conditiontex.concat(content[78]);
  conditiontex.concat(content[79]);
   */
 Serial.println(address);
  //Serial.println(conditiontex);
 
  temp  = address.toInt();
  Serial.println("temperature::::: ");
Serial.println(temp);  
Serial.println("conditions::::: ");
Serial.println(conditiontex);
}

/*
if (conditiontex != "") {
    Serial.println("conditiontex are");
  address.concat(conditiontex[65]);
   address.concat(conditiontex[66]);
 
// Serial.println(content[65]);
 //Serial.println(content[66]);
 
 Serial.println(address);
 
  temp  = address.toInt();
  Serial.println("condition::::: ");
Serial.println(temp);  
}
*/

   
  Serial.println("Running SendSMS - Run #" + String(numRuns++));
    
    TembooChoreo SendSMSChoreo(client1);

    // Invoke the Temboo client
    SendSMSChoreo.begin();

    // Set Temboo account credentials
    SendSMSChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendSMSChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendSMSChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // Set Choreo inputs
    SendSMSChoreo.addInput("AuthToken", "8ddc1230a698a6a8845e370faeb56aa9");
    SendSMSChoreo.addInput("To", "+14696643694");
    SendSMSChoreo.addInput("From", "+14155992671");
    
    if ((temp > 70) && (conditiontex == "Mostly Cloudy"))
    {
    SendSMSChoreo.addInput("Body", "weather looks fine. no need to use sweater. But you may need umbrella today");
   // SendSMSChoreo.addInput("Body", address);
      
  }
  else if (temp > 70)
    {
    SendSMSChoreo.addInput("Body", "weather looks fine. no need to use sweater. ");
   // SendSMSChoreo.addInput("Body", address);
      
  }
  
    else if (temp < 70)
    {
    SendSMSChoreo.addInput("Body", "weather is cold. use sweater!!");
    }
    
    
    SendSMSChoreo.addInput("AccountSID", "AC6a5d94e8437d02c4db06962ee157d092");
    
    // Identify the Choreo to run
    SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");
    
    // Run the Choreo; when results are available, print them to serial
    SendSMSChoreo.run();
    
    while(SendSMSChoreo.available()) {
      char c = SendSMSChoreo.read();
      Serial.print(c);
    }
    SendSMSChoreo.close();
  
 GetWeatherByAddressChoreo.close();
  }
 // Serial.println("Waiting...");
 // delay(30000); // wait 30 seconds between SendSMS calls
Serial.println(address);
  Serial.println("Waiting...");
  Serial.println("");
  delay(300); 



}


void playwithjoy()
{
 valx = analogRead(potpinx);            // reads the value of the potentiometer (value between 0 and 1023) 
  valx = map(valx, 0, 4096, 0, 179);   


 valy = analogRead(potpiny);            // reads the value of the potentiometer (value between 0 and 1023) 
  valy = map(valy, 0, 4096, 0, 179);
  
   myservo2.write(valx);
  delay(2);      
  
  
  myservo1.write(valy);
  delay(2);      

Serial.println(valx);
Serial.println(valy);


}
