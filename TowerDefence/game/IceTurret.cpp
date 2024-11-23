#include "Turrets.h"


const int LVL1_DAMAGE = 80.f;
const int LVL1_RANGE = 850;
const float LVL1_FIRERATE = 1.f;
const float LVL1_SLOWAMOUNT = 0.2f;

const int ICE_GOLD_2 = 1500;

const int LVL2_DAMAGE = 150;
const float LVL2_SLOWAMOUNT = 0.4f;

const int ICE_GOLD_3 = 2400;

const int LVL3_DAMAGE = 200;
const float LVL3_SLOWAMOUNT = 0.6f;


IceTurret::IceTurret(CModel* particle, CModel* base, CModel* iceTurret, CSprite* iceImg, CSprite* originalCoin) : Turret(particle, iceImg, originalCoin)
{
	isFieldDown = false;
	otherbase = new CModel(*base);
	turret = new CModel(*iceTurret);
	shot = CModel(0, 0, 0, 100, 30, 30, CColor(1, 136, 129));
	area.LoadModel("area/area.obj");
	CColor areaColor = CColor::Cyan();
	areaColor.SetAlpha(0.8);
	area.SetColor(areaColor);
	area.SetScale(4.f);
	area.SetBoxScale(0.8);
	//area.SetRotation(rand() % 360);
	shot = CModel(0, 0, 0, 100, 20, 20, CColor(37, 193, 156));

	delete shop;
	shop = new CSprite(960, 540, *iceImg);
	shop->SetFrame(1);
	coin->SetPosition(880, 270);

	buttons = {};
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(960, 270);
	buttons.back()->SetSize(285, 90);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(520, 270);
	buttons.back()->SetSize(100, 90);

	totalGold = ICE_GOLD;
	Upgrade();
}

void IceTurret::Level1()
{
	damage = LVL1_DAMAGE;
	range = LVL1_RANGE;
	fireRate = LVL1_FIRERATE;
	slowAmount = LVL1_SLOWAMOUNT;
	gold = ICE_GOLD_2;
}

void IceTurret::Level2()
{
	damage = LVL2_DAMAGE;
	slowAmount = LVL2_SLOWAMOUNT;
	gold = ICE_GOLD_3;
	particles->SetColor(1, 136, 129);
	shop->SetFrame(2);
}

void IceTurret::Level3()
{
	damage = LVL3_DAMAGE;
	slowAmount = LVL3_SLOWAMOUNT;
	area.SetScale(5.f);
	shop->SetFrame(3);
}

void IceTurret::GetEnemiesInRange(vector<Enemy*> enemies)
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
				previousEnemy->SetSpeed(previousEnemy->GetWalkingSpeed());
			}
		}
	}
}

void IceTurret::Shoot()
{
	if (enemiesInRange.size() != 0) {
		if (!isFieldDown) {
			CModel* newShot = new CModel(shot);
			CVector turretDir = turret->GetDirectionV().Normalized() * 130;
			newShot->SetPositionV(turret->GetPositionV() + CVector(0, 240, 0) + turretDir);
			CVector dir = (enemiesInRange[0]->GetPositionV() - CVector(0, enemiesInRange[0]->GetY(), 0)  - newShot->GetPositionV()).Normalized();
			newShot->SetDirectionV(dir);
			newShot->SetRotationV(dir);
			newShot->SetSpeed(5000);
			shots.push_back(newShot);
		}
		else {
			for (Enemy* enemy : enemiesInRange) {
				if (enemy->GetType() != "float")
					enemy->TakeDamage(damage);
			}
		}
	}
	else {
		fireRateCounter = fireRate;
	}
}


void IceTurret::Update(long t, long dt)
{
	if (state != 0 && enemiesInRange.size() != 0 && !isFieldDown) {
		SetDirectionAndRotationToPoint(enemiesInRange[0]->GetPositionV());
	}
	if (isFieldDown) {
		for (Enemy* enemy : enemiesInRange) {
			if (enemy->GetSpeed() != 0 && enemy->GetType() != "float")
				enemy->SetSpeed((1.f - slowAmount) * enemy->GetWalkingSpeed());
		}
	}
	ShotsController();
	Turret::Update(t, dt);
	area.Update(t);
}

void IceTurret::Draw(CGraphics* g)
{
	Turret::Draw(g);
	if (isFieldDown) {
		area.Draw(g);
	}
}


void IceTurret::ShotsController()
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