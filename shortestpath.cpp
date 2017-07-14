/*Author: Advaith Auron Suresh*/


#include <iostream>
#include <fstream>
#include <limits.h>
#include <map>
#include <vector>
#include <string>
#include <stack>
#include <list>
#include <string.h>
#include <queue>
#include <utility>
#include <cstdlib>

using namespace std;

/*Global Variables*/
int INFINITY = INT_MAX;
map<string, bool> GloMap;
//Vertex Class Declaration
class Vertex;

//Type Definitions
typedef map<string,Vertex *> vmap;
typedef pair<string,Vertex *> vpair;

//Edge Class Definition
class Edge {
public:
    string name;
    Vertex * head;  // tail ------> head
    double length;  // Edge Length
    bool up;        //Edge Status
    Edge() {}
    Edge(string name, Vertex * head, double length, bool up)
    {
        this->name = name;
        this->head = head;
        this->length = length;
        this->up = up;
    }
};

//Vertex Class Definition
class Vertex {
public:
    string name;                // Vertex name
    map<string, Edge*> adj;     // Adjacent vertices
    bool up;
    float dist;                 //Cost
    Vertex* parent;             //Parent Vertex
    Vertex(){}
    Vertex( const string & nm ) : name( nm )
      { reset( ); }

    void reset( )
    { 
        dist = INFINITY;
        up = true;
    }
};

/*Priority Queue Implementation Class*/
class MinHeap
{
public: 
    vector <Vertex*> pq;
    /*Min Heapify*/
    void BubbleDown(int index, int n );
    MinHeap();
    void Heapify();
    Vertex* GetMin();
    void DeleteMin();
    void addNodes(vector <Vertex*> vector);   
};

/*Performs min-heapify operation*/
void MinHeap::BubbleDown(int index, int n)
{
    int smallest = 0;
    int leftChildIndex = (2*index) + 1;
    int rightChildIndex = (2*index) + 2;
    Vertex* CurrentVertex = this->pq[index];
    
    if(leftChildIndex <= n )
    {
        Vertex* leftNode = this->pq[leftChildIndex];
        if (leftNode->dist < CurrentVertex->dist)
        {
            smallest = leftChildIndex;
        }
        else
            smallest = index;
    }
    
    if(rightChildIndex <= n )
    {
        Vertex * smallNode = this->pq[smallest];
        Vertex * rightNode = this->pq[rightChildIndex];
        if(rightNode->dist < smallNode->dist)
        {
            smallest = rightChildIndex;
        }       
    }
    /*Swapping operation*/
    if(smallest != index)
    {
        Vertex* temp = this->pq[index];
        this->pq[index] = this->pq[smallest];
        this->pq[smallest] = temp;
        if(index == 0)
            return;
        smallest = index - 1;
        BubbleDown(smallest, n);
    }
}

/*Performs the build-heap operation*/
void MinHeap::Heapify()
{
    int length = this->pq.size();
    for(int i = (length/2)-1; i>=0; --i )
    {
        /*Internally calls min-heap operation*/
        BubbleDown(i, length-1);
    }
}

/*Copying Vertex Node information to the priority queue*/
void MinHeap::addNodes(vector <Vertex*> MapVertices)
{
    this->pq.reserve(MapVertices.size());
    copy(MapVertices.begin(), MapVertices.end(), back_inserter(this->pq));
    Heapify();
}


MinHeap::MinHeap(){}

/*Extract Min Operation*/
Vertex* MinHeap::GetMin()
{
    return this->pq[0];
}

/*Deletes the minimum value and performs a build-heap operation*/
void MinHeap::DeleteMin()
{
    int length = this->pq.size();
    if(length == 0)
        return;
    this->pq[0] = this->pq[length-1];
    this->pq.pop_back();
    Heapify();
}

