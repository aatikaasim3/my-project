int main()
{

	RenderWindow window(VideoMode(screen_x, screen_y), "Tumble-POP", Style::Resize);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	int playerScore = 0;

	Texture livechecktexture;
	if (!livechecktexture.loadFromFile("./Data/live.png"))
	{
		cerr << "Failed to load playerselection.png. Make sure the file exists in the working directory!" << endl;
		return -1;
	}

	Sprite livechecksprite;
	livechecksprite.setTexture(livechecktexture);
	livechecksprite.setScale(1.8, 1.8);
	livechecksprite.setPosition(0, 0);

	int current = 0;

	Texture menubgtexture;
	if (!menubgtexture.loadFromFile("./Data/menu_bg1.png"))
	{
		cerr << "Failed to load menu_bg1.png. Make sure the file exists in the working directory!" << endl;
		return -1;
	}

	Sprite menubgsprite;
	menubgsprite.setTexture(menubgtexture);
	menubgsprite.setScale(
		float(screen_x) / menubgtexture.getSize().x,
		float(screen_y) / menubgtexture.getSize().y);

	Texture instbgtexture;
	if (!instbgtexture.loadFromFile("./Data/inst.png"))
	{
		cerr << "Failed to load inst.png. Make sure the file exists in the working directory!" << endl;
		return -1;
	}

	Sprite instbgsprite;
	instbgsprite.setTexture(instbgtexture);
	instbgsprite.setScale(
		float(screen_x) / instbgtexture.getSize().x,
		float(screen_y) / instbgtexture.getSize().y);

	Texture inst_Pbgtexture;
	if (!inst_Pbgtexture.loadFromFile("./Data/inst_P.png"))
	{
		cerr << "Failed to load inst_P.png. Make sure the file exists in the working directory!" << endl;
		return -1;
	}

	Sprite inst_Pbgsprite;
	inst_Pbgsprite.setTexture(inst_Pbgtexture);
	inst_Pbgsprite.setScale(
		float(screen_x) / inst_Pbgtexture.getSize().x,
		float(screen_y) / inst_Pbgtexture.getSize().y);

	Texture pausebgtexture;
	if (!pausebgtexture.loadFromFile("./Data/pause.png"))
	{
		cerr << "Failed to load pause.png. Make sure the file exists in the working directory!" << endl;
		return -1;
	}

	Sprite pausebgsprite;
	pausebgsprite.setTexture(pausebgtexture);
	pausebgsprite.setScale(1, 1);
	pausebgsprite.setPosition(330, 250);

	Font font;
	if (!font.loadFromFile("./Data/Aileron.ttf"))
	{
		cout << "Failed to load arial.ttf\n";
		return -1;
	}

	Texture selectbgtexture;
	if (!selectbgtexture.loadFromFile("./Data/playerselection.png"))
	{
		cerr << "Failed to load playerselection.png. Make sure the file exists in the working directory!" << endl;
		return -1;
	}

	Sprite selectbgsprite;
	selectbgsprite.setTexture(selectbgtexture);
	selectbgsprite.setScale(
		float(screen_x) / selectbgtexture.getSize().x,
		float(screen_y) / selectbgtexture.getSize().y);

	Texture leaderboardtexture;
	if (!leaderboardtexture.loadFromFile("./Data/leaderboard.png"))
	{
		cerr << "Failed to load leaderboard.png. Make sure the file exists in the working directory!" << endl;
		return -1;
	}

	Sprite leaderboardsprite;
	leaderboardsprite.setTexture(leaderboardtexture);
	leaderboardsprite.setScale(
		float(screen_x) / leaderboardtexture.getSize().x,
		float(screen_y) / leaderboardtexture.getSize().y);

	// for player selection yellow

	Texture yellowbgtexture;
	if (!yellowbgtexture.loadFromFile("./Data/yellow.png"))
	{
		cerr << "Failed to load yellow.png. Make sure the file exists in the working directory!" << endl;
		return -1;
	}

	Sprite yellowbgsprite;
	yellowbgsprite.setTexture(yellowbgtexture);
	yellowbgsprite.setScale(3, 3);
	yellowbgsprite.setPosition(220, 400);

	// for player selection green
	Texture greenbgtexture;
	if (!greenbgtexture.loadFromFile("./Data/green.png"))
	{
		cerr << "Failed to load green.png. Make sure the file exists in the working directory!" << endl;
		return -1;
	}

	Sprite greenbgsprite;
	greenbgsprite.setTexture(greenbgtexture);
	greenbgsprite.setScale(3, 3);
	greenbgsprite.setPosition(600, 410);

	window.clear();
	window.draw(menubgsprite); // background first
							   // Menu options text
	const int numButtons = 4;
	const int numButtons_P = 4;

	string labels[numButtons] = {"START GAME", "INSTRUCTIONS", "LEADERBOARD", "QUIT"};
	string label[numButtons_P] = {"RESUME", "RESTART", "MAIN MENU", "INSTRUCTION"};

	Text buttonText[numButtons];
	Text buttonText_P[numButtons_P];

	for (int i = 0; i < numButtons; i++)
	{

		buttonText[i].setFillColor(Color(255, 140, 0)); // darkblue
		// Button label
		buttonText[i].setFont(font);
		buttonText[i].setString(labels[i]);
		buttonText[i].setCharacterSize(40);
		buttonText[i].setFillColor(Color(255, 140, 0));
		// buttonText[i].setPosition(550, 330 + i * 120);
	}

	for (int i = 0; i < numButtons_P; i++)
	{

		buttonText_P[i].setFillColor(Color(255, 140, 0)); // darkblue
		// Button label
		buttonText_P[i].setFont(font);
		buttonText_P[i].setString(label[i]);
		buttonText_P[i].setCharacterSize(20);
		buttonText_P[i].setFillColor(Color(255, 140, 0));
		// buttonText[i].setPosition(550, 330 + i * 120);
	}
	// for MAIN MENU BUTTON
	buttonText[0].setPosition(450, 310 + 0 * 120);
	buttonText[1].setPosition(430, 310 + 1 * 120);
	buttonText[2].setPosition(430, 310 + 2 * 120);
	buttonText[3].setPosition(520, 310 + 3 * 120);

	// FOR PAUSE SYSTEM BUTTONS
	buttonText_P[0].setPosition(530, 365);
	buttonText_P[1].setPosition(527, 330 + 1 * 120);
	buttonText_P[2].setPosition(515, 285 + 2 * 120);
	buttonText_P[3].setPosition(505, 360 + 2 * 120);

	int selectedButton = 0;
	buttonText[selectedButton].setFillColor(Color(100, 100, 255)); // glow for selected

	int selectedButton_P = 0;
	buttonText_P[selectedButton_P].setFillColor(Color(100, 100, 255));

	int selectedplayer = 0, numplayers = 2;
	if (selectedplayer == 0)
	{
		yellowbgsprite.setScale(4, 4);
	}

	// level specifics
	const int cell_size = 64;
	const int height = 14;
	const int width = 18;
	char **lvl;

	// level and background textures and sprites
	Texture bgTex;
	Sprite bgSprite;
	Texture blockTexture;
	Sprite blockSprite;

	Texture lvl2BgTex;
	Texture lvl2BlockTex;
	Texture lvl2SBlockTex;
	Sprite lvl2SBlockSprite;
	Texture lvl2SIBlockTex;
	Sprite lvl2SIBlockSprite;

	int player;

	bgTex.loadFromFile("./Data/bg.png");
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0, 0);

	lvl2BgTex.loadFromFile("./Data/bg2.png");

	blockTexture.loadFromFile("./Data/block1.png");
	blockSprite.setTexture(blockTexture);

	lvl2BlockTex.loadFromFile("./Data/block41.png");
	lvl2SBlockTex.loadFromFile("./Data/blockt.png");
	lvl2SIBlockTex.loadFromFile("./Data/blockti.png");

	// Music initialisation
	Music lvlMusic;

	lvlMusic.openFromFile("./Data/mus.ogg");
	lvlMusic.setVolume(10);
	lvlMusic.play();
	lvlMusic.setLoop(true);

	// player data
	float player_x = 500;
	float player_y = 150;

	int playerLives = 3;
	bool playerDead = false;
	int playerRespawnTimer = 0;

	float playerDefaultX = 500;
	float playerDefaultY = 150;

	const int capLimitLvl1 = 3;
	int capLimit = capLimitLvl1;

	// stack arrays
	int capEnemyID[capLimit];	// stores index (0–7 for ghost, 0–3 for skeleton)
	int capEnemyType[capLimit]; // true = ghost, false = skeleton
	int capTop = -1;			// top of stack (-1 = empty)

	float speed = 5;

	const float jumpStrength = -20; // Initial jump velocity
	const float gravity = 1;		// Gravity acceleration

	bool isJumping = false; // Track if jumping

	int playerRunIndex = 0;
	float playerRunTimer = 0;

	bool up_collide = false;
	bool left_collide = false;
	bool right_collide = false;

	Texture playerTextureArr[2];
	Sprite playerSpriteArr[2];

	Texture greenShootBeamTexture;
	Texture yellowShootBeamTexture;
	Sprite shootBeamSprite;
	Texture GhostTexture;
	Sprite GhostSprite;

	bool onGround = false;

	float offset_x = 0;
	float offset_y = 0;
	int velocityY = 0;

	float terminal_Velocity = 20;

	int PlayerHeight = 102;
	int PlayerWidth = 96;

	bool up_button = false;

	char top_left = '\0';
	char top_right = '\0';
	char top_mid = '\0';

	char left_mid = '\0';
	char right_mid = '\0';

	char bottom_left = '\0';
	char bottom_right = '\0';
	char bottom_mid = '\0';

	char bottom_left_down = '\0';
	char bottom_right_down = '\0';
	char bottom_mid_down = '\0';

	char top_right_up = '\0';
	char top_mid_up = '\0';
	char top_left_up = '\0';

	// for green player
	playerTextureArr[1].loadFromFile("./Data/greenplayer.png");
	playerSpriteArr[1].setTexture(playerTextureArr[1]);
	playerSpriteArr[1].setScale(float(PlayerWidth) / playerTextureArr[1].getSize().x, float(PlayerHeight) / playerTextureArr[1].getSize().y);
	playerSpriteArr[1].setPosition(player_x, player_y);

	// for yellow player
	playerTextureArr[0].loadFromFile("./Data/yellowplayer.png");
	playerSpriteArr[0].setTexture(playerTextureArr[0]);
	playerSpriteArr[0].setScale(float(PlayerWidth) / playerTextureArr[0].getSize().x, float(PlayerHeight) / playerTextureArr[0].getSize().y);
	playerSpriteArr[0].setPosition(player_x, player_y);

	Texture greenPlayerRunFrames[4];
	greenPlayerRunFrames[0].loadFromFile("./Data/greenplayer2.png");
	greenPlayerRunFrames[1].loadFromFile("./Data/greenplayer3.png");
	greenPlayerRunFrames[2].loadFromFile("./Data/greenplayer4.png");
	greenPlayerRunFrames[3].loadFromFile("./Data/greenplayer5.png");

	Texture yellowPlayerRunFrames[4];
	yellowPlayerRunFrames[0].loadFromFile("./Data/yellowplayer2.png");
	yellowPlayerRunFrames[1].loadFromFile("./Data/yellowplayer3.png");
	yellowPlayerRunFrames[2].loadFromFile("./Data/yellowplayer4.png");
	yellowPlayerRunFrames[3].loadFromFile("./Data/yellowplayer5.png");

	Texture ghostThrownFrames[8];
	ghostThrownFrames[0].loadFromFile("./Data/ghost20.png");
	ghostThrownFrames[1].loadFromFile("./Data/ghost21.png");
	ghostThrownFrames[2].loadFromFile("./Data/ghost22.png");
	ghostThrownFrames[3].loadFromFile("./Data/ghost23.png");
	ghostThrownFrames[4].loadFromFile("./Data/ghost24.png");
	ghostThrownFrames[5].loadFromFile("./Data/ghost25.png");
	ghostThrownFrames[6].loadFromFile("./Data/ghost26.png");
	ghostThrownFrames[7].loadFromFile("./Data/ghost27.png");

	Texture skThrownFrames[9];
	skThrownFrames[0].loadFromFile("./Data/skeleton25.png");
	skThrownFrames[1].loadFromFile("./Data/skeleton26.png");
	skThrownFrames[2].loadFromFile("./Data/skeleton27.png");
	skThrownFrames[3].loadFromFile("./Data/skeleton28.png");
	skThrownFrames[4].loadFromFile("./Data/skeleton29.png");
	skThrownFrames[5].loadFromFile("./Data/skeleton30.png");
	skThrownFrames[6].loadFromFile("./Data/skeleton31.png");
	skThrownFrames[7].loadFromFile("./Data/skeleton32.png");
	skThrownFrames[8].loadFromFile("./Data/skeleton33.png");

	Text livesText; //****************************************ADD THIS FOR LIFE SYSTEM TEXT*************************
	Text scoreText;

	int shootBeamWidth = (selectedplayer == 0 ? 216 : 180);
	int shootBeamHeight = 71;
	bool shootBeamShow = false;

	greenShootBeamTexture.loadFromFile("./Data/shootBeam1.png");
	yellowShootBeamTexture.loadFromFile("./Data/splash3.png");
	shootBeamSprite.setTexture(greenShootBeamTexture);
	shootBeamSprite.setPosition(player_x - shootBeamWidth, player_y + 43);

	int ghostCount = 8;
	int skeletonCount = 4;
	int invisibleCount = 0;
	int chelnovCount = 0;

	Texture ghostTexture;
	Texture skeletonTexture;
	Texture inivisibleTexture;
	Texture chelnovTexture;

	Sprite ghostSpriteArr[20];
	Sprite skeletonSpriteArr[20];
	Sprite invisibleSpriteArr[20];
	Sprite chelnovSpriteArr[20];

	float ghostX[20];
	float ghostY[20];
	float skeletonX[20];
	float skeletonY[20];

	int ghostThrowAnimIndex[20] = {0};
	float ghostThrowAnimTimer[20] = {0};

	int skThrownAnimIndex[20] = {0};
	float skThrownAnimTimer[20] = {0};

	float ghostSpeed = 0.8f;

	bool ghostRight[20];			  // ghost direction
	bool ghostCaptured[20] = {false}; // ghost is inside vacuum
	bool ghostDead[20] = {false};	  // ghost killed after shooting
	bool ghostInVac[20] = {false};
	bool ghostOnGround[20] = {true};
	int ghostThrownVerDir[20] = {0};

	int playerCapturedCount = 0; // how many enemies inside bag

	bool skeletonRight[20];
	bool skeletonCaptured[20] = {false};
	bool skeletonDead[20] = {false};
	bool skeletonInVac[20] = {false};

	ghostTexture.loadFromFile("./Data/ghost1.png");
	skeletonTexture.loadFromFile("./Data/skeleton1.png");
	inivisibleTexture.loadFromFile("./Data/invisible1.png");
	chelnovTexture.loadFromFile("./Data/chelnov1.png");

	float ghostWidth = ghostTexture.getSize().x * 1.5;
	float ghostHeight = ghostTexture.getSize().y * 1.1;

	float skWidth = skeletonTexture.getSize().x * 1.0;
	float skHeight = skeletonTexture.getSize().y * 1.1;

	int ghostRandom[20] = {0};
	int ghostVelocityY[20] = {0};

	int skeletonVelocityY[20];
	bool skeletonOnGround[20];
	int skeletonCooldown[20] = {0};
	int skeletonThrownVerDir[20] = {0};

	bool ghostThrown[20] = {false};
	int ghostThrowDir[20] = {0}; // 0 none, 1 up, 2 right, 3 down, 4 left

	bool skeletonThrown[20] = {false};
	int skeletonThrowDir[20] = {0};

	float invisibleX[20];
	float invisibleY[20];
	bool invisibleRight[20];
	bool invisibleInvisible[20];
	int invisibleTimer[20];
	int invisibleVelocityY[20] = {0};
	bool invisibleOnGround[] = {false};
	int invisibleCoolDown[] = {0};
	bool invisibleThrown[] = {false};
	bool invisibleCaptured[] = {false};
	bool invisibleDead[] = {false};
	bool invisibleInVac[] = {false};

	// CHELNOV DATA
	float chelnovX[20];
	float chelnovY[20];
	bool chelnovRight[20];
	int chelnovCooldown[20];
	bool chelnovShooting[20];

	float chelnovBulletX[20];
	float chelnovBulletY[20];
	bool chelnovBulletActive[20];
	int chelnovBulletDir[20];

	// creating level array
	lvl = new char *[height];
	for (int i = 0; i < height; i += 1)
	{
		lvl[i] = new char[width];
	}

	for (int i = 0; i < width; i++)
	{
		// TOP platform

		if (i == 0 || i == 1 || i == 17 || i == 16)
		{
			lvl[4][i] = ' ';
		}
		else
			lvl[4][i] = '#';

		// Second platform

		if (i == 3 || i == 4 || i == 5 || i == 10 || i == 11 || i == 12)
			lvl[7][i] = ' ';
		else
			lvl[7][i] = '#';

		// Third platform

		if (i == 5 || i == 7 || i == 6 || i == 11 || i == 12 || i == 13)
			lvl[10][i] = ' ';
		else
			lvl[10][i] = '#';

		// Last platform

		lvl[13][i] = '#';
	}

	int prev;

	initGhosts(ghostTexture, ghostSpriteArr, ghostX, ghostY, ghostRight, ghostCount, ghostWidth, ghostHeight, ghostOnGround);
	initSkeletons(skeletonTexture, skeletonSpriteArr,
				  skeletonX, skeletonY, skeletonRight,
				  skeletonVelocityY, skeletonOnGround,
				  skeletonCooldown,
				  skeletonCount, skWidth, skHeight);

	bool facing_right = false; // player facing direction
	bool playerIsRunning = false;
	float pX = 0;

	float movX = 0;
	Event ev;
	// main loop
	while (window.isOpen())
	{

		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
			{
				window.close();
			}

			if (current == 0)
			{
				if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::Down)
				{
					buttonText[selectedButton].setFillColor(Color(255, 140, 0));
					selectedButton = (selectedButton + 1) % numButtons;
					buttonText[selectedButton].setFillColor(Color(0, 0, 255));
				}
				else if (Keyboard::isKeyPressed(Keyboard::Up))
				{
					buttonText[selectedButton].setFillColor(Color(255, 140, 0));
					selectedButton = (selectedButton + numButtons - 1) % numButtons;
					buttonText[selectedButton].setFillColor(Color(0, 0, 255));
				}
				else if (Keyboard::isKeyPressed(Keyboard::Enter))
				{
					if (selectedButton == 0)
						current = 2;
					else if (selectedButton == 1)
						current = 1;
					else if (selectedButton == 2)
						current = 5;
					else if (selectedButton == 3)
						window.close();
				}
			}
			else if (current == 1)
			{
				if (Keyboard::isKeyPressed(Keyboard::Escape))
				{
					current = 0;
				}
			}
			else if (current == 2)
			{
				if (Keyboard::isKeyPressed(Keyboard::Right))
				{
					greenbgsprite.setScale(4, 4);
					selectedplayer = 1;
					shootBeamWidth = 180;
					shootBeamHeight = 71;
					yellowbgsprite.setScale(3, 3);
				}
				else if (Keyboard::isKeyPressed(Keyboard::Left))
				{
					yellowbgsprite.setScale(4, 4);
					selectedplayer = 0;
					shootBeamWidth = 216;
					shootBeamHeight = 100;
					greenbgsprite.setScale(3, 3);
				}
			}
			else if (current == 6)
			{
				if (ev.key.code == Keyboard::Down)
				{
					buttonText_P[selectedButton_P].setFillColor(Color(255, 140, 0)); // reset color
					selectedButton_P = (selectedButton_P + 1) % numButtons_P;
					buttonText_P[selectedButton_P].setFillColor(Color(0, 0, 255)); // glow
				}
				else if (ev.key.code == Keyboard::Up)
				{
					buttonText_P[selectedButton_P].setFillColor(Color(255, 140, 0));
					selectedButton_P = (selectedButton_P + numButtons_P - 1) % numButtons_P;
					buttonText_P[selectedButton_P].setFillColor(Color(0, 0, 255));
				}
				else if (ev.key.code == Keyboard::Enter)
				{
					if (selectedButton_P == 0)
						current = 3;
					else if (selectedButton_P == 1)
					{
						current = 3;
						for (int i = 0; i < ghostCount; i++)
						{
							ghostCaptured[i] = false;
							ghostDead[i] = false;
							ghostInVac[i] = false;
							ghostThrown[i] = false;
						}
						for (int i = 0; i < skeletonCount; i++)
						{
							skeletonCaptured[i] = false;
							skeletonDead[i] = false;
							skeletonInVac[i] = false;
							skeletonThrown[i] = false;
						}

						initGhosts(ghostTexture, ghostSpriteArr, ghostX, ghostY, ghostRight, ghostCount, ghostWidth, ghostHeight, ghostOnGround);
						initSkeletons(skeletonTexture, skeletonSpriteArr,
									  skeletonX, skeletonY, skeletonRight,
									  skeletonVelocityY, skeletonOnGround,
									  skeletonCooldown,
									  skeletonCount, skWidth, skHeight);

						playerScore = 0;
						playerLives = 3;
					}
					else if (selectedButton_P == 2)
						current = 0;
					else if (selectedButton_P == 3)
					{
						current = 1;
						prev = 6;
					}
				}
			}

			else if (current == 5)
			{
				if (ev.key.code == Keyboard::Escape)
				{
					current = 0;
				}
			}
		}

		window.clear();

		if (current == 0)
		{
			window.draw(menubgsprite);
			// window.draw(buttonbgsprite);
			for (int i = 0; i < numButtons; i++)
			{
				// window.draw(buttons[i]);
				window.draw(buttonText[i]);
			}
		}
		else if (current == 1)
		{
			if (prev == 6)
				window.draw(inst_Pbgsprite);
			else
				window.draw(instbgsprite);
			if (prev == 6 && ev.type == Event::KeyPressed && ev.key.code == Keyboard::E)
			{
				current = 6;
			}
			else if (prev == 6 && (ev.type == Event::KeyPressed && ev.key.code == Keyboard::Escape))
			{
				window.draw(menubgsprite);

				current = 0; // ← Go back to main menu
			}
		}
		else if (current == 2)
		{
			window.draw(selectbgsprite);
			window.draw(yellowbgsprite);
			window.draw(greenbgsprite);

			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				window.draw(menubgsprite);
				current = 0;
			}
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				current = 3;
				ghostCount = 8;
				skeletonCount = 4;
			}
		}

		else if (current == 6)
		{
			
			display_level(window, lvl, bgTex, bgSprite, blockTexture, blockSprite, lvl2SBlockSprite, lvl2SIBlockSprite, height, width, cell_size);
			window.draw(pausebgsprite);

			for (int i = 0; i < numButtons_P; i++)
			{
				window.draw(buttonText_P[i]);
			}
		}
		else if (current == 5)
		{
			window.draw(leaderboardsprite);
			window.draw(scoreText);

			if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::Escape)
			{
				window.draw(menubgsprite);

				current = 0; // ← Go back to main menu
			}
		}
		else if (current == 3)
		{
			// *******************************************************************************Level 1**************************************************
			if (current == 3 && ev.type == Event::KeyPressed && ev.key.code == Keyboard::Escape)
			{
				current = 6;
			}

			movX = 0;
			playerIsRunning = false;

			updateGhosts(lvl, ghostSpriteArr, ghostTexture, ghostX, ghostY, ghostRight, ghostThrown, ghostRandom,
						 ghostCount, ghostSpeed, ghostWidth, ghostHeight, cell_size, screen_x, ghostDead, ghostCaptured, ghostInVac);

			updateSkeletons(lvl,
							skeletonSpriteArr, skeletonTexture, skeletonX, skeletonY,
							skeletonRight, skeletonThrown, skeletonVelocityY,
							skeletonOnGround, skeletonCooldown,
							skeletonCount,
							1.0f, jumpStrength, gravity,
							terminal_Velocity,
							cell_size, screen_x,
							skWidth, skHeight, skeletonDead, skeletonCaptured, skeletonInVac);

			int prevPlayerLives = playerLives;

			updatePlayerLifeSystem(
				player_x, player_y, facing_right, PlayerWidth, PlayerHeight, playerDefaultX, playerDefaultY, playerLives,
				playerDead, playerRespawnTimer, ghostX, ghostY, ghostRight, ghostDead, ghostCaptured, ghostInVac, ghostThrown, ghostCount,
				ghostWidth, ghostHeight, skeletonX, skeletonY, skeletonRight, skeletonDead, skeletonCaptured, skeletonInVac,
				skeletonThrown, skeletonCount, skWidth, skHeight, playerScore);

			if (playerLives < prevPlayerLives)
			{
				for (int i = capLimit - 1; i >= 0; i--)
				{
					int newThrowDir = facing_right ? 2 : 4;
					if (capTop >= 0)
					{
						int id = capEnemyID[capTop];
						int type = capEnemyType[capTop];

						if (type == 1)
							ghostThrowDir[id] = newThrowDir;
						else if (type == 2)
							skeletonThrowDir[id] = newThrowDir;
					}

					// now actually throw the enemy
					throwLastCapturedEnemy(
						player_x, player_y, PlayerWidth, PlayerHeight, facing_right,
						ghostX, ghostY, ghostThrown, ghostThrowDir, ghostCaptured, ghostOnGround, ghostVelocityY,
						ghostThrowAnimIndex, ghostThrowAnimTimer, ghostThrownVerDir, ghostHeight, ghostWidth,
						skeletonX, skeletonY, skeletonThrown, skeletonThrowDir, skeletonCaptured, skeletonOnGround,
						skeletonVelocityY, skeletonThrownVerDir, skWidth, skHeight, capEnemyID, capEnemyType, capTop, playerCapturedCount);
				}
			}

			// FOR PLAYER LIVE TEXT

			livesText.setFont(font);
			livesText.setCharacterSize(35);
			livesText.setFillColor(sf::Color::White);
			livesText.setPosition(70, 10);
			livesText.setString("X" + std::to_string(playerLives));

			// FOR PLAYERSCORE TEXT

			if (playerScore < 0)
			{
				playerScore = 0;
			}
			scoreText.setFont(font);
			scoreText.setCharacterSize(35);
			scoreText.setFillColor(sf::Color::White);
			scoreText.setPosition(980, 10);
			scoreText.setString("X" + std::to_string(playerScore));

			if (!playerDead)
			{
				if (Keyboard::isKeyPressed(Keyboard::Left))
				{
					if (facing_right) // changing player direction
					{
						playerSpriteArr[selectedplayer].setScale(float(PlayerWidth) / playerTextureArr[selectedplayer].getSize().x, float(PlayerHeight) / playerTextureArr[selectedplayer].getSize().y);
						player_x -= PlayerWidth;
						facing_right = false;
					}
					else if (!(player_x <= 0)) // handeling left movement
					{
						playerIsRunning = true;
						movX = -(selectedplayer == 0 ? speed : speed * 1.5);
						top_left = lvl[int(player_y) / cell_size][int(player_x + movX) / cell_size];
						left_mid = lvl[int(player_y + PlayerHeight / 2) / cell_size][int(player_x + movX) / cell_size];
						bottom_left = lvl[int(player_y + PlayerHeight) / cell_size][int(player_x + movX) / cell_size];
						if (!(top_left == '#' || left_mid == '#' || bottom_left == '#'))
						{
							player_x += movX;
						}
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Right))
				{
					if (!facing_right) // changing player direction
					{
						playerSpriteArr[selectedplayer].setScale(-float(PlayerWidth) / playerTextureArr[selectedplayer].getSize().x, float(PlayerHeight) / playerTextureArr[selectedplayer].getSize().y);
						player_x += PlayerWidth;
						facing_right = true;
					}
					else if (!(player_x >= screen_x)) // handeling right movement
					{
						playerIsRunning = true;
						movX = (selectedplayer == 0 ? speed : speed * 1.5);
						top_right = lvl[int(player_y) / cell_size][int(player_x + movX) / cell_size];
						right_mid = lvl[int(player_y + PlayerHeight / 2) / cell_size][int(player_x + movX) / cell_size];
						bottom_right = lvl[int(player_y + PlayerHeight) / cell_size][int(player_x + movX) / cell_size];
						if (!(top_right == '#' || right_mid == '#' || bottom_right == '#'))
						{
							player_x += movX;
						}
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Up)) // ADDED THE JUMP FUNCTION
				{
					if (onGround)
					{
						velocityY = jumpStrength;
						onGround = false;
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Down))
				{
					if (onGround && !(player_y + PlayerHeight >= 896))
					{
						player_y += 64;
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Space))
				{
					shootBeamShow = true;
				}
				else
				{
					shootBeamShow = false;
				}

				static bool zWasPressed = false;

				bool zNow = Keyboard::isKeyPressed(Keyboard::Z);

				if (zNow && !zWasPressed)
				{
					int newThrowDir = 0; // default forward

					if (Keyboard::isKeyPressed(Keyboard::W))
						newThrowDir = 1;
					else if (Keyboard::isKeyPressed(Keyboard::D))
						newThrowDir = 2;
					else if (Keyboard::isKeyPressed(Keyboard::S))
						newThrowDir = 3;
					else if (Keyboard::isKeyPressed(Keyboard::A))
						newThrowDir = 4;
					else
						newThrowDir = facing_right ? 2 : 4;
					if (capTop >= 0)
					{
						int id = capEnemyID[capTop];
						int type = capEnemyType[capTop];

						if (type == 1)
							ghostThrowDir[id] = newThrowDir;
						else if (type == 2)
							skeletonThrowDir[id] = newThrowDir;
					}

					// now actually throw the enemy
					throwLastCapturedEnemy(
						player_x, player_y, PlayerWidth, PlayerHeight, facing_right,
						ghostX, ghostY, ghostThrown, ghostThrowDir, ghostCaptured, ghostOnGround, ghostVelocityY,
						ghostThrowAnimIndex, ghostThrowAnimTimer, ghostThrownVerDir, ghostHeight, ghostWidth,
						skeletonX, skeletonY, skeletonThrown, skeletonThrowDir, skeletonCaptured, skeletonOnGround,
						skeletonVelocityY, skeletonThrownVerDir, skWidth, skHeight, capEnemyID, capEnemyType, capTop,
						playerCapturedCount);
				}

				zWasPressed = zNow;
			}

			if (playerCapturedCount >= capLimit)
			{
				for (int i = capLimit - 1; i >= 0; i--)
				{
					int newThrowDir = facing_right ? 2 : 4;
					if (capTop >= 0)
					{
						int id = capEnemyID[capTop];
						int type = capEnemyType[capTop];

						if (type == 1)
							ghostThrowDir[id] = newThrowDir;
						else if (type == 2)
							skeletonThrowDir[id] = newThrowDir;
					}

					// now actually throw the enemy
					throwLastCapturedEnemy(
						player_x, player_y, PlayerWidth, PlayerHeight, facing_right,
						ghostX, ghostY, ghostThrown, ghostThrowDir, ghostCaptured, ghostOnGround, ghostVelocityY,
						ghostThrowAnimIndex, ghostThrowAnimTimer, ghostThrownVerDir, ghostHeight, ghostWidth,
						skeletonX, skeletonY, skeletonThrown, skeletonThrowDir, skeletonCaptured, skeletonOnGround,
						skeletonVelocityY, skeletonThrownVerDir, skWidth, skHeight, capEnemyID, capEnemyType, capTop, playerCapturedCount);
				}
			}

			pX = facing_right ? player_x - PlayerWidth : player_x; // player x for player_gravity func bcz after scaling x on -ve makes player x on the other side

			window.clear();

			display_level(window, lvl, bgTex, bgSprite, blockTexture, blockSprite, lvl2SBlockSprite, lvl2SIBlockSprite, height, width, cell_size);

			applyVerticalPhysics(
				lvl, pX, player_y, velocityY, onGround,
				gravity, terminal_Velocity, PlayerWidth, PlayerHeight,
				cell_size, screen_x, screen_y);

			playerSpriteArr[selectedplayer].setPosition(player_x, player_y);
			if (facing_right && shootBeamShow)
			{
				shootBeamSprite.setTexture((selectedplayer == 0 ? yellowShootBeamTexture : greenShootBeamTexture));
				shootBeamSprite.setScale(-(shootBeamWidth / (selectedplayer == 0 ? yellowShootBeamTexture : greenShootBeamTexture).getSize().x * (selectedplayer == 0 ? 1.2 : 1.0)), shootBeamHeight / (selectedplayer == 0 ? yellowShootBeamTexture : greenShootBeamTexture).getSize().y);
				shootBeamSprite.setPosition(player_x + shootBeamWidth + 30, player_y + 43);
				window.draw(shootBeamSprite);
				updateGhostSuction(player_x, player_y, selectedplayer, facing_right, lvl,
								   ghostX, ghostY,
								   ghostCaptured, ghostDead, ghostThrown,
								   ghostInVac,
								   ghostRight,
								   playerCapturedCount,
								   ghostCount,
								   ghostWidth, ghostHeight,
								   cell_size, capTop, capEnemyID, capEnemyType, playerScore);

				updateSkeletonSuction(player_x, player_y, selectedplayer, lvl,
									  skeletonX, skeletonY,
									  skeletonCaptured, skeletonDead, skeletonThrown,
									  playerCapturedCount,
									  skeletonCount,
									  skWidth, skHeight,
									  cell_size, skeletonInVac, skeletonRight, facing_right, capTop, capEnemyID, capEnemyType, playerScore);
			}
			else if (!facing_right && shootBeamShow)
			{
				shootBeamSprite.setTexture((selectedplayer == 0 ? yellowShootBeamTexture : greenShootBeamTexture));
				shootBeamSprite.setScale(shootBeamWidth / (selectedplayer == 0 ? yellowShootBeamTexture : greenShootBeamTexture).getSize().x * (selectedplayer == 0 ? 1.2 : 1.0), shootBeamHeight / (selectedplayer == 0 ? yellowShootBeamTexture : greenShootBeamTexture).getSize().y);
				shootBeamSprite.setPosition(player_x - shootBeamWidth - 30, player_y + 43);
				window.draw(shootBeamSprite);

				updateGhostSuction(player_x, player_y, selectedplayer, facing_right, lvl,
								   ghostX, ghostY,
								   ghostCaptured, ghostDead, ghostThrown,
								   ghostInVac,
								   ghostRight,
								   playerCapturedCount,
								   ghostCount,
								   ghostWidth, ghostHeight,
								   cell_size, capTop, capEnemyID, capEnemyType, playerScore);

				updateSkeletonSuction(player_x, player_y, selectedplayer, lvl,
									  skeletonX, skeletonY,
									  skeletonCaptured, skeletonDead, skeletonThrown,
									  playerCapturedCount,
									  skeletonCount,
									  skWidth, skHeight,
									  cell_size, skeletonInVac, skeletonRight, facing_right, capTop, capEnemyID, capEnemyType, playerScore);
			}

			if (playerIsRunning)
			{
				animateCharacter(playerSpriteArr[selectedplayer], (selectedplayer == 0 ? yellowPlayerRunFrames : greenPlayerRunFrames), 4, playerRunIndex, playerRunTimer,
								 0.10f, facing_right, float(PlayerWidth) / ((selectedplayer == 0 ? yellowPlayerRunFrames : greenPlayerRunFrames)[playerRunIndex].getSize().x),
								 float(PlayerHeight) / ((selectedplayer == 0 ? yellowPlayerRunFrames : greenPlayerRunFrames)[playerRunIndex].getSize().y));
			}
			else
			{
				playerSpriteArr[selectedplayer].setTexture(playerTextureArr[selectedplayer]);
				playerSpriteArr[selectedplayer].setScale(((facing_right) ? (-float(PlayerWidth) / (playerTextureArr[selectedplayer].getSize().x))
																		 : (float(PlayerWidth) / (playerTextureArr[selectedplayer].getSize().x))),
														 (float(PlayerHeight) / playerTextureArr[selectedplayer].getSize().y));
			}

			for (int i = 0; i < ghostCount; i++)
			{
				if (!shootBeamShow && ghostInVac[i])
				{
					ghostInVac[i] = false;
				}
			}

			for (int i = 0; i < skeletonCount; i++)
			{
				if (!shootBeamShow && skeletonInVac[i])
				{
					skeletonInVac[i] = false;
				}
			}

			if (!playerDead)
				window.draw(playerSpriteArr[selectedplayer]);

			updateThrownEnemies(
				lvl, screen_x, screen_y, cell_size, player_x, player_y, PlayerWidth,
				PlayerHeight, facing_right, ghostX, ghostY, ghostThrown, ghostThrowDir, ghostThrownVerDir,
				ghostCaptured, ghostOnGround, ghostVelocityY, ghostCount, ghostDead, ghostInVac,
				ghostRight, ghostWidth, ghostHeight, skeletonX, skeletonY,
				skeletonCount, skWidth, skHeight, skeletonThrown, skeletonThrowDir, skeletonThrownVerDir, skeletonInVac, skeletonCaptured, skeletonOnGround,
				skeletonVelocityY, skeletonDead, playerScore);

			for (int i = 0; i < ghostCount; i++)
			{
				if (ghostThrown[i])
				{
					animateCharacter(
						ghostSpriteArr[i],													// sprite
						ghostThrownFrames,													// 8-frame throw animation
						8,																	// frameCount
						ghostThrowAnimIndex[i],												// animation index
						ghostThrowAnimTimer[i],												// animation timer
						0.2f,																// animation speed
						ghostRight[i],														// flip direction
						ghostWidth / ghostThrownFrames[ghostThrowAnimIndex[i]].getSize().x, // widthScale
						ghostHeight / ghostThrownFrames[ghostThrowAnimIndex[i]].getSize().y // heightScale
					);
				}
			}

			for (int i = 0; i < skeletonCount; i++)
			{
				if (skeletonThrown[i])
				{
					animateCharacter(
						skeletonSpriteArr[i],										// sprite
						skThrownFrames,												// 8-frame throw animation
						9,															// frameCount
						skThrownAnimIndex[i],										// animation index
						skThrownAnimTimer[i],										// animation timer
						0.14f,														// animation speed
						skeletonRight[i],											// flip direction
						skWidth / skThrownFrames[skThrownAnimIndex[i]].getSize().x, // widthScale
						skHeight / skThrownFrames[skThrownAnimIndex[i]].getSize().y // heightScale
					);
				}
			}

			for (int i = 0; i < ghostCount; i++)
			{
				if (!ghostDead[i] && !ghostCaptured[i])
					window.draw(ghostSpriteArr[i]);
			}

			for (int i = 0; i < skeletonCount; i++)
			{
				if (!skeletonDead[i] && !skeletonCaptured[i])
					window.draw(skeletonSpriteArr[i]);
			}

			bool allGhostDead = false;
			for (int i = 0; i < ghostCount; i++)
			{
				if (!ghostDead[i])
					allGhostDead = false;
				else
					allGhostDead = true;
			}

			bool allSkeletonDead = false;
			for (int i = 0; i < skeletonCount; i++)
			{
				if (!skeletonDead[i])
					allSkeletonDead = false;
				else
					allSkeletonDead = true;
			}

			if (allGhostDead && allSkeletonDead)
			{

				current = 4;

				playerScore += 1000;
				if (playerLives == 3)
				{
					playerScore += 1500;
				}
				else
				{
					playerLives = 3;
				}

				for (int i = 0; i < ghostCount; i++)
				{
					ghostOnGround[i] = false;
					ghostDead[i] = false;
				}

				for (int i = 0; i < skeletonCount; i++)
				{
					skeletonOnGround[i] = false;
					skeletonDead[i] = false;
				}

				ghostCount = 4;
				skeletonCount = 9;
				invisibleCount = 3;
				chelnovCount = 4;

				bgSprite.setTexture(lvl2BgTex);
				bgSprite.setScale(float(screen_x) / lvl2BgTex.getSize().x, float(screen_y) / lvl2BgTex.getSize().y);
				bgSprite.setPosition(0, 0);

				blockSprite.setTexture(lvl2BlockTex);
				lvl2SBlockSprite.setTexture(lvl2SBlockTex);
				lvl2SIBlockSprite.setTexture(lvl2SIBlockTex);

				srand(time(0));
				int option;
				option = rand() % 5;

				for (int i = 0; i < height; i++)
					for (int j = 0; j < width; j++)
						lvl[i][j] = ' ';

				switch (option)
				{
				case 0:
				{
					for (int i = 0; i < width; i++)
					{

						if (i == 0 || i == 1)
							lvl[7][i] = '#';
						if (i <= 3)
							lvl[10][i] = '#';
					}
					for (int i = 0; i < width; i++)
					{
						if (i > 8 && i < 16)
							lvl[2][i] = '#';

						if (i > 9 && i < 12)
							lvl[5][i] = '#';

						if (i > 15)
							lvl[5][i] = '#';
						if (i > 12 && i < 17)

							lvl[8][i] = '#';
						if (i > 14)
							lvl[11][i] = '#';
					}

					// Slant platforms
					for (int i = 0; i < width; i++)
					{
						if (i <= 2)
						{
							lvl[3][i] = '#';
							lvl[3][3] = '\\';
						}

						lvl[3][3] = '#';
						lvl[3][4] = '\\';

						lvl[4][4] = '#';
						lvl[4][5] = '\\';

						lvl[5][5] = '#';
						lvl[5][6] = '\\';

						lvl[6][6] = '#';
						lvl[6][7] = '\\';

						lvl[7][7] = '#';
						lvl[7][8] = '\\';

						lvl[8][8] = '#';
						lvl[8][9] = '\\';

						lvl[9][9] = '#';
						lvl[9][10] = '\\';

						lvl[10][10] = '#';
						lvl[10][11] = '\\';
					}

					break;
				}
				case 1:
				{
					for (int i = 0; i < width; i++)
					{
						if (i >= 0 && i <= 6)
							lvl[2][i] = '#';

						if (i > 0 && i < 4)
							lvl[5][i] = '#';

						if (i > 5 && i < 8)
							lvl[5][i] = '#';
						if (i > 0 && i < 6)

							lvl[8][i] = '#';
						if (i < 5)
							lvl[11][i] = '#';
					}
					for (int i = 0; i < width; i++)
					{

						if (i == 17 || i == 16)
							lvl[7][i] = '#';
						if (i >= 14)
							lvl[10][i] = '#';
					}
					// Slant platforms
					for (int i = 0; i < width; i++)
					{
						if (i >= 16)
						{
							lvl[3][i] = '#';
							// lvl[3][15]='/';
						}

						lvl[3][15] = '#';
						lvl[3][14] = '/';

						lvl[4][14] = '#';
						lvl[4][13] = '/';

						lvl[5][13] = '#';
						lvl[5][12] = '/';

						lvl[6][12] = '#';
						lvl[6][11] = '/';

						lvl[7][11] = '#';
						lvl[7][10] = '/';

						lvl[8][10] = '#';
						lvl[8][9] = '/';

						lvl[9][9] = '#';
						lvl[9][8] = '/';

						lvl[10][8] = '#';
						lvl[10][7] = '/';
					}
					break;
				}
				case 2:
				{
					for (int i = 0; i < width; i++)
					{
						if (i >= 0 && i <= 4)
							lvl[3][i] = '#';
						if (i >= 7 && i <= 10)
							lvl[3][i] = '#';
						if (i >= 13)
							lvl[3][i] = '#';
						if (i >= 15)
							lvl[5][i] = '#';
						if (i >= 12)
							lvl[8][i] = '#';
					}
					// Slant platforms
					for (int i = 0; i < width; i++)
					{
						if (i <= 2)
						{
							lvl[6][i] = '#';
							lvl[6][3] = '\\';
						}

						lvl[7][3] = '#';
						lvl[7][4] = '\\';

						lvl[8][4] = '#';
						lvl[8][5] = '\\';

						lvl[9][5] = '#';
						lvl[9][6] = '\\';

						lvl[10][6] = '#';
						lvl[10][7] = '\\';

						lvl[11][7] = '#';
						lvl[11][8] = '\\';

						lvl[12][8] = '#';
						lvl[12][9] = '\\';

						lvl[13][8] = '#';
						lvl[13][10] = '\\';
					}
					break;
				}
				case 3:
				{
					for (int i = 0; i < width; i++)
					{
						if (i >= 4 && i <= 6)
						{
							lvl[3][i] = '#';
							lvl[3][7] = '\\';
						}

						lvl[4][7] = '#';
						lvl[4][8] = '\\';

						lvl[5][8] = '#';
						lvl[5][9] = '\\';

						lvl[6][9] = '#';
						lvl[6][10] = '\\';

						lvl[7][10] = '#';
						lvl[7][11] = '\\';

						lvl[8][11] = '#';
						lvl[8][12] = '\\';

						lvl[9][12] = '#';
						lvl[9][13] = '\\';

						lvl[10][13] = '#';
						lvl[10][14] = '#';
						lvl[10][15] = '#';
					}

					for (int i = 0; i < width; i++)
					{
						if (i > 12)
							lvl[3][i] = '#';
						if (i > 15)
							lvl[6][i] = '#';
						if (i >= 0 && i <= 4)
							lvl[6][i] = '#';
						if (i > 4 && i < 8)
							lvl[8][i] = '#';
						if (i >= 0 && i <= 5)
							lvl[11][i] = '#';
					}
					break;
				}
				case 4:
				{
					for (int i = 0; i < width; i++)
					{
						if (i >= 14 && i <= 16)
						{
							lvl[3][i] = '#';
							// lvl[3][15]='/';
						}

						lvl[3][15] = '#';
						lvl[3][14] = '/';

						lvl[4][14] = '#';
						lvl[4][13] = '/';

						lvl[5][13] = '#';
						lvl[5][12] = '/';

						lvl[6][12] = '#';
						lvl[6][11] = '/';

						lvl[7][11] = '#';
						lvl[7][10] = '/';

						lvl[8][10] = '#';
						lvl[8][9] = '/';

						lvl[9][9] = '#';
						lvl[9][8] = '/';

						lvl[10][8] = '#';
						lvl[10][7] = '#';
						lvl[10][6] = '#';
					}

					for (int i = 0; i < width; i++)
					{
						if (i >= 0 && i <= 5)
							lvl[4][i] = '#';
						if (i >= 1 && i <= 4)
							lvl[7][i] = '#';
						if (i >= 0 && i <= 3)
							lvl[10][i] = '#';
						if (i >= 13)
							lvl[10][i] = '#';
						if (i >= 15)
							lvl[7][i] = '#';
					}

					break;
				}
				}

				playerSpriteArr[selectedplayer].setPosition(player_x, player_y);

				for (int i = 0; i < ghostCount; i++)
				{
					ghostX[i] = ghostWidth + rand() % 700;
					ghostY[i] = ghostHeight + rand() % 700;
					ghostSpriteArr[i].setTexture(ghostTexture);
				}
				for (int i = 0; i < skeletonCount; i++)
				{
					skeletonX[i] = skWidth + rand() % 700;
					skeletonY[i] = skHeight + rand() % 700;
					skeletonSpriteArr[i].setTexture(skeletonTexture);
				}
				for (int i = 0; i < invisibleCount; i++)
				{
					invisibleX[i] = skWidth + rand() % 700;
					invisibleY[i] = skHeight + rand() % 700;
					invisibleSpriteArr[i].setTexture(inivisibleTexture);
				}
				for (int i = 0; i < chelnovCount; i++)
				{
					chelnovX[i] = skWidth + rand() % 700;
					chelnovY[i] = skHeight + rand() % 700;
					chelnovSpriteArr[i].setTexture(chelnovTexture);
				}
			}
			else if (playerLives <= 0)
			{
				playerScore -= 200;
				current = 3;
				for (int i = 0; i < ghostCount; i++)
				{
					ghostCaptured[i] = false;
					ghostDead[i] = false;
					ghostInVac[i] = false;
					ghostThrown[i] = false;
				}
				for (int i = 0; i < skeletonCount; i++)
				{
					skeletonCaptured[i] = false;
					skeletonDead[i] = false;
					skeletonInVac[i] = false;
					skeletonThrown[i] = false;
				}

				initGhosts(ghostTexture, ghostSpriteArr, ghostX, ghostY, ghostRight, ghostCount, ghostWidth, ghostHeight, ghostOnGround);
				initSkeletons(skeletonTexture, skeletonSpriteArr,
							  skeletonX, skeletonY, skeletonRight,
							  skeletonVelocityY, skeletonOnGround,
							  skeletonCooldown,
							  skeletonCount, skWidth, skHeight);

				playerScore = 0;
				playerLives = 3;
			}

			window.draw(livechecksprite);
			window.draw(livesText);
			window.draw(scoreText);
		}
		else if (current == 4)
		{

			// ****************************************************************************Level 2***********************************************************************8
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				window.close();
			}

			movX = 0;
			playerIsRunning = false;

			updateGhosts(lvl, ghostSpriteArr, ghostTexture, ghostX, ghostY, ghostRight, ghostThrown, ghostRandom,
						 ghostCount, ghostSpeed, ghostWidth, ghostHeight, cell_size, screen_x, ghostDead, ghostCaptured, ghostInVac);

			updateSkeletons(lvl,
							skeletonSpriteArr, skeletonTexture, skeletonX, skeletonY,
							skeletonRight, skeletonThrown, skeletonVelocityY,
							skeletonOnGround, skeletonCooldown,
							skeletonCount,
							1.0f, jumpStrength, gravity,
							terminal_Velocity,
							cell_size, screen_x,
							skWidth, skHeight, skeletonDead, skeletonCaptured, skeletonInVac);

			updateInvisibles(lvl, inivisibleTexture, invisibleSpriteArr, invisibleX, invisibleY, invisibleRight,
							 invisibleVelocityY, invisibleOnGround, invisibleCoolDown, invisibleCount, skWidth,
							 skHeight, invisibleThrown, 1.0f, -20.0f, 1.0f, 20.0f, 64, screen_x, invisibleDead, invisibleCaptured, invisibleInVac, invisibleInvisible);

			if (!playerDead)
			{
				if (Keyboard::isKeyPressed(Keyboard::Left))
				{
					if (facing_right) // changing player direction
					{
						playerSpriteArr[selectedplayer].setScale(float(PlayerWidth) / playerTextureArr[selectedplayer].getSize().x, float(PlayerHeight) / playerTextureArr[selectedplayer].getSize().y);
						player_x -= PlayerWidth;
						facing_right = false;
					}
					else if (!(player_x <= 0)) // handeling left movement
					{
						playerIsRunning = true;
						movX = -(selectedplayer == 0 ? speed : speed * 1.5);
						top_left = lvl[int(player_y) / cell_size][int(player_x + movX) / cell_size];
						left_mid = lvl[int(player_y + PlayerHeight / 2) / cell_size][int(player_x + movX) / cell_size];
						bottom_left = lvl[int(player_y + PlayerHeight) / cell_size][int(player_x + movX) / cell_size];
						if (!(top_left == '#' || left_mid == '#' || bottom_left == '#'))
						{
							player_x += movX;
						}
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Right))
				{
					if (!facing_right) // changing player direction
					{
						playerSpriteArr[selectedplayer].setScale(-float(PlayerWidth) / playerTextureArr[selectedplayer].getSize().x, float(PlayerHeight) / playerTextureArr[selectedplayer].getSize().y);
						player_x += PlayerWidth;
						facing_right = true;
					}
					else if (!(player_x >= screen_x)) // handeling right movement
					{
						playerIsRunning = true;
						movX = (selectedplayer == 0 ? speed : speed * 1.5);
						top_right = lvl[int(player_y) / cell_size][int(player_x + movX) / cell_size];
						right_mid = lvl[int(player_y + PlayerHeight / 2) / cell_size][int(player_x + movX) / cell_size];
						bottom_right = lvl[int(player_y + PlayerHeight) / cell_size][int(player_x + movX) / cell_size];
						if (!(top_right == '#' || right_mid == '#' || bottom_right == '#'))
						{
							player_x += movX;
						}
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Up)) // ADDED THE JUMP FUNCTION
				{
					if (onGround)
					{
						velocityY = jumpStrength;
						onGround = false;
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Down))
				{
					if (onGround && !(player_y + PlayerHeight >= 896))
					{
						player_y += 64;
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Space))
				{
					shootBeamShow = true;
				}
				else
				{
					shootBeamShow = false;
				}

				static bool zWasPressed = false;

				bool zNow = Keyboard::isKeyPressed(Keyboard::Z);

				if (zNow && !zWasPressed)
				{
					int newThrowDir = 0; // default forward

					if (Keyboard::isKeyPressed(Keyboard::W))
						newThrowDir = 1;
					else if (Keyboard::isKeyPressed(Keyboard::D))
						newThrowDir = 2;
					else if (Keyboard::isKeyPressed(Keyboard::S))
						newThrowDir = 3;
					else if (Keyboard::isKeyPressed(Keyboard::A))
						newThrowDir = 4;
					else
						newThrowDir = facing_right ? 2 : 4;
					if (capTop >= 0)
					{
						int id = capEnemyID[capTop];
						int type = capEnemyType[capTop];

						if (type == 1)
							ghostThrowDir[id] = newThrowDir;
						else if (type == 2)
							skeletonThrowDir[id] = newThrowDir;
					}

					// now actually throw the enemy
					throwLastCapturedEnemy(
						player_x, player_y, PlayerWidth, PlayerHeight, facing_right,
						ghostX, ghostY, ghostThrown, ghostThrowDir, ghostCaptured, ghostOnGround, ghostVelocityY,
						ghostThrowAnimIndex, ghostThrowAnimTimer, ghostThrownVerDir, ghostHeight, ghostWidth,
						skeletonX, skeletonY, skeletonThrown, skeletonThrowDir, skeletonCaptured, skeletonOnGround,
						skeletonVelocityY, skeletonThrownVerDir, skWidth, skHeight, capEnemyID, capEnemyType, capTop,
						playerCapturedCount);
				}

				zWasPressed = zNow;
			}
			pX = facing_right ? player_x - PlayerWidth : player_x;

			window.clear();
			display_level(window, lvl, bgTex, bgSprite, blockTexture, blockSprite, lvl2SBlockSprite, lvl2SIBlockSprite, height, width, cell_size);
			applyVerticalPhysics(
				lvl, pX, player_y, velocityY, onGround,
				gravity, terminal_Velocity, PlayerWidth, PlayerHeight,
				cell_size, screen_x, screen_y);
			if (!playerDead)
			{
				facing_right = false;
				window.draw(playerSpriteArr[selectedplayer]);
			}
			for (int i = 0; i < ghostCount; i++)
			{
				window.draw(ghostSpriteArr[i]);
			}
			for (int i = 0; i < skeletonCount; i++)
			{
				window.draw(skeletonSpriteArr[i]);
			}
			for (int i = 0; i < invisibleCount; i++)
			{
				window.draw(invisibleSpriteArr[i]);
			}
			for (int i = 0; i < chelnovCount; i++)
			{
				window.draw(chelnovSpriteArr[i]);
			}

			window.draw(livechecksprite);
			window.draw(livesText);
			window.draw(scoreText);
		}

		window.display();
	}

	// stopping music and deleting level array
	lvlMusic.stop();
	for (int i = 0; i < height; i++)
	{
		delete[] lvl[i];
	}
	delete[] lvl;

	return 0;
}
