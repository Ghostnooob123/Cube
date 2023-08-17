#include "CubeBoss.h"

//Constructors & Destructors
CubeBoss::CubeBoss() {
	this->initVariables();
	this->initObjects();
}
CubeBoss::~CubeBoss() {

}

//Accessors
const int CubeBoss::getHealth() const {
	/*
       @return const int
	   - Return boss health.
	*/
	return this->bossHealth;
}
void CubeBoss::setHealth() {
	/*
	   @return void
	   - set (new) boss health.
	*/
	this->bossHealth = 200;
}
void CubeBoss::reset() {
	/*
	   @return void
	   - Reset cube boss values.
	   - Clear cube boss attacks.
	*/
	this->maxBosses = 1;
	this->bossAttackTimer = this->bossAttackTimerMax;
	this->moveRight = false;

	this->cubeAttacks.clear();
}

//Spawn Cube boss enemy
void CubeBoss::spawnCubeBoss(float& centerX, float& centerY) {
	/*
	   @return void
	   - Set cube boss size.
	   - Set cube boss position.
	   - Set cube boss color.
	*/
	//Spawn Cube boss 
	this->cubeBossEnemy.setSize(sf::Vector2f(350.0f, 300.0f));
	float cubeBossX = centerX - 200.0f; // Adjust this value based on the size of your cubeBossEnemy
	float cubeBossY = centerY - 450.0f; // Adjust this value based on the size of your cubeBossEnemy
	this->cubeBossEnemy.setPosition(cubeBossX, cubeBossY);
	this->cubeBossEnemy.setFillColor(sf::Color(28, 28, 26));
	this->cubeBossEnemy.setOutlineColor(sf::Color(232, 63, 16));
	this->cubeBossEnemy.setOutlineThickness(5.0f);
}
void CubeBoss::updateCubeBoss(float& centerX, float& centerY, sf::Vector2f& mousePosView, sf::RenderTarget& target, int& playerHealth) {
	/*
	   @return void
	   - Update cube boss and his attacks.
	   - Spawn cube boss.
	   - Spawn cube boss attacks.
	   - Move cube boss attacks.
	*/
	//Spawning Cube boss
	if (this->maxBosses == 1)
	{
		this->maxBosses = 0;
		this->spawnCubeBoss(centerX, centerY);
	}

	//Spawn Cube boss attack
	if (this->cubeAttacks.size() < maxAttacks) {
		if (this->bossAttackTimer >= this->bossAttackTimerMax)
		{
			this->spawnCubeBossAttack(centerX, centerY);
			this->bossAttackTimer = 0.0f;
		}
		else
		{
			this->bossAttackTimer += 1.0f;

		}
	}
	// Calculate the new position
	sf::Vector2f newPosition = this->cubeBossEnemy.getPosition() + sf::Vector2f(0.0f, 1.0f);

	// Check if the new left position is within the left boundary
	if (newPosition.x >= this->LeftBoarder.getPosition().x + this->LeftBoarder.getSize().x && !this->moveRight) {
		// Move the cubeBossEnemy to the left
		this->cubeBossEnemy.move(-0.5f, 0.0f);
		this->cubeBoss_Summon_Attack.move(0.0f, 0.7f);
	}
	else {
		this->moveRight = true;
	}
	// Check if the new right position is within the right boundary
	if (newPosition.x + this->cubeBossEnemy.getSize().x <= this->RightBoarder.getPosition().x && this->moveRight) 
	{
		// Move the cubeBossEnemy to the right
		this->cubeBossEnemy.move(0.5f, 0.0f);
		this->cubeBoss_Summon_Attack.move(0.0f, 1.0f);
	}
	else 
	{
		this->moveRight = false;
	}

	//Move Cube boss attack
	for (size_t i = 0; i < this->cubeAttacks.size(); ++i) {
		this->cubeAttacks[i].move(0.0f, 0.8f);

		//If the enemy is past the bottom of the screen
		if (this->cubeAttacks[i].getPosition().y > target.getSize().y)
		{
			//Delete enemy on the screen
			this->cubeAttacks.erase(this->cubeAttacks.begin() + i);

			//Lose health when enemy reach bottom of the screen
			playerHealth -= 10;
		}
	}

	//Check if clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//Check if mouse is hold
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			bool deleted{ false };

			for (size_t i = 0; i < this->cubeAttacks.size() && deleted == false; ++i)
			{
				if (this->cubeAttacks[i].getGlobalBounds().contains(mousePosView))
				{
					//Delete Cube boss attack
					deleted = true;
					this->cubeAttacks.erase(this->cubeAttacks.begin() + i);
				}
			}

			if (this->cubeBossEnemy.getGlobalBounds().contains(mousePosView)) 
			{
				//Cube boss losing health upon click
				this->bossHealth -= 5;
			}
		}
	}
	else { this->mouseHeld = false; }
}
void CubeBoss::renderCubeBoss(sf::RenderTarget& target) {
	//Draw cube boss
	target.draw(this->cubeBossEnemy);
	//Draw cube boss attacks
	for (auto& cubeAttack : cubeAttacks) {
		target.draw(cubeAttack);
	}
}

