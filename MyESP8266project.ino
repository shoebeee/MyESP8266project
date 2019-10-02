//Shoeb
//Al Masum
#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<FS.h>
ESP8266WebServer server;

const char* ssid_file="/ssid.txt";
const char* pass_file="/pass.txt";

const char* MySsid="esp8266";
const char* pass="12345678";
char* ssid_read;
char* pass_read;
String ssd_r="";
String pass_r="";
int success_write=0;

IPAddress ip(192,168,10,2);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);

char page[3000];

void homepage(){
  //char page[] PROGMEM = R"=====(

  char page[]= R"RAW(

<!DOCTYPE html>
<html>
<head> </head>
<body>
<form action="/save" method="get">
<div>
  <label for="ssid"> SSID</label>
  <input name="ssid" value="" id="ssid" placeholder="SSID" style="width:80%;" style="width:20%;" />
</div>

<div>
  <label for="password">PASSWORD</label>
  <input name="pass" value="" type="password" id="pass" placeholder="PASSWORD" style="width:80%;" style="width:20%;" />
</div>

<div>
<input type="submit" value="submit" style="width:80%;" style="width:20%;">
</div>

</form>
</body>
</html>
  )RAW";

 //snprintf(page,3000,page);
  
server.send(200,"text/html",page);

  }

  void save_page(){
Serial.println("save data ");

Serial.println(server.arg("ssid"));
Serial.println(server.arg("pass"));
      //Format File System    
  SPIFFS.format();
  Serial.println("SPIFFS Formated");
       
File f1=SPIFFS.open(ssid_file,"w");
File f2=SPIFFS.open(pass_file,"w");

if (!f1 || !f2){
  Serial.println("File open error");
  }
  else {
    Serial.println("Writing Data");
    f1.print(server.arg("ssid"));
    f1.close();
    f2.print(server.arg("pass"));
    f2.close();
    Serial.println("write to file succesful");
    success_write=1;
    }

    
char save_page[]=R"RAW(
<html>
<h1>Save page </h1>
</html>
)RAW";
server.send(200,"text/html",save_page);
ESP.restart();
    }
 void WiFi_AP(){
WiFi.mode(WIFI_AP_STA);
WiFi.softAPConfig(ip,gateway,subnet);
WiFi.softAP(MySsid,pass);

Serial.println(" ");
Serial.println("Wifi AP Started...");
}
  

void setup() {
  delay (1000);
  Serial.begin(115200);
  Serial.println("Serial Started");
  //Initializing File System

if(SPIFFS.begin()){
  Serial.println("SPIFFS Initialization... ok");
  }
  else {
  Serial.println("SPIFFS Failed");  
    }
  /*  if (! (SPIFFS.exists(ssid_file) ) ){//|| SPIFFS.exists(pass_file) ){
      //Format File System    
      SPIFFS.format();
       Serial.println("SPIFFS Formated");
      }
*/
      
//Open config files
File f1 =SPIFFS.open(ssid_file,"r");
File f2 =SPIFFS.open(pass_file,"r");
if( !f1 || !f2 ){
  Serial.println("File read error");
  WiFi_AP();
  }
  else {
    Serial.println("reading data from file");
    while(f1.available()) {
    ssd_r+=char(f1.read());
      }
    f1.close();

    while(f2.available()) {
    pass_r+=char(f2.read());
    }
    f2.close();
    
    Serial.println("\nReading SSID: ["+ssd_r+"]"+", Pass: ["+pass_r+"]");
    Serial.println("File closed");
  }
//wifi begin 
int wifiAttempt=0; 
unsigned long timeMillis=0;
WiFi.begin(ssd_r, pass_r);
Serial.println("WiFi.begin("+ssd_r+", "+pass_r+");");
//timeMillis=millis();
//while (WiFi.status() != WL_CONNECTED){
//Serial.print(".");
    //timeMillis2=millis;
  //if(millis()-timeMillis>3000) {
    //break;
    //}
  //  delay(100);
 // }
  delay(5000);
  if (WiFi.status() != WL_CONNECTED){
    Serial.println("Wifi connection failed. Set correct AP and Pass");
    WiFi_AP();
    }
    else {
      Serial.println("wifi successful...");
      Serial.println("IP Address: "+WiFi.localIP());
      }
Serial.println(WiFi.localIP());

server.on("/",homepage);
server.on("/save",save_page);

server.begin();  
Serial.println("webserver started");

//WiFi_AP();
//

//RFID setup code
}
void loop() {
  // put your main code here, to run repeatedly:
server.handleClient();
// RFID code
}
