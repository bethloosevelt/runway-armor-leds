#ifndef Armor_h
#define Armor_h
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

template <int segmentCount>
class AddressableArea
{
public:
  AddressableArea(uint16_t numLEDs, uint8_t pin, int16_t segmentStarts[segmentCount], int16_t segmentEnds[segmentCount]);
  uint8_t numSegments;
  uint16_t numLEDs;
  uint8_t pin;
  int16_t segmentStartIndicies[segmentCount];
  int16_t segmentEndIndicies[segmentCount];
  Adafruit_NeoPixel *areaInstance;
};

class Armor
{
public:
  Armor();
  void setBrightness(uint8_t brightness);
  void begin();
  void show();

  /* sections */
  /* chest */
  AddressableArea<3> *chestLeft;
  AddressableArea<2> *chestRight;
};

#endif