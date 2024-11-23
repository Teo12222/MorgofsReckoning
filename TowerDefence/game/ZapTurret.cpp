#include "Turrets.h"


const int LVL1_DAMAGE = 650;
const int LVL1_RANGE = 1200;
const float LVL1_FIRERATE = 3.5f;

const int SPARK_GOLD_2 = 1400;
const int ZAP_GOLD_2 = 1700;

const int LVL2_SPARK_DAMAGE = 850;
const float LVL2_SPARK_FIRERATE = 3.f;

const int LVL2_ZAP_DAMAGE = 300;
const float LVL2_ZAP_FIRERATE = 1.5f;
const float LVL2_ZAP_STUNDURATION = 0.5f;
const int LVL2_ZAP_NUMBEROFENEMIES = 10;

const int ZAP_GOLD_3 = 2200;

const int LVL3_SPARK_DAMAGE = 1300;

const int LVL3_ZAP_DAMAGE = 400;
const int LVL3_ZAP_NUMBEROFENEMIES = 15;

ZapTurret::ZapTurret(CModel* particle, CModel* base, CModel* zapTurret, CSprite* zapImg, CSprite* originalCoin) : Turret(particle, zapImg, originalCoin)
{
	stunDuration = 0;
	stunCounter = -1;
	numberOfEnemies = 0;
	otherbase = new CModel(*base);
	turret = new CModel(*zapTurret);
	shot.LoadModel("zapTurret/shot/shot.obj");
	shot.SetScale(50.f);

	delete shop;
	shop = new CSprite(960, 540, *zapImg);
	shop->SetFrame(1);
	coin->SetPosition(880, 270);

	buttons = {};
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(960, 270);
	buttons.back()->SetSize(285, 90);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(520, 270);
	buttons.back()->SetSize(100, 90);

	// Add other two buttons.
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(705, 475);
	buttons.back()->SetSize(480, 285);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(1210, 475);
	buttons.back()->SetSize(480, 285);

	totalGold = ARCHER_GOLD;

	Upgrade();
}

void ZapTurret::SetPath(int selectedPath)
{
	Turret::SetPath(selectedPath);
	if (state == 1) {
		if (path == 1) {
			gold = SPARK_GOLD_2;
		}
		else {
			gold = ZAP_GOLD_2;
		}
	}
}

void ZapTurret::Level1()
{
	damage = LVL1_DAMAGE;
	range = LVL1_RANGE;
	fireRate = LVL1_FIRERATE;
	gold = SPARK_GOLD_2;
}

void ZapTurret::Level2()
{
	if (path == 1) {
		damage = LVL2_SPARK_DAMAGE;
		fireRate = LVL2_SPARK_FIRERATE;
		particles->SetColor(211, 168, 0);
	}
	else if (path == 2) {
		damage = LVL2_ZAP_DAMAGE;
		fireRate = LVL2_ZAP_FIRERATE;
		stunDuration = LVL2_ZAP_STUNDURATION;
		numberOfEnemies = LVL2_ZAP_NUMBEROFENEMIES;
		particles->SetColor(35, 123, 41);
	}
	buttons.pop_back();
	buttons.pop_back();
	shop->SetFrame(state + 1 + path);
	gold = ZAP_GOLD_3;
}

void ZapTurret::Level3()
{
	if (path == 1) {
		damage = LVL3_SPARK_DAMAGE;
		shot.SetScale(90.f);
	}
	else if (path == 2) {
		damage = LVL3_ZAP_DAMAGE;
		numberOfEnemies = LVL3_ZAP_NUMBEROFENEMIES;
	}
	shop->SetFrame(path == 1 ? shop->GetMaxFrames() - 1 : shop->GetMaxFrames());
}

