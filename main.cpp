#include"Game.h"
#include "Linking/GLFW/include/glfw3.h"


//Sending sizeof pointers and errors


int main()
{
	Game game("CARS_GAME_ENGINE",
		1920, 1080,						//size
		4, 4,							//version
		false);



	//MAIN LOOP
	while (!game.getWindowShouldClose())
	{
		//UPDATE INPUT ---
		game.update();
		game.render();// the curson can interract with the window
		
		
	}


	return 0;
}