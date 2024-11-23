#pragma once

#include "Enemy.h"
#include "Sword.h"

const int ARCHER_GOLD = 1000;
const int ZAP_GOLD = 1200;
const int ICE_GOLD = 900;
const int FIRE_GOLD = 1000;
const int DRAGON_GOLD = 2200;


class Turret
{
protected:
	CModel base;
	CModel* otherbase;
	CModel* turret;
	CModel* particles;
	CModelList shots;
	CModel shot;
	vector<Enemy*> enemiesInRange;
	vector<Enemy*> allEnemies;
	CSprite* shop;
	CSprite* coin;
	vector<CSprite*> buttons;
	bool isOpen = false;

	float range;
	int damage;
	float fireRate;  // 0 for a turret that doesn't shoot
	float fireRateCounter;
	int damageTick;
	float damageTickDuration;
	
	//bool showHologram;
	int state;
	int path;  // 1 for speed path and 2 for flaming path
	int gold;
	int totalGold;

public:
	static float fireRateIncrease;
	Turret(CModel* particle, CSprite* turretImg, CSprite* originalCoin);
	virtual void SetPath(int selectedPath);
	int GetState();
	bool HitTest(CModel *pModel);
	bool HitTest(CVector pos);
	virtual void GetEnemiesInRange(vector<Enemy*> enemies);
	virtual void SetPosition(float x, float z);
	void SetPositionV(CVector pos);
	CVector GetPosition();
	virtual void SetDirectionAndRotationToPoint(CVector pos);
	virtual void Update(long t , long dt);
	virtual void Draw(CGraphics* g);
	void DrawMenu(CGraphics* g);
	virtual void OpenShop(int turret=0);
	void CloseShop();
	bool IsOpen();
	// 0 for buying, 1 for selling and 2 or 3 for changing paths
	int HandleButtons(float x, float y);
	void Upgrade();
	int GetGold(int selectedTurret=-1);
	int GetTotalGold();
	virtual void Level1();
	virtual void Level2() {};
	virtual void Level3() {};
	virtual void ShotsController() {};
	virtual void Shoot() {};
};

