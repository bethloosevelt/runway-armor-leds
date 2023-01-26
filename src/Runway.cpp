#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <Armor.h>

Armor *armor = new Armor();
uint32_t yellow = armor->chestLeft->areaInstance->Color(250, 237, 39);
uint32_t orange = armor->chestLeft->areaInstance->Color(255, 95, 31);
uint32_t blue = armor->chestLeft->areaInstance->Color(0, 50, 255);

uint32_t basePalette[3] = {yellow, orange, blue};

int startTime = millis();
Cycle<CHEST_RIGHT_SEGMENTS, 3> *cycleChestRight = new Cycle<CHEST_RIGHT_SEGMENTS, 3>(armor->chestRight, 200, basePalette);
Radiate<CHEST_LEFT_SEGMENTS, 3> *radiateChestLeft = new Radiate<CHEST_LEFT_SEGMENTS, 3>(basePalette, 400, true, true, armor->chestLeft);

void setup()
{
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  Serial.begin(9600);
  armor->begin();
  armor->setBrightness(10);
  armor->setAllOneColor(blue);
  armor->show();
}

boolean rising = true;
uint8_t brightness = 1;
int lastGlobalBreatheFrameTime = millis();
void globalBreathe(int throttle)
{
  int currentTime = millis();
  if (currentTime - lastGlobalBreatheFrameTime >= throttle)
  {
    lastGlobalBreatheFrameTime = currentTime;
    armor->setBrightness(brightness);
    if (rising)
    {
      brightness++;
    }
    else
    {
      brightness--;
    }
    if (brightness == 10)
    {
      rising = false;
    }
    if (brightness == 3)
    {
      rising = true;
    }
  }
}

boolean isFirstPhaseOneCycle = true;
void phaseOne()
{
  if (isFirstPhaseOneCycle)
  {
    isFirstPhaseOneCycle = false;
    radiateChestLeft->duration = 400;
    radiateChestLeft->bounce = true;
    radiateChestLeft->outward = true;
    cycleChestRight->reset(400);
  }
  cycleChestRight->advance();
  radiateChestLeft->advance();
  globalBreathe(30);
}

boolean isFirstPhaseTwoCycle = true;
void phaseTwo()
{
  if (isFirstPhaseTwoCycle)
  {
    isFirstPhaseTwoCycle = false;
    radiateChestLeft->duration = 200;
    radiateChestLeft->bounce = false;
    radiateChestLeft->outward = false;
    cycleChestRight->reset(2000);
    armor->setBrightness(20);
  }
  cycleChestRight->advance();
  radiateChestLeft->advance();
  // globalBreathe(10);
}

int phaseOneLengh = 7000;
int phaseTwoLengh = 3000;
boolean finished = false;
void loop()
{
  int runtime = millis() - startTime;
  if (runtime < phaseOneLengh && !finished)
  {
    phaseOne();
  }
  else if (runtime < phaseOneLengh + phaseTwoLengh && !finished)
  {
    phaseTwo();
  }
  else if (runtime > phaseOneLengh + phaseTwoLengh)
  {
    finished = true;
    armor->setBrightness(0);
  }
  delay(10);
  armor->show();
}