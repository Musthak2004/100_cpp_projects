#include <iostream>
#include <vector>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;
const char WALL = '#';
const char SNAKE_HEAD = 'O';
const char SNAKE_BODY = 'o';
const char FOOD = '@';
const char EMPTY = ' ';

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

struct Point
{
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}

    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }
};

class SnakeGame
{
private:
    deque<Point> snake;
    Point food;
    Direction dir;
    Direction nextDir;
    int score;
    int highScore;
    int level;
    int speed;
    bool gameOver;
    bool paused;
    bool speedChanged;

    HANDLE hConsole;
    CONSOLE_CURSOR_INFO cursorInfo;

    void gotoxy(int x, int y)
    {
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(hConsole, coord);
    }

    void setColor(int color)
    {
        SetConsoleTextAttribute(hConsole, color);
    }

    void hideCursor()
    {
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

    void showCursor()
    {
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

    void spawnFood()
    {
        vector<Point> freeCells;
        for (int y = 1; y < HEIGHT - 1; y++)
        {
            for (int x = 1; x < WIDTH - 1; x++)
            {
                bool occupied = false;
                for (size_t i = 0; i < snake.size(); i++)
                {
                    if (snake[i].x == x && snake[i].y == y)
                    {
                        occupied = true;
                        break;
                    }
                }
                if (!occupied)
                    freeCells.push_back(Point(x, y));
            }
        }

        if (freeCells.empty())
        {
            gameOver = true;
            return;
        }

        food = freeCells[rand() % freeCells.size()];
    }

public:
    SnakeGame() : score(0), highScore(0), level(1), speed(150),
                  gameOver(false), paused(false), speedChanged(false)
    {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        hideCursor();

        srand((unsigned int)time(nullptr));
    }

    void init()
    {
        snake.clear();
        int startX = WIDTH / 2;
        int startY = HEIGHT / 2;

        snake.push_back(Point(startX, startY));
        snake.push_back(Point(startX - 1, startY));
        snake.push_back(Point(startX - 2, startY));

        dir = RIGHT;
        nextDir = RIGHT;
        score = 0;
        level = 1;
        speed = 150;
        gameOver = false;
        paused = false;

        spawnFood();
    }

    void draw()
    {
        stringstream ss;
        ss << "  SCORE: " << setw(4) << score
           << "   HIGH: " << setw(4) << highScore
           << "   LEVEL: " << level
           << "   SPEED: " << speed << "ms";

        gotoxy(0, 0);
        setColor(15);
        cout << ss.str() << string(WIDTH - (int)ss.str().length() + 4, ' ');

        setColor(8);
        for (int x = 0; x < WIDTH; x++)
        {
            gotoxy(x + 2, 1);
            cout << WALL;
            gotoxy(x + 2, HEIGHT);
            cout << WALL;
        }
        for (int y = 1; y <= HEIGHT; y++)
        {
            gotoxy(2, y);
            cout << WALL;
            gotoxy(WIDTH + 1, y);
            cout << WALL;
        }

        setColor(14);
        gotoxy(food.x + 2, food.y);
        cout << FOOD;

        for (size_t i = 0; i < snake.size(); i++)
        {
            gotoxy(snake[i].x + 2, snake[i].y);
            if (i == 0)
            {
                setColor(10);
                cout << SNAKE_HEAD;
            }
            else
            {
                int shade = 10 - (i % 3);
                if (shade < 8) shade = 10;
                setColor(shade);
                cout << SNAKE_BODY;
            }
        }

        if (paused)
        {
            setColor(12);
            gotoxy(WIDTH / 2 - 4, HEIGHT / 2);
            cout << "PAUSED";
        }
    }

    void input()
    {
        if (_kbhit())
        {
            int ch = _getch();
            if (ch == 224)
            {
                ch = _getch();
                switch (ch)
                {
                case 72:
                    if (dir != DOWN) nextDir = UP;
                    break;
                case 80:
                    if (dir != UP) nextDir = DOWN;
                    break;
                case 75:
                    if (dir != RIGHT) nextDir = LEFT;
                    break;
                case 77:
                    if (dir != LEFT) nextDir = RIGHT;
                    break;
                }
            }
            else if (ch == 'p' || ch == 'P')
            {
                paused = !paused;
            }
            else if (ch == 27)
            {
                gameOver = true;
            }
        }
    }

    void update()
    {
        if (paused)
            return;

        dir = nextDir;

        Point newHead = snake.front();
        switch (dir)
        {
        case UP:    newHead.y--; break;
        case DOWN:  newHead.y++; break;
        case LEFT:  newHead.x--; break;
        case RIGHT: newHead.x++; break;
        default: break;
        }

        if (newHead.x <= 0 || newHead.x >= WIDTH - 1 ||
            newHead.y <= 1 || newHead.y >= HEIGHT)
        {
            gameOver = true;
            return;
        }

        for (size_t i = 0; i < snake.size(); i++)
        {
            if (snake[i] == newHead)
            {
                gameOver = true;
                return;
            }
        }

        snake.push_front(newHead);

        if (newHead == food)
        {
            score += 10;
            if (score > highScore)
                highScore = score;

            level = (score / 50) + 1;
            int newSpeed = 150 - (level - 1) * 15;
            if (newSpeed < 30) newSpeed = 30;
            if (newSpeed != speed)
            {
                speed = newSpeed;
                speedChanged = true;
            }

            spawnFood();
        }
        else
        {
            snake.pop_back();
        }
    }

    void showGameOver()
    {
        system("cls");

        setColor(12);
        cout << "\n\n";
        cout << "  " << string(30, '=') << "\n";
        cout << "  " << string(10, ' ') << "GAME OVER" << "\n";
        cout << "  " << string(30, '=') << "\n\n";

        setColor(15);
        cout << "    Final Score:  " << score << "\n";
        cout << "    High Score:   " << highScore << "\n";
        cout << "    Level:        " << level << "\n";
        cout << "    Snake Length: " << snake.size() << "\n\n";

        setColor(14);
        cout << "  " << string(30, '-') << "\n";
        cout << "    1. Play Again\n";
        cout << "    2. Main Menu\n";
        cout << "    0. Exit\n";
        cout << "  " << string(30, '-') << "\n\n";
        cout << "  Choose: ";
    }

    void showWelcome()
    {
        system("cls");
        setColor(10);
        cout << "\n\n";
        cout << "    " << string(34, '=') << "\n";
        cout << "    " << string(8, ' ') << "SNAKE GAME" << "\n";
        cout << "    " << string(34, '=') << "\n\n";

        setColor(15);
        cout << "    Controls:\n";
        cout << "      Arrow Keys  - Move snake\n";
        cout << "      P           - Pause/Resume\n";
        cout << "      ESC         - Quit game\n\n";

        cout << "    Rules:\n";
        cout << "      Eat food (@) to grow and score\n";
        cout << "      Don't hit walls or yourself\n";
        cout << "      Speed increases every 50 points\n\n";

        setColor(14);
        cout << "    " << string(34, '-') << "\n";
        cout << "      1. Start Game\n";
        cout << "      2. View High Score\n";
        cout << "      0. Exit\n";
        cout << "    " << string(34, '-') << "\n\n";
        cout << "    Choose: ";
    }

    void showHighScore()
    {
        system("cls");
        setColor(14);
        cout << "\n\n";
        cout << "  " << string(30, '=') << "\n";
        cout << "  " << string(7, ' ') << "HIGH SCORE" << "\n";
        cout << "  " << string(30, '=') << "\n\n";

        setColor(15);
        cout << "    High Score: " << highScore << "\n\n";

        setColor(14);
        cout << "  Press any key to return...";
        _getch();
    }

    void run()
    {
        int choice;
        do
        {
            showWelcome();
            cin >> choice;
            cin.ignore();

            switch (choice)
            {
            case 1:
                init();
                play();
                break;
            case 2:
                showHighScore();
                break;
            case 0:
                system("cls");
                setColor(15);
                cout << "Thanks for playing!\n";
                showCursor();
                return;
            }
        } while (true);
    }

    void play()
    {
        system("cls");

        auto lastUpdate = clock();

        while (!gameOver)
        {
            input();

            clock_t now = clock();
            double elapsed = (double)(now - lastUpdate) * 1000 / CLOCKS_PER_SEC;

            if (elapsed >= speed)
            {
                update();
                lastUpdate = now;
                system("cls");
                draw();
            }

            if (speedChanged)
            {
                speedChanged = false;
            }
        }

        showGameOver();
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1)
        {
            init();
            play();
        }
        else if (choice == 2)
        {
            return;
        }
    }
};

int main()
{
    SetConsoleTitle("Snake Game");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    SnakeGame game;
    game.run();

    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    return 0;
}
