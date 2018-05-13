
//glutSpecialFunc(void (*func)(int key, int x, int y));
/*
typedef struct Position{
    float x,y,z;
    float arcx,arcy,arcz;
}Position;
void initPosition(Position p){
    p.x=0;
    p.y=0;
    p.z=0;
    p.arcx=0;
    p.arcy=0;
    p.arcz=0;    
}
extern Position playerPos;
*/

void Key_Released(unsigned char, int, int);

void keyboard(unsigned char key, int x, int y);