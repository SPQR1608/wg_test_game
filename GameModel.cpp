#include "GameModel.h"
#include "GameUtils.h"
#include <tinyxml.h>

GameModel::GameModel()
{
	game_stage_ = EGameStage::START;
	selected_map_ = 0;
	LoadMaps();
	Init();
}

void GameModel::Init()
{
	head_.clear();
	grid_items_.clear();

	first_selected_ = { 0,0 };
	second_selected_ = { 0,0 };

	switch (game_stage_)
	{
	case EGameStage::START:
		LoadMenu();
		break;
	case EGameStage::PLAY:
		solved_ = false;
		SetItemColors();		
		SetMap();
		break;
	}
}

void GameModel::Move(const sf::Vector2i& position)
{
	int x = position.x / BLOCK_WIDTH;
	int y = position.y / BLOCK_WIDTH - 1;

	if (x == 0 || y == 0) return;

	if (grid_items_[grid_[x][y]].GetItemType() == EItemType::NOT_MOVABLE) return;

	if (grid_items_[grid_[x][y]].GetItemType() != EItemType::EMPTY)
	{
		first_selected_.x = x;
		first_selected_.y = y;
		std::swap(grid_items_[grid_[x][y]].GetSprite(), grid_items_[grid_[x][y]].GetHoverSprite());
		return;
	}

	if (first_selected_.x && first_selected_.y)
	{
		second_selected_.x = x;
		second_selected_.y = y;
		std::swap(grid_items_[grid_[first_selected_.x][first_selected_.y]].GetSprite(), grid_items_[grid_[first_selected_.x][first_selected_.y]].GetHoverSprite());
	}

	if ((!first_selected_.x && !first_selected_.y) || (!second_selected_.x && !second_selected_.y)) return;

	sf::Vector2i direction_vector = GetDirectionVector<sf::Vector2i>(second_selected_, first_selected_);
	
	if (abs(direction_vector.x) == abs(direction_vector.y) || (abs(direction_vector.x) > 1 || abs(direction_vector.y) > 1))
	{
		second_selected_.x = second_selected_.y = 0;
		return;
	}

	std::swap(grid_[first_selected_.x][first_selected_.y], grid_[second_selected_.x][second_selected_.y]);

	first_selected_.x = first_selected_.y = second_selected_.x = second_selected_.y = 0;

	if (Check()) 
	{
		solved_ = true;
		game_stage_ = EGameStage::WIN;
	}
}

void GameModel::SelectMap(const sf::Vector2i& position)
{

	int button_width = map_buttons_[0].getTexture()->getSize().x;
	int button_height = map_buttons_[0].getTexture()->getSize().y;

	for(int i = 0; i < map_buttons_.size(); ++i)
	{
		if (FindCollision(position, map_buttons_[i].getPosition(), button_width, button_height))
		{
			selected_map_ = i;
			game_stage_ = EGameStage::PLAY;
			Init();
			break;
		}
	}
}

void GameModel::MouseAction(const sf::Vector2i& position)
{
	switch (game_stage_)
	{
	case EGameStage::START:
		SelectMap(position);
		break;
	case EGameStage::PLAY:
		Move(position);
		break;
	}
}

bool GameModel::FindCollision(const sf::Vector2i& first, const sf::Vector2f& second, const int width, const int height)
{
	return (first.x > second.x && first.x < second.x + width) &&
		(first.y > second.y && first.y < second.y + height);
}

bool GameModel::Check()
{
	int headIterator = 0, winIterator = 0;
	for (int i = 0; i < 5; ++i)
	{
		if (i & 1) continue;
		for (int j = 0; j < 5; ++j)
		{
			int n = grid_[i + 1][j + 1];

			if (grid_items_[n].GetColorId() != head_[headIterator].GetColorId())
			{
				return false;
			}
		}
		++headIterator;
	}

	return true;
}

