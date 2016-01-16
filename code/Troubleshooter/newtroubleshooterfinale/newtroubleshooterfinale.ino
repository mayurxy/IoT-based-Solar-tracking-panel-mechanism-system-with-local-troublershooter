

 #include <Servo.h> 
 #include <SPI.h>
#include "Ethernet.h"
// Prototypes
void printConfig();
void printEthernetData();
void printIndex();
void printHelp();

EthernetServer server(80);
int statusConfig = 0;

Servo myservo1;
Servo myservo2;  
 
 
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
const int ledPin1 =  PG_1;
const int ledPin2 =  PN_0;
const int ledPin3 =  PF_4;
const int ledPin4 =  PF_0;

int valx;    // variable to read the value from the analog pin 
int valy; 

void setup() {
  Serial.begin(9600);    
 pinMode(RGB_BLUE_LED, OUTPUT);
    pinMode(RGB_GREEN_LED, OUTPUT);
    pinMode(RGB_RED_LED, OUTPUT);
    
      myservo1.attach(PM_7);  // attaches the servo on pin 9 to the servo object 
      myservo2.attach(PF_1);
      
    byte mac[] = { 0x00, 0x1A, 0xB6, 0x02, 0xEA, 0x77 };
    
  pinMode(D1_LED, OUTPUT);
  pinMode(D2_LED, OUTPUT);
   pinMode(D3_LED, OUTPUT);
   pinMode(D4_LED, OUTPUT);
  pinMode(PUSH1, INPUT_PULLUP); // released = HIGH, pressed = LOW
  pinMode(PUSH2, INPUT_PULLUP);

pinMode(ledPin1, OUTPUT);
   pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
     pinMode(ledPin4, OUTPUT);
     
  Serial.println("Connecting to Ethernet....");  
  IPAddress ip = IPAddress(146,252,242,129);
  IPAddress dns = IPAddress(146,252,242,12);
  IPAddress gw = IPAddress(146,252,242,254);
  IPAddress mask = IPAddress(255,255,255,0);

  Ethernet.begin(mac);
  //  Ethernet.begin(0, ip, dns, gw);

  server.begin();

  printEthernetData();
}

EthernetClient client;

void loop() {
  client = server.available();

  if (client) {                             // if you get a client,
    Serial.print("new client on port ");           // print a message out the serial port
    Serial.println(client.port());
    String currentLine = "";                // make a String to hold incoming data from the client
    boolean newConnection = true;     // flag for new connections
    unsigned long connectionActiveTimer;  // will hold the connection start time

    while (client.connected()) {       // loop while the client's connected
      if (newConnection){                 // it's a new connection, so
        connectionActiveTimer = millis(); // log when the connection started
        newConnection = false;          // not a new connection anymore
      }
      if (!newConnection && connectionActiveTimer + 1000 < millis()){ 
        // if this while loop is still active 1000ms after a web client connected, something is wrong
        break;  // leave the while loop, something bad happened
      }


      if (client.available()) {             // if there's bytes to read from the client,    
        char c = client.read();             // read a byte, then
        // This lockup is because the recv function is blocking.
        Serial.print(c);
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {  
            break;         
          } 
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }     
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        if (currentLine.endsWith("GET / ")) {
          statusConfig = 0;
          printIndex();
        }
        if (currentLine.endsWith("GET /config.html ")) {
          statusConfig = 1;
          printConfig();
        }
      //  if (currentLine.endsWith("GET /index.html ")) {
      //    statusConfig = 0;
      //    printIndex();
       // }
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /LED1_H")) {
          //digitalWrite(D1_LED, HIGH);
           boardtest();
          printConfig();
        }         
        if (currentLine.endsWith("GET /LED1_L")) {
          digitalWrite(D1_LED, LOW);
          printConfig();
        }     
        if (currentLine.endsWith("GET /LED2_H")) {
          //digitalWrite(D2_LED, HIGH);
          servohorifwd();
          printConfig();
        }       
        if (currentLine.endsWith("GET /LED2_L")) {
          digitalWrite(D2_LED, LOW);
          printConfig();
        }
        if (currentLine.endsWith("GET /LED3_H")) {
          //boardtest();
          servohorirvs();
          // digitalWrite(D3_LED, HIGH);
          
          printConfig();
        }       
        if (currentLine.endsWith("GET /LED3_L")) {
          //digitalWrite(D3_LED, LOW);
         // panelclock();
          printConfig();
        }
        if (currentLine.endsWith("GET /LED4_H")) {
         // digitalWrite(D4_LED, HIGH);
           panelclock();
          printConfig();
        }       
        if (currentLine.endsWith("GET /LED4_L")) {
          digitalWrite(D4_LED, LOW);
          printConfig();
        }
        if (currentLine.endsWith("GET /LED5_H")) {
         // digitalWrite(D4_LED, LOW);
           panelanti();
          printConfig();
        }
        if (currentLine.endsWith("GET /LED5_L")) {
         // digitalWrite(D4_LED, LOW);
          printConfig();
        }
      
       if (currentLine.endsWith("GET /LED6_H")) {
         // digitalWrite(D4_LED, LOW);
         cellphonenotify();
          printConfig();
        }
       if (currentLine.endsWith("GET /LED6_L")) {
         // digitalWrite(D4_LED, LOW);
          printConfig();
        }
      
      if (currentLine.endsWith("GET /LED7_H")) {
         // digitalWrite(D4_LED, LOW);
         ldrserial();
          printConfig();
        }
       if (currentLine.endsWith("GET /LED7_L")) {
         // digitalWrite(D4_LED, LOW);
          printConfig();
        }
        if (currentLine.endsWith("GET /LED8_H")) {
         // digitalWrite(D4_LED, LOW);
         acclmeterserial();
          printConfig();
        }
       if (currentLine.endsWith("GET /LED8_L")) {
         // digitalWrite(D4_LED, LOW);
          printConfig();
        }
        if (currentLine.endsWith("GET /LED9_H")) {
         // digitalWrite(D4_LED, LOW);
         boostercheck();
          printConfig();
        }
       if (currentLine.endsWith("GET /LED9_L")) {
         // digitalWrite(D4_LED, LOW);
          printConfig();
        }
        
        
        
      }
    }
    // close the connection:
    client.stop();
    //Serial.println("client disonnected");
  }
}

