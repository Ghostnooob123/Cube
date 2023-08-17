#ifndef CUBE_BOSS_H
#define CUBE_BOSS_H

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>

class CubeBoss {
public:
	//Constructors & Destructors
	CubeBoss();
	~CubeBoss();

	//Accessors
	const int getHealth() const; //Getter
	void setHealth(); //Setter
	void reset(); //Setter

	//Spawn Cube boss enemy
	void spawnCubeBoss(float& centerX, float& centerY);
	void updateCubeBoss(float& centerX, float& centerY, sf::Vector2f& mousePosView, sf::RenderTarget& target, int& playerHealth);
	void renderCubeBoss(sf::RenderTarget& target);
	
	//Spawn Cube boss attack
	void spawnCubeBossAttack(float& centerX, float& centerY);
	
	//Spawn Cube boss boarders
	void spawnBoarders(float& centerX, float& centerY);
	void updateBoarders(float& centerX, float& centerY);
	void renderBoarders(sf::RenderTarget& target);

private:
	//Variables

	//Cube boss logic
	int bossHealth;
	int maxBosses;
	int maxAttacks;
	bool moveRight;
	bool mouseHeld;

	float bossAttackTimerMax;
	float bossAttackTimer;

	//Cube boss objects
	sf::RectangleShape cubeBossEnemy; 	//Cube boss
	sf::RectangleShape cubeBoss_Summon_Attack; //Cube boss attack
	std::vector<sf::RectangleShape> cubeAttacks; 	//Cube boss attacks

	sf::RectangleShape TopBoarder; 	//Top boarder
	sf::RectangleShape BottomBoarder; 	//Bottom boarder
	sf::RectangleShape LeftBoarder; 	//Left boarder
	sf::RectangleShape RightBoarder; 	//Right boarder

	//Private functions
	void initVariables();
	void initObjects();

};
#endif // !CUBE_BOSS_H