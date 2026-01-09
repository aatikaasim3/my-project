#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

int screen_x = 1136;
int screen_y = 896;

void display_level(RenderWindow &window, char **lvl,
				   Texture &bgTex, Sprite &bgSprite,
				   Texture &blockTexture, Sprite &blockSprite,
				   Sprite &triLeftSprite, Sprite &triRightSprite,
				   const int height, const int width, const int cell_size)

{
	window.draw(bgSprite);

	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{

			if (lvl[i][j] == '#')
			{
				blockSprite.setPosition(j * cell_size, i * cell_size);
				window.draw(blockSprite);
			}
			else if (lvl[i][j] == '\\') // IMPORTANT: double slash!
			{
				triLeftSprite.setPosition(j * cell_size, i * cell_size);
				window.draw(triLeftSprite);
			}
			else if (lvl[i][j] == '/')
			{
				triRightSprite.setPosition(j * cell_size, i * cell_size);
				window.draw(triRightSprite);
			}
		}
	}
}

void applyVerticalPhysics(
	char **lvl,
	float &objX, float &objY, // position
	int &velY,				  // vertical velocity
	bool &onGround,			  // output: is object standing?
	float gravity, float terminalVel,
	float objWidth, float objHeight,
	int cell_size,
	int screen_x, int screen_y)
{
	// Predict next Y
	float nextY = objY + velY;

	// Compute tile row below sprite bottom
	int bottomRow = (int)(nextY + objHeight) / cell_size;

	// Clamp to valid rows
	if (bottomRow >= 14)
		bottomRow = 13;
	if (bottomRow < 0)
		bottomRow = 0;

	// Columns for bottom-left, bottom-mid, bottom-right
	int leftCol = (int)(objX) / cell_size;
	int midCol = (int)(objX + objWidth / 2) / cell_size;
	int rightCol = (int)(objX + objWidth) / cell_size;

	// Safe clamp
	if (leftCol < 0)
		leftCol = 0;
	if (midCol < 0)
		midCol = 0;
	if (rightCol < 0)
		rightCol = 0;

	if (rightCol >= 18)
		rightCol = 17;
	if (midCol >= 18)
		midCol = 17;
	if (leftCol >= 18)
		leftCol = 17;

	char bL = lvl[bottomRow][leftCol];
	char bM = lvl[bottomRow][midCol];
	char bR = lvl[bottomRow][rightCol];

	// --- FLOOR COLLISION ---
	if (velY >= 0 && (bL == '#' || bM == '#' || bR == '#' || bL == '/' || bM == '/' || bR == '/' || bL == '\\' || bM == '\\' || bR == '\\'))
	{
		onGround = true;
		velY = 0;
	}
	else
	{
		// Apply motion
		objY = nextY;
		onGround = false;

		// Apply gravity
		velY += gravity;
		if (velY > terminalVel)
			velY = terminalVel;
	}

	// --- Screen boundaries ---
	if (objY < 0)
	{
		objY = 0;
		velY = 0;
	}

	if (objY + objHeight > screen_y)
	{
		objY = screen_y - objHeight;
		onGround = true;
		velY = 0;
	}
}

void initGhosts(Texture &ghostTexture, Sprite ghostSpriteArr[], float ghostX[],
				float ghostY[], bool ghostRight[], int ghostCount, float ghostWidth, float ghostHeight,
				bool ghostOnGround[])
{
	for (int i = 0; i < ghostCount; i++)
	{
		ghostSpriteArr[i].setTexture(ghostTexture);

		// Initial positions based on your game design
		switch (i)
		{
		case 0:
			ghostX[i] = 129;
			ghostY[i] = 195.5;
			ghostRight[i] = true;
			break;
		case 1:
			ghostX[i] = 915.2;
			ghostY[i] = 195.5;
			ghostRight[i] = false;
			break;
		case 2:
			ghostX[i] = 1;
			ghostY[i] = 387.5;
			ghostRight[i] = true;
			break;
		case 3:
			ghostX[i] = 530.2;
			ghostY[i] = 387.5;
			ghostRight[i] = false;
			break;
		case 4:
			ghostX[i] = 833;
			ghostY[i] = 387.5;
			ghostRight[i] = true;
			break;
		case 5:
			ghostX[i] = 211.2;
			ghostY[i] = 579.5;
			ghostRight[i] = false;
			break;
		case 6:
			ghostX[i] = 513;
			ghostY[i] = 579.5;
			ghostRight[i] = true;
			break;
		case 7:
			ghostX[i] = 1043.2;
			ghostY[i] = 579.5;
			ghostRight[i] = false;
			break;
		default:
			break;
		}

		// Direction Initial
		if (ghostRight[i])
		{
			ghostSpriteArr[i].setScale(-1.5f, 1.1f);
			ghostX[i] += ghostWidth; // fix to avoid shifting left
		}
		else
		{
			ghostSpriteArr[i].setScale(1.5f, 1.1f);
		}

		// Applying Initial Position
		ghostSpriteArr[i].setPosition(ghostX[i], ghostY[i]);
	}
}

