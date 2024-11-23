#include "MyGame.h"
#include <algorithm>

using namespace std;

CMyGame::CMyGame(void) :
rightClick(0, 0, 0, 50, 50, 50, CColor::White()),
leftClick(0, 0, 0, 50, 50, 50, CColor::White())
{
	srand(time(NULL));
}

CMyGame::~CMyGame(void) {}


// --------  game initialisation --------
void CMyGame::OnInitialize()
{
	// loading font
	font.Load("font2.bmp");

	// enable lighting
	Light.Enable();

	skydome.LoadModel("Skydome/Skydome.obj", "Skydome/Skydome.bmp");
	skydome.SetScale(875.f);
	skydome.SetY(-700.0f);
	// player md2 model and texture
	player.LoadModel("Abarlith/Abarlith.md2"); 
	player.SetScale( 5.0f);
	
	playerside.LoadModel("Abarlith/Abarlith.md2");
	playerside.SetScale(5.0f);

	playerbottom.LoadModel("Abarlith/Abarlith.md2");
	playerbottom.SetScale(5.0f);

	playerfront.LoadModel("Abarlith/Abarlith.md2");
	playerfront.SetScale(5.0f);

	selectedTurret = ARCHER;

	// Loads enemies models and sets the stats
	regularEnemy.LoadModel("bull/bull.md3", "bull/bull.png");
	regularEnemy.SetScale(80.f);
	regularEnemy.SetHealth(1200);
	regularEnemy.SetWalkingSpeed(550);
	regularEnemy.SetToFloor(5);
	regularEnemy.SetType("regular");

	fireEnemy.LoadModel("fire/fire.md3", "fire/fire.png");
	fireEnemy.SetScale(95.f);
	fireEnemy.SetColor(210, 100, 0);
	fireEnemy.SetHealth(1000);
	fireEnemy.SetWalkingSpeed(600);
	fireEnemy.SetToFloor(33);
	fireEnemy.SetType("fire");

	floatEnemy.LoadModel("ghost/ghost.md3", "ghost/ghost.png");
	floatEnemy.SetScale(65.f);
	floatEnemy.SetColor(20, 100, 20);
	floatEnemy.SetHealth(900);
	floatEnemy.SetWalkingSpeed(800);
	floatEnemy.SetToFloor(0);
	floatEnemy.SetType("float");

	speedEnemy.LoadModel("endSpider/endSpider.md3", "endSpider/endSpiderScaled.png");
	speedEnemy.SetScale(150.f);
	speedEnemy.SetColor(20, 20, 100);
	speedEnemy.SetHealth(800);
	speedEnemy.SetWalkingSpeed(1000);
	speedEnemy.SetToFloor(5);
	speedEnemy.SetType("speed");

	
	Listar.LoadModel("bull/bull.md3", "bull/bull.png");
	Listar.SetScale(190.f);
	Listar.SetHealth(75000);
	Listar.SetWalkingSpeed(750);
	Listar.SetToFloor(5);
	Listar.SetType("boss");

	Melchor.LoadModel("bull/bull.md3", "bull/purpleBull.png");
	Melchor.SetScale(210.f);
	Melchor.SetHealth(150000);
	Melchor.SetWalkingSpeed(1400);
	Melchor.SetToFloor(5);
	Melchor.SetType("boss");
	
	Morgof.LoadModel("bull/bull.md3", "bull/redBull.png");
	Morgof.SetScale(250.f);
	Morgof.SetHealth(400000);
	Morgof.SetWalkingSpeed(1100);
	Morgof.SetToFloor(5);
	Morgof.SetType("boss");

	path.LoadModel("paths/path.obj");
	path.SetScale(10.f);
	path.SetToFloor(0);

	// Loads Nature Models
	for (int i = 0; i <= 7; i++) {
		CModel* trunk = new CModel(); trunk->LoadModel("Nature/Nature/trunks/trunk_"+to_string(i)+"/obj/trunk_" + to_string(i) + ".obj"); trunk->SetScale(100.f);
		trunks.push_back(trunk);
	}
	for (int i = 0; i <= 5; i++) {
		CModel* canopy = new CModel(); canopy->LoadModel("Nature/Nature/canopies/canopy_" + to_string(i) + "/obj/canopy_"+to_string(i)+".obj"); canopy->SetScale(100.f);
		canopies.push_back(canopy);
	}
	for (int i = 0; i <= 2; i++) {
		CModel* grassType = new CModel(); grassType->LoadModel("Nature/Nature/grass_" + to_string(i) + "/obj/grass_" + to_string(i) + ".obj"); grassType->SetScale(100.f);
		grass.push_back(grassType);
	}

	CModel* rose = new CModel(); rose->LoadModel("Nature/Nature/flower_rose/obj/flower_rose.obj"); rose->SetScale(100.f);
	flowers.push_back(rose);
	CModel* sunflower = new CModel(); sunflower->LoadModel("Nature/Nature/flower_sunflower/obj/flower_sunflower.obj"); sunflower->SetScale(100.f);
	flowers.push_back(sunflower);
	CModel* whiteTulip = new CModel(); whiteTulip->LoadModel("Nature/Nature/flower_white_tulip/obj/flower_white_tulip.obj"); whiteTulip->SetScale(100.f);
	flowers.push_back(whiteTulip);

	for (int i = 0; i <= 3; i++) {
		CModel* rock = new CModel(); rock->LoadModel("Nature/Nature/rocks/rock_" + to_string(i) + "/obj/rock_" + to_string(i) + ".obj"); rock->SetScale(250.f);
		rocks.push_back(rock);
	}

	// Miscellaneous Models
	blackThing.LoadModel("blackThing/blackThing.obj");
	blackThing.SetScale(3.5f);
	poster.LoadModel("poster/poster.obj");
	poster.SetScale(0.3f);
	castle.LoadModel("castle/castle.obj");
	castle.SetScale(1250.f);
	castle.SetBoxScale(0.7);

	blackHole.LoadModel("area/area.obj");
	blackHole.SetColor(CColor::Black());
	blackHole.SetScale(4.f);
	blackHole.SetBoxScale(0.8);
	blackHole.Delete();

	coin.LoadImage("coin.png", CColor::Black(), 7);
	coin.SetScale(2.5f);
	coin.SetPosition(Width - 330, Height - 30);
	coin.PlayAnimation(0, 6, 8, true);
	swordCoin = new CSprite(940, 270, coin, GetTime());
	swordCoin->PlayAnimation(0, 6, 8, true);
	

	// Load turret models
	particle.LoadModel("rotatingDonut/thing.obj");
	particle.SetScale(150.f);
	archerBase.LoadModel("archerTurret/base/base.obj");
	archerBase.SetScale(150.f);
	archerTurret.LoadModel("archerTurret/turret/turret.obj");
	archerTurret.SetScale(150.f);
	zapBase.LoadModel("zapTurret/base/base.obj");
	zapBase.SetScale(150.f);
	zapTurret.LoadModel("zapTurret/turret/turret.obj");
	zapTurret.SetScale(150.f);
	iceBase.LoadModel("iceTurret/base/base.obj");
	iceBase.SetScale(150.f);
	iceTurret.LoadModel("iceTurret/turret/turret.obj");
	iceTurret.SetScale(150.f);
	fireBase.LoadModel("fireTurret/base/base.obj");
	fireBase.SetScale(150.f);
	fireTurret.LoadModel("fireTurret/turret/turret.obj");
	fireTurret.SetScale(150.f);
	dragonBase.LoadModel("dragonTurret/base/base.obj");
	dragonBase.SetScale(150.f);
	dragonTurret.LoadModel("dragonTurret/turret/turret.obj");
	dragonTurret.SetScale(150.f);
	

	// Load Menu Images
	turretsImg.LoadImage("turrets1.bmp", CColor::Black(), 5);
	archerImg.LoadImage("archerTurret.bmp", CColor::Black(), 6);
	zapImg.LoadImage("zapTurret.bmp", CColor::Black(), 6);
	iceImg.LoadImage("iceTurret.bmp", CColor::Black(), 3);
	fireImg.LoadImage("fireTurret.bmp", CColor::Black(), 3);
	dragonImg.LoadImage("dragonTurret.bmp", CColor::Black(), 3);
	

	pauseMenu.LoadImage("pauseMenu.png");
	pauseMenu.SetPosition(Width / 2, Height / 2);
	pauseButtons.push_back(new CSprite());
	pauseButtons.back()->SetPosition(960, 585);
	pauseButtons.back()->SetSize(535, 110);
	pauseButtons.push_back(new CSprite());
	pauseButtons.back()->SetPosition(960, 440);
	pauseButtons.back()->SetSize(535, 110);
	pauseButtons.push_back(new CSprite());
	pauseButtons.back()->SetPosition(960, 295);
	pauseButtons.back()->SetSize(535, 110);

	endMenu.LoadImage("endMenu.png");
	endMenu.SetPosition(Width / 2, Height / 2);
	endButtons.push_back(new CSprite());
	endButtons.back()->SetPosition(800, 275);
	endButtons.back()->SetSize(265, 105);
	endButtons.push_back(new CSprite());
	endButtons.back()->SetPosition(1125, 275);
	endButtons.back()->SetSize(265, 105);

	mainMenu.LoadImage("mainMenu.png");
	mainMenu.SetPosition(Width / 2, Height / 2);
	mainButtons.push_back(new CSprite());
	mainButtons.back()->SetPosition(960, 450);
	mainButtons.back()->SetSize(790, 165);
	mainButtons.push_back(new CSprite());
	mainButtons.back()->SetPosition(960, 230);
	mainButtons.back()->SetSize(790, 165);

	cursor.LoadImage("crosshair.png");
	cursor.SetImage("crosshair.png");

	menuCursor.LoadImage("cursor.png", CColor::White());
	menuCursor.SetScale(0.2f);
	menuCursor.SetImage("cursor.png");
	HideMouse();

	// Load Music
	music.Play("music.wav", -1);
	footsteps.Play("footsteps.wav", -1);
	footsteps.Pause();
	morgofSound.Play("demonsound.wav", -1);
	morgofSound.Pause();

	// Set Up health bars
	structHealthBar.SetSize(800, 40);
	structHealthBar.SetPosition(Width / 2, Height - 40);
	structHealthBar.SetColors(CColor::Red(), CColor::White(), CColor::LightGray());

	timeBetweenWavesBar.SetSize(800, 10);
	timeBetweenWavesBar.SetColors(CColor(135, 211, 255), CColor::White(), CColor::LightGray());
	timeBetweenWavesBar.SetPosition(Width / 2, Height - 67);
	
	bossHealth.SetSize(800, 40);
	bossHealth.SetPosition(Width / 2, Height - 130);
	bossHealth.SetColors(CColor::Red(), CColor::Black(), CColor::LightGray());

	// floor texture
	floor.LoadTexture("texture_grass_dark.bmp");
	floor.SetTiling(true);
}

	
void CMyGame::OnStartLevel(int level)
{ 
	// set size of the game world
	floor.SetSize(35000, 35000);
	floor.SetPosition(11000, 12000);
	skydome.SetPosition(11000, 12000);
	player.SetRotationV(0, 0, 0); 
	player.SetDirection(1, 0, -0);
	playerfront.SetDirection(1, 0, -0);
	player.SetToFloor(0); // set player to align with floor
	playerfront.SetToFloor(0);
	playerside.SetToFloor(0);
	playerbottom.SetToFloor(0);
	playerbottom.SetRotationV(0, -90, 0);
	playerbottom.SetDirectionV(playerbottom.GetRotationV());
	player.Rotate(0, -90, 13);
	playerside.SetRotation(player.GetRotation() + 90);
	playerside.SetDirection(playerside.GetRotation());
	player.SetDirectionV(player.GetRotationV());
	playerfront.SetDirection(player.GetRotation());

	sword.Reset();
	gold = 1000;
	wave = 0;
	isSpeed = false;
	playState = PLAYING;
	won = false;
	structHealthBar.SetHealth(100);

	music.SetVolume(100);

	bossNames = { "Morgof", "Enraged Listar", "Melchor", "Listar" };
	isBoss = false;
	blackThing.SetColor(CColor(47, 25, 95));
	morgofSound.Pause();

	nature.clear();
	paths.clear();
	turretList.clear();
	enemies.clear();
	GenerateWorld();
	playerside.SetPositionV(player.GetPositionV()); playerfront.SetPositionV(player.GetPositionV()); playerbottom.SetPositionV(player.GetPositionV());

	timeBetweenWaves = 10.f;
	waveTimer = timeBetweenWaves;
	offset = GetTime();

	cursor.SetPosition(Width / 2, Height / 2);
}

