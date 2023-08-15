#include "Game.h"

//Constructors & Destructors
Game::Game() {
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initMenu();
	this->initGameText();
	this->initGameObjects();
}
Game::~Game() {
	delete this->window;
}

//Accessors
const bool Game::running() const {
	return this->window->isOpen();
}
const unsigned Game::getHighestScore() {
	unsigned score{ 0 };

	std::ifstream inputFile(this->fileName);
	std::string line;
	while (std::getline(inputFile, line)) {
		score = std::stoi(line);
	}
	inputFile.close();

	return score;
}
const sf::RectangleShape Game::setEnemyType() {

	sf::Color randomColor(rand() % 256, rand() % 256, rand() % 256);
	this->enemy.setFillColor(randomColor);

	return this->enemy;
}

//UI functions
void Game::updateText() {
	std::ostringstream ostr;

	ostr << "Points: " << this->points << '\n';
	ostr << "Health: " << this->health << '\n';

	this->uiText.setString(ostr.str());
}
void Game::renderText(sf::RenderTarget& target) {
	//Render Points and Health text
	target.draw(this->uiText);
}

void Game::updateMenu() {

	if (this->startGame == false)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (this->playButton.getGlobalBounds().contains(this->mousePosView))
			{
				this->startGame = true;
			}
			if (this->quitButton.getGlobalBounds().contains(this->mousePosView))
			{
				this->window->close();
			}
		}
	}

	std::ostringstream ostrPlay;
	std::ostringstream ostrHighestScore;
	std::ostringstream ostrQuit;

	ostrHighestScore << " Highest score:  " << this->getHighestScore() << '\n';
	ostrPlay << " Play " << '\n';
	ostrQuit << " Quit " << '\n';

	this->uiHighestScore.setString(ostrHighestScore.str());
	this->uiPlayText.setString(ostrPlay.str());
	this->uiQuitText.setString(ostrQuit.str());
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
}

//Game objects functions
void Game::spawnEnemy() {
	/*
	   @return void

	   - Spawns enemies and sets their types, colors and positions randomly.
	   - Sets random type(diff).
	   - Sets a random positions.
	   - Sets a random color.
	   - Sets a random size.
	   - Add enemy to the vector(enemies).
	*/

	this->enemy.setPosition(
		static_cast<float>(50.0f + rand() % static_cast<int>(this->window->getSize().x - 2 * 50.0f - this->enemy.getSize().x)), 0.0f
	);

	//Randomazie enemy type
	sf::Color randomColor(rand() % 256, rand() % 256, rand() % 256);
	this->enemy.setFillColor(randomColor);

	//Randomazie enemy size
	float randomSize = 50.0f + static_cast<float>(rand() % (int)(80.0f - 50.0f + 1));
	this->enemy.setSize(sf::Vector2f(randomSize, randomSize));

	//Spawn enemy
	this->enemies.push_back(this->enemy);
}
void Game::updateEnemies() {
	/*
	   @return void

	   - Updates the enemy spawn timer and spawn enemies
	   when the total amount of enemies is smaller than the minimum.
	   - Moves enemies.
	   - Removes enemies of the edge of the screen.
	*/

	//Updating the timer and spawning enemy
	if (this->enemies.size() < this->maxEnemies) {
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			//Spawn enemy and reset timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.0f;
		}
		else
			this->enemySpawnTimer += 1.0f;
	}

	//Moving and updating enemies
	for (size_t i = 0; i < this->enemies.size(); ++i)
	{

		//Move enemy on the screen
		this->enemies[i].move(0.0f, 1.5f);

		if (this->points >= 1000) 
		{ 
			this->enemies[i].move(0.0f, 2.0f); 
		}

		//If the enemy is past the bottom of the screen
		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			//Delete enemy on the screen
			this->enemies.erase(this->enemies.begin() + i);

			//Lose health when enemy reach bottom of the screen
			this->health -= 5;
		}
	}

	//Check if clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			bool deleted{ false };

			for (size_t i = 0; i < this->enemies.size() && deleted == false; ++i)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					//Gain points
					this->points += 10;

					//Delete enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else{ this->mouseHeld = false; }
}
void Game::renderEnemies(sf::RenderTarget& target) {
	for (auto& enemy : this->enemies) {
		target.draw(enemy);
	}
}

void Game::spawnHealthPack() {
	/*
	 @return void

	 - Spawing health pack.
	 - Set health pack spawn location.

	*/

	//Health pack spawn
	this->healthPack.setPosition( static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - 2* this->healthPack.getRadius())), 0.0f);

	this->healthPack.setFillColor(sf::Color(230, 16, 87));

	this->healthPacks.push_back(this->healthPack);

}
void Game::updateHealthPacks() {
	/*
	 @return void

	 - Updating health pack timer.
	 - Spawing health pack.
	
    */
	//Updating the timer and spawning health pack
	if (this->healthPacks.size() < 1) {
		if (this->healthPackSpawnTimer >= this->healthPackSpawnTimerMax && this->points > 1000) {
			//Spawn enemy and reset timer
			this->spawnHealthPack();
			this->healthPackSpawnTimer = 0.0f;
		}
		else
			this->healthPackSpawnTimer += 0.5f;
	}

	//Moving the health pack
	for (size_t i = 0; i < this->healthPacks.size(); i++)
	{
		this->healthPacks[i].move(0.0f, 2.0f);

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
		for (size_t i = 0; i < this->healthPacks.size(); i++)
		{
			if (this->healthPacks[i].getGlobalBounds().contains(this->mousePosView)) {
				//Gain health
				if (this->health < 100)
				{
					this->health += 5;
					if (this->health > 100) { this->health = 100; }

				}

				//Delete health pack when touched
				this->healthPacks.erase(this->healthPacks.begin() + i);

			}
		}
	}
}
void Game::renderHealthPacks(sf::RenderTarget& target) {
	for (auto& healthDrop : this->healthPacks) {
		target.draw(healthDrop);
	}
}

