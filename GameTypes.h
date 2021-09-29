#pragma once
#include <SFML/Graphics.hpp>

enum class EColor
{
	GREEN,
	YELLOW,
	BLUE,
	RED,
	WHITE
};

enum class EItemType
{
	NOT_MOVABLE,
	MOVABLE,
	EMPTY
};

struct GameItem
{
public:
	GameItem()
	{
	}
	GameItem(EColor ColorId, EItemType Type)
	{
		color_id_ = ColorId;
		type_ = Type;
	}
	GameItem(sf::Sprite Sprite, EColor ColorId, EItemType Type) : GameItem(ColorId, Type)
	{
		sprite_ = Sprite;
	}
	GameItem(sf::Sprite Sprite, sf::Sprite HoverSprite, EColor ColorId, EItemType Type) : GameItem(Sprite, ColorId, Type)
	{
		hover_sprite_ = HoverSprite;
	}

	sf::Sprite& GetSprite() { return sprite_; }
	sf::Sprite& GetHoverSprite() { return hover_sprite_; }
	EColor GetColorId() { return color_id_; }
	EItemType& GetItemType() { return type_; }
protected:
	sf::Sprite sprite_;
	sf::Sprite hover_sprite_;
	EColor color_id_;
	EItemType type_;
};

enum class EGameStage
{
	START,
	PLAY,
	WIN
};