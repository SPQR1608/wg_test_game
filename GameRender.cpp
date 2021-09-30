#include "GameRender.h"

GameRender::GameRender(GameModel* game)
{
	game_ = game;
	Init();
}

void GameRender::Init()
{
	setPosition(50.f, 50.f);
	window_.create(sf::VideoMode(700, 720), "Wargaming test game");
	window_.setFramerateLimit(60);

	font_.loadFromFile("assets/fonts/calibri.ttf");

	info_text_ = sf::Text("M - Menu (select map) / F2 - Reset Game / F3 - Shuffle items / Esc - Exit\nMouse left - Choise of movable, second click - choice place to move", font_, 20);
	info_text_.setFillColor(sf::Color::Cyan);
	info_text_.setPosition(5.0f, 5.0f);	
}

void GameRender::Render()
{
	window_.clear();
	window_.draw(*this);
	window_.draw(info_text_);
	window_.display();
}

void GameRender::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	switch (game_->GetGameStage())
	{
	case EGameStage::START:
		draw_menu(target, states);
		break;
	case EGameStage::PLAY:
		draw_game(target, states);
		break;
	case EGameStage::WIN:
		draw_win(target, states);
	}
}

void GameRender::draw_game(sf::RenderTarget& target, sf::RenderStates& states) const
{
	auto& grid_items = game_->GetGridItems();
	auto* grid = game_->GetGrid();
	auto head = game_->GetHead();

	if (grid_items.empty() || head.empty()) return;

	for (int i = 0; i < head.size(); ++i)
	{
		head[i].GetSprite().setPosition(static_cast<float>(100.0f + (2 * i * GameModel::BLOCK_WIDTH)), 100.f);
		target.draw(head[i].GetSprite(), states);
	}

	for (int i = 0; i < GameModel::SIZE; ++i)
	{
		for (int j = 0; j < GameModel::SIZE; ++j)
		{
			int n = grid[i + 1][j + 1];
			grid_items[n].GetSprite().setPosition(static_cast<float>(100.0f + (i * GameModel::BLOCK_WIDTH)), static_cast<float>(230.0f + (j * GameModel::BLOCK_WIDTH)));
			target.draw(grid_items[n].GetSprite(), states);
		}
	}
}

void GameRender::draw_menu(sf::RenderTarget& target, sf::RenderStates& states) const
{
	auto& map_buttons = game_->GetMapButtons();

	if (map_buttons.empty()) return;

	for (int i = 0; i < map_buttons.size(); ++i)
	{
		map_buttons[i].setPosition(static_cast<float>(30 + (i * map_buttons[i].getTexture()->getSize().x) + (i * 50)), 270.f);
		target.draw(map_buttons[i], states);
	}

	sf::Text title_text_("Select map", font_, 80);
	title_text_.setFillColor(sf::Color::Cyan);
	title_text_.setPosition(165.0f, 120.0f);

	target.draw(title_text_, states);
}

void GameRender::draw_win(sf::RenderTarget& target, sf::RenderStates& states) const
{
	sf::Text title_text_("You win", font_, 100);
	title_text_.setFillColor(sf::Color::Cyan);
	title_text_.setPosition(180.0f, 250.0f);

	target.draw(title_text_, states);
}

