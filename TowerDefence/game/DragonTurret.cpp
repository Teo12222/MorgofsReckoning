#include "Turrets.h"


const int LVL1_DAMAGE = 200;
const int LVL1_RANGE = 1200;
const float LVL1_FIRERATE = 1.f;

const int DRAGON_GOLD_2 = 2200;

const int LVL2_DAMAGE = 400;

const int DRAGON_GOLD_3 = 3000;

const int LVL3_DAMAGE = 800;


DragonTurret::DragonTurret(CModel* particle, CModel* base, CModel* dragTurret, CSprite* dragImg, CSprite* originalCoin) : Turret(particle, dragImg, originalCoin)
{
	otherbase = new CModel(*base);
	turret = new CModel(*dragTurret);
	shot.LoadModel("zapTurret/shot/shot.obj");
	shot.SetScale(50.f);
	shot.SetHealth(50);

	delete shop;
	shop = new CSprite(960, 540, *dragImg);
	shop->SetFrame(1);
	coin->SetPosition(880, 270);

	buttons = {};
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(960, 270);
	buttons.back()->SetSize(285, 90);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(520, 270);
	buttons.back()->SetSize(100, 90);

	totalGold = DRAGON_GOLD;
	Upgrade();
}

void DragonTurret::Level1()
{
	damage = LVL1_DAMAGE;
	range = LVL1_RANGE;
	fireRate = LVL1_FIRERATE;
	gold = DRAGON_GOLD_2;
}

void DragonTurret::Level2()
{
	damage = LVL2_DAMAGE;
	shot.SetScale(80.f);
	gold = DRAGON_GOLD_3;
	particles->SetColor(141, 7, 141);
	shop->SetFrame(2);
}

void DragonTurret::Level3()
{
	damage = LVL3_DAMAGE;
	shot.SetHealth(100);
	shop->SetFrame(3);
}

void DragonTurret::Shoot()
{
	if (enemiesInRange.size() != 0) {
		CModel* newShot = new CModel(shot);
		CVector turretDir = turret->GetDirectionV().Normalized() * 160;
		newShot->SetPositionV(turret->GetPositionV() + CVector(0, 200, 0) + turretDir);
		CVector dir = (enemiesInRange[0]->GetPositionV() - newShot->GetPositionV()).Normalized();
		newShot->SetDirectionV(dir);
		newShot->SetRotationV(dir);
		newShot->SetID(newShot->GetRotation());
		newShot->SetSpeed(1800);
		newShot->SetStatus(distance(allEnemies.begin(), find(allEnemies.begin(), allEnemies.end(), enemiesInRange[0]))); 
		shots.push_back(newShot);
	}
	else {
		fireRateCounter = fireRate;
	}
}

void DragonTurret::Update(long t, long dt)
{
	if (state != 0 && enemiesInRange.size() != 0) {
		SetDirectionAndRotationToPoint(enemiesInRange[0]->GetPositionV());
	}
	Turret::Update(t, dt);
	bool deleted = false;
	for (CModel* shot : shots) {
		if (shot->GetHealth() == 25) {
			shot->SetID(shot->GetID() + dt / 10);
			if (shot->GetID() > 30) {
				shot->Delete();
				deleted = true;
			}
		}
	}
	shots.delete_if(deleted);
	ShotsController();
}

void DragonTurret::ShotsController()
{
	CModelList addedShots;
	bool deleted = false;
	for (CModel* cShot : shots) {
		if (cShot->GetHealth() != 25) {
			Enemy* targetEnemy = allEnemies[cShot->GetStatus()];
			CVector dir = (targetEnemy->GetPositionV() - cShot->GetPositionV()).Normalized();
			cShot->SetDirectionV(dir);
			cShot->SetRotationV(dir);
			if (cShot->GetPositionV().Distance(targetEnemy->GetPositionV()) < 15) {
				if (state == 1) {
					targetEnemy->TakeDamage(damage);
				}
				else if (state > 1) {
					for (Enemy* enemy : allEnemies) {
						if (cShot->HitTest(enemy) && !enemy->IsDeleted()) {
							enemy->TakeDamage(damage);
						}
					}
					if (state == 3 && cShot->GetHealth() == 100) {
						//Add 3 splitting bullets, make sure that it spawns away from the targeted enemy
						CModel* newShot = new CModel(shot);
						newShot->SetPositionV(cShot->GetPositionV());
						newShot->SetRotation(cShot->GetID());
						newShot->SetDirection(newShot->GetRotation());
						newShot->SetScale(50.f);
						newShot->SetSpeed(1500);
						newShot->SetHealth(25);
						newShot->SetID(0);
						newShot->SetStatus(distance(allEnemies.begin(), find(allEnemies.begin(), allEnemies.end(), targetEnemy)));
						addedShots.push_back(newShot);
						newShot = new CModel(shot);
						newShot->SetPositionV(cShot->GetPositionV());
						newShot->SetRotation(cShot->GetID() + 45);
						newShot->SetDirection(newShot->GetRotation());
						newShot->SetScale(50.f);
						newShot->SetSpeed(1500);
						newShot->SetHealth(25);
						newShot->SetID(0);
						newShot->SetStatus(distance(allEnemies.begin(), find(allEnemies.begin(), allEnemies.end(), targetEnemy)));
						addedShots.push_back(newShot);
						newShot = new CModel(shot);
						newShot->SetPositionV(cShot->GetPositionV());
						newShot->SetRotation(cShot->GetID() - 45);
						newShot->SetDirection(newShot->GetRotation());
						newShot->SetScale(50.f);
						newShot->SetSpeed(1500);
						newShot->SetHealth(25);
						newShot->SetID(0);
						newShot->SetStatus(distance(allEnemies.begin(), find(allEnemies.begin(), allEnemies.end(), targetEnemy)));
						addedShots.push_back(newShot);
					}
				}
				cShot->Delete();
				deleted = true;
			}
		}
		else {
			for (Enemy* enemy : allEnemies) {
				if (cShot->HitTest(enemy) && !enemy->IsDeleted() && enemy != allEnemies[cShot->GetStatus()]) {
					enemy->TakeDamage(damage);
					cShot->Delete();
					deleted = true;
				}				
			}
		}
	}
	shots.delete_if(deleted);
	for (CModel* newShot : addedShots) {
		shots.push_back(newShot);
	}
}
