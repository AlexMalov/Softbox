/*
  Скетч к проекту "Softbox с динамическими эффектами"
  Аппаратная часть построена на Arduino Nano
  Исходники на GitHub: https://github.com/AlexMalov/Softbox/
  Автор: МЕХАТРОН DIY, AlexMalov, 2020
  v 1.0
 
  впроекте использованы и переработаны материалы из поекта AlexGyver
  https://alexgyver.ru/GyverLamp/
*/

// ============= НАСТРОЙКИ =============

// ---------- МАТРИЦА ---------
#define CURRENT_LIMIT 20000   // лимит по току в миллиамперах, автоматически управляет яркостью
#define WIDTH 16              // ширина матрицы
#define HEIGHT 20             // высота матрицы
#define COLOR_ORDER GRB       // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB
#define MATRIX_TYPE 0         // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE 0    // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION 0     // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
// при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
// шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/

#define numHold_Time 1500     // время отображения индикатора уровня яркости/скорости/масштаба в мс

// ============= ДЛЯ РАЗРАБОТЧИКОВ =============
#define LED_PIN 2             // пин ленты
#define BTN_PIN 3             // кнопка переключения режимов
#define HUI_PIN A0            // потенциометр Тон / scale
#define SAT_PIN A1            // потенциометр Насыщеность      
#define BRI_PIN A2            // потенциометр Яркость

#define MODE_AMOUNT 7
#define NUM_LEDS WIDTH * HEIGHT

// ---------------- БИБЛИОТЕКИ -----------------
#include <EEPROM.h>
#include <FastLED.h>
#include <GyverButton.h>

// ------------------- ТИПЫ --------------------
CRGB leds[NUM_LEDS];
GButton btnMode(BTN_PIN, HIGH_PULL, NORM_OPEN);  //механическая кнопка

// ----------------- ПЕРЕМЕННЫЕ ------------------
byte brightness = 100; // brightness
byte speed;      // saturation or speed
byte scale;      // Hue or scale

byte currentMode = 0;
boolean ONflag = true;
byte numHold;
unsigned long numHold_Timer = 0;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)/*.setCorrection( TypicalLEDStrip )*/;
  FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  FastLED.clear();
  Serial.begin(115200);
  if (EEPROM.read(0) == 2) currentMode = EEPROM.read(1);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), btnInt, CHANGE);
  effectsTick();
  changePower();
}

void btnInt(){
  btnMode.tick();
}

void debugPrint(){
    Serial.print(F(" mode:")); Serial.print(currentMode);  
    Serial.print(F(" brightness:")); Serial.print(brightness);
    Serial.print(F(" speed:")); Serial.print(speed);
    Serial.print(F(" scale:")); Serial.println(scale); 
}

void buttonCheck() {
  btnMode.tick();
  if (btnMode.isClick()) {
    if (!ONflag) {
      ONflag = true;
      Serial.println(F("power on!"));
      changePower();
      return;
    }
    currentMode++;
    if (currentMode >= MODE_AMOUNT) currentMode = 0;
    FastLED.setBrightness(brightness);
    FastLED.clear();
    EEPROM.update(0, 2);
    EEPROM.update(1, currentMode);  // запоминаем текущий эфект
    debugPrint();
  }
  if (btnMode.isHolded() && ONflag) {
    ONflag = false;
    Serial.println(F("power off!"));
    changePower();
  }
}

void loop() {
  byte tmp = (analogRead(BRI_PIN) >> 4) << 2;
  if (brightness != tmp) {
    brightness = tmp;
    numHold = 1;    // включаем индикатор уровня яркости
    numHold_Timer = millis() + numHold_Time;
    if (ONflag) FastLED.setBrightness(brightness);
    debugPrint();
  }
  tmp = (analogRead(SAT_PIN) >> 4) << 2;
  if (speed != tmp) {
    speed = tmp;
    numHold = 2;    // включаем индикатор уровня насыщенности
    numHold_Timer = millis() + numHold_Time;
    debugPrint();
  }
  tmp = (analogRead(HUI_PIN) >> 4) << 2;
  if (scale != tmp) {
    scale = tmp;
    numHold = 3;    // включаем индикатор уровня тона
    numHold_Timer = millis() + numHold_Time;
    debugPrint();
  }
  if ((numHold != 0) && (millis() > numHold_Timer)) numHold = 0;
  buttonCheck();
  effectsTick();
}