// Game Logic in the OnUpdate function called every frame 
void CMyGame::OnUpdate() 
{
	if (IsMenuMode() || IsGameOver()) return;
	static long t = GetTime();
	long dt = GetTime() - t;
	t = GetTime();
	if (waveTimer != -1) {
		waveTimer -= dt / 1000.f;
		timeBetweenWavesBar.SetHealth(waveTimer/timeBetweenWaves * 100.f);
	}
	if (waveTimer < 0 && waveTimer > -1) {
		waveStart.Play("startwave.wav");
		waveTimer = -1;
		NextWave();
	}
	PlaceWave(dt);
	if (playState == PLAYING)
		PlayerControl(t);
	if (enemiesInWave.size() == 0 && enemies.size() != 0 && wave != 0) {
		bool allDeleted = true;
		for (Enemy* enemy : enemies) {
			if (!enemy->IsDeleted()) {
				allDeleted = false;
				break;
			}
		}
		if (allDeleted && waveTimer == -1) {
			if (wave == 100 && structHealthBar.GetHealth() > 0) {
				playState = MENU;
				menuState = END;
				won = true;
			}
			else if (menuState != END) {
				waveTimer = timeBetweenWaves;
				static float goldPerRound = 2000.f;
				gold += goldPerRound;
				goldPerRound *= 1.05;
				if (goldPerRound > 10000.f) {
					goldPerRound = 10000.f;
				}
			}
		}
	}
	for (int i = 0; i < turretList.size(); i++) {
		if (rightClick.GetSpeed() != 0 && turretList[i]->HitTest(&rightClick)) {
			turretList[i]->OpenShop(selectedTurret);
			rightClick.SetSpeed(0);
			playState = MENU;
			menuState = OTHER;
			
		}
	}
	vector<Enemy*> hitEnemies;
	for (Enemy* enemy : enemies) {
		if (leftClick.GetSpeed() != 0 && !enemy->IsDeleted() && enemy->HitTest(&leftClick)) {
			hitEnemies.push_back(enemy);
		}
	}
	// Damages the closest enemy hit by the "swing" of the sword
	sort(hitEnemies.begin(), hitEnemies.end(), [&](Enemy* a, Enemy* b) { return a->GetPositionV().Distance(player.GetPositionV()) < b->GetPositionV().Distance(player.GetPositionV()); });
	if (hitEnemies.size() != 0) {
		leftClick.SetSpeed(0);
		hitEnemies[0]->TakeDamage(sword.GetDamage());
	}

	rightClick.Update(t);
	leftClick.Update(t);
	sword.Update(t);
	sword.Action(dt);
	
	for (Turret* turret : turretList) {
		turret->GetEnemiesInRange(enemies);
		turret->Update(t, dt);
	}

	for (Enemy* enemy : enemies) {
		if (enemy->GetTurn() < Enemy::enemyPath.size()) {
			CVector nextTurn = Enemy::enemyPath[enemy->GetTurn()];
			if ((enemy->GetX() > nextTurn.x - 375 && enemy->GetX() < nextTurn.x + 375 && enemy->GetZ() + enemy->GetStart() > nextTurn.z  - 25 && enemy->GetZ() + enemy->GetStart() < nextTurn.z + 25 && enemy->GetRotation() == -90)
				|| (enemy->GetZ() > nextTurn.z - 375 && enemy->GetZ() < nextTurn.z + 375 && enemy->GetX() + enemy->GetStart() > nextTurn.x - 25 && enemy->GetX() + enemy->GetStart() < nextTurn.x + 25 && (enemy->GetRotation() == 0 || enemy->GetRotation() == -180))) {
				if (nextTurn.y == 0) {
					enemy->Rotate(90);
					enemy->SetDirection(enemy->GetRotation());
					CVector pos = enemy->GetPositionV();
					enemy->Update(t + 300);
					Enemy::enemyPath[enemy->GetTurn()].y = 2;
					for (CModel* path : paths) {
						if (path->HitTest(enemy)) {
							Enemy::enemyPath[enemy->GetTurn()].y = 1;
						}
					}
					enemy->Rotate(-90);
					enemy->SetDirection(enemy->GetRotation());
				}
				CVector nextTurn = Enemy::enemyPath[enemy->GetTurn()];
				if (nextTurn.y == 1) {
					enemy->Rotate(90);
					enemy->SetDirection(enemy->GetRotation());
					enemy->IncrementTurn();
				}
				else if (nextTurn.y == 2) {
					enemy->Rotate(-90);
					enemy->SetDirection(enemy->GetRotation());
					enemy->IncrementTurn();
				}
			}
		}
		if (!blackHole.IsDeleted() && enemy->GetPositionV().z > blackHole.GetPositionV().z && blackHole.GetStatus() < 5 && enemy->GetType() != "boss" && enemy->GetSpeed() != 0) {
			enemy->SetSpeed(0);
			blackHole.SetStatus(blackHole.GetStatus() + 1);
			if (blackHole.GetStatus() == 5) {
				for (Enemy* hitEnemy : enemies) {
					if (!hitEnemy->IsDeleted() && hitEnemy->HitTest(&blackHole)) {
						hitEnemy->TakeDamage(75000.f);
					}
				}
				blackHole.SetStatus(0);
				blackHole.Delete();
			}
		}
		enemy->Update(t);
		enemy->Action(dt);
		if (enemy->HitTest(&castle) && !enemy->IsDeleted()) {
			enemy->Delete();
			structHealthBar.SetHealth(structHealthBar.GetHealth() - 1);
			if (enemy->GetType() == "boss") {
				isBoss = false;
				if (bossNames.size() != 0) {
					if (bossNames.back() == "Listar") {
						structHealthBar.SetHealth(structHealthBar.GetHealth() - 49);
						Listar.SetHealth(75000 * 4);
						Listar.SetWalkingSpeed(850);
					}
					else if (bossNames.back() == "Melchor") {
						structHealthBar.SetHealth(structHealthBar.GetHealth() - 49);
						blackThing.SetColor(CColor::Black());
					}
					else if (bossNames.back() == "Enraged Listar") {
						structHealthBar.SetHealth(structHealthBar.GetHealth() - 74);
					}
					else if (bossNames.back() == "Morgof") {
						structHealthBar.SetHealth(structHealthBar.GetHealth() - 99);
					}
					bossNames.pop_back();
				}
				else {
					structHealthBar.SetHealth(structHealthBar.GetHealth() - 99);
				}
			}
		}

		if (enemy->GetType() == "boss") {
			if (isBoss) {
				bossHealth.SetHealth((float)enemy->GetHealth()/ (float)currentBossHealth * 100.f);
				cout << enemy->GetHealth() << " " << currentBossHealth << " " << bossHealth.GetHealth() << endl;
			}
			if (enemy->IsDeleted() && isBoss) {
				isBoss = false;
				if (bossNames.size() != 0) {
					if (bossNames.back() == "Listar" || bossNames.back() == "Enraged Listar") {
						enemy->UnDelete();
						enemy->SetHealth(1000000);
						enemy->PlayAnimation(42, 62, 28, true);
						Turret* turretToCharge = turretList[0];
						for (Turret* turret : turretList) {
							if (turret->GetState() > 0 && enemy->GetPositionV().Distance(turret->GetPosition()) < enemy->GetPositionV().Distance(turretToCharge->GetPosition())) {
								turretToCharge = turret;
							}
						}
						enemy->SetDirectionV((CVector(turretToCharge->GetPosition().x, enemy->GetY(), turretToCharge->GetPosition().z) - enemy->GetPositionV()).Normalized());
						enemy->SetRotation(enemy->GetDirection());
						enemy->SetSpeed(2500);
						Listar.SetHealth(75000 * 4);
						Listar.SetWalkingSpeed(850);
					}
					if (bossNames.back() == "Melchor") {
						blackThing.SetColor(CColor::Black());
					}
					if (bossNames.back() == "Morgof") {
						isBoss = true;
						enemy->UnDelete();
						enemy->SetHealth(200000);
						enemy->SetWalkingSpeed(1500);
						enemy->SetScale(300.f);
						morgofSound.Resume();
					}
					bossNames.pop_back();
				}
			}
			else {
				for (int i = 0; i < turretList.size(); i++) {
					if (turretList[i]->HitTest(enemy)) {
						enemy->Delete();
						enemy->SetSpeed(0);
						turretList.erase(turretList.begin() + i);
						break;
					}
				}
			}
		}
	}	
	if (structHealthBar.GetHealth() <= 0) {
		playState = MENU;
		menuState = END;
	}

	coin.Update(t);
	if (sword.IsOpen()) {
		swordCoin->Update(t);
	}
}


