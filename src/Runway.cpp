#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <Armor.h>

Armor *armor = new Armor();
uint32_t yellow = armor->chestLeft->areaInstance->Color(250, 237, 39);
uint32_t orange = armor->chestLeft->areaInstance->Color(255, 95, 31);
uint32_t blue = armor->chestLeft->areaInstance->Color(0, 50, 255);
uint32_t black = armor->chestLeft->areaInstance->Color(0, 0, 0);

uint32_t basePalette[1] = {orange};
uint16_t initialBrightness = 3;

int startTime = millis();

// initialize animators
Cycle<CHEST_RIGHT_SEGMENTS, 3> *cycleChestRight = new Cycle<CHEST_RIGHT_SEGMENTS, 3>(armor->chestRight, 200, basePalette);
Radiate<CHEST_LEFT_SEGMENTS, 1> *radiateChestLeft = new Radiate<CHEST_LEFT_SEGMENTS, 1>(basePalette, 400, true, false, false, armor->chestLeft);
Radiate<CHEST_RIGHT_SEGMENTS, 1> *radiateChestRight = new Radiate<CHEST_RIGHT_SEGMENTS, 1>(basePalette, 400, true, false, false, armor->chestRight);
GlobalBreathe *globalBreathe = new GlobalBreathe(armor, 400, initialBrightness, 30);

void setup()
{
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  armor->begin();
  armor->setAllOneColor(black);
  armor->setBrightness(initialBrightness);
  armor->show();
}

boolean isFirstPhaseOneCycle = true;
void phaseOne()
{
  if (isFirstPhaseOneCycle)
  {
    isFirstPhaseOneCycle = false;
    radiateChestLeft->reset(500, true, false, false);
    radiateChestLeft->paused = false;
    radiateChestRight->reset(500, true, false, false);
    radiateChestRight->paused = false;
  }
  radiateChestRight->advance();
  radiateChestLeft->advance();
  globalBreathe->advance();
}

boolean isFirstPhaseTwoCycle = true;
void phaseTwo()
{
  if (isFirstPhaseTwoCycle)
  {
    isFirstPhaseTwoCycle = false;
    radiateChestLeft->reset(1000, false, false, false);
    radiateChestLeft->paused = false;
    radiateChestRight->reset(1000, false, false, false);
    radiateChestRight->paused = false;
    globalBreathe->reset(100, 5, 100);
  }
  radiateChestRight->advance();
  radiateChestLeft->advance();
  globalBreathe->advance();
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