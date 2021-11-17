#include "GameController.h"

GameController::GameController(GameModel* game, GameRender* render)
	: game_(game)
	, render_(render)
{
}

void GameController::Run()
{
	sf::Event event;

	while (render_->window().isOpen())
	{
		while (render_->window().pollEvent(event))
		{
			if (event.type == sf::Event::Closed) render_->window().close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					game_->MouseAction(sf::Mouse::getPosition(render_->window()));
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					render_->window().close();
					break;
				case sf::Keyboard::F2:
					if (game_->IsSolved()) game_->GetGameStage() = EGameStage::PLAY;
					game_->Init();
					break;
				case sf::Keyboard::F3:
					game_->Init();
					game_->Shuffle();
					break;
				case sf::Keyboard::M:
					game_->GetGameStage() = EGameStage::START;
					game_->Init();
					break;
				}
			}
		}

		render_->Render();

		if (game_->IsSolved())
		{
			game_->Init();
		}
	}
}