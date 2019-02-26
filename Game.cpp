#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"
#include <ctime>

//const float Game::PlayerSpeed = 150.f;
const float Game::PlayerSpeed = 300.f;
const float Game::GravityForce = 300.f;
const float Game::VitesseTonneauxBase = 300.f; // Doit être supérieur à GravityForce
int lifePlayer = 10;


int tonneauPlaces[2] = { 300, 90 };
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: mWindow(sf::VideoMode(1000, 900), "Donkey Kong 1981 Refacture", sf::Style::Close)
	, _TexturePlayer()
	, _Player()
	, mFont()
	, mStatisticsText()
	, scoreText()
	, levelText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, mIsMovingUp(false)
	, mIsMovingDown(false)
	, mIsMovingRight(false)
	, mIsMovingLeft(false)
	, mJump(false)
	, isFrappe(false)
{

	mWindow.setFramerateLimit(160);

	// Draw blocks

	_TextureBlock.loadFromFile("Media/Textures/Block.png");
	_sizeBlock = _TextureBlock.getSize();

	// Create Blocks
	for (int i = 0; i < BLOCK_COUNT_Y; i++) { // Iter lignes
		for (int j = 0; j < BLOCK_COUNT_X; j++) { // Pour chaque lignes on créé les colonnes

			_Block[i][j].setTexture(_TextureBlock);

			if (i == BLOCK_COUNT_Y - 1) {
				_Block[i][j].setPosition(10.f + (70.f * j), 75.f + (155.f * i));
			}
			else {
				if ((i % 2) == 0) {
					if (j < BLOCK_COUNT_X - 1)
						_Block[i][j].setPosition(10.f + (70.f * j), 100.f + (150.f * i) + (3.f * j));
				}
				else {
					if (j > 0)
						_Block[i][j].setPosition(10.f + (70.f * j), 130.f + (150.f * i) - (3.f * j));
				}
			}


			std::shared_ptr<Entity> se = std::make_shared<Entity>();
			se->m_sprite = _Block[i][j];
			se->m_type = EntityType::block;
			se->m_size = _TextureBlock.getSize();
			se->m_position = _Block[i][j].getPosition();
			EntityManager::m_Entities.push_back(se);
		}
	}

	// Create Block sous princesse
	_TextureBlockSousPrincesse.loadFromFile("Media/Textures/Block.png");

	for (int i = 0; i < BLOCK_SOUS_PRINCESSE; i++) {
		_BlockSousPrincesse[i].setTexture(_TextureBlockSousPrincesse);
		
		_BlockSousPrincesse[i].setPosition(750 + (35 * i), 90);

		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _BlockSousPrincesse[i];
		se->m_sprite.setScale(0.5, 0.5);
		se->m_type = EntityType::tonneauStock;
		se->m_size = _TextureBlockSousPrincesse.getSize();
		se->m_sprite.setOrigin(se->m_size.x / 2, se->m_size.y / 2); // x milieu et y milieu
		se->m_enabled = true;
		se->m_position = _BlockSousPrincesse[i].getPosition();
		EntityManager::m_Entities.push_back(se);
	}


	// Create Echelles Monkey
	_TextureEchelle.loadFromFile("Media/Textures/Echelle.png");

	for (int i = 0; i < ECHELLE_MONKEY_COUNT; i++) {

		_EchelleMonkey[i].setTexture(_TextureEchelle);

		if (i < 9) {
			_EchelleMonkey[i].setPosition(265, 140 + (92 * i));
		} else {
			_EchelleMonkey[i].setPosition(305, 140 + (92 * (i-9)));
		}

		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _EchelleMonkey[i];
		se->m_type = EntityType::echelleMonkey;
		se->m_sprite.setScale(1.5, 1.5);
		se->m_size = _TextureEchelleMonkey.getSize();
		se->m_position = _EchelleMonkey[i].getPosition();
		EntityManager::m_Entities.push_back(se);
	}


	// Create Echelles
	int echellesPlaces[14][2] = { // Gauche-Droite/Haut-Bas
								{ 770, 95  },
								{ 870, 768 },
								{ 870, 788 },
								{ 100, 609 },
								{ 100, 639 },
								{ 240, 502 },
								{ 240, 472 },
								{ 660, 459 },
								{ 660, 490 },
								{ 100, 310 },
								{ 100, 341 },
								{ 870, 168 },
								{ 870, 178 },
								{ 240, 442 } };

	for (int i = 0; i < ECHELLE_COUNT; i++) {

		_Echelle[i].setTexture(_TextureEchelle);

		_Echelle[i].setPosition((float)echellesPlaces[i][0], (float)echellesPlaces[i][1]);

		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _Echelle[i];
		se->m_type = EntityType::echelle;
		se->m_size = _TextureEchelle.getSize();
		se->m_position = _Echelle[i].getPosition();
		EntityManager::m_Entities.push_back(se);
	}


	// Create Tonneau Stock
	_TextureTonneauStock.loadFromFile("Media/Textures/Texture.png", sf::IntRect(62, 109, 10, 16	));

	int tonneauPlacesDepot[4][2] = { { 240, 88 }, { 210, 85 }, { 240, 48 }, { 210, 45 } };

	for (int i = 0; i < 4; i++) {
		_TonneauStock[i].setTexture(_TextureTonneauStock);
		_TonneauStock[i].setPosition((float)tonneauPlacesDepot[i][0], (float)tonneauPlacesDepot[i][1]);

		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _TonneauStock[i];
		se->m_sprite.setScale(2.5, 2.5);
		se->m_type = EntityType::tonneauStock;
		se->m_size = _TextureTonneauStock.getSize();
		se->m_sprite.setOrigin(se->m_size.x / 2, se->m_size.y / 2); // x milieu et y milieu
		se->m_enabled = true;
		se->m_position = _TonneauStock[i].getPosition();
		EntityManager::m_Entities.push_back(se);
	}

	// Create Marteaux
	_TextureMarteau.loadFromFile("Media/Textures/Texture.png", sf::IntRect(104, 33, 9, 9));

	int marteauxPos[3][2] = { { 185, 245 }, { 325, 680 }, { 810, 520 }};

	for (int i = 0; i < MARTEAU_COUNT; i++) {
		_Marteau[i].setTexture(_TextureMarteau);
		_Marteau[i].setPosition((float)marteauxPos[i][0], (float)marteauxPos[i][1]);

		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _Marteau[i];
		se->m_sprite.setScale(4, 4);
		se->m_type = EntityType::marteau;
		se->m_size = _TextureMarteau.getSize();
		se->m_sprite.setOrigin(se->m_size.x / 2, se->m_size.y / 2); // x milieu et y milieu
		se->m_enabled = true;
		se->m_position = _Marteau[i].getPosition();
		EntityManager::m_Entities.push_back(se);
	}
	

	// Create Tonneau
	_TextureTonneau.loadFromFile("Media/Textures/Texture.png", sf::IntRect(1, 109, 14, 14));

	for (int i = 0; i < TONNEAU_COUNT; i++) {

		_Tonneau[i].setTexture(_TextureTonneau);
		_Tonneau[i].setPosition((float)tonneauPlaces[0], (float)tonneauPlaces[1]);
		
		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _Tonneau[i];

		se->m_sprite.setScale(2.5, 2.5);
		se->spriteNumber = i;

		se->m_type = EntityType::tonneau;
		se->m_size = _TextureTonneau.getSize();
		se->m_sprite.setOrigin(se->m_size.x / 2, se->m_size.y / 2); // x milieu et y milieu
		
		se->m_enabled = false;
		se->activateTonneau = false;

		se->m_position = _Tonneau[i].getPosition();
		EntityManager::m_Entities.push_back(se);
	}


	// Create Mario
	for (int textureIncr = 0; textureIncr < SPRITE_PLAYER; textureIncr++) {

		// Textures Droite / Gauche
		if (textureIncr == 0) {
			_TexturePlayer[0].loadFromFile("Media/Textures/Texture.png", sf::IntRect(2, 6, 12, 16));
			_Player[0].setTexture(_TexturePlayer[0]);
		}
		else if (textureIncr == 1) {
			_TexturePlayer[1].loadFromFile("Media/Textures/Texture.png", sf::IntRect(17, 6, 18, 14));
			_Player[1].setTexture(_TexturePlayer[1]);
		}
		else if (textureIncr == 2) {
			_TexturePlayer[2].loadFromFile("Media/Textures/Texture.png", sf::IntRect(35, 7, 15, 15));
			_Player[2].setTexture(_TexturePlayer[2]);
		}
		else if (textureIncr == 3) {
			_TexturePlayer[3].loadFromFile("Media/Textures/Texture.png", sf::IntRect(53, 7, 16, 15));
			_Player[3].setTexture(_TexturePlayer[3]);
		}
		// Textures Haut / Bas
		else if (textureIncr == 4) {
			_TexturePlayer[4].loadFromFile("Media/Textures/Texture.png", sf::IntRect(71, 30, 16, 15));
			_Player[4].setTexture(_TexturePlayer[4]);
		}
		else if (textureIncr == 5) {
			_TexturePlayer[5].loadFromFile("Media/Textures/Texture.png", sf::IntRect(2, 30, 13, 15));
			_Player[5].setTexture(_TexturePlayer[5]);
		}
		else if (textureIncr == 6) {
			_TexturePlayer[6].loadFromFile("Media/Textures/Texture.png", sf::IntRect(35, 31, 14, 15));
			_Player[6].setTexture(_TexturePlayer[6]);
		}
		else if (textureIncr == 7) {
			_TexturePlayer[7].loadFromFile("Media/Textures/Texture.png", sf::IntRect(19, 31, 13, 15));
			_Player[7].setTexture(_TexturePlayer[7]);
		}
		// Marteau haut
		else if (textureIncr == 8) {
			_TexturePlayer[8].loadFromFile("Media/Textures/Texture.png", sf::IntRect(103, 2, 12, 26));
			_Player[8].setTexture(_TexturePlayer[8]);
		}
		// Marteau bas
		else if (textureIncr == 9) {
			_TexturePlayer[9].loadFromFile("Media/Textures/Texture.png", sf::IntRect(75, 12, 25, 16));
			_Player[9].setTexture(_TexturePlayer[9]);
		}

		//posMario.x = 50.f;
		//posMario.y = 800.f;
		posMario.x = 200.f;
		posMario.y = 470.f;

		lastSolVal = posMario.y;
		_Player[textureIncr].setPosition(posMario);

		std::shared_ptr<Entity> player = std::make_shared<Entity>();
		player->m_sprite = _Player[textureIncr];
		player->m_type = EntityType::player;
		player->m_size = _TexturePlayer[textureIncr].getSize();
		player->m_sprite.setScale(4, 4);
		player->spriteNumber = textureIncr;
		_sizeMario = player->m_size;
		
		if (textureIncr < 9) {
			player->m_sprite.setOrigin(player->m_size.x / 2, player->m_size.y); // x milieu et y bas
		} else {
			player->m_sprite.setOrigin((player->m_size.x / 2) + 9, player->m_size.y); // Adapté marteau bas
		}
		
		player->m_position = _Player[textureIncr].getPosition();
		EntityManager::m_Entities.push_back(player);

	}


	// Create Monkey
	for (int textureIncr = 0; textureIncr < SPRITE_MONKEY; textureIncr++) {

		// Textures monte echelle début jeu
		if (textureIncr == 0) {
			_TextureMonkey[0].loadFromFile("Media/Textures/Texture.png", sf::IntRect(156, 299, 43, 32));
			_Monkey[0].setTexture(_TextureMonkey[0]);
		}
		else if (textureIncr == 1) {
			_TextureMonkey[1].loadFromFile("Media/Textures/Texture.png", sf::IntRect(160, 334, 38, 36));
			_Monkey[1].setTexture(_TextureMonkey[1]);
		}
		// Textures se déplace jusqu'aux toneaux
		else if (textureIncr == 2) {
			_TextureMonkey[2].loadFromFile("Media/Textures/Texture.png", sf::IntRect(113, 299, 38, 32));
			_Monkey[2].setTexture(_TextureMonkey[2]);
		}
		// Textures bouche ouverte
		else if (textureIncr == 3) {
			_TextureMonkey[3].loadFromFile("Media/Textures/Texture.png", sf::IntRect(112, 264, 38, 32));
			_Monkey[3].setTexture(_TextureMonkey[3]);
		}
		// Textures énervé bouche ouverte mains en l'air pieds à terre
		else if (textureIncr == 4) {
			_TextureMonkey[4].loadFromFile("Media/Textures/Texture.png", sf::IntRect(58, 337, 47, 32));
			_Monkey[4].setTexture(_TextureMonkey[4]);
		}
		// Textures énervé bouche ouverte mains en l'air pieds gauche à terre
		else if (textureIncr == 5) {
			_TextureMonkey[5].loadFromFile("Media/Textures/Texture.png", sf::IntRect(110, 335, 44, 28));
			_Monkey[5].setTexture(_TextureMonkey[5]);
		}
		// Textures énervé bouche ouverte mains en l'air pieds droit à terre
		else if (textureIncr == 6) {
			_TextureMonkey[6].loadFromFile("Media/Textures/Texture.png", sf::IntRect(156, 264, 46, 32));
			_Monkey[6].setTexture(_TextureMonkey[6]);
		}
		
		// Textures prend tonneau
		else if (textureIncr == 7) {
			_TextureMonkey[7].loadFromFile("Media/Textures/Texture.png", sf::IntRect(156, 229, 43, 32));
			_Monkey[7].setTexture(_TextureMonkey[7]);
		}
		// Textures porte tonneau
		else if (textureIncr == 8) {
			_TextureMonkey[8].loadFromFile("Media/Textures/Texture.png", sf::IntRect(112, 229, 40, 32));
			_Monkey[8].setTexture(_TextureMonkey[8]);
		}
		
		
		posMonkey.x = 280.f;
		posMonkey.y = 800.f;

		spriteLastPosMonkeyX = posMonkey.x;
		spriteLastPosMonkeyY = posMonkey.y;

		_Monkey[textureIncr].setPosition(posMonkey);

		std::shared_ptr<Entity> monkey = std::make_shared<Entity>();

		if (textureIncr != 0) {
			monkey->m_enabled = false;
		}

		monkey->m_sprite = _Monkey[textureIncr];
		monkey->m_type = EntityType::monkey;
		monkey->m_size = _TexturePlayer[textureIncr].getSize();
		monkey->m_sprite.setScale(3, 3);
		monkey->spriteNumber = textureIncr;
		_sizeMonkey = monkey->m_size;
		monkey->m_sprite.setOrigin(monkey->m_size.x / 2, monkey->m_size.y); // x milieu et y bas
		monkey->m_position = _Monkey[textureIncr].getPosition();
		EntityManager::m_Entities.push_back(monkey);

	}

	// Create Princesse
	for (int textureIncr = 0; textureIncr < SPRITE_PRINCESSE; textureIncr++) {

		// Textures monte echelle début jeu
		if (textureIncr == 0) {
			_TexturePrincesse[0].loadFromFile("Media/Textures/Texture.png", sf::IntRect(2, 243, 16, 22));
			_Princesse[0].setTexture(_TexturePrincesse[0]);
		}
		else if (textureIncr == 1) {
			_TexturePrincesse[1].loadFromFile("Media/Textures/Texture.png", sf::IntRect(22, 243, 15, 22));
			_Princesse[1].setTexture(_TexturePrincesse[1]);
		}
		// Textures se déplace jusqu'aux toneaux
		else if (textureIncr == 2) {
			_TexturePrincesse[2].loadFromFile("Media/Textures/Texture.png", sf::IntRect(39, 243, 15, 22));
			_Princesse[2].setTexture(_TexturePrincesse[2]);
		}
		// Textures bouche ouverte
		else if (textureIncr == 3) {
			_TexturePrincesse[3].loadFromFile("Media/Textures/Texture.png", sf::IntRect(57, 243, 14, 23));
			_Princesse[3].setTexture(_TexturePrincesse[3]);
		}

		posPrincesse.x = 350.f;
		posPrincesse.y = 85.f;

		_Princesse[textureIncr].setPosition(posPrincesse);

		std::shared_ptr<Entity> princesse = std::make_shared<Entity>();
		princesse->m_enabled = false;
		princesse->m_sprite = _Princesse[textureIncr];
		princesse->m_type = EntityType::princesse;
		princesse->m_size = _TexturePlayer[textureIncr].getSize();
		princesse->spriteNumber = textureIncr;
		_sizeMonkey = princesse->m_size;
		princesse->m_sprite.setOrigin(princesse->m_size.x / 2, princesse->m_size.y); // x milieu et y bas
		princesse->m_sprite.setScale(3, 3);
		princesse->m_position = _Monkey[textureIncr].getPosition();
		EntityManager::m_Entities.push_back(princesse);
	}


	// FPS Compteur text
	mFont.loadFromFile("Media/Sansation.ttf");
	mStatisticsText.setString("");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(15);

	// Score text
	mFont.loadFromFile("Media/Sansation.ttf");
	scoreText.setString("");
	scoreText.setFont(mFont);
	scoreText.setPosition(10.f, 20.f);
	scoreText.setCharacterSize(30);

	// New level text
	mFont.loadFromFile("Media/Sansation.ttf");
	levelText.setString("");
	levelText.setFont(mFont);
	levelText.setPosition(400.f, 350.f);
	levelText.setCharacterSize(60);


}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}

		updateStatistics(elapsedTime);
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::update(sf::Time elapsedTime) {

	// A mettre en globale
	bool playerCanMove[4] = { 0, 0, 1, 1 }; // Haut / Bas / Droite / Gauche

	// Gravity all items
	int findGround = 0;
	int findGroundPlayer = 0;
	for (std::shared_ptr<Entity> entityObj : EntityManager::m_Entities) {

		// Gravity Player
		if ((entityObj->m_enabled == true) and (entityObj->m_type == EntityType::player)) {
			sf::Vector2f playerPos = { posMario.x, posMario.y + 2 };
			for (std::shared_ptr<Entity> entitySol : EntityManager::m_Entities) {
				if ((entitySol->m_enabled == true) and (entitySol->m_type == EntityType::block)) {
					sf::FloatRect boxSol = entitySol->m_sprite.getGlobalBounds();
					if (boxSol.contains(playerPos)) {
						findGroundPlayer++;
						//printf("Find Ground");
					}
				}
			}
		}

		// Gravity Tonneau
		if ((entityObj->activateTonneau == true) & (entityObj->m_enabled == true) & (entityObj->m_type == EntityType::tonneau)) {
			findGround = 0;
			float groundValY = 0;
			sf::FloatRect boxObj = entityObj->m_sprite.getGlobalBounds();

			for (std::shared_ptr<Entity> entitySol : EntityManager::m_Entities) {
				if ((entitySol->m_enabled == true) and (entitySol->m_type == EntityType::block)) {
					sf::FloatRect boxSol = entitySol->m_sprite.getGlobalBounds();
					if (boxObj.intersects(boxSol)) {
						findGround++;
						groundValY = entitySol->m_position.y;
					}
				}
			}
			if (findGround == 0) {
				//printf("\nGravity Tonneau");
				entityObj->compteGravity++;
				entityObj->m_position.y += (GravityForce * elapsedTime.asSeconds());

				if (entityObj->sensMove == 1) { // Fait tourner dans le vide
					//printf("\nPente vers Droite");
					entityObj->m_sprite.rotate(5.f);
				}
				else if (entityObj->sensMove == 2) {
					//printf("\nPente vers Gauche");
					entityObj->m_sprite.rotate(-5.f);
				}
			}
			else { // Tonneau roule
				// Cherche le sens de la pente
				//printf("\nRoule");
				if (entityObj->compteGravity > 3) { // Regarde si tombe d'un étage
					//printf("\nStage");
					if (entityObj->m_position.y > 800) {
						//printf("\nStop bas");
						entityObj->sensMove = 0;
					}
					else if (entityObj->sensMove == 1) {
						//printf("\nGo Left");
						entityObj->sensMove = 2;
					}
					else if (entityObj->sensMove == 2) {
						//printf("\nGo Right");
						entityObj->sensMove = 1;
					}
				}
				entityObj->compteGravity = 0;
				if (entityObj->sensMove == 1) { // Fait rouler
					//printf("\nPente vers Droite");
					entityObj->m_position.x += (VitesseTonneaux * elapsedTime.asSeconds());
					entityObj->m_sprite.rotate(8.f);
				}
				else if (entityObj->sensMove == 2) {
					//printf("\nPente vers Gauche");
					entityObj->m_position.x -= (VitesseTonneaux * elapsedTime.asSeconds());
					entityObj->m_sprite.rotate(-8.f);
				}
			}
		}
	} // End gravity all items

	// Cherche Echelle
	int findEchelle = 0;
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities) {
		if ((entity->m_enabled == true) and (entity->m_type == EntityType::echelle)) {
			sf::FloatRect boxEchelle = entity->m_sprite.getGlobalBounds();
			sf::Vector2f playerPosDessus = { posMario.x, posMario.y }; // Dessus
			if (boxEchelle.contains(playerPosDessus)) {
				//printf("\nEchelle sous pied");
				playerCanMove[0] = 1; // Haut
				playerCanMove[1] = 1; // Bas
				findEchelle = 1;
			}
			sf::Vector2f playerPosUp = { posMario.x, posMario.y + 35.f }; // Dessus
			if (boxEchelle.contains(playerPosUp)) {
				//printf("\nEchelle sous sol");
				playerCanMove[0] = 1; // Haut
				playerCanMove[1] = 1; // Bas
				findEchelle = 2;
			}
		}
	} // End cherche Echelle

	// Empeche Mario d'aller dans Sol
	if (findEchelle != 2) {
		for (std::shared_ptr<Entity> entitySol : EntityManager::m_Entities) {
			if ((entitySol->m_enabled == true) and (entitySol->m_type == EntityType::block)) {
				sf::Vector2f playerPos = { posMario.x, posMario.y };
				sf::FloatRect boxSol = entitySol->m_sprite.getGlobalBounds();
				if (boxSol.contains(playerPos)) { // On est sur le bon ground
					if ((posMario.y - 3) > entitySol->m_position.y) { // Si on est sous la surface
						//printf("\nCorrige Sol");
						posMario.y -= (GravityForce * elapsedTime.asSeconds());
					}
				}
			}
		}
	}

	// Gravity Player
	if ((findEchelle == 0) & (findGroundPlayer == 0)) {
		//printf("\nGravity Player");
		posMario.y += (GravityForce * elapsedTime.asSeconds());
	}

	// Bloque Murs
	// Mur gauche
	if (posMario.x < 20) {
		playerCanMove[2] = 0;
		//printf("\nMur Gauche");
	}
	// Mur droit
	if (posMario.x > 960) {
		playerCanMove[3] = 0;
		//printf("\nMur Droit");
	}
	// End bloque Murs

	// Bloque player tant que la cinematique n'est pas terminée
	if ((gameState < 2) | (gameState == 4)){ // Cinématique
		//printf("\nBloque mode jeu : %d", gameState);
		playerCanMove[0] = 0; // Haut
		playerCanMove[1] = 0; // Bas
		playerCanMove[2] = 0; // Gauche
		playerCanMove[3] = 0; // Droite
	}

	//printf("\nMove %d %d %d %d", playerCanMove[0], playerCanMove[1], playerCanMove[2], playerCanMove[3]);

	// Detect appuye touches
	if (mIsMovingUp & playerCanMove[0]) { // Haut
		posMario.y -= (PlayerSpeed * elapsedTime.asSeconds());
	}
	if (mIsMovingDown & playerCanMove[1]) { // Bas
		posMario.y += (PlayerSpeed * elapsedTime.asSeconds());
	}
	if (mIsMovingLeft & playerCanMove[2]) { // Gauche
		posMario.x -= (PlayerSpeed * elapsedTime.asSeconds());
	}
	if (mIsMovingRight & playerCanMove[3]) { // Droite
		posMario.x += (PlayerSpeed * elapsedTime.asSeconds());
	}
	if (isFrappe) { // Frappe
		marteauTape = true;
	}
	else {
		marteauTape = false;
	}
	if ((mJump) & (jumpCompte == 0)) { // Jump
		if (findGroundPlayer > 0) { // On est bien sur le sol
			jumpCompte = 10;
		}
	}
	// End detect appuye touches

	// Jump
	if (jumpCompte > 0) {
		jumpCompte--;
		posMario.y -= ((GravityForce * 2) * elapsedTime.asSeconds());
	}

	// Change Sprite Player
	if (!mIsMovingUp & !mIsMovingDown & !mIsMovingLeft & !mIsMovingRight) { // Remet à zero la position à chaque arrêt
		if ((spritePlayerCompte < 4) | (spritePlayerCompte > 7)) {
			if (marteauTimeRestant == 0) {
				spritePlayerCompte = 0;
			} else {
				if (marteauTape == false) {
					spritePlayerCompte = 8; // Haut
				} else {
					spritePlayerCompte = 9; // Bas
				}
			}
		}
		else {
			spritePlayerCompte = 4;
		}
		spriteLastPosMarioX = posMario.x;
		spriteLastPosMarioY = posMario.y;
		//printf("Arret");
	}
	if (spriteLastPosMarioX < posMario.x) { // Player avance vers la droite
		int compte = (int)posMario.x - (int)spriteLastPosMarioX;
		if (compte > 20) {
			spriteLastPosMarioX = posMario.x;
			//printf("\n PAS");
			if (spritePlayerCompte < 3) {
				spritePlayerCompte++;
			}
			else {
				spritePlayerCompte = 0;
			}
			//printf("\n %d", spritePlayerCompte);
		}
	}
	else if (spriteLastPosMarioX > posMario.x) { // Player avance vers la gauche
		int compte = (int)spriteLastPosMarioX - (int)posMario.x;
		if (compte > 20) {
			spriteLastPosMarioX = posMario.x;
			//printf("\n PAS");
			if (spritePlayerCompte < 3) {
				spritePlayerCompte++;
			}
			else {
				spritePlayerCompte = 0;
			}
			//printf("\n %d", spritePlayerCompte);
		}
	}
	if (spriteLastPosMarioY < posMario.y) { // Player avance vers le haut
		int compte = (int)posMario.y - (int)spriteLastPosMarioY;
		if (compte > 20) {
			spriteLastPosMarioY = posMario.y;
			//printf("\n PAS");
			if (spritePlayerCompte < 4) {
				spritePlayerCompte = 4;
			}
			else
				if (spritePlayerCompte < 7) {
					spritePlayerCompte++;
				}
				else {
					spritePlayerCompte = 4;
				}
			//printf("\n %d", spritePlayerCompte);
		}
	}
	else if (spriteLastPosMarioY > posMario.y) { // Player avance vers le bas
		int compte = (int)spriteLastPosMarioY - (int)posMario.y;
		if (compte > 20) {
			spriteLastPosMarioY = posMario.y;
			//printf("\n PAS");
			if (spritePlayerCompte < 4) {
				spritePlayerCompte = 4;
			}
			else {
				if (spritePlayerCompte < 7) {
					spritePlayerCompte++;
				}
				else {
					spritePlayerCompte = 4;
				}
			}
			//printf("\n %d", spritePlayerCompte);
		}
	} // End player avance
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities) { // Active seulement le bon sprite parmis les 4
		if (entity->m_type == EntityType::player) {
			//printf("\n%d", entity->spriteNumber);
			if (entity->spriteNumber == spritePlayerCompte) {
				entity->m_enabled = true;
			}
			else {
				entity->m_enabled = false;
			}
		}
	}
	// End change Sprite Player

	// Remet à zero les tonneaux arrivés en bas pour qu'ils puissent être réutilisés
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities) {
		if ((entity->m_enabled == true) & (entity->activateTonneau == true) & (entity->m_type == EntityType::tonneau)) {
			if ((entity->m_position.y > 830) | (entity->m_position.x > 975) | (entity->m_position.x < 5 )) { // Quand ils sortent de la zone d'action (en bas et sur les cotés de cas de bug
				//printf("\nTonneau Reset");
				entity->m_position = _Tonneau[entity->spriteNumber].getPosition();
				entity->sensMove = 1;
				entity->compteGravity = 0;
				entity->m_enabled = false;
				entity->activateTonneau = false;
				if (demandeNewTonneauState == 0) {
					enerveMonkey = true;
				}
			}
		}
	}

	// Marteau Flotte
	marteauFlotteVal++;
	if (marteauFlotteVal > 100) {
		marteauFlotteVal = 0;
		marteauFlotteSens = !marteauFlotteSens;
		//printf("Change sens");
	}
	for (std::shared_ptr<Entity> entityMarteau : EntityManager::m_Entities) {
		if ((entityMarteau->m_enabled == true) & (entityMarteau->m_type == EntityType::marteau)) {
			if (marteauFlotteSens == true) {
				//printf("Change haut");
				entityMarteau->m_position.y -= (10 * elapsedTime.asSeconds());
			}
			else {
				//printf("Change bas");
				entityMarteau->m_position.y += (10 * elapsedTime.asSeconds());
			}
		}
	} // End Marteau Flotte

	// Horloge pour les tonneaux
	if (gameState == 3) { // Seulement pendant la phase de jeu
		// Tick tous les n temps
		if (gameTimeCompteurTwo < 50) {
			gameTimeCompteurTwo++;
			if (tickMarteau > 0) tickMarteau--;
			if (tickTonneau > 0) tickTonneau--;
			if (marteauTimeRestant > 0) marteauTimeRestant--;
			
			if (scorePlayer > 0) {
				if (marteauTimeRestant == 0) {
					scoreText.setString("Lifes : " + toString(lifePlayer) + "\nScore : " + toString(scorePlayer));
				} else {
					int marteauTime = (int)(marteauTimeRestant / 100);
					scoreText.setString("Lifes : " + toString(lifePlayer) + "\nScore : " + toString(scorePlayer) + "\nMarteau : " + toString(marteauTime));
				}
			}

		}
		else {
			//printf("\n Tick");
			gameTimeCompteurTwo = 0;

			scorePlayer++;
			// Prévoit le prochain tonneau
			if (compteAReboursNewTonneau == 0) {
				// Demande un nouveau tonneau seulement si il y en a de disponible
				for (std::shared_ptr<Entity> entityTonneau : EntityManager::m_Entities) {
					if ((entityTonneau->activateTonneau == false) & (entityTonneau->m_enabled == false) & (entityTonneau->m_type == EntityType::tonneau)) {
						//printf("\n Trouve free tonneau");
						demandeNewTonneau = true;
					}
				}
				//printf("\n New tonneau programme");
			}

			if (compteAReboursNewTonneau == 0) {
				std::srand(std::time(nullptr));
				if (gameLevel == 1) {
					compteAReboursNewTonneau = rand() % 10 + 1;
					compteAReboursNewTonneau = rand() % 2 + 1;
				}
				else if (gameLevel == 2) {
					compteAReboursNewTonneau = rand() % 7 + 1;
				}
				else if (gameLevel == 3) {
					compteAReboursNewTonneau = rand() % 3 + 1;
				}
				else if (gameLevel == 4) {
					compteAReboursNewTonneau = 10000;
				}
			}
			else {
				compteAReboursNewTonneau--;
			}
			//printf("\n Rebours : %d", compteAReboursNewTonneau);
		}
	}

	if ((gameState == 3) & (lifePlayer == 0)) {
		gameState = 4; // Player ne peut plus bouger
		levelText.setPosition(180.f, 320.f);
		levelText.setString("***** !! YOU LOOSE !! ***** \n\n           Score : " + toString(scorePlayer));
	}
	
	// Change Sprite Monkey
	if (((posMonkey.y < 65) & (gameState == 0)) | (enerveMonkey == true)) {
		enerveMonkey = false;
		gameState = 2;
		spriteMonkeyCompte = 2;
	}
	if ((gameLoopCompteur > 2) & (gameState == 2)) {
		gameState = 3;
		gameLoopCompteur = 0;
	}

	if (gameState == 0) { // Monkey monte l'echelle
		//printf("\n State 0");
		posMonkey.y -= (PlayerSpeed * elapsedTime.asSeconds());

		// Supprime les échelles sous monkey pendant la cinématique
		for (std::shared_ptr<Entity> entity : EntityManager::m_Entities) {
			if (entity->m_type == EntityType::echelleMonkey) {
				if (entity->m_position.y > ((float)posMonkey.y+30)) {
					//printf("\n Desactive echelle Monkey");
					entity->m_enabled = false;
				}
			}
		}

		if (((int)spriteLastPosMonkeyY - (int)posMonkey.y) > 50) {
			spriteLastPosMonkeyY = posMonkey.y;
			if (spriteMonkeyCompte == 1) {
				spriteMonkeyCompte = 0;
			}
			else if (spriteMonkeyCompte == 0) {
				spriteMonkeyCompte = 1;
			}
			//printf("\n %d", spriteMonkeyCompte);
		}
	}
	if (gameState == 1) { // Monkey se place à coté des tonneaux
		//printf("\n State 1");
		// Creer saut de block en block en tenant compte de la côte
		posMonkey.x -= (PlayerSpeed * elapsedTime.asSeconds());
	}
	if (gameState == 2) { // Monkey animation enervé
		//printf("\n State 2");
		// Textures 3 à 6
		if (posPrincesse.x < 630) {
			posPrincesse.x += (PlayerSpeed * elapsedTime.asSeconds());
		}

		gameTimeCompteur ++;
		//printf("%d", gameTimeCompteur);

		if (gameTimeCompteur > 10) {
			//printf("\n PAS");
			gameTimeCompteur = 0;
			if (spriteMonkeyCompte < 3) {
				spriteMonkeyCompte = 3;
			} else {
				if (spriteMonkeyCompte < 6) {
					spriteMonkeyCompte++;
				}
				else {
					spriteMonkeyCompte = 3;
					gameLoopCompteur++;
				}
			}
			//printf("\n %d", spriteMonkeyCompte);
		}
	}
	// Etat principale
	if (gameState == 3) { // Monkey lance des tonneaux
		//printf("\n State 3 : Game");
		if (demandeNewTonneau == true) {
			gameTimeCompteur++;

			if (demandeNewTonneauState == 0) {
				//printf("\n 1");
				demandeNewTonneauState = 1;
				spriteMonkeyCompte = 7;
				gameTimeCompteur = 0;
			}
			else if (demandeNewTonneauState == 1) {
				if (gameTimeCompteur > 30) {
					//printf("\n 2");
					gameTimeCompteur = 0;
					demandeNewTonneauState = 2;
					spriteMonkeyCompte = 2;
					// Affiche un tonneau dans la main mais ne l'active pas
					for (std::shared_ptr<Entity> entityTonneau : EntityManager::m_Entities) {
						if ((entityTonneau->activateTonneau == false) & (entityTonneau->m_enabled == false) & (entityTonneau->m_type == EntityType::tonneau)) {
							//printf("\n Create tonneau");
							entityTonneau->m_enabled = true;
							entityTonneau->m_sprite.setPosition(posMonkey.x, posMonkey.y - 20);
							break;
						}
					}
				}
			}
			else if (demandeNewTonneauState == 2) {
				if (gameTimeCompteur > 30) {
					//printf("\n 3");
					gameTimeCompteur = 0;
					demandeNewTonneauState = 3;
					spriteMonkeyCompte = 7;
					for (std::shared_ptr<Entity> entity : EntityManager::m_Entities) {
						if (entity->m_type == EntityType::monkey) {
							if (entity->spriteNumber == 7) {
								entity->m_sprite.setScale(-3.f, 3.f);
								break;
							}
						}
					}
				}
			}
			else if (demandeNewTonneauState == 3) {
				if (gameTimeCompteur > 30) {
					gameTimeCompteur = 0;
					demandeNewTonneau = false;
					for (std::shared_ptr<Entity> entityTonneau : EntityManager::m_Entities) {
						if ((entityTonneau->activateTonneau == false) & (entityTonneau->m_enabled == true) & (entityTonneau->m_type == EntityType::tonneau)) {
							//printf("\t Drop tonneau");
							entityTonneau->m_sprite.setPosition(posMonkey.x + _sizeMonkey.x/2, posMonkey.y - 20);
							entityTonneau->activateTonneau = true;
							break;
						}
					}
					spriteMonkeyCompte = 2;
					for (std::shared_ptr<Entity> entity : EntityManager::m_Entities) { // Remet monkey dans sa position initial
						if (entity->m_type == EntityType::monkey) {
							if (entity->spriteNumber == 7) {
								entity->m_sprite.setScale(3.f, 3.f);
								break;
							}
						}
					}
					demandeNewTonneauState = 0;
				}
			}
		}
	}

	//printf("\n %d", spriteMonkeyCompte);

	// Active seulement le bon sprite de Monkey
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities) {
		if (entity->m_type == EntityType::monkey) {
			if (entity->spriteNumber == spriteMonkeyCompte) {
				//printf("\n Sprite : %d", entity->spriteNumber);
				entity->m_enabled = true;
			}
			else {
				entity->m_enabled = false;
			}
		}
	} // End change Sprite Player
	// End change Sprite Monkey

	// Amène Princesse sur sa base
	if (gameState == 2) { // Marche jusqu'à sous la base
		if (posPrincesse.x < 780) {
			posPrincesse.x += (PlayerSpeed/2 * elapsedTime.asSeconds());

			// Gravity princesse
			int findGroundPrincesse = 0;
			for (std::shared_ptr<Entity> entity : EntityManager::m_Entities) {
				if ((entity->m_enabled == true) and (entity->m_type == EntityType::princesse)) {
					sf::Vector2f princessePos = { posPrincesse.x, posPrincesse.y + 25 };
					for (std::shared_ptr<Entity> entitySol : EntityManager::m_Entities) {
						if ((entitySol->m_enabled == true) and (entitySol->m_type == EntityType::block)) {
							sf::FloatRect boxSol = entitySol->m_sprite.getGlobalBounds();
							if (boxSol.contains(princessePos)) {
								findGroundPrincesse++;
								//printf("Find Ground");
							}
						}
					}
				}
			}
			if (findGroundPrincesse == 0) {
				//printf("\nGravity Princesse");
				posPrincesse.y += (GravityForce * elapsedTime.asSeconds());
			}

			// Change Sprite Princesse
			if (spritePrincesseIncr < 10) {
				spritePrincesseIncr++;
			}
			else {
				spritePrincesseIncr = 0;
			}

			// Change sprite quand marche
			if (spritePrincesseIncr == 0) {
				if (spritePrincesseCompte < 3) {
					spritePrincesseCompte++;
				}
				else {
					spritePrincesseCompte = 0;
				}
				//printf("\n %d", spritePrincesseCompte);
			}
		}else if (posPrincesse.y > 68) { // Monte sur la base
			posPrincesse.y -= (GravityForce * elapsedTime.asSeconds());
		}
	}
	else if (gameState == 3) {
		// Fait danser Princesse
		if (spritePrincesseIncr < 10) {
			spritePrincesseIncr++;
		}
		else {
			spritePrincesseIncr = 0;
		}

		if (spritePrincesseIncr == 0) {
			for (std::shared_ptr<Entity> entity : EntityManager::m_Entities) {
				if ((entity->m_enabled == true) and (entity->m_type == EntityType::princesse)) {
					if (spritePrincesseCompte < 2){
						entity->m_sprite.setScale(3.f, 3.f);
					} else {
						entity->m_sprite.setScale(-3.f, 3.f);
					}
				}
			}

			if (spritePrincesseCompte < 3) {
				spritePrincesseCompte++;
			}
			else {
				spritePrincesseCompte = 0;
			}
			//printf("\n %d", spritePrincesseCompte);
		}
	}

	// Active seulement le bon sprite de Princesse
	if (gameState > 1) {
		for (std::shared_ptr<Entity> entity : EntityManager::m_Entities) {
			if (entity->m_type == EntityType::princesse) {
				if (entity->spriteNumber == spritePrincesseCompte) {
					//printf("\n Sprite : %d", entity->spriteNumber);
					entity->m_enabled = true;
				}
				else {
					entity->m_enabled = false;
				}
			}
		} // End change Sprite Princesse
	}

	// Met à jour l'affichage
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities) {
		// Move Player
		if ((entity->m_enabled == true) and (entity->m_type == EntityType::player)) {
			if (mIsMovingRight) {
				sensPlayer = true;
			}
			if (mIsMovingLeft) {
				sensPlayer = false;
			}
			entity->m_sprite.setPosition(posMario);

			if (sensPlayer == true) {
				entity->m_sprite.setScale(-4.f, 4.f);
			} else {
				entity->m_sprite.setScale(4.f, 4.f);
			}
		}
		
		// Move Monkey
		if ((entity->m_enabled == true) and (entity->m_type == EntityType::monkey)) {
			entity->m_sprite.setPosition(posMonkey);
		}

		// Move Princesse
		if ((entity->m_enabled == true) and (entity->m_type == EntityType::princesse)) {
			entity->m_sprite.setPosition(posPrincesse);
		}
		
		// Move Tonneaux
		if ((entity->m_enabled == true) and (entity->m_type == EntityType::tonneau)) {
			entity->m_sprite.setPosition(entity->m_position);
		}
		
		// Move Marteaux
		if ((entity->m_enabled == true) and (entity->m_type == EntityType::marteau)) {
			entity->m_sprite.setPosition(entity->m_position);
		}

	}
	playerCanMove[0] = 0; // Haut
	playerCanMove[1] = 0; // Bas

	// Gestion levels
	// Horloge level
	if (gameTimeCompteLevel > 100) {
		//printf("\n Tick Level");
		gameTimeCompteLevel = 0;
		tickLevel = true;
	}

	if (gameLevel == 0) {
		//printf("Level 1");
		if (scorePlayer > 0) {
			//levelText.setString("Level 1");
			VitesseTonneaux = VitesseTonneauxBase;
			if (tickLevel == true) {
				gameLevel = 1;
				levelText.setString(" ");
			}
			else {
				gameTimeCompteLevel++;
			}
		}
	}
	if (gameLevel == 1) {
		//printf("Level 2");
		if (scorePlayer > 100) {
			//levelText.setString("Level 2");
			VitesseTonneaux = VitesseTonneauxBase * 1.5;
			if (tickLevel == true) {
				gameLevel = 2;
				levelText.setString(" ");
			}
			else {
				gameTimeCompteLevel++;
			}
		}
	}
	if (gameLevel == 2) {
		//printf("Level 3");
		if (scorePlayer > 200) {
			//levelText.setString("Level 3");
			VitesseTonneaux = VitesseTonneauxBase * 2;
			if (tickLevel == true) {
				gameLevel = 3;
				levelText.setString(" ");
			}
			else {
				gameTimeCompteLevel++;
			}
		}
	}
	tickLevel = false;
	
}

