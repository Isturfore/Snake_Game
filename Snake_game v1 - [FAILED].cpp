#include <iostream>
#include <ctime>
#include <chrono>
#include <conio.h>
#include <thread>
#include <deque>

using namespace std;

//////////////////////////////////////////////////////////// Variables & Arrays

bool isOver = false;

const int ROWS = 30;
const int COLS = 30;

int AppleROW;
int AppleCOL;

int prev_AppleROW;
int prev_AppleCOL;

int global_counter = 0;

char field[ROWS][COLS];

//////////////////////////////////////////////////////////// Structures & Classes & Enums

enum class Buttons { W, A, S, D, NONE };
class Coordinates
{
public:
	Coordinates(int headROW, int headCOL)
	{
		this->headROW = headROW;
		this->headCOL = headCOL;
	}
	void Set(int headROW, int headCOL)
	{
		this->headROW = headROW;
		this->headCOL = headCOL;
	}

	int get_ROW() { return headROW; }
	int get_COL() { return headCOL; }
private:
	int headROW;
	int headCOL;
};

//////////////////////////////////////////////////////////// System & One-Time Functions

void fill_array()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int y = 0; y < COLS; y++)
		{
			if (i == 0 || i == ROWS - 1 || y == 0 || y == COLS - 1)
			{
				field[i][y] = '#';
			}
			else
			{
				field[i][y] = ' ';
			}
		}
	}
	field[ROWS / 2][COLS / 2] = 'P';
}
void console_clear()
{
#ifdef _WIN32
	system("cls"); // Для Windows
#else
	system("clear"); // Для Linux и macOS
#endif
}
Buttons key_logger()
{
	if (_kbhit())
	{
		int key = _getch();

		switch (key)
		{
		case 87: case 119: return Buttons::W;
		case 65: case 97:  return Buttons::A;
		case 83: case 115: return Buttons::S;
		case 68: case 100: return Buttons::D;
		}
	}
	return Buttons::NONE;
}
Coordinates search(char ch)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int y = 0; y < COLS; y++)
		{
			if (field[i][y] == ch) { return Coordinates(i, y); }
		}
	}
	return Coordinates(-1, -1);
}

Buttons prevbutton = Buttons::NONE; // exception
Coordinates oldhead(0, 0);
deque<Coordinates> snake_array; // exception

//////////////////////////////////////////////////////////// Functions

