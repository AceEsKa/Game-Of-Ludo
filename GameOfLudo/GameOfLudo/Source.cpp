#include "Libraries.h"
#include "Game.cpp"

int main()
{
	Game ludo;
	ludo.DeafultDisplay();
	ludo.SetUpGame();
	ludo.StartGame();


	std::cout << std::endl;
	system("pause");

	return 0;
}