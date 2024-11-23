#pragma once

#include "Turret.h"

class ArcherTurret : public Turret
{
	
public:
	ArcherTurret(CModel* particle, CModel* base, CModel* turret, CSprite* archerImg, CSprite* originalCoin);
	virtual void Level1();
	virtual void Level2();
	virtual void Level3();
	virtual void Shoot();
	virtual void Update(long t, long dt);
	virtual void ShotsController();
};

class ZapTurret : public Turret
{
	float stunDuration;
	float stunCounter;
	int numberOfEnemies;
public:
	ZapTurret(CModel* particle, CModel* base, CModel* turret, CSprite* zapImg, CSprite* originalCoin);
	virtual void SetPath(int selectedPath);
	virtual void Level1();
	virtual void Level2();
	virtual void Level3();
	virtual void Shoot();
	virtual void Update(long t, long dt);
	virtual void ShotsController();
};

class IceTurret : public Turret
{
	float slowAmount;
	bool isFieldDown;
	CModel area;
public:
	IceTurret(CModel* particle, CModel* base, CModel* turret, CSprite* iceImg, CSprite* originalCoin);
	virtual void Level1();
	virtual void Level2();
	virtual void Level3();
	virtual void GetEnemiesInRange(vector<Enemy*> enemies);
	virtual void Shoot();
	virtual void Update(long t, long dt);
	virtual void Draw(CGraphics* g);
	virtual void ShotsController();
};

class FireTurret : public Turret
{
	bool isFieldDown;
	CModel area;
public:
	FireTurret(CModel* particle, CModel* base, CModel* turret, CSprite* fireImg, CSprite* originalCoin);
	virtual void Level1();
	virtual void Level2();
	virtual void Level3();
	virtual void GetEnemiesInRange(vector<Enemy*> enemies);
	virtual void Shoot();
	virtual void Update(long t, long dt);
	virtual void Draw(CGraphics* g);
	virtual void ShotsController();
};

class DragonTurret : public Turret
{

public:
	DragonTurret(CModel* particle, CModel* base, CModel* turret, CSprite* dragImg, CSprite* originalCoin);
	virtual void Level1();
	virtual void Level2();
	virtual void Level3();
	virtual void Shoot();
	virtual void Update(long t, long dt);
	virtual void ShotsController();
};