const float DEFAULT_SPEED = 1700.f;
const float SPRINTING_SPEED = 2400.f;
const float PLAYER_REACH = 600.f;

void CMyGame::PlayerControl(long t)
{

	// -----  player control --------
	bool onGround = false;
	if (IsKeyDown(SDLK_LCTRL)) {
		playerSpeed = SPRINTING_SPEED;
	}
	else playerSpeed = DEFAULT_SPEED;
	playerbottom.SetSpeed(0);
	playerside.SetSpeed(0);
	playerfront.SetSpeed(0);
	CVector playerBottomPos = playerbottom.GetPositionV();
	CVector playerFrontPos = playerfront.GetPositionV();
	CVector playerSidePos = playerside.GetPositionV();
	if (IsKeyDown(SDLK_SPACE) && player.GetPositionV().y < 10000) {
		playerbottom.SetSpeed(playerbottom.GetSpeed() - DEFAULT_SPEED);
	}
	if (IsKeyDown(SDLK_LSHIFT)) {
		playerbottom.SetSpeed(playerbottom.GetSpeed() + DEFAULT_SPEED);
		
	}
	if (IsKeyDown(SDLK_a)) {
		playerside.SetSpeed(playerside.GetSpeed() + DEFAULT_SPEED);
	}
	if (IsKeyDown(SDLK_d)) {
		playerside.SetSpeed(playerside.GetSpeed() - DEFAULT_SPEED);
	}
	if (IsKeyDown(SDLK_s)) { 
		playerfront.SetSpeed(playerfront.GetSpeed() - DEFAULT_SPEED);
	}
	if (IsKeyDown(SDLK_w)) {
		playerfront.SetSpeed(playerfront.GetSpeed() + playerSpeed);
	} 
	
	player.Update(t);
	playerside.Update(t);
	playerbottom.Update(t);
	playerfront.Update(t);
	if (playerbottom.GetBottom() < 0) {
		playerbottom.SetToFloor(0);
	}
	if (playerbottom.GetBottom() <= 5) {
		onGround = true;
		
	}
	for (CModel* green : nature) {
		if (green->GetStatus() == 1) {
			if (playerfront.HitTestFront(green)) {
				playerfront.SetPositionV(playerFrontPos);
			}
			if (playerfront.HitTestBack(green)) {
				playerfront.SetPositionV(playerFrontPos);
			}
			if (playerside.HitTestFront(green)) {
				playerside.SetPositionV(playerSidePos);
			}
			if (playerside.HitTestBack(green)) {
				playerside.SetPositionV(playerSidePos);
			}
			if (playerbottom.HitTestFront(green)) {
				playerbottom.SetPositionV(playerBottomPos);
			}
			if (playerbottom.HitTestBack(green)) {
				playerbottom.SetPositionV(playerBottomPos);
			}
		}
	}
	if (playerfront.GetPositionV().x < -700 || playerfront.GetPositionV().x > 22000 || playerfront.GetPositionV().z < -900 || playerfront.GetPositionV().z > 24800) {
		playerfront.SetPositionV(playerFrontPos);
	}
	if (playerside.GetPositionV().x < -700 || playerside.GetPositionV().x > 22000 || playerside.GetPositionV().z < -900 || playerside.GetPositionV().z > 24800) {
		playerside.SetPositionV(playerSidePos);
	}
	CVector f = playerfront.GetPositionV() - player.GetPositionV();
	CVector s = playerside.GetPositionV() - player.GetPositionV();
	CVector b = playerbottom.GetPositionV() - player.GetPositionV();
	if (f != CVector(0, 0, 0) && s != CVector(0, 0, 0)) { 
		f /= 2; s /= 2; 
		if (playerfront.GetSpeed() == SPRINTING_SPEED) {
			f *= 2;
		}
	}
	if ((f != CVector(0, 0, 0) || s != CVector(0, 0, 0)) && onGround)
		footsteps.Resume();
	else
		footsteps.Pause();
	float speedIncrease = 1;
	if (!onGround) speedIncrease += 0.5;
	if (isSpeed) speedIncrease += 0.5;
	player.SetPositionV(player.GetPositionV() + ((s + b + f) * speedIncrease));
	playerfront.SetPositionV(player.GetPositionV());
	playerside.SetPositionV(player.GetPositionV());
	playerbottom.SetPositionV(player.GetPositionV());

	
	

	sword.SetPositionV(player.GetPositionV()+player.GetDirectionV().Normalized()*90-playerside.GetDirectionV().Normalized()*65 + CVector(0, 22, 0));

	
	if (sword.IsAttacking()) {
		sword.Rotate(30);
	}
	else {
		sword.SetRotationV(-player.GetRotationV());
	}
	
	if (rightClick.GetPositionV().Distance(player.GetPositionV()) > PLAYER_REACH) {
		rightClick.SetSpeed(0);
	}
	if (leftClick.GetPositionV().Distance(player.GetPositionV()) > sword.GetRange()) {
		leftClick.SetSpeed(0);
	}
}

