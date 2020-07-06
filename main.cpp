#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>


using namespace std;


///  Global Variables and constants

//Initialize empty vector of open set
vector<int> open_set;

//Initialize empty vector of closed set
vector<int> closed_set ;


/// Classes
class Graph
{
    public:
        //Class Functions
        void randomly_generate_graph(int size, int density);
        int prob(void);
        void print_graph(int size);
        int get_graph_xy(int x, int y);

        

    private:
       //2-D array of int to represent values of undirected paths
       int **graph;

};


///  Function Prototypes
int find_shortest_path(int size, Graph graph);
void update_open_set(int chosen_node, int size, Graph graph);
int select_next_node(int size);


/// Main Entry point
int main()
{
    //Declare a graph object of Class graph
    Graph graph;

    //Declarations needed for generating the graph
    int size, path;
    int density;

    //Prompts the user to Input the number of size
    cout << "Please Enter the size: ";
	cin >> size;

    //Prompts the user to Input the number of size
    cout << "Please Enter the density of graph: ";
	cin >> density;

    //Generate a graph randomly, based on the size and density provided by the user
    graph.randomly_generate_graph(size,density);

     //Print the generated graph to the user
    graph.print_graph(size);

    //Initialize all elements in open set to zero
    open_set.assign(size,0) ;

    //Find the shortest path using Dijkstra’s algorithm
    if( (path = find_shortest_path(size,graph)) == 0)
    {
        cout << "There is no path from Source to Destination " << endl;
    }
    else
    {
        cout << "The shortest path from Source to Destination is :  " << path << endl;
    }


    return 0;
}


///Functions Implementation
void Graph::randomly_generate_graph(int size, int density)
{
    // Use current time as seed for random generator
    srand(time(0));

    //Create the 2D graph in heap, and Set the size of the graph to be (size X size) matrix
    graph = new int*[size];

    for(int i=0; i<size; i++)
    {
        graph[i] = new int[size];
    }

    //Place edges according to density
    for(int i =0; i<size; i++)
    {
        for(int j = i; j<size; j++)
        {
            if(i==j)
            {
                graph[i][j] = 0;    //Place zeros across the main diagonal
            }
            else
            {
                if(prob() < density)
                {
                    graph[i][j] = graph[j][i] = (prob() % 10);    //Places a number between 0-10
                }
                else
                {
                    graph[i][j] = graph[j][i] = 0;
                }
            }
        }
    }
}


int Graph::prob(void)
{
    //returns a random number between 0-100
    return (rand() % 100) ;
}


void Graph::print_graph(int size)
{
    cout << "The generated graph is : \n"  << endl;
    for(int i = 0; i<size; i++)
    {
        for(int j = 0; j<size; j++)
        {
            cout << graph[i][j] << " " ;
        }
        cout << "\n" << endl;
    }
}


int Graph::get_graph_xy(int x, int y)
{
    return (graph[x][y]);
}


int find_shortest_path(int size, Graph graph)
{
    /// Dijkstra Shortest Path Algorithm
    //initialize with s = 0
    int chosen_node = 0;

    //iterate until we have visited all size
    while( closed_set.size() != size )
    {
       //Because this node was visited
       closed_set.push_back(chosen_node);

       //Look for least cost path and add it to the open set
       update_open_set(chosen_node,size,graph);

       //choose the next node, if the returned node is zero, then there is no left size in the open set
       if( (chosen_node = select_next_node(size)) == 0 )
       {
          //This means that the graph is not connected
          break;
       }

    }

     //return the shortest path from source to destination if found
     return open_set[size-1] ;

}


void update_open_set(int chosen_node, int size, Graph graph)
{
     //Check the chosen node for shorter paths
        for(int i=0, closed =0; i<size; i++)
        {
            //check for path
            if( graph.get_graph_xy(chosen_node,i) != 0)
            {
                //Check the other node isn't in the closed set
                for(int j =0; j< closed_set.size(); j++)
                {
                    if( (i == closed_set[j])  )
                    {
                        closed = 1;
                        break;
                    }
                }

                if(closed == 1)
                {
                    closed = 0;
                    continue;
                }

                //Check whether there is no path so far
                if(open_set[i] == false)
                {
                    //Update the open set of the searched node with the total cost of path
                    open_set[i] = open_set[chosen_node] + graph.get_graph_xy(chosen_node,i);
                }
                else
                {
                   //compare which path is shorter
                   if( (open_set[chosen_node] + graph.get_graph_xy(chosen_node,i)) < open_set[i])
                    {
                        //update the open set with the found shorter path
                        open_set[i] = open_set[chosen_node] + graph.get_graph_xy(chosen_node,i);
                    }
                }
            }
        }
}


int select_next_node(int size)
{
    //declare variable to hold the chosen node and initialize it to zero to check if there is a connected path
    int chosen_node = 0;

    for(int i=0 , closed = 0; i<size; i++)
        {
            //Find the first unvisited node
            for(int j =0; j< closed_set.size(); j++)
            {
                if( (i == closed_set[j]) || (open_set[i] == 0) )
                {
                    closed = 1;
                    break;
                }
            }

            if(closed == 1)
            {
                closed = 0;
                continue;
            }

            //start with this node and find if there's less path cost
            chosen_node = i;
            int least = open_set[i++];

            //Find the least cost and update the open set if found
            for(int j = i ; j<size; j++ )
            {
                if( (open_set[j] < least) && (open_set[j] != 0) )
                {
                    //Reset closed flag
                    closed = 0;

                    //Check this node is not in the closed set
                    for(int k =0; k< closed_set.size(); k++)
                    {
                        if(j == closed_set[k])
                        {
                            //This node was found in the closed set
                            closed = 1;
                            break;
                        }
                    }

                    if(closed == 0)
                    {
                        //this code executes if this node isn't in the closed set
                        least = open_set[j];
                        chosen_node = j;
                    }
                }
            }

            break;
        }

    return chosen_node;
}