void updateGhosts(char **lvl, Sprite ghostSpriteArr[], Texture ghostTexture, float ghostX[], float ghostY[], bool ghostRight[], bool ghostIsThrown[], int ghostRandom[], int ghostCount, float ghostSpeed, float ghostWidth, float ghostHeight, int cell_size, int screen_x, bool ghostDead[], bool ghostCaptured[], bool ghostInVac[])
{

	for (int i = 0; i < ghostCount; i++)
	{
		bool old = ghostRight[i];

		if (ghostDead[i] || ghostCaptured[i] || ghostInVac[i] || ghostIsThrown[i])
		{
			ghostSpriteArr[i].setPosition(ghostX[i], ghostY[i]);
			continue;
		}

		// Checking PLatform
		if (ghostRight[i])
		{
			// Check next tile (right side bottom)
			if (lvl[(int)(ghostY[i] + ghostHeight) / cell_size + 1][(int)(ghostX[i] + ghostWidth) / cell_size] != '#')
				ghostRight[i] = false;
		}
		else
		{
			// Check next tile (left side bottom)
			if (lvl[(int)(ghostY[i] + ghostHeight) / cell_size + 1][(int)(ghostX[i] - 1) / cell_size] != '#')
				ghostRight[i] = true;
		}

		// Next Position of Ghost
		if (ghostRight[i])
			ghostX[i] += ghostSpeed;
		else
			ghostX[i] -= ghostSpeed;

		// Horizontal Screen Collision
		if (ghostX[i] < 0 && !ghostRight[i])
		{
			ghostRight[i] = true;
			ghostX[i] += ghostSpeed;
		}

		if (ghostX[i] > screen_x - ghostWidth && ghostRight[i])
		{
			ghostRight[i] = false;
			ghostX[i] -= ghostSpeed;
		}

		// Random change of direction
		if (old == ghostRight[i] && rand() % 200 == 0 && ghostRandom[i] == 0)
		{
			ghostRight[i] = !ghostRight[i];
			ghostRandom[i] = 240; // cooldown
		}

		// Flipping of sprite (if direction changes)
		if (old != ghostRight[i])
		{
			if (ghostRight[i])
			{
				ghostSpriteArr[i].setScale(-1.5f, 1.1f);
				ghostX[i] += ghostWidth;
			}
			else
			{
				ghostSpriteArr[i].setScale(1.5f, 1.1f);
				ghostX[i] -= ghostWidth;
			}
		}

		// Random Cooldown
		if (ghostRandom[i] > 0)
			ghostRandom[i]--;

		// Apply New Position
		ghostSpriteArr[i].setPosition(ghostX[i], ghostY[i]);
	}
}

void initSkeletons(Texture &skTexture, Sprite skSprite[],
				   float skX[], float skY[], bool skRight[],
				   int skVelocityY[], bool skOnGround[],
				   int skCooldown[], int skCount, float skWidth, float skHeight)
{
	for (int i = 0; i < skCount; i++)
	{
		skSprite[i].setTexture(skTexture);
		skSprite[i].setScale(1.0f, 1.1f);

		// Starting positions (you can change)
		switch (i)
		{
		case 0:
			skX[i] = 1;
			skY[i] = (7 * 64) - skHeight;
			skRight[i] = true;
			break;
		case 1:
			skX[i] = 1081;
			skY[i] = (10 * 64) - skHeight;
			skRight[i] = false;
			break;
		case 2:
			skX[i] = 1;
			skY[i] = (13 * 64) - skHeight;
			skRight[i] = true;
			break;
		case 3:
			skX[i] = 1081;
			skY[i] = (13 * 64) - skHeight;
			skRight[i] = false;
			break;
		}

		// Flip sprite if facing right
		if (skRight[i])
		{
			skSprite[i].setScale(-1.0f, 1.1f);
			skX[i] += skWidth;
		}

		skY[i] -= 20; // lift slightly so they start above the floor
		skVelocityY[i] = 0;
		skOnGround[i] = false;
		skCooldown[i] = 0;

		skSprite[i].setPosition(skX[i], skY[i]);
	}
}

bool canSkeletonMove(float newX, float skY, int skWidth, int skHeight,
					 char **lvl, int cell_size)
{
	int topRow = (int)(skY) / cell_size;
	int midRow = (int)(skY + skHeight / 2) / cell_size;
	int bottomRow = (int)(skY + skHeight) / cell_size;

	int leftCol = (int)(newX) / cell_size;
	int rightCol = (int)(newX + skWidth) / cell_size;

	char tl = lvl[topRow][leftCol];
	char ml = lvl[midRow][leftCol];
	char bl = lvl[bottomRow][leftCol];

	char tr = lvl[topRow][rightCol];
	char mr = lvl[midRow][rightCol];
	char br = lvl[bottomRow][rightCol];

	if (tl == '#' || ml == '#' || bl == '#')
		return false;
	if (tr == '#' || mr == '#' || br == '#')
		return false;

	return true;
}

