#include<iostream>
#include "Graph.h"
#include <list>
//#include "Object.h"
using namespace std;

Node::Node(){
	for(int i=0;i<4;i++)
		adj[i] = -1;
}

void Node::set(const int halfBase,const int halfDepth){
    const int cols = halfBase/4;
    const int rows = halfDepth/4;
    adj[0] = idx-cols<0? -1:idx-cols;
	adj[1] = (idx+1)%cols==0? -1:idx+1;
	adj[2] = idx+cols<rows*cols? idx+cols:-1;
	adj[3] = idx%cols==0? -1:idx-1;
    
    posX = (2*(idx%cols)+1)*4-halfBase;
    posZ = halfDepth-(2*(idx/cols)+1)*4;
}

Node::~Node(){}

Graph::Graph(int V)
{
    this->V = V;
    nodes = new Node[V];
}

Graph::~Graph(){
	delete[] nodes;
}

void Graph::generate(const int halfBase, const int halfDepth)
{
	cols = halfBase/4;
	rows = halfDepth/4;
	//cout<<cols<<endl;
	for(int i=0; i<V; i++){
		nodes[i].idx = i;
        nodes[i].set(halfBase,halfDepth);
        //cout<<nodes[i].posX<<" "<<nodes[i].posZ<<endl;
		/*
        nodes[i].adj[0] = i-cols<0? -1:i-cols;
		nodes[i].adj[1] = (i+1)%cols==0? -1:i+1;
		nodes[i].adj[2] = i+cols<V? i+cols:-1;
		nodes[i].adj[3] = i%cols==0? -1:i-1;
        */
	}
}

Node Graph::getNode(int idx){
	return nodes[idx];
}

int Graph::getV(){
	return V;
}

void Graph::randomMap()
{
	for(int i=0; i<V; i++){
		int r = rand()%100;
		if (r>80)
			nodes[i].numObject = 1;
		else 
			nodes[i].numObject = 0;
	}
}

int map1[18][21] = {
			{ 0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0 },
			{ 0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0 },
			{ 0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0 },
			{ 0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,0,0,0 },
			{ 0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,0,0,0 },
			{ 0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0 },
			{ 0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0 },
			{ 0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0 },
			{ 0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0 },
			{ 0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }		 
};

void Graph::readMap()
{	
	for(int i=0; i<V; i++){
		if(map1[i/cols][i%cols]>0)
			nodes[i].numObject = 1;
		else
			nodes[i].numObject = 0;
	}
}

void Graph::print(){
	for(int i = 0;i<rows;i++){
		for(int j =0;j<cols;j++){
			if(nodes[j+i*cols].numObject==1)
				cout<<" M ";
			else
				cout<< "   ";
		}
		cout<<endl;
	}
}

int Graph::BFS(int start,int end)
{
	// Mark all the vertices as not visited
	bool *visited = new bool[V];
	for(int i = 0; i < V; i++)
		visited[i] = false;
	// Create a queue for BFS
	list<int> queue;
	// Mark the current node as visited and enqueue it
	visited[start] = true;
	queue.push_back(start);
	int s=-1;
	while(!queue.empty() && s!=end)
	{
		// Dequeue a vertex from queue and print it
		s = queue.front();
//		cout << s << " ";
		queue.pop_front();
		for (int i = 0;i<4; i++)
		{	
			int idx = nodes[s].adj[i];
			if(idx >-1 ){
				if (!visited[idx] && nodes[idx].numObject==0)
				{
					visited[idx] = true;
					nodes[idx].parent = s;
					queue.push_back(idx);
                    //if()
				}
			}
		}
	}
	s = end;
	while(nodes[s].parent!=start){
		s = nodes[s].parent;
	}
	return s;
//	cout<<endl;
}

void Node::BFS(int end, int V)
{

}