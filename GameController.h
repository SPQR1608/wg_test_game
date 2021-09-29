#pragma once
#include <SFML/Graphics.hpp>
#include "GameRender.h"

class GameController
{
public:
	GameController(GameModel* game, GameRender* render);
	~GameController() {};
	void Run();

private:
	GameModel* game_;
	GameRender* render_;
};