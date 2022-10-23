#include <raylib.h>

struct Ball 
{
	float x, y;
	float speedX = 500;
	float speedY = 500;
	float radius = 8;

	void Draw()
	{
		DrawCircle(x, y, radius, WHITE);
	}
};

struct Paddle
{
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect()
	{
		return Rectangle{ x - width / 2, y - height / 2, 20, 200 };
	}

	void Draw()
	{
		DrawRectangleRec(GetRect(), WHITE);
	}
};

int main()
{
	const int WIDTH = 1920;
	const int HEIGHT = 1080;

	InitWindow(WIDTH, HEIGHT, "Pong by ImOsiris :)");
	SetWindowState(FLAG_VSYNC_HINT);

	Ball ball;

	ball.x = WIDTH / 2;
	ball.y = HEIGHT / 2;

	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = HEIGHT / 2;
	leftPaddle.width = 20;
	leftPaddle.height = 200;
	leftPaddle.speed = 20;
	int leftScore = 0;

	Paddle rightPaddle;
	rightPaddle.x = WIDTH - 50;
	rightPaddle.y = HEIGHT / 2;
	rightPaddle.width = 20;
	rightPaddle.height = 200;
	rightPaddle.speed = 20;
	int rightScore = 0;

	const char* winnerText = nullptr;
	
	// MAIN PROGRAM LOOP
	while (!WindowShouldClose())
	{

		// BALL MOVEMENT
		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		if (ball.y > HEIGHT)
		{
			ball.y = HEIGHT;
			ball.speedY *= -1;
		}
		else if (ball.y < 0)
		{
			ball.y = 0;
			ball.speedY *= -1;
		}

		// WALL STUFF
		if (ball.x < 0)
		{

			ball.x = WIDTH / 2;
			ball.y = HEIGHT / 2;
			ball.speedX *= -1;
			rightScore += 1;
		}

		if (ball.x > WIDTH)
		{
			ball.x = WIDTH / 2;
			ball.y = HEIGHT / 2;
			ball.speedX *= -1;
			leftScore += 1;
		}

		// WIN CONDITION
		if (rightScore == 1)
		{
			winnerText = "Right Player Wins!";
		}
		else if (leftScore == 1)
		{
			winnerText = "Left Player Wins!";
		}

		if (winnerText && IsKeyPressed(KEY_SPACE))
		{
			ball.x = WIDTH / 2;
			ball.y = HEIGHT / 2;
			ball.speedX = 500;
			ball.speedY = 500;
			leftScore = 0;
			rightScore = 0;
			winnerText = nullptr;
		}

		// LEFT PADDLE MOVEMENT
		if (IsKeyDown(KEY_W) && leftPaddle.y >= 100)
		{
			leftPaddle.y -= leftPaddle.speed;
		}
		else if (IsKeyDown(KEY_S) && leftPaddle.y <= HEIGHT - 100)
		{
			leftPaddle.y += leftPaddle.speed;
		}

		// RIGHT PADDLE MOVEMENT
		if (IsKeyDown(KEY_UP) && rightPaddle.y >= 100)
		{
			rightPaddle.y -= rightPaddle.speed;
		}
		else if (IsKeyDown(KEY_DOWN) && rightPaddle.y <= HEIGHT - 100)
		{
			rightPaddle.y += rightPaddle.speed;
		}
		
		// COLLISION
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect()))
		{
			if (ball.speedX < 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect()))
		{
			if (ball.speedX > 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
			}
		}

		BeginDrawing();
			ClearBackground(BLACK);

			ball.Draw();
			leftPaddle.Draw();
			rightPaddle.Draw();

			DrawText(TextFormat("%i", leftScore), 400, HEIGHT / 2 - 85, 200, GRAY);
			DrawText(TextFormat("%i", rightScore), WIDTH - 400, HEIGHT / 2 - 85, 200, GRAY);

			if (winnerText)
			{
				int textWidth = MeasureText(winnerText, 100);
				DrawText(winnerText, WIDTH / 2 - textWidth / 2, HEIGHT / 2, 100, YELLOW);
			}

			DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}