#include "data.h"
#include <assimp/Importer.hpp>	//OO version Header!
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

const int maxAmmo = 5;
Assimp::Importer importer;
class Object{
	public:
	float posX, posZ;
	int direction;
	int state;
	float radius;
	Object();
	Object(float, float, int);
    //virtual void move();
    virtual void display();
	~Object();
};

class Bullet:public Object
{
	public:
	Object *owner;
	bool shot;
	Bullet();
	Bullet(float,float,int);
	bool destroyObject(Object*);
	void move();
	bool impactOn();
	void display();
	~Bullet();
};

class Tank:public Object{
	public:
	float iniX,iniZ;
	int iDir;
	Model *model;
    //clock_t time;
	Bullet *ammo[maxAmmo];
	Tank();
	Tank(float,float,int);
	//void move();
	//void MoverIA(Mapa, Nodo *, clock_t);
	//void MoverIA2(Mapa, int, Nodo *, clock_t,Jugador*);	
	bool limit(float);
	void move(int);
	void shoot();
	//void AttackIA(Mapa, Nodo *);
	/*
    void BFS(Mapa, Nodo *);
	void DFSvisit(Nodo*);
	void DFS(Mapa, Nodo *);
	*/
    void display();
	void display2();
	void respawn();
	void reloadBullets();
	~Tank();
};

//extern Tank *player;
