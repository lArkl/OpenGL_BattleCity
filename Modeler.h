// IL + ASSIMP
#include <assimp/scene.h>
#include <map>
#include <string>
/*
void createAILogger();
void destroyAILogger();
void logInfo(std::string logString)
void logDebug(const char* logString)
*/
void createAILogger();
void destroyAILogger();
void logInfo(std::string logString);
/*
void logDebug(const char* logString)
*/
class Mesh{
    public:
    std::map<std::string, unsigned int*> textureIdMap;  // map image filenames to textureIds
    unsigned int* textureIds;
    const aiScene *scene;
    Mesh();
    Mesh(const std::string& pFile);
    ~Mesh();
    void setMesh(const std::string& pFile);
    void recursive_render (const struct aiScene *sc, const struct aiNode* nd, float scale);
    void apply_material(const aiMaterial *mtl);
    int LoadGLTextures(const std::string& modelpath);
    void drawAiScene();
};

//bool Import3DFromFile( const std::string& pFile, aiScene *scene);
//std::string getBasePath(const std::string& path)
//static std::string modelpath = "Knuckles/Knuckles.obj";
//int LoadGLTextures(const aiScene* scene);
//void drawAiScene(const aiScene* scene);
/*
void Color4f(const aiColor4D *color)
void set_float4(float f[4], float a, float b, float c, float d)
void color4_to_float4(const aiColor4D *c, float f[4])
void apply_material(const aiMaterial *mtl)
void recursive_render (const struct aiScene *sc, const struct aiNode* nd, float scale)
*/