void CMyGame::NextWave()
{
	if (wave != 0)
		enemies.clear();
	wave++;
	if (sword.GetState() == 3 && sword.path == sword.BLACKHOLE) {
		blackHole.UnDelete();
	}
	// Dont do health increase first wave
	static float healthIncrease = 1;
	static int amountOfEnemies = 10;
	if (wave % 10 == 0) {
		if (wave <= 50) {
			healthIncrease -= 0.01;
			regularEnemy.SetWalkingSpeed(regularEnemy.GetWalkingSpeed() * 1.1);
			floatEnemy.SetWalkingSpeed(floatEnemy.GetWalkingSpeed() * 1.1);
			fireEnemy.SetWalkingSpeed(fireEnemy.GetWalkingSpeed() * 1.1);
			speedEnemy.SetWalkingSpeed(speedEnemy.GetWalkingSpeed() * 1.15);
		}
		if (wave == 10) {
			timeBetweenWaves = 7.f;
		}
	}
	regularEnemy.SetHealth(regularEnemy.GetHealth() * healthIncrease);
	floatEnemy.SetHealth(floatEnemy.GetHealth() * healthIncrease);
	fireEnemy.SetHealth(fireEnemy.GetHealth() * healthIncrease);
	speedEnemy.SetHealth(speedEnemy.GetHealth() * healthIncrease);
	if (wave == 1) healthIncrease = 1.05;
	vector<Enemy> possibleEnemies = { regularEnemy };
	if (wave >= 5) {
		possibleEnemies.push_back(fireEnemy);
		possibleEnemies.push_back(floatEnemy);
		possibleEnemies.push_back(regularEnemy);
	}
	if (wave >= 7) possibleEnemies.push_back(fireEnemy);
	if (wave >= 9) possibleEnemies.push_back(floatEnemy);
	if (wave >= 10) {
		possibleEnemies.push_back(speedEnemy);
		possibleEnemies.push_back(regularEnemy);
	}
	if (wave >= 15) {
		possibleEnemies.push_back(speedEnemy);
	}
	for (int i = 0; i < amountOfEnemies; i++) {
		if (i == amountOfEnemies/2 && wave % 25 == 0) {
			isBoss = true;
			bossHealth.SetHealth(100);
			Enemy* newBoss;
			if ((wave + 25) % 50 == 0) {
				newBoss = new Enemy(Listar);
			}
			else if (wave == 50) {
				newBoss = new Enemy(Melchor);
			}
			else {
				newBoss = new Enemy(Morgof);
			}
			newBoss->SetPosition(enemyStartingPos.x, enemyStartingPos.z);
			newBoss->SetStart(0);
			currentBossHealth = newBoss->GetHealth();
			enemiesInWave.push_back(newBoss);
		}
		Enemy* newEnemy = new Enemy(possibleEnemies[rand() % possibleEnemies.size()]);
		float startX = -350 + rand() % 700;
		newEnemy->SetPosition(startX + enemyStartingPos.x, enemyStartingPos.z);
		newEnemy->SetStart(startX);
		enemiesInWave.push_back(newEnemy);

	}
	amountOfEnemies *= 1.1;
	if (amountOfEnemies > 300) {
		amountOfEnemies = 300;
	}
	
}

