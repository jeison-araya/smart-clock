// COMMON
#include <FS.h>
#include <ArduinoJson.h>

#ifndef PSTR
#define PSTR
#endif

// MATRIX LIBRARIES
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// ESP AP
#include <ESP8266WiFi.h>

// ESP WEB SERVER
#include <ESP8266WebServer.h>

// PIN CONFIGURATION
#define RESET_BUTTON D0
#define STATE_LED D1
#define PIN D2

// ACCESS POINT CONFIGURATION
const char *ssid = "SMART-WATCH";
const char *password = "12345678";

IPAddress local_IP(192, 168, 4, 22);
IPAddress gateway(192, 168, 4, 9);
IPAddress subnet(255, 255, 255, 0);

// CONFIGURATION DATA
char config_ssid[32];
char config_pass[32];
bool configLoaded = false;
bool restartRequest = false;

// WEB SERVER
ESP8266WebServer server(80);

// WIFI CLIENT
WiFiClient espClient;

// MATRIX CONFIGURATION
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
                                               NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                                               NEO_GRB + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(111, 3, 252), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};

void setup() {
  serialSetup();
  pinSetup();
  loadConfig();

  if (configLoaded) {
    wifiClientSetup();
  } else {
    accessPointSetup();
    buildEndpoints();
  }

  matixSetup();
}

void serialSetup() {
  Serial.begin(115200);
  delay(10);
}

void pinSetup() {
  pinMode(RESET_BUTTON, INPUT);
  pinMode(STATE_LED, OUTPUT);
}

void resetConfig() {
  Serial.println(F("\n\nWait...I am formatting  the FLASH !!!"));
  SPIFFS.format();
  Serial.println(F("Done!"));
  WiFi.disconnect(true);
  restart();
}

void restart() {
  Serial.println("Restarting...");
  ESP.restart();
}

void loadConfig() {
  if (SPIFFS.begin()) {
    Serial.println("Mounted file system");
    if (SPIFFS.exists("/config.json")) {
      Serial.println("Reading config file...");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        size_t size = configFile.size();
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(buf.get());

        Serial.print("Config: ");
        json.printTo(Serial);

        if (json.success()) {
          strcpy(config_ssid, json["ssid"]);
          strcpy(config_pass, json["pass"]);

          configLoaded = true;
        } else {
          Serial.println("Failed to load json config");
        }
      }
    }
  } else {
    Serial.println("Failed to mount FS");
  }
}

void accessPointSetup() {
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

void wifiClientSetup() {
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(config_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(config_ssid, config_pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void matixSetup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
}


void handleRootResponse() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void handleConfigResponse() {

  if (server.hasArg("plain") == false) {
    server.send(400, "text/plain", "Body not received");
    return;
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.parseObject(server.arg("plain"));

  if (json.success()) {
    json.printTo(Serial);
    strcpy(config_ssid, json["ssid"]);
    strcpy(config_pass, json["pass"]);

    Serial.println("saving config");

    DynamicJsonBuffer configBuffer;
    JsonObject &config = configBuffer.createObject();

    config["ssid"] = config_ssid;
    config["pass"] = config_pass;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println(F("failed to open config file for writing"));
    }

    config.printTo(Serial);
    config.printTo(configFile);
    configFile.close();

    server.send(200, "text/plain", "Configuration saved");
    delay(1000);

    restartRequest = true;
  } else {
    Serial.println("failed to load json config");
    server.send(400, "text/plain", "Invalid JSON format");
  }
}

void buildEndpoints() {
  server.on("/", handleRootResponse);
  server.on("/conf", handleConfigResponse);
  server.begin();
}

void loop() {
  checkResetButtonState();
  checkRestartRequest();
  updateStateLed();

  if (!configLoaded) {
    server.handleClient();
  }
}

void checkResetButtonState() {
  if (digitalRead(RESET_BUTTON) == 1) {
    Serial.println("Reset button pressed");
    resetConfig();
    delay(400);
  }
}

void checkRestartRequest() {
  if (restartRequest) {
    restart();
  }
}

void updateStateLed() {
  if (configLoaded) {
    digitalWrite(STATE_LED, HIGH);
  } else {
    digitalWrite(STATE_LED, HIGH);
    delay(300);
    digitalWrite(STATE_LED, LOW);
    delay(300);
  }
}
