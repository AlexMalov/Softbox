// --------------------------------- конфетти ------------------------------------
void sparklesRoutine() {
  for (byte i = 0; i < scale; i++) {
    byte x = random(0, WIDTH);
    byte y = random(0, HEIGHT);
    if (getPixColorXY(x, y) == 0)
      leds[getPixelNumber(x, y)] = CHSV(random(0, 255), 255, 255);
  }
  fader(70);
}

// функция плавного угасания цвета для всех пикселей
void fader(byte step) {
  for (byte i = 0; i < WIDTH; i++) 
    for (byte j = 0; j < HEIGHT; j++) fadePixel(i, j, step);
}

void fadePixel(byte i, byte j, byte step) {     // новый фейдер
  int pixelNum = getPixelNumber(i, j);
  if (getPixColor(pixelNum) == 0) return;
  if (leds[pixelNum].r >= 30 || leds[pixelNum].g >= 30 || leds[pixelNum].b >= 30) 
    leds[pixelNum].fadeToBlackBy(step);
    else leds[pixelNum] = 0;
}

// ---------------------------------------- радуга ------------------------------------------
byte hue;
void rainbowHorizontal() {
  hue += 2;
  for (byte i = 0; i < WIDTH; i++) {
    CHSV thisColor = CHSV((byte)(hue + i * (scale>>3)), 255, 255);
    for (byte j = 0; j < HEIGHT; j++)
      drawPixelXY(i, j, thisColor); 
  }
}

// ---------------------------------------- ЦВЕТА ------------------------------------------
void colorsRoutine() {
  hue += scale>>3;
  FastLED.showColor(CHSV(hue, 255, 255));
  //for (int i = 0; i < NUM_LEDS; i++)
  //  leds[i] = CHSV(hue, 255, 255);
}

// --------------------------------- ЦВЕТ ------------------------------------
void colorRoutine() {
  FastLED.showColor(CHSV(scale, speed, 255));
}

void colorRGBRoutine() {
  FastLED.showColor(CRGB(scale, speed, brightness));
}
