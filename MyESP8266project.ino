//Shoeb
//Al Masum
#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<FS.h>

#include <MFRC522.h>
#include <ESP8266HTTPClient.h>
#include <string.h>

//MFRC522 setup
#define RST_PIN         5           // Configurable, see typical pin layout above
#define SS_PIN          4          // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
/*
 * Initialize.
 */
 byte* data;
String a;
String b;
String c;
String d;
// RC522 setup end

#ifndef htmlData_h
#define htmlData_h

const char* dataHTML = "MY HTML CODE";
//const char* dataHTML1 = "MY 2 HTML CODE";
#endif

ESP8266WebServer server;

const char* ssid_file="/ssid.txt";
const char* pass_file="/pass.txt";

const char* MySsid="esp8266";
const char* MyPass="12345678";
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

  const char page[]= R"RAW(
<!DOCTYPE html>
<html>
<head> 
    <style>
    input[type=text],input[type=password],button{
        width: 100%;
        padding: 12px 20px;
        margin: 8px 0;
        display: block;
        border: 1px solid #ccc;
        box-sizing: border-box;
    }
    button:hover {
        opacity: 0.8;
    }

    div{

     width: 80%;
     margin: auto; 
    }
    </style>
</head>
<body>
<form action="/save" method="get">
<div>
  <label for="ssid"> WiFi SSID</label>
  <input type="text" name="ssid" value="" id="ssid" placeholder="SSID" />
</div>

<div>
  <label for="password">WiFi Password</label>
  <input name="pass" value="" type="password" id="pass" placeholder="PASSWORD" />
</div>

<div>
<button type="submit" value="submit">Save Settings</button>
</div>
<H3>Available Hotspots</h3>

</form>
)RAW";

//.................ssid list
WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
int n = WiFi.scanNetworks();
String ssids[10];
String ssid_list="";
char ssids_page[1000];
  if (n){
    for (int i=0;i<n;i++){
Serial.println(WiFi.SSID(i));
ssids[i]=WiFi.SSID(i);
ssid_list += ssids[i]+"<br>";
      }
    }
//..................ssid list
//page +=ssid_list;
char cBuf[55];
ssid_list.toCharArray(cBuf,1000);
Serial.println(ssid_list);
 //snprintf(page,3000,page);
 // sprintf(page,3000,page);
server.send(200,"text/html",page+ssid_list+"</body> </html>");

  }

  void save_page(){
Serial.println("save data ");

Serial.println(server.arg("ssid"));
Serial.println(server.arg("pass"));
      //Format File System    
  SPIFFS.format();
  Serial.println("SPIFFS Formated");
    delay(2000);   
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
delay(2000);
ESP.restart();
    }
 void WiFi_AP(){
WiFi.mode(WIFI_AP_STA);
WiFi.softAPConfig(ip,gateway,subnet);
WiFi.softAP(MySsid,MyPass);

Serial.println(" ");
Serial.println("Wifi AP Started...SSID: ");//+MySsid+"; Pass: "+MyPass);
}
//...........
//............
void ssids(){
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
int n = WiFi.scanNetworks();
String ssids[10];
String ssid_list="";
//const char* dataHTML; 
char ssids_page[1000];
int s1=1233, s2=4567;
  if (n){
    for (int i=0;i<n;i++){
Serial.println(WiFi.SSID(i));
ssids[i]=WiFi.SSID(i);
ssid_list += ssids[i]+"<br>";
      }
    }
  int sec = millis() / 1000;
  int min1 = sec / 60;
  int hr = min1 / 60;
/*
dataHTML=
"<html>\
  <head>\
    <title>YOUR_TITLE</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <p>Uptime: %02d:%02d:%02d</p>\
  </body>\
</html>";
*/
//const char ssids[0]="Wifi1";
//ssids[1]="Wifi2";
String dataHTML=R"RAW(
<html>
<body>

<H1>List of wifi hostspot -</H1>
<h2>
)RAW";

String dataHTML_last=R"RAW(
</h2>

</body>
</html>
)RAW";
dataHTML +=ssid_list+dataHTML_last;
//, hr, min % 60, sec % 60);
 //snprintf(ssids_page,100,dataHTML,sec,min1,hr); //, ssids[0],ssids[1]);
//char charBuf[50];
//return dataHTML;  //.toCharArray(charBuf,1000);
// Serial.println(ssid_list);
  //server.send(200, "text/html", dataHTML);
}
void setup() {
  
  Serial.begin(115200);
  Serial.println("Serial Started");
  
   //WiFi.mode(WIFI_STA);
   ssids();
   
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
WiFi.mode(WIFI_AP_STA);
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
server.on("/ssids",ssids);

server.begin();  
Serial.println("webserver started");

//WiFi_AP();
//

//RFID setup code
}
void loop() {
  // put your main code here, to run repeatedly:
//server.handleClient();
//Serial.println("loop");
//delay(1000);
HTTPClient http;

// RFID code
  // Look for new cards
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  
  Serial.println("first if statement");
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }
  Serial.println("2nd if statement");
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();

  data = mfrc522.uid.uidByte, mfrc522.uid.size;
  for(int i=0; i<4; i++)
  {
    Serial.print(data[i],HEX);
  }
  a=String(data[0],HEX);
b=String(data[1],HEX);
c=String(data[2],HEX);
d=String(data[3],HEX);
a.toUpperCase();
b.toUpperCase();
c.toUpperCase();
d.toUpperCase();
delay(1000);  
  //Data sending 
 //String url ="/index.php?rfid="+ String(a)+String(b)+String(c)+String(d);
String host, url;
host="13.233.130.204";
const int port=80;
url="/index.php?rfid="+ String(a)+String(b)+String(c)+String(d);

 http.begin(host,port, url);
 int httpCode=http.GET();
 String payload=http.getString();
 Serial.println("HTTP Response: ");
 Serial.println(payload); 
 
}

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
