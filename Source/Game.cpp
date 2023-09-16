#include "Game.h"

//Constructors & Destructors
Game::Game() {
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initMenu();
	this->initSettingsMenu();
	this->initMusic();
	this->initIcon();
	this->initGameText();
	this->initGameObjects();
}
Game::~Game() {
	delete this->window;
}

//Accessors
const bool Game::running() const {
	/*
	   @return const bool
	   -Return is window open.

	*/
	return this->window->isOpen();
}
const unsigned Game::getHighestScore() {
	/*
       @return const unsigned
	   -Return highest score recorded.
	
	*/
	unsigned score{ 0 };

	std::ifstream inputFile(this->fileName);
	std::string line;
	while (std::getline(inputFile, line)) {
		score = std::stoi(line);
	}
	inputFile.close();

	return score;
}

//UI functions
void Game::updateText() {
	/*
	   @return void

	   -Update Points and Health text when game start.
	*/
	std::ostringstream ostrHealthAndPoints;
	std::ostringstream ostrBossFlag;
	std::ostringstream ostrCubeBossHealth;

	ostrHealthAndPoints << "Points: " << this->points << '\n';
	ostrHealthAndPoints << "Health: " << this->playerHealth << '\n';

	ostrBossFlag << " BOSS! " << '\n';
	ostrCubeBossHealth << "BOSS HEALTH: " <<  this->cubeBoss.getHealth() << '\n';

	this->uiText.setString(ostrHealthAndPoints.str());

	this->uiBossText.setString(ostrBossFlag.str());
	this->uiCubeBossHealth.setString(ostrCubeBossHealth.str());
}
void Game::renderText(sf::RenderTarget& target) {
	//Render Points and Health text
	target.draw(this->uiText);

	if (this->bossPoints == 4990) {
		target.draw(this->uiBossText);
	}

	if (this->bossFight) {
		//Render Boss! text
		target.draw(this->uiCubeBossHealth);
	}
}

void Game::updateMenu() {
	/*
	   @return void

	   - Update Menu.
	   - Check for game start or game quit.
	   - Update high score text.
	   - Show all menu panels.
	*/
	if (this->startGame == false)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (this->playButton.getGlobalBounds().contains(this->mousePosView))
			{
				this->startGame = true;
				this->music.play(); // Start playing the music here
			}
			if (this->quitButton.getGlobalBounds().contains(this->mousePosView))
			{
				this->window->close();
			}
			if (this->settingsButton.getGlobalBounds().contains(this->mousePosView))
			{
				this->isInSettings = true;
			}
			if (this->settingsExitButton.getGlobalBounds().contains(this->mousePosView))
			{
				this->isInSettings = false;
			}
			if (this->fullScreenToggle.getGlobalBounds().contains(this->mousePosView))
			{
				// Toggle fullscreen
				if (isFullScreen)
				{
					this->window->create(sf::VideoMode::getFullscreenModes()[0], "Cube", sf::Style::Resize | sf::Style::Close);
					this->fullScreenToggle.setFillColor(sf::Color::Red);
					isFullScreen = false;
				}
				else
				{
					this->window->create(sf::VideoMode::getFullscreenModes()[0], "Cube", sf::Style::Fullscreen);
					this->fullScreenToggle.setFillColor(sf::Color::Green);
					isFullScreen = true;
				}
			}
		}
	}

	std::ostringstream ostrPlay;
	std::ostringstream ostrHighestScore;
	std::ostringstream ostrQuit;
	std::ostringstream ostrSettings;
	std::ostringstream ostrFullScreenToggle;

	ostrHighestScore << " Highest score:  " << this->getHighestScore() << '\n';
	ostrPlay << " Play " << '\n';
	ostrQuit << " Quit " << '\n';
	ostrSettings << " Settings " << '\n';
	ostrFullScreenToggle << " Fullscreen " << '\n';

	this->uiHighestScore.setString(ostrHighestScore.str());
	this->uiPlayText.setString(ostrPlay.str());
	this->uiQuitText.setString(ostrQuit.str());
	this->uiSettingsText.setString(ostrSettings.str());
	this->uiFullScreenToggle.setString(ostrFullScreenToggle.str());
}
void Game::renderMenu(sf::RenderTarget& target) {
	//Render high score panel
	target.draw(this->highestScoreMenu);
	target.draw(this->uiHighestScore);
	//Render play button
	target.draw(this->playButton);
	target.draw(this->uiPlayText);
	//Render quit button
	target.draw(this->quitButton);
	target.draw(this->uiQuitText);
	//Render settings button
	target.draw(this->settingsButton);
	target.draw(this->uiSettingsText);
	if (this->isInSettings)
	{
		target.draw(this->settingsMenu);
		target.draw(this->settingsExitButton);
		target.draw(this->fullScreenToggle);
		target.draw(this->uiFullScreenToggle);
	}
}

