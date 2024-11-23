#include "Turret.h"
#include <algorithm>




float Turret::fireRateIncrease = 0;

Turret::Turret(CModel* particle, CSprite* turretImg, CSprite* originalCoin)
{
	damageTick = 0;
	damageTickDuration = 0;
	gold = 0;
	state = 0;
	path = 1;
	fireRateCounter = 0;
	base = CModel(0, 0, 0, 290, 30, 290, CColor::White());
	particles = new CModel(*particle);
	turret = nullptr;
	otherbase = nullptr;

	particles->SetOmega(40);
	coin = new CSprite(940, 270, *originalCoin);
	coin->PlayAnimation(0, 6, 8, true);
	shop = new CSprite(960, 540, *turretImg);

	// Creates the buttons for UI
	buttons = {};
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(425, 800);
	buttons.back()->SetSize(190, 110);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(425, 670);
	buttons.back()->SetSize(190, 110);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(425, 540);
	buttons.back()->SetSize(190, 110);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(425, 410);
	buttons.back()->SetSize(190, 110);
	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(425, 280);
	buttons.back()->SetSize(190, 110);

	buttons.push_back(new CSprite());
	buttons.back()->SetPosition(1020, 265);
	buttons.back()->SetSize(280, 90);

}


void Turret::SetPath(int selectedPath)
{
	path = selectedPath;
	shop->SetFrame(path);
}

int Turret::GetState()
{
	return state;
}

bool Turret::HitTest(CModel *pModel) {
	if (base.HitTest(pModel)) return true;
	else if (state != 0 && (otherbase->HitTest(pModel) || turret->HitTest(pModel))) return true;
	return false;
}

bool Turret::HitTest(CVector pos)
{
	return base.HitTest(pos);
}

void Turret::GetEnemiesInRange(vector<Enemy*> enemies)
{
	allEnemies = enemies;
	enemiesInRange.clear();
	for (Enemy* enemy : enemies) {
		if (!enemy->IsDeleted() && CVector(enemy->GetX(), 0 , enemy->GetZ()).Distance(base.GetPositionV()) < range) {
			enemiesInRange.push_back(enemy);
		}
	}
	// https://stackoverflow.com/questions/26903602/an-enclosing-function-local-variable-cannot-be-referenced-in-a-lambda-body-unles
	sort(enemiesInRange.begin(), enemiesInRange.end(), [&](Enemy* a, Enemy* b) { return a->GetPositionV().Distance(base.GetPositionV()) < b->GetPositionV().Distance(base.GetPositionV()); });
}

void Turret::SetPosition(float x, float z)
{
	base.SetPosition(x, z);
	particles->SetPosition(x, z);
	if (otherbase != nullptr && turret != nullptr) {
		otherbase->SetPosition(x, z);
		turret->SetPosition(x, z);
	}
}

void Turret::SetPositionV(CVector pos)
{
	SetPosition(pos.x, pos.z);
}

CVector Turret::GetPosition()
{
	return base.GetPositionV();
}

void Turret::SetDirectionAndRotationToPoint(CVector pos)
{
	turret->SetDirectionAndRotationToPoint(pos.x, pos.z);
}

void Turret::Upgrade()
{
	switch (state)
	{
	case 0:
		state++;
		Level1();
		break;
	case 1:
		Level2();
		totalGold += gold;
		state++;
		break;
	case 2:
		Level3();
		base.SetColor(255, 191, 0);
		totalGold += gold;
		state++;
		break;
	default:
		break;
	}
}

int Turret::GetGold(int selectedTurret) 
{
	if (state == 0) {
		if (selectedTurret == 0) return ARCHER_GOLD;
		else if (selectedTurret == 1) return ZAP_GOLD;
		else if (selectedTurret == 2) return ICE_GOLD;
		else if (selectedTurret == 3) return FIRE_GOLD;
		else if (selectedTurret == 4) return DRAGON_GOLD;
	}
	return gold;
}

void Turret::Level1()
{
	state--; 
}

int Turret::GetTotalGold()
{
	return totalGold;
}

void Turret::Update(long t, long dt)
{
	if (state != 0) {
		turret->Update(t);
		particles->Update(t);
		shots.Update(t);
		if (fireRate != 0) {
			fireRateCounter += dt / 1000.f;
		}
		if (fireRateCounter > (fireRate * (1-fireRateIncrease))) {
			fireRateCounter = 0;
			Shoot();
		}
	}
	if (isOpen) {
		coin->Update(t);
	}
}

void Turret::Draw(CGraphics* g) {
	base.Draw(g);
	shots.Draw(g);
	if (state != 0) {
		otherbase->Draw(g);
		turret->Draw(g);
	}
	if (state >= 2) {
		particles->Draw(g);
	}
}

void Turret::DrawMenu(CGraphics* g)
{
	if (isOpen) {
		shop->Draw(g);
		if (state != 3)
			coin->Draw(g);
	}
}

void Turret::OpenShop(int turret)
{
	isOpen = true;
	if (state == 0)
		shop->SetFrame(turret + 1);
}

void Turret::CloseShop()
{
	isOpen = false;
}

bool Turret::IsOpen()
{
	return isOpen;
}

int Turret::HandleButtons(float x, float y)
{
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i]->HitTest(x, y)) {
			return i;
		}
	}
	return -1;
}