void printIndex()
{
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:    
  //  Serial.println("Index");
  client.println("HTTP/1.1 200 OK");

  client.println("Content-type:text/html");
  client.println();
client.println("<html><head><title>Welcome to local webserver for sun tracking solar panel system</title></head><body align=center>");
  client.println("<h1 align=center><font color=\"green\">The server provides local trouble shooter </font></h1>");
  client.println("</br><font size=\"4px\"><table border=\"0\" align=center width=1200px height=590px>");
  client.println("<tr><td align=center width=375></td><td width=450px align=left valign=\"top\">");


  client.println("<p>This features");
  client.println("the ability to control all the function of system remotely ");
  client.println(".</p></br></br>");
  client.println("<p><a href=\"/config.html\">Click here</a> ");
  client.println("to enter into testing area of system</p>");
  client.println("<td align=cneter width=375></td></tr></table></font></body></html>");

  client.println();

}

void printConfig()
{
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:    
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.println("<html><head><title>Solar Tracking Panel system</title></head><body align=center>");
  client.println("<h1 align=left><font color=\"blue\">Welcome to the Local troubleshooter</font></h1>");

  // the content of the HTTP response follows the header:
  // Added: nicer buttons
    client.print("Board test : <button onclick=\"location.href='/LED1_H'\">START</button>");
  client.println(" <button onclick=\"location.href='/LED1_L'\">STOP</button><br>");
 
  client.print("Servo horizontal motion reverse :<button onclick=\"location.href='/LED2_H'\">Test</button>");
  client.println(" <button onclick=\"location.href='/LED2_L'\">LOW</button><br><br>");

client.print("Servo horizontal motion forward :<button onclick=\"location.href='/LED3_H'\">Test</button>");
  client.println(" <button onclick=\"location.href='/LED3_L'\">STOP</button><br><br>");
  //client.println(" ");
  
  
  client.print("panel inclination clockwise: <button onclick=\"location.href='/LED4_H'\">Test</button>");
  client.println(" <button onclick=\"location.href='/LED4_L'\">STOP</button><br><br>");
 // client.println(" ");
  
  client.print("panel inclination anticlockwise: <button onclick=\"location.href='/LED5_H'\">Test</button>");
  client.println(" <button onclick=\"location.href='/LED5_L'\">STOP</button><br><br>");
 // client.println(" ");
  
  
  client.print("Any suggestion about weather!!: <button onclick=\"location.href='/LED6_H'\">get suggestion</button>");
   client.println(" <button onclick=\"location.href='/LED6_L'\">STOP</button><br><br>");
 // client.println(" ");
  
    client.print("LDR test on serial :<button onclick=\"location.href='/LED7_H'\">Test</button>");
  client.println(" <button onclick=\"location.href='/LED5_L'\">STOP</button><br><br>");
  //client.println(" ");
  
    client.print("Accelerometer on serial: <button onclick=\"location.href='/LED8_H'\">Test</button>");
  client.println(" <button onclick=\"location.href='/LED5_L'\">STOP</button><br><br>");
  //client.println(" ");
  
    client.print("Boosterpack check: <button onclick=\"location.href='/LED9_H'\">Test</button>");
   client.println(" <button onclick=\"location.href='/LED5_L'\">STOP</button><br><br>");
  //client.println(" ");
  
  
  client.println("automatic mode ");
  if(digitalRead(PUSH1))client.print("is ON<br>");
  else client.print("is LOW<br>");
  client.println("trouble shooter ");
  if(digitalRead(PUSH2))client.print("is ON<br>");
  else client.print("is LOW<br>");  
  client.println("<a href=\"/config.html\" >refresh</a> <br>");
  client.println("<a href=\"/index.html\" >home</a> <br>");
  client.println("</body></html>");
  // The HTTP response ends with another blank line:
  client.println();
  // break out of the while loop:
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    switch(inChar) {
    case 'h':
      printHelp();
      break;
    case 'i':
      printEthernetData();
      break;
    default:
      Serial.println();
      Serial.println("Invalid menu option");
    }
  } 
}

