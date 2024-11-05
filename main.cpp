#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
int snakeLen, speed, score, facing;//0-NONE 1-LEFT 2-RIGHT 3-DOWN 4-UP
struct Part {int x, y;} snake[400];
void snakeInit(){score = snakeLen = (facing = 0) + 1;   //Snake's ititial settings
    snake[0] = {20/4 + rand() % 20 - 20/4, 20/4 + rand() % 20 - 20/4};
    speed = 40;}
Part spawnFood(){return{ rand() % 20, rand() % 20 };}
void moveSnake(){for (int i = snakeLen - 1; i > 0; i--) snake[i] = snake[i - 1]; //Shift snake parts
    snake[0].y = (snake[0].y + (facing == 1 ? -1 : facing == 2 ? 1 : 0) + 20) % 20;
    snake[0].x = (snake[0].x + (facing == 4 ? -1 : facing == 3 ? 1 : 0) + 20) % 20;}
void fixedUpdate() {Part food = spawnFood();
    while (true) {auto start = std::chrono::steady_clock::now();
        system("clear");
        std::cout << std::string(42, '#')+"\r\n";
        if(snake[0].x == food.x && snake[0].y == food.y){snake[snakeLen++] = {-2, -2};   //Food logic
            speed += ++score / 2;
            food = spawnFood();}
        for(int i = 0; i < 20; i++) {std::cout << "#";  //Display
            for(int j = 0; j < 20; j++) {bool isSnakePart = false;
                for (int k = 0; k < snakeLen; k++) if(snake[k].x == i && snake[k].y == j) isSnakePart = true;
                std::cout << (isSnakePart ? "[]" : (food.x == i && food.y == j) ? "()" : "  ");}
            std::cout << "#\r\n";}
        std::cout << std::string(42, '#')+"\r\n" << "\r\nScore: " + std::to_string(score) << "\r\n";
        for(int i = 1; i < snakeLen; i++)if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)snakeInit();    //End of game
        moveSnake();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count() < 10.00/speed? (10.00/speed - std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count()) * 1000 : 0)));}}
int main() {snakeInit();
    std::thread loop(fixedUpdate);
    system("stty raw"); 
    while (true) {
        char c = std::getchar();
        if(c == 'q'){system("stty cooked");
            return 0;}
        else facing = (c == 'w')? 4 : (c == 's')? 3 : (c == 'a')? 1 : (c == 'd')? 2 : facing;}
    return 0;}
