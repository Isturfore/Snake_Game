#include <iostream>
#include <vector>
#include <ctime>
#include <conio.h>
#include <thread>
#include <chrono>
#include <cmath>

const int size = 30;
typedef std::pair<int, int> Segment;
enum Buttons { W, A, S, D, NONE };





class Snake
{
public:
    friend class Food;
    friend class Game;
    Snake()
    {
        snake_coords_array.push_back(Segment(size / 2, size / 2));
    }
private:
    std::vector<Segment> snake_coords_array;
};


class Food
{
public:
    friend class Game;
    Food()
    {
        food_coords.first = 14;
        food_coords.second = 15;
    }
    void food_update(Game& obj_game, Snake obj_snake);
private:
    Segment food_coords;
};


class Game
{
public:
    friend class Food;
    Game(Snake& obj_snake, Food& obj_food)
    {
        score = 0;
        isWin = false;
        isOver = false;
        old_button = Buttons::NONE;
        old = Segment(size / 2, size / 2);
        prev_snake = obj_snake.snake_coords_array;

        for (int i = 0; i < size; i++)
        {
            field[i] = new char[size];
        }
        for (int i = 0; i < size; i++)
        {
            for (int y = 0; y < size; y++)
            {
                field[i][y] = ' ';
                if (i == obj_snake.snake_coords_array.at(0).first && y == obj_snake.snake_coords_array.at(0).second) { field[i][y] = 'O'; }
                if (i == obj_food.food_coords.first && y == obj_food.food_coords.second) { field[i][y] = 'K'; }
                if (i == 0 || i == size - 1 || y == 0 || y == size - 1) { field[i][y] = '#'; }
            }
        }
    }
    ~Game()
    {
        for (int i = 0; i < size; i++)
        {
            delete[] field[i];
            field[i] = nullptr;
        }
        delete[] field;
        field = nullptr;
    }

    void Draw()
    {
        for (int i = 0; i < size; i++)
        {
            for (int y = 0; y < size; y++)
            {
                std::cout << field[i][y];
                if (i == 15 && y == 29)
                {
                    std::cout << "\t\t\t\t\tScore: " << score;
                }
            }
            std::cout << std::endl;
        }
    }
    Buttons key_log()
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
    void key_insert(Snake& obj_snake)
    {
        Buttons pressed = key_log();
        if (pressed != NONE) 
        {
            // Запрещаем разворот на 180°
            if (!((old_button == W && pressed == S) ||
                (old_button == S && pressed == W) ||
                (old_button == A && pressed == D) ||
                (old_button == D && pressed == A))) 
            {
                old_button = pressed;
            }
        }

        // Фикс: запрет движения, если направление не выбрано
        if (old_button == NONE) return;

        Segment new_head = obj_snake.snake_coords_array[0]; // Копируем голову

        // Двигаем голову
        switch (old_button)
        {
        case W: new_head.first--; break;
        case A: new_head.second--; break;
        case S: new_head.first++; break;
        case D: new_head.second++; break;
        case NONE: return; // Не двигаемся, если нет направления
        }

        // Сдвигаем хвост
        for (int i = obj_snake.snake_coords_array.size() - 1; i > 0; i--)
        {
            obj_snake.snake_coords_array[i] = obj_snake.snake_coords_array[i - 1];
        }
        obj_snake.snake_coords_array[0] = new_head; // Обновляем голову
    }
    void clear()
    {
#ifdef _WIN32
        system("cls");
#elif __APPLE__ || __linux__
        system("clear");
#else
        std::cout << "\033[2J\033[1;1H";
#endif 
    }
    bool is_Ate(Snake& obj_snake, Food obj_food) 
    {
        if (obj_snake.snake_coords_array[0] == obj_food.food_coords)
        {
            Segment new_tail = obj_snake.snake_coords_array.back();
            obj_snake.snake_coords_array.push_back(new_tail);
            score += 10;
            return true;
        }
        return false;
    }
    void field_update(Snake& obj_snake, bool result, Food obj_food)
    {
        // 1. Очищаем предыдущее положение змейки
        for (const auto& seg : prev_snake)
        {
            field[seg.first][seg.second] = ' ';
        }

        // 2. Рисуем новую змейку
        for (int i = 0; i < obj_snake.snake_coords_array.size(); i++)
        {
            int x = obj_snake.snake_coords_array[i].first;
            int y = obj_snake.snake_coords_array[i].second;
            if (i == 0)
            {
                field[x][y] = 'O'; // Голова
            }
            else 
            {
                field[x][y] = 'o'; // Тело
            }
        }

        // 3. Рисуем еду (если не съедена)
        if (!result)
        {
            field[obj_food.food_coords.first][obj_food.food_coords.second] = 'K';
        }

        // 4. Сохраняем текущее состояние для следующего кадра
        prev_snake = obj_snake.snake_coords_array;
    }
    void over(Snake& obj_snake)
    {
        if ((obj_snake.snake_coords_array[0].first >= size - 1 || obj_snake.snake_coords_array[0].first <= 0) || (obj_snake.snake_coords_array[0].second >= size - 1 || obj_snake.snake_coords_array[0].second <= 0)) { isOver = true; }

        for (int i = 1; i < obj_snake.snake_coords_array.size() - 1; i++)
        {
            if (obj_snake.snake_coords_array[0] == obj_snake.snake_coords_array[i]) { isOver = true; }
        }
    }
    void is_Win(Snake& obj_snake)
    {
        if (obj_snake.snake_coords_array.size() >= pow(size - 2, 2)) { isWin = true; }
    }

    bool Get_isOver() { return isOver; }
    bool Get_isWin() { return isWin; }
private:
    int score;
    bool isOver;
    std::vector<Segment> prev_snake;
    Segment temp;
    Buttons old_button;
    bool isWin;
    Segment old;
    char** field = new char* [size];
};





int main()
{
    srand(unsigned(time(NULL)));

    Snake snake;
    Food food;
    Game game(snake, food);

    game.Draw();
    while (!_kbhit()) {}
    game.clear();

    while (!game.Get_isOver() && !game.Get_isWin())
    {
        game.Draw();
        game.key_insert(snake);
        bool result = game.is_Ate(snake, food);
        if (result)
        {
            food.food_update(game, snake);
        }
        game.field_update(snake, result, food);
        game.is_Win(snake);
        game.over(snake);
        game.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    return 0;
}

void Food::food_update(Game& obj_game, Snake obj_snake)
{
    bool is_valid;
    do {
        is_valid = true;
        food_coords.first = 1 + std::rand() % 28;
        food_coords.second = 1 + std::rand() % 28;

        for (const auto& seg : obj_snake.snake_coords_array)
        {
            if (food_coords == seg)
            {
                is_valid = false;
                break;
            }
        }
    } while (!is_valid);

    obj_game.field[food_coords.first][food_coords.second] = 'K';
}
