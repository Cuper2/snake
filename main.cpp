#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
const int SIZEX = 20, SIZEY = 20;
struct Part {int x, y;};
Part snake[SIZEX * SIZEY];
int snakeLen = 1, score = 0, hScore = 0;
enum axis {UP, DOWN, LEFT, RIGHT, NONE} facing = NONE;
bool open = true;
double speed = 0.25;
void snakeInit(){   //Snake's ititial settings
    facing = NONE;
    score = 0;
    snakeLen = 1;
    snake[0] = {SIZEX/4 + rand() % SIZEX - SIZEX/4, SIZEY/4 + rand() % SIZEY - SIZEY/4};
    speed = 0.25;}
Part spawnFood(){return{ rand() % SIZEX, rand() % SIZEY };}
void moveSnake(){
    for (int i = snakeLen - 1; i > 0; i--) snake[i] = snake[i - 1]; //Shift snake parts
    snake[0].y = (snake[0].y + (facing == LEFT ? -1 : facing == RIGHT ? 1 : 0) + SIZEY) % SIZEY;
    snake[0].x = (snake[0].x + (facing == UP ? -1 : facing == DOWN ? 1 : 0) + SIZEX) % SIZEX;}
void fixedUpdate() {
    Part food = spawnFood();
    while (open) {
        auto start = std::chrono::steady_clock::now();
        system("clear");
        std::string boardStr = std::string(SIZEX * 2 + 2, '█')+"\r\n";
        if(snake[0].x == food.x && snake[0].y == food.y){     //Food logic
            snake[snakeLen++] = {-2, -2};
            speed /= 1.05;
            food = spawnFood();
            score ++;
            if (score > hScore)hScore = score;}
        for(int i = 0; i < SIZEX; i++) {    //Display
            boardStr += "██";
            for(int j = 0; j < SIZEY; j++) {
                bool isSnakePart = false;
                for (int k = 0; k < snakeLen; k++) if(snake[k].x == i && snake[k].y == j) isSnakePart = true;
                boardStr += isSnakePart ? "[]" : (food.x == i && food.y == j) ? "()" : "  ";}
            boardStr += "██\r\n";}
        std::cout << boardStr << std::string(SIZEX *2 + 2, '█')+"\r\n" << "\r\nWynik: " + std::to_string(score) << "\r\n" << (hScore != score? "Najlepszy wynik: " + std::to_string(hScore) : "") << "\r\n";
        std::cout.flush();
        for(int i = 1; i < snakeLen; i++)if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)snakeInit();    //End of game
        moveSnake();
        std::chrono::duration<double> delta = std::chrono::steady_clock::now() - start;
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delta.count() < speed? (speed - delta.count()) * 1000 : 0)));}}
void update() {
    system("stty raw");
    char c;
    while (open) {
        c = std::getchar();
        if(c == 'q'){open = false; system("stty cooked"); return;}
        else facing = (c == 'w')? UP : (c == 's')? DOWN : (c == 'a')? LEFT : (c == 'd')? RIGHT : facing;}}
int main() {
    snakeInit();
    std::thread tf(fixedUpdate), tn(update);
    tf.join(); tn.join();
    return 0;}