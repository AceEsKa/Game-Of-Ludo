#pragma once
#include <iostream>
#include <string>

unsigned int GetUserInput(std::string message)
{
	unsigned int input;
	bool InvalidInput = false;
	do
	{
		InvalidInput = false;
		std::cout << message << std::endl;
		std::cout << "----------------------" << std::endl;
		std::cin >> input;
		try
		{
			if (std::cin.fail())
			{
				throw "error";
				std::cin.clear();
				std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			}
		}
		catch (const char* error)
		{
			std::cerr << error << std::endl;
			InvalidInput = true;
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}
		std::cout << std::endl;
	} while (InvalidInput);

	return input;
}