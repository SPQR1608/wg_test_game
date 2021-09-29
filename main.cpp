#include "GameModel.h"
#include "GameRender.h"
#include "GameController.h"


int main()
{	
	GameModel game;
	GameRender render(&game);
	GameController controller(&game, &render);
	controller.Run();

	return 0;
}