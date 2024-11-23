#include "Turrets.h"


const int LVL1_DAMAGE = 180;
const int LVL1_RANGE = 1200;
const float LVL1_FIRERATE = 0.6f;

const int ARCHER_GOLD_2 = 1200;

const int LVL2_SPEED_DAMAGE = 220;
const float LVL2_SPEED_FIRERATE = 0.4f;

const int LVL2_FLAMING_DAMAGE = 220;
const int LVL2_TICKING_DAMAGE = 20;
const float LVL2_TICKING_DURATION = 3.f;

const int ARCHER_GOLD_3 = 2000;

const int LVL3_SPEED_DAMAGE = 260;
const float LVL3_SPEED_FIRERATE = 0.2f;

const int LVL3_FLAMING_DAMAGE = 260;
const int LVL3_TICKING_DAMAGE = 30;
const float LVL3_TICKING_DURATION = 5.f;

ArcherTurret::ArcherTurret(CModel* particle, CModel* base, CModel* archTurret, CSprite* archerImg, CSprite* originalCoin) : Turret(particle, archerImg, originalCoin)
{
	otherbase = base->Clone();
	turret = archTurret->Clone();
	shot = CModel(0, 0, 0, 100, 20, 20, CColor(37, 193, 156));

	delete shop;
	shop = new CSprite(960, 540, *archerImg);
	shop->SetFrame(1);
	coin->SetPosition(880, 270);

	buttons = {};
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(960, 270);
	buttons.back()->SetSize(285, 90);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(520, 270);
	buttons.back()->SetSize(100, 90);

	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(705, 475);
	buttons.back()->SetSize(480, 285);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(1210, 475);
	buttons.back()->SetSize(480, 285);

	totalGold = ARCHER_GOLD;
	Upgrade();
}

void ArcherTurret::Level1()
{
	damage = LVL1_DAMAGE;
	range = LVL1_RANGE;
	fireRate = LVL1_FIRERATE;
	gold = ARCHER_GOLD_2;
}

void ArcherTurret::Level2()
{
	if (path == 1) {
		damage = LVL2_SPEED_DAMAGE;
		fireRate = LVL2_SPEED_FIRERATE;
		particles->SetColor(CColor(37, 193, 156));
	}
	else if (path == 2) {
		damage = LVL2_FLAMING_DAMAGE;
		damageTick = LVL2_TICKING_DAMAGE;
		damageTickDuration = LVL2_TICKING_DURATION;
		particles->SetColor(255, 0, 0);
		shot.SetColor(255, 0, 0);
	}
	buttons.pop_back();
	buttons.pop_back();
	shop->SetFrame(state + 1 + path);
	gold = ARCHER_GOLD_3;
}

void ArcherTurret::Level3()
{
	if (path == 1) {
		damage = LVL3_SPEED_DAMAGE;
		fireRate = LVL3_SPEED_FIRERATE;
	}
	else if (path == 2) {
		damage = LVL3_FLAMING_DAMAGE;
		damageTick = LVL3_TICKING_DAMAGE;
		damageTickDuration = LVL3_TICKING_DURATION;
	}
	shop->SetFrame(path == 1 ? shop->GetMaxFrames() - 1 : shop->GetMaxFrames());
}

void ArcherTurret::Shoot()
{
	if (enemiesInRange.size() != 0) {
		CModel* newShot = new CModel(shot);
		CVector turretDir = turret->GetDirectionV().Normalized() * 120;
		newShot->SetPositionV(turret->GetPositionV() + CVector(0, 250, 0) +  turretDir);
		CVector dir = (enemiesInRange[0]->GetPositionV() - newShot->GetPositionV()).Normalized();
		newShot->SetDirectionV(dir);
		newShot->SetRotationV(dir);
		newShot->SetSpeed(2000);
		newShot->SetStatus(distance(allEnemies.begin(), find(allEnemies.begin(), allEnemies.end(), enemiesInRange[0])));  //https://unstop.com/blog/find-in-vector-cpp
		shots.push_back(newShot);
	}
	else {
		fireRateCounter = fireRate;
	}
}

void ArcherTurret::Update(long t, long dt)
{
	if (state != 0 && enemiesInRange.size() != 0) {
		SetDirectionAndRotationToPoint(enemiesInRange[0]->GetPositionV());
	}
	Turret::Update(t, dt);
	ShotsController();
}

void ArcherTurret::ShotsController()
{
	bool deleted = false;
	for (CModel* shot : shots) {
		Enemy* targetEnemy = allEnemies[shot->GetStatus()];
		CVector dir = (targetEnemy->GetPositionV() - shot->GetPositionV()).Normalized();
		shot->SetDirectionV(dir);
		shot->SetRotationV(dir);
		if (shot->HitTest(targetEnemy)) {
			targetEnemy->SetTickDamage(damageTick, damageTickDuration);
			targetEnemy->TakeDamage(damage);
			shot->Delete();
			deleted = true;
		}
	}
	shots.delete_if(deleted);
}


