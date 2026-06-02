#include<iostream>
#include<windows.h>
#include<cstdlib>
#include<ctime>
#include <conio.h>
using namespace std;

const int mapWidth = 20;
const int mapHeight = 10;
int dirX = 1;
int dirY = 0;
int fruitX, fruitY;
int score = 0;
bool gameOver = false;

struct Point {
	int x, y;

};

class Snake {
private:
	Point* body;
	int length;
	int capacity;

public:
	Snake(int startX, int startY) {
		length = 1;
		capacity = 10;
		body = new Point[capacity];
		body[0].x = startX;
		body[0].y = startY;
		cout << "create a position of snake" << startX << " " << startY << " " << endl;
	}
	~Snake() {		//analysing function
		delete[]body;
		cout << "memory release" << endl;
	}

	void move(int dx, int dy, bool grow) {	//snake
		if (grow) {
			if (length == capacity) {
				int newCap = capacity * 2;
				Point* newBody = new Point[newCap];
				for (int i = 0; i < length; i++) {
					newBody[i] = body[i];	//memory expansion
				}
				delete[] body;
				body = newBody;
				capacity = newCap;
				cout << "exapnd to " << capacity << endl;
			}
			length++;
		}
		for (int i = length - 1; i > 0; i--) {	//body move 
			body[i] = body[i - 1];				
		}
		body[0].x = body[0].x + dx;			//coordinate move
		body[0].y = body[0].y + dy;			//coordinate move
	}


	bool checkcollision() {
		for (int i = 1; i < length; i++) {
			if (body[i].x == body[0].x && body[i].y == body[0].y) {
				return true;
			}
		}
		return false;
	}

	bool checkWallCollision() {
		if (body[0].x < 0 || body[0].x >= mapWidth || body[0].y < 0 || body[0].y >= mapHeight) {
			return true;
		}
		return false;
	}
		
	int getHeadX()const {return body[0].x;}
	int getHeadY()const { return body[0].y;}
	int getLength() const{ return length; }
	int getBodyX(int i) const {return body[i].x;}
	int getBodyY(int i) const {return body[i].y;}
	bool contains(int x, int y) const {		//food cannot genertate in body of snake
		for (int i = 0; i < length; i++) {
			if (body[i].x == x && body[i].y == y) { return true; }
		}
		return false;
	}
};

void SetUp(Snake& snake) {
	srand(time(0));
	do {
		fruitX = rand() % mapWidth;
		fruitY = rand() % mapHeight;

	} while (snake.contains
	(fruitX, fruitY));
}

void Draw(const Snake& snake) {
	system("cls");
	for (int i = 0; i < mapWidth + 2; i++)cout << "#";
	cout << endl;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (x == 0)cout << "#"; //left wall

			if (x == snake.getHeadX() && y == snake.getHeadY()) {
				cout << "O";
			}
			else if (x == fruitX && y == fruitY) {
				cout << "F";	//food
			}
			else {
				bool isBody = false;
				for (int i = 1; i < snake.getLength(); i++) {
					if (x == snake.getBodyX(i) && y == snake.getBodyY(i)) {
						cout << "o";//snake body
						isBody = true;
						break;
					}
				}
				if (!isBody)cout << " ";

			}
			if (x == mapWidth - 1)cout << "#";	//right wall
		}cout << endl;
	}
	for (int i = 0; i < mapWidth + 2; i++) cout << "#";		//lower wall
	cout << endl;
	cout << "Score: " << score << endl;
}
void Input() {
	if (_kbhit()) {
		char key = _getch();
		switch (key) {
		case 'a': case 'A':
			if (dirX != 1) { dirX = -1; dirY = 0; }
			break;
		case 'd': case 'D':
			if (dirX != -1) { dirX = 1; dirY = 0; }
			break;
		case 'w': case 'W':
			if (dirY != 1) { dirX = 0; dirY = -1; }
			break;
		case 's': case 'S':
			if (dirY != -1) { dirX = 0; dirY = 1; }
			break;
		case 'x': case 'X':
			gameOver = true;
			break;
		}
	}
}
void logic(Snake& snake) {
	int newHeadX = snake.getHeadX() + dirX;
	int newHeadY = snake.getHeadY() + dirY;

	bool ateFood = false;
	if (newHeadX == fruitX && newHeadY == fruitY) {
		ateFood = true;
		score += 10;
	}

	snake.move(dirX, dirY, ateFood);

	//collision validation
	if (snake.checkWallCollision() || snake.checkcollision()) {
		
			gameOver = true;
		return;
	}

	if (ateFood) {
		do {
			fruitX = rand() % mapWidth;
			fruitY = rand() % mapHeight;
		} while (snake.contains(fruitX, fruitY));
	}
}
int main() {
	system("chcp 65001 > nul");
	Snake snake(5, 5);
	SetUp(snake);

	while (!gameOver) {
		Draw(snake);
		Input();
		logic(snake);
		Sleep(100);		//control spped
	}

	cout << "Game Over! Final Score: " << score << endl;
	return 0;

}