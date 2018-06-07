#include "Object.h"
#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

extern std::vector<Object*> SceneObjects;


const char* windowTitle = "OpenGL Framework";

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, 15.0f, 1.0f };



// the global Assimp scene object
const aiScene* scene = NULL;
GLuint scene_list = 0;
aiVector3D scene_min, scene_max, scene_center;

// images / texture
std::map<std::string, GLuint*> textureIdMap;	// map image filenames to textureIds
GLuint*		textureIds;							// pointer to texture Array



Object::Object()
{
	posX = 0.0;
	posZ = 0.0;
	direction = 0;
	state = 1;
}

Object::Object(float x,float z, int dir){
	posX = x;
	posZ = z;
	direction = dir;
	state = 1;//Tells if it's recently created
}

void Object::display(){
	radius = 4;
	int size = radius;
	glPushMatrix();
	glTranslatef(posX,size,posZ);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
		glTexCoord2f(0, 1); glVertex3f(-size,  size, -size);
		glTexCoord2f(1, 1); glVertex3f( size,  size, -size);
		glTexCoord2f(1, 0); glVertex3f( size, -size, -size);

		glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
		glTexCoord2f(0, 1); glVertex3f(-size,  size, size);
		glTexCoord2f(1, 1); glVertex3f( size,  size, size);
		glTexCoord2f(1, 0); glVertex3f( size, -size, size);

		glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
		glTexCoord2f(0, 1); glVertex3f(-size,  size, -size);
		glTexCoord2f(1, 1); glVertex3f(-size,  size,  size);
		glTexCoord2f(1, 0); glVertex3f(-size, -size,  size);

		glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
		glTexCoord2f(0, 1); glVertex3f(size,  size, -size);
		glTexCoord2f(1, 1); glVertex3f(size,  size,  size);
		glTexCoord2f(1, 0); glVertex3f(size, -size,  size);
		
		glTexCoord2f(0, 1); glVertex3f(-size, size,  size);
		glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
		glTexCoord2f(1, 0); glVertex3f( size, size, -size);
		glTexCoord2f(1, 1); glVertex3f( size, size,  size);
	glEnd();
	glPopMatrix();
}

Object::~Object(){};


Bullet::Bullet():Object(){
	state = -1;;//Tells if it's recently created
	radius = 1;
}

Bullet::Bullet(float x, float z, int dir):Object(x,z,dir)
{
	radius = 1;
	const int step = 4;
	switch (dir) {
		case 0: posZ = z + step;break;
		case 1: posX = x + step;break;
		case 2: posZ = z - step;break;
		case 3: posX = x - step;break;
	}
}

bool Bullet::destroyObject(Object *obj){
	state = -1;
	obj->state = 0;
	cout<<posX<<" "<<posZ<<" "<<endl;
	cout<<obj->posX<<" "<<obj->posZ<<" "<<endl;
	return true;
}

bool Bullet::impactOn()
{
	const float Limit = 70; // Scenario Limits
	if(posX > Limit || posX < -Limit || posZ > Limit || posZ <-Limit){
		state = -1;
		return true;
	}
	for(int i=0; i< SceneObjects.size(); i++){
		Object *obj = SceneObjects[i];
		if(obj == this|| owner == obj || obj->state<1) continue;
		float notDistance = (direction % 2 ==0)? posX - obj->posX: posZ - obj->posZ;
		float sumRadius = radius + obj->radius;
		if(abs(notDistance) > sumRadius) continue;
		float difInAxis;
		switch(direction){
			case 0: difInAxis = abs(posZ - obj->posZ); break;
			case 1: difInAxis = abs(posX - obj->posX); break;
			case 2: difInAxis = abs(posZ - obj->posZ); break;
			case 3: difInAxis = abs(posX - obj->posX); break;
		}
		if(difInAxis < sumRadius) return destroyObject(obj);



	}
	return false;	
}