void updateInvisibles(char **lvl, Texture &invisibleTexture, Sprite invisibleSprite[], float invisibleX[], float invisibleY[], bool invisibleRight[], int invisibleVelocityY[], bool invisibleOnGround[],
					  int invisibleCoolDown[], int invisibleCount, float invisibleWidth, float invisibleHeight,
					  bool invisibleThrown[], float invisibleSpeed, float jumpStrength, float gravity,
					  float terminalVelocity, int cellSize, int screen_x, bool invisibleDead[], bool invisibleCaptured[], bool invisibleInVac[], bool invisibleHidden[])
{
	for (int i = 0; i < invisibleCount; i++)
	{
		bool oldDir = invisibleRight[i];

		if (invisibleDead[i] || invisibleCaptured[i] || invisibleInVac[i] || invisibleThrown[i])
		{
			invisibleSprite[i].setPosition(invisibleX[i], invisibleY[i]);
			continue;
		}

		// Detect top & bottom platform
		int currentRow = (int)(invisibleY[i] + invisibleHeight) / cellSize;

		bool onTopPlatform = (currentRow == 4);
		bool onBottomPlatform = (currentRow == 13);

		if (invisibleCoolDown[i] == 0 && rand() % 480 == 0)
		{
			invisibleRight[i] = !invisibleRight[i];
			invisibleCoolDown[i] = 480;
		}

		if (invisibleCoolDown[i] > 0)
			invisibleCoolDown[i]--;

		// Horizontal Movement
		float movX = invisibleRight[i] ? invisibleSpeed : -invisibleSpeed;
		float newX = invisibleX[i] + movX;

		// TRY MOVE — ONLY move if no block collision
		if (canSkeletonMove(newX, invisibleY[i], invisibleWidth, invisibleHeight, lvl, cellSize))
		{
			invisibleX[i] = newX;
		}
		else
		{
			// DO NOT change direction
			// DO NOT move
			// skeleton stays stuck until cooldown flips direction
		}

		// Screen Collission
		if (invisibleX[i] <= 0)
		{
			invisibleX[i] = 0;
			invisibleRight[i] = true;
		}
		if (invisibleX[i] + invisibleWidth >= screen_x)
		{
			invisibleX[i] = screen_x - invisibleWidth;
			invisibleRight[i] = false;
		}

		if (invisibleOnGround[i] && !onTopPlatform && rand() % 200 == 0)
		{
			invisibleVelocityY[i] = jumpStrength;
			invisibleOnGround[i] = false;
		}

		applyVerticalPhysics(
			lvl,
			invisibleX[i], invisibleY[i],
			invisibleVelocityY[i],
			invisibleOnGround[i],
			gravity, terminalVelocity,
			invisibleWidth, invisibleHeight,
			cellSize,
			screen_x, screen_y);

		// Flipping of Sprite
		if (oldDir != invisibleRight[i])
		{
			if (invisibleRight[i])
			{
				invisibleSprite[i].setScale(-1.1f, 1.1f);
				invisibleX[i] += invisibleWidth;
			}
			else
			{
				invisibleSprite[i].setScale(1.1f, 1.1f);
			}
		}

		// Setting New Positions
		invisibleSprite[i].setPosition(invisibleX[i], invisibleY[i]);
	}
}

void updateSkeletons(char **lvl,
					 Sprite skSprite[], Texture skeletonTexture, float skX[], float skY[],
					 bool skRight[], bool skThrown[], int skVelocityY[],
					 bool skOnGround[], int skCooldown[],
					 int skCount, float skSpeed, float jumpStrength,
					 float gravity, float terminal_Velocity,
					 int cell_size, int screen_x, float skWidth, float skHeight, bool skeletonDead[], bool skeletonCaptured[], bool skeletonInVac[])
{
	for (int i = 0; i < skCount; i++)
	{

		bool oldDir = skRight[i];

		if (skeletonDead[i] || skeletonCaptured[i] || skeletonInVac[i] || skThrown[i])
		{
			skSprite[i].setPosition(skX[i], skY[i]);
			continue;
		}

		// Detect top & bottom platform
		int currentRow = (int)(skY[i] + skHeight) / cell_size;

		bool onTopPlatform = (currentRow == 4);
		bool onBottomPlatform = (currentRow == 13);

		// Random Horizontal Direction Change
		if (skCooldown[i] == 0 && rand() % 480 == 0)
		{
			skRight[i] = !skRight[i];
			skCooldown[i] = 480;
		}

		if (skCooldown[i] > 0)
			skCooldown[i]--;

		// Horizontal Movement
		float movX = skRight[i] ? skSpeed : -skSpeed;
		float newX = skX[i] + movX;

		// TRY MOVE — ONLY move if no block collision
		if (canSkeletonMove(newX, skY[i], skWidth, skHeight, lvl, cell_size))
		{
			skX[i] = newX;
		}
		else
		{
			// DO NOT change direction
			// DO NOT move
			// skeleton stays stuck until cooldown flips direction
		}

		// Screen Collission
		if (skX[i] <= 0)
		{
			skX[i] = 0;
			skRight[i] = true;
		}
		if (skX[i] + skWidth >= screen_x)
		{
			skX[i] = screen_x - skWidth;
			skRight[i] = false;
		}

		// Random Jump
		if (skOnGround[i] && !onTopPlatform && rand() % 200 == 0)
		{
			skVelocityY[i] = jumpStrength;
			skOnGround[i] = false;
		}

		applyVerticalPhysics(
			lvl,
			skX[i], skY[i],
			skVelocityY[i],
			skOnGround[i],
			gravity, terminal_Velocity,
			skWidth, skHeight,
			cell_size,
			screen_x, screen_y);

		// Flipping of Sprite
		if (oldDir != skRight[i])
		{
			if (skRight[i])
			{
				skSprite[i].setScale(-1.1f, 1.1f);
				skX[i] += skWidth;
			}
			else
			{
				skSprite[i].setScale(1.1f, 1.1f);
			}
		}

		// Setting New Positions
		skSprite[i].setPosition(skX[i], skY[i]);
	}
}

