//Shoeb
//Al Masum
#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

const char* MySsid="esp8266";
const char* pass="12345678";
IPAddress ip(192,168,10,2);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server;

void setup() {
  // put your setup code here, to run once:
WiFi.mode(WIFI_AP_STA);


WiFi.softAPConfig(ip,gateway,subnet);
WiFi.softAP(MySsid,pass);

server.on("/home.html",homepage);
server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
server.handleClient();
}

void homepage(){
  //char page[] PROGMEM = R"=====(
  char page[]= R"=====(

<!DOCTYPE html>
<html>
<head> </head>
<body>
<form>
<div>
  <label for="ssid"> SSID</label>
  <input value="" id="ssid" placeholder="SSID" />
</div>

<div>
  <label for="password">PASSWORD</label>
  <input value="" type="password" id="pass" placeholder="PASSWORD"/>
</div>

<div>
<button> SAVE</button>
</div>

</form>
</body>
</html>
  )=====";
  server.send(200,"text/plain",page);
  }
