#include "Object.h"
#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <IL/il.h>
#include <map>
#include <fstream>
using namespace std;

extern std::vector<Object*> SceneObjects;


const char* windowTitle = "OpenGL Framework";

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, 15.0f, 1.0f };



// the global Assimp scene object
const aiScene* scene = NULL;
GLuint scene_list = 0;
aiVector3D scene_min, scene_max, scene_center;

// images / texture
std::map<std::string, GLuint*> textureIdMap;	// map image filenames to textureIds
GLuint*		textureIds;							// pointer to texture Array



Object::Object()
{
	posX = 0.0;
	posZ = 0.0;
	direction = 0;
	state = 1;
}

Object::Object(float x,float z, int dir){
	posX = x;
	posZ = z;
	direction = dir;
	state = 1;//Tells if it's recently created
}

void Object::display(){
	radius = 4;
	int size = radius;
	glPushMatrix();
	glTranslatef(posX,size,posZ);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
		glTexCoord2f(0, 1); glVertex3f(-size,  size, -size);
		glTexCoord2f(1, 1); glVertex3f( size,  size, -size);
		glTexCoord2f(1, 0); glVertex3f( size, -size, -size);

		glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
		glTexCoord2f(0, 1); glVertex3f(-size,  size, size);
		glTexCoord2f(1, 1); glVertex3f( size,  size, size);
		glTexCoord2f(1, 0); glVertex3f( size, -size, size);

		glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
		glTexCoord2f(0, 1); glVertex3f(-size,  size, -size);
		glTexCoord2f(1, 1); glVertex3f(-size,  size,  size);
		glTexCoord2f(1, 0); glVertex3f(-size, -size,  size);

		glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
		glTexCoord2f(0, 1); glVertex3f(size,  size, -size);
		glTexCoord2f(1, 1); glVertex3f(size,  size,  size);
		glTexCoord2f(1, 0); glVertex3f(size, -size,  size);
		
		glTexCoord2f(0, 1); glVertex3f(-size, size,  size);
		glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
		glTexCoord2f(1, 0); glVertex3f( size, size, -size);
		glTexCoord2f(1, 1); glVertex3f( size, size,  size);
	glEnd();
	glPopMatrix();
}

Object::~Object(){};


Bullet::Bullet():Object(){
	state = -1;;//Tells if it's recently created
	radius = 1;
}

Bullet::Bullet(float x, float z, int dir):Object(x,z,dir)
{
	radius = 1;
	const int step = 4;
	switch (dir) {
		case 0: posZ = z + step;break;
		case 1: posX = x + step;break;
		case 2: posZ = z - step;break;
		case 3: posX = x - step;break;
	}
}

bool Bullet::destroyObject(Object *obj){
	state = -1;
	obj->state = 0;
	cout<<posX<<" "<<posZ<<" "<<endl;
	cout<<obj->posX<<" "<<obj->posZ<<" "<<endl;
	return true;
}