bool enemyIsInFront(bool facing_right, int selectedplayer, bool enemy_right, float player_x, float enemy_x, float enemyWidth)
{
	bool result;
	if (facing_right)
	{
		if (enemy_right)
			result = enemy_x - enemyWidth >= player_x && enemy_x - enemyWidth <= player_x + (selectedplayer == 0 ? 216 : 180);
		else if (!enemy_right)
			result = enemy_x > player_x && enemy_x <= player_x + (selectedplayer == 0 ? 216 : 180);
	}
	else if (!facing_right)
	{
		if (enemy_right)
			result = enemy_x <= player_x && enemy_x >= player_x - (selectedplayer == 0 ? 216 : 180);
		else if (!enemy_right)
			result = enemy_x + enemyWidth <= player_x && enemy_x + enemyWidth >= player_x - (selectedplayer == 0 ? 216 : 180);
	}
	return result;
}

void updateGhostSuction(float player_x, float player_y, int selectedplayer, bool facing_right, char **lvl,
						float ghostX[], float ghostY[],
						bool ghostCaptured[], bool ghostDead[], bool ghostThrown[],
						bool ghostInVac[],
						bool ghostRight[],
						int &playerCapturedCount,
						const int ghostCount,
						float ghostWidth, float ghostHeight,
						int cell_size, int &capTop, int capEnemyID[], int capEnemyType[], int &playerScore)
{

	for (int i = 0; i < ghostCount; i++)
	{
		if (ghostDead[i] || ghostCaptured[i] || ghostThrown[i])
			continue;

		// ------- 1. Same vertical platform -------
		int playerRow = (int)(player_y + 40) / cell_size;
		int ghostRow = (int)(ghostY[i] + ghostHeight) / cell_size;

		if (playerRow != ghostRow)
			continue;

		// ------- 2. Horizontal distance within (selectedplayer == 0 ? 216 : 180) px -------
		float dist = fabs((ghostX[i] + ghostWidth / 2) - (player_x + 48));
		if (dist > (selectedplayer == 0 ? 216 : 180))
			continue;

		if (!enemyIsInFront(facing_right, selectedplayer, ghostRight[i], player_x, ghostX[i], ghostWidth))
			continue; // ← enemy is behind player, skip

		// ------- 3. SUCTION: move ghost towards player -------
		float speed = 3.0f;
		ghostInVac[i] = true;

		if (ghostX[i] < player_x)
			ghostX[i] += speed;
		else
			ghostX[i] -= speed;

		// ghost suction logic
		if (facing_right)
		{
			if (ghostRight[i] && player_x >= ghostX[i] - ghostWidth)
			{
				capTop++;
				capEnemyID[capTop] = i;	  // index
				capEnemyType[capTop] = 1; // or false for skeleton

				ghostCaptured[i] = true;
				ghostInVac[i] = false;
				ghostX[i] = -9999;
				ghostY[i] = -9999;
				playerCapturedCount++;
			}
			else if (!ghostRight[i] && player_x >= ghostX[i])
			{
				capTop++;
				capEnemyID[capTop] = i;	  // index
				capEnemyType[capTop] = 1; // or false for skeleton

				ghostCaptured[i] = true;
				ghostInVac[i] = false;
				ghostX[i] = -9999;
				ghostY[i] = -9999;
				playerCapturedCount++;
			}
		}
		else if (!facing_right)
		{
			if (ghostRight[i] && player_x <= ghostX[i])
			{
				capTop++;
				capEnemyID[capTop] = i;	  // index
				capEnemyType[capTop] = 1; // or false for skeleton

				ghostCaptured[i] = true;
				ghostInVac[i] = false;
				ghostX[i] = -9999;
				ghostY[i] = -9999;
				playerCapturedCount++;
			}
			else if (!ghostRight[i] && player_x <= ghostX[i] + ghostWidth)
			{
				capTop++;
				capEnemyID[capTop] = i;	  // index
				capEnemyType[capTop] = 1; // or false for skeleton

				ghostCaptured[i] = true;
				ghostInVac[i] = false;
				ghostX[i] = -9999;
				ghostY[i] = -9999;
				playerCapturedCount++;
			}
		}

		if (ghostCaptured[i])
		{
			playerScore += 50;
		}
	}
}

void updateSkeletonSuction(float player_x, float player_y, int selectedplayer,
						   char **lvl,
						   float skX[], float skY[],
						   bool skeletonCaptured[], bool skeletonDead[], bool skeletonThrown[],
						   int &playerCapturedCount,
						   int skeletonCount,
						   float skWidth, float skHeight,
						   int cell_size, bool skeletonInVac[], bool skeletonRight[], bool facing_right, int &capTop, int capEnemyID[], int capEnemyType[], int &playerScore)
{

	for (int i = 0; i < skeletonCount; i++)
	{
		if (skeletonDead[i])
			continue;
		if (skeletonCaptured[i] || skeletonThrown[i])
			continue;

		// ------ 1. Same vertical platform ------
		int playerRow = (int)(player_y + 40) / cell_size;
		int skRow = (int)(skY[i] + skHeight) / cell_size;

		if (playerRow != skRow)
			continue;

		// ------ 2. Horizontal distance ≤ (selectedplayer == 0 ? 216 : 180) ------
		float dist = fabs((skX[i] + skWidth / 2) - (player_x + 48));
		if (dist > (selectedplayer == 0 ? 216 : 180))
			continue;

		if (!enemyIsInFront(facing_right, selectedplayer, skeletonRight[i], player_x, skX[i], skWidth))
			continue;

		// ------ 3. Suction pull ------
		float pullSpeed = 3.0f;
		skeletonInVac[i] = true;
		if (skX[i] < player_x)
			skX[i] += pullSpeed;
		else
			skX[i] -= pullSpeed;

		// ------ 4. Captured ------
		if (facing_right)
		{
			if (skeletonRight[i] && player_x >= skeletonRight[i] - skWidth)
			{
				capTop++;
				capEnemyID[capTop] = i; // index
				capEnemyType[capTop] = 2;

				skeletonCaptured[i] = true;
				skeletonInVac[i] = false;
				skX[i] = -9999;
				skY[i] = -9999;
				playerCapturedCount++;
			}
			else if (!skeletonRight[i] && player_x >= skX[i])
			{
				capTop++;
				capEnemyID[capTop] = i; // index
				capEnemyType[capTop] = 2;

				skeletonCaptured[i] = true;
				skeletonInVac[i] = false;
				skX[i] = -9999;
				skY[i] = -9999;
				playerCapturedCount++;
			}
		}
		else if (!facing_right)
		{
			if (skeletonRight[i] && player_x <= skX[i])
			{
				capTop++;
				capEnemyID[capTop] = i; // index
				capEnemyType[capTop] = 2;

				skeletonCaptured[i] = true;
				skeletonInVac[i] = false;
				skX[i] = -9999;
				skY[i] = -9999;
				playerCapturedCount++;
			}
			else if (!skeletonRight[i] && player_x <= skX[i] + skWidth)
			{
				capTop++;
				capEnemyID[capTop] = i; // index
				capEnemyType[capTop] = 2;

				skeletonCaptured[i] = true;
				skeletonInVac[i] = false;
				skX[i] = -9999;
				skY[i] = -9999;
				playerCapturedCount++;
			}
		}

		if (skeletonCaptured[i])
		{
			playerScore += 75;
		}
	}
}

