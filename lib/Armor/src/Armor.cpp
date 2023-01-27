#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "Armor.h"

template <uint8_t segmentCount>
AddressableArea<segmentCount>::AddressableArea(uint16_t ledCount, uint8_t pin, int16_t segmentStarts[segmentCount], int16_t segmentEnds[segmentCount], uint8_t longestSegmentLength)
{
  numLEDs = ledCount;
  pin = pin;
  numSegments = segmentCount;
  memcpy(segmentStartIndicies, segmentStarts, sizeof(segmentStarts[0]) * segmentCount);
  memcpy(segmentEndIndicies, segmentEnds, sizeof(segmentEnds[0]) * segmentCount);
  areaInstance = new Adafruit_NeoPixel(numLEDs, pin, NEO_GRB + NEO_KHZ800);
  longestSegment = longestSegmentLength;
};

void Armor::setFullSection(Adafruit_NeoPixel *stripInstance, uint16_t numLEDs, uint32_t color)
{
  for (uint16_t i = 0; i < numLEDs; i++)
  {
    stripInstance->setPixelColor(i, color);
  }
};

Armor::Armor()
{
  int16_t chestTopStarts[6] = {0, -15, 16, 24, -39, 40};
  int16_t chestTopEnds[6] = {-7, 8, -23, -31, 32, -47};
  chestTop = new AddressableArea<CHEST_TOP_SEGMENTS>(48, 6, chestTopStarts, chestTopEnds, 8);

  int16_t chestRightStarts[2] = {0, 3};
  int16_t chestRightEnds[2] = {-2, -7};
  chestRight = new AddressableArea<2>(8, 5, chestRightStarts, chestRightEnds, 5);
}

void Armor::begin()
{
  chestTop->areaInstance->begin();
  chestRight->areaInstance->begin();
}

void Armor::show()
{
  chestTop->areaInstance->show();
  chestRight->areaInstance->show();
}

void Armor::setBrightness(uint8_t brightness)
{
  chestTop->areaInstance->setBrightness(brightness);
  chestRight->areaInstance->setBrightness(brightness);
}
