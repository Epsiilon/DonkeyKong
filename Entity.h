#pragma once

enum EntityType
{
	player,
	monkey,
	princesse,
	block,
	echelle,
	echelleMonkey,
	marteau,
	tonneau,
	tonneauStock
};

class Entity
{
public:
	Entity() { };
	~Entity() { };

public:
	sf::Sprite m_sprite;
	sf::Vector2u m_size;
	sf::Vector2f m_position;
	EntityType m_type;
	bool m_enabled = true;
	int spriteNumber = 0;

	// Tonneau only
	bool activateTonneau = false;
	int sensMove = 1;
	int compteGravity = 0;

};

