//Inusah Seidu
//Project 4/Final Project
//dijkstra.cpp
//works for ginfile1-ginfile4(input: 1)

#include "queue.h"
#include "graph.h"
#include "HashTable.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <climits>
#include <iomanip>
#include <stack>
#include <set>
using namespace std;

struct VertexType
{
	string name;
	bool marked;
	int distance;
	string previous;	
};

bool checkFile(int argc, char* argv[]);
void Dijkstra(Graph<string> &dijkstra_graph, int number_of_vertices, VertexType my_vertices[]); //performs Dijkstra's algorithm
bool nodesMarked(int number_of_vertices, VertexType my_vertices[]);
int findIndex(VertexType my_vertices[], string node);
void buildDijkstraGraph(Graph<string> &dijkstra_graph, string filename, string nodes[], int &number_of_vertices, int &number_of_edges, VertexType my_vertices[]); //Builds the graph from the external file.
void printMenu(string nodes[], int number_of_vertices);
void printNodes(string nodes[], int number_of_vertices);
void insertionSort(string nodes[], int number_of_vertices);
string setStartingVertex(string start, int number_of_vertices, VertexType my_vertices[]);
int findMin(VertexType my_vertices[], int number_of_vertices);
void printRow(int location, VertexType my_vertices[]);

int main(int argc, char* argv[])//command line arguments
{
  if(!checkFile(argc,argv))
  {  
    return 1;
  }
  //input file
  string filename = argv[1];
  Graph<string> dijkstra_graph; 
  int number_of_vertices, number_of_edges;
  VertexType my_vertices [50];
  string nodes[50];
  buildDijkstraGraph(dijkstra_graph, filename, nodes, number_of_vertices, number_of_edges, my_vertices);
  printMenu(nodes, number_of_vertices);
  Dijkstra(dijkstra_graph, number_of_vertices, my_vertices);
  return 0;
}

//returns t/f if external file exists
bool checkFile(int argc, char* argv[])
{
  if(argc > 1)
  {
    ifstream my_file;
    my_file.open(argv[1]);
    if(my_file.fail())
    {
      cout << "ERROR: File does not exist" << endl;
      return false;
    } 
    else 
    {
      my_file.close();
      return true;
    }
  } 
  else 
  {
    cout << "File name was not given." << endl;
    return false;
  }
}

//Dijkstra's algorithm:
void Dijkstra(Graph<string> &dijkstra_graph, int number_of_vertices, VertexType my_vertices[])
{
  Queue<string> adjacent_queue(50);
  string start = "";
  start = setStartingVertex(start,number_of_vertices,my_vertices);
  int startIndex = findIndex(my_vertices,start);
  my_vertices[startIndex].previous = "N/A";
  my_vertices[startIndex].marked = true;
  dijkstra_graph.MarkVertex(my_vertices[startIndex].name);
  my_vertices[startIndex].distance = 0;
  int currentIndex = startIndex;
  printRow(currentIndex,my_vertices);
  
  while(!nodesMarked(number_of_vertices,my_vertices))
  {
    dijkstra_graph.GetToVertices(my_vertices[currentIndex].name,adjacent_queue);
    while(!adjacent_queue.isEmpty())
    {
      int adjacentIndex = findIndex(my_vertices,adjacent_queue.getFront());
      if(my_vertices[adjacentIndex].marked == false)
      {
        int localDistance = dijkstra_graph.WeightIs(my_vertices[currentIndex].name,my_vertices[adjacentIndex].name);
        if( (localDistance + my_vertices[currentIndex].distance) < my_vertices[adjacentIndex].distance)
        {
          my_vertices[adjacentIndex].previous = my_vertices[currentIndex].name;
          my_vertices[adjacentIndex].distance = localDistance + my_vertices[currentIndex].distance;
        }
      }
      adjacent_queue.dequeue();
    }
    currentIndex = findMin(my_vertices,number_of_vertices);
    dijkstra_graph.MarkVertex(my_vertices[currentIndex].name);
    my_vertices[currentIndex].marked = true;
  
    printRow(currentIndex,my_vertices);
  }
  cout << "\t--------------------------------------------------" << endl;
}

bool nodesMarked(int number_of_vertices, VertexType my_vertices[])
{
  for(int i = 0; i < number_of_vertices; i++)
  {
    if(my_vertices[i].marked == false)
    {
      return false;
    }
  }
  return true;
}

int findIndex(VertexType my_vertices[], string node)
{
  int index = 0;
  while(my_vertices[index].name != node)
  {
    index++;
  }
  return index;
}

