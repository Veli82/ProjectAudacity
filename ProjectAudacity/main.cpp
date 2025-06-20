#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Project/Project.h"


int main()
{
	//setup random
	std::srand((unsigned)std::time(0));

	Project project(44100, 1);
	project.runUI();

	std::cout << "stop";





}