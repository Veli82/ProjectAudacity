#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "Project/Project.h"


int main()
{
	//setup random
	std::srand((unsigned)std::time(0));

	Project pr(44100, 1);
	pr.runUI();


	//std::ifstream ifs("res/save1.dat");
	//if (!ifs.is_open())
	//{
	//	throw std::runtime_error("could not open file!");
	//}

	//try
	//{
	//Project project(ifs);
	//project.runUI();
	//}
	//catch (const std::exception& e)
	//{
	//	std::cout << e.what();
	//}

	std::cout << "stop";


}