bool Bullet::impactOn()
{
	const float Limit = 70; // Scenario Limits
	if(posX > Limit || posX < -Limit || posZ > Limit || posZ <-Limit){
		state = -1;
		return true;
	}
	for(int i=0; i< SceneObjects.size(); i++){
		Object *obj = SceneObjects[i];
		if(obj == this|| owner == obj || obj->state<1) continue;
		float notDistance = (direction % 2 ==0)? posX - obj->posX: posZ - obj->posZ;
		float sumRadius = radius + obj->radius;
		if(abs(notDistance) > sumRadius) continue;
		float difInAxis;
		switch(direction){
			case 0: difInAxis = abs(posZ - obj->posZ); break;
			case 1: difInAxis = abs(posX - obj->posX); break;
			case 2: difInAxis = abs(posZ - obj->posZ); break;
			case 3: difInAxis = abs(posX - obj->posX); break;
		}
		if(difInAxis < sumRadius) return destroyObject(obj);



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

Tank::Tank():Object(){
	iniX = posX;
	iniZ = posZ;
	iDir = direction;
	radius = 4;
	reloadBullets();
}

Tank::Tank(float x,float z,int dir):Object(x,z,dir){
	iniX = x;
	iniZ = z;
	iDir = dir;
	radius = 4;
	reloadBullets();
}


void Tank::move(int dir){
	const float step = 0.9;
	direction = dir;
	if(limit(step))return;
	switch(direction){
		case 0: posZ += step;break;
		case 1: posX += step;break;
		case 2: posZ -= step;break;
		case 3: posX -= step;break;
	}
}

bool Tank::limit(float step){
	const float Limit = 60; //Tanks outside limit
	switch(direction){
		case 0: if(posZ + step > Limit) return true; break;
		case 1: if(posX + step > Limit) return true; break;
		case 2: if(posZ - step < -Limit) return true; break;
		case 3: if(posX - step < -Limit) return true; break;
	}
	float difInAxis;
	for(int i=0; i< SceneObjects.size(); i++){
		Object *obj = SceneObjects[i];
		//std::cout<<this<<" "<<obj <<std::endl;
		if(obj == this) continue;
		if(obj->state==0) continue;
		float notDistance = (direction % 2 ==0)? posX - obj->posX: posZ-obj->posZ;
		float sumRadius = radius + obj->radius;
		if(abs(notDistance) > sumRadius) continue; 
		
		switch(direction){
			case 0: difInAxis = abs(posZ + step - obj->posZ); break;
			case 1: difInAxis = abs(posX + step - obj->posX); break;
			case 2: difInAxis = abs(posZ - step - obj->posZ); break;
			case 3: difInAxis = abs(posX - step - obj->posX); break;
		}
		if(difInAxis < sumRadius) return true;
	}
	return false;
}


// IL + ASSIMP
void createAILogger()
{
    // Change this line to normal if you not want to analyse the import process
	//Assimp::Logger::LogSeverity severity = Assimp::Logger::NORMAL;
	Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;

	// Create a logger instance for Console Output
	Assimp::DefaultLogger::create("",severity, aiDefaultLogStream_STDOUT);

	// Create a logger instance for File Output (found in project folder or near .exe)
	Assimp::DefaultLogger::create("assimp_log.txt",severity, aiDefaultLogStream_FILE);

	// Now I am ready for logging my stuff
	Assimp::DefaultLogger::get()->info("this is my info-call");
}

void destroyAILogger()
{
	// Kill it after the work is done
	Assimp::DefaultLogger::kill();
}

void logInfo(std::string logString)
{
	// Will add message to File with "info" Tag
	Assimp::DefaultLogger::get()->info(logString.c_str());
}

void logDebug(const char* logString)
{
	// Will add message to File with "debug" Tag
	Assimp::DefaultLogger::get()->debug(logString);
}
bool Import3DFromFile( const std::string& pFile)
{
	// Check if file exists
	std::ifstream fin(pFile.c_str());
	if(!fin.fail())
	{
		fin.close();
	}
	else
	{
		//MessageBox(NULL, ("Couldn't open file: " + pFile).c_str() , "ERROR", MB_OK | MB_ICONEXCLAMATION);
		logInfo( importer.GetErrorString());
		return false;
	}

	scene = importer.ReadFile( pFile, aiProcessPreset_TargetRealtime_Quality);

	// If the import failed, report it
	if( !scene)
	{
		logInfo( importer.GetErrorString());
		return false;
	}

	// Now we can access the file's contents.
	logInfo("Import of scene " + pFile + " succeeded.");

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}


std::string getBasePath(const std::string& path)
{
	size_t pos = path.find_last_of("/");
	return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
}
//static std::string modelpath = "Knuckles/Knuckles.obj";
int LoadGLTextures(const aiScene* scene)
{
	ILboolean success;
	//std::cout<<"lelele"<<std::endl;
	/* Before calling ilInit() version should be checked. */
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		/// wrong DevIL version ///
		std::string err_msg = "Wrong DevIL version. Old devil.dll in system32/SysWow64?";
		char* cErr_msg = (char *) err_msg.c_str();
		//abortGLInit(cErr_msg);
		return -1;
	}

	ilInit(); /* Initialization of DevIL */

	if (scene->HasTextures())
		return -1;

	/* getTexture Filenames and Numb of Textures */
	for (unsigned int m=0; m<scene->mNumMaterials; m++)
	{
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;

		aiString path;	// filename

		while (texFound == AI_SUCCESS)
		{
			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			std::cout<<path.data<<std::endl;
			textureIdMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
			texIndex++;
		}
	}

	int numTextures = textureIdMap.size();

	/* array with DevIL image IDs */
	ILuint* imageIds = NULL;
	imageIds = new ILuint[numTextures];

	/* generate DevIL Image IDs */
	ilGenImages(numTextures, imageIds); /* Generation of numTextures image names */

	/* create and fill array with GL texture ids */
	textureIds = new GLuint[numTextures];
	glGenTextures(numTextures, textureIds); /* Texture name generation */

	/* get iterator */
	std::map<std::string, GLuint*>::iterator itr = textureIdMap.begin();

	std::string basepath = getBasePath(modelpath);
	for (int i=0; i<numTextures; i++)
	{

		//save IL image ID
		std::string filename = (*itr).first;  // get filename
		//std::cout<< filename << std::endl;
		(*itr).second =  &textureIds[i];	  // save texture id for filename in map
		itr++;								  // next texture


		ilBindImage(imageIds[i]); /* Binding of DevIL image name */
		std::string fileloc = basepath + filename;	/* Loading of image */
		success = ilLoadImage(fileloc.c_str());

		if (success) /* If no error occurred: */
		{
            // Convert every colour component into unsigned byte.If your image contains
            // alpha channel you can replace IL_RGB with IL_RGBA
            success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
			if (!success)
			{
				/* Error occurred */
				//abortGLInit("Couldn't convert image");
				return -1;
			}
            // Binding of texture name
            glBindTexture(GL_TEXTURE_2D, textureIds[i]);
			// redefine standard texture values
            // We will use linear interpolation for magnification filter
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            // We will use linear interpolation for minifying filter
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            // Texture specification
            glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
				ilGetData());
            // we also want to be able to deal with odd texture dimensions
            glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
            glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
            glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
            glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
		}
		else
		{
			/* Error occurred */
			//MessageBox(NULL, ("Couldn't load Image: " + fileloc).c_str() , "ERROR", MB_OK | MB_ICONEXCLAMATION);
		}
	}
    // Because we have already copied image data into texture data  we can release memory used by image.
	ilDeleteImages(numTextures, imageIds);

	// Cleanup
	delete [] imageIds;
	imageIds = NULL;

	return true;
}

