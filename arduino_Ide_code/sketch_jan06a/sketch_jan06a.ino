#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <HttpClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#define LED1 D1
#define LED2 D2
#define RELLAY D3
//Variables
int i = 0;
int h = 0;
int statusCode;
const char* ssid = "text";
const char* passphrase = "text";
String st;
String content;
int onbutton=D7;
int offbutton=D6;
int onvalue=0;
int offvalue=0;
int add=100;
int add2=102;
String offline_status;
String serverName = "http://student1.erisetechnology.com/nodemcu/nodemcu_api.php";
//String switch_off_status_api = "http://student1.erisetechnology.com/nodemcu/nodemcu_api.php";
String switch_off_status_api2 = "http://student1.erisetechnology.com/nodemcu/device_api.php";
//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);
 
//Establishing Local server at port 80 whenever required
ESP8266WebServer server(80);
HTTPClient http;
WiFiClient client;




void setup() {
  EEPROM.begin(512);
  //Serial.begin(9600);
//  pinMode(LED2, OUTPUT);
//  pinMode(RELLAY, OUTPUT);
Serial.begin(115200);
  pinMode(onbutton, INPUT_PULLUP);
  pinMode(offbutton, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, LOW);
  pinMode(RELLAY, OUTPUT);
  digitalWrite(RELLAY, LOW);
  EEPROM.write(add, 0);
  
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  
  Serial.println("Communication Started \n\n");
  pinMode(LED_BUILTIN, OUTPUT);

 Serial.println("Reading EEPROM ssid");
 
  String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");
 
  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);
 
 
  WiFi.begin(esid.c_str(), epass.c_str());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  if (testWifi())
  {
    Serial.println("Succesfully Connected!!!");
    return;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    launchWeb();
    setupAP();// Setup HotSpot
    //server.handleClient();
    //delay(3000000000000);
  }

  Serial.print("Connecting");
  while ( h < 150) {
  
    delay(200);
    Serial.print(h);
    h++;
    server.handleClient();
    digitalWrite(LED1, HIGH);
    delay(200);
    digitalWrite(LED1, LOW);
  }
//  else
//  {
    Serial.print("connected");
    delay(500);
    //server.handleClient();
  //}

}
void loop() {

  onvalue = digitalRead(onbutton);
  offvalue = digitalRead(offbutton);

  
   if(WiFi.status() == WL_CONNECTED)
  {
    int a=EEPROM.read(add);
    //Serial.print(a);
    if(a==1)
    {
      http.begin(client, switch_off_status_api2);
           http.addHeader("Content-Type", "application/x-www-form-urlencoded");
           String httpRequestData = "device_status=1"; 
           int httpResponseCode = http.POST(httpRequestData);
           Serial.print(httpResponseCode);
           EEPROM.write(add, 7);
           delay(3000);
      
    }
    if(a==0)
    {
      http.begin(client, switch_off_status_api2);
           http.addHeader("Content-Type", "application/x-www-form-urlencoded");
           String httpRequestData = "device_status=0"; 
           int httpResponseCode = http.POST(httpRequestData);
           Serial.print(httpResponseCode);
           EEPROM.write(add, 7);
           delay(3000);
    }
    Serial.print("I am online");
    digitalWrite(LED1, HIGH);
    online();
    
  }
  else
  {
    digitalWrite(LED1, LOW);
    offline();
    Serial.print("");
    delay(500);
  }


  
  
  


}
void offline(){
 // server.handleClient();
 WiFi.softAPdisconnect (true);
 String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  //Serial.println();
 // Serial.print("SSID: ");
  //Serial.println(esid);
  //Serial.println("Reading EEPROM pass");
 
  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  //Serial.print("PASS: ");
  //Serial.println(epass);
 
 
  //WiFi.begin(esid.c_str(), epass.c_str());
  int numberOfNetworks = WiFi.scanNetworks();
 for(int i =0; i<numberOfNetworks; i++){
 
      //Serial.print("Network name: ");
      //Serial.println(WiFi.SSID(i));
      //Serial.print("Signal strength: ");
      //Serial.println(WiFi.RSSI(i));
      //Serial.println("-----------------------");
      String find_ssid=WiFi.SSID(i);
      if(find_ssid==esid.c_str())
      {
        Serial.println("matches find");
        WiFi.begin(esid.c_str(), epass.c_str());
        delay(10000);
        if (testWifi())
          {
            Serial.println("Succesfully Connected!!!");
            return;
          }
      }
     
 
  }
  Serial.print(" Work on offline \n");
       //onvalue = digitalRead(onbutton);
       if(onvalue==LOW)
         {
           digitalWrite(LED2, HIGH);
           digitalWrite(RELLAY, HIGH);
           Serial.print("on \n");
           EEPROM.write(add, 1);
          
         }
     //offvalue = digitalRead(offbutton);
      if(offvalue==LOW)
        {
         digitalWrite(LED2, LOW);
         digitalWrite(RELLAY, LOW);
         EEPROM.write(add, 0);
        
        }
  
}
void online(){
  if(onvalue==LOW || offvalue==LOW)
  {
     switch_cntl();
  }
  else
  {
    web_cntl();
  }
  
}

