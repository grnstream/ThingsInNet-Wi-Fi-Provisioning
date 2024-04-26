#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <EEPROM.h>

//Variables
int i = 0;
int statusCode;
const char* ssid = "Your-default-SSID";
const char* passphrase = "Your-default-SSID-PASSWORD";
String st;
String content;
String esid;
String epass = "";

#define LED_PIN 2

//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);

//Establishing Local server at port 80
WebServer server(80);

void setup()
{

  // Initialize GPIO2 as an output
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200); //Initialising if(DEBUG)Serial Monitor
  Serial.println();
  Serial.println("Disconnecting current wifi connection");
  WiFi.disconnect();
  EEPROM.begin(512); //Initialasing EEPROM
  delay(10);
  pinMode(34, INPUT);
  Serial.println();
  Serial.println();
  Serial.println("Startup");

  //---------------------------------------- Read eeprom for ssid and pass
  Serial.println("Reading EEPROM ssid");


  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");

  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);

  WiFi.begin(esid.c_str(), epass.c_str());
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED))
  {
    Serial.print("Connected to ");
    Serial.print(esid);
    Serial.println(" Successfully");
    delay(100);
    
    //****************************************
    // Call the blinkLED function
    blinkLED();
    Serial.println("YOUR CODE WILL RUN HERE");
 
    //****************************************
  }
  else
  {
    Serial.println(" Wi-Fi id not connected.");  
  }

  if (testWifi() && (digitalRead(34) != 1))
  {
    Serial.println(" connection status positive");
    return;
  }
  else
  {
    Serial.println("Connection Status Negative / D15 HIGH");
    Serial.println("Turning the HotSpot On");
    launchWeb();
    setupAP();// Setup HotSpot
  }

  Serial.println();
  Serial.println("Waiting.");

  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(100);
    server.handleClient();
  }
  delay(1000);
}

//----------------------------------------------- Fuctions used for WiFi credentials saving and connecting to it which you do not need to change
bool testWifi(void)
{
  int c = 0;
  //Serial.println("Waiting for Wifi to connect");
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}

void launchWeb()
{
  Serial.println("");
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
      //Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
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
    //st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  WiFi.softAP("Master_AP001", "");
  Serial.println("Initializing_softap_for_wifi credentials_modification");
  launchWeb();
  Serial.println("over");
}

void blinkLED() {
  // Turn the LED on (HIGH)
  digitalWrite(LED_PIN, HIGH);
  delay(1000); // Wait for 1 second

  // Turn the LED off (LOW)
  digitalWrite(LED_PIN, LOW);
  delay(1000); // Wait for 1 second
}

 void createWebServer()
{
  server.on("/", []() {
    IPAddress ip = WiFi.softAPIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    String content = "<!DOCTYPE HTML>\r\n<html>";
    content += "<head><style>";
    content += "body { text-align: center; background-color: #f2f2f2; }";
    content += ".container { display: flex; flex-direction: column; justify-content: center; align-items: center; height: 100vh; }";
    content += ".form-container { background-color: #ffffff; padding: 20px; border-radius: 10px; box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1); }";
    content += ".scan-btn { background-color: #2196F3; color: #ffffff; border: none; padding: 10px 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin-top: 10px; cursor: pointer; border-radius: 5px; }";
    content += ".input-field { width: 300px; padding: 10px; margin: 10px 0; }";
    content += ".submit-btn { background-color: #2196F3; color: #ffffff; border: none; padding: 10px 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin-top: 10px; cursor: pointer; border-radius: 5px; }";
    content += ".wifi-list { list-style-type: none; padding: 0; text-align: center; }";
    content += ".wifi-list li { margin: 5px; }";
    content += "</style></head>";
    content += "<body>";
    content += "<div class=\"container\">";
    content += "<div class=\"form-container\">";
    content += "<h1 style=\"color: #2196F3;\">ThingsInNet</h2>";
    content += "<h1 style=\"color: #2196F3;\">Wi-Fi Provisioning</h1>";
    content += "<form action=\"/scan\" method=\"POST\">";
    content += "<input class=\"scan-btn\" type=\"submit\" value=\"Scan\">";
    content += "</form>";
    content += "<p>IP Address: " + ipStr + "</p>";
    content += "<form method='get' action='setting'>";
    content += "<label for='ssid'>SSID:</label><br>";
    content += "<input class=\"input-field\" type='text' id='ssid' name='ssid'><br>";
    content += "<label for='pass'>Password:</label><br>";
    content += "<input class=\"input-field\" type='password' id='pass' name='pass'><br>";
    content += "<input class=\"submit-btn\" type='submit' value='Submit'>";
    content += "</form>";
    content += "</div>";
    content += "</div>";
    content += "</body>";
    content += "</html>";
    server.send(200, "text/html", content);
  });

  server.on("/scan", []() {
    IPAddress ip = WiFi.softAPIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    String content = "<!DOCTYPE HTML>\r\n<html>";
    content += "<head><style>";
    content += "body { text-align: center; background-color: #f2f2f2; }";
    content += ".container { display: flex; flex-direction: column; justify-content: center; align-items: center; height: 100vh; }";
    content += ".form-container { background-color: #ffffff; padding: 20px; border-radius: 10px; box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1); }";
    content += ".input-field { width: 300px; padding: 10px; margin: 10px 0; }";
    content += ".wifi-list { list-style-type: none; padding: 0; text-align: center; }";
    content += ".wifi-list li { margin: 5px; }";
    content += "</style></head>";
    content += "<body>";
    content += "<div class=\"container\">";
    content += "<div class=\"form-container\">";
    content += "<h1 style=\"color: #2196F3;\">Wi-Fi Networks Available</h1>";
    content += "<ul class=\"wifi-list\">";
  
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
      content += "<li>" + WiFi.SSID(i) + "</li>";
    }

    content += "</ul>";
    content += "</div>";
    content += "</div>";
    content += "</body>";
    content += "</html>";

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
        ESP.restart();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);

    });
}
