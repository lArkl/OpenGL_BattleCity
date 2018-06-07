#include "data.h"

const int maxAmmo = 5;
	
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
	int iDir,timer;
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
	void attackIA(Tank *player, int rand);
	void moveIA(Tank *player);
	void idle();
	/*
    void BFS(Mapa, Nodo *);
	void DFSvisit(Nodo*);
	void DFS(Mapa, Nodo *);
	*/
    void display();
	void respawn();
	void reloadBullets();
	~Tank();
};

//extern Tank *player;