void Game::spawnHealthPack() {
	/*
	 @return void

	 - Spawing health pack.
	 - Set health pack spawn location.
	 - Set health pack color.
	*/

	//Health pack spawn
	this->healthPack.setPosition( static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - 2* this->healthPack.getRadius())), 0.0f);

	this->healthPack.setFillColor(sf::Color(240, 7, 61));

	this->healthPacks.push_back(this->healthPack);

}
void Game::updateHealthPacks() {
	/*
	 @return void

	 - Updating health pack timer.
	 - Spawing health pack.
	 - Moving the health pack.
	
    */
	//Updating the timer and spawning health pack
	if (this->healthPacks.size() < 1) {
		if (this->healthPackSpawnTimer >= this->healthPackSpawnTimerMax && this->points >= 500) {
			//Spawn enemy and reset timer
			this->spawnHealthPack();
			this->healthPackSpawnTimer = 0.0f;
		}
		else
			this->healthPackSpawnTimer += 1.0f;
	}

	//Moving the health pack
	for (size_t i = 0; i < this->healthPacks.size(); ++i)
	{
		//Move health pack on the screen
		this->healthPacks[i].move(0.0f, 1.7f);

		//If the health pack is past the bottom of the screen
		if (this->healthPacks[i].getPosition().y > this->window->getSize().y)
		{
			//Delete health pack on the screen
			this->healthPacks.erase(this->healthPacks.begin() + i);
		}
	}

	//Updating health packs
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		for (size_t i = 0; i < this->healthPacks.size(); ++i)
		{
			if (this->healthPacks[i].getGlobalBounds().contains(this->mousePosView)) {
				//Gain health
				if (this->playerHealth < 100)
				{
					this->playerHealth += 5;
					if (this->playerHealth > 100) { this->playerHealth = 100; }

				}
				//Delete health pack when touched
				this->healthPacks.erase(this->healthPacks.begin() + i);
			}
		}
	}
	else { this->mouseHeld = false; }
}
void Game::renderHealthPacks(sf::RenderTarget& target) {
	//Render health packs on the screen
	for (auto& healthDrop : this->healthPacks) {
		target.draw(healthDrop);
	}
}

void Game::updateScore() {
	/*
	  @return void
	  - Set high score to all gained points and check if there is a new high score.
	*/
	this->highestScore = this->points;

	std::ifstream inputFile(this->fileName);
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) {
			if (!line.empty()) {
				int scoreFromFile = std::stoi(line);
				if (this->highestScore > static_cast<unsigned>(scoreFromFile)) {
					std::ofstream outputFile(this->fileName, std::ios::trunc);
					if (outputFile.is_open()) {
						outputFile << this->highestScore << '\n';
						outputFile.close();
					}
					else {
						std::cout << "[-] Error::Game::updateScore Failed to open output file\n";
					}
				}
			}
		}
		inputFile.close();
	}
	else {
		std::cout << "[-] Error::Game::updateScore Failed to open input file\n";
	}
}

