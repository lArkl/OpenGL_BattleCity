#include "Object.h"
#include <GL/glut.h>
#include <vector>

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
	alive = false;//indica primera vez
}

void Object::display(){
}

Object::~Object(){};


Bullet::Bullet():Object(){}

Bullet::Bullet(float x, float z, int dir):Object(x,z,dir)
{
	const int step = 1;
	switch (dir) {
		case 0: posZ = z + step;break;
		case 1: posX = x + step;break;
		case 2: posZ = z - step;break;
		case 3: posX = x - step;break;
	}
}

bool Bullet::impactOn()
{
	const float Limit = 70; // Scenario Limits
	if(posX > Limit || posX < -Limit || posZ > Limit || posZ <-Limit){
		alive = false;
		return true;
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

Tank::Tank(){
	iniX = posX;
	iniZ = posZ;
	iDir = direction;
};

Tank::Tank(float x,float z,int dir):Object(x,z,dir){
	iniX = x;
	iniZ = z;
	iDir = dir;
	reloadBullets();
}


void Tank::move(int dir){
	const float step = 0.9;
	direction = dir;
	switch(direction){
		case 0: posZ += step;break;
		case 1: posX += step;break;
		case 2: posZ -= step;break;
		case 3: posX -= step;break;
	}
}

void Tank::display(){
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
			Point p = model->vertexs.at(index);
			glVertex3f(p.x,p.y,p.z );
		}	
	}
	glEnd();
}

void Tank::shoot(){
	int i = 0;
	while (ammo[i]->alive == true && i<4){
		i++;
	}
	if (!ammo[i]->alive) {
		delete ammo[i];
		ammo[i] = new Bullet(posX,posZ,direction);
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
		//Imprimir(direccion);
	//}
}

Tank::~Tank(){}