void Game::updateScore() {
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

//Game Functions
void Game::pollEvents() {
	//Event polling
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

	  Update mouse postitions:
	  - Mouse position relative to window.
	*/

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}
void Game::update() {
	//Poll the events
	this->pollEvents();

	//Update mouse position
	this->updateMousePos();

	//Update menu
	this->updateMenu();

	if (this->startGame) {

		if (this->health <= 0) {
			this->endGame = true;
		}

		if (!this->endGame)
		{
			//Update text
			this->updateText();

			//Update enemies
			this->updateEnemies();

			//Update health packs
			this->updateHealthPacks();
		}
		else
		{
			//Update the score after the game
			this->updateScore();

			//Reset game variables
			this->points = 0;
			this->health = 100;
			this->enemySpawnTimer = this->enemySpawnTimerMax;
			this->enemies.clear(); // Clear the enemies vector

			//Game start again
			this->startGame = false;
			this->endGame = false;
		}
	}
}
void Game::render() {
	/*
		@return void

		- clear old frame
		- render objects
		- display frame in window

		Renders the game objects
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
		this->renderEnemies(*this->window);

		this->renderHealthPacks(*this->window);

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
	this->points = 0; // Points player gained
	this->highestScore = 0; //Highest score recorded on the game
	this->health = 100; //Player health
	this->enemySpawnTimerMax = 60.0f; //Spawner timer max
	this->enemySpawnTimer = this->enemySpawnTimerMax; //Spawner timer 
	this->maxEnemies = 4; //Max enemies on the screen
	this->mouseHeld = false; //Is mouse being hold

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

	this->window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Cube", sf::Style::Fullscreen | sf::Style::Close);

	this->window->setFramerateLimit(500);

	//Center of the screen
	this->centerX = this->window->getSize().x / 2.0f;
	this->centerY = this->window->getSize().y / 2.0f;
}
void Game::initFonts() {
	//Check if font can be loaded
	if (!this->font.loadFromFile("Fonts/PixellettersFull.ttf")) {
		std::cout << "[-] Error::Game::InitFonts Failed to load the font\n";
	}
}
void Game::initMenu() {
	//Set highest score menu by default
	this->highestScoreMenu.setFillColor(sf::Color::White);
	this->highestScoreMenu.setSize(sf::Vector2f(200.0f, 58.0f));
	this->highestScoreMenu.setPosition(centerX - this->highestScoreMenu.getSize().x / 2.0f, 50.0f);

	//Set highest score text by default
	this->uiHighestScore.setFont(this->font);
	this->uiHighestScore.setCharacterSize(25);
	this->uiHighestScore.setPosition(this->highestScoreMenu.getPosition().x, this->highestScoreMenu.getPosition().y + 10.f);
	this->uiHighestScore.setFillColor(sf::Color::Black);
	this->uiHighestScore.setString("NONE");

	//Set play button by default
	this->playButton.setFillColor(sf::Color::White);
	this->playButton.setSize(sf::Vector2f(160.0f, 58.0f));
	this->playButton.setPosition(centerX - this->playButton.getSize().x / 2.0f, this->highestScoreMenu.getPosition().y + this->highestScoreMenu.getSize().y + 20.0f);

	//Set play button text by default
	this->uiPlayText.setFont(this->font);
	this->uiPlayText.setCharacterSize(35);
	this->uiPlayText.setPosition(this->playButton.getPosition().x + 45.f, this->playButton.getPosition().y);
	this->uiPlayText.setFillColor(sf::Color::Black);
	this->uiPlayText.setString("NONE");

	//Set quit button by default
	this->quitButton.setFillColor(sf::Color::White);
	this->quitButton.setSize(sf::Vector2f(160.0f, 58.0f));
	this->quitButton.setPosition(centerX - this->quitButton.getSize().x / 2.0f, this->playButton.getPosition().y + this->playButton.getSize().y + 20.0f);

	//Set quit button text by default
	this->uiQuitText.setFont(this->font);
	this->uiQuitText.setCharacterSize(35);
	this->uiQuitText.setPosition(this->quitButton.getPosition().x + 45.f, this->quitButton.getPosition().y);
	this->uiQuitText.setFillColor(sf::Color::Black);
	this->uiQuitText.setString("NONE");
}
void Game::initGameText() {
	//Set text by default
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(35);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");
}
void Game::initGameObjects() {
	//Default game objects values
	this->healthPack.setRadius(25.0f);
	this->enemy.setSize(sf::Vector2f(0.0f,0.0f));
}