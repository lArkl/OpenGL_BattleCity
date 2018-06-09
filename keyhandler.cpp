#include <GL/glut.h>
//#include <iostream>
#include "Object.h"
#include <string>

extern Graph Scenario;
extern Tank *player;
void keyboard(unsigned char key, int x, int y){
	//std::cout<<key<<std::endl;
	switch (key){
    /*
    case 'h':
		std::cout<<"help\nc - switch culling\nq/escape - Salir\n1/2 - rotar"<<std::endl;
        break;
	*/
    case 'c':
		if (glIsEnabled(GL_CULL_FACE))
			glDisable(GL_CULL_FACE);
		else glEnable(GL_CULL_FACE);
        break;
	case 's':
		player->movePlayer(0,&Scenario);
        //player->move(0);
        glutPostRedisplay();
        break;
	case 'w':
		//player->move(2);
        player->movePlayer(2,&Scenario);
        glutPostRedisplay();
        break;
	case 'a':
		//player->move(3);
        player->movePlayer(3,&Scenario);
        glutPostRedisplay();
        break;
	case 'd':
		player->movePlayer(1,&Scenario);
        //player->move(1);
        glutPostRedisplay();
        break;
    case 'v':
        player->shoot();break;
	case 't':
		break;
    case 'g':
		break;
    case 'q':
    //case 27:
		exit(0);
        break;
    //default: std::cout<<key;
    }
   	glutPostRedisplay();
}