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
  uint32_t yellow;
  uint32_t orange;
  uint32_t blue;
  uint32_t black;
  uint32_t basePalette[3];
  uint32_t blankPalette[1];
  Colors()
  {
    yellow = Adafruit_NeoPixel::Color(250, 237, 39);
    orange = Adafruit_NeoPixel::Color(255, 95, 31);
    blue = Adafruit_NeoPixel::Color(0, 50, 255);
    black = Adafruit_NeoPixel::Color(0, 0, 0);

    int32_t baseColors[3] = {orange, blue, yellow};
    memcpy(basePalette, baseColors, sizeof(baseColors[0]) * 3);
    int32_t blankColors[1] = {black};
    memcpy(blankPalette, blankColors, sizeof(blankColors[0]) * 3);
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
      return 3;
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

template <uint8_t numSegments>
class Cycle
{
public:
  Cycle(AddressableArea<numSegments> *area, int duration, PaletteKey paletteKey, Colors *colors) : area(area), duration(duration), cycle(0), lastCycleTime(millis()), palette(colors->getPalette(paletteKey)), paletteLength(colors->getPaletteLength(paletteKey)), colors(colors) {}
  void advance()
  {
    int currentTime = millis();
    if (currentTime - lastCycleTime >= duration / this->paletteLength)
    {
      lastCycleTime = currentTime;
      Armor::setFullSection(area->areaInstance, area->numLEDs, this->palette[cycle % this->paletteLength]);
      cycle++;
    }
  };
  void reset(int duration)
  {
    cycle = 0;
    lastCycleTime = millis();
    this->duration = duration;
  };
  void setPalette(PaletteKey paletteKey)
  {
    this->palette = colors->getPalette(paletteKey);
    this->palette = colors->getPaletteLength(paletteKey);
  }
  AddressableArea<numSegments> *area;
  uint32_t *palette;
  uint8_t paletteLength;
  int duration;
  int cycle;
  int lastCycleTime;
  Colors *colors;
};

template <uint8_t segmentCount>
class Radiate
{
public:
  Radiate(int duration, int holdDuration, int delayDuration, AddressableArea<segmentCount> *area, int currentTime, PaletteKey paletteKey, Colors *colors) : duration(duration), holdDuration(holdDuration), delayDuration(delayDuration), paused(delayDuration > 0), area(area), pauseDuration(delayDuration), lastFrameTime(currentTime), frame(0), palette(colors->getPalette(paletteKey)), paletteLength(colors->getPaletteLength(paletteKey)), cycle(0) {}
  void advance(int currentTime)
  {
    int timeSinceLastUpdate = currentTime - lastFrameTime;
    int timePerUpdate = floor(duration / area->longestSegment);
    if (paused && timeSinceLastUpdate < pauseDuration)
    {
      return;
    }
    else if (paused && timeSinceLastUpdate >= pauseDuration)
    {
      paused = false;
      pauseDuration = delayDuration;
      lastFrameTime = currentTime;
      return;
    }
    if (timeSinceLastUpdate >= timePerUpdate || frame == 0)
    {
      lastFrameTime = currentTime;
      for (int i = 0; i < area->numSegments; i++)
      {
        int pixelIndex = abs((true ? area->segmentStartIndicies[i] : area->segmentEndIndicies[i]) + frame);
        area->areaInstance->setPixelColor(pixelIndex, this->palette[cycle % this->paletteLength]);
      }
      frame++;
    }
    if (((area->longestSegment - frame) <= 1) && ((timeSinceLastUpdate - timePerUpdate) < 1))
    {
      cycle++;
      lastFrameTime = currentTime;
      frame = 0;
      pauseDuration = holdDuration;
      paused = true;
    }
  }
  int duration;
  int holdDuration;
  int delayDuration;
  boolean paused;
  AddressableArea<segmentCount> *area;
  int pauseDuration;
  int lastFrameTime;
  int frame;
  int32_t *palette;
  int paletteLength;
  int cycle;
};

class GlobalBreathe
{
public:
  GlobalBreathe(Armor *armor, int duration, uint16_t minBrightness, uint16_t maxBrightness) : armor(armor), duration(duration), lastFrameTime(millis()), frameDuration(duration / (maxBrightness - minBrightness)), maxBrightness(maxBrightness), minBrightness(minBrightness), brightness(minBrightness), rising(true) {}
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
  uint16_t maxBrightness;
  uint16_t minBrightness;
  uint16_t brightness;
  boolean rising;
};
#endif