Bullet::~Bullet(){}

void Bullet::move(){
	const float step = 1.2;
	if(!impactOn()){
		switch(direction){
			case 0: posZ += step; break;
			case 1: posX += step; break;
			case 2: posZ -= step; break;
			case 3: posX -= step; break;
		}
	}
}

void Bullet::display(){
	//glTranslatef(player.ammo[i]->posX,.0,player.ammo[i]->posZ);
	//glRotatef((player.ammo[i]->direction-1)*90,0,1.0,0);
	glVertex3f(posX,2.8,posZ);
	glRotatef((direction-1)*90,0,1.0,0);
	//glVertex3f(.0,2.8,.0);
}

Tank::Tank():Object(){
	iniX = posX;
	iniZ = posZ;
	iDir = direction;
	radius = 4;
	reloadBullets();
}

Tank::Tank(float x,float z,int dir):Object(x,z,dir){
	iniX = x;
	iniZ = z;
	iDir = dir;
	radius = 4;
	reloadBullets();
}


void Tank::move(int dir){
	const float step = 0.9;
	direction = dir;
	if(limit(step))return;
	switch(direction){
		case 0: posZ += step;break;
		case 1: posX += step;break;
		case 2: posZ -= step;break;
		case 3: posX -= step;break;
	}
}

bool Tank::limit(float step){
	const float Limit = 60; //Tanks outside limit
	switch(direction){
		case 0: if(posZ + step > Limit) return true; break;
		case 1: if(posX + step > Limit) return true; break;
		case 2: if(posZ - step < -Limit) return true; break;
		case 3: if(posX - step < -Limit) return true; break;
	}
	float difInAxis;
	for(int i=0; i< SceneObjects.size(); i++){
		Object *obj = SceneObjects[i];
		//std::cout<<this<<" "<<obj <<std::endl;
		if(obj == this) continue;
		if(obj->state==0) continue;
		float notDistance = (direction % 2 ==0)? posX - obj->posX: posZ-obj->posZ;
		float sumRadius = radius + obj->radius;
		if(abs(notDistance) > sumRadius) continue; 
		
		switch(direction){
			case 0: difInAxis = abs(posZ + step - obj->posZ); break;
			case 1: difInAxis = abs(posX + step - obj->posX); break;
			case 2: difInAxis = abs(posZ - step - obj->posZ); break;
			case 3: difInAxis = abs(posX - step - obj->posX); break;
		}
		if(difInAxis < sumRadius) return true;
	}
	return false;
}

void Tank::display()
{
	glPushMatrix();
	glTranslatef(posX,.0,posZ);
	glRotatef((direction-1)*90,0,1.0,0);
	mesh->drawAiScene();
	//drawAiScene(scene);
	glPopMatrix();
}

void Tank::shoot(){
	int i = 0;
	while (i< maxAmmo-1 && ammo[i]->state>-1){
		i++;
	}
	//cout<<i<<" "<<ammo[i]->state<<endl;
	if (ammo[i]->state<0) {
		delete ammo[i];
		ammo[i] = new Bullet(posX,posZ,direction);
		ammo[i]->state=1;
		ammo[i]->owner = this;
	}
}
	//void AttackIA(Mapa, Nodo *);
	/*
    void BFS(Mapa, Nodo *);
	void DFSvisit(Nodo*);
	void DFS(Mapa, Nodo *);
	*/

void Tank::reloadBullets(){
	for (int i = 0; i< maxAmmo; i++){
		ammo[i] = new Bullet();
		//cout<<ammo[i]->alive<<endl;
	}
}

void Tank::respawn() {
	//if () {
	state = 1;
	posX = iniX;
	posZ = iniZ;
	reloadBullets();
		//Imprimir(direccion);
	//}
}

Tank::~Tank(){
	for(int i=0;i < maxAmmo;i++)
		//if(ammo[i] == nullptr)
			delete ammo[i];
}
