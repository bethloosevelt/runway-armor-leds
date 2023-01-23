#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <Armor.h>

Armor armor = Armor();
uint32_t yellow = armor.chestLeft->areaInstance->Color(250, 237, 39);
uint32_t orange = armor.chestLeft->areaInstance->Color(255, 95, 31);
uint32_t blue = armor.chestLeft->areaInstance->Color(0, 50, 255);

int startTime = millis();

void setAll(Adafruit_NeoPixel *stripInstance, uint16_t numLEDs, uint32_t color)
{
  Serial.print(numLEDs);
  for (uint16_t i = 0; i < numLEDs; i++)
  {
    stripInstance->setPixelColor(i, color);
  }
}

void setup()
{
  // debugging
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  Serial.begin(9600);
  armor.begin();
  armor.setBrightness(10);
  setAll(armor.chestLeft->areaInstance, armor.chestLeft->numLEDs, blue);
  setAll(armor.chestRight->areaInstance, armor.chestRight->numLEDs, blue);
  armor.show();
}

uint8_t frame = 0;
int cycle = 1;
uint32_t colors[3] = {yellow, orange, blue};
int lastRadiateChestFrameTime = millis();
boolean outward = true;
void radiateChest(int throttle, boolean bounce)
{
  int currentTime = millis();
  if (currentTime - lastRadiateChestFrameTime >= throttle)
  {
    lastRadiateChestFrameTime = currentTime;
    for (int i = 0; i < armor.chestLeft->numSegments; i++)
    {
      int pixelIndex = abs((outward ? armor.chestLeft->segmentStartIndicies[i] : armor.chestLeft->segmentEndIndicies[i]) + frame);
      armor.chestLeft->areaInstance->setPixelColor(pixelIndex, colors[cycle % 3]);
    }
    if (frame == 7)
    {
      frame = 0;
      cycle++;
      if (bounce)
      {
        outward = !outward;
      }
    }
    else
    {
      frame++;
    }
  }
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
    armor.setBrightness(brightness);
    if (rising)
    {
      brightness++;
    }
    else
    {
      brightness--;
    }
    if (brightness == 25)
    {
      rising = false;
    }
    if (brightness == 3)
    {
      rising = true;
    }
  }
}

int cycleSectionCycle = 0;
uint32_t cycleColors[3] = {yellow, orange, blue};
int lastCycleSection = millis();
void cycleSection(Adafruit_NeoPixel *stripInstance, int throttle, uint16_t numLEDs)
{
  int currentTime = millis();
  if (currentTime - lastCycleSection >= throttle)
  {
    lastCycleSection = currentTime;
    setAll(stripInstance, numLEDs, cycleColors[cycleSectionCycle % 3]);
    cycleSectionCycle++;
  }
}
void phaseOne()
{
  cycleSection(armor.chestRight->areaInstance, 100, armor.chestRight->numLEDs);
  radiateChest(150, false);
  globalBreathe(30);
}

void phaseTwo()
{
  cycleSection(armor.chestRight->areaInstance, 400, armor.chestRight->numLEDs);
  radiateChest(21, true);
  globalBreathe(10);
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
    armor.setBrightness(0);
  }
  delay(10);
  armor.show();
}