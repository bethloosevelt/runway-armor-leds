#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <Armor.h>

Armor *armor = new Armor();
Colors *colors = new Colors();
uint16_t initialBrightness = 3;

// initialize animators
Cycle<CHEST_RIGHT_SEGMENTS> *cycleChestRight = new Cycle<CHEST_RIGHT_SEGMENTS>(armor->chestRight, 200, PaletteKey::base, colors);
Radiate<CHEST_LEFT_SEGMENTS> *radiateChestLeft = new Radiate<CHEST_LEFT_SEGMENTS>(400, true, false, false, armor->chestLeft, PaletteKey::base, 0, colors, 400);
Radiate<CHEST_RIGHT_SEGMENTS> *radiateChestRight = new Radiate<CHEST_RIGHT_SEGMENTS>(400, true, false, false, armor->chestRight, PaletteKey::base, 400, colors, 400);
GlobalBreathe *globalBreathe = new GlobalBreathe(armor, 400, initialBrightness, 4);

void setup()
{
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  armor->begin();
  armor->setAllOneColor(colors->black);
  armor->setBrightness(initialBrightness);
  armor->show();
}

boolean isFirstPhaseOneCycle = true;
void phaseOne(int runtime)
{
  if (isFirstPhaseOneCycle)
  {
    isFirstPhaseOneCycle = false;
    radiateChestLeft->reset(300, true, false, true, 0, 300, runtime);
    radiateChestRight->reset(300, true, false, true, 0, 300, runtime);
  }
  radiateChestRight->advance(runtime);
  radiateChestLeft->advance(runtime);
  // globalBreathe->advance();
}

boolean isFirstPhaseTwoCycle = true;
void phaseTwo(int runtime)
{
  if (isFirstPhaseTwoCycle)
  {
    isFirstPhaseTwoCycle = false;
    radiateChestLeft->reset(30, false, false, true, 0, 0, runtime);
    radiateChestRight->reset(30, false, false, true, 0, 0, runtime);
    return;
    // globalBreathe->reset(100, 3, 4);
  }
  radiateChestRight->advance(runtime);
  radiateChestLeft->advance(runtime);
  // globalBreathe->advance();
}

int phaseOneLengh = 20000;
int phaseTwoLengh = 3000;
boolean finished = false;
int runtime = 0;
void loop()
{
  if (runtime < phaseOneLengh && !finished)
  {
    phaseOne(runtime);
  }
  else if (runtime < phaseOneLengh + phaseTwoLengh && !finished)
  {
    phaseTwo(runtime);
  }
  else if (runtime > phaseOneLengh + phaseTwoLengh)
  {
    finished = true;
    armor->setBrightness(0);
  }
  delay(10);
  armor->show();
  runtime++;
}