void switch_cntl() {
   onvalue = digitalRead(onbutton);
       Serial.print("light pressed");
       if(onvalue==LOW)
         {
           digitalWrite(LED2, HIGH);
           digitalWrite(RELLAY, HIGH);
           Serial.print("on");
           http.begin(client, switch_off_status_api2);
           http.addHeader("Content-Type", "application/x-www-form-urlencoded");
           String httpRequestData = "device_status=1"; 
           int httpResponseCode = http.POST(httpRequestData);
           Serial.print(httpResponseCode);
           offline_status="on";
         }
     offvalue = digitalRead(offbutton);
      if(offvalue==LOW)
        {
         digitalWrite(LED2, LOW);
         digitalWrite(RELLAY, LOW);
         Serial.print("off");
         http.begin(client, switch_off_status_api2);
           http.addHeader("Content-Type", "application/x-www-form-urlencoded");
           String httpRequestData = "device_status=0"; 
           int httpResponseCode = http.POST(httpRequestData);
           Serial.print(httpResponseCode);
           offline_status="off";
        }
        delay(500);
}
void web_cntl()
{
  
  String serverPath = serverName + "?id=1";
  http.begin(client,serverPath.c_str());
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.GET(); 
  if (httpResponseCode>0) 
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    String device_status = doc["device_status"];
    //Serial.println(device_status);
    String check="1";
    String check2="0";
    if(device_status == check)
    {
      digitalWrite(LED2, HIGH);
      digitalWrite(RELLAY, HIGH);
      DynamicJsonDocument switch_off(1024);
      Serial.println(device_status);
      offline_status="on";
      //delay(500);
    }
    if(device_status == check2)
    {
      digitalWrite(LED2, LOW);
      digitalWrite(RELLAY, LOW);
      
      Serial.println(device_status);
      offline_status="off";
    }
    
    
    delay(1000);
  }
  else
  {
    Serial.println(httpResponseCode);
    Serial.println("Failed to upload values. \n");
    //return;
    delay(1000);
  }

}
bool testWifi(void)
{
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while ( c < 40 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(200);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}
 
void launchWeb()
{
  Serial.println("hi web");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started");
}
 
void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);
 
    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  WiFi.softAP("MPAP Soft World", "123456789");
  Serial.println("softap");
  launchWeb();
  Serial.println("over");
}
 