void throwLastCapturedEnemy(
	float player_x, float player_y, float playerWidth, float playerHeight, bool facing_right,
	float ghostX[], float ghostY[],
	bool ghostThrown[], int ghostThrowDir[],
	bool ghostCaptured[], bool ghostOnGround[], int ghostVelocityY[],
	int ghostThrowAnimIndex[], float ghostThrowAnimTimer[], int ghostThrownVerDir[], float ghostHeight,
	float ghostWidth,

	float skX[], float skY[],
	bool skThrown[], int skThrowDir[],
	bool skCaptured[], bool skOnGround[], int skVelocityY[], int skThrownVerDir[], float skWidth, float skHeight,

	int capEnemyID[], int capEnemyType[], int &capTop, int &playerCapturedCount)
{
	if (capTop < 0)
	{
		capTop = -1;
		return;
	}

	int id = capEnemyID[capTop];
	int enemyType = capEnemyType[capTop];
	capTop--;
	playerCapturedCount--;

	switch (enemyType)
	{
	case 1:
		cout << "Ghost " << id << " died.\n";
		if (ghostThrowDir[id] == 0)
		{
			if (facing_right)
			{
				ghostX[id] = player_x + 10;
				ghostY[id] = player_y + playerHeight - ghostHeight;
				ghostThrowDir[id] = 2;
				ghostCaptured[id] = false;
				ghostThrown[id] = true;
				ghostThrowAnimIndex[id] = 0;
				ghostThrowAnimTimer[id] = 0;
			}
			else if (!facing_right)
			{
				ghostX[id] = player_x - 10;
				ghostY[id] = player_y + playerHeight - ghostHeight;
				ghostThrowDir[id] = 4;
				ghostCaptured[id] = false;
				ghostThrown[id] = true;
				ghostThrowAnimIndex[id] = 0;
				ghostThrowAnimTimer[id] = 0;
			}
		}
		else if (ghostThrowDir[id] == 1)
		{
			if (facing_right)
			{
				ghostX[id] = player_x + 10;
				ghostY[id] = player_y + 10;
				ghostOnGround[id] = false;
				ghostVelocityY[id] = -20;
				ghostCaptured[id] = false;
				ghostThrown[id] = true;
				ghostThrowAnimIndex[id] = 0;
				ghostThrowAnimTimer[id] = 0;
				ghostThrownVerDir[id] = 1;
			}
			else if (!facing_right)
			{
				ghostX[id] = player_x - 10;
				ghostY[id] = player_y + 20;
				ghostOnGround[id] = false;
				ghostVelocityY[id] = -20;
				ghostCaptured[id] = false;
				ghostThrown[id] = true;
				ghostThrowAnimIndex[id] = 0;
				ghostThrowAnimTimer[id] = 0;
				ghostThrownVerDir[id] = 2;
			}
		}
		else if (ghostThrowDir[id] == 2)
		{
			if (facing_right)
			{
				ghostX[id] = player_x + 10;
				ghostY[id] = player_y + playerHeight - ghostHeight;
				ghostCaptured[id] = false;
				ghostThrown[id] = true;
				ghostThrowAnimIndex[id] = 0;
				ghostThrowAnimTimer[id] = 0;
			}
			else if (!facing_right)
			{
				ghostX[id] = player_x + playerWidth + 10;
				ghostY[id] = player_y + playerHeight - ghostHeight;
				ghostCaptured[id] = false;
				ghostThrown[id] = true;
				ghostThrowAnimIndex[id] = 0;
				ghostThrowAnimTimer[id] = 0;
			}
		}
		else if (ghostThrowDir[id] == 3)
		{
			if (facing_right)
			{
				ghostX[id] = player_x + 10;
				ghostY[id] = player_y + playerHeight + 64;
				ghostCaptured[id] = false;
				ghostThrown[id] = true;
				ghostOnGround[id] = false;
				ghostVelocityY[id] = 0;
				ghostThrowAnimIndex[id] = 0;
				ghostThrowAnimTimer[id] = 0;
				ghostThrownVerDir[id] = 1;
			}
			else if (!facing_right)
			{
				ghostX[id] = player_x - 10;
				ghostY[id] = player_y + 64;
				ghostCaptured[id] = false;
				ghostThrown[id] = true;
				ghostOnGround[id] = false;
				ghostVelocityY[id] = 0;
				ghostThrowAnimIndex[id] = 0;
				ghostThrowAnimTimer[id] = 0;
				ghostThrownVerDir[id] = 2;
			}
		}
		else if (ghostThrowDir[id] == 4)
		{
			if (facing_right)
			{
				ghostX[id] = player_x - playerWidth - 10;
				ghostY[id] = player_y + playerHeight - ghostHeight;
				ghostCaptured[id] = false;
				ghostThrown[id] = true;
				ghostThrowAnimIndex[id] = 0;
				ghostThrowAnimTimer[id] = 0;
			}
			else if (!facing_right)
			{
				ghostX[id] = player_x - 10;
				ghostY[id] = player_y + playerHeight - ghostHeight;
				ghostCaptured[id] = false;
				ghostThrown[id] = true;
				ghostThrowAnimIndex[id] = 0;
				ghostThrowAnimTimer[id] = 0;
			}
		}
		break;

	case 2:
		cout << "Skeleton " << id << " died.\n";
		if (skThrowDir[id] == 0)
		{
			if (facing_right)
			{
				skX[id] = player_x + 10;
				skY[id] = player_y + playerHeight - skHeight;
				skThrowDir[id] = 2;
				skCaptured[id] = false;
				skThrown[id] = true;
			}
			else
			{
				skX[id] = player_x - 10;
				skY[id] = player_y + playerHeight - skHeight;
				skThrowDir[id] = 4;
				skCaptured[id] = false;
				skThrown[id] = true;
			}
		}
		else if (skThrowDir[id] == 1)
		{
			if (facing_right)
			{
				skX[id] = player_x + 10;
				skY[id] = player_y + 10;
				skOnGround[id] = false;
				skVelocityY[id] = -20;
				skCaptured[id] = false;
				skThrown[id] = true;
				skThrownVerDir[id] = 1;
			}
			else
			{
				skX[id] = player_x - 10;
				skY[id] = player_y + 20;
				skOnGround[id] = false;
				skVelocityY[id] = -20;
				skCaptured[id] = false;
				skThrown[id] = true;
				skThrownVerDir[id] = 2;
			}
		}
		else if (skThrowDir[id] == 2)
		{
			if (facing_right)
			{
				skX[id] = player_x + 10;
				skY[id] = player_y + playerHeight - skHeight;
				skCaptured[id] = false;
				skThrown[id] = true;
			}
			else
			{
				skX[id] = player_x + playerWidth + 10;
				skY[id] = player_y + playerHeight - skHeight;
				skCaptured[id] = false;
				skThrown[id] = true;
			}
		}
		else if (skThrowDir[id] == 3)
		{
			if (facing_right)
			{
				skX[id] = player_x + 10;
				skY[id] = player_y + playerHeight + 64;
				skCaptured[id] = false;
				skThrown[id] = true;
				skOnGround[id] = false;
				skVelocityY[id] = 0;
				skThrownVerDir[id] = 1;
			}
			else
			{
				skX[id] = player_x - 10;
				skY[id] = player_y + 64;
				skCaptured[id] = false;
				skThrown[id] = true;
				skOnGround[id] = false;
				skVelocityY[id] = 0;
				skThrownVerDir[id] = 2;
			}
		}
		else if (skThrowDir[id] == 4)
		{
			if (facing_right)
			{
				skX[id] = player_x - playerWidth - 10;
				skY[id] = player_y + playerHeight - skHeight;
				skCaptured[id] = false;
				skThrown[id] = true;
			}
			else
			{
				skX[id] = player_x - 10;
				skY[id] = player_y + playerHeight - skHeight;
				skCaptured[id] = false;
				skThrown[id] = true;
			}
		}

	default:
		break;
	}
}

