#include <ncurses.h>
#include <vector>
#include <thread>
#include <random>
#include <ctime>
#define DELAY	150

bool game_over = false;
bool new_food = true;
//int x, y;
int width, height;
int score = 0;

struct punkt{
	int x;
	int y;
};
punkt food_punkt;

enum direction{
	RIGHT,
	LEFT, 
	UP,
       	DOWN
}move_to;

std::vector<punkt> snake(3);

void setup()
{
	for (int i = 0; i < snake.size(); i++)
	{
		snake[i].x = (COLS/2) + i ;
		snake[i].y = (LINES/2) ;

	move_to = RIGHT;
	}
}

void check()
{
	getmaxyx(stdscr, height, width);
	punkt head = snake[snake.size()-1];
	if(head.x < 0 || head.x > width || head.y < 0 || head.y > height)
		game_over = true;
	for(int i = 0; i < snake.size()-1; i++)
	{
		if (head.x == snake[i].x && head.y == snake[i].y)
			game_over = true;
	}
}


void move()
{


	int x_new, y_new;
	switch(move_to)
	{
		case RIGHT:
				x_new = (snake[snake.size()-1].x)+1;
				y_new = snake[snake.size()-1].y;
				snake.push_back({x_new, y_new});
				if(x_new == food_punkt.x && y_new == food_punkt.y)
				{
					new_food = true;
					score++;
				}
				else
					snake.erase(snake.begin());
				break;
		case LEFT:
				
				x_new = (snake[snake.size()-1].x)-1;
				y_new = snake[snake.size()-1].y;
				snake.push_back({x_new, y_new });
				if(x_new == food_punkt.x && y_new == food_punkt.y)
				{
					new_food = true;
					score++;
				}
				else
					snake.erase(snake.begin());
				break;
		case UP:
				x_new = snake[snake.size()-1].x;
				y_new = (snake[snake.size()-1].y)-1;
				snake.push_back({x_new, y_new });
				if(x_new == food_punkt.x && y_new == food_punkt.y)
				{
					new_food = true;
					score++;

				}
				else
					snake.erase(snake.begin());
				break;
		case DOWN:
				x_new = snake[snake.size()-1].x;
				y_new = (snake[snake.size()-1].y)+1;
				snake.push_back({x_new, y_new });
				if(x_new == food_punkt.x && y_new == food_punkt.y)
				{
					new_food = true;
					score++;
				}
				else
					snake.erase(snake.begin());
				break;
	}
	refresh();

}

void food()
{
	getmaxyx(stdscr, height, width);
	std::srand(std::time(nullptr));
	int x_food = std::rand()% width;
	int y_food = std::rand()% height;
	for(int i = 0; i < snake.size()-1; i++)
	{
		if(x_food == snake[i].x && y_food == snake[i].y)
			food();
	}
	food_punkt.x = x_food;
	food_punkt.y = y_food;
	new_food = false;
}


void draw()
{
		erase();
		for(int i = 0; i < snake.size(); i++)
		{
			mvaddch(snake[i].y, snake[i].x, '*');
		}
		
		mvaddch(food_punkt.y, food_punkt.x, 'x');
		refresh();
}

int main()
{
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	cbreak();
	nodelay(stdscr, TRUE);
	curs_set(0);
	setup();

	while(!game_over)
	{
		if(new_food)
			food();
		draw();
		check();
		std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
		switch(getch())
		{
			case KEY_RIGHT:
				if(move_to != LEFT)
					move_to = RIGHT;
				break;
			case KEY_LEFT:
				if(move_to != RIGHT)
					move_to = LEFT;
				break;
			case KEY_DOWN:
				if(move_to != UP)
					move_to = DOWN;
				break;
			case KEY_UP:
				if(move_to != DOWN)
					move_to = UP;
				break;
			case 'q':
				game_over = true;
		}

		move();

	}
	erase();
	move(height/2, width/2);
	printw("%d", score);

	nodelay(stdscr, false);
	getch();
	endwin();
}




