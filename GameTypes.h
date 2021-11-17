#pragma once
#include <SFML/Graphics.hpp>

enum class EColor : uint8_t
{
	GREEN,
	YELLOW,
	BLUE,
	RED,
	WHITE
};

enum class EItemType : uint8_t
{
	NOT_MOVABLE,
	MOVABLE,
	EMPTY
};

struct GameItem
{
public:
	GameItem()
		: color_id_(EColor::WHITE)
		, type_(EItemType::EMPTY)
	{
	}
	GameItem(EColor ColorId, EItemType Type)
		: color_id_(ColorId)
		, type_(Type)
	{
	}
	GameItem(sf::Sprite Sprite, EColor ColorId, EItemType Type) : GameItem(ColorId, Type)
	{
		sprite_ = Sprite;
	}
	GameItem(sf::Sprite Sprite, sf::Sprite HoverSprite, EColor ColorId, EItemType Type) : GameItem(Sprite, ColorId, Type)
	{
		hover_sprite_ = HoverSprite;
	}

	EColor GetColorId() { return color_id_; }
	EItemType GetItemType() const { return type_; }

	sf::Sprite& GetSprite() { return sprite_; }
	const sf::Sprite& GetSprite() const { return sprite_; }

	sf::Sprite& GetHoverSprite() { return hover_sprite_; }
	const sf::Sprite& GetHoverSprite() const { return hover_sprite_; }

	sf::Vector2i& GetGridPosition() { return grid_position_; }
	const sf::Vector2i& GetGridPosition() const { return grid_position_; }
protected:
	EColor color_id_;
	EItemType type_;
	sf::Sprite sprite_;
	sf::Sprite hover_sprite_;
	sf::Vector2i grid_position_;
};

enum class EGameStage : uint8_t
{
	START,
	PLAY,
	WIN
};