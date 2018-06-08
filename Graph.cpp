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
	
	//cout<<cols<<endl;
	for(int i=0; i<V; i++){
		nodes[i].idx = i;
        nodes[i].set(halfBase,halfDepth);
        /*
        nodes[i].adj[0] = i-cols<0? -1:i-cols;
		nodes[i].adj[1] = (i+1)%cols==0? -1:i+1;
		nodes[i].adj[2] = i+cols<V? i+cols:-1;
		nodes[i].adj[3] = i%cols==0? -1:i-1;
        */
	}
}

void Graph::BFS(int s,int end)
{
	// Mark all the vertices as not visited
	bool *visited = new bool[V];
	for(int i = 0; i < V; i++)
		visited[i] = false;
	// Create a queue for BFS
	list<int> queue;
	// Mark the current node as visited and enqueue it
	visited[s] = true;
	queue.push_back(s);

	while(!queue.empty())
	{
		// Dequeue a vertex from queue and print it
		s = queue.front();
//		cout << s << " ";
		queue.pop_front();
		for (int i = 0;i<4; i++)
		{	
			if(nodes[s].adj[i]>-1){
				int idx = nodes[s].adj[i];
				if (!visited[idx])
				{
					visited[idx] = true;
					queue.push_back(idx);
                    //if()
				}
			}
		}
	}
//	cout<<endl;
}

void Node::BFS(int end, int V)
{

}