#include "Turrets.h"


const int LVL1_DAMAGE = 120;
const int LVL1_RANGE = 850;
const float LVL1_FIRERATE = 1.f;
const int LVL1_TICKING_DAMAGE = 40;
const int LVL1_TICKING_DURATION = 3.f;

const int FIRE_GOLD_2 = 1400;

const int LVL2_DAMAGE = 180;
const int LVL2_TICKING_DURATION = 5.f;

const int FIRE_GOLD_3 = 2400;

const int LVL3_DAMAGE = 220;
const int LVL3_TICKING_DURATION = 800.f;


FireTurret::FireTurret(CModel* particle, CModel* base, CModel* fireTurret, CSprite* fireImg, CSprite* orignalCoin) : Turret(particle, fireImg, orignalCoin)
{
	isFieldDown = false;
	otherbase = new CModel(*base);
	turret = new CModel(*fireTurret);
	shot = CModel(0, 0, 0, 100, 30, 30, CColor(167, 7, 7));
	area.LoadModel("area/area.obj");
	CColor areaColor = CColor::Red();
	areaColor.SetAlpha(0.8);
	area.SetColor(areaColor);
	area.SetScale(4.f);
	area.SetBoxScale(0.8);

	delete shop;
	shop = new CSprite(960, 540, *fireImg);
	shop->SetFrame(1);
	coin->SetPosition(880, 270);

	buttons = {};
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(960, 270);
	buttons.back()->SetSize(285, 90);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(520, 270);
	buttons.back()->SetSize(100, 90);

	totalGold = FIRE_GOLD;
	//area.SetRotation(rand() % 360);
	Upgrade();
}

void FireTurret::Level1()
{
	damage = LVL1_DAMAGE;
	range = LVL1_RANGE;
	fireRate = LVL1_FIRERATE;
	damageTick = LVL1_TICKING_DAMAGE;
	damageTickDuration = LVL1_TICKING_DURATION;
	gold = FIRE_GOLD_2;
}

void FireTurret::Level2()
{
	damage = LVL2_DAMAGE;
	damageTickDuration = LVL2_TICKING_DURATION;
	gold = FIRE_GOLD_3;
	particles->SetColor(167, 7, 7);
	shop->SetFrame(2);
}

void FireTurret::Level3()
{
	damage = LVL3_DAMAGE;
	damageTickDuration = LVL3_TICKING_DURATION;
	area.SetScale(5.f);
	shop->SetFrame(3);
}

void FireTurret::GetEnemiesInRange(vector<Enemy*> enemies)
{
	if (!isFieldDown) {
		Turret::GetEnemiesInRange(enemies);
		vector<Enemy*> newInRange;
		for (Enemy* enemy : enemiesInRange) {
			if ((enemy->GetX() > turret->GetX() - 75 && enemy->GetX() < turret->GetX() + 75) || (enemy->GetZ() > turret->GetZ() - 75 && enemy->GetZ() < turret->GetZ() + 75)) {
				newInRange.push_back(enemy);
			}
		}
		enemiesInRange.clear();
		enemiesInRange = newInRange;
	}
	else {
		allEnemies = enemies;
		static vector<Enemy*> previousEnemies = allEnemies;
		previousEnemies.clear();
		previousEnemies.assign(enemiesInRange.begin(), enemiesInRange.end());
		enemiesInRange.clear();
		for (Enemy* enemy : enemies) {
			if (enemy->HitTest(&area) && !enemy->IsDeleted()) {
				enemiesInRange.push_back(enemy);
			}
		}
		for (Enemy* previousEnemy : previousEnemies) {
			bool present = false;
			for (Enemy* enemy : enemiesInRange) {
				if (previousEnemy == enemy) {
					present = true;
				}
			}
			if (!present) {
				previousEnemy->SetTickDamage(damageTick, damageTickDuration);
			}
		}
	}
}

void FireTurret::Shoot()
{
	if (enemiesInRange.size() != 0) {
		if (!isFieldDown) {
			CModel* newShot = new CModel(shot);
			CVector turretDir = turret->GetDirectionV().Normalized() * 130;
			newShot->SetPositionV(turret->GetPositionV() + CVector(0, 240, 0) + turretDir);
			CVector dir = (enemiesInRange[0]->GetPositionV() - CVector(0, enemiesInRange[0]->GetY(), 0) - newShot->GetPositionV()).Normalized();
			newShot->SetDirectionV(dir);
			newShot->SetRotationV(dir);
			newShot->SetSpeed(5000);
			shots.push_back(newShot);
		}
		else {
			for (Enemy* enemy : enemiesInRange) {
				if (enemy->GetType() == "fire")
					enemy->TakeDamage(damage * 0.6);
				else
					enemy->TakeDamage(damage);
			}
		}
	}
	else {
		fireRateCounter = fireRate;
	}
}


void FireTurret::Update(long t, long dt)
{
	if (state != 0 && enemiesInRange.size() != 0 && !isFieldDown) {
		SetDirectionAndRotationToPoint(enemiesInRange[0]->GetPositionV());
	}
	ShotsController();
	Turret::Update(t, dt);
	area.Update(t);
}

void FireTurret::Draw(CGraphics* g)
{
	Turret::Draw(g);
	if (isFieldDown) {
		area.Draw(g);
	}
}


void FireTurret::ShotsController()
{
	if (!isFieldDown) {
		for (CModel* cShot : shots) {
			if (cShot->GetY() < 0) {
				isFieldDown = true;
				area.SetPositionV(cShot->GetPositionV());
				area.SetToFloor(0);
				CVector dir = turret->GetDirectionV().Normalized();
				float dist = base.GetPositionV().Distance(area.GetPositionV());
				if (dist < 700) {
					area.SetPositionV(area.GetPositionV() + dir * (700 - dist));
				}
				else if (dist > 700) {
					area.SetPositionV(area.GetPositionV() + dir * (700 - dist));
				}
				break;
			}
		}
		if (isFieldDown) {
			enemiesInRange.clear();
			shots.clear();
		}
	}

}