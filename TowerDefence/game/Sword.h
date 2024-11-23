#pragma once

#include "Turret.h"

class Sword : public CModel
{
	CSprite shop;
	vector<CSprite*> buttons;
	bool isOpen;
	int damage;
	float range;
	float attackingTimer;
	float attackingTimerDuration;
	int state;
	int gold;
	int totalGold;
public:
	enum SelectedPath { BASE=0, BLACKHOLE=1, SHADOW=2, ENGINEER=3 } path;
	Sword();
	bool Attack();
	float GetRange();
	int GetDamage();
	int GetState();
	bool IsAttacking();
	virtual void Action(long GameTime);
	void Reset();
	void Upgrade(string path);
	void DrawMenu(CGraphics* g);
	bool IsOpen();
	void OpenShop();
	void CloseShop();
	bool HandleButtons(float x, float y, int &balance);
};

