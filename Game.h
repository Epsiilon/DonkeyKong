#pragma once

#define ECHELLE_COUNT 20
#define TONNEAU_COUNT 20
#define ECHELLE_MONKEY_COUNT 18

#define BLOCK_COUNT_X 14
#define BLOCK_COUNT_Y 6

#define MARTEAU_COUNT 3

#define BLOCK_SOUS_PRINCESSE 3

#define SPRITE_PLAYER 10
#define SPRITE_MONKEY 9
#define SPRITE_PRINCESSE 4


class Game
{
public:
	Game();
	~Game() { };
	void run();

private:
	void processEvents();
	void update(sf::Time elapsedTime);
	void render();

	void updateStatistics(sf::Time elapsedTime);
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

private:
	static const float		PlayerSpeed;
	static const float		GravityForce;
	static const float		VitesseTonneauxBase;
	static const sf::Time	TimePerFrame;
	float lastSolVal;
	float VitesseTonneaux;

	sf::RenderWindow		mWindow;
	sf::Font	mFont;
	sf::Text	mStatisticsText;
	sf::Text	scoreText;
	sf::Text	levelText;
	sf::Time	mStatisticsUpdateTime;

	std::size_t	mStatisticsNumFrames;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;
	bool mJump;
	bool isFrappe;
	int lastSensPente = 0;
	int gameLevel = 0;
	int gameTimeCompteLevel = 0;
	bool tickLevel = false;

	int scorePlayer = 0;

	int tickMarteau = 0;
	int tickTonneau = 0;

	int marteauFlotteVal = 0;
	bool marteauFlotteSens = false;

	int jumpCompte = 0;

	int spritePlayerCompte = 0;
	int spriteLastPosMarioX = 0;
	int spriteLastPosMarioY = 0;
	int marteauTimeRestant = 0;
	bool marteauTape = false;
	bool sensPlayer = false;

	int gameState = 0;
	int gameTimeCompteur = 0;
	int gameTimeCompteurTwo = 0;
	int gameLoopCompteur = 0;

	int spriteMonkeyCompte = 0;
	int spriteLastPosMonkeyX = 0;
	int spriteLastPosMonkeyY = 0;
	bool enerveMonkey = false;

	int spritePrincesseCompte = 0;
	int spritePrincesseIncr = 0;

	bool demandeNewTonneau = false;
	int demandeNewTonneauState = 0;
	int compteAReboursNewTonneau = 0;

	sf::Vector2f posMario;
	sf::Texture	_TexturePlayer[SPRITE_PLAYER];
	sf::Sprite	_Player[SPRITE_PLAYER];
	sf::Vector2u _sizeMario;

	sf::Vector2f posMonkey;
	sf::Texture	_TextureMonkey[SPRITE_MONKEY];
	sf::Sprite	_Monkey[SPRITE_MONKEY];
	sf::Vector2u _sizeMonkey;

	sf::Texture	_TexturePrincesse[SPRITE_PRINCESSE];
	sf::Sprite	_Princesse[SPRITE_PRINCESSE];
	sf::Vector2f posPrincesse;

	sf::Texture	_TextureEchelle;
	sf::Sprite	_Echelle[ECHELLE_COUNT];

	sf::Texture	_TextureEchelleMonkey;
	sf::Sprite	_EchelleMonkey[ECHELLE_MONKEY_COUNT];

	sf::Texture	_TextureBlock;
	sf::Sprite	_Block[BLOCK_COUNT_X][BLOCK_COUNT_Y];
	sf::Vector2u _sizeBlock;
	
	sf::Texture	_TextureBlockSousPrincesse;
	sf::Sprite	_BlockSousPrincesse[BLOCK_SOUS_PRINCESSE];

	sf::Texture	_TextureTonneau;
	sf::Sprite	_Tonneau[TONNEAU_COUNT];

	sf::Texture	_TextureMarteau;
	sf::Sprite	_Marteau[MARTEAU_COUNT];
	
	sf::Texture	_TextureTonneauStock;
	sf::Sprite	_TonneauStock[4];

};

