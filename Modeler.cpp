// IL + ASSIMP
#include "Modeler.h"
#include <IL/il.h>

#include <fstream>
#include <assimp/Importer.hpp>	//OO version Header!
#include <assimp/postprocess.h>
#include <GL/gl.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include <iostream>

using namespace std;
Assimp::Importer importer;
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

const aiScene* Import3DFromFile( const std::string& pFile, const aiScene *scene)
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
		return scene;
	}
	
	scene = importer.ReadFile( pFile, aiProcessPreset_TargetRealtime_Quality);
	//scene = importer.GetOrphanedScene();
	// If the import failed, report it
	if( !scene)
	{
		logInfo( importer.GetErrorString());
		return scene;
	}

	// Now we can access the file's contents.
	logInfo("Import of scene " + pFile + " succeeded.");

	// We're done. Everything will be cleaned up by the importer destructor
	return scene;
	//return true;
}


std::string getBasePath(const std::string& path)
{
	size_t pos = path.find_last_of("/");
	return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
}

int Mesh::LoadGLTextures(const std::string& modelpath)
{
	ILboolean success;
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
		cout<<"texfau: "<<texFound<<endl;
		while (texFound == AI_SUCCESS)
		{
			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			cout<<"textfound: "<<texFound<<endl;
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

void Mesh::apply_material(const aiMaterial *mtl)
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

void Mesh::recursive_render (const struct aiScene *sc, const struct aiNode* nd, float scale)
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

void Mesh::drawAiScene()
{
	logInfo("drawing objects");
	recursive_render(scene, scene->mRootNode, 0.5);
}

Mesh::Mesh(){
	scene = NULL;
}

Mesh::Mesh(const std::string& pFile){
	scene = Import3DFromFile(pFile, scene);
	//cout<<"ptr:"<<scene<<endl;
	//cout<<"lele "<<"numMaterials: "<<scene->mNumMaterials<<endl;
	
    LoadGLTextures(pFile);
}

void Mesh::setMesh(const std::string& pFile){
	Import3DFromFile(pFile, scene);
	//cout<<"lele "<<"numMaterials: "<<scene->mNumMaterials<<endl;
    LoadGLTextures(pFile);
}

Mesh::~Mesh(){
    delete scene;
	textureIdMap.clear(); //no need to delete pointers in it manually here. (Pointers point to textureIds deleted in next step)
	if (textureIds)
	{
		delete[] textureIds;
		textureIds = NULL;
	}
	destroyAILogger();
}