#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "Armor.h"

template <int segmentCount>
AddressableArea<segmentCount>::AddressableArea(uint16_t ledCount, uint8_t pin, int16_t segmentStarts[segmentCount], int16_t segmentEnds[segmentCount])
{
  numLEDs = ledCount;
  pin = pin;
  numSegments = segmentCount;
  memcpy(segmentStartIndicies, segmentStarts, sizeof(segmentStarts[0]) * segmentCount);
  memcpy(segmentEndIndicies, segmentEnds, sizeof(segmentEnds[0]) * segmentCount);
  areaInstance = new Adafruit_NeoPixel(numLEDs, pin, NEO_GRB + NEO_KHZ800);
};

Armor::Armor()
{
  int16_t chestLeftStarts[3] = {0, -14, 15};
  int16_t chestLeftEnds[3] = {-7, 8, -20};
  chestLeft = new AddressableArea<3>(21, 6, chestLeftStarts, chestLeftEnds);

  int16_t chestRightStarts[2] = {0, 3};
  int16_t chestRightEnds[2] = {2, 6};
  chestRight = new AddressableArea<2>(7, 5, chestRightStarts, chestRightEnds);
}

void Armor::begin()
{
  chestLeft->areaInstance->begin();
  chestRight->areaInstance->begin();
}

void Armor::show()
{
  chestLeft->areaInstance->show();
  chestRight->areaInstance->show();
}

void Armor::setBrightness(uint8_t brightness)
{
  chestLeft->areaInstance->setBrightness(brightness);
  chestRight->areaInstance->setBrightness(brightness);
}