void printHelp() {
  Serial.println();
  Serial.println("+++++++++++++++++++++++++++++++++++++");
  Serial.println("Help menu:");
  Serial.println("\th: This help menu");
  Serial.println("\ti: IP address information");
  Serial.println("+++++++++++++++++++++++++++++++++++++");
}

void printHex(int num, int precision) {
  char tmp[16];
  char format[128];

  sprintf(format, "%%.%dX", precision);

  sprintf(tmp, format, num);
  Serial.print(tmp);
}

void printEthernetData() {
  // print your IP address:
  Serial.println();
  Serial.println("IP Address Information:");  
  IPAddress ip = Ethernet.localIP();
  Serial.print("IP Address:\t");
  Serial.println(ip);

  // print your MAC address:

  IPAddress subnet = Ethernet.subnetMask();
  Serial.print("NetMask:\t");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = Ethernet.gatewayIP();
  Serial.print("Gateway:\t");
  Serial.println(gateway);

  // print your gateway address:
  IPAddress dns = Ethernet.dnsServerIP();
  Serial.print("DNS:\t\t");
  Serial.println(dns);

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void servohorifwd()
{
 /*valx = analogRead(potpinx);            // reads the value of the potentiometer (value between 0 and 1023) 
  valx = map(valx, 0, 4096, 0, 179);   


 valy = analogRead(potpiny);            // reads the value of the potentiometer (value between 0 and 1023) 
  valy = map(valy, 0, 4096, 0, 179);
  
   myservo2.write(valx);
  delay(15);      
  */
  valy = 150;
  myservo1.write(valy);
  delay(15);      

//Serial.println(valx);
//Serial.println(valy);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void servohorirvs()
{
 /*valx = analogRead(potpinx);            // reads the value of the potentiometer (value between 0 and 1023) 
  valx = map(valx, 0, 4096, 0, 179);   


 valy = analogRead(potpiny);            // reads the value of the potentiometer (value between 0 and 1023) 
  valy = map(valy, 0, 4096, 0, 179);
  */
 //  myservo2.write(valx);
 // delay(15);      
  
 valy = 30; 
  myservo1.write(valy);
  delay(15);      

//Serial.println(valx);
//Serial.println(valy);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void panelclock(){
valx = 30;
myservo2.write(valx);
  delay(15); 

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void panelanti()
{

valx = 150;
myservo2.write(valx);
  delay(15); 


}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void boardtest()
{


digitalWrite(ledPin1, HIGH);
  delay(500);
  digitalWrite(ledPin2, HIGH);
  delay(500);
  digitalWrite(ledPin3, HIGH);
  delay(500);
  digitalWrite(ledPin4, HIGH);
  delay(500);
  digitalWrite(ledPin1, LOW);
  delay(500);
  digitalWrite(ledPin2, LOW);
  delay(500);
  digitalWrite(ledPin3, LOW);
  delay(500);
  digitalWrite(ledPin4, LOW);
  delay(500);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void acclmeterserial(){

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
  delay(5000);
 // Serial.println("mamuuuuu");


}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ldrserial(){

sensor1Value = analogRead(analogInPin1);            
  // map it to the range of the analog out:
  output1Value = map(sensor1Value, 0, 1023, 0, 255);  
  // change the analog out value:
  sensor2Value = analogRead(analogInPin2);            
  // map it to the range of the analog out:
  output2Value = map(sensor2Value, 0, 1023, 0, 255); 
  
  
    // print the results to the serial monitor:
  Serial.print("sensor1 = " );                       
//  Serial.print(sensor1Value);      
  Serial.print("\t output1 = ");      
  Serial.println(output1Value); 
 Serial.print("sensor2 = " );                       
 // Serial.print(sensor2Value);      
  Serial.print("\t output2 = ");      
  Serial.println(output2Value); 
  

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void boostercheck(){
for (int i = 0; i < 4; i++)
  {
    // blink the BLUE LED 10x with the help of the for-loop! 
    digitalWrite(RGB_BLUE_LED, HIGH); 
    delay(50); 
    digitalWrite(RGB_BLUE_LED, LOW); 
    delay(50);
  }

for (int i = 0; i < 4; i++)
  {
    // blink the BLUE LED 10x with the help of the for-loop! 
    digitalWrite(RGB_RED_LED, HIGH); 
    delay(50); 
    digitalWrite(RGB_RED_LED, LOW); 
    delay(50);
  }
  
  for (int i = 0; i < 4; i++)
  {
    // blink the BLUE LED 10x with the help of the for-loop! 
    digitalWrite(RGB_GREEN_LED, HIGH); 
    delay(50); 
    digitalWrite(RGB_GREEN_LED, LOW); 
    delay(50);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void cellphonenotify()
{



}



