//#include<iostream>

class Node
{
	public:
	int idx;
	int parent;
	int numObject;
	int posX,posZ;
	int adj[4];
	Node();
	void set(const int,const int);
	void BFS(int end, int V);
	~Node();
};

class Graph
{
	int V;    // No. of vertices
	Node *nodes;
	public:
	Graph(int V);  // Constructor
	Node getNode(int);
	int getV();
	// function to add an edge to graph
	void generate(int,int);
	void randomMap();
	// prints BFS traversal from a given source s
	int BFS(int,int);
	~Graph();
};