void buildDijkstraGraph(Graph<string> &dijkstra_graph, string filename, string nodes[], int &number_of_vertices, int &number_of_edges, VertexType my_vertices[])
{
  ifstream my_file;
  my_file.open(filename.c_str());
  string line,origin,destination,trip_length;
  int distance,token1,token2;
  string not_found = "ZZZ";
  HashTable<string> node_name(not_found, 50);
  
  while(getline(my_file,line))
  {  
    token1 = line.find(";");
    origin = line.substr(0,token1);
    token2 = line.find(";",token1+1);
    destination = line.substr(origin.length()+1,token2-token1-1);
    trip_length = line.substr(token2+1,line.length()-token2);
    distance = atoi(trip_length.c_str());

    if(node_name.find(origin) != origin)
    {
      my_vertices[number_of_vertices].name = origin;
      my_vertices[number_of_vertices].previous = "";
      my_vertices[number_of_vertices].marked = false;
      my_vertices[number_of_vertices].distance = INT_MAX;
      nodes[number_of_vertices] = origin;
      node_name.insert(origin);
      dijkstra_graph.AddVertex(my_vertices[number_of_vertices].name);
      number_of_vertices++;
    }

    if(node_name.find(destination) != destination)
    {
      my_vertices[number_of_vertices].name = destination;
      my_vertices[number_of_vertices].previous = "";
      my_vertices[number_of_vertices].marked = false;
      my_vertices[number_of_vertices].distance = INT_MAX;
      nodes[number_of_vertices] = destination;
      node_name.insert(destination);
      dijkstra_graph.AddVertex(my_vertices[number_of_vertices].name);
      number_of_vertices++;
    }

    int origin_index = findIndex(my_vertices,origin);
    int destination_index = findIndex(my_vertices,destination);
    dijkstra_graph.AddEdge(my_vertices[origin_index].name,my_vertices[destination_index].name,distance);
  }
}

//prints menu
void printMenu(string nodes[], int number_of_vertices)
{
  system("clear");   
  cout << "\t^^^^^^^^^^^^^^^^^^DIJKSTRA'S ALGORITHM^^^^^^^^^^^^^^^^^^\n" << endl;
  cout << "\tA weighted graph has been built for these " << number_of_vertices << " destinations: \n" << endl;
  printNodes(nodes,number_of_vertices);
}

// Prints cities/destinations
void printNodes(string nodes[], int number_of_vertices)
{
  insertionSort(nodes,number_of_vertices);
  for(int i = 0; i < number_of_vertices; i++)
  {
    cout << setw(18) << nodes[i];
    if( ((i + 1) % 3) == 0){
      cout << "\n";
    }
    if(i + 1 == number_of_vertices)
    {
      cout << "\n" << endl;
    }
  }
}

// Sorts the destinations in alphabetical order using insertion sort
void insertionSort(string nodes[], int number_of_vertices)
{
  string key;
  int j;
  for(int i = 1; i < number_of_vertices; i++)
  {
    key = nodes[i];
    j = i - 1;
    while(j >= 0 && nodes[j] > key)
    {
      nodes[j+1] = nodes[j];
      j--;
    }
    nodes[j+1] = key;
  }
}

//Sets starting value in the algorithm
string setStartingVertex(string start, int number_of_vertices, VertexType my_vertices[])
{
	string input;bool inVertex = false;
    cout << " \tPlease input your starting vertex: ";
	getline(cin,input);
	for (int i = 0;	i < number_of_vertices; i++)
	{
	    if (my_vertices[i].name.compare(input) == 0)
		{
			start = input;
            cout << "\t--------------------------------------------------" << endl;
            cout << "            " << "Vertex" << "            " << "Distance" << "          " << "Previous" << "\n" << endl;
			return start;
		}
	}
	while (!inVertex)
	{
		cout << " \tStarting location does not exist..." << endl;
		cout << " \tPlease input your new vertex: ";
        cin >> input;
		for (int i = 0; i < number_of_vertices; i++)
        {
            if (my_vertices[i].name.compare(input) == 0)
			{
                start = input;
                return start;
        	}
        }
	}
    return start; 
}

//Finds the minimum unvisited values
int findMin(VertexType my_vertices[], int number_of_vertices)
{
 	int minWeight = INT_MAX - 1;
    int minThing = -1;
    for (int i = 0; i < number_of_vertices; i++)
    {                    
        if(my_vertices[i].distance < minWeight && !my_vertices[i].marked)
        {                        
            minWeight = my_vertices[i].distance;
            minThing = i;
        }
		else if (my_vertices[i].distance == minWeight && !my_vertices[i].marked)
        {
            minWeight = my_vertices[i].distance;
            minThing = i;
        }

	}        
		return minThing;
}

//Summary table
void printRow(int location, VertexType my_vertices[])
{
	if (my_vertices[location].distance != -1)
    {
	    cout << setw(18) << my_vertices[location].name << setw(18) << my_vertices[location].distance << setw(18) << my_vertices[location].previous << endl;
    }
	else
    {
	    cout << setw(14) << my_vertices[location].name << setw(18) << "Not On Path" << setw(16) << my_vertices[location].previous << endl;
    }
}