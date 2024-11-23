#include "Enemy.h"

 
const float RED_TIME = 0.2f;

vector<CVector> Enemy::enemyPath = {};
float Enemy::damageIncrease = 1;


Enemy::Enemy() : CModelMd3() { 
	turn = 0;
	damageCounter = -1; 
	tickDamage = {};
	tickDamageDuration = {};
	tickDamageDurationCounter = {};
	SetRotation(-90);
	SetDirection(GetRotation());
}

Enemy::Enemy(const CModelMd3& m) : CModelMd3(m)
{
	damageCounter = -1;
}


void Enemy::TakeDamage(int damage)
{
	Health -= damage*damageIncrease;
	cout << Health << endl;
	SetColor(255, 0, 0);
	damageCounter = 0;
	//cout << Health << endl;
	if (Health <= 0) {
		Delete();
		SetSpeed(0);
	}
}

void Enemy::SetTickDamage(int damageTick, float damageTickDuration)
{
	if (damageTickDuration != 0 && type != "fire") {
		tickDamage.push_back(damageTick);
		tickDamageDuration.push_back(damageTickDuration);
		tickDamageDurationCounter.push_back(0);
	}
}

void Enemy::SetWalkingSpeed(float walkingSpeed)
{
	speed = walkingSpeed;
	Speed = speed;
}

float Enemy::GetWalkingSpeed()
{
	return speed;
}

//void Enemy::SetGoldAmount(int gold)
//{
//	goldAmount = gold;
//}
//
//int Enemy::GetGoldAmount()
//{
//	return goldAmount;
//}

void Enemy::SetStart(float enemyStart)
{
	start = enemyStart;
}

float Enemy::GetStart() {
	return start;
}

void Enemy::SetType(string newType)
{
	type = newType;
}

string Enemy::GetType()
{
	return type;
}

void Enemy::IncrementTurn() {
	turn++;
}

int Enemy::GetTurn()
{
	return turn;
}

void Enemy::Action(long GameTime)
{
	/*static long t = 0;
	long dt = GameTime - t;
	t = GameTime;*/
	vector<int> removeTick = {};
	for (int i = 0; i < tickDamageDurationCounter.size(); i++) {
		tickDamageDurationCounter[i] += GameTime / 1000.f;
		if (tickDamageDurationCounter[i] >= 1) {
			tickDamageDurationCounter[i] = 0;
			tickDamageDuration[i] -= 1.f;
			TakeDamage(tickDamage[i]);
		}
		if (tickDamageDuration[i] == 0) {
			removeTick.push_back(i);
		}
	}
	for (int i : removeTick) {
		tickDamage.erase(tickDamage.begin() + i);
		tickDamageDuration.erase(tickDamageDuration.begin() + i);
		tickDamageDurationCounter.erase(tickDamageDurationCounter.begin() + i);
	}
	if (damageCounter >= 0) {
		damageCounter += GameTime / 1000.f;
	}
	if (damageCounter > RED_TIME) {
		SetColor(255, 255, 255);
		damageCounter = -1;
	}
	if (Health <= 0) {
		SetSpeed(0);
	}
}


