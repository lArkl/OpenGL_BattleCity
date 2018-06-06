/* -- INCLUDE FILES ------------------------------------------------------ */

#include "keyhandler.h"
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
//#include "data.h"
#include "Object.h"

using namespace std;


unsigned int texture;
const int textnum = 1;

GLuint texID[textnum];


vector<Object*> SceneObjects;
Tank *player;
Tank *enemy1;
Object *block1;
/*
Tank enemy1(50,0,50);
Tank enemy2(-50,0,50);
*/
void Init( void )  {
    //LoadTextures();
	if (!LoadGLTextures(scene))
	{
		return false;
	}
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
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	/* AssimpTex
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
	*/
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	
	
	
	//Llamada global para no hacer lectura constante	
	//string path1 = "Models/Knuckles/Knuckles.obj";
	//string path = "sample.txt";
	//string path1 = "Models/Caral/caral_piramide.obj";
	//string path1 = "Models/Tank/BaseTank.obj";
	//string path1 = "Models/Tank2/Tank.obj";
	std::string path1 = "Models/Tank/tank2.obj";
	Model *tankModel = readFile(path1);
	player = new Tank(0,0,0);
	enemy1 = new Tank(40,30,2);
	player->model = tankModel;
	enemy1->model = tankModel;
	//cout<<player->alive<<endl;
	block1 = new Object(0,40,0);
	SceneObjects.push_back(player);
	SceneObjects.push_back(block1);
	SceneObjects.push_back(enemy1);
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
	//glColor3f(.0, 0.8, .1);
	if(player->state==1) player->display();
	
	//glColor3f(.8, 0.0, .1);
	if(enemy1->state==1) enemy1->display();
	
	if(block1->state==1) block1->display();
	//DrawBullets
	glBegin(GL_POINTS);
		for(int i=0;i<maxAmmo;i++){
			if(player->ammo[i]->state!=-1)
				player->ammo[i]->display();
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
/* 
Crear texto

void initText2D(const char * texturePath);
void printText2D(const char * text, int x, int y, int size);
void cleanupText2D();
std::vector<glm::vec2> vertices;
std::vector<glm::vec2> UVs;
*/

//https://github.com/sprintr/opengl-examples/blob/master/OpenGL-Menu.cpp





/* ----------------------------------------------------------------------- */
/* Function    : void myDisplay( void )
 *
 * Description : This function gets called everytime the window needs to
 *               be redrawn.
 *
 * Parameters  : void
 *
 * Returns     : void
 */

void Display( void)  {
	glClear( GL_COLOR_BUFFER_BIT );
	//ifstream infile("Knuckle.obj");
	plotPoints();//,model2);
//	plotPoints(model2);
}

void update(int value) {
	for(int i=0;i<maxAmmo;i++){
		//cout<<"shot "<<i<<" "<<player->ammo[i]->state<< endl;
		if(player->ammo[i]->state==1){
			player->ammo[i]->move();
		}
	}
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
}

/* ----------------------------------------------------------------------- */
/*
int main(int argc, char **argv)
{
	//struct aiLogStream stream;
	glutInitWindowSize(900,600);
	glutInitWindowPosition(100,100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	glutCreateWindow("Assimp - Very simple OpenGL sample");
	glutDisplayFunc(DrawGLScene);
	glutReshapeFunc(ReSizeGLScene);

	createAILogger();
	logInfo("App fired!");
	
	std::cout<<"lelel: "<<argv[1]<<std::endl;
	
	if (argc > 1)
	{
		std::string modelpathW(argv[1]);
		modelpath = std::string(modelpathW.begin(), modelpathW.end());
		if (!Import3DFromFile(modelpath)) return 0;	
	}

	glClearColor(0.8f,0.8f,0.8f,1.f);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters 

	glEnable(GL_DEPTH_TEST);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	if(getenv("MODEL_IS_BROKEN"))
		glFrontFace(GL_CW);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

	glutGet(GLUT_ELAPSED_TIME);
	InitGL();
	glutMainLoop();
	
	//aiReleaseImport(scene);
	textureIdMap.clear(); //no need to delete pointers in it manually here. (Pointers point to textureIds deleted in next step)
	if (textureIds)
	{
		delete[] textureIds;
		textureIds = NULL;
	}
	destroyAILogger();
	return 0;
}
*/