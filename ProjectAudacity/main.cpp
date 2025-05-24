#include <iostream>
#include "WAVSound.h"

int main()
{
	WAVSound sound("test.dat");
	std::cout << sound.getSample(1) << ' ' << sound.getSample(1) << ' ' << sound.getSample(2) << ' ' << sound.getSample(6);
}