void CMyGame::PlaceWave(long dt)
{
	static float timeBetweenEnemy = timeBetweenEnemySpawn;
	if (enemiesInWave.size() != 0) {
		timeBetweenEnemy -= dt / 1000.f;
		if (timeBetweenEnemy <= 0) {
			timeBetweenEnemy = timeBetweenEnemySpawn;
			enemies.push_back(enemiesInWave.back());
			string enemyType = enemies.back()->GetType();
			if (enemyType == "regular" || enemyType == "boss")
				enemies.back()->PlayAnimation(0, 41, 28, true);
			if (enemyType == "fire")
				enemies.back()->PlayAnimation(0, 41, 24, true);
			if (enemyType == "float") 
				enemies.back()->PlayAnimation(0, 121, 40, true);
			if (enemyType == "speed")
				enemies.back()->PlayAnimation(0, 12, 18, true);
			enemiesInWave.pop_back();
		}
	}
}

void CMyGame::GenerateWorld()
{
	vector<gridspace> generatedPath = PathGenerator::Get()->GenerateRandomPath(40, 4);
	vector<CVector> enemyPath;
	// Places 4 pieces of path for every path in the grid
	for (gridspace &pathInGrid : generatedPath) {
		float x = pathInGrid.i * 1000; float z = pathInGrid.j * 1000;
		CModel* newPath = new CModel(path);
		newPath->SetPosition(x - 250, z - 250);
		newPath->SetToFloor(0);
		paths.push_back(newPath); 
		newPath = new CModel(path);
		newPath->SetPosition(x + 250, z - 250);
		newPath->SetToFloor(0);
		paths.push_back(newPath);
		newPath = new CModel(path);
		newPath->SetPosition(x - 250, z + 250);
		newPath->SetToFloor(0);
		paths.push_back(newPath);
		newPath = new CModel(path);
		newPath->SetPosition(x + 250, z + 250);
		newPath->SetToFloor(0);
		paths.push_back(newPath);
		if (pathInGrid.type == 5) enemyStartingPos = CVector(x, 0, z-400);
		if (pathInGrid.type == 3)
			enemyPath.push_back(CVector(x, 0, z));
		if (pathInGrid.type == 6) {
			blackHole.SetPosition(x, z);
			castle.SetPosition(x, z + 2300);
			castle.SetRotation(180);
		}
	}
	// Spawns an enemy to test the path and store t=where other eneies have to turn
	Enemy::enemyPath = enemyPath;
	Enemy* testEnemy = new Enemy(speedEnemy);
	testEnemy->SetType("test");
	testEnemy->SetSpeed(2000);
	testEnemy->SetPosition(enemyStartingPos.x, enemyStartingPos.z);
	testEnemy->SetToFloor(0);
	testEnemy->Delete();
	enemies.push_back(testEnemy);


	// Loop through the path blocks and add the turrets
	for (CModel* path : paths) {
		// Checks if there is anything adjacent to the blocks
		vector<CVector> neighbourPos = {CVector(500, 0, 0), CVector(-500, 0, 0), CVector(0, 0, 500), CVector(0, 0, -500)};
		path->SetScale(7.f);
		for (CVector pos : neighbourPos) {
			bool empty = true;
			path->SetPositionV(path->GetPositionV() + pos);
			for (Turret* turret : turretList) {
				if (turret->HitTest(path)) {
					empty = false;
					break;
				}
			}
			if (empty) {
				for (CModel* checkPath : paths) {
					if (checkPath != path) {
						if (checkPath->HitTest(path)) {
							empty = false;
							break;
						}
					}
				}
			}
			// If it empty it adds the turret
			if (empty) {
				Turret* newTurret = new Turret(&particle, &turretsImg, &coin);
				newTurret->SetPosition(path->GetPositionV().x, path->GetPositionV().z);
				turretList.push_back(newTurret);
			}
			path->SetPositionV(path->GetPositionV() - pos);
			
		}
		path->SetScale(10.f);
	}
	// Removes the first and last four of the turrets, as these turrets would be placed near cave and castle
	for (int i = 0; i < 4; i++) {
		turretList.erase(turretList.begin());
		turretList.pop_back();
	}
	// Removes 50% of all turrets at random
	for (int i = 0; i < turretList.size(); i++) {
		if (rand() % 100 > 50) {
			turretList.erase(turretList.begin() + i);
		}
	}

	// Gets the grid and uses this to place the flowers, trees, grass and rocks
	vector<vector<gridspace>> selectedGrid = PathGenerator::Get()->GetGrid();
	for (vector<gridspace> row : selectedGrid) {
		for (gridspace& space : row) {
			float x = space.i * 1000; float z = space.j * 1000;
			if (space.type == 0 || space.type == 1) {
				int numberOfGrass = 5;
				int numberOfFlowers = 1;
				bool isTurret = false;
				// Places a trees if it is a 1
				if (space.type == 1) {
					CModel* newTrunk = new CModel(*trunks[rand() % trunks.size()]);
					newTrunk->SetPosition(-400 + x + rand() % 800, -400 + z + rand() % 800);
					newTrunk->SetRotation(rand() % 360);
					CModel* newCanopy = new CModel(*canopies[rand() % canopies.size()]);
					newCanopy->SetPositionV(newTrunk->GetPositionV() + CVector(0, newTrunk->GetTop() - 40, 0));
					newCanopy->SetRotation(newTrunk->GetRotation());
					nature.push_back(newTrunk);
					nature.push_back(newCanopy);
				}
				// Always tries to place 5 pieces of grass, if there are trees or turrets in the way, it deletes it
				for (int i = 0; i < numberOfGrass; i++) {
					CModel* newGrass = new CModel(*grass[rand() % grass.size()]);
					bool foundPos = true;
					newGrass->SetPosition(-400 + x + rand() % 800, -400 + z + rand() % 800);
					for (CModel* green : nature) {
						if (green->HitTest(newGrass)) {
							foundPos = false;
							break;
						}
					}
					for (Turret* turret : turretList) {
						if (turret->HitTest(newGrass)) {
							foundPos = false;
							if (!isTurret) {
								isTurret = true;
							}
							break;
						}
					}
					if (foundPos)
						nature.push_back(newGrass);
					else delete newGrass;
				}
				// if it is a 0, it tries to place a flower
				if (space.type == 0) {
					for (int i = 0; i < numberOfFlowers && !isTurret; i++) {
						CModel* newFlower = new CModel(*flowers[rand() % flowers.size()]);
						bool foundPos = true;
						newFlower->SetPosition(-400 + x + rand() % 800, -400 + z + rand() % 800);
						newFlower->SetRotation(rand() % 360);
						for (CModel* green : nature) {
							if (green->HitTest(newFlower)) {
								foundPos = false;
								break;
							}
						}
						if (foundPos)
							nature.push_back(newFlower);
						else delete newFlower;
					}
				}
			}
			// If it is a 4, it creates a rock
			if (space.type == 4) {
				CModel* newRock = new CModel(*rocks[2]);
				newRock->SetPosition(x, z);
				newRock->SetStatus(1);
				if (newRock->HitTest(enemyStartingPos)) newRock->SetPosition(x, z - 150);
				if (newRock->HitTest(&castle)) delete newRock;
				else nature.push_back(newRock);
			}
			// If it is a 5, it creates the starting cave structure
			if (space.type == 5) {
				CModel* newRock = new CModel(*rocks[0]);
				newRock->SetPosition(x - 50, 850, z - 350);
				newRock->SetScale(520.f);
				newRock->SetStatus(1);
				nature.push_back(newRock);
				newRock = new CModel(*rocks[1]);
				newRock->SetPosition(x + 830, z - 300);
				newRock->SetScale(320.f);
				newRock->SetStatus(1);
				nature.push_back(newRock);
				newRock = new CModel(*rocks[1]);
				newRock->Rotate(180);
				newRock->SetPosition(x - 830, z - 300);
				newRock->SetScale(320.f);
				newRock->SetStatus(1);
				nature.push_back(newRock);
				blackThing.SetPosition(x, z - 380);
				blackThing.SetRotation(90);
			}
			// If it is a 6, it places the poster showing controls, and moves the player in front of it.
			if (space.type == 6) {
				CModel* newPoster = new CModel(poster);
				newPoster->SetPosition(x + 1100, 480, z+900);
				newPoster->SetRotation(-90);
				nature.push_back(newPoster);
				player.SetPosition(x + 1100, z);
			}
		}
	}
}


 

