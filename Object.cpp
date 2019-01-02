#include "Object.h"
#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <SDL2/SDL_image.h>

using namespace std;

extern std::vector<Object*> SceneObjects;

const int numTEXT = 5;
GLuint texID[numTEXT]; // Textures

Object::Object()
{
	posX = 0.0;
	posZ = 0.0;
	direction = 0;
	state = 1;
	radius = 3.8;
}

Object::Object(float x,float z, int dir){
	posX = x;
	posZ = z;
	direction = dir;
	radius = 3.8;
	state = 1;//Tells if it's recently created
}

Object::Object(Node *n, int dir){
	posX = n->posX;
	posZ = n->posZ;
	direction = dir;
	node = n;
	radius = 3.8;
	state = 1;//Tells if it's recently created
}

void Object::display(){
	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID[2]); //binds Block2 texture
	int size = 4;

	glPushMatrix();
	glTranslatef(posX,size,posZ);
	glBegin(GL_QUADS);
		glNormal3f(0,0,-1.0);
		glTexCoord2f(0, 0); glVertex3f(-size, -size/2, -size);
		glTexCoord2f(0, 1); glVertex3f(-size,  size/2, -size);
		glTexCoord2f(1, 1); glVertex3f( size,  size/2, -size);
		glTexCoord2f(1, 0); glVertex3f( size, -size/2, -size);

		glNormal3f(0,0,1.0);
		glTexCoord2f(0, 0); glVertex3f(-size, -size/2, size);
		glTexCoord2f(0, 1); glVertex3f(-size,  size/2, size);
		glTexCoord2f(1, 1); glVertex3f( size,  size/2, size);
		glTexCoord2f(1, 0); glVertex3f( size, -size/2, size);

		glNormal3f(-1.0,0,0);
		glTexCoord2f(0, 0); glVertex3f(-size, -size/2, -size);
		glTexCoord2f(0, 1); glVertex3f(-size,  size/2, -size);
		glTexCoord2f(1, 1); glVertex3f(-size,  size/2,  size);
		glTexCoord2f(1, 0); glVertex3f(-size, -size/2,  size);

		glNormal3f(1.0,0,0);
		glTexCoord2f(0, 0); glVertex3f(size, -size/2, -size);
		glTexCoord2f(0, 1); glVertex3f(size,  size/2, -size);
		glTexCoord2f(1, 1); glVertex3f(size,  size/2,  size);
		glTexCoord2f(1, 0); glVertex3f(size, -size/2,  size);
		
		glNormal3f(0,1.0,0);
		glTexCoord2f(0, 1); glVertex3f(-size, size/2,  size);
		glTexCoord2f(0, 0); glVertex3f(-size, size/2, -size);
		glTexCoord2f(1, 0); glVertex3f( size, size/2, -size);
		glTexCoord2f(1, 1); glVertex3f( size, size/2,  size);
	glEnd();
	glPopMatrix();
}

Object::~Object(){};


Bullet::Bullet():Object(){
	state = -1;;//Tells if it's recently created
	radius = 1;
}

Bullet::Bullet(Node *n, float x, float z, int dir):Object(x,z,dir)
{
	radius = 1;
	node = n;
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
	obj->node->numObject = 0;
	return true;
}

