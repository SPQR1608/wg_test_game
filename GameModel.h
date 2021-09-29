#pragma once
#include <unordered_map>
#include <vector>
#include <tinyxml.h>
#include "GameTypes.h"
#include <SFML/Graphics.hpp>

class GameModel
{
public:
	using items_vector = std::vector<GameItem>;
	using texture_vector = std::vector<sf::Texture>;
	using sprite_vector = std::vector<sf::Sprite>;
public:
	static const int SIZE = 5;
	static const int GRID_SIZE = SIZE + 2;
	static const int ARRAY_SIZE = SIZE * SIZE;
	static const int BLOCK_WIDTH = 120;

public:
	GameModel();
	void Init();
	void LoadMaps();
	void SetMap();
	void LoadMenu();
	bool Check();
	void Move(const sf::Vector2i& position);
	void SelectMap(const sf::Vector2i& position);
	void MouseAction(const sf::Vector2i& position);
	bool FindCollision(const sf::Vector2i& first, const sf::Vector2f& second, const int width, const int height);
	bool IsSolved() { return solved_; }
	const items_vector& GetHead() { return head_; }
	items_vector& GetGridItems() { return grid_items_; }
	int (*GetGrid())[GRID_SIZE] { return grid_; }
	EGameStage& GetGameStage() { return game_stage_; }
	sprite_vector& GetMapButtons() { return map_buttons_; }	
	
protected:
	items_vector head_;
	items_vector grid_items_;
	int grid_[GRID_SIZE][GRID_SIZE];
	std::unordered_map<std::string, GameItem> grid_item_colors_;
	bool solved_;
	EGameStage game_stage_;
	int selected_map_;	
	sprite_vector map_buttons_;

private:
	sf::Vector2i first_selected_{ 0, 0 };
	sf::Vector2i second_selected_{ 0,0 };
	sf::Texture texture_;
	texture_vector items_texture_;
	texture_vector items_hover_texture_;
	texture_vector map_buttons_texture_;
	std::vector<TiXmlElement*> maps_;
	TiXmlDocument maps_doc_;

	void SetItemColors();
};