#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <Armor.h>

Armor *armor = new Armor();
Colors *colors = new Colors();
uint16_t initialBrightness = 1;
int buttonPin = 2;

// initialize animators

#if controller == 1
Radiate<SHOULDER_SEGMENTS> *radiateShoulders;
Radiate<ARM_SEGMENTS> *radiateArms;
Radiate<THIGH_SEGMENTS> *radiateThighs;
Radiate<LEG_SEGMENTS> *radiateLegs;
#endif

#if controller == 2
Radiate<CHEST_TOP_SEGMENTS> *radiateChestTop;
Radiate<CHEST_TOP_SEGMENTS> *radiateAbs;
Radiate<HEAD_SEGMENTS> *radiateHead;
Radiate<SPIRAL_SEGMENTS> *radiateSpiral;
Radiate<GROIN_RAYS_SEGMENTS> *radiateGroinRays;
#endif

void initializeAnimators()
{
#if controller == 1
  radiateShoulders = new Radiate<SHOULDER_SEGMENTS>(100, 200, 0, armor->shoulders, 0, PaletteKey::base, colors);
  radiateArms = new Radiate<ARM_SEGMENTS>(100, 200, 0, armor->arms, 0, PaletteKey::base, colors);
  radiateThighs = new Radiate<THIGH_SEGMENTS>(100, 200, 0, armor->thighs, 0, PaletteKey::base, colors);
  radiateLegs = new Radiate<LEG_SEGMENTS>(100, 200, 0, armor->legs, 0, PaletteKey::base, colors);
#endif
#if controller == 2
  radiateChestTop = new Radiate<CHEST_TOP_SEGMENTS>(100, 200, 0, armor->chestTop, 0, PaletteKey::base, colors);
  radiateAbs = new Radiate<CHEST_TOP_SEGMENTS>(100, 200, 0, armor->abs, 0, PaletteKey::base, colors);
  radiateHead = new Radiate<HEAD_SEGMENTS>(100, 200, 0, armor->head, 0, PaletteKey::base, colors);
  radiateSpiral = new Radiate<SPIRAL_SEGMENTS>(100, 200, 0, armor->spiral, 0, PaletteKey::base, colors);
  radiateGroinRays = new Radiate<GROIN_RAYS_SEGMENTS>(100, 200, 0, armor->groinRays, 0, PaletteKey::base, colors);
#endif
};

GlobalBreathe *globalBreathe = new GlobalBreathe(armor, 150, initialBrightness, 30);

void setup()
{
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(buttonPin, INPUT);
  initializeAnimators();
  Serial.println('initialized');
  armor->begin();
  armor->setAllOneColor(colors->black);
  armor->setBrightness(initialBrightness);
  armor->show();
  Serial.println(freeMemory());
}

boolean firstPhaseOneRun = true;
void phaseOne(uint64_t runtime)
{
  if (firstPhaseOneRun)
  {
    firstPhaseOneRun = false;
#if controller == 1
    radiateShoulders->changeTimings(50, 0, 0, runtime);
    radiateArms->changeTimings(50, 0, 0, runtime);
    radiateThighs->changeTimings(50, 0, 0, runtime);
    radiateLegs->changeTimings(50, 0, 0, runtime);
#endif
#if controller == 2
    radiateHead->changeTimings(50, 0, 0, runtime);
    radiateChestTop->changeTimings(50, 0, 0, runtime);
    radiateAbs->changeTimings(50, 0, 0, runtime);
    radiateSpiral->changeTimings(50, 0, 0, runtime);
    radiateGroinRays->changeTimings(50, 0, 0, runtime);
#endif
  }
#if controller == 1
  radiateShoulders->advance(runtime);
  radiateArms->advance(runtime);
  radiateThighs->advance(runtime);
  radiateLegs->advance(runtime);
#endif
#if controller == 2
  radiateHead->advance(runtime);
  radiateChestTop->advance(runtime);
  radiateAbs->advance(runtime);
  radiateSpiral->advance(runtime);
  radiateGroinRays->advance(runtime);
#endif
}

boolean finished = false;
uint64_t runtime = 0;
boolean previousButtonState = LOW;
boolean currentButtonState = LOW;
boolean previousComInState = LOW;
boolean currentComInState = LOW;
int phase = 1;
void loop()
{
  // currentButtonState = digitalRead(buttonPin);
  // if (currentButtonState == HIGH && previousButtonState == LOW)
  // {
  //   previousButtonState = HIGH;
  //   if (phase < 4)
  //   {
  //     phase++;
  //   }
  //   else
  //   {
  //     firstPhaseOneRun = true;
  //     secondPhaseFirstRun = true;
  //     thirdPhaseFirstRun = true;
  //     phase = 1;
  //   }
  // }
  // if (currentButtonState == LOW && previousButtonState == HIGH)
  // {
  //   previousButtonState = LOW;
  // }

  if (phase == 1)
  {
    phaseOne(runtime);
  }
  delay(5);
  armor->show();
  runtime++;
}