void Game::render()
{
	mWindow.clear();

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		mWindow.draw(entity->m_sprite);
	}

	mWindow.draw(mStatisticsText);
	mWindow.draw(scoreText);
	mWindow.draw(levelText);
	mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(0.5f))
	{
		mStatisticsText.setString(toString(mStatisticsNumFrames*2) + " FPS");
		
		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;

	}

	// Collide all objects

	// Collide Marteau / Tonneau
	if (marteauTape == true) {
		sf::Vector2f marteauPos;
		if (sensPlayer == true) { marteauPos = { posMario.x + 20, posMario.y - 10 }; }
		if (sensPlayer == false)  { marteauPos = { posMario.x - 20, posMario.y - 10 }; }

		for (std::shared_ptr<Entity> entityTonneau : EntityManager::m_Entities) {
			if ((entityTonneau->m_enabled == true) and (entityTonneau->m_type == EntityType::tonneau)) {

				sf::FloatRect boxTonneau = entityTonneau->m_sprite.getGlobalBounds();

				if (boxTonneau.contains(marteauPos)) { // Collision
					//printf("\n Collision Marteau");
					scorePlayer += 10;
					entityTonneau->m_position = _Tonneau[entityTonneau->spriteNumber].getPosition();
					entityTonneau->m_enabled = false;
					entityTonneau->activateTonneau = false;
				}
			}
		}
	}

	// Collide Player / Tonneau
	for (std::shared_ptr<Entity> entityTonneau : EntityManager::m_Entities) {
		if ((entityTonneau->m_enabled == true) and (entityTonneau->m_type == EntityType::tonneau)) {

			sf::Vector2f playerPos = { posMario.x, posMario.y - 20 };
			sf::FloatRect boxTonneau = entityTonneau->m_sprite.getGlobalBounds();

			if ((boxTonneau.contains(playerPos)) & (tickTonneau == 0)) { // Collision
				//printf("\n Collision, life : %d", lifePlayer);
				tickTonneau = 20;
				lifePlayer--;
			}
		}
	}
	
	// Collide Player / Marteau
	//if (marteauTimeRestant > 0) printf("\n Marteau time : %d", marteauTimeRestant);
	for (std::shared_ptr<Entity> entityTonneau : EntityManager::m_Entities) {
		if ((entityTonneau->m_enabled == true) and (entityTonneau->m_type == EntityType::marteau)) {

			sf::Vector2f playerPos = { posMario.x, posMario.y - 20 };
			sf::FloatRect boxMarteau = entityTonneau->m_sprite.getGlobalBounds();

			if ((boxMarteau.contains(playerPos)) & (tickMarteau == 0)) {
				//printf("\n Marteau win");
				tickMarteau = 50;
				marteauTimeRestant = 2100;
			}
		}
	}

	// Collide Player / Princesse : Win
	sf::FloatRect boxPlayer;
	sf::FloatRect boxPrincesse;
	for (std::shared_ptr<Entity> entityTonneau : EntityManager::m_Entities) {
		if ((entityTonneau->m_enabled == true) and (entityTonneau->m_type == EntityType::player)) {
			boxPlayer = entityTonneau->m_sprite.getGlobalBounds();
		} else if ((entityTonneau->m_enabled == true) and (entityTonneau->m_type == EntityType::princesse)) {
			boxPrincesse = entityTonneau->m_sprite.getGlobalBounds();
		}
			
		if (boxPlayer.intersects(boxPrincesse)) { // Collision Win
			levelText.setPosition(190.f, 320.f);
			levelText.setString("***** !! YOU WIN !! ***** \n\n           Score : " + toString(scorePlayer));
			gameState = 4; // Player ne peut plus bouger
		} 
		
	}

	if (mStatisticsUpdateTime >= sf::seconds(0.050f))
	{
		// Handle collision weapon enemies
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::Up)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::Down)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::Left)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::Right)
		mIsMovingRight = isPressed;

	if (key == sf::Keyboard::Space) { // Jump
		mJump = isPressed;
	}
	if (key == sf::Keyboard::E) { // Marteau
		isFrappe = isPressed;
	}
}
