#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <Armor.h>

Armor *armor = new Armor();
Colors *colors = new Colors();
uint16_t initialBrightness = 6;
int buttonPin = 2;
int comPinIn = 9;
int comPinOut = 3;

// initialize animators
Radiate<CHEST_TOP_SEGMENTS> *radiatechestTop = new Radiate<CHEST_TOP_SEGMENTS>(100, 200, 0, armor->chestTop, 0, PaletteKey::base, colors);
Radiate<CHEST_RIGHT_SEGMENTS> *radiateChestRight = new Radiate<CHEST_RIGHT_SEGMENTS>(100, 0, 100, armor->chestRight, 0, PaletteKey::base, colors);
GlobalBreathe *globalBreathe = new GlobalBreathe(armor, 150, initialBrightness, 30);

void setup()
{
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(comPinIn, INPUT);
  pinMode(comPinOut, OUTPUT);
  digitalWrite(comPinOut, LOW);
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

boolean finished = false;
uint64_t runtime = 0;
boolean previousButtonState = LOW;
boolean currentButtonState = LOW;
boolean previousComInState = LOW;
boolean currentComInState = LOW;
int phase = 0;
void loop()
{
  currentButtonState = digitalRead(buttonPin);
  if (currentButtonState == HIGH && previousButtonState == LOW)
  {
    previousButtonState = HIGH;
    if (phase < 4)
    {
      phase++;
    }
    else
    {
      firstPhaseOneRun = true;
      secondPhaseFirstRun = true;
      thirdPhaseFirstRun = true;
      phase = 1;
    }
  }
  if (currentButtonState == LOW && previousButtonState == HIGH)
  {
    previousButtonState = LOW;
  }

  if (phase == 1)
  {
    phaseOne(runtime);
  }
  if (phase == 2)
  {
    phaseTwo(runtime);
  }
  if (phase == 3)
  {
    phaseThree(runtime);
  }
  if (phase == 4)
  {
    armor->setAllOneColor(colors->black);
  }
  delay(3);
  armor->show();
  runtime++;
}