void animateCharacter(
	Sprite &sprite,
	Texture frames[],  // array of textures
	int frameCount,	   // number of frames
	int &frameIndex,   // current frame index (counter)
	float &frameTimer, // timer for animation speed
	float frameSpeed,  // how fast to animate
	bool facingRight,  // flip sprite?
	float widthScale,
	float heightScale)
{
	frameTimer += frameSpeed;

	if (frameTimer >= 1.0f)
	{
		frameTimer = 0;
		frameIndex++;

		if (frameIndex >= frameCount)
			frameIndex = 0;
	}

	sprite.setTexture(frames[frameIndex]);

	// Handle flipping
	if (facingRight)
		sprite.setScale(-widthScale, heightScale);
	else
		sprite.setScale(widthScale, heightScale);
}

void updateThrownEnemies(
	char **lvl, int screen_x, int screen_y, int cell_size,
	float player_x, float player_y, float playerWidth, float playerHeight, bool facing_right,
	float ghostX[], float ghostY[],
	bool ghostThrown[], int ghostThrowDir[], int ghostThrownVerDir[],
	bool ghostCaptured[], bool ghostOnGround[], int ghostVelocityY[], int ghostCount, bool ghostDead[],
	bool ghostInVac[], bool ghostRight[], float ghostWidth, float ghostHeight,

	float skX[], float skY[], int skCount, float skWidth, float skHeight,
	bool skThrown[], int skThrowDir[], int skThrownVerDir[], bool skInVac[],
	bool skCaptured[], bool skOnGround[], int skVelocityY[], bool skDead[], int &playerScore)
{
	int combCount = 0;

	for (int i = 0; i < ghostCount; i++)
	{
		if (ghostThrown[i])
		{
			if (ghostX[i] >= screen_x - 82 || ghostX[i] <= 0)
			{
				ghostThrown[i] = false;
				ghostDead[i] = true;
				ghostX[i] = -9999;
				ghostY[i] = -9999;
				continue;
			}

			if (ghostThrowDir[i] == 1)
			{
				ghostX[i] += ghostThrownVerDir[i] == 1 ? +5 : -5;
				applyVerticalPhysics(
					lvl, ghostX[i], ghostY[i], (ghostVelocityY[i]),
					ghostOnGround[i], 1.0f, 20.0f, 96.0f, 102.0f, cell_size,
					screen_x, screen_y);
			}
			else if (ghostThrowDir[i] == 2)
			{
				ghostX[i] += 5;
				applyVerticalPhysics(
					lvl, ghostX[i], ghostY[i], (ghostVelocityY[i]),
					ghostOnGround[i], 1.0f, 20.0f, 96.0f, 102.0f, cell_size,
					screen_x, screen_y);
			}
			else if (ghostThrowDir[i] == 3)
			{
				ghostX[i] += ghostThrownVerDir[i] == 1 ? +5 : -5;
				applyVerticalPhysics(
					lvl, ghostX[i], ghostY[i], (ghostVelocityY[i]),
					ghostOnGround[i], 1.0f, 20.0f, 96.0f, 102.0f, cell_size,
					screen_x, screen_y);
			}
			else if (ghostThrowDir[i] == 4)
			{
				ghostX[i] -= 5;
				applyVerticalPhysics(
					lvl, ghostX[i], ghostY[i], (ghostVelocityY[i]),
					ghostOnGround[i], 1.0f, 20.0f, 96.0f, 102.0f, cell_size,
					screen_x, screen_y);
			}

			for (int j = 0; j < ghostCount; j++)
			{
				if (i == j)
					continue; // skip self
				if (ghostDead[j])
					continue;
				if (ghostCaptured[j] || ghostInVac[j])
					continue;

				// ONLY trigger if THIS ghost is thrown AND target is NOT thrown
				if (!ghostThrown[j])
				{
					bool overlap =
						ghostX[i] < ghostX[j] + ghostWidth &&
						ghostX[i] + ghostWidth > ghostX[j] &&
						ghostY[i] < ghostY[j] + ghostHeight &&
						ghostY[i] + ghostHeight > ghostY[j];

					if (overlap)
					{
						combCount += 1;
						cout << "Ghost " << j << " died.\n";
						ghostThrown[j] = true;
						ghostCaptured[j] = false;
						ghostInVac[j] = false;
						ghostDead[j] = false;

						ghostThrowDir[j] = ghostThrowDir[i];
						ghostThrownVerDir[j] = ghostThrownVerDir[i];

						// give same velocity
						ghostVelocityY[j] = ghostVelocityY[i];

						// OPTIONAL slight push
						ghostX[j] += (ghostThrowDir[i] == 4 ? -5 : 5);
					}
				}
			}

			// ----- GHOST -> hits SKELETON -----
			for (int s = 0; s < skCount; s++)
			{
				if (skDead[s])
					continue;
				if (skCaptured[s] || skInVac[s])
					continue;
				if (skThrown[s])
					continue; // already projectile

				bool overlap =
					ghostX[i] < skX[s] + skWidth &&
					ghostX[i] + ghostWidth > skX[s] &&
					ghostY[i] < skY[s] + skHeight &&
					ghostY[i] + ghostHeight > skY[s];

				if (overlap)
				{
					combCount += 1;
					cout << "Skelton " << s << " died.\n";
					skThrown[s] = true;
					skCaptured[s] = false;
					skInVac[s] = false;
					skDead[s] = false;

					skThrowDir[s] = ghostThrowDir[i];
					skThrownVerDir[s] = ghostThrownVerDir[i];
					skVelocityY[s] = ghostVelocityY[i];

					// nudging away
					skX[s] += (ghostThrowDir[i] == 4 ? -5 : 5);
				}
			}
		}
	}

	for (int i = 0; i < skCount; i++)
	{
		if (skThrown[i])
		{
			if (skX[i] >= screen_x - 82 || skX[i] <= 0)
			{
				skThrown[i] = false;
				skDead[i] = true;
				skX[i] = -9999;
				skY[i] = -9999;
				continue;
			}

			if (skThrowDir[i] == 1)
			{
				skX[i] += (skThrownVerDir[i] == 1 ? 5 : -5);
				applyVerticalPhysics(lvl, skX[i], skY[i], skVelocityY[i],
									 skOnGround[i], 1.0f, 20.0f, skWidth, skHeight, cell_size, screen_x, screen_y);
			}
			else if (skThrowDir[i] == 2)
			{
				skX[i] += 5;
				applyVerticalPhysics(lvl, skX[i], skY[i], skVelocityY[i],
									 skOnGround[i], 1.0f, 20.0f, skWidth, skHeight, cell_size, screen_x, screen_y);
			}
			else if (skThrowDir[i] == 3)
			{
				skX[i] += (skThrownVerDir[i] == 1 ? 5 : -5);
				applyVerticalPhysics(lvl, skX[i], skY[i], skVelocityY[i],
									 skOnGround[i], 1.0f, 20.0f, skWidth, skHeight, cell_size, screen_x, screen_y);
			}
			else if (skThrowDir[i] == 4)
			{
				skX[i] -= 5;
				applyVerticalPhysics(lvl, skX[i], skY[i], skVelocityY[i],
									 skOnGround[i], 1.0f, 20.0f, skWidth, skHeight, cell_size, screen_x, screen_y);
			}

			for (int g = 0; g < ghostCount; g++)
			{
				if (ghostDead[g])
					continue;
				if (ghostCaptured[g] || ghostInVac[g])
					continue;
				if (ghostThrown[g])
					continue;

				bool overlap =
					skX[i] < ghostX[g] + ghostWidth &&
					skX[i] + skWidth > ghostX[g] &&
					skY[i] < ghostY[g] + ghostHeight &&
					skY[i] + skHeight > ghostY[g];

				if (overlap)
				{
					combCount += 1;
					cout << "Ghost " << g << " died.\n";
					ghostThrown[g] = true;
					ghostCaptured[g] = false;
					ghostInVac[g] = false;
					ghostDead[g] = false;

					ghostThrowDir[g] = skThrowDir[i];
					ghostThrownVerDir[g] = skThrownVerDir[i];
					ghostVelocityY[g] = skVelocityY[i];

					ghostX[g] += (skThrowDir[i] == 4 ? -5 : 5);
				}
			}

			for (int s = 0; s < skCount; s++)
			{
				if (i == s)
					continue;
				if (skDead[s])
					continue;
				if (skCaptured[s] || skInVac[s])
					continue;
				if (skThrown[s])
					continue;

				bool overlap =
					skX[i] < skX[s] + skWidth &&
					skX[i] + skWidth > skX[s] &&
					skY[i] < skY[s] + skHeight &&
					skY[i] + skHeight > skY[s];

				if (overlap)
				{
					combCount += 1;
					cout << "skelton " << s << " died.\n";
					skThrown[s] = true;
					skCaptured[s] = false;
					skInVac[s] = false;
					skDead[s] = false;

					skThrowDir[s] = skThrowDir[i];
					skThrownVerDir[s] = skThrownVerDir[i];
					skVelocityY[s] = skVelocityY[i];

					skX[s] += (skThrowDir[i] == 4 ? -5 : 5);
				}
			}
		}
	}

	if (combCount == 0)
	{
		playerScore += 0;
	}
	else if (combCount == 1)
	{
		playerScore += 200;
	}
	else if (combCount > 1)
	{
		playerScore += 500;
	}
}

