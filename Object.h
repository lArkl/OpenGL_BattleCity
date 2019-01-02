#include "data.h"
#include "Graph.h"
const int maxAmmo = 5;

bool loadTextures();
void displayScenario();

class Object{
	public:
	float posX, posZ;
	int direction;
	int state;
	Node *node;
	float radius;
	Object();
	Object(float, float, int);
	Object(Node *,int);
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
	Bullet(Node *,float,float,int);
	bool destroyObject(Object*);
	void move();
	bool impactOn();
	void display();
	~Bullet();
};

class Tank:public Object{
	public:
	float iniX,iniZ;
	int iDir,timer,nodeIdx;
	Model *model;
    //clock_t time;
	Bullet *ammo[maxAmmo];
	Tank();
	Tank(float,float,int);
	Tank(Node *,int);
	//void move();
	//void MoverIA(Mapa, Nodo *, clock_t);
	//void MoverIA2(Mapa, int, Nodo *, clock_t,Jugador*);	
	bool limit(float);
	void move(int);
	void shoot();
	void attackIA(Tank *player);
	void moveIA(Tank *player);
	void moveNode(int,Graph*);
	void movePlayer(int,Graph*);
	void moveBFS(Tank *, Graph *);
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

class Platform{
private:
	int level;
	Model *model;
public:
	Platform();
	Platform(std::string);
	void display();
	~Platform();
};