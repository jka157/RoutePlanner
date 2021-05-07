
//libraries
#include <iostream>
#include <vector>
#include "GL/glew.h" 
#include "GL/glut.h" 
#include <utility>
#include <limits>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "GraphElements.h"
#include "Graph.h"
#include "DijkstraShortestPathAlg.h"
#include "YenTopKShortestPathsAlg.h"
#include "Graph.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;


////////////////////////structs ////////////////////////////////////////////////////////////////////////////////
struct node {//struct to hold information on nodes
	pair<int, int> nodeCoords;//x and y coords
	bool selected, passenger, destination;//booleans for if node is selected, if node is a passneger pickup, or if node is a dropoff
	double passProb;//probability of a passenger calling from node
	vector<int> ConnectedLinks;//index values of links leaving node
	
};

struct Agent {//struct to contain information on agent
	float x, y, currentNode, nextNode,passengers,Distweigth,Tweight,Pweight,Destweight,Aweight;//x and y coords, the agents current node, the next node, and how many passengers it is carrying
	bool moving;
};

struct path {//struct to contain information on a path
	vector<int> P;//vector of nodes in path
	double Length, Tutility, Putility,Dutility,TotalUtility;//length of path, traffic utility of path, pasenger utility, destination utility, total utility
	bool blocked;//if the next link in the path is blocked, so the agent cannot use it


};


/////////////////////////////////////Global Varibles/////////////////////////////////////////////////////////////////
vector<node> nodes = {//vector containing information on all nodes in network
	//{xcoord, ycoord},selected, passenger,destination,passProb
	{{260, 10},false,false,false,0.009},
{{ 335, 723 - 595},false,false,false,0.009},
{{355, 723 - 615 },false,false,false,0.009},
{{423,723 - 679},false,false,false,0.009},
{{98,723 - 223},false,false,false,0.009},
{{211,723 - 338 },false,false,false,0.009},
{{359,723 - 487},false,false,false,0.009},
{{404,723 - 526},false,false,false,0.009},
{{422, 723 - 549},false,false,false,0.009},
{{521, 723 - 646},false,false,false,0.0009},
{{574, 723 - 633},false,false,false,0.009},
{{161, 723 - 162},false,false,false,0.01},
{{280, 723 - 274},false,false,false,0.001},
{{431, 723 - 417},false,false,false,0.001},
{{468, 723 - 461},false,false,false,0.001},
{{491, 723 - 481},false,false,false,0.001},
{{530, 723 - 521},false,false,false,0.001},
{{653, 723 - 647},false,false,false,0.001},
{{555, 723 - 415},false,false,false,0.001},
{{597, 723 - 453},false,false,false,0.001},
{{265, 723 - 58},false,false,false,0.001},
{{380, 723 - 167},false,false,false,0.001},
{{484, 723 - 273},false,false,false,0.01},
{{526, 723 - 314 },false,false,false,0.001},
{{570, 723 - 356},false,false,false,0.01},
{{591, 723 - 376},false,false,false,0.001},
{{725, 723 - 512},false,false,false,0.001},
{{791, 723 - 554 },false,false,false,0.001},
{{563, 723 - 281},false,false,false,0.001},
{{606, 723 - 319},false,false,false,0.001},
{{626, 723 - 341},false,false,false,0.001},
{{760, 723 - 475},false,false,false,0.001},
{{585, 723 - 175},false,false,false,0.001},
{{698, 723 - 223},false,false,false,0.001},
{{762, 723 - 254},false,false,false,0.01}
};

