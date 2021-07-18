#include<string.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(5, 4, 3, 2, A4, A5);
String ssid     = "Simulator Wifi";  // SSID to connect to
String password = ""; // Our virtual wifi has no password (so dont do your banking stuff on this network)
String host     = "api.thingspeak.com"; // Open Weather Map API
const int httpPort   = 80;
String uri   "/channels/1107821/fields/1.json?api_key=B9WBYR4P29O61CW7&results=2";
int motor = 12;
int red = 10;
int setupESP8266(void) {
  // Start our ESP8266 Serial Communication
  Serial.begin(115200);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 1;
  // Connect to 123D Circuits Simulator Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 2;
  // Open TCP connection to the host:
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 3; 
  return 0;
}
void anydata(void) { 
  // Construct our HTTP call
  String httpPacket = "GET " + uri + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();
  //String httpPacket = "GET https://api.thingspeak.com/channels/1104800/fields/1.json?api_key=CX5ZHR91RBPV0RSW&results=2";
  //int length = httpPacket.length();   
  // Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
delay(10); // Wait a little for the ESP to respond if (!Serial.find(">")) return -1;

 
// Send our http request
  Serial.print(httpPacket);
  delay(10); // Wait a little for the ESP to respond
  String ch;
  ch = Serial.readString();
  Serial.println(ch);
  int point  = 0;
  for(int i = 0 ;i<500 ; i++)
  {
    if(ch[i]=='x')
    {
      point  = i+1;
      break;
    }
  }
  point=ch.length()-7;
  Serial.println(ch[point]);
  if(ch[point]=='0')
  {
    Serial.println("No mask");
    digitalWrite(motor,LOW);
    digitalWrite(red,HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("No mask");
    lcd.setCursor(0,1);
    lcd.print("Please wear Mask!!");
    delay(2000);
  }
  else if (ch[point]=='1')
  {
    Serial.println("Mask is detected..You can enter IN!!");
    digitalWrite(motor,HIGH);
    digitalWrite(red,LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Mask detected..You can enter IN!!");
    lcd.setCursor(0,1);
    lcd.print("You can enter IN!!");
    delay(2000);
  }
  else
    Serial.println("Please scan");
    digitalWrite(motor,LOW);
    digitalWrite(red,LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Please scan ");
    lcd.setCursor(0,1);
    lcd.print("Yourself");
    delay(2000);
  }
  if (!Serial.find("SEND OK\r\n")) return;
}
void setup() {
  setupESP8266();
  pinMode(motor,OUTPUT);
  pinMode(red,OUTPUT);
  lcd.begin(16, 2);
   lcd.setCursor(0,0);          
  }
void loop() {
 anydata();
delay(1000);
}
