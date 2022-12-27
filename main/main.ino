#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN D2

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_GRBW    Pixels are wired for GRBW bitstream (RGB+W NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.  The shield uses
// 800 KHz (v2) pixels that expect GRB color data.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(111, 3, 252), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
}

int x    = matrix.width();
int pass = 0;

void loop() {
  // byte logo[8][8][3] = { 
  //  {{255,255,255}, {50,50,50}, {255,255,255}, {255,255,255}, {255,255,255}, {0,255,0}, {0,0,255}, {255,0,0}},
  //  {{255,255,255}, {50,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {0,255,0}, {0,0,255}, {255,0,0}},
  //  {{255,255,255}, {50,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {0,255,0}, {0,0,255}, {255,0,0}},
  //  {{255,255,255}, {50,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {0,255,0}, {0,0,255}, {255,0,0}},
  //  {{255,255,255}, {255,255,50}, {255,255,255}, {255,255,255}, {255,255,255}, {0,255,0}, {0,0,255}, {255,0,0}},
  //  {{255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {0,255,0}, {0,0,255}, {255,0,0}},
  //  {{255,255,255}, {255,50,50}, {255,255,255}, {255,255,255}, {255,255,255}, {0,255,0}, {0,0,255}, {255,0,0}},
  //  {{255,255,255}, {255,50,255}, {255,255,255}, {255,255,255}, {255,255,255}, {0,255,0}, {0,0,255}, {255,0,0}},
  // };

  // drawMatrix(logo);
  // matrix.show();

  // delay(2000);
  // matrix.fillScreen(0);
  // matrix.show();

  // delay(1000);
  // for(int col=0; col < 8; col++) {
  //   for(int row=0; row < 32; row++) {
  //     matrix.fillScreen(0);
  //     matrix.drawPixel(row, col, colors[0]);
  //     matrix.show();
  //     delay(500);
  //   }
  // }

  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("12:10"));
  if(--x < -36) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(100);
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