vector<Link> links = {//vector containing information on all links in the network
	//start node, end node, static weight, associated link in the other direction, accident on link, PAA, PNA, THM, THH, TLM, TLL,TMH, TML
{0,2,3,51,false, 0.8, 0.005,0.1, 0.8,0.1,0.8,0.1,0.1},//0                 
{1,2,1,52,false, 0.7, 0.007,0.1, 0.8,0.1,0.8,0.1,0.1},//1                 
{1,7,3,53,false, 0.7, 0.02,0.1, 0.8,0.1,0.8,0.1,0.1},//2                 
{2,3,3,54,false, 0.8, 0.01,0.1, 0.8,0.1,0.8,0.1,0.1},//3
{2,8,3,55,false, 0.7, 0.009,0.1, 0.8,0.1,0.8,0.1,0.1},//4
{3,9,3,56,false, 0.7, 0.009,0.1, 0.8,0.1,0.8,0.1,0.1},//5
{4,5,4,57,false, 0.7, 0.005,0.1, 0.8,0.1,0.8,0.1,0.1},//6
{4,11,3,58,false, 0.7, 0.005,0.1, 0.8,0.1,0.8,0.1,0.1},//7
{5,12,3,59,false, 0.7, 0.01,0.1, 0.8,0.1,0.8,0.1,0.1},//8
{5,6,5,60,false, 0.7, 0.03,0.1, 0.8,0.1,0.8,0.1,0.1},//9
{6,7,2,61,false, 0.7, 0.02,0.1, 0.1,0.1,0.1,0.1,0.1},//10
{6,13,3,62,false, 0.7, 0.02,0.1, 0.1,0.1,0.1,0.1,0.1},//11
{7,8,1,63,false, 0.8, 0.015,0.1, 0.1,0.1,0.1,0.1,0.1},//12
{7,14,3,64,false, 0.7, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//13
{8,9,4,65,false, 0.7, 0.035,0.1, 0.1,0.1,0.1,0.1,0.1},//14
{8,15,3,66,false, 0.65, 0.03,0.1, 0.1,0.1,0.1,0.1,0.1},//15
{9,10,2,67,false, 0.7, 0.015,0.1, 0.1,0.1,0.1,0.1,0.1},//16
{10,17,2,68,false, 0.7, 0.015,0.1, 0.1,0.1,0.1,0.1,0.1},//17
{11,12,4,69,false, 0.6, 0.005,0.1, 0.1,0.1,0.1,0.1,0.1},//18
{11,20,4,70,false, 0.7, 0.03,0.1, 0.1,0.1,0.1,0.1,0.1},//19
{12,21,4,71,false, 0.7, 0.03,0.1, 0.1,0.1,0.1,0.1,0.1},//20
{12,13,5,72,false, 0.7, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//21
{13,14,2,73,false, 0.7, 0.035,0.1, 0.1,0.1,0.1,0.1,0.1},//22
{13,23,4,74,false, 0.75, 0.04,0.1, 0.1,0.1,0.1,0.1,0.1},//23
{14,15,1,75,false, 0.65, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//24
{14,24,4,76,false, 0.75, 0.035,0.1, 0.1,0.1,0.1,0.1,0.1},//25
{15,16,2,77,false, 0.7, 0.03,0.1, 0.1,0.1,0.1,0.1,0.1},//26
{15,18,3,78,false, 0.7, 0.035,0.1, 0.1,0.1,0.1,0.1,0.1},//27
{16,17,4,79,false, 0.5, 0.01,0.1, 0.1,0.1,0.1,0.1,0.1},//28
{16,19,3,80,false, 0.7, 0.035,0.1, 0.1,0.1,0.1,0.1,0.1},//29
{17,27,4,81,false, 0.7, 0.01,0.1, 0.1,0.1,0.1,0.1,0.1},//30
{18,19,2,82,false, 0.7, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//31
{18,25,1,83,false, 0.7, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//32
{20,21,4,84,false, 0.8, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//33
{21,22,3,85,false, 0.75, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//34
{22,23,2,86,false, 0.7, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//35
{22,32,4,87,false, 0.9, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//36
{23,24,2,88,false, 0.7, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//37
{23,28,1,89,false, 0.6, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//38
{24,25,1,90,false, 0.8, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//39
{24,29,1,91,false, 0.76, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//40
{25,26,4,92,false, 0.8, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//41
{25,30,1,93,false, 0.68, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//42
{26,27,2,94,false, 0.7, 0.01,0.1, 0.1,0.1,0.1,0.1,0.1},//43
{26,31,2,95,false, 0.67, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//44
{28,29,2,96,false, 0.7, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//45
{29,30,1,97,false, 0.7, 0.025,0.1, 0.1,0.1,0.1,0.1,0.1},//46
{29,33,3,98,false, 0.55, 0.015,0.1, 0.1,0.1,0.1,0.1,0.1},//44
{30,31,4,99,false, 0.65, 0.015,0.1, 0.1,0.1,0.1,0.1,0.1},//48
{32,33,3,100,false, 0.7, 0.005,0.1, 0.1,0.1,0.1,0.1,0.1},//49
{33,34,2,101,false, 0.78, 0.005,0.1, 0.1,0.1,0.1,0.1,0.1},//50
{2,0,3,0,false},//51
{2,1,1,1,false},//52
{7,1,3,2,false},//53
{3,2,3,3,false},//54
{8,2,3,4,false},//55
{9,3,3,5,false},//56
{5,4,4,6,false},//57
{11,4,3,7,false},//58
{12,5,3,8,false},//59
{6,5,5,9,false},//60
{7,6,2,10,false},//61
{13,6,3,11,false},//62
{8,7,1,12,false},//63
{14,7,3,13,false},//64
{9,8,4,14,false},//65
{15,8,3,15,false},//66
{10,9,2,16,false},//67
{17,10,2,17,false},//68
{12,11,4,18,false},//69
{20,11,4,19,false},//70
{21,12,4,20,false},//71
{13,12,5,21,false},//72
{14,13,2,22,false},//73
{23,13,4,23,false},//74
{15,14,1,24,false},//75
{24,14,4,25,false},//76
{16,15,2,26,false},//77
{18,15,3,27,false},//78
{17,16,4,28,false},//79
{19,16,3,29,false},//80
{27,17,4,30,false},//81
{19,18,2,31,false},//82
{25,18,1,32,false},//83
{21,20,4,33,false},//84
{22,21,3,34,false},//85
{23,22,2,35,false},//86
{32,22,4,36,false},//87
{24,23,2,37,false},//88
{28,23,1,38,false},//89
{25,24,1,39,false},//90
{29,24,1,40,false},//91
{26,25,4,41,false},//92
{30,25,1,42,false},//93
{27,26,2,43,false},//94
{31,26,2,44,false},//95
{29,28,2,45,false},//96
{30,29,1,46,false},//97
{33,29,3,47,false},//98
{31,30,4,48,false},//99
{33,32,3,49,false},//100
{34,33,2,50,false}//101
};
Agent Taxi = { nodes[0].nodeCoords.first,nodes[0].nodeCoords.second,0 ,false,0,-2,-1,1,1,-100};//initialize global taxi agent
vector<path> paths(20);//global vector holding information on possible paths from current node to next destination
int updateCount;//gloab varible to keep cound of updates while moving agent from one node to next
vector<int> currentPath;//the path the agent is currently following
int pathCount = 0;//count of the number of paths from current node to end node
std::knuth_b rand_engine;//random number generator
float xtran;//change in x for movement
float ytran;//change in y for movement
float MoveDistance;//static weight of current link
int movementStages = 3;//number of stages to move. Used for incremental movements on link
vector<int> destinations;//vector containing list of destinations for agent to visit
GLuint tex;//texture
unsigned char* texture_data;//texture data
int Gcount;//global count of number of movements the agent has made (also count of number of times dynamic paramaters have been updated)
double HighT = 60,MedT=30, LowT=10;//values for low, medium and high traffic
bool Debug;//debug boolean


void drawNetwork() {//draw background map, all links and nodes. Color of links and nodes changes based on properties of the associated struct
	
	//draw background quad textured with map
	glClear(GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, tex);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 1); glVertex2i(0, 723);
	glTexCoord2i(1, 1); glVertex2i(1000, 723);
	glTexCoord2i(1, 0); glVertex2i(1000, 0);
	glTexCoord2i(0, 0); glVertex2i(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glMatrixMode(GL_PROJECTION);

	//draw links
	glLineWidth(8);
	
	for (int i = 0; i < links.size(); i++) {
		glBegin(GL_LINES);
		glColor3f(links[i].Traffic / 60.0, 1.0 - (links[i].Traffic / 60.0), 0);//color of link changes based on traffic value
		if (links[i].Traffic == LowT) {
			glColor3f(0, .4, 0);

		}
		else if (links[i].Traffic == MedT) {
			glColor3f(1, .65, 0);
		}
		else {
			glColor3f(.7, 0, 0);
		}
		glVertex2i(nodes[links[i].u].nodeCoords.first, nodes[links[i].u].nodeCoords.second);
		glVertex2i(nodes[links[i].v].nodeCoords.first, nodes[links[i].v].nodeCoords.second);
		glEnd();
	}
	//draw links with accidents
	glLineWidth(8);
	for (int i = 0; i < links.size() / 2; i++) {
		if (links[i].accident == true) {
			//blue link
			glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 1.0);
			glVertex2i(nodes[links[i].u].nodeCoords.first, nodes[links[i].u].nodeCoords.second);
			glVertex2i(nodes[links[i].v].nodeCoords.first, nodes[links[i].v].nodeCoords.second);
			glEnd();
			//black point
			glPointSize(20);
			glBegin(GL_POINTS);
			glColor3f(0, 0.0, 0.0);
			glVertex2i((nodes[links[i].u].nodeCoords.first + nodes[links[i].v].nodeCoords.first) / 2, (nodes[links[i].u].nodeCoords.second + nodes[links[i].v].nodeCoords.second) / 2);
			glEnd();
		}
	}
	//draw current path of decision agent
	glLineWidth(3);
	if (currentPath.size() != 0) {
		for (int i = 0; i < currentPath.size() - 1; i++) {
				glColor3f(1, 0, 1);
				glBegin(GL_LINES);
				
				glVertex2i(nodes[currentPath[i]].nodeCoords.first, nodes[currentPath[i]].nodeCoords.second);
				glVertex2i(nodes[currentPath[i + 1]].nodeCoords.first, nodes[currentPath[i + 1]].nodeCoords.second);
				glEnd();
		}
	}
	//draw nodes
	glPointSize(10);
	glBegin(GL_POINTS);
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].passenger == true) {//passenger node
			glColor3f(0, 1, .3);
		}
		else if (nodes[i].destination == true) {//destination node
			glColor3f(1, 0, 0);
		}
		else {
			glColor3f(0, 0, 0);//empty node
		}
		glVertex2i(nodes[i].nodeCoords.first, nodes[i].nodeCoords.second);
	}
	glEnd();

}

void loadTexture() {//load png into texture
	//turrn png into texture data readable by gl
	int w, h;
	stbi_set_flip_vertically_on_load(true);
	texture_data = stbi_load("Map.png", &w, &h, nullptr, 4);//get texture data from png
	//upload to GPU texture
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texture_data);//free texture data
	glMatrixMode(GL_PROJECTION);//reset camera view
	glOrtho(0, 1000, 0, 723, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

void YenAlg(const int& k,
	vector<link> lk,
	const int& size,
	const int& s,
	const int& d,
	const int& nodes)
{	//find K shortest paths from node s to node d, based on vector of links connecting nodes with set weights. 
	//credit to Andy
	//https://www.technical-recipes.com/2012/the-k-shortest-paths-algorithm-in-c/
	Graph my_graph;
	my_graph.set_number_vertices(nodes);
	for (int i = 0; i < size; i++)
	{
		my_graph.add_link(lk[i].u, lk[i].v, lk[i].weight);
	}
	my_graph.setv();
	YenTopKShortestPathsAlg yenAlg(my_graph,
		my_graph.get_vertex(s),
		my_graph.get_vertex(d));
	//output paths to global paths vector, with length of path
	int i = 0;
	int c = 0;
	while (yenAlg.has_next() && i < k)//go throug all 20 possible paths
	{
		++i;
		pathCount++;//increment global path count
		BasePath path = *yenAlg.next();
		int size = path.length();
		vector<int> P;
		for (int j = 0; j < size; j++) {
			BaseVertex node = *path.GetVertex(j);
			int nodeID = node.getID();
			P.push_back(nodeID);//push back nodes to vector
		}
		paths[c].P = P;//add vector of nodes to current path
		paths[c].Length = path.length();//add length of current path
		paths[c].blocked = false;
		c++;
	}
}
void drawAgent() {//draw taxi agent at x and y coordinates set in agent struct
	glPointSize(10);
	glColor3f(1, 0, 1);
	glBegin(GL_POINTS);
	glVertex2f(Taxi.x, Taxi.y);
	glEnd();

}
void draw() {//draw background image, nodes and links, and taxi
	drawNetwork();//background image, nodes, links
	drawAgent();//agent
	glColor3f(1, 1, 1);//reset color
	glFlush();
}
bool rand_bool(double prob) {//return true of false given probability
	bernoulli_distribution d(prob);
	return d(rand_engine);
}
int findDest() {//find random node that is not currently a passenger or destination
	while (true) {
		int i = rand() % 34;//random int between 0 and 34
		if (nodes[i].passenger == false and nodes[i].destination == false) {//if not currently a passenger or destination
			return i;
		}
	}

}
void pow2(double in[2][2], double out[2][2], int n)
//set output array to be the input array to the power of n. input and output arrays must be 2X2 arrays
{
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			out[i][j] = (i == j);
		}
	}
	double temp[2][2];
	for (int w = 0; w < n; w++)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				temp[i][j] = 0;
				for (int k = 0; k < 2; k++)
				{
					temp[i][j] += (out[i][k] * in[k][j]);
				}
			}
		}
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
			{
				out[i][j] = temp[i][j];
			}
		}
	}
}
void pow3(double in[3][3], double out[3][3], int n)
//set output array to be the input array to the power of n. input and output arrays must be 2X2 arrays
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			out[i][j] = (i == j);
		}
	}
	double temp[3][3];
	for (int w = 0; w < n; w++)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				temp[i][j] = 0;
				for (int k = 0; k < 3; k++)
				{
					temp[i][j] += (out[i][k] * in[k][j]);
				}
			}
		}
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
			{
				out[i][j] = temp[i][j];
			}
		}
	}
}

void execute() {//Function defined as the glut idle function. 
	if(Taxi.moving==false){
	/////////////////////Update Dynamic Network Parameters//////////////////////////////////
		Gcount++;//increment cound of updates

		///////////////////////generate passengers///////////////////////////////
		for (int i = 0; i < nodes.size(); i++) {
			if (rand_bool(nodes[i].passProb) == true and nodes[i].passenger==false and  nodes[i].destination ==false and Taxi.currentNode!=i) {//if not destination or passenger, not current node, and rand_bool returns true
				nodes[i].passenger = true;//set passenger
				destinations.push_back(i);//add to back of destinations list
			}
		}
		////////////////////////////generate accident and traffic values////////////////////////////////////////////////
		for (int i = 0; i < links.size() / 2; i++) {//only go halfway through nodes, traffic and accident is same for both directions
			if (links[i].accident == false) {
				links[i].accident = rand_bool(links[i].PNA);//set innitial accidents (No accident to accident)
				links[links[i].EqLink].accident = links[i].accident;
			}
			else {
				links[i].accident = rand_bool(links[i].PAA);//set innitial accidents (No accident to accident)
				links[links[i].EqLink].accident = links[i].accident;

			}
			///////////////////////Traffic Generation//////////////////////////////
			int randInt = rand() % 100 + 1;//get random integer between 1 and 100
			double Traffic;
			if (links[i].Traffic == LowT) {//current state is low traffic
				if (randInt <= links[i].TLL * 100) {//random int is between 1 and TLL*100, trafffic transitions to low traffic
					Traffic = LowT;
				}
				else if ((randInt > links[i].TLL * 100) and (randInt<=(links[i].TLL+links[i].TLM)*100)) {
					//random int is between TLL*100 and (TLL+TLM)*100, trafffic transitions to medium traffic
					Traffic = MedT;
				}
				else {
					//random int is greater than (TLL+TLM)*100, trafffic transitions to high traffic
					Traffic = HighT;
				}
			}
			else if (links[i].Traffic == MedT) {//current state is medium traffic
				if (randInt <= links[i].TML * 100) {//random int is between 1 and TML*100, trafffic transitions to low traffic
					Traffic = LowT;
				}
				else if ((randInt > links[i].TML * 100) and (randInt <= (links[i].TML + links[i].TMH) * 100)) {
					//random int is between TML*100 and (TML+TMH)*100, trafffic transitions to high traffic
					Traffic = HighT;
				}
				else {//random int is greater than (TML+TMH)*100, trafffic transitions to medium traffic
					Traffic = MedT;
				}
			}
			else {//current state is high traffic
				if (randInt <= links[i].THM * 100) {//random int is between 1 and THM*100, trafffic transitions to medium traffic
					Traffic = MedT;
				}
				else if ((randInt > links[i].THM * 100) and (randInt <= (links[i].THM + links[i].THH) * 100)) {
					//random int is between THM*100 and (THM+THH)*100, trafffic transitions to high traffic
					Traffic = HighT;
				}
				else {//random int is greater than (THM+THH)*100, trafffic transitions to low traffic
					Traffic = LowT;
				}
			}
			links[i].Traffic = Traffic;//set link traffic value
			links[links[i].EqLink].Traffic = Traffic;//set the link in the opposite direction to have the same traffic value
		}
	}
	////////////////////////////////////////Decision Agent Decision process and movement/////////////////////////////////////////
	if (destinations.size() > 0) {
		if (!Taxi.moving) {//decide on new path when not moving from node to node
			int currentDestination = destinations[0];//get current destnation

			if (nodes[currentDestination].passenger == true) {//If the decision agent does not currently have a passenger(there are no destination nodes in destinations vector), look for the closest passenger to pick up.
				YenAlg(1, links, links.size(), Taxi.currentNode, currentDestination, 35);//find current length of path
				if (paths[0].Length > 1) {//if current destination is more than 3 away (had error where agent got stuck moving between 2 nodes if this distinction is not made)
						int shortestlength = 100;
						int shortestindex = 0;
								for (int i = 0; i < destinations.size(); i++) {//for all current passenger nodes in destinations vector
									YenAlg(1, links, links.size(), Taxi.currentNode, destinations[i], 35);//find shortest path to passenger node
											if (paths[0].Length < shortestlength) {//distance to current passenger is shorter than current shortest path
												shortestlength = paths[0].Length;//set shortest path length
												shortestindex = i;//set shortest path index

											}
								}
						currentDestination = destinations[shortestindex];//set current destination to closets passenger
				}
			}
			pathCount = 0;
			YenAlg(20, links, links.size(), Taxi.currentNode, currentDestination, 35);//find 20 shortest paths based on static distances. paths are added to paths vector
			if (Debug == true) {//if in debug mode, output information
				cout << "\nPossible Paths from node " << Taxi.currentNode << " to node " << currentDestination<<":"<<endl;
			}
			///////////////////////Assigning Utility to paths//////////////////////////////////////
			double BestUtility = -9999;
			int BestPath = -1;
			for (int i = 0; i < pathCount; i++) {//for each path in paths vector
				path CurPa = paths[i];//get current path
				double Paccident = 0;
				double Tutility = 0;
				double Dutility = 0;
				double Putility = 0;
				int tempPassengers = Taxi.passengers;//get current amount of passengers
				for (int j = 1; j < CurPa.P.size(); j++) {//for all nodes in current path
					int LstartNode = CurPa.P[j-1];//start node of link
					int LendNode = CurPa.P[j];//end node of link
					link CurLink = links[nodes[LstartNode].ConnectedLinks[LendNode]];//link between start and end nodes
					////////////////////accident utility////////////////////////
					if (j == 1) {//If the current link is the next link the agent will have to cross
						if (CurLink.accident == true) {//if the next link is currently blocked by an accident
							Paccident++;//add 100% to probability of encountering an accident
							paths[i].blocked = true;//indicate that the path is blocked
						}
					}
						//initialize 2X2 transition matrix using values from the link
						double Aarray[2][2];
						double Aarray_P[2][2];
						Aarray[0][0] = CurLink.PAA;
						Aarray[0][1] = CurLink.PNA;
						Aarray[1][0] = 1 - CurLink.PAA;
						Aarray[1][1] = 1 - CurLink.PNA;
						//raise transition matric to the power of j-1 to calculate probability of accident in j-1 moves of the agent at that link
						pow2(Aarray, Aarray_P, j - 1);
						if (CurLink.accident == true and j!=1) {//link currently has an accident
							Paccident += Aarray_P[0][0];//add to probility of accident. this is equivalent to multiplying Array_P by a vector [1,0]
						}

						else if (j!=1) {//link currently does not have an accident, and is not the first link in path
							Paccident += Aarray_P[0][1];//add ot probability
						}

						////////////////////////////////Traffic Utility////////////////////////////////////////
						if (j == 1) {//current link is the first link in the path
							Tutility += CurLink.Traffic;
						}
						if (j != 1) {
							//initialize transition matrix with transition probabilities from link
							double Tarray[3][3];
							double Tarray_P[3][3];
							Tarray[0][0] = CurLink.THH;
							Tarray[0][1] = CurLink.THM;
							Tarray[0][2] = 1-CurLink.THH-CurLink.THM;
							Tarray[1][0] = CurLink.TMH;
							Tarray[1][1] = 1-CurLink.TMH-CurLink.TML;
							Tarray[1][2] = CurLink.TML;
							Tarray[2][0] =1- CurLink.TLL-CurLink.TLM;
							Tarray[2][1] = CurLink.TLM;
							Tarray[2][2] = CurLink.TLL;
							//raise transition matric to the power of j-1 to calculate probability of traffic values  at that link in j-1 moves of the agent
							pow3(Tarray, Tarray_P, j - 1);
							//calculate traffic utility value of link based on the current state of traffic, and the probabilities of traffic
							if (CurLink.Traffic == LowT) {//currently low traffic
								Tutility += (Tarray_P[2][0] * HighT + Tarray_P[2][1] * MedT + Tarray_P[2][2] * LowT);
							}
							else if (CurLink.Traffic == MedT) {//currently medium traffic
								Tutility += (Tarray_P[1][0] * HighT + Tarray_P[1][1] * MedT + Tarray_P[1][2] * LowT);
							}
							else {//currently high traffic
								Tutility += (Tarray_P[0][0] * HighT + Tarray_P[0][1] * MedT + Tarray_P[0][2] * LowT);
							}
						}
						
						//destination utility
					if (nodes[LendNode].destination == true) {
						tempPassengers--;//subtract from temp passengers, so decision agent knows it can pick up another passenger on path if currently full
						Dutility++;//add to destination utility count

					}
					//passenger utility
					if (nodes[LendNode].passenger == true) {//node has a passenger at it
						if (tempPassengers < 6) {//make sure decision agent will not have 6 passengers before adding another
							Putility += 1;//add to passenger count
							tempPassengers++;//add to temp passengers
						}
					}
					else {//current node does not have a passenger at it
						if (nodes[LendNode].destination == false) {//current node is not currently a destination node, and can spawn a passenger
							Putility += nodes[LendNode].passProb;//add the probability of a passenger appearing to passenger count
						}
					}

				}
				if (Debug == true) {//if in debug mode, output information on path
					cout << "\n\nPath " << i + 1 << ":" << endl;
					cout << " Route: " << endl;
					for (int k = 0; k < paths[i].P.size(); k++) {
						cout << paths[i].P[k];
						if (k != paths[i].P.size() - 1) {
							cout << "->";
						}
					}
					cout << endl;
					cout << "Route Length: " << paths[i].Length<<endl;
					cout << "Expected Traffic Utility: " <<Taxi.Tweight* Tutility<<endl;
					cout << " Expected Utility From passenger pickup: " << (1 - (Taxi.passengers / 6)) *40  * Putility<<endl;
					cout << "Expected Utility from passenger drop-offs:" << (Taxi.passengers / 6) * 80 * Dutility << endl;
					cout << "Probability of encountering an accident: " << Paccident << endl;
					cout << "Total Utility: " << ((Taxi.Distweigth* paths[i].Length) + (Taxi.Tweight* Tutility) + ((1 - (Taxi.passengers / 6)) * 40 * Putility) + ((Taxi.passengers / 6) * 80 * Dutility) + (Paccident * Taxi.Aweight)) << endl;
					if (paths[i].blocked == true) {
						cout << "Path currently blocked." << endl;
					}
				}
				//Set current Traffic, passenger and destination utilities
				paths[i].Tutility = Tutility;
				paths[i].Putility = Putility;
				paths[i].Dutility = Dutility;
				//calculate weights of picking up and dropping off passengers based on how many passengers he currently has
				Taxi.Destweight = (Taxi.passengers / 6) * 80;
				Taxi.Pweight = (1 - (Taxi.passengers / 6)) * 40;
				
				//calculate total utility of path
				paths[i].TotalUtility = (Taxi.Distweigth * paths[i].Length) + (Taxi.Tweight * paths[i].Tutility) + (Taxi.Pweight * paths[i].Putility) + (Taxi.Destweight * paths[i].Dutility)+(Paccident*Taxi.Aweight);

				if (paths[i].TotalUtility > BestUtility and paths[i].blocked !=true) {//if the total utility of path is better than the current best path, and the path is not blocked, make current path the best path
					BestPath = i;//save path index
					BestUtility = paths[i].TotalUtility;//save path utility
				}
				
			}//if in debug mode, output information wich path the agent chose
			if (Debug == true) {
				cout << "\nDecision agent decided on path " << BestPath+1 << endl;
				cout << "Press Enter to continue:" << endl;
				cin.ignore();
			}
			currentPath = paths[BestPath].P;//set current path to decided path

			Taxi.nextNode = currentPath[1];//get next node to move to


			MoveDistance = links[nodes[Taxi.currentNode].ConnectedLinks[Taxi.nextNode]].weight;//find distance to nest node
			int xchange = nodes[Taxi.nextNode].nodeCoords.first - nodes[Taxi.currentNode].nodeCoords.first;//find difference in x
			int ychange = nodes[Taxi.nextNode].nodeCoords.second - nodes[Taxi.currentNode].nodeCoords.second;//find difference in y
			movementStages = 3;
			xtran = xchange / (MoveDistance * movementStages);//define small change in x for movement
			ytran = ychange / (MoveDistance * movementStages);//define small change in y for movement
		}
			Taxi.x += xtran;//add to taxi x coords
			Taxi.y += ytran;//add to taxi y coords
		
		if (updateCount == MoveDistance* movementStages) {//taxi has reached next node
			//cin.ignore();
			Taxi.moving = false;
			updateCount = 0;

			Taxi.x = nodes[Taxi.nextNode].nodeCoords.first;//make sure taxi is on node
			Taxi.y = nodes[Taxi.nextNode].nodeCoords.second;
			Taxi.currentNode = Taxi.nextNode;//set next node to current node
			auto it = find(destinations.begin(), destinations.end(), Taxi.currentNode);//look for current node in vector of destinations
			if (it != destinations.end()){//if destination
				destinations.erase(it);//remove from destinations
				if (nodes[Taxi.currentNode].passenger == true and Taxi.passengers < 6) {//if passenger
					int PassDest = findDest();//find a destination for passenger

					Taxi.passengers++;
					nodes[PassDest].destination = true;
					bool set = false;
					if (destinations.size() == 0) 
					{ 
						destinations.push_back(PassDest); 
					} else {
						for (auto it = destinations.begin(); it != destinations.end(); it++) {
							if (nodes[*it].destination == false) {
								destinations.insert(it, PassDest);//insert new destination after other destinations, before passengers
								set = true;
								break;
							}
						}
						if (set == false) {
							destinations.push_back(PassDest);
						}
					}
					
					nodes[Taxi.currentNode].passenger = false;//remove passenger from node
				}
				if (nodes[Taxi.currentNode].destination == true) {//if destination
					Taxi.passengers--;
					nodes[Taxi.currentNode].destination = false;//remove destination
				}
			}
		}
		else {//taxi still moving
			Taxi.moving = true;
			updateCount += 1;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			
		}


	}

	
	end:

	glutPostRedisplay();//tell glut to redraw scene


}
void initNetwork() {
	//initialize connected links for each node. THis gives information on links that connect one node to another
	for (int i = 0; i < nodes.size(); i++) {
		vector<int> CL(35);
		for (int j = 0; j < CL.size(); j++) {
			CL[j] = -1;
		}
		for (int j = 0; j < links.size(); j++) {
			if (links[j].u == i) {
				CL[links[j].v] = j;
			}
		}
		nodes[i].ConnectedLinks = CL;
		for (int i = 0; i < links.size(); i++) {
			links[i].Traffic = LowT;
		}
	}
}

int main(int argc, char** argv)
{
	//user decision on debug mode
	cout << "To run the program in debug mode, which allows you to see possible routes and their utilities, enter 1, else enter 0." << endl;
	int choice;
	cin >> choice;
	if (choice == 1) {
		Debug = true;
	}
	else {
		Debug = false;
	}
	
	//initialize network. 
	initNetwork();
	//create GL context
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(1000, 723);
	glutInitWindowPosition(50, 50);
	glClearColor(1, 1, 1, 0);
	glutCreateWindow("Taxi Route Finder");
	

	loadTexture();//load background png into texture
	glutDisplayFunc(draw);//define display function
	glutIdleFunc(execute);//define idle function
	glutMainLoop();//execute glut loop
	return 0;
}