void ZapTurret::Shoot()
{
	if (enemiesInRange.size() != 0) {
		CVector turretDir = turret->GetDirectionV().Normalized() * 150;
		CVector exit = turret->GetPositionV() + CVector(0, 250, 0) + turretDir;
		if (path == 1 || state == 1) {
			CModel* newShot = new CModel(shot);
			newShot->SetPositionV(exit);
			CVector dir = (enemiesInRange[0]->GetPositionV() - newShot->GetPositionV()).Normalized();
			newShot->SetDirectionV(dir);
			newShot->SetRotationV(dir);
			newShot->SetSpeed(1600);
			newShot->SetStatus(distance(allEnemies.begin(), find(allEnemies.begin(), allEnemies.end(), enemiesInRange[0]))); 
			shots.push_back(newShot);
		}
		else if (path == 2 && state > 1) {
			CLine* newLine = new CLine(exit, enemiesInRange[0]->GetPositionV(), CColor::Yellow());
			newLine->SetWidth(30.f);
			newLine->SetStatus(distance(allEnemies.begin(), find(allEnemies.begin(), allEnemies.end(), enemiesInRange[0])));
			shots.push_back(newLine); 
			int i;
			for (i = 0; i < enemiesInRange.size()-1 && i < numberOfEnemies-1; i++) {
				CModel* newVertex = new CModel(0, 0, 0, 25, 25, 25, CColor::Yellow());
				newVertex->SetPositionV(enemiesInRange[i]->GetPositionV());
				newVertex->SetStatus(-1);
				shots.push_back(newVertex);
				newLine = new CLine(enemiesInRange[i]->GetPositionV(), enemiesInRange[i + 1]->GetPositionV(), CColor::Yellow());
				newLine->SetStatus(distance(allEnemies.begin(), find(allEnemies.begin(), allEnemies.end(), enemiesInRange[i+1])));
				newLine->SetWidth(30.f);
				shots.push_back(newLine);
				enemiesInRange[i]->TakeDamage(damage);
				if (enemiesInRange[i]->GetType() != "boss")
					enemiesInRange[i]->SetSpeed(0);
			}
			enemiesInRange[i]->TakeDamage(damage); 
			if (enemiesInRange[i]->GetType() != "boss")
				enemiesInRange[i]->SetSpeed(0);
			CModel* newVertex = new CModel(0, 0, 0, 25, 25, 25, CColor::Yellow());
			newVertex->SetPositionV(enemiesInRange[i]->GetPositionV());
			newVertex->SetStatus(-1);
			shots.push_back(newVertex);
			stunCounter = 0;
		}
	}
	else {
		fireRateCounter = fireRate;
	}
}

void ZapTurret::Update(long t, long dt)
{
	if (state != 0) {
		if (enemiesInRange.size() != 0) {
			SetDirectionAndRotationToPoint(enemiesInRange[0]->GetPositionV());
		}
		if (stunCounter != -1)
			stunCounter += dt / 1000.f;
	}
	Turret::Update(t, dt);
	ShotsController();
}

void ZapTurret::ShotsController()
{
	if (path == 1 || state == 1) {
		bool deleted = false;
		for (CModel* shot : shots) {
			Enemy* targetEnemy = allEnemies[shot->GetStatus()];
			CVector dir = (targetEnemy->GetPositionV() - shot->GetPositionV()).Normalized();
			shot->SetDirectionV(dir);
			shot->SetRotationV(dir);
			if (shot->GetPositionV().Distance(targetEnemy->GetPositionV()) < 30) {
				for (Enemy* enemy : allEnemies) {
					if (shot->HitTest(enemy) && !enemy->IsDeleted()) {
						enemy->TakeDamage(damage);
					}
				}
				shot->Delete();
				deleted = true;
			}
		}
		shots.delete_if(deleted);
	}
	else if (path == 2 && state > 1) {
		if (stunCounter >= stunDuration) {
			for (CModel* shot : shots) {
				if (shot->GetStatus() != -1 && allEnemies.size() != 0)
					allEnemies[shot->GetStatus()]->SetSpeed(allEnemies[shot->GetStatus()]->GetWalkingSpeed());
			}
			shots.clear();
			stunCounter = -1;
		}
	}
}
