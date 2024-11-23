#include "Sword.h"

const int BASE_DAMAGE = 300;
const float BASE_RANGE = 600.f;
const float BASE_ATTACKSPEED = 0.3f;

const float BLACKHOLE_LVL1_ATTACKSPEED = 0.2f;
const int BLACKHOLE_LVL1_DAMAGE = 500;
const float BLACKHOLE_LVL1_RANGE = 800.f;

const int BLACKHOLE_LVL2_DAMAGE = 900;

const int SHADOW_LVL1_DAMAGE = 400;
const float SHADOW_LVL1_ATTACKSPEED = 0.15f;

const int SHADOW_LVL2_DAMAGE = 700;
const float SHADOW_LVL2_ATTACKSPEED = 0.1f;
const float SHADOW_LVL2_RANGE = 700.f;

const int LVL1_COST = 2500;
const int LVL2_COST = 5000;

Sword::Sword() : CModel()
{
	isOpen = false;
	attackingTimer = -1;
}

void Sword::Reset() {
	shop.LoadImage("sword.png", CColor::Black(), 10);
	shop.SetPosition(960, 540);
	shop.SetFrame(1);

	buttons.clear();
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(425, 775);
	buttons.back()->SetSize(190, 110);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(425, 615);
	buttons.back()->SetSize(190, 110);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(425, 455);
	buttons.back()->SetSize(190, 110);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(425, 295);
	buttons.back()->SetSize(190, 110);

	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(1020, 265);
	buttons.back()->SetSize(280, 90);

	path = BASE;
	state = 1;
	attackingTimerDuration = BASE_ATTACKSPEED;
	range = BASE_RANGE;
	damage = BASE_DAMAGE;
	gold = LVL1_COST;
	totalGold = 0;
	LoadModel("baseSword/baseSword.obj");
	SetScale(1.2f);
	Turret::fireRateIncrease = 0;
	Enemy::damageIncrease = 1;
}

bool Sword::Attack()
{
	if (attackingTimer == -1) {
		attackingTimer = 0;
		return true;
	}
	return false;
}


float Sword::GetRange()
{
	return range;
}

int Sword::GetDamage()
{
	return damage;
}

int Sword::GetState()
{
	return state;
}

bool Sword::IsAttacking()
{
	if (attackingTimer == -1) {
		return false;
	}
	return true;
}

void Sword::Action(long GameTime)
{
	if (attackingTimer != -1)
		attackingTimer += GameTime/1000.f;
	if (attackingTimer > attackingTimerDuration) {
		attackingTimer = -1;
	}
}

void Sword::Upgrade(string selectedPath="")
{
	if (state < 3) {
		totalGold += gold;
	}
	if (selectedPath == "blackhole" || path == BLACKHOLE) {
		path = BLACKHOLE;
		if (state == 1) {
			LoadModel("blackHoleSword/1/blackHole1.obj");
			SetScale(1.f);
			attackingTimer = BLACKHOLE_LVL1_ATTACKSPEED;
			damage = BLACKHOLE_LVL1_DAMAGE;
			range = BLACKHOLE_LVL1_RANGE;
			gold = LVL2_COST;
		}
		else if (state == 2) {
			LoadModel("blackHoleSword/2/blackHole2.obj");
			SetScale(1.f);
			damage = BLACKHOLE_LVL2_DAMAGE;
		}
		state++;
	}
	else if (selectedPath == "shadow" || path == SHADOW) {
		path = SHADOW;
		if (state == 1) {
			LoadModel("shadowSword/1/shadowSword.obj");
			//SetScale(1.f);
			damage = SHADOW_LVL1_DAMAGE;
			attackingTimer = SHADOW_LVL1_ATTACKSPEED;
			gold = LVL2_COST;
		} 
		else if (state == 2) {
			LoadModel("shadowSword/2/shadowSword.obj");
			//SetScale(1.f);
			damage = SHADOW_LVL2_DAMAGE;
			attackingTimer = SHADOW_LVL2_ATTACKSPEED;
			range = SHADOW_LVL2_RANGE;
		}
		state++;
	}
	else if (selectedPath == "engineer" || path == ENGINEER) {
		path = ENGINEER;
		if (state == 1) {
			LoadModel("engineerSword/1/engineerSword.obj");
			SetScale(1.4f);
			Turret::fireRateIncrease = 0.05;
			gold = LVL2_COST;
		}
		else if (state == 2) {
			LoadModel("engineerSword/2/engineerSword.obj");
			SetScale(1.2f);
			Turret::fireRateIncrease = 0.07;
			damage /= 1.05;
			Enemy::damageIncrease = 1.05;
		}
		state++;
	}
	if (buttons.size() > 2) {
		buttons.clear();
		buttons.push_back(new CSprite());
		buttons.back()->SetPosition(960, 270);
		buttons.back()->SetSize(285, 90);
		buttons.push_back(new CSprite());
		buttons.back()->SetPosition(520, 270);
		buttons.back()->SetSize(100, 90);
	}
}

void Sword::DrawMenu(CGraphics* g)
{
	if (isOpen) {
		shop.Draw(g);
	}
}

bool Sword::IsOpen()
{
	return isOpen;
}

void Sword::OpenShop()
{
	isOpen = true;
}

void Sword::CloseShop()
{
	isOpen = false;
}

bool Sword::HandleButtons(float x, float y, int& balance)
{
	if (isOpen) {
		int goldToAdd = 0;
		for (int i = 0; i < buttons.size(); i++) {
			if (buttons[i]->HitTest(x, y)) {
				if (state == 1) {
					if (i < 4) {
						shop.SetFrame(1 + i);
						path = static_cast<SelectedPath>(i);
					}
					if (i == 4 && path != BASE && balance >= gold) {
						balance -= gold;
						Upgrade();
						CloseShop();
						shop.SetFrame(4 + path);
					}
				}
				else {
					if (i == 0 && balance >= gold && state < 3) {
						balance -= gold;
						Upgrade();
						CloseShop();
						int addFromLevel = state == 2 ? 4 : 7;
						shop.SetFrame(addFromLevel + path);
					}
					if (i == 1) {
						balance += totalGold * 0.6;
						Reset();
						CloseShop();
					}
				}
				return true;
			}
		}
	}
	return false;
}
