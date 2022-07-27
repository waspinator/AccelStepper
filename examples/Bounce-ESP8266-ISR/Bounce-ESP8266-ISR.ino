// Bounce.pde
// -*- mode: C++ -*-
//
// Make a single stepper bounce from one limit to another
//
// Copyright (C) 2012 Mike McCauley
// $Id: Random.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

// Define a stepper and the pins it will use
#define DIR             12              // esp8266 gpio
#define STEP            13              // esp8266 gpio
AccelStepper stepper(AccelStepper::DRIVER, STEP, DIR);

IRAM_ATTR
void stepperRun ()
{
    stepper.run();
}

void setup()
{
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(10000);
  stepper.setAcceleration(10000);
  stepper.moveTo(500);

  // initialize ISR
  timer1_isr_init();
  timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);
  timer1_attachInterrupt(stepperRun);
  timer1_write(microsecondsToClockCycles(100));
}

void loop()
{
    // cli() / sei() barriers are necessary when timer is enabled
    cli();

    // If at the end of travel go to the other end
    if (stepper.distanceToGo() == 0)
      stepper.moveTo(-stepper.currentPosition());

    sei(); // barrier ends

    delay(1000); // ISR does not care long delays or WiFi operations
}
