#include "data.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Model *readFile(string path){
	ifstream source(path);
	if(!source.is_open()){
		throw string("No se pudo leer el archivo");
	}
	std::vector <Point> vertexs;
	std::vector <Point> normals;
	std::vector <Dot> textures;
	std::vector <Face> faces;
	Dot d;
	Point p;
	Face f;
	string type;
	for(string line; getline(source, line); )   //read stream line by line
	{
		istringstream in(line);      //make a stream for the line itself
		if(line=="") continue;
		in >> type;                  //and read the first whitespace-separated token
		if(type == "v")       //and check its value
		{
			in >> p.x >> p.y >> p.z;       //now pass the whitespace-separated floats
			vertexs.push_back(p);
		}
		if(type == "f")       //and check its value
		{
			char slash;
			unsigned int number;
			for(int i=0;i<9;i++){
				in >> number;
				//cout<<number<<"\t";
				switch(i%3){
					case 0: f.vert[i/3] = number; break;
					case 1: f.vtext[i/3] = number; break;
					case 2: f.vnorm[i/3] = number; break;
				}
				if((i+1)%3!=0)
					in >> slash;
			}//cout<<endl;
			faces.push_back(f);
		}
		if(type=="vt"){
			in >> d.x >> d.y;       //now pass the whitespace-separated floats
			textures.push_back(d);
		}
		if(type=="vn"){
			in >> p.x >> p.y >> p.z;       //now pass the whitespace-separated floats
			normals.push_back(p);
		}
	}
	source.close();
	Model *model = new Model();
	model->faces=faces;
	model->vertexs=vertexs;
	model->textures=textures;
	model->normals=normals;
	cout<<"Archivo "<< path <<" leido"<<endl;
	return model;
}