void updatePlayerLifeSystem(
	float &player_x, float &player_y, bool facing_right,
	float playerWidth, float playerHeight,
	float defaultX, float defaultY,
	int &playerLives,
	bool &playerDead,
	int &playerRespawnTimer,

	// Ghost Arrays
	float ghostX[], float ghostY[], bool ghostRight[],
	bool ghostDead[], bool ghostCaptured[], bool ghostInVac[], bool ghostThrown[],
	int ghostCount, float ghostWidth, float ghostHeight,

	// Skeleton Arrays
	float skX[], float skY[], bool skRight[],
	bool skDead[], bool skCaptured[], bool skInVac[], bool skThrown[],
	int skCount, float skWidth, float skHeight, int &playerScore)
{
	// -----------------------------
	// 1. PLAYER IS DEAD → WAIT
	// -----------------------------
	if (playerDead)
	{
		playerRespawnTimer--;

		if (playerRespawnTimer <= 0)
		{
			playerDead = false;
			player_x = defaultX;
			player_y = defaultY;
		}

		return;
	}

	// -----------------------------
	// 2. ACTIVE PLAYER CHECK COLLISION
	// -----------------------------
	float pL = (facing_right ? player_x - playerWidth : player_x);
	float pR = player_x + (facing_right ? 0 : playerWidth);
	float pT = player_y;
	float pB = player_y + playerHeight;

	// -----------------------------
	// GHOST collision
	// -----------------------------
	for (int i = 0; i < ghostCount; i++)
	{
		if (ghostDead[i] || ghostThrown[i])
			continue;
		if (ghostCaptured[i] || ghostInVac[i])
			continue; // <-- NEW: sucking = SAFE

		float gL = (ghostRight[i] ? ghostX[i] - ghostWidth : ghostX[i]);
		float gR = ghostX[i] + (ghostRight[i] ? 0 : ghostWidth);
		float gT = ghostY[i];
		float gB = ghostY[i] + ghostHeight;

		bool overlap =
			pL < gR && pR > gL &&
			pT < gB && pB > gT;

		if (overlap)
		{
			playerLives--;
			playerDead = true;
			playerRespawnTimer = 60;
			playerScore -= 50;

			return;
		}
	}

	// -----------------------------
	// SKELETON collision
	// -----------------------------
	for (int i = 0; i < skCount; i++)
	{
		if (skDead[i] || skThrown[i])
			continue;
		if (skCaptured[i] || skInVac[i])
			continue; // <-- NEW: sucking = SAFE

		float sL = (skRight[i] ? skX[i] - skWidth : skX[i]);
		float sR = skX[i] + (skRight[i] ? 0 : skWidth);
		float sT = skY[i];
		float sB = skY[i] + skHeight;

		bool overlap =
			pL < sR && pR > sL &&
			pT < sB && pB > sT;

		if (overlap)
		{
			playerLives--;
			playerDead = true;
			playerRespawnTimer = 60;
			playerScore -= 50;

			return;
		}
	}
}

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
