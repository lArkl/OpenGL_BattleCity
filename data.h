#include<vector>
#include<string>

typedef struct Point{
	float x;
	float y;
	float z;
}Point;

typedef struct Dot{
	float x;
	float y;
}Dot;

typedef struct Face{
	unsigned int vert[3];
	unsigned int vtext[3];
	unsigned int vnorm[3];
}Face;

typedef struct Model{
	std::vector <Point> vertexs;
	std::vector <Point> normals;
	std::vector <Dot> textures;
	std::vector <Face> faces;
}Model;

Model *readFile(std::string);