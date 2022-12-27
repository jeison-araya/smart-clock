// MATRIX LIBRARIES 
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// ESP AP
#include <ESP8266WiFi.h>

// ESP WEB SERVER
#include <ESP8266WebServer.h>

#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN D2

// ACCESS POINT CONFIGURATION
const char *ssid = "SMART-WATCH";
const char *password = "12345678";

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

// WEB SERVER
ESP8266WebServer server(80);


// MATRIX CONFIGURATION
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(111, 3, 252), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

void setup() {
  Serial.begin(115200);
  Serial.println();

  accessPointSetup();
  buildEndpoints();
  matixSetup();
}

void accessPointSetup() {
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!"); 
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid,password) ? "Ready" : "Failed!");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

void matixSetup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
}


void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void buildEndpoints() {
  server.on("/", handleRoot);
  server.begin();  
}

void loop() {
  Serial.print("[Server Connected] ");
  Serial.println(WiFi.softAPIP());

  delay(500);

  server.handleClient();
}

void drawMatrix(byte matrix_values[8][8][3]) {
  matrix.fillScreen(0);

  for(int row = 0; row < 8; row++) {
    for(int column = 0; column < 8; column++) {
      matrix.drawPixel(row, column, matrix.Color(matrix_values[row][column][0], matrix_values[row][column][1], matrix_values[row][column][2]));
   }
  }
}

// void drawLogo() {
//   // This 8x8 array represents the LED matrix pixels. 
//   // A value of 1 means we’ll fade the pixel to white
//   int logo[8][8] = {  
//    {matrix[row][column][], 0, 0, 0, 0, 0, 0, 0},
//    {0, 1, 1, 0, 0, 1, 1, 0},
//    {0, 1, 1, 0, 0, 1, 1, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 1, 1, 0, 0, 1, 1, 0},
//    {0, 1, 1, 0, 0, 1, 1, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0}
//   };

//   for(int row = 0; row < 8; row++) {
//     for(int column = 0; column < 8; column++) {
//      if(logo[row][column] == 1) {
//        matrix.drawPixel(row, column, colors[0]);
//      }
//    }
//   }
// }
