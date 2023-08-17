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

#include "CubeBoss.h"
#include "Enemy.h"

/*
	Class that acts as the game engine.
	Wrapper class
*/

class Game {
public:
	//Constructors & Destructors
	Game();
	~Game();

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
	sf::Font gamefont;
	sf::Font menuFont;
	sf::Music music;
	sf::Image icon;

	//Text
	sf::Text uiText;
	sf::Text uiBossText;
	sf::Text uiCubeBossHealth;

	sf::RectangleShape highestScoreMenu;
	sf::Text uiHighestScore;

	sf::RectangleShape playButton;
	sf::Text uiPlayText;

	sf::RectangleShape quitButton;
	sf::Text uiQuitText;

	sf::RectangleShape settingsButton;
	sf::Text uiSettingsText;
	sf::RectangleShape settingsMenu;
	sf::CircleShape settingsExitButton;
	sf::CircleShape fullScreenToggle;
	sf::Text uiFullScreenToggle;

	//Game logic
	bool startGame;
	bool endGame;
	bool bossFight;
	int bossPoints;
	int playerHealth;
	unsigned points;
	unsigned highestScore;
	float healthPackSpawnTimer;
	float healthPackSpawnTimerMax;
	bool mouseHeld;
	float centerX;
	float centerY;

	bool isInSettings;
	bool isFullScreen;

	std::string fileName; //Save highest score in this file

	//Game objects
	Enemy enemy;
	CubeBoss cubeBoss;

	sf::RectangleShape TopBoarder; 	//Top boarder
	sf::RectangleShape BottomBoarder; 	//Bottom boarder
	sf::RectangleShape LeftBoarder; 	//Left boarder
	sf::RectangleShape RightBoarder; 	//Right boarder

	std::vector<sf::CircleShape> healthPacks;
	sf::CircleShape healthPack;

	//Private functions
	void initVariables();
	void initWindow();
	void initIcon();
	void initFonts();
	void initMusic();
	void initTextures();
	void initMenu();
	void initSettingsMenu();
	void initGameText();
	void initGameObjects();
};
#endif // !GAME_H

