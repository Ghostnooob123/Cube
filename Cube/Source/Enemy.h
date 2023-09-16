#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>

class Enemy final {
public:
	//Constructors & Destructors
	Enemy();
	~Enemy();

	//Accessors
	void reset();
	
	//Spawn enemies
	void spawnEnemy(sf::RenderTarget& target);
	void updateEnemies(sf::RenderTarget& target, unsigned& points, int& playerHealth, sf::Vector2f& mousePosView, int& bossPoints);
	void renderEnemies(sf::RenderTarget& target);

private:
	//Game logic
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;

	//Enemy objects 
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy; 	//Normal enemy

	//Private functions
	void initVariables();
	void initGameObjects();
};

#endif // !ENEMY_H

