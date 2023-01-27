#ifndef Armor_h
#define Armor_h
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

enum class PaletteKey
{
  base,
  blank,
};

class Colors
{
public:
  uint32_t orange;
  uint32_t yellow;
  uint32_t green;
  uint32_t pink;
  uint32_t black;
  uint32_t basePalette[4];
  uint32_t blankPalette[1];
  Colors()
  {
    // orange yellow green pink
    orange = Adafruit_NeoPixel::Color(200, 50, 0);
    yellow = Adafruit_NeoPixel::Color(253, 248, 0);
    green = Adafruit_NeoPixel::Color(69, 247, 255);
    pink = Adafruit_NeoPixel::Color(255, 0, 146);
    black = Adafruit_NeoPixel::Color(0, 0, 0);

    int32_t baseColors[4] = {orange, yellow, green, pink};
    memcpy(basePalette, baseColors, sizeof(baseColors[0]) * 4);
    int32_t blankColors[1] = {black};
    memcpy(blankPalette, blankColors, sizeof(blankColors[0]) * 1);
  }

  uint32_t *getPalette(PaletteKey paletteKey)
  {
    switch (paletteKey)
    {
    case PaletteKey::base:
      return basePalette;
    default:
      return Colors::blankPalette;
      break;
    }
  }

  uint8_t getPaletteLength(PaletteKey paletteKey)
  {
    switch (paletteKey)
    {
    case PaletteKey::base:
      return 4;
    default:
      return 1;
      break;
    }
  }
};

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

const uint8_t CHEST_TOP_SEGMENTS = 6;
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
    setFullSection(chestTop->areaInstance, chestTop->numLEDs, color);
    setFullSection(chestRight->areaInstance, chestRight->numLEDs, color);
  }

  /* sections */
  /* chest */
  AddressableArea<CHEST_TOP_SEGMENTS> *chestTop;
  AddressableArea<CHEST_RIGHT_SEGMENTS> *chestRight;
};

template <uint8_t segmentCount>
class Radiate
{
public:
  Radiate(int duration, int holdDuration, int delayDuration, AddressableArea<segmentCount> *area, uint64_t currentTime, PaletteKey paletteKey, Colors *colors) : duration(duration), holdDuration(holdDuration), delayDuration(delayDuration), paused(delayDuration > 0), area(area), lastFrameTime(currentTime), frame(0), palette(colors->getPalette(paletteKey)), paletteLength(colors->getPaletteLength(paletteKey)), cycle(0), isHolding(false), cycleTime(0), colors(colors), reverse(false) {}
  void changeTimings(int duration, int holdDuration, int delayDuration, uint64_t currentTime)
  {
    this->duration = duration;
    this->holdDuration = holdDuration;
    this->delayDuration = delayDuration;
    this->lastFrameTime = currentTime;
    this->frame = 0;
    this->cycle = 0;
    this->paused = delayDuration > 0;
    this->reverse = false;
    this->isHolding = false;
    this->cycleTime = 0;
    Armor::setFullSection(area->areaInstance, area->numLEDs, colors->black);
  };
  void advance(uint64_t currentTime)
  {
    int timeSinceLastUpdate = currentTime - lastFrameTime;
    int timePerUpdate = floor(duration / area->longestSegment);
    if (!reverse && paused && timeSinceLastUpdate < delayDuration)
    {
      return;
    }
    if (reverse && paused && timeSinceLastUpdate < holdDuration)
    {
      return;
    }
    if (paused)
    {
      isHolding = false;
      paused = false;
      lastFrameTime = currentTime;
    }
    if (timeSinceLastUpdate >= timePerUpdate || frame == 0)
    {
      lastFrameTime = currentTime;
      for (int i = 0; i < area->numSegments; i++)
      {
        int pixelIndex = abs((!reverse ? area->segmentStartIndicies[i] : area->segmentEndIndicies[i]) + frame);
        area->areaInstance->setPixelColor(pixelIndex, !reverse ? this->palette[cycle % this->paletteLength] : colors->black);
      }
      frame++;
    }
    cycleTime++;
    if (cycleTime == duration)
    {
      if (reverse)
      {
        cycle++;
        if (cycle == 1)
        {
          delayDuration = delayDuration * 2;
        }
      }
      cycleTime = 0;
      lastFrameTime = currentTime;
      frame = 0;
      paused = true;
      isHolding = true;
      reverse = !reverse;
    }
  }
  Colors *colors;
  int duration;
  int holdDuration;
  int delayDuration;
  boolean paused;
  AddressableArea<segmentCount> *area;
  uint64_t lastFrameTime;
  int frame;
  int32_t *palette;
  int paletteLength;
  int cycle;
  boolean isHolding;
  int cycleTime;
  boolean reverse;
};

class GlobalBreathe
{
public:
  GlobalBreathe(Armor *armor, int duration, uint16_t minBrightness, uint16_t maxBrightness) : armor(armor), duration(duration), lastFrameTime(0), frameDuration(floor(duration / (maxBrightness - minBrightness))), maxBrightness(maxBrightness), minBrightness(minBrightness), brightness(minBrightness), rising(true) {}
  void advance(int currentTime)
  {
    if (currentTime - lastFrameTime == frameDuration)
    {
      lastFrameTime = currentTime;
      armor->setBrightness(brightness);
      if (rising)
      {
        brightness++;
      }
      else
      {
        brightness--;
      }
      if (brightness == maxBrightness)
      {
        rising = false;
      }
      if (brightness == minBrightness)
      {
        rising = true;
      }
    }
  }
  void reset(int duration, uint16_t minBrightness, uint16_t maxBrightness, int currentTime)
  {
    this->duration = duration;
    this->maxBrightness = maxBrightness;
    this->minBrightness = minBrightness;
    this->frameDuration = duration / (maxBrightness - minBrightness);
    this->brightness = minBrightness;
    this->rising = true;
    lastFrameTime = currentTime;
  }

  Armor *armor;
  int duration;
  int lastFrameTime;
  int frameDuration;
  int cycleDuration;
  uint16_t maxBrightness;
  uint16_t minBrightness;
  uint16_t brightness;
  boolean rising;
};
#endif