//Graph Class Definition
class Graph
{
  public:
    Graph( ) { }
    ~Graph( );
    void addEdge( const string & sourceName, const string & destName, double length, bool bidirectional );
    void vertexUp (const string &vertexName);
    void vertexDown (const string &vertexName);
    void edgeUp (const string & sourceName, const string & destName);
    void edgeDown (const string & sourceName, const string & destName);
    void deleteEdge (const string & sourceName, const string & destName);
    void reachable();
    void depthFirstSearch(map<string,bool> &visited, Vertex *vertex, bool sourceVertex);
    void shortestPath(const string & sourceName, const string & destName);
    void printPath(Vertex * SourceVertex, Vertex * DestVertex);
    void print();
      
  private:
    Vertex * getVertex( const string & vertexName );
    vmap vertexMap;
    vector<Vertex *> allVertices;
};

/*Member function used to print path from Source Vertex to Destination Vertex.
 *Recursively calls itself until Source Vertex is reached */
void Graph::printPath(Vertex * SourceVertex, Vertex * DestVertex)
{
    /*Source Vertex has been reached*/
    if(SourceVertex->name.compare(DestVertex->name) == 0)
    {
        cout<<SourceVertex->name<<"\t";
        return;
    }
    /*No parent Vertex*/
    else if(DestVertex->parent == NULL)
    {
        cout<<DestVertex->name;
        return;
    }
    else
    {
    /*Recursively calls itself where destination is replaced with it's parent each time*/
        printPath(SourceVertex, DestVertex->parent);
        cout<<DestVertex->name<<"\t";
    }
}

/*Member function to determine shortest path between two given vertices*/
void Graph::shortestPath(const string & sourceName, const string & destName)
{
    Vertex * SourceVertex = getVertex( sourceName );
    Vertex * DestVertex = getVertex( destName );
    Edge *edge;
    MinHeap obj;
    Vertex* adjVert, *currVer;
    float edgeWeight;
    vector <Vertex*> mapVertices;
    
    map<string, Edge*>::iterator iterate;
    vmap::iterator it;
    /*Initializes the map setting distance to source as INT_MAX and parent as 0*/
    for(it = vertexMap.begin(); it!= vertexMap.end(); ++it)
    {
        if(it->second->up)
        {
            if((it->second->name).compare(SourceVertex->name) == 0 )
            {
                it->second->dist = 0;
                it->second->parent = NULL;
            }
            else
            {
                it->second->dist = INFINITY;
                it->second->parent = NULL;
            }
            mapVertices.push_back(it->second);
        }
    }  
    obj.addNodes(mapVertices);
    /*Source Vertex is Down*/
    if(!(SourceVertex->up))
    {
        cout<<"\nSource Vertex is Down";
        return;
    }
    /*Destination Vertex is Down*/
    if(!(DestVertex->up))
    {
        cout<<"\nDestination Vertex is Down";
        return;
    }
    /*Process the loop for all the vertices*/
    for(int no = 0; no < allVertices.size(); ++no)
    {
        /*Obtains the minimum value from the priority queue*/
        currVer = obj.GetMin();
        /*Decrease Key operation*/
        obj.DeleteMin();
        for(iterate = currVer->adj.begin(); iterate != currVer->adj.end(); iterate++)
        {
            edge = iterate->second;
            if(edge->up)
            {
                adjVert = edge->head;
                if(adjVert->up)
                {
                    edgeWeight = edge->length;
                    if((currVer->dist)!= INFINITY && (adjVert->dist) > (currVer->dist) + edgeWeight)
                    {
                        adjVert->dist = currVer->dist + edgeWeight;
                        adjVert->parent = currVer;
                    }
                }
            }
        }
        /*Calling build heap to restore order to the priority queue*/
        obj.Heapify();
    }
    printPath(SourceVertex, DestVertex);
    cout<<DestVertex->dist;
    
}

