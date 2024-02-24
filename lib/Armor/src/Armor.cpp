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
  // const uint8_t HEAD_SEGMENTS = 1;
  // int16_t headStarts[HEAD_SEGMENTS] = {0, -15, 16, 24, -39, 40};
  // int16_t headEnds[HEAD_SEGMENTS] = {-7, 8, -23, -31, 32, -47};
  // headOne = new AddressableArea<CHEST_TOP_SEGMENTS>(48, headOnePin, headStarts, headEnds, 8);
  // headTwo = new AddressableArea<CHEST_TOP_SEGMENTS>(48, headOnePin, headStarts, headEnds, 8);

#if controller == 1
  int16_t shoulderStarts[SHOULDER_SEGMENTS] = {0, -14, 15, -38, 39};
  int16_t shoulderEnds[SHOULDER_SEGMENTS] = {-5, 6, -29, 30, -44};
  shoulders = new AddressableArea<SHOULDER_SEGMENTS>(45, shoulderOnePin, shoulderStarts, shoulderEnds, 15);

  int16_t armStarts[ARM_SEGMENTS] = {0, -29, 30};
  int16_t armEnds[ARM_SEGMENTS] = {-11, 12, -42};
  arms = new AddressableArea<ARM_SEGMENTS>(43, armsPin, armStarts, armEnds, 18);

  int16_t thighStarts[THIGH_SEGMENTS] = {0, -28, 29, -55, 56, -75};
  int16_t thighEnds[THIGH_SEGMENTS] = {-13, 13, -42, 43, -67, 68};
  thighs = new AddressableArea<THIGH_SEGMENTS>(76, thighsPin, thighStarts, thighEnds, 15);

  int16_t legStarts[LEG_SEGMENTS] = {0, -26, 27, -54};
  int16_t legEnds[LEG_SEGMENTS] = {-16, 17, -36, 37};
  legs = new AddressableArea<LEG_SEGMENTS>(55, legsPin, legStarts, legEnds, 18);
#endif
#if controller == 2
  int16_t headStarts[HEAD_SEGMENTS] = {0, -34};
  int16_t headEnds[HEAD_SEGMENTS] = {-20, 21};
  head = new AddressableArea<HEAD_SEGMENTS>(35, headPin, headStarts, headEnds, 21);

  int16_t chestTopStarts[CHEST_TOP_SEGMENTS] = {-7, 8, -23, -31, 32, -47};
  int16_t chestTopEnds[CHEST_TOP_SEGMENTS] = {0, -15, 16, 24, -39, 40};
  chestTop = new AddressableArea<CHEST_TOP_SEGMENTS>(48, chestTopPin, chestTopStarts, chestTopEnds, 8);

  int16_t absStarts[ABS_SEGMENTS] = {-10, 11, -30, 31};
  int16_t absEnds[ABS_SEGMENTS] = {0, -20, 21, -41};
  abs = new AddressableArea<ABS_SEGMENTS>(42, absPin, absStarts, absEnds, 11);

  int16_t groinRayStarts[GROIN_RAYS_SEGMENTS] = {-1, 2, -20, 21, -24, 25, 28, -42};
  int16_t groinRayEnds[GROIN_RAYS_SEGMENTS] = {0, -9, 10, -22, 23, -27, -36, 37};
  groinRays = new AddressableArea<GROIN_RAYS_SEGMENTS>(43, groinRaysPin, groinRayStarts, groinRayEnds, 11);

  int16_t spiralEnds[SPIRAL_SEGMENTS] = {-11};
  int16_t spiralStarts[SPIRAL_SEGMENTS] = {0};
  spiral = new AddressableArea<SPIRAL_SEGMENTS>(12, spiralPin, spiralStarts, spiralEnds, 12);
#endif
}

void Armor::begin()
{
#if controller == 1
  shoulders->areaInstance->begin();
  arms->areaInstance->begin();
  thighs->areaInstance->begin();
  legs->areaInstance->begin();
#endif
#if controller == 2
  head->areaInstance->begin();
  chestTop->areaInstance->begin();
  abs->areaInstance->begin();
  spiral->areaInstance->begin();
  groinRays->areaInstance->begin();
#endif
}

void Armor::show()
{
#if controller == 1
  shoulders->areaInstance->show();
  arms->areaInstance->show();
  thighs->areaInstance->show();
  legs->areaInstance->show();
#endif
#if controller == 2
  head->areaInstance->show();
  chestTop->areaInstance->show();
  abs->areaInstance->show();
  spiral->areaInstance->show();
  groinRays->areaInstance->show();
#endif
}

void Armor::setBrightness(uint8_t brightness)
{
#if controller == 1
  shoulders->areaInstance->setBrightness(brightness);
  arms->areaInstance->setBrightness(brightness);
  thighs->areaInstance->setBrightness(brightness);
  legs->areaInstance->setBrightness(brightness);
#endif
#if controller == 2
  head->areaInstance->setBrightness(brightness);
  chestTop->areaInstance->setBrightness(brightness);
  abs->areaInstance->setBrightness(brightness);
  spiral->areaInstance->setBrightness(brightness);
  groinRays->areaInstance->setBrightness(brightness);
#endif
}
