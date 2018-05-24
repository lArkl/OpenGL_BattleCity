#include "Level.h"
#include <GL/glut.h>
#include <vector>
#include<iostream>

extern Level *level1;

Object::Object()
{
	posX = 0.0;
	posZ = 0.0;
	direction = 0;
	alive = false;
}

Object::Object(float x,float z, int dir){
	posX = x;
	posZ = z;
	direction = dir;
	alive = false;//Tells if it's recently created
}

void Object::display(){
	//std::cout<<posZ<<std::endl;
	//glColor3f(.0,.0,.0);
	//if(Object)
	radius = 4;
	/*
	glBegin(GL_TRIANGLE_STRIP);// wasted block :'v
		glVertex3f(posX-radius,2.0,posZ-radius);
		glVertex3f(posX-radius,4.0,posZ-radius);	
		glVertex3f(posX+radius,2.0,posZ-radius);
		glVertex3f(posX+radius,4.0,posZ-radius);
		glVertex3f(posX+radius,0.0,posZ+radius);
		glVertex3f(posX+radius,4.0,posZ+radius);
		glVertex3f(posX-radius,4.0,posZ+radius);
		glVertex3f(posX-radius,4.0,posZ+radius);
		glVertex3f(posX+radius,4.0,posZ-radius);
		glVertex3f(posX-radius,4.0,posZ-radius);	
	glEnd();
	*/
	int size = radius/2;
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
		/*
		glTexCoord2f(0, 1); glVertex3f(-size, -size,  size);
		glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
		glTexCoord2f(1, 0); glVertex3f( size, -size, -size);
		glTexCoord2f(1, 1); glVertex3f( size, -size,  size);
		*/
		glTexCoord2f(0, 1); glVertex3f(-size, size,  size);
		glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
		glTexCoord2f(1, 0); glVertex3f( size, size, -size);
		glTexCoord2f(1, 1); glVertex3f( size, size,  size);
	glEnd();
	glPopMatrix();
}

Object::~Object(){};


Bullet::Bullet():Object(){}

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

Bullet::~Bullet(){}

bool Bullet::destroyObject(Object *obj){
	alive = false;
	obj->alive = false;
	return true;
}

bool Bullet::impactOn()
{
	const float Limit = 70; // Scenario Limits
	if(posX > Limit || posX < -Limit || posZ > Limit || posZ <-Limit){
		alive = false;
		return true;
	}
	/*
	for(int i=0; i< SceneObjects.size() ;i++){
		float x = SceneObjects[i]->posX;
		float r = SceneObjects[i]->radius;

	}
	return false;
	*/
	
	for(int i=0; i< level1->SceneObjects.size(); i++){
		Object *obj = level1->SceneObjects[i];
		//std::cout<<this<<" "<<obj <<std::endl;
		if(obj == this || owner == obj) continue;
		float notDistance = (direction % 2 ==0)? posX - obj->posX: posZ-obj->posZ;
		float sumRadius = radius + obj->radius;
		if(notDistance*notDistance > sumRadius) continue; 
		switch(direction){
			case 0: if(posZ + sumRadius > obj->posZ) return destroyObject(obj); break;
			case 1: if(posX + sumRadius > obj->posX) return destroyObject(obj); break;
			case 2: if(posZ < obj->posZ + sumRadius) return destroyObject(obj); break;
			case 3: if(posX < obj->posX + sumRadius) return destroyObject(obj); break;
		}
	}
	return false;

	
}

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

Tank::Tank(){
	iniX = posX;
	iniZ = posZ;
	iDir = direction;
};

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

bool Tank::limit(int step){
	const float Limit = 60; //Tanks outside limit
	switch(direction){
		case 0: if(posZ + step > Limit) return true; break;
		case 1: if(posX + step > Limit) return true; break;
		case 2: if(posZ - step < -Limit) return true; break;
		case 3: if(posX - step < -Limit) return true; break;
	}
	for(int i=0; i< level1->SceneObjects.size(); i++){
		Object *obj = level1->SceneObjects[i];
		//std::cout<<this<<" "<<obj <<std::endl;
		if(obj == this) continue;
		float notDistance = (direction % 2 ==0)? posX - obj->posX: posZ-obj->posZ;
		float sumRadius = radius + obj->radius;
		if(notDistance*notDistance > sumRadius) continue; 
		
		switch(direction){
			case 0: if(posZ + step + sumRadius > obj->posZ) return true; break;
			case 1: if(posX + step + sumRadius > obj->posX) return true; break;
			case 2: if(posZ - step < obj->posZ + sumRadius) return true; break;
			case 3: if(posX - step < obj->posX + sumRadius) return true; break;
		}
	}
	return false;
}

void Tank::display(){
	glPushMatrix();
		glTranslatef(posX,.0,posZ);
		glRotatef((direction-1)*90,0,1.0,0);
		//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		/*
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		*/
		std::vector<Face>::iterator f;
		glBegin(GL_TRIANGLES);
		for(f = model->faces.begin(); f!= model->faces.end(); ++f){
			for(int i=0;i<3;i++){
				int index;
				//Texture coord
				/*
				index = (*f).vtext[i]-1;
				if(index>-1){
					Dot d = model.textures.at(index);
					//cout<<d.x<<" "<<d.y<<endl;
					glTexCoord2f(d.x,d.y);
				}*/		
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
	while (ammo[i]->alive == true && i<4){
		i++;
	}
	if (!ammo[i]->alive) {
		delete ammo[i];
		ammo[i] = new Bullet(posX,posZ,direction);
		ammo[i]->owner = this;
		ammo[i]->alive = true;
	}
}	
	//void AttackIA(Mapa, Nodo *);
	/*
    void BFS(Mapa, Nodo *);
	void DFSvisit(Nodo*);
	void DFS(Mapa, Nodo *);
	*/

void Tank::reloadBullets(){
	for (int i = 0; i<5; i++)
		ammo[i] = new Bullet();
}

void Tank::respawn() {
	//if () {
	alive = true;
	posX = iniX;
	posZ = iniZ;
	reloadBullets();
}

Tank::~Tank(){
	for(int i=0; i<maxAmmo; i++)
        if(ammo[i]!=NULL) delete ammo[i];
}