//-----------------  Draw 2D overlay ----------------------
void CMyGame::OnDraw(CGraphics* g)
{
	if (IsMenuMode()) {
		mainMenu.Draw(g);
	}
	else {
		structHealthBar.Draw(g);
		timeBetweenWavesBar.Draw(g);
		font.SetSize(48); font.SetColor(CColor::Gray()); font.DrawText(50, Height - 50, "Wave: " + to_string(wave));
		font.SetSize(48); font.SetColor(CColor::Yellow()); font.DrawText(Width - 300, Height - 50, "Gold: " + to_string(gold));
		coin.Draw(g);
		if (isBoss) {
			font.SetSize(48); font.SetColor(CColor::Black()); font.DrawText(Width / 2 - bossNames.back().length() * 15, Height - 110, bossNames.back());
			bossHealth.Draw(g);

		}
		stringstream s; s << setfill('0') << setw(2) << (GetTime() - offset) / 60000 % 100 << ":" << setw(2) << (GetTime() - offset) / 1000 % 60 << "." << setw(2) << (GetTime() - offset) / 10 % 100;
		for (Turret* turret : turretList) {
			turret->DrawMenu(g);
		}
		sword.DrawMenu(g);
		if (sword.IsOpen() && sword.GetState() <= 2 && sword.path != sword.BASE) {
			swordCoin->Draw(g);
		}
		if (playState == MENU) {
			if (menuState == PAUSE) {
				pauseMenu.Draw(g);
			}
			else if (menuState == END) {
				footsteps.Pause();
				endMenu.Draw(g);
				if (won) {
					static string wonTime = s.str();
					font.SetSize(120); font.SetColor(CColor(239, 142, 50)); font.DrawText(Width / 2 - 240, 730, "YOU WON!");
					font.SetSize(50); font.SetColor(CColor(143, 64, 41)); font.DrawText(Width / 2 - 200, 470, "You took: " + wonTime);
				}
				else {
					font.SetSize(120); font.SetColor(CColor(239, 142, 50)); font.DrawText(Width / 2 - 300, 730, "YOU LOST:(");
					font.SetSize(50); font.SetColor(CColor(143, 64, 41)); font.DrawText(Width / 2 - 120, 470, "Try again");
				}
				font.SetSize(50); font.DrawText(Width / 2 - 350, 575, "You survived until wave " + to_string(wave) + "!");
			}
		}
	}
	if (playState == PLAYING) {
		cursor.Draw(g);
	}
	else {
		menuCursor.Draw(g);
	}
}