void createWebServer()
{
 {
    server.on("/", []() {
 
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      
      
     content ="<!DOCTYPE HTML>\r\n<html>";
      content +="<head>";
      content +="<meta charset='utf-8'>";
      content +="<meta name='viewport' content='width=device-width, initial-scale=1'>";
      content +="<title>Change Password</title>";
      content +="<style type='text/css'>";
      content +="@import url(https://fonts.googleapis.com/css?family=Open+Sans);";
      content +=".btn { display: inline-block; *display: inline; *zoom: 1; padding: 4px 10px 4px; margin-bottom: 0; font-size: 13px; line-height: 18px; color: #333333; text-align: center;text-shadow: 0 1px 1px rgba(255, 255, 255, 0.75); vertical-align: middle; background-color: #f5f5f5; background-image: -moz-linear-gradient(top, #ffffff, #e6e6e6); background-image: background-image: -webkit-linear-gradient(top, #ffffff, #e6e6e6); background-image: -o-linear-gradient(top, #ffffff, #e6e6e6); background-image: linear-gradient(top, #ffffff, #e6e6e6); background-repeat: repeat-x; border-color: #e6e6e6 #e6e6e6 #e6e6e6; border-color: rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.25); border: 1px solid #e6e6e6; -webkit-border-radius: 4px; -moz-border-radius: 4px; border-radius: 4px; -webkit-box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.2), 0 1px 2px rgba(0, 0, 0, 0.05); -moz-box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.2), 0 1px 2px rgba(0, 0, 0, 0.05); box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.2), 0 1px 2px rgba(0, 0, 0, 0.05); cursor: pointer; *margin-left: .3em; }";
      content +=".btn:hover, .btn:active, .btn.active, .btn.disabled, .btn[disabled] { background-color: #e6e6e6; }";
      content +=".btn-large { padding: 9px 14px; font-size: 15px; line-height: normal; -webkit-border-radius: 5px; -moz-border-radius: 5px; border-radius: 5px; }";
      content +=".btn:hover { color: #333333; text-decoration: none; background-color: #e6e6e6; background-position: 0 -15px; -webkit-transition: background-position 0.1s linear; -moz-transition: background-position 0.1s linear; -ms-transition: background-position 0.1s linear; -o-transition: background-position 0.1s linear; transition: background-position 0.1s linear; }";
      content +=".btn-primary, .btn-primary:hover { text-shadow: 0 -1px 0 rgba(0, 0, 0, 0.25); color: #ffffff; }";
      content +=".btn-primary.active { color: rgba(255, 255, 255, 0.75); }";
      content +=".btn-primary { background-color: #4a77d4; background-image: -moz-linear-gradient(top, #6eb6de, #4a77d4); background-image: -ms-linear-gradient(top, #6eb6de, #4a77d4); background-image: -webkit-linear-gradient(top, #6eb6de, #4a77d4); background-image: -o-linear-gradient(top, #6eb6de, #4a77d4); background-image: linear-gradient(top, #6eb6de, #4a77d4); background-repeat: repeat-x;   border: 1px solid #3762bc; text-shadow: 1px 1px 1px rgba(0,0,0,0.4); box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.2), 0 1px 2px rgba(0, 0, 0, 0.5); }";
      content +=".btn-primary:hover, .btn-primary:active, .btn-primary.active, .btn-primary.disabled, .btn-primary[disabled] { filter: none; background-color: #4a77d4; }";
      content +=".btn-block { width: 100%; display:block; }";
      content +="* { -webkit-box-sizing:border-box; -moz-box-sizing:border-box; -ms-box-sizing:border-box; -o-box-sizing:border-box; box-sizing:border-box; }";
      content +="html { width: 100%; height:100%; overflow:hidden; }";
      content +="body { width: 100%;height:100%;font-family: 'Open Sans', sans-serif;background: #092756;background: -moz-radial-gradient(0% 100%, ellipse cover, rgba(104,128,138,.4) 10%,rgba(138,114,76,0) 40%),-moz-linear-gradient(top,  rgba(57,173,219,.25) 0%, rgba(42,60,87,.4) 100%), -moz-linear-gradient(-45deg,  #670d10 0%, #092756 100%);background: -webkit-radial-gradient(0% 100%, ellipse cover, rgba(104,128,138,.4) 10%,rgba(138,114,76,0) 40%), -webkit-linear-gradient(top,  rgba(57,173,219,.25) 0%,rgba(42,60,87,.4) 100%), -webkit-linear-gradient(-45deg,  #670d10 0%,#092756 100%);background: -o-radial-gradient(0% 100%, ellipse cover, rgba(104,128,138,.4) 10%,rgba(138,114,76,0) 40%), -o-linear-gradient(top,  rgba(57,173,219,.25) 0%,rgba(42,60,87,.4) 100%), -o-linear-gradient(-45deg,  #670d10 0%,#092756 100%);background: -ms-radial-gradient(0% 100%, ellipse cover, rgba(104,128,138,.4) 10%,rgba(138,114,76,0) 40%), -ms-linear-gradient(top,  rgba(57,173,219,.25) 0%,rgba(42,60,87,.4) 100%), -ms-linear-gradient(-45deg,  #670d10 0%,#092756 100%);background: -webkit-radial-gradient(0% 100%, ellipse cover, rgba(104,128,138,.4) 10%,rgba(138,114,76,0) 40%), linear-gradient(to bottom,  rgba(57,173,219,.25) 0%,rgba(42,60,87,.4) 100%), linear-gradient(135deg,  #670d10 0%,#092756 100%);filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#3E1D6D', endColorstr='#092756',GradientType=1 );}";
      content +=".login { position: relative;top: 30%;left: 50%;margin: -150px 0 0 -150px;width:300px;height:300px;}";
      content +=".login h2 { color: #fff; text-shadow: 0 0 10px rgba(0,0,0,0.3); letter-spacing:1px; text-align:center; }";
      content +="input { width: 100%; margin-bottom: 10px; background: rgba(0,0,0,0.3);border: none;outline: none;padding: 10px;font-size: 13px;color: #fff;text-shadow: 1px 1px 1px rgba(0,0,0,0.3);border: 1px solid rgba(0,0,0,0.3);border-radius: 4px;box-shadow: inset 0 -5px 45px rgba(100,100,100,0.2), 0 1px 1px rgba(255,255,255,0.2);-webkit-transition: box-shadow .5s ease;-moz-transition: box-shadow .5s ease;-o-transition: box-shadow .5s ease;-ms-transition: box-shadow .5s ease;transition: box-shadow .5s ease;}input:focus { box-shadow: inset 0 -5px 45px rgba(100,100,100,0.4), 0 1px 1px rgba(255,255,255,0.2); }.network{position: relative;top: 20%;left: 50%;margin: -150px 0 0 -150px;width:300px;height:250px;color: skyblue;}.footer{position: relative;top: 60%;left: 50%;right: 50%;margin: -150px 0 0 -150px;width:30%;height:auto;color: skyblue;}h3{color: #fff; text-shadow: 0 0 10px rgba(0,0,0,0.3); letter-spacing:1px; text-align:center; }";
      content +="</style>";
      content +="</head><body>";
      content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
      content += ipStr;
      content += "<p>";
      content += st;
      
      content +="</p><div class='login'><h3>Welcome to Nodemcu</h3><h2>Change Your Pasword</h2><form method='get' action='setting'><input type='text' name='ssid' placeholder='Enter SSID' required='required' /><input type='password' name='pass' placeholder='Enter Password' required='required' /><button type='submit' class='btn btn-primary btn-block btn-large'>Save Password</button></form></div><footer class='footer'>Copyright © <a style='text-decoration: none;color: white;' href='https://www.mpap.in' target='blank'>MPAP Group </a>&nbsp; 2022</footer></body>";
      content +="</html>";
      server.send(200, "text/html", content);
    });
    server.on("/scan", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
 
      content = "<!DOCTYPE HTML>\r\n<html>go back";
      server.send(200, "text/html", content);
    });
 
    server.on("/setting", []() {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");
 
        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
        EEPROM.commit();
 
        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.reset();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);
 
    });
  } 
}
