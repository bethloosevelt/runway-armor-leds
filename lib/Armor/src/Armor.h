#ifndef Armor_h
#define Armor_h
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

template <uint8_t segmentCount>
class AddressableArea
{
public:
  AddressableArea(uint16_t numLEDs, uint8_t pin, int16_t segmentStarts[segmentCount], int16_t segmentEnds[segmentCount], uint8_t longestSegment);
  uint8_t numSegments;
  uint8_t longestSegment;
  uint16_t numLEDs;
  uint8_t pin;
  int16_t segmentStartIndicies[segmentCount];
  int16_t segmentEndIndicies[segmentCount];
  Adafruit_NeoPixel *areaInstance;
};

const uint8_t CHEST_LEFT_SEGMENTS = 3;
const uint8_t CHEST_RIGHT_SEGMENTS = 2;
class Armor
{
public:
  Armor();
  static void setFullSection(Adafruit_NeoPixel *stripInstance, uint16_t numLEDs, uint32_t color);
  void setBrightness(uint8_t brightness);
  void begin();
  void show();
  void setAllOneColor(uint32_t color)
  {
    setFullSection(chestLeft->areaInstance, chestLeft->numLEDs, color);
    setFullSection(chestRight->areaInstance, chestRight->numLEDs, color);
  }

  /* sections */
  /* chest */
  AddressableArea<CHEST_LEFT_SEGMENTS> *chestLeft;
  AddressableArea<CHEST_RIGHT_SEGMENTS> *chestRight;
};

template <uint8_t numSegments, uint8_t numColors>
class Cycle
{
public:
  Cycle(AddressableArea<numSegments> *area, int duration, uint32_t colorCycles[numColors]) : area(area), duration(duration), cycle(0), lastCycleTime(millis())
  {
    memcpy(colors, colorCycles, sizeof(colorCycles[0]) * numColors);
  }
  void advance()
  {
    int currentTime = millis();
    if (currentTime - lastCycleTime >= duration / numColors)
    {
      lastCycleTime = currentTime;
      Armor::setFullSection(area->areaInstance, area->numLEDs, colors[cycle % 3]);
      cycle++;
    }
  };
  void setColors(uint32_t colorCycles[numColors])
  {
    memcpy(colors, colorCycles, sizeof(colorCycles[0]) * numColors);
  };
  void reset(int duration)
  {
    cycle = 0;
    lastCycleTime = millis();
    this->duration = duration;
  };
  AddressableArea<numSegments> *area;
  uint32_t colors[numColors];
  int duration;
  int cycle;
  int lastCycleTime;
};

template <uint8_t segmentCount, uint8_t numColors>
class Radiate
{
public:
  Radiate(uint32_t colorCycles[numColors], int duration, boolean outward, boolean bounce, AddressableArea<segmentCount> *area)
      : lastFrameTime(millis()), frame(0), cycle(0), outward(outward), duration(duration), bounce(bounce), area(area)
  {
    memcpy(colors, colorCycles, sizeof(colorCycles[0]) * numColors);
  }
  void advance()
  {
    int currentTime = millis();
    if (currentTime - lastFrameTime >= duration / area->longestSegment)
    {
      lastFrameTime = currentTime;
      for (int i = 0; i < area->numSegments; i++)
      {
        int pixelIndex = abs((outward ? area->segmentStartIndicies[i] : area->segmentEndIndicies[i]) + frame);
        area->areaInstance->setPixelColor(pixelIndex, colors[cycle % numColors]);
      }
      if (frame == area->longestSegment - 1)
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
  };
  int duration;
  boolean outward;
  boolean bounce;

private:
  AddressableArea<segmentCount> *area;
  uint8_t frame;
  int lastFrameTime;
  int cycle;
  uint32_t colors[numColors];
};
#endif