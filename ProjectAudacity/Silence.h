#pragma once
#include "Sound.h"

class Silence : public Sound
{
	float getSample(int index) override;
};

