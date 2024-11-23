
#include "GameApp.h"
#include "MyGame.h"

// export game to be accessed from other classes
extern CMyGame game;

// make these global
CMyGame game;
CGameApp app;

int main(int argc, char* argv[])
{
	
	app.OpenWindow(1920, 1080, "Tower Defense");
    app.OpenFullScreen();

	app.SetFPS(60);
	app.Run(&game);
	return(0);
}
