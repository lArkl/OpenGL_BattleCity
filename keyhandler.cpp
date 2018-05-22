#include <GL/glut.h>
//#include <iostream>
#include "Level.h"
#include <string>

extern Level *level1;

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
		level1->player->move(0);
        //zz -= 0.1;
        glutPostRedisplay();
        //glTranslatef(0.0,-1.0,0.0);
        //glRotatef(1.0, 1., 0., 0.);
        //playerPos.x = 0;
        break;
	case 'w':
		//zz += 0.1;
        level1->player->move(2);
        glutPostRedisplay();
        //glTranslatef(0.0,1.0,0.0);
        //glRotatef(1.0, -1., 0., 0.);
        break;
	case 'a':
		//xx -= 0.1 ;
        level1->player->move(3);
        glutPostRedisplay();
        //glTranslatef(-1.0,0.0,0.0);
        //glRotatef(1.0, 0.0, 1.0, 0.);
        break;
	case 'd':
		//xx += 0.1;
        level1->player->move(1);
        glutPostRedisplay();
        //glTranslatef(1.0,0.0,0.0);
        //glRotatef(1.0, 0.0, -1.0, 0.);
        break;
    case 'v':
        level1->player->shoot();break;
	case 't':
		//glTranslatef(0.0,0.0,-1.0);
        //glTranslatef(-0.01, -0.00,0.01);
        break;
    case 'g':
		//glTranslatef(0.0,0.0,1.0);
        //glTranslatef(-0.01, -0.00,0.01);
        break;
    case 'q':
    //case 27:
		exit(0);
        break;
    //default: std::cout<<key;
    }
   	glutPostRedisplay();
}