//Game functions
void Game::pollEvents() {
	/*
		@return void

		- Event polling
	*/
	while (this->window->pollEvent(this->event))
	{
		switch (this->event.type) {
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->event.key.code == sf::Keyboard::Escape) { this->window->close(); }
			break;
		}
	}
}
void Game::updateMousePos() {
	/**
	  @return void

	  - Update mouse postitions:
	  - Mouse position relative to window.
	*/
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}
void Game::update() {
	/*
		@return void

		- Poll the event.
		- Update mouse position.
		- Update start menu.
		- Update enemies.
		- Update UI's.
		- Update health packs.
		- Update high score system.

		Restarting the game until Quit.
	*/
	//Poll the events
	this->pollEvents();

	//Update mouse position
	this->updateMousePos();

	//Update menu
	this->updateMenu();

	if (this->startGame) {
		//Check if game end
		if (this->playerHealth <= 0) {
			this->endGame = true;
		}
		//Check if boss spawn
		if (this->bossPoints >= 5000) {
			this->bossFight = true;
			//Check if boss died and reset it
			if (this->cubeBoss.getHealth() <= 0) {
				this->bossFight = false;
				this->bossPoints = 0;
				this->cubeBoss.setHealth();
				this->points += 1500;
				this->playerHealth += 10;
				if (this->playerHealth >= 100){ this->playerHealth = 100; }
			}
		}
		if (!this->endGame)
		{
			//Update text
			this->updateText();

			if (!this->bossFight) {
				//Update enemies
				this->enemy.updateEnemies(*this->window,this->points, this->playerHealth, this->mousePosView, this->bossPoints);
				//Update health packs
				this->updateHealthPacks();
			}
			else
			{
				//Update cube boss
				this->cubeBoss.updateCubeBoss(this->centerX, this->centerY, this->mousePosView, *this->window, this->playerHealth);
				this->cubeBoss.updateBoarders(this->centerX, this->centerY);
			}
		}
		else
		{
			//Update the score after the game
			this->updateScore();

			//Reset game variables
			this->points = 0;
			this->playerHealth = 100;
			this->bossPoints = 0;

			this->cubeBoss.reset();
			this->enemy.reset();

			this->music.stop(); // Stop playing the music here

			//Game start again
			this->startGame = false;
			this->endGame = false;
			this->bossFight = false;
		}
	}
}
void Game::render() {
	/*
		@return void

		- Clear old frame.
		- Render objects.
		- Render start menu.
		- Renders the game objects.
	    - Display frame in window.
	*/
	this->window->clear(sf::Color::Black);

	if (!this->startGame)
	{
		//Draw menu
		this->renderMenu(*this->window);
	}
	else
	{
		//Draw game objects
		if (!this->bossFight) 
		{
			this->enemy.renderEnemies(*this->window);
			this->renderHealthPacks(*this->window);
		}
		else 
		{
			this->cubeBoss.renderCubeBoss(*this->window);
			this->cubeBoss.renderBoarders(*this->window);
		}
		this->renderText(*this->window);
	}
	this->window->display();
}

