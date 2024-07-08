/*
  ==============================================================================

    СounterComponent.h
    Created: 31 Jul 2023 8:14:41pm
    Author:  LKDat

  ==============================================================================
*/

#pragma once

#include "ADSRComponent.h"
#include "OSCComponent.h"
#include "FilterComponent.h"

class CounterComponent {
public:
	int getAmountOsc();

private:

	int amountOSC = 0;

};
