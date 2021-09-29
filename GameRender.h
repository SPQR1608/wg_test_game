#pragma once
#include <SFML/Graphics.hpp>
#include "GameModel.h"

class GameRender : public sf::Drawable, public sf::Transformable
{
public:
	GameRender(GameModel* game);
	~GameRender() {};
	sf::RenderWindow& window() { return window_; };
	void Init();
	void Render();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	virtual void draw_game(sf::RenderTarget& target, sf::RenderStates& states) const;
	virtual void draw_menu(sf::RenderTarget& target, sf::RenderStates& states) const;
	virtual void draw_win(sf::RenderTarget& target, sf::RenderStates& states) const;

protected:
	GameModel* game_;
	sf::RenderWindow window_;
	sf::Text info_text_;
	sf::Font font_;
};