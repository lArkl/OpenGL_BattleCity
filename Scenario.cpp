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
	//Llamada global para no hacer lectura constante	
	//string path1 = "Models/Knuckles/Knuckles.obj";
	//string path = "sample.txt";
	//string path1 = "Models/Caral/caral_piramide.obj";
	//string path1 = "Models/Tank/BaseTank.obj";
	//string path1 = "Models/Tank2/Tank.obj";
	std::string path1 = "Models/Tank/BaseTank.obj";
	Model *tankModel = readFile(path1);
	player = new Tank(0,0,0);
	enemy1 = new Tank(40,30,2);
	player->model = tankModel;
	enemy1->model = tankModel;
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
	glColor3f(.0, 0.8, .1);
	player->display();
	glColor3f(.8, 0.0, .1);
	enemy1->display();
	block1->display();
	//DrawBullets
	glBegin(GL_POINTS);
		for(int i=0;i<5;i++){
			if(player->ammo[i]->alive==false) continue;
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

void reshape(int width, int height){
	int x = width<height? width: height;
	glViewport(0, 0, x, x);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60.0, 1, 1.0, 128.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0.0, 2.0, 1.5, 0.0, -5.0, 0.2, 0.0, 1.0, 0.0);
	//gluLookAt(0.0, 2.0, 2.0, 0.0, 1.0, 0.0, 0.0, -1.0, -1.0);
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D( 0.0, 640.0, 0.0, 480.0 );
	gluPerspective(45.0, (double)w / (double)h, 0.1, 180.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 170.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, -1.0);
	//glLoadIdentity();
}

void displayText( float x, float y, float r, float g, float b ) {
	
	string name = "Attack";
    
	glColor3f( r, g, b );
	glRasterPos2f( x, y );
	for( int i = 0; i < name.length(); i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, name[i] );
	}
}
   
void special(int key, int x, int y){
    switch(key){
    	case GLUT_KEY_UP: view_rotx += 5.0; break;
    	case GLUT_KEY_DOWN: view_rotx -= 5.0; break;
    	case GLUT_KEY_LEFT: view_roty += 5.0; break;
    	case GLUT_KEY_RIGHT: view_roty -= 5.0;break;
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
	for(int i=0;i<5;i++){
		if(player->ammo[i]->alive){
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
	glutCreateWindow( "Tanks" );
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
	displayText(50,100, 0.85f,0.75f,0.0f);
	
	glutTimerFunc(25, update, 0); //Add a timer
	// Ahora que tenemos todo definido, el loop  que responde  a eventos.
	glutMainLoop( );
}

/* ----------------------------------------------------------------------- */