//Private Game Functions
void Game::initVariables() {
	this->window = nullptr;

	//Game logic
	this->startGame = false; //Is game started
	this->endGame = false; //Is game ended
	this->bossFight = false; //Is boss fight begin
	this->bossPoints = 0; //Cube boss points to spawn
	this->playerHealth = 100; //Player health
	this->points = 0; //Points player gained
	this->highestScore = 0; //Highest score recorded on the game
	this->mouseHeld = false;

	this->isInSettings = false;
	this->isFullScreen = true;

	this->healthPackSpawnTimerMax = 2000.0f; //Health pack spawn timer max
	this->healthPackSpawnTimer = 0.0f; //Health pack spawn timer

	this->centerX = 0.0f; //Center of the screen by X
	this->centerY = 0.0f; //Center of the screen by Y
		
	this->fileName = "Higest Score.txt";
}
void Game::initWindow() {
	//Set window by default
	this->videoMode.height = 800;
	this->videoMode.width = 1300;

	this->window = new sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], "Cube", sf::Style::Fullscreen);

	//Set window frame rate limit
	this->window->setFramerateLimit(500);

	//Center of the screen
	this->centerX = this->window->getSize().x / 2.0f;
	this->centerY = this->window->getSize().y / 2.0f;
}
void Game::initIcon() {
	//Load icon from an png file
	//Check if icon can be loaded
	if (!this->icon.loadFromFile("Icon/Cube.png"))
	{
		// Error handling...
		std::cout << "[-] Error::Game::initIcon Failed to load the font\n";
	}
	this->window->setIcon(this->icon.getSize().x, this->icon.getSize().y, this->icon.getPixelsPtr());
}
void Game::initFonts() {
	//Load font from an font file
	//Check if font can be loaded
	if (!this->gamefont.loadFromFile("Fonts/PixellettersFull.ttf")) {
		std::cout << "[-] Error::Game::initFonts Failed to load the font\n";
	}
	if (!this->menuFont.loadFromFile("Fonts/PixellettersFull.ttf")) {
		std::cout << "[-] Error::Game::initFonts Failed to load the font\n";
	}
}
void Game::initMusic() {
	//Open music from an audio file
	//Check if music can be loaded
	if (!this->music.openFromFile("Music/Cube Chillv2.wav")) {
		std::cout << "[-] Error::Game::initMusic Failed to load the font\n";
	}

	// Change some parameters
	this->music.setLoop(true);
	this->music.setVolume(2.5f);
}
void Game::initMenu() {
	//Set highest score menu by default
	this->highestScoreMenu.setFillColor(sf::Color::White);
	this->highestScoreMenu.setSize(sf::Vector2f(700.0f, 90.0f));
	this->highestScoreMenu.setPosition(centerX - this->highestScoreMenu.getSize().x / 2.0f, 50.0f);

	//Set highest score UI by default
	this->uiHighestScore.setFont(this->menuFont);
	this->uiHighestScore.setCharacterSize(65);
	this->uiHighestScore.setPosition(this->highestScoreMenu.getPosition().x + 120.0f, this->highestScoreMenu.getPosition().y -5.0f);
	this->uiHighestScore.setFillColor(sf::Color::Black);
	this->uiHighestScore.setString("NONE");

	//Set play button by default
	this->playButton.setFillColor(sf::Color::White);
	this->playButton.setSize(sf::Vector2f(300.0f, 60.0f));
	this->playButton.setPosition(centerX - this->playButton.getSize().x / 2.0f, centerY + 50.0f);

	//Set play button UI by default
	this->uiPlayText.setFont(this->menuFont);
	this->uiPlayText.setCharacterSize(55);
	this->uiPlayText.setPosition(this->playButton.getPosition().x + 100.0f, this->playButton.getPosition().y - 12.0f);
	this->uiPlayText.setFillColor(sf::Color::Black);
	this->uiPlayText.setString("NONE");

	//Set settings button by default
	this->settingsButton.setFillColor(sf::Color::White);
	this->settingsButton.setSize(sf::Vector2f(300.0f, 60.0f));
	this->settingsButton.setPosition(centerX - this->settingsButton.getSize().x / 2.0f, centerY + 120.0f);

	//Set settings button UI by default
	this->uiSettingsText.setFont(this->menuFont);
	this->uiSettingsText.setCharacterSize(55);
	this->uiSettingsText.setPosition(this->settingsButton.getPosition().x + 65.0f, this->settingsButton.getPosition().y - 12.0f);
	this->uiSettingsText.setFillColor(sf::Color::Black);
	this->uiSettingsText.setString("NONE");

	//Set quit button by default
	this->quitButton.setFillColor(sf::Color::White);
	this->quitButton.setSize(sf::Vector2f(300.0f, 60.0f));
	this->quitButton.setPosition(centerX - this->quitButton.getSize().x / 2.0f, centerY + 250.0f);

	//Set quit button UI by default
	this->uiQuitText.setFont(this->menuFont);
	this->uiQuitText.setCharacterSize(55);
	this->uiQuitText.setPosition(this->quitButton.getPosition().x + 100.0f, this->quitButton.getPosition().y - 12.0f);
	this->uiQuitText.setFillColor(sf::Color::Black);
	this->uiQuitText.setString("NONE");
}
void Game::initSettingsMenu() {
	//Set settings menu by default
	this->settingsMenu.setFillColor(sf::Color::White);
	this->settingsMenu.setSize(sf::Vector2f(650.0f, 400.0f));
	this->settingsMenu.setPosition(centerX - this->highestScoreMenu.getSize().x + 750.0f / 2.0f, 170.0f);
	this->settingsMenu.setOutlineColor(sf::Color(84, 81, 79));
	this->settingsMenu.setOutlineThickness(6.0f);
	//Exit from settings menu
	this->settingsExitButton.setFillColor(sf::Color(84, 81, 79));
	this->settingsExitButton.setOutlineColor(sf::Color::Black);
	this->settingsExitButton.setOutlineThickness(3.0f);
	this->settingsExitButton.setRadius(15.0f);
	this->settingsExitButton.setScale(sf::Vector2f(1.5f, 1.0f));
	float settingsExitButtonX = this->settingsMenu.getPosition().x + this->settingsMenu.getSize().x + 580.0f / - this->settingsExitButton.getRadius();
	float settingsExitButtonY = this->settingsMenu.getPosition().y + this->settingsMenu.getSize().y - 410.0f; // Adjusted Y position
	this->settingsExitButton.setPosition(settingsExitButtonX, settingsExitButtonY);

	//Toggle full screen button
	this->fullScreenToggle.setFillColor(sf::Color::Green);
	this->fullScreenToggle.setOutlineColor(sf::Color::Black);
	this->fullScreenToggle.setOutlineThickness(3.0f);
	this->fullScreenToggle.setRadius(15.0f);
	this->fullScreenToggle.setScale(1.5f, 1.0f);
	this->fullScreenToggle.setPosition(settingsExitButtonX - 580.0f, settingsExitButtonY + 30.0f);

	//Set fullScreenToggle button text by default
	this->uiFullScreenToggle.setFont(this->menuFont);
	this->uiFullScreenToggle.setCharacterSize(45);
	this->uiFullScreenToggle.setScale(sf::Vector2f(1.0f, 1.0f));
	this->uiFullScreenToggle.setPosition(this->fullScreenToggle.getPosition().x + 70.0f, this->fullScreenToggle.getPosition().y - 20.0f);
	this->uiFullScreenToggle.setFillColor(sf::Color::Black);
	this->uiFullScreenToggle.setString("NONE");
}
void Game::initGameText() {
	//Set text by default
	this->uiText.setFont(this->gamefont);
	this->uiText.setCharacterSize(40);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");

	//Set boss text by default
	float cubeBossX = centerX - 210.0f; // Adjust this value based on the size of your cubeBossEnemy
	float cubeBossY = centerY - 580.0f; // Adjust this value based on the size of your cubeBossEnemy

	this->uiBossText.setFont(this->gamefont);
	this->uiBossText.setCharacterSize(150);
	this->uiBossText.setFillColor(sf::Color::White);
	this->uiBossText.setPosition(cubeBossX, cubeBossY);
	this->uiBossText.setString("NONE");

	//Set cube boss health text by default
	this->uiCubeBossHealth.setFont(this->gamefont);
	this->uiCubeBossHealth.setCharacterSize(75);
	this->uiCubeBossHealth.setFillColor(sf::Color::White);
	this->uiCubeBossHealth.setPosition(cubeBossX - 75.0f, cubeBossY + 900.0f);
	this->uiCubeBossHealth.setString("NONE");
}
void Game::initGameObjects() {
	//Default game objects values
	this->healthPack.setRadius(25.0f);
	this->cubeBoss;
	this->enemy;
}