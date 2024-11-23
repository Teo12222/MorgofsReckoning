#pragma once


#include "Game.h"

class Enemy : public CModelMd3
{
	float speed;
	int goldAmount;
	string type;
	int turn;
	float start;
	float damageCounter;
	vector<int> tickDamage;
	vector<float> tickDamageDuration;
	vector<float> tickDamageDurationCounter;
public:
	static vector<CVector> enemyPath;
	static float damageIncrease;
	Enemy();
	Enemy(const CModelMd3& m);
	void TakeDamage(int damage);
	void SetTickDamage(int damageTick, float damageTickDuration);
	void SetWalkingSpeed(float walkingSpeed);
	float GetWalkingSpeed();
	/*void SetGoldAmount(int gold);
	int GetGoldAmount();*/
	void SetStart(float start);
	float GetStart();
	void SetType(string newType);
	string GetType();
	int GetTurn();
	void IncrementTurn();
	virtual void Action(long GameTime);
};

