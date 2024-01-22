#include "raylib.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <string>
#define MAX_PIPES 10

struct Player
{
	Vector2 pos;
	Texture zicc;
	float velocity;
	Color color;
};

struct Pipe
{
	Rectangle rect;
	float velocity;
	bool active;
};

static Player player = { 0 };
static Pipe pipes[2][MAX_PIPES] = { 0 };

static const int screenWidth = 960;
static const int screenHeight = 540;
static const int Y_GAP = 190;
static const int X_GAP = 230;
static const int PIPE_HEIGHT = screenHeight - Y_GAP;
static const int PIPE_WIDTH = 65;
static const int MARGIN = 20;
static int farthestRight = MAX_PIPES - 1;
static int score = 0;
static int highscore = 0;
static bool gameOver = false;

static void InitGame();
static void UpdateGame();
static void DrawGame();
static void UpdatePlayer();
static void UpdatePipes();
static void CheckCollisions();
static void CheckPastPipe();

int main()
{
	InitWindow(screenWidth, screenHeight, "FLAPPY FLUP");
	InitGame();
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		UpdateGame();
		DrawGame();
	}
	CloseWindow();

	return 0;
}

void InitGame()
{
	player.zicc = LoadTexture("resources/zicc.png"); //change this to the file path in windows explorer if the texture is not loading. use / not \
	player.zicc.width = 55;
	player.zicc.height = 55;
	player.velocity = 3;
	player.pos = Vector2(100, screenHeight / 2);
	for (int i = 0; i < MAX_PIPES; i++)
	{
		int TOP_PIPE_Y = GetRandomValue(-PIPE_HEIGHT + MARGIN, -MARGIN);
		pipes[0][i].rect = Rectangle(screenWidth + X_GAP * i, TOP_PIPE_Y, PIPE_WIDTH, PIPE_HEIGHT);
		pipes[0][i].velocity = -150;
		pipes[0][i].active = true;

		pipes[1][i].rect = Rectangle(screenWidth + X_GAP * i, TOP_PIPE_Y + PIPE_HEIGHT + Y_GAP, PIPE_WIDTH, PIPE_HEIGHT);
		pipes[1][i].velocity = -150;
		pipes[1][i].active = true;
	}
	if (score > highscore) highscore = score;
	score = 0;
	farthestRight = MAX_PIPES - 1;
}

void UpdateGame()
{
	if (gameOver && IsKeyPressed(KEY_ENTER))
	{
		InitGame();
		gameOver = false;
	}
	else if (gameOver) return;

	UpdatePlayer();
	UpdatePipes();
	CheckCollisions();
	CheckPastPipe();
}

void DrawGame()
{
	BeginDrawing();

	ClearBackground(BLACK);
	if (!gameOver)
	{
		for (int i = 0; i < MAX_PIPES; i++)
		{
			DrawRectangle(pipes[0][i].rect.x, pipes[0][i].rect.y, pipes[0][i].rect.width, pipes[0][i].rect.height, GREEN);
			DrawRectangle(pipes[1][i].rect.x, pipes[1][i].rect.y, pipes[1][i].rect.width, pipes[1][i].rect.height, GREEN);
		}
		DrawTexture(player.zicc, player.pos.x, std::roundf(player.pos.y), WHITE);
		DrawText(TextFormat("Score: %i", score), 10, 10, 50, RED);
		DrawText(TextFormat("Highscore: %i", highscore), screenWidth - MeasureText(TextFormat("Highscore: %i", highscore), 50) - 10, 10, 50, RED);
	}
	else
	{
		const char* text1 = "GAME OVER!";
		const char* text2 = "ENTER TO PLAY AGAIN";
		DrawText(text1, screenWidth / 2 - MeasureText(text1, 50) / 2, screenHeight / 2 - 75, 50, RAYWHITE);
		DrawText(text2, screenWidth / 2 - MeasureText(text2, 50) / 2, screenHeight / 2, 50, RAYWHITE);
	}
	EndDrawing();
}

void UpdatePlayer()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		player.velocity = -350;
	}
	player.pos.y += player.velocity * GetFrameTime();
	player.velocity += 1000 * GetFrameTime();
	if (player.pos.y + player.zicc.height >= screenHeight)
	{
		player.pos.y = screenHeight - player.zicc.height;
	}
}

void UpdatePipes()
{
	for (int i = 0; i < MAX_PIPES; i++)
	{
		if (pipes[0][i].rect.x <= -PIPE_WIDTH)
		{
			int TOP_PIPE_Y = GetRandomValue(-PIPE_HEIGHT + MARGIN, -MARGIN);
			pipes[0][i].rect.x = pipes[0][farthestRight].rect.x + X_GAP;
			pipes[0][i].rect.y = TOP_PIPE_Y;
			pipes[0][i].active = true;
			pipes[1][i].rect.x = pipes[1][farthestRight].rect.x + X_GAP;
			pipes[1][i].rect.y = TOP_PIPE_Y + PIPE_HEIGHT + Y_GAP;
			farthestRight++;
			if (farthestRight > MAX_PIPES - 1)
				farthestRight = 0;
		}
		pipes[0][i].rect = Rectangle(pipes[0][i].rect.x + pipes[0][i].velocity * GetFrameTime(), pipes[0][i].rect.y, PIPE_WIDTH, PIPE_HEIGHT);
		pipes[1][i].rect = Rectangle(pipes[1][i].rect.x + pipes[1][i].velocity * GetFrameTime(), pipes[1][i].rect.y, PIPE_WIDTH, PIPE_HEIGHT);
	}

}

void CheckCollisions()
{
	for (int i = 0; i < MAX_PIPES; i++)
	{
		Rectangle ziccRect = Rectangle(player.pos.x, player.pos.y, player.zicc.width, player.zicc.height);
		if (CheckCollisionRecs(pipes[0][i].rect, ziccRect) || CheckCollisionRecs(pipes[1][i].rect, ziccRect))
		{
			gameOver = true;
			break;
		}
	}
}

void CheckPastPipe()
{
	for (int i = 0; i < MAX_PIPES; i++)
	{
		if (player.pos.x > pipes[0][i].rect.x && pipes[0][i].active)
		{
			score++;
			pipes[0][i].active = false;
			break;
		}
	}
}
