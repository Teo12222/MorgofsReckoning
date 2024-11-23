#pragma once


#include "Turrets.h"
#include "PathGeneration.cpp"
#include <iomanip>
#include <sstream> 


const float timeBetweenEnemySpawn = 0.15f;

class CMyGame : public CGame
{
public:
	CMyGame();
	~CMyGame();

    // ----  Declare your game variables and objects here -------------

	// Variables
	int wave = 0;
	float waveTimer;
	int timeBetweenWaves;
	vector<Enemy*> enemiesInWave;

	int gold;
	CHealthBar structHealthBar;
	CHealthBar timeBetweenWavesBar;
    // Models
	CModel skydome;

    CModelMd2 player;   // animated player model
	CModelMd2 playerside;
	CModelMd2 playerbottom;
	CModelMd2 playerfront;
	float playerSpeed;
	
	CModel rightClick;
	Sword sword;
	CModel blackHole;
	bool isSpeed;
	CModel leftClick;

	CVector enemyStartingPos;
	Enemy regularEnemy;
	Enemy fireEnemy;
	Enemy floatEnemy;
	Enemy speedEnemy;
	Enemy Listar;
	Enemy Melchor;
	Enemy Morgof;
	CHealthBar bossHealth;
	int currentBossHealth;
	vector<string> bossNames;
	bool isBoss;

	
	CModel path;
	CModelList paths;
	CModelList nature;
	vector<CModel*> trunks;
	vector<CModel*> canopies;
	vector<CModel*> grass;
	vector<CModel*> flowers;
	vector<CModel*> rocks;
	CModel blackThing;
	CModel poster;
	CModel castle;

	CSprite coin;
	CSprite* swordCoin;

	CSprite turretsImg;
	CModel particle;
	CModel archerBase;
	CModel archerTurret;
	CSprite archerImg;
	CModel zapBase;
	CModel zapTurret;
	CSprite zapImg;
	CModel iceBase;
	CModel iceTurret;
	CSprite iceImg;
	CModel fireBase;
	CModel fireTurret;
	CSprite fireImg;
	CModel dragonBase;
	CModel dragonTurret;
	CSprite dragonImg;
	vector<Turret*> turretList;
	enum SelectedTurret { ARCHER=0, ZAP=1, ICE=2, FIRE=3, DRAGON=4 } selectedTurret;
	vector<Enemy*> enemies;

	enum { MENU, PLAYING } playState;
	enum {PAUSE, END, OTHER} menuState;
	CSprite pauseMenu;
	vector<CSprite*> pauseButtons;
	CSprite endMenu;
	vector<CSprite*> endButtons;
	bool won;
	long offset;
	CSprite mainMenu;
	vector<CSprite*> mainButtons;
	CSprite cursor;
	CSprite menuCursor;

	CSoundPlayer music;
	CSoundPlayer footsteps;
	CSoundPlayer swordSwing;
	CSoundPlayer waveStart;
	CSoundPlayer morgofSound;
	CSoundPlayer buttonSound;

	// game world floor
	CFloor floor;
	
	// Font
	CFont font;
	
   // -----   Add your member functions here ------
   
   void PlayerControl(long t);
   void GenerateWorld();
   void NextWave();
   void PlaceWave(long dt);
   void CameraControl(CGraphics* g);
   
	
   // ---------------------Event Handling --------------------------

	// Game Loop Funtions
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);
	virtual void OnRender3D(CGraphics* g);

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartLevel(int level);
	virtual void OnStartGame();
	virtual void OnGameOver();
	virtual void OnTerminate();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};