//Spawn Cube boss attack
void CubeBoss::spawnCubeBossAttack(float& centerX, float& centerY) {
	/*
	   @return void
	   - Set cube boss attacks size.
	   - Set cube boss attacks position.
	   - Set cube boss attacks color.
	   - Spawn the attack.
	*/
	// Spawn Cube boss summon attack
	this->cubeBoss_Summon_Attack.setSize(sf::Vector2f(50.0f, 50.0f));

	float cubeBossX = centerX - 200.0f; // Adjust this value based on the size of your cubeBossEnemy
	float cubeBossY = centerY - 440.0f; // Adjust this value based on the size of your cubeBossEnemy

	// Calculate a random position under the cubeBossEnemy
	float summonAttackX = cubeBossX + (this->cubeBossEnemy.getSize().x - this->cubeBoss_Summon_Attack.getSize().x) / 2.0f;
	float summonAttackY = cubeBossY + this->cubeBossEnemy.getSize().y; // Adjust this value as needed

	// Generate random offsets for X and Y positions within a certain range
	float offsetX = static_cast<float>(rand() % 100 - 100); // Random value between -200 and 200
	float offsetY = static_cast<float>(rand() % 100); // Random value between 0 and 100

	// Apply the random offsets to the summonAttackX and summonAttackY
	summonAttackX += offsetX;
	summonAttackY += offsetY;

	this->cubeBoss_Summon_Attack.setPosition(summonAttackX, summonAttackY);
	this->cubeBoss_Summon_Attack.setFillColor(sf::Color(28, 28, 26));
	this->cubeBoss_Summon_Attack.setOutlineColor(sf::Color(232, 63, 16));
	this->cubeBoss_Summon_Attack.setOutlineThickness(5.0f);

	this->cubeAttacks.push_back(cubeBoss_Summon_Attack);
}

//Spawn Cube boss boarders
void CubeBoss::spawnBoarders(float& centerX, float& centerY) {
	/*
	   @return void
	   - Set boarders size.
	   - set boarders positions.
	*/
	float cubeBossX = centerX; // Adjust this value based on the size of your cubeBossEnemy
	float cubeBossY = centerY; // Adjust this value based on the size of your cubeBossEnemy

	//Top boarder
	this->TopBoarder.setSize(sf::Vector2f(300.0f, 60.0f));
	this->TopBoarder.setPosition(sf::Vector2f(centerX - 170.0f, centerY - 500.0f));

	//Bottom boarder
	this->BottomBoarder.setSize(sf::Vector2f(300.0f, 60.0f));
	this->BottomBoarder.setPosition(sf::Vector2f(centerX - 170.0f, centerY + 10.0f));

	//Left boarder
	this->LeftBoarder.setSize(sf::Vector2f(60.0f, 300.0f));
	this->LeftBoarder.setPosition(sf::Vector2f(centerX - 350.0f, centerY - 350.0f));

	//Right boarder
	this->RightBoarder.setSize(sf::Vector2f(60.0f, 300.0f));
	this->RightBoarder.setPosition(sf::Vector2f(centerX + 250.0f, centerY - 350.0f));
}
void CubeBoss::updateBoarders(float& centerX, float& centerY) {
	//Spawn all way boarders
	spawnBoarders(centerX, centerY);
}
void CubeBoss::renderBoarders(sf::RenderTarget& target) {
	//Draw all way boarders
	target.draw(this->TopBoarder);
	target.draw(this->BottomBoarder);
	target.draw(this->LeftBoarder);
	target.draw(this->RightBoarder);
}

//Private functions
void CubeBoss::initVariables() {
	//Game logic
	this->bossHealth = 200; //Cube boss health
	this->maxBosses = 1; //Max amount of bosses to spawn
	this->maxAttacks = 6; //Max amount of bosses to spawn
	this->mouseHeld = false; //Is mouse being hold

	this->bossAttackTimerMax = 100.0f;
	this->bossAttackTimer = this->bossAttackTimerMax;

	this->moveRight = false;
}
void CubeBoss::initObjects() {
	this->cubeBossEnemy.setSize(sf::Vector2f(0.0f, 0.0f));
	this->TopBoarder.setFillColor(sf::Color(255, 255, 255, 0)); //sf::Color(255, 255, 255, 0)
	this->BottomBoarder.setFillColor(sf::Color(255, 255, 255, 0));
	this->LeftBoarder.setFillColor(sf::Color(255, 255, 255, 0));
	this->RightBoarder.setFillColor(sf::Color(255, 255, 255, 0));
}
