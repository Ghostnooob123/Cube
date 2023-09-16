#include "Enemy.h"

//Constructors & Destructors
Enemy::Enemy() {
	this->initVariables();
	this->initGameObjects();
}
Enemy::~Enemy() {

}

//Accessors
void Enemy::reset() {
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->mouseHeld = false;
	this->enemies.clear();
}

//Spawn enemies
void Enemy::spawnEnemy(sf::RenderTarget& target) {
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
		static_cast<float>(50.0f + rand() % static_cast<int>(target.getSize().x - 2 * 50.0f - this->enemy.getSize().x)), 0.0f
	);

	//Randomazie enemy type
	sf::Color randomColor(rand() % 256, rand() % 256, rand() % 256);
	this->enemy.setFillColor(randomColor);

	//Randomazie enemy size
	float randomSize = 60.0f + static_cast<float>(rand() % (int)(80.0f - 60.0f + 1));
	this->enemy.setSize(sf::Vector2f(randomSize, randomSize));

	//Spawn enemy
	this->enemies.push_back(this->enemy);
}
void Enemy::updateEnemies(sf::RenderTarget& target, unsigned& points, int& playerHealth, sf::Vector2f& mousePosView, int& bossPoints) {
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
			this->spawnEnemy(target);
			this->enemySpawnTimer = 0.0f;
		}
		else
			this->enemySpawnTimer += 1.0f;
	}

	//Moving and updating enemies
	for (size_t i = 0; i < this->enemies.size(); ++i)
	{

		//Move enemy on the screen
		if (points >= 10000)
		{
			this->enemies[i].move(0.0f, 1.7f);
		}
		else
		{
			this->enemies[i].move(0.0f, 1.3f);
		}

		//If the enemy is past the bottom of the screen
		if (this->enemies[i].getPosition().y > target.getSize().y)
		{
			//Delete enemy on the screen
			this->enemies.erase(this->enemies.begin() + i);

			//Lose health when enemy reach bottom of the screen
			playerHealth -= 10;
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
				if (this->enemies[i].getGlobalBounds().contains(mousePosView))
				{
					//Gain points
					points += 20;
					//Gain points for the boss
					bossPoints += 20;

					//Delete enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else { this->mouseHeld = false; }
}
void Enemy::renderEnemies(sf::RenderTarget& target) {
	//Render enemies on the screen
	for (auto& enemy : this->enemies) {
		target.draw(enemy);
	}
}

//Private functions
void Enemy::initVariables() {
	this->maxEnemies = 8;
	this->enemySpawnTimerMax = 65.0f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->mouseHeld = false;
}
void Enemy::initGameObjects() {
	this->enemies;
	this->enemy;
}