/*This member function performs DFS on a given vertex*/
void Graph::depthFirstSearch(map<string,bool> &visited, Vertex *Currvertex, bool sourceVertex)
{
    visited.insert(make_pair(Currvertex->name,true) );
    if(sourceVertex)
    {
        cout<<"\n"<<Currvertex->name;
    }
    map<string, Edge*>::iterator itr;
    Vertex *adjVertex;
    Edge *edge;
    for(itr = Currvertex->adj.begin(); itr!= Currvertex->adj.end(); ++itr)
    {
        edge = itr->second;
        /*Process if status of edge is UP*/
        if(edge->up)
        {
            adjVertex = edge->head;
            /*Process if status of vertex is UP*/
            if(adjVertex->up)
            {
                map<string, bool>::iterator it = visited.find(adjVertex->name);
                if(it == visited.end())
                {
                    GloMap.insert(make_pair(adjVertex->name,sourceVertex));
                    depthFirstSearch(visited, adjVertex, false);
                }
            }
        }
    }   
}

/*This member function gives a list of reachable vertices from each vertex*/
void Graph::reachable()
{
    vmap::iterator itr;
    Vertex *CurrVertex;
    for(itr = vertexMap.begin(); itr!= vertexMap.end(); ++itr)
    {
        GloMap.clear();
        /*A map of each vertex and a boolean value indicating visit status*/
        map <string, bool> visited;
        CurrVertex = itr->second;
        /*Perform DFS if the status of the current vertex is up*/
        if(CurrVertex->up)
        {
            /*A true boolean value indicates that this is the source vertex*/
            depthFirstSearch(visited, CurrVertex, true );
            for(map<string,bool>::iterator iter = GloMap.begin(); iter!=GloMap.end(); iter++)
            {
                cout<<"\n\t"<<iter->first;   
            }
        }
    }
    
}

/*Changes the status of a vertex to UP*/
void Graph::vertexUp(const string & vertexName)
{
    Vertex * vertex = getVertex(vertexName);
    if(vertex != NULL)
        vertex->up = true;
}

/*Changes the status of a vertex to DOWN*/
void Graph::vertexDown(const string & vertexName)
{
    Vertex * vertex = getVertex(vertexName);
    if(vertex != NULL)
        vertex->up = false;
}

/*Changes the status of a edge to UP*/
void Graph::edgeUp(const string & sourceName, const string & destName)
{
    Edge *edge;
    std::map <string, Edge*>::iterator it;
    Vertex * SourceVertex = getVertex( sourceName );
    it = SourceVertex->adj.find(destName);
    if(it != SourceVertex->adj.end())
    {
        edge = it->second;
        edge->up = true;
    }
    
}

/*Changes the status of a edge to DOWN*/
void Graph::edgeDown (const string & sourceName, const string & destName)
{
    Edge *edge;
    std::map <string, Edge*>::iterator it;
    Vertex * SourceVertex = getVertex( sourceName );
    it = SourceVertex->adj.find(destName);
    if(it != SourceVertex->adj.end())
    {
        edge = it->second;
        edge->up = false;
    }  
}

/*Deletes the edge between the two given vertices*/
void Graph::deleteEdge (const string & sourceName, const string & destName)
{
    std::map <string, Edge*>::iterator it;
    Vertex * SourceVertex = getVertex( sourceName );
    it = SourceVertex->adj.find(destName);
    if(it != SourceVertex->adj.end())
    {
        SourceVertex->adj.erase(it);
    }
    
}

/*Adds edges between the given vertices, if the bidirectional flag is set,
 the edges are added in both the directions*/
void Graph::addEdge( const string & sourceName, const string & destName, double length, bool bidirectional )
{
    Edge *edge;
    std::map <string, Edge*>::iterator it;
    Vertex * SourceVertex = getVertex( sourceName );
    Vertex * DestVertex = getVertex( destName );
    it = SourceVertex->adj.find(destName);
    if(it == SourceVertex->adj.end())
    {
        edge = new Edge(destName, DestVertex, length, true);
    }
    else
    {
        edge = it->second;
        edge->length = length;
    }
    SourceVertex->adj.insert(make_pair(edge->name, edge));
    if(bidirectional)
        addEdge(destName, sourceName, length, false);
}

