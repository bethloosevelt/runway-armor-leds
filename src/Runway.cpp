#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <Armor.h>

Armor *armor = new Armor();
Colors *colors = new Colors();
uint16_t initialBrightness = 6;

// initialize animators
Radiate<CHEST_TOP_SEGMENTS> *radiatechestTop = new Radiate<CHEST_TOP_SEGMENTS>(100, 200, 0, armor->chestTop, 0, PaletteKey::base, colors);
Radiate<CHEST_RIGHT_SEGMENTS> *radiateChestRight = new Radiate<CHEST_RIGHT_SEGMENTS>(100, 0, 100, armor->chestRight, 0, PaletteKey::base, colors);
GlobalBreathe *globalBreathe = new GlobalBreathe(armor, 150, initialBrightness, 30);

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

boolean firstPhaseOneRun = true;
void phaseOne(uint64_t runtime)
{
  if (firstPhaseOneRun)
  {
    firstPhaseOneRun = false;
    radiatechestTop->changeTimings(100, 200, 0, runtime);
    radiateChestRight->changeTimings(100, 0, 100, runtime);
  }
  radiateChestRight->advance(runtime);
  radiatechestTop->advance(runtime);
}

boolean secondPhaseFirstRun = true;
void phaseTwo(uint64_t runtime)
{
  if (secondPhaseFirstRun)
  {
    secondPhaseFirstRun = false;
    radiatechestTop->changeTimings(50, 100, 0, runtime);
    radiateChestRight->changeTimings(50, 0, 50, runtime);
  }
  radiateChestRight->advance(runtime);
  radiatechestTop->advance(runtime);
}

boolean thirdPhaseFirstRun = true;
void phaseThree(uint64_t runtime)
{
  if (thirdPhaseFirstRun)
  {
    thirdPhaseFirstRun = false;
    radiatechestTop->changeTimings(25, 50, 0, runtime);
    radiateChestRight->changeTimings(25, 0, 25, runtime);
  }
  radiateChestRight->advance(runtime);
  radiatechestTop->advance(runtime);
}

uint64_t phaseOneDuration = 1000;
uint64_t phaseTwoDuration = 1000;
boolean finished = false;
uint64_t runtime = 0;
void loop()
{
  if (runtime < phaseOneDuration)
  {
    phaseOne(runtime);
  }
  if (runtime < phaseTwoDuration + phaseOneDuration && runtime > phaseOneDuration)
  {
    phaseTwo(runtime);
  }
  if (runtime > phaseTwoDuration + phaseOneDuration)
  {
    phaseThree(runtime);
  }
  delay(3);
  armor->show();
  runtime++;
}