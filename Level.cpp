#include "Level.h"
#include <GL/glut.h>

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

Player::~Player(){}

void Player::update(){
    //update players info at screen
}

void Player::restart(){//????
    points = 0;
    totalPoints = 0;
}

Level::Level(){
     numBlocks = 4;
     numEnemies = 2;
     enemies = new Tank*;
     blocks = new Object*;
 }

Level::Level(int nBlocks, int nEnemies){
     numBlocks = nBlocks;
     numEnemies = nEnemies;
     enemies = new Tank*;
     blocks = new Object*;
}

Level::~Level(){
    for(int i=0;i<numEnemies;i++)
        delete enemies[i];
    delete enemies;
    for(int i=0;i<numBlocks;i++)
        delete blocks[i];
    delete blocks;
    delete player->model;
    delete player;
}

void Level::create(int powers){
    //numEnemies = 2;
    //Tank *enemies[numEnemies];
    int cont = 0;
    std::string path1 = "Models/Tank/BaseTank.obj";
	Model *tankModel = readFile(path1);
	
    for(int i=0; i < numEnemies; i++){
        int pos = i==1?-1:1;
        enemies[i] = new Tank(pos*40,30,2);
        enemies[i]->model = tankModel;
        SceneObjects.push_back(enemies[i]);
        cont++;
        /*
        for(int j=0; j < maxAmmo; j++){
            SceneObjects.push_back(enemies[i].ammo[j]);
            cont++;
        }
        */
    }
    //numBlocks = nBlocks;
    //Object *blocks[nBlocks];
    for(int i=0;i < numBlocks; i++){
        int a = i>1?1:-1;
        int b = i%2?1:-1;

        blocks[i] = new Object(a*20,b*20,0);
        SceneObjects.push_back(blocks[i]);
        cont++;
    }
    
    player = new Tank(0,0,0);
    player->model = tankModel;
    SceneObjects.push_back(player);

    totalObjects = cont;
}

extern float view_rotx,view_roty;

void Level::plotLevel(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPointSize(4.0);
	glLineWidth(3.0);
	
	glPushMatrix();
	
	glRotatef(view_rotx, 1.0, 0.0, 0.0);
	glRotatef(view_roty, 0.0, 1.0, 0.0);
    
    glPushMatrix();
	// Base
	glColor3f(.8, 0.8, .8);
	
    //4.0 for model base, 20 for scenario
	float halfSection = 70.0;
	glBegin(GL_QUADS);
		glVertex3f(-halfSection,0.0,-halfSection);
		glVertex3f(-halfSection,0.0,halfSection);
		glVertex3f(halfSection,0.0,halfSection);
		glVertex3f(halfSection,0.0,-halfSection);
	glEnd();
	
	//glScalef(0.1,0.1,0.1);
	glColor3f(.0, 0.8, .1);
	player->display();
	
    glColor3f(.8, 0.0, .1);
	for(int i=0; i<numEnemies; i++)
        enemies[i]->display();
    for(int i=0; i<numBlocks; i++)
	    blocks[i]->display();
	//DrawBullets
	glBegin(GL_POINTS);
		for(int i=0;i<5;i++){
			if(player->ammo[i]->alive==false) continue;
			player->ammo[i]->display();
		}
	glEnd();
	
	glPopMatrix();
	
	glPopMatrix();
	glutSwapBuffers();
}