// ----------------   Draw 3D world -------------------------
void CMyGame::OnRender3D(CGraphics* g)
{	
	CameraControl(g);
	
	// ------- Draw your 3D Models here ----------
	
	floor.Draw(g);
	paths.Draw(g);

	castle.Draw(g);
	blackThing.Draw(g);
	// Stops drawing objects in nature when they are not in the screen of the player
	for (CModel* green : nature) {
		CVector pos(WorldToScreenCoordinate(green->GetPositionV()));
		if (pos.x > -2000 && pos.x < Width + 2000 && pos.y > -2000 && pos.y < Height + 2000) {
			green->Draw(g);
		}
	}
	sword.Draw(g);		

	if (sword.GetState() == 3 && sword.path == sword.BLACKHOLE && !blackHole.IsDeleted())
		blackHole.Draw(g);

	for (CModel* enemy : enemies) {
		if (!enemy->IsDeleted())
			enemy->Draw(g);
	}
	for (Turret* turret : turretList) {
		turret->Draw(g);
	}
	//ShowCoordinateSystem();
	//ShowBoundingBoxes();
}

void CMyGame::CameraControl(CGraphics* g)
{
	world.position.y = -100;    // move game world down for higher viewing position
	world.position.z = 1400;
	world.scale = 2.f;	       // scaling the game world

	// ------ Global Transformation Functions -----
	glScalef(world.scale, world.scale, world.scale);  // scale the game world	
	glTranslatef(world.position.x, world.position.y, world.position.z);  // translate game world
	glRotatef(world.rotation.x, 1, 0, 0);	// rotate game world around x-axis

	// -----  Player camera control ----------
	glRotatef(-player.GetRotation() + 90.0f, 0, 1, 0);
	glRotatef(player.GetRotationA().z, player.GetDirectionV().z, 0, -player.GetDirectionV().x);

	glTranslatef(0, -player.GetY(), 0);
	
	glTranslatef(-player.GetX(), 0, -player.GetZ());    // translate game world to player position 
	skydome.Draw(g);
	UpdateView();
	Light.Apply();
}


// called at the start of a new game - prepare menu here
void CMyGame::OnDisplayMenu()
{
	playState = MENU;
}

// called when Game Mode entered
void CMyGame::OnStartGame()
{
	OnStartLevel(1);
}


// called when Game is Over
void CMyGame::OnGameOver()
{
	
}

// one time termination code
void CMyGame::OnTerminate()
{
	
}

// -------    Keyboard Event Handling ------------

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	
	if (sym == SDLK_ESCAPE && !IsMenuMode()) {
		if (playState == MENU) {
			if (menuState != END) {
				for (Turret* turret : turretList) {
					if (turret->IsOpen()) {
						turret->CloseShop();
						playState = PLAYING;
					}
				}
				sword.CloseShop();
				playState = PLAYING;
				ResumeGame();
				music.SetVolume(100);
			}
		}
		else {
			playState = MENU;
			menuState = PAUSE;
			PauseGame();
			music.SetVolume(50);
		}
	}
	if (sym == SDLK_e && playState == PLAYING) {
		if (sword.GetState() > 1) {
			swordCoin->SetPosition(880, 270);
		}
		else {
			swordCoin->SetPosition(940, 270);
		}
		sword.OpenShop();
		playState = MENU;
		menuState = OTHER;
	}
	
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	
}

