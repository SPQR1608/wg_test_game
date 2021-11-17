#include "GameModel.h"
#include "GameUtils.h"
#include <tinyxml.h>

GameModel::GameModel()
	: game_stage_(EGameStage::START)
	, selected_map_(0)
{
	LoadMaps();
	Init();
}

void GameModel::Init()
{
	head_.clear();
	grid_items_.clear();

	first_selected_ = 0;
	second_selected_ = 0;

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
	//int x = position.x / BLOCK_WIDTH;
	//int y = position.y / BLOCK_WIDTH;

	int selected_item = 0;
	for (int i = 1; i < grid_items_.size(); ++i)
	{
		if (FindCollision(position, grid_items_[i].GetSprite().getPosition(), BLOCK_WIDTH, BLOCK_WIDTH))
		{
			selected_item = i;
		}
	}
	
	if (selected_item == 0) return;

	if (grid_items_[selected_item].GetItemType() == EItemType::NOT_MOVABLE) return;

	if (grid_items_[selected_item].GetItemType() != EItemType::EMPTY)
	{
		first_selected_ = selected_item;
		std::swap(grid_items_[selected_item].GetSprite(), grid_items_[selected_item].GetHoverSprite());
		return;
	}

	if (first_selected_)
	{
		second_selected_ = selected_item;
		std::swap(grid_items_[first_selected_].GetSprite(), grid_items_[first_selected_].GetHoverSprite());
	}
	
	if (!first_selected_ || !second_selected_) return;
	
	sf::Vector2i f_select_xy = { grid_items_[first_selected_].GetGridPosition().x, grid_items_[first_selected_].GetGridPosition().y };
	sf::Vector2i s_select_xy = { grid_items_[second_selected_].GetGridPosition().x, grid_items_[second_selected_].GetGridPosition().y };
	sf::Vector2i direction_vector = GetDirectionVector<sf::Vector2i>(s_select_xy, f_select_xy);
	
	if (abs(direction_vector.x) == abs(direction_vector.y) || (abs(direction_vector.x) > 1 || abs(direction_vector.y) > 1))
	{
		second_selected_ = 0;
		return;
	}

	std::swap(grid_[f_select_xy.x][f_select_xy.y], grid_[s_select_xy.x][s_select_xy.y]);
	std::swap(grid_items_[first_selected_].GetGridPosition(), grid_items_[second_selected_].GetGridPosition());

	first_selected_ = second_selected_ = 0;

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

bool GameModel::FindCollision(const sf::Vector2i& first, const sf::Vector2f& second, const int width, const int height) const
{
	return (first.x > second.x && first.x < second.x + width) &&
		(first.y > second.y && first.y < second.y + height);
}

bool GameModel::Check()
{
	int headIterator = 0, winIterator = 0;
	for (int i = 0; i < SIZE; ++i)
	{
		if (i & 1) continue;
		for (int j = 0; j < SIZE; ++j)
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

void GameModel::Shuffle()
{
	if (grid_items_.empty()) return;

	int shuffled_items[PLAYABLE_ITEMS];

	for (int i = 1, j = 0; i < grid_items_.size(); ++i)
	{
		if (grid_items_[i].GetItemType() == EItemType::MOVABLE)
		{
			shuffled_items[j] = i;
			++j;
		}
	}

	int random_arr[PLAYABLE_ITEMS];
	GetRandomNum(random_arr, PLAYABLE_ITEMS);
	
	sf::Vector2i gr_xy;
	for (int i = 0; i < PLAYABLE_ITEMS; ++i)
	{
		gr_xy = grid_items_[shuffled_items[i]].GetGridPosition();
		grid_[gr_xy.x][gr_xy.y] = shuffled_items[random_arr[i]];
	}

	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			grid_items_[grid_[i + 1][j + 1]].GetGridPosition() = { i + 1, j + 1 };
		}
	}

	ShuffleSTL<items_vector>(head_);
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
		{"White", GameItem(sf::Sprite(), EColor::WHITE, EItemType::EMPTY)},
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

	head_.reserve(ITEMS_IN_HEAD);
	while (mapHeadItem)
	{
		std::string itemColor = mapHeadItem->FirstChildElement("color")->GetText();
		if (!itemColor.empty())
		{
			const auto gItem = grid_item_colors_.find(itemColor);
			if (gItem != grid_item_colors_.end())
			{
				head_.push_back(gItem->second);
			}
		}

		mapHeadItem = mapHeadItem->NextSiblingElement("Item");
	}

	grid_items_.reserve(ITEMS_IN_GRID);
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
				if (auto gItem(grid_item_colors_.find(itemColor)); gItem != grid_item_colors_.end())
				{
					grid_items_.push_back(gItem->second);
					grid_items_[gridIterator].GetGridPosition() = { grid_j, grid_i };
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

	size_t maps_count = maps_.size();

	map_buttons_texture_.reserve(maps_count);
	map_buttons_.reserve(maps_count);

	for (size_t i = 0; i < maps_count; ++i)
	{
		map_buttons_texture_.push_back(sf::Texture());
	}

	for (size_t i = 0; i < maps_count; ++i)
	{
		map_buttons_texture_[i].loadFromFile("assets/images/map" + std::to_string(i + 1) + ".png");
		map_buttons_.push_back(sf::Sprite(map_buttons_texture_[i]));
	}
}
