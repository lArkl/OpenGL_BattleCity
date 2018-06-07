/* -- INCLUDE FILES ------------------------------------------------------ */

#include "keyhandler.h"
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
//#include "data.h"
#include "stb_image.h"
#include "Object.h"

using namespace std;


unsigned int texture;
void LoadTextures(){
	//Texturas we
	// set the texture wrapping/filtering options (on the currently bound texture object)
	/*
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	*/
	// load and generate the texture
	int width, height, nrChannels;
	//char imagepath[] = "Models/Caral/untitled/texture_1.jpg";
	char imagepath[] = "Models/Knuckles/Knuckles.png";
	unsigned char *data = stbi_load(imagepath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		//glPixelStorei(GL_UNPACK_ALIGNMENT,1);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		*/
		cout<<"Texura: "<<imagepath<<" leida."<<endl;
		if(nrChannels>3){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		}else{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		//glEnable(GL_TEXTURE_2D);
		//glGenerateMipmap(GL_TEXTURE_2D);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		throw string("No se pudo leer el archivo");
		//cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
	//return texture;
}

const int numEnemies = 2;
vector<Object*> SceneObjects;
Tank *player;
Tank *enemies[numEnemies];
Object *block1;

void Init( void )  {
    //LoadTextures();
	glEnable(GL_TEXTURE_2D);
	/*
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f( 0.0, 0.0, 0.0 );
    gluOrtho2D( 0.0, 640.0, 0.0, 480.0 );
	*/
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);

	//std::string path = "Models/Tank/BaseTank.obj";
	std::string path = "Models/Tank/tank2.obj";
	Model *tankModel = readFile(path);

	player = new Tank(0,0,0);
	player->model = tankModel;
	for(int j=0;j<numEnemies;j++){
		int pos = j==0?1:-1;
		enemies[j] = new Tank(pos*40,30,2);
		enemies[j]->model = tankModel;
		SceneObjects.push_back(enemies[j]);
	}
	block1 = new Object(0,40,0);
	SceneObjects.push_back(player);
	SceneObjects.push_back(block1);
}

/* ----------------------------------------------------------------------- */
/* Function    : void drawArc(  )
 *
 * Description : Dibuja un arco.
 *
 * Parameters  : float x, float y, float r, float t0, float sweep
 *
 * Returns     : void
 */

float spin = 0;
float view_rotx=0.0, view_roty=0.0;//, view_rotz=0.0;
//float view_rotx=20.0, view_roty=30.0, view_rotz=0.0;

//extern Tank player;

//Tank player(0,0,0);

void plotPoints(){//,Model model2){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPointSize(4.0);
	glLineWidth(3.0);
	
	glPushMatrix();
	//glTranslatef(0.0f,-10.0f,-50.0f);
	
	glRotatef(view_rotx, 1.0, 0.0, 0.0);
	glRotatef(view_roty, 0.0, 1.0, 0.0);
	//glRotatef(view_rotz, 0.0, 0.0, 1.0);
    
	glPushMatrix();
	// Base
	glColor3f(.8, 0.8, .8);
	//4.0 for model base, 20 for scenario
	float halfSection = 70.0;
	glBegin(GL_QUADS);
		glVertex3f(-halfSection,0.0,-halfSection);
		glVertex3f(-halfSection,0.0,halfSection);
		glVertex3f(halfSection,0.0,halfSection);
		glVertex3f(halfSection,0.0,-halfSection);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, texture);
	//glScalef(0.1,0.1,0.1);
	glColor3f(.0, 0.8, .1);
	if(player->state==1) player->display();
	
	glColor3f(.8, 0.0, .1);
	for(int j=0; j<numEnemies; j++)
		if(enemies[j]->state==1)enemies[j]->display();

	if(block1->state==1) block1->display();
	//DrawBullets
	glBegin(GL_POINTS);
		for(int i=0;i<maxAmmo;i++){
			if(player->ammo[i]->state!=-1)
				player->ammo[i]->display();
			for(int j=0; j<numEnemies; j++){
				if(enemies[j]->ammo[i]->state!=-1){
					enemies[j]->ammo[i]->display();
				}
			}
		}
	glEnd();
	
	//glColor3f(0.0, 0.1, 0.8);
	//drawModel(model2);
	glPopMatrix();
	
	glPopMatrix();
	glutSwapBuffers();
	//glFlush();
}


void mousefunction(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		spin = fmod(spin + 30.0, 360);
		glutPostRedisplay();
    }
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D( 0.0, 640.0, 0.0, 480.0 );
	gluPerspective(45.0, (double)w / (double)h, 0.1, 220.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 170.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, -1.0);
	//glLoadIdentity();
}
  
void special(int key, int x, int y){
    switch(key){
    	case GLUT_KEY_UP: view_rotx += 3.0; break;
    	case GLUT_KEY_DOWN: view_rotx -= 3.0; break;
    	case GLUT_KEY_LEFT: view_roty += 3.0; break;
    	case GLUT_KEY_RIGHT: view_roty -= 3.0;break;
    }
    glutPostRedisplay();
}

void Display( void)  {
	glClear( GL_COLOR_BUFFER_BIT );
	//ifstream infile("Knuckle.obj");
	plotPoints();//,model2);
//	plotPoints(model2);
}

int timer=0;
void update(int value) {
	for(int i=0;i<maxAmmo;i++){
		//cout<<"shot "<<i<<" "<<player->ammo[i]->state<< endl;
		if(player->ammo[i]->state==1){
			player->ammo[i]->move();
		}
		for(int j=0; j<numEnemies; j++){
			if(enemies[j]->ammo[i]->state==1){
				enemies[j]->ammo[i]->move();
			}
		}
	}
	timer = (timer+1)%12;
	if(timer==0)
		for(int j=0; j<numEnemies; j++)
			if(enemies[j]->state==1)enemies[j]->AttackIA(player);
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

int main( int argc, char *argv[] )  {
	// Inicializar GLUT.
	glutInit( &argc, argv );
	// Definir Color RGB y Single Buffer.
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	// Definir la ventana en pixeles de la pantalla .
	glutInitWindowSize( 640, 480 );
	// Definir la posicion de la ventanaen pixeles de la pantalla.
	glutInitWindowPosition( 100, 150 );
	// Crear la ventana.
	glutCreateWindow( "Battle City - OpenGL" );
	// Definir la funcion de callback que usaremos para dibujar algo.

	glutDisplayFunc( Display );
	// Inicializar algunas cosas.
	Init( );
	//glutReshapeFunc(reshape);
	glutReshapeFunc(handleResize);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
    
	//glutKeyboardFunc(Key_Released);
	glutMouseFunc(mousefunction);
	
	glutTimerFunc(25, update, 0); //Add a timer
	// Ahora que tenemos todo definido, el loop  que responde  a eventos.
	glutMainLoop( );
	// Destroy everything
	for(int i = 0; i<SceneObjects.size(); i++)
		delete SceneObjects[i];
}

/* ----------------------------------------------------------------------- */
