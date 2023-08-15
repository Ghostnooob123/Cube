#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>

/*
	Class that acts as the game engine.
	Wrapper class
*/

class Game {
public:
	//Constructors & Destructors
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;
	const unsigned getHighestScore();
	const sf::RectangleShape setEnemyType();

	//UI functions
	void updateText();
	void renderText(sf::RenderTarget& target);

	void updateMenu();
	void renderMenu(sf::RenderTarget& target);

	//Game objects functions
	//Spawn enemies
	void spawnEnemy();
	void updateEnemies();
	void renderEnemies(sf::RenderTarget& target);
	//Spawn health packs
	void spawnHealthPack();
	void updateHealthPacks();
	void renderHealthPacks(sf::RenderTarget& target);

	//Game score update
	void updateScore();

	//Game Functions
	void pollEvents();
	void updateMousePos();
	void update();
	void render();

protected:

private:
	//Variables
	//Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event event;

	//Mouse positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Resources
	sf::Font font;

	//Text
	sf::Text uiText;

	sf::RectangleShape highestScoreMenu;
	sf::Text uiHighestScore;

	sf::RectangleShape playButton;
	sf::Text uiPlayText;

	sf::RectangleShape quitButton;
	sf::Text uiQuitText;

	//Game logic
	bool startGame;
	bool endGame;
	unsigned points;
	unsigned highestScore;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;

	float healthPackSpawnTimer;
	float healthPackSpawnTimerMax;

	float centerX;
	float centerY;

	std::string fileName; //Save highest score in this file

	//Game objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	std::vector<sf::CircleShape> healthPacks;
	sf::CircleShape healthPack;

	//Private functions
	void initVariables();
	void initWindow();
	void initFonts();
	void initMenu();
	void initGameText();
	void initGameObjects();
};
#endif // !GAME_H