// -----  Mouse Events Handlers -------------

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
	if (playState == PLAYING) {
		ShowMouse();
		static CVector prev = CVector(Width / 2, Height / 2, 0);
		if (x <= 250 || x >= Width - 250 || y <= 250 || y >= Height - 250) {
			SetCursorPos(Width / 2, Height / 2);
			prev = CVector(Width / 2, Height / 2, 0);
			return;
		}
		CVector dis = CVector(x, y, 0) - prev;

		player.Rotate(0, -dis.GetX() / 30, 0);
		if ((player.GetRotationA().z >= -85 && dis.GetY() < 0) || player.GetRotationA().z <= 85 && dis.GetY() > 0) {
			player.Rotate(0, 0, dis.GetY() / 30);
		}
		playerside.SetRotation(player.GetRotation() + 90);
		playerside.SetDirection(playerside.GetRotation());
		player.SetDirectionV(player.GetRotationV());
		playerfront.SetDirection(player.GetRotation());

		prev = CVector(x, y, 0);
		HideMouse();
	}
	else {
		menuCursor.SetPosition(x, y);
	}
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{    
	if (IsMenuMode()) {
		for (int i = 0; i < mainButtons.size(); i++) {
			if (mainButtons[i]->HitTest(x, y)) {
				buttonSound.Play("button.wav");
				if (i == 0) {
					StartGame();
				}
				if (i == 1) {
					StopGame();
				}
			}
		}
		return;
	}
	if (sword.Attack() && playState == PLAYING) {
		swordSwing.Play("sword.wav", 1);
		swordSwing.SetVolume(70);
		leftClick.SetPositionV(player.GetPositionV() + CVector(0, 100, 0) + player.GetDirectionV().Normalized()*70);
		leftClick.SetRotationV(player.GetRotationA());
		leftClick.SetDirectionV(player.GetRotationV());
		leftClick.SetSpeed(7000);
	}
	else if (playState == MENU) {
		switch (menuState)
		{
		case PAUSE:
			for (int i = 0; i < pauseButtons.size(); i++) {
				if (pauseButtons[i]->HitTest(x, y)) {
					buttonSound.Play("button.wav");
					if (i == 0) {
						playState = PLAYING;
						ResumeGame();
						music.SetVolume(100);
					}
					else if (i == 1) {
						ResumeGame();
						StartGame();
					}
					else if (i == 2) {
						NewGame();
					}
				}
			}
			break;
		case END:
			for (int i = 0; i < endButtons.size(); i++) {
				if (endButtons[i]->HitTest(x, y)) {
					buttonSound.Play("button.wav");
					if (i == 0) {
						StartGame();
					}
					if (i == 1) {
						NewGame();
					}
				}
			}
			break;
		case OTHER:
			if (sword.IsOpen()) {
				bool buttonPressed = sword.HandleButtons(x, y, gold);
				if (buttonPressed) buttonSound.Play("button.wav");
				if (sword.GetState() == 3 && sword.path == sword.BLACKHOLE) {
					blackHole.UnDelete();
				}
				if (sword.GetState() == 3 && sword.path == sword.SHADOW) {
					isSpeed = true;
				}
				else {
					isSpeed = false;
				}
				if (!sword.IsOpen()) {
					playState = PLAYING;
				}
			}
			for (Turret* turret : turretList) {
				if (turret->IsOpen()) {
					int buttonPress = turret->HandleButtons(x, y);
					if (buttonPress != -1) {
						buttonSound.Play("button.wav");
						if (turret->GetState() == 0) {
							if (buttonPress < 5) {
								selectedTurret = static_cast<SelectedTurret>(buttonPress);
								turret->OpenShop(selectedTurret);
							}
							else {
								if (turret->GetGold(selectedTurret) <= gold) {
									gold -= turret->GetGold(selectedTurret);
									turret->Upgrade();
									if (selectedTurret == ARCHER) {
										ArcherTurret* newTurret = new ArcherTurret(&particle, &archerBase, &archerTurret, &archerImg, &coin);
										newTurret->SetPositionV(turret->GetPosition());
										turretList.push_back(newTurret);
									}
									else if (selectedTurret == ZAP) {
										ZapTurret* newTurret = new ZapTurret(&particle, &zapBase, &zapTurret, &zapImg, &coin);
										newTurret->SetPositionV(turret->GetPosition());
										turretList.push_back(newTurret);
									}
									else if (selectedTurret == ICE) {
										IceTurret* newTurret = new IceTurret(&particle, &iceBase, &iceTurret, &iceImg, &coin);
										newTurret->SetPositionV(turret->GetPosition());
										turretList.push_back(newTurret);
									}
									else if (selectedTurret == FIRE) {
										FireTurret* newTurret = new FireTurret(&particle, &fireBase, &fireTurret, &fireImg, &coin);
										newTurret->SetPositionV(turret->GetPosition());
										turretList.push_back(newTurret);
									}
									else if (selectedTurret == DRAGON) {
										DragonTurret* newTurret = new DragonTurret(&particle, &dragonBase, &dragonTurret, &dragonImg, &coin);
										newTurret->SetPositionV(turret->GetPosition());
										turretList.push_back(newTurret);
									}
									turretList.erase(find(turretList.begin(), turretList.end(), turret));
									playState = PLAYING;
								}
							}
						}
						else {
							if (buttonPress == 0) {
								if (turret->GetGold(selectedTurret) <= gold && turret->GetState() != 3) {
									gold -= turret->GetGold();
									turret->Upgrade();
								}
								playState = PLAYING;
								turret->CloseShop();
							}
							else if (buttonPress == 1) {
								gold += turret->GetTotalGold() * 0.6;
								Turret* newTurret = new Turret(&particle, &turretsImg, &coin);
								newTurret->SetPositionV(turret->GetPosition());
								turretList.push_back(newTurret);
								turretList.erase(find(turretList.begin(), turretList.end(), turret));
								playState = PLAYING;
							}
							else if (buttonPress == 2) {
								turret->SetPath(1);
							}
							else if (buttonPress == 3) {
								turret->SetPath(2);
							}
						}
						break;
					}
				}
			}
			break;
		}
	}
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
	
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
	if (playState == PLAYING) {
		rightClick.SetPositionV(player.GetPositionV() + CVector(0, 100, 0));
		rightClick.SetRotationV(player.GetRotationA());
		rightClick.SetDirectionV(player.GetRotationV());
		rightClick.SetSpeed(6000);
	}
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
