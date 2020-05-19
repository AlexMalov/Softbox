uint32_t effTimer;

void effectsTick() {
    if (ONflag && millis() - effTimer >= ( currentMode > 0 ? speed : 50) ) {
      effTimer = millis();
      switch (currentMode) {
        case 0: colorRoutine(); break;
        case 1: colorsRoutine(); break;
        case 2: rainbowHorizontal(); break;
        case 3: sparklesRoutine(); break;
        case 4: rainbowStripeNoise(); break;
        case 5: cloudNoise(); break;
        case 6: lavaNoise(); break;
      }
      
      CHSV cl; byte ind, xc;
      switch (numHold) {    // индикатор уровня яркости/скорости/масштаба
        case 1:
          ind = brightness >> 4;
          cl = CHSV(255, 0, 255);
          xc = 0;
          break;
        case 2:
          ind = speed >> 4;
          cl = CHSV(255, 0, 255);
          xc = WIDTH >> 1;
          break;
        case 3:
          ind = scale >> 4;
          cl = CHSV(255, 0, 255);
          xc = WIDTH - 1;
          break;
      }
      if (numHold != 0){
        for (byte y = 0; y < HEIGHT ; y++)
          if (ind > y) drawPixelXY(xc, y, cl);
            else drawPixelXY(xc, y, 0);
      }
      
      if (currentMode > 1) FastLED.show();
    }
}

void changePower() {    // плавное включение/выключение
  if (ONflag) {
    effectsTick();
    for (int i = 0; i < brightness; i++) {
      FastLED.setBrightness(i);
      delay(1);
      FastLED.show();
    }
  } else {
    for (int i = brightness; i > 8; i--) {
      FastLED.setBrightness(i);
      delay(1);
      FastLED.show();
    }
    FastLED.clear();
    FastLED.show();
  }
}
