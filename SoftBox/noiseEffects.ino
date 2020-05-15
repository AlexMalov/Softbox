// The 16 bit version of our coordinates
static uint16_t x;
static uint16_t y;
static uint16_t z;


// This is the array that we keep our computed noise values in (это массив, в котором храним вычисленные значения шума)
#define MAX_DIMENSION (max(WIDTH, HEIGHT))
#if (WIDTH > HEIGHT)
  uint8_t noise[WIDTH][WIDTH];
#else
  uint8_t noise[HEIGHT][HEIGHT];
#endif

CRGBPalette16 currentPalette( PartyColors_p );
uint8_t colorLoop;
uint8_t ihue = 0;

void rainbowStripeNoise() {
  currentPalette = RainbowStripeColors_p;
  colorLoop = 1;
  fillNoiseLED();
}

void oceanNoise() {
  currentPalette = OceanColors_p;
  colorLoop = 0;
  fillNoiseLED();
}

void plasmaNoise() {
  currentPalette = PartyColors_p;
  colorLoop = 1;
  fillNoiseLED();
}
void cloudNoise() {
  currentPalette = CloudColors_p;
  colorLoop = 0;
  fillNoiseLED();
}

void lavaNoise() {
  currentPalette = LavaColors_p;
  colorLoop = 0;
  fillNoiseLED();
}

// ******************* СЛУЖЕБНЫЕ *******************
void fillNoiseLED() {
  uint8_t dataSmoothing = 0;
  if ( speed < 50) dataSmoothing = 200 - (speed << 2);
  for (int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for (int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;
      uint8_t data = inoise8(x + ioffset, y + joffset, z);
      data = qsub8(data, 16);
      data = qadd8(data, scale8(data, 39));
      if ( dataSmoothing ) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }
      noise[i][j] = data;
    }
  }
  z += speed;
  // apply slow drift to X and Y, just for visual variation.
  x += speed >> 3;
  y -= speed >> 4;

  for (byte i = 0; i < WIDTH; i++) {
    for (byte j = 0; j < HEIGHT; j++) {
      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];
      // if this palette is a 'loop', add a slowly-changing base value
      if ( colorLoop ) index += ihue;
      // brighten up, as the color palette itself often contains the
      // light/dark dynamic range desired
      if ( bri > 127 ) {
        bri = 255;
      } else bri = dim8_raw( bri);
      CRGB color = ColorFromPalette( currentPalette, index, bri);
      drawPixelXY(i, j, color);
    }
  }
  ihue += 1;
}
