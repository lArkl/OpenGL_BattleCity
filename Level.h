#include "Object.h"
#include <vector>
#include <string>

class Player{
    public:
    std::string name;
    int actualLevel;
    int points;
    int totalPoints;
    int lives;
    Player();
    Player(std::string);
    void restart();
    void update();
};

class Level{
    public:
    std::vector<Object*> SceneObjects; //Objects in scene
    void create(int,int);
};