// All Setup For OpenGL goes here
/*
int InitGL()
{
	if (!LoadGLTextures(scene))
	{
		return false;
	}


	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glClearColor(0.2f, .0f, .0f, 0.0f);
	glClearDepth(1.0f);				// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
	return true;					// Initialization Went OK
}
*/

// Can't send color down as a pointer to aiColor4D because AI colors are ABGR.
void Color4f(const aiColor4D *color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}

void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void apply_material(const aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;	// changed: to unsigned

	int texIndex = 0;
	aiString texPath;	//contains filename of texture

	if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
	{
		//bind texture
		/*
		aiString lele("Knuckles/");
		lele.Append(texPath.data);
		std::cout<<lele.data<<std::endl;
		*/
		unsigned int texId = *textureIdMap[texPath.data];
		glBindTexture(GL_TEXTURE_2D, texId);
	}

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
	if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}


void recursive_render (const struct aiScene *sc, const struct aiNode* nd, float scale)
{
	unsigned int i;
	unsigned int n=0, t;
	aiMatrix4x4 m = nd->mTransformation;

	aiMatrix4x4 m2;
	aiMatrix4x4::Scaling(aiVector3D(scale, scale, scale), m2);
	m = m * m2;

	// update transform
	m.Transpose();
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		apply_material(sc->mMaterials[mesh->mMaterialIndex]);


		if(mesh->mNormals == NULL)
		{
			glDisable(GL_LIGHTING);
		}
		else
		{
			glEnable(GL_LIGHTING);
		}

		if(mesh->mColors[0] != NULL)
		{
			glEnable(GL_COLOR_MATERIAL);
		}
		else
		{
			glDisable(GL_COLOR_MATERIAL);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices)
			{
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++)		// go through all vertices in face
			{
				int vertexIndex = face->mIndices[i];	// get group index for current index
				if(mesh->mColors[0] != NULL)
					Color4f(&mesh->mColors[0][vertexIndex]);
				if(mesh->mNormals != NULL)

					if(mesh->HasTextureCoords(0))		//HasTextureCoords(texture_coordinates_set)
					{
						glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
					}

					glNormal3fv(&mesh->mNormals[vertexIndex].x);
					glVertex3fv(&mesh->mVertices[vertexIndex].x);
			}
			glEnd();
		}
	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		recursive_render(sc, nd->mChildren[n], scale);
	}

	glPopMatrix();
}


void drawAiScene(const aiScene* scene)
{
	logInfo("drawing objects");
	recursive_render(scene, scene->mRootNode, 0.5);
}




void Tank::display()
{
	glPushMatrix();
	glTranslatef(posX,.0,posZ);
	glRotatef((direction-1)*90,0,1.0,0);
	drawAiScene(scene);
	glPopMatrix();
}

void Tank::display2(){
	glPushMatrix();
		glTranslatef(posX,.0,posZ);
		glRotatef((direction-1)*90,0,1.0,0);
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
				Point p = model->vertexs[index];
				glVertex3f(p.x,p.y,p.z );
			}	
		}
		glEnd();
	glPopMatrix();
}

void Tank::shoot(){
	int i = 0;
	while (i< maxAmmo-1 && ammo[i]->state>-1){
		i++;
	}
	//cout<<i<<" "<<ammo[i]->state<<endl;
	if (ammo[i]->state<0) {
		delete ammo[i];
		ammo[i] = new Bullet(posX,posZ,direction);
		ammo[i]->state=1;
		ammo[i]->owner = this;
	}
}
	//void AttackIA(Mapa, Nodo *);
	/*
    void BFS(Mapa, Nodo *);
	void DFSvisit(Nodo*);
	void DFS(Mapa, Nodo *);
	*/

void Tank::reloadBullets(){
	for (int i = 0; i< maxAmmo; i++){
		ammo[i] = new Bullet();
		//cout<<ammo[i]->alive<<endl;
	}
}

void Tank::respawn() {
	//if () {
	state = 1;
	posX = iniX;
	posZ = iniZ;
	reloadBullets();
		//Imprimir(direccion);
	//}
}

Tank::~Tank(){
	for(int i=0;i < maxAmmo;i++)
		//if(ammo[i] == nullptr)
			delete ammo[i];
}