void draw_field()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int y = 0; y < COLS; y++)
		{
			cout << field[i][y];
		}
		cout << endl;
	}
}
void game_logic()
{
	Coordinates head_coords = search('P');

	if (((head_coords.get_ROW() == prev_AppleROW) && (head_coords.get_COL() == prev_AppleCOL)) || global_counter == 0)
	{
		AppleROW = 1 + rand() % (ROWS - 2);
		AppleCOL = 1 + rand() % (COLS - 2);
		while ((AppleROW == head_coords.get_ROW()) && (AppleCOL == head_coords.get_COL()))
		{
			AppleROW = 1 + rand() % (ROWS - 2);
			AppleCOL = 1 + rand() % (COLS - 2);
		}

		prev_AppleROW = AppleROW;
		prev_AppleCOL = AppleCOL;

		field[AppleROW][AppleCOL] = 'O';
	}

	draw_field();

	while (global_counter == 0 && !_kbhit()) { this_thread::sleep_for(chrono::milliseconds(15)); continue; }

	switch (key_logger())
	{
	case Buttons::W:
		field[head_coords.get_ROW()][head_coords.get_COL()] = ' ';
		field[head_coords.get_ROW() - 1][head_coords.get_COL()] = 'P';

		prevbutton = Buttons::W;
		break;
	case Buttons::A:
		field[head_coords.get_ROW()][head_coords.get_COL()] = ' ';
		field[head_coords.get_ROW()][head_coords.get_COL() - 1] = 'P';

		prevbutton = Buttons::A;
		break;
	case Buttons::S:
		field[head_coords.get_ROW()][head_coords.get_COL()] = ' ';
		field[head_coords.get_ROW() + 1][head_coords.get_COL()] = 'P';

		prevbutton = Buttons::S;
		break;
	case Buttons::D:
		field[head_coords.get_ROW()][head_coords.get_COL()] = ' ';
		field[head_coords.get_ROW()][head_coords.get_COL() + 1] = 'P';

		prevbutton = Buttons::D;
		break;
	case Buttons::NONE:

		switch (prevbutton)
		{
		case Buttons::W:
			field[head_coords.get_ROW()][head_coords.get_COL()] = ' ';
			field[head_coords.get_ROW() - 1][head_coords.get_COL()] = 'P';

			break;
		case Buttons::A:
			field[head_coords.get_ROW()][head_coords.get_COL()] = ' ';
			field[head_coords.get_ROW()][head_coords.get_COL() - 1] = 'P';

			break;
		case Buttons::S:
			field[head_coords.get_ROW()][head_coords.get_COL()] = ' ';
			field[head_coords.get_ROW() + 1][head_coords.get_COL()] = 'P';

			break;
		case Buttons::D:
			field[head_coords.get_ROW()][head_coords.get_COL()] = ' ';
			field[head_coords.get_ROW()][head_coords.get_COL() + 1] = 'P';

			break;
		case Buttons::NONE:
			
			break;
		}

		break;
	}

	head_coords = search('P');
	snake_array.push_front(Coordinates(head_coords.get_ROW(), head_coords.get_COL()));

	if (head_coords.get_ROW() == AppleROW && head_coords.get_COL() == AppleCOL)
	{
		snake_array.push_back(Coordinates(AppleROW, AppleCOL));
		int willBe = 0;
		if (willBe == willBe + 1)
		{
			switch (prevbutton)
			{
			case Buttons::W:
				field[head_coords.get_ROW() + 1][head_coords.get_COL()] = '*';
				break;
			case Buttons::A:
				field[head_coords.get_ROW()][head_coords.get_COL() + 1] = '*';
				break;
			case Buttons::S:
				field[head_coords.get_ROW() - 1][head_coords.get_COL()] = '*';
				break;
			case Buttons::D:
				field[head_coords.get_ROW()][head_coords.get_COL() - 1] = '*';
				break;
			}
		}
		willBe++;
	}
	else
	{
		switch (prevbutton)
		{
		case Buttons::W:
			field[head_coords.get_ROW() + 1][head_coords.get_COL()] = '*';
			field[oldhead.get_ROW()][oldhead.get_COL()] = '*';
			field[snake_array.back().get_ROW()][snake_array.back().get_COL()] = ' ';

			break;
		case Buttons::A:
			field[head_coords.get_ROW()][head_coords.get_COL() + 1] = '*';
			field[oldhead.get_ROW()][oldhead.get_COL()] = '*';
			field[snake_array.back().get_ROW()][snake_array.back().get_COL()] = ' ';
			break;
		case Buttons::S:
			field[head_coords.get_ROW() - 1][head_coords.get_COL()] = '*';
			field[oldhead.get_ROW()][oldhead.get_COL()] = '*';
			field[snake_array.back().get_ROW()][snake_array.back().get_COL()] = ' ';
			break;
		case Buttons::D:
			field[head_coords.get_ROW()][head_coords.get_COL() - 1] = '*';
			field[oldhead.get_ROW()][oldhead.get_COL()] = '*';
			field[snake_array.back().get_ROW()][snake_array.back().get_COL()] = ' ';
			break;
		}
	}
	Coordinates oldhead = search('P');
}
void game_end()
{
	if (search('P').get_ROW() == 0 || search('P').get_ROW() == ROWS - 1 || search('P').get_COL() == 0 || search('P').get_COL() == COLS - 1)
	{
		isOver = true;
	}
}

//////////////////////////////////////////////////////////// Main Function

int main()
{
	srand(unsigned(time(NULL)));



	fill_array();

	while (!isOver)
	{
		game_logic();
		game_end();
		console_clear();

		global_counter++;
		this_thread::sleep_for(chrono::milliseconds(5));
	}
	
	return 0;
}