bool Bullet::impactOn()
{
	const float Limit = 72; // Scenario Limits
	const int halfBase = 84, halfDepth = 72;
	if(posX > halfBase || posX < -halfBase || posZ > halfDepth || posZ <-halfDepth){
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
	timer = 0;
	reloadBullets();
}

Tank::Tank(float x,float z,int dir):Object(x,z,dir){
	iniX = x;
	iniZ = z;
	iDir = dir;
	radius = 4;
	timer = 0;
	//Random start
	random_device rnd;
	srand(rnd());
	reloadBullets();
}

Tank::Tank(Node *n, int dir):Object(n,dir){
	iniX = posX;
	iniZ = posZ;
	iDir = dir;
	radius = 4;
	timer = 0;
	nodeIdx = n->idx;
	//Random start
	random_device rnd;
	srand(rnd());
	reloadBullets();
}

bool Tank::limit(float step){
	//const float Limit = 60; //Tanks outside limit
	const int halfBase = 84, halfDepth = 72;
	switch(direction){
		case 0: if(posZ + step > halfDepth) return true; break;
		case 1: if(posX + step > halfBase) return true; break;
		case 2: if(posZ - step < -halfDepth) return true; break;
		case 3: if(posX - step < -halfBase) return true; break;
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

void Tank::moveNode(int s, Graph *scenario){
	Node n = scenario->nodes[s];
	float dist = 0;
	switch(direction){
		case 0: dist = posZ - n.posZ; break; 
		case 1: dist = posX - n.posX; break; 
		case 2: dist = n.posZ - posZ; break; 
		case 3: dist = n.posX - posX; break; 
	}
	if(dist==0){
		nodeIdx = s;
		node = &scenario->nodes[s];
	}
	//cout<<"Enemy Node: "<<nodeIdx<<endl;
}

void Tank::move(int dir){
	const float step = 1.0;
	direction = dir;
	if(limit(step))return;
	switch(direction){
		case 0: posZ += step;break;
		case 1: posX += step;break;
		case 2: posZ -= step;break;
		case 3: posX -= step;break;
	}
}

void Tank::movePlayer(int dir, Graph *scenario){
	move(dir);
	int dist,s;
	Node *n = scenario->nodes;
	switch(dir){
		case 0: s = nodeIdx - scenario->cols; dist = posZ - n[s].posZ; break; 
		case 1: s = nodeIdx + 1; dist = posX - n[s].posX; break; 
		case 2: s = nodeIdx + scenario->cols; dist = n[s].posZ - posZ; break; 
		case 3: s = nodeIdx - 1; dist = n[s].posX - posX; break; 
	}
	if(dist==0)
		nodeIdx = s;
	//cout<<"Player node: "<<nodeIdx<<" s:"<<s<<endl;
}

void Tank::display(){
	glBindTexture(GL_TEXTURE_2D, texID[3]); //tankTex.jpg
	glPushMatrix();
		glTranslatef(posX,.0,posZ);
		glRotatef((direction-1)*90,0,1.0,0);
		//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		/*
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		*/
		vector<Face>::iterator f;
		glBegin(GL_TRIANGLES);
		for(f = model->faces.begin(); f!= model->faces.end(); ++f){
			for(int i=0;i<3;i++){
				int index;
				//Normal coord
				index = (*f).vnorm[i]-1;
				if(index>-1){
					Point p = model->normals[index];
					//cout<<d.x<<" "<<d.y<<endl;
					glNormal3f(p.x,p.y,p.z);
				}
				//Texture coord
				index = (*f).vtext[i]-1;
				if(index>-1){
					Dot d = model->textures[index];
					//cout<<d.x<<" "<<d.y<<endl;
					glTexCoord2f(d.x,d.y);
				}
				index = (*f).vert[i]-1;
				Point p = model->vertexs[index];
				glVertex3f(p.x,p.y,p.z );
			}	
		}
		glEnd();
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
		ammo[i] = new Bullet(node,posX,posZ,direction);
		ammo[i]->state=1;
		ammo[i]->owner = this;
	}
}

void Tank::attackIA(Tank *player){
	const int maxVision = 30;
	int threshold = 95;
	float difAxis=1000,axis=1000;
	if(player->state){
		switch(direction){
			case 0: difAxis = player->posX - posX;axis = player->posZ - posZ; break;
			case 1: difAxis = player->posZ - posZ;axis = player->posX - posX; break;
			case 2: difAxis = player->posX - posX;axis = posZ-player->posZ; break;
			case 3: difAxis = player->posZ - posZ;axis = posX-player->posX; break;
		}
		if(axis>0 && axis<maxVision && abs(difAxis)<radius)
			shoot();
	}else
		threshold = 80;
	int r = rand()%100;
	if(r> threshold)
		shoot();	
}

void Tank::moveIA(Tank *player){
	int r = rand()%4;
	move(r);
	//attackIA(player);
}

void Tank::moveBFS(Tank *player, Graph *scenario){
	int dir,s = scenario->BFS(nodeIdx,player->nodeIdx);
	/*
	int x= nodeIdx- s;
	switch(x){
		case 
	}*/
	for(int i=0;i<4;i++){
		if(scenario->nodes[nodeIdx].adj[i] == s){
			dir = i;
			break;
		}
	}
	move(dir);
	moveNode(s,scenario);
	attackIA(player);
}

void Tank::reloadBullets(){
	for (int i = 0; i< maxAmmo; i++){
		ammo[i] = new Bullet();
		//cout<<ammo[i]->alive<<endl;
	}
}

void Tank::idle(){
	const int idleTime = 200;
	if(state==0){
		timer++;
		if(timer >idleTime)
			respawn();
	}
}

void Tank::respawn() {
	state = 1;
	posX = iniX;
	posZ = iniZ;
	reloadBullets();
	timer = 0;
}

Tank::~Tank(){
	for(int i=0;i < maxAmmo;i++)
		delete ammo[i];
}


//Path to textures
const string texdir = "Textures/";
string textureFileNames[numTEXT] = {texdir+"platform2.jpg",texdir+"block.jpg",
						texdir+"block.png",texdir+"tankTex.jpg",texdir+"tankPlayer.jpg"};

bool loadTextures() {
	glGenTextures(numTEXT,texID);  // Obtener el Id textura 
	for (int i = 0; i < numTEXT; ++i) {
		// this surface will tell us the details of the image
		SDL_Surface *surface = IMG_Load(textureFileNames[i].c_str()); 

		GLint nColors;
		GLenum textureFormat;

		if(surface){
			//get number of channels in the SDL surface
			nColors = surface->format->BytesPerPixel;

			//contains an alpha channel
			if(nColors == 4)
			{
				if(surface->format->Rmask==0x000000ff)
			    	textureFormat = GL_RGBA;
				else
			    	textureFormat = GL_BGRA;
			}
			else if(nColors == 3) //no alpha channel
			{
				if(surface->format->Rmask==0x000000ff)
			    	textureFormat = GL_RGB;
				else
			    	textureFormat = GL_BGR;
			}
			else
			{
				std::cout << "warning: the image is not truecolor…this will break " << std::endl;
			}

			// Have OpenGL generate a texture object handle for us
			//glGenTextures(i, texture[i]);

			// Bind the texture object
			glBindTexture(GL_TEXTURE_2D, texID[i]);
			// Set the texture’s stretching properties
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
			glTexImage2D( GL_TEXTURE_2D, 0, nColors, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

			// we also want to be able to deal with odd texture dimensions
			glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
			glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
			glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
			glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
			//width = surface->w;
			//height = surface->h;
		}
		else {
			std::cout << "LoadTexture:: Could not load " << textureFileNames[i].c_str()<< ": " << SDL_GetError() << std::endl;
			return false;
		//SDL_Quit();
		}
		// Free the SDL_Surface only if it was successfully created
		if(surface) {
			SDL_FreeSurface(surface);
		}
	}
	return true;
}
/*
void displayScenario(){
	// Base
	const int halfBase = 84, halfDepth = 72;	
	//glColor3f(.8, 0.8, .8);
	glBindTexture(GL_TEXTURE_2D, texID[0]);
	//4.0 for model base, 20 for scenario
	
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-halfBase,0.0,-halfDepth);
		glTexCoord2f(0, 2.1);glVertex3f(-halfBase,0.0,halfDepth);
		glTexCoord2f(2.4, 2.1);glVertex3f(halfBase,0.0,halfDepth);
		glTexCoord2f(2.4, 0);glVertex3f(halfBase,0.0,-halfDepth);
	glEnd();
}
*/

Platform::Platform()
{
	level = 1;
	model = nullptr;
}

Platform::Platform(std::string path)
{
	model = readFile(path);
	level = 1;
}

Platform::~Platform()
{
	delete model;
}

void Platform::display(){
	glBindTexture(GL_TEXTURE_2D, texID[0]); //platform-low
	glPushMatrix();
		vector<Face>::iterator f;
		glBegin(GL_TRIANGLES);
		for(f = model->faces.begin(); f!= model->faces.end(); ++f){
			for(int i=0;i<3;i++){
				int index;
				//Normal coord
				index = (*f).vnorm[i]-1;
				if(index>-1){
					Point p = model->normals[index];
					//cout<<d.x<<" "<<d.y<<endl;
					glNormal3f(p.x,p.y,p.z);
				}
				//Texture coord
				index = (*f).vtext[i]-1;
				if(index>-1){
					Dot d = model->textures[index];
					//cout<<d.x<<" "<<d.y<<endl;
					glTexCoord2f(d.x,d.y);
				}
				index = (*f).vert[i]-1;
				Point p = model->vertexs[index];
				glVertex3f(p.x,p.y,p.z );
			}	
		}
		glEnd();
	glPopMatrix();
}