void GameModel::SetItemColors()
{
	if (!grid_item_colors_.empty()) return;

	for (int i = 0; i < 4; ++i)
	{
		items_texture_.push_back(sf::Texture());
		items_hover_texture_.push_back(sf::Texture());
	}

	items_texture_[0].loadFromFile("assets/images/red.png");
	items_texture_[1].loadFromFile("assets/images/green.png");
	items_texture_[2].loadFromFile("assets/images/yellow.png");
	items_texture_[3].loadFromFile("assets/images/blue.png");

	items_hover_texture_[0].loadFromFile("assets/images/green-hover.png");
	items_hover_texture_[1].loadFromFile("assets/images/yellow-hover.png");
	items_hover_texture_[2].loadFromFile("assets/images/blue-hover.png");

	sf::Sprite RedSprite(items_texture_[0]), GreenSprite(items_texture_[1]), YellowSprite(items_texture_[2]), BlueSprite(items_texture_[3]);
	sf::Sprite GreenSpriteHover(items_hover_texture_[0]), YellowSpriteHover(items_hover_texture_[1]), BlueSpriteHover(items_hover_texture_[2]);

	grid_item_colors_ = {
		{"Green", GameItem(GreenSprite, GreenSpriteHover, EColor::GREEN, EItemType::MOVABLE)},
		{"Yellow", GameItem(YellowSprite, YellowSpriteHover, EColor::YELLOW, EItemType::MOVABLE)},
		{"Blue", GameItem(BlueSprite, BlueSpriteHover, EColor::BLUE, EItemType::MOVABLE)},
		{"Red", GameItem(RedSprite, EColor::RED, EItemType::NOT_MOVABLE)},
		{"White", GameItem(EColor::WHITE, EItemType::EMPTY)},
	};
}

void GameModel::LoadMaps()
{
	maps_doc_ = TiXmlDocument("Maps.xml");
	if (!maps_doc_.LoadFile())
	{
		return;
	}

	TiXmlElement* mapItem = maps_doc_.FirstChildElement("Maps")->FirstChildElement("Map");

	while(mapItem)
	{
		maps_.push_back(mapItem);
		mapItem = mapItem->NextSiblingElement("Map");
	}	
}

void GameModel::SetMap()
{
	if (maps_.empty() || grid_item_colors_.empty()) return;

	TiXmlElement* mapHeadItem = maps_[selected_map_]->FirstChildElement("Head")->FirstChildElement("Item");
	TiXmlElement* mapGridRow = maps_[selected_map_]->FirstChildElement("Grid")->FirstChildElement("Row");

	head_.reserve(3);
	while (mapHeadItem)
	{
		std::string itemColor = mapHeadItem->FirstChildElement("color")->GetText();
		if (!itemColor.empty())
		{
			auto gItem = grid_item_colors_.find(itemColor);
			if (gItem != grid_item_colors_.end())
			{
				head_.push_back(gItem->second);
			}
		}

		mapHeadItem = mapHeadItem->NextSiblingElement("Item");
	}

	grid_items_.reserve(26);
	grid_items_.push_back(GameItem()); // In grid starts at 1, this 0

	grid_[GRID_SIZE - 1][GRID_SIZE - 1] = { 0 };

	int gridIterator = 0, grid_i = 0, grid_j = 0;

	while (mapGridRow)
	{
		TiXmlElement* mapGridItem = mapGridRow->FirstChildElement("Item");

		++grid_i;
		grid_j = 0;
		while (mapGridItem)
		{
			++grid_j;
			++gridIterator;

			std::string itemColor = mapGridItem->FirstChildElement("color")->GetText();
			if (!itemColor.empty())
			{
				const auto gItem = grid_item_colors_.find(itemColor);
				if (gItem != grid_item_colors_.end())
				{
					grid_items_.push_back(gItem->second);
				}
			}
			grid_[grid_j][grid_i] = gridIterator;
			mapGridItem = mapGridItem->NextSiblingElement("Item");
		}
		mapGridRow = mapGridRow->NextSiblingElement("Row");
	}
}

void GameModel::LoadMenu()
{
	if (!map_buttons_texture_.empty() && !map_buttons_.empty()) return;

	if (maps_.empty()) return;

	for (int i = 0; i < maps_.size(); ++i)
	{
		map_buttons_texture_.push_back(sf::Texture());
	}

	for (int i = 0; i < maps_.size(); ++i)
	{
		map_buttons_texture_[i].loadFromFile("assets/images/map" + std::to_string(i + 1) + ".png");
		map_buttons_.push_back(sf::Sprite(map_buttons_texture_[i]));
	}
}