/*If vertexName is not present, add it to vertexMap
  In either case, return the Vertex*/
Vertex * Graph::getVertex( const string & vertexName )
{
    vmap::iterator itr = vertexMap.find( vertexName );

    if( itr == vertexMap.end( ) )
    {
        Vertex *newv = new Vertex( vertexName );
        allVertices.push_back( newv );
        vertexMap.insert( vpair( vertexName, newv ) );
        return newv;
    }
    return (*itr).second;
}

Graph::~Graph( )
{
    for( int i = 0; i < allVertices.size( ); i++ )
        delete allVertices[ i ];
}

/*Prints all the vertices, their adjacent vertices and their edge weights*/
void Graph::print()
{
    Vertex * vertex = NULL;
    Vertex * adjacentVertex = NULL;
    Edge *edge = NULL;
    vmap::iterator itr;
    std::map <string, Edge*>::iterator it;
    for(itr = vertexMap.begin(); itr!=vertexMap.end(); itr++)
    {
        vertex = itr->second;
        cout<<"\n"<<vertex->name;
        if(!vertex->up)
        {
            cout<<" DOWN";
        }
        for(it = vertex->adj.begin(); it!=vertex->adj.end(); it++)
        {
            edge = it->second;
            adjacentVertex = edge->head;
            cout<<"\n";
            cout<<"\t"<<adjacentVertex->name<<" "<<edge->length;
            if(!(edge->up))
            {
                cout<<" DOWN";
            }
            
        }
    }
    
}


/**
 * Process a request; return false if end of file.
 */
bool processRequest( istream & in, Graph & graph )
{
    string startName;
    string destName;
    double length;
    string option;
    in >> option;
    if((strcmp(option.c_str(),"print")== 0))
    {
        graph.print();
        cout<<"\n\n";
        
    }
    else if((strcmp(option.c_str(),"addedge")== 0))
    {
        in >> startName >> destName >> length;
        graph.addEdge(startName, destName, length, false);
    }
    else if((strcmp(option.c_str(),"deleteedge")== 0))
    {
        in >> startName >> destName;
        graph.deleteEdge(startName, destName);
    }
    else if((strcmp(option.c_str(),"edgedown")== 0))
    {
        in >> startName >> destName;
        graph.edgeDown(startName, destName);
    }
    else if((strcmp(option.c_str(),"edgeup")== 0))
    {
        in >> startName >> destName;
        graph.edgeUp(startName, destName);
    }
    else if((strcmp(option.c_str(),"vertexdown")== 0))
    {
        in >> startName;
        graph.vertexDown(startName);
    }
    else if((strcmp(option.c_str(),"vertexup")== 0))
    {
        in >> startName;
        graph.vertexUp(startName);
    }
    else if((strcmp(option.c_str(),"reachable")== 0))
    {
        graph.reachable();
        cout<<"\n\n";
    }
    else if((strcmp(option.c_str(),"path")== 0))
    {
        in >> startName >> destName;
        graph.shortestPath(startName, destName);
        cout<<"\n";
    }
    else if((strcmp(option.c_str(),"quit")== 0))
    {
        exit(0);
    }
    return true;
}


/**
 * A simple main that reads the file given by argv[1]
 * and then calls processRequest to compute shortest paths.
 * Skimpy error checking in order to concentrate on the basics.
 */
int main( int argc, char *argv[ ] )
{
    //Graph object
    Graph g;
    /*Expected number of input parameters*/
    if( argc < 2 )
    {
        cerr <<"\nMore Parameters expected";
        return 1;
    }
    ifstream inFile( argv[ 1 ] );
    if( !inFile )
    {
        cerr << "Cannot open " << argv[ 1 ] << endl;
        return 1;
    }

    string source, dest;
    float transit_time;
        // Read the words; add them to wordMap
    while( inFile >> source >> dest >> transit_time )
    {
        g.addEdge( source, dest, transit_time, true );
    }
    inFile.close();
    while(processRequest( cin, g) );
    return 0;
}

