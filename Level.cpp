#include "Level.h"
using namespace std;

Player::Player(){
    points=0;
    totalPoints=0;
    actualLevel = 0;
    lives = 5;
}

Player::Player(string newname){
    name = newname;
    points=0;
    totalPoints=0;
    actualLevel = 0;
    lives = 5;
}

void Player::update(){
    //update players info at screen
}

void Player::restart(){//????
    points = 5;
    points=0;
    totalPoints=0;
}

void Level::create(int blocks, int powers){
    const int numEnemies = 2;
    Tank enemies[numEnemies];
    for(int i=0; i<numEnemies; i++){
        SceneObjects.push_back(&enemies[i]);
        for(int j=0; j < maxAmmo; j++)
            SceneObjects.push_back(enemies[i].ammo[j]);
    }
}