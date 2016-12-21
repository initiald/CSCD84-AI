/*
	CSC D84 - Unit 1 - Search

	This file contains stubs for implementing the different search
	algorithms covered in the course. Please read the assignment
	handout carefully - it describes the game, the data you will
	have to handle, and the search functions you must provide.

	Once you have read the handout carefully, implement your search
	code in the sections below marked with

	**************
	*** TO DO:
	**************

	Make sure to add it to your report.txt file - it will be marked!

	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Jul. 15
*/

#include "AI_search.h"

//define a queue for BFS
typedef struct Queue{
        int MAX_Size;
        int numOfEle;
        int head;
        int tail;
        int *ele;
        int *priority;
}Queue;

Queue *initialQ(int size){
        Queue *Q;
        Q = (Queue *)malloc(sizeof(Queue));
        Q->ele = (int *)malloc(sizeof(int)*size);
        Q->priority = (int *)malloc(sizeof(int)*size);
        Q->numOfEle = 0;
        Q->MAX_Size = size;
        Q->head = 0;
        Q->tail = -1;
        return Q;
}

int Dequeue(Queue *Q, int mode){
        /* if empty*/
        int front;
        if(Q->numOfEle==0){
            //printf("Empty\n");
            return 0;
        }
        else{
			//DFS stack pop
			if (mode == 1){
				//last element
				front = Q->ele[Q->numOfEle];
				Q->numOfEle--;
				return (front);
			}
            Q->numOfEle--;
            front = Q->ele[Q->head];
            Q->head++;
            if(Q->head==Q->MAX_Size){
				Q->head=0;
			}
            return (front);
        }
}
void Enqueue(Queue *Q,int elem, int mode, int h){
        /* If the Queue is full, we cannot push an element into it as there is no space for it.*/
        if(Q->numOfEle == Q->MAX_Size){
            //printf("Full\n");
        }
        else{
        	//for A* search
        	if ((mode == 2) && (Q->numOfEle != 0)){
        		//Adjustment for the bounds
        		int adjust = 0;
        		int adjust2 = 1;

        		for (int i = 0; i<Q->numOfEle; i++){
        			int cur = Q->head + i;
        			//if it reaches a bound
        			if (cur >= Q->MAX_Size){
        				cur = adjust;
        				adjust++;
        			}
        			if (h < Q->priority[(Q->ele[cur])]){
        				for (int j = Q->numOfEle+1; j> i; j--){
        					//the remaining amount of elements  to move
        					int back = Q->tail - j;

        					//if it reaches a bound
        					if (back < 0){
        						back = Q->MAX_Size - adjust2;
        						adjust2++;
        					}
        					//special case movement of 0 and Max_Size
        					if (back == 0){
        						Q->ele[back] = Q->ele[Q->MAX_Size-1];
        					}
        					else{
        						Q->ele[back] = Q->ele[back-1];
        					}
        				}
        				//Finally enqueue the element you want
						Q->ele[i] = elem;
						Q->priority[elem] = h; //set the elem prior
						Q->numOfEle++;
						return;
        			} //end of backwards adjustment
        		}//end of searching through current node

        	} //end of A*
            Q->numOfEle++;
            Q->tail++;
            if(Q->tail == Q->MAX_Size){
            	Q->tail = 0;
            }
            Q->ele[Q->tail] = elem;
            Q->priority[elem] = h; //set the elem prior
        }
        return;
}

/*calculate the index of each neighbour*/
int indexCalc(int i,int x, int y){
	//top
    if (i == 0){
        return (x + ((y-1)*32));
    }
	//right
    if (i == 1){
        return ((x+1) + (y*32));
    }
	//bottom
    if (i == 2){
        return (x + ((y+1)*32));
    }
	//left
    if (i == 3){
        return ((x-1) + (y*32));
    }
    //if i isn't a number from 0-3
    return 0;
}

void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]))
{

 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in AI_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:
	
	The game takes place on a grid of size 32x32, the file board_layout.h specifies the size and defines two
	constants 'size_X' and 'size_Y' for the horizontal and vertical size of the board, respectively. For our
	purposes, the grid of locations is represented by a graph with one node per grid location, so given
	the 32x32 cells, the graph has 1024 nodes. 

	To create a maze, we connect cell locations in the grid in such a way that a) there is a path from any
	grid location to any other grid location (i.e. there are no disconnected subsets of nodes in the graph),
	and b) there are loops.

	Since each node represents a grid location, each node can be connected to up to 4 neighbours in the
	top, right, bottom, and left directions respectively:

			node at (i,j-1)
				^
				|
	(node at i-1, j) <- node at (i,j) -> node at (i+1, j)
				|
				v
			node at (i,j+1)

	The graph is theredore stored as an adjacency list with size 1024 x 4, with one row per node in the
	graph, and 4 columns corresponding to the weight of an edge linking the node with each of its 4
	possible neighbours in the order towp, right, bottom, left (clockwise from top).

	Since all we care is whether nodes are connected. Weights will be either 0 or 1, if the weight is
	1, then the neighbouring nodes are connected, if the weight is 0, they are not. For example, if

	graph[i][0] = 0 
	graph[i][1] = 1
	graph[i][2] = 0
	graph[i][3] = 1

	then node i is connected to the right and left neighbours, but not to top or bottom.

	The index in the graph for the node corresponding to grid location (x,y) is

	index = x + (y*size_X) 		or in this case		index = x + (y*32)

	Conversely, if you have the index and want to figure out the grid location,

	x = index % size_X		or in this case		x = index % 32
	y = index / size_Y		or in this case		y = index / 32

	(all of the above are *integer* operations!)

	A path is a sequence of (x,y) grid locations. We store it using an array of size	
	1024 x 2 (since there are 1024 locations, this is the maximum length of any
	path that visits locations only once).

	Agent locations are coordinate pairs (x,y)	

   Arguments:
		gr[graph_size][4]   - This is an adjacency list for the maze
		path[graph_size][2] - An initially empty path for your code to fill.
				      In this case, empty means all entries are initially -1
		visit_order[size_X][size_Y]	- An array in which your code will store the
						  *order* in which grid locations were 
						  visited during search. For example, while
						  doing BFS, the initial location is the
						  start location, it's visit order is 1.
						  Then the search would expand the immediate
						  neighbours of the initial node in some order,
						  these would get a visit order of 2, 3, 4, and
						  5 respectively, and so on.
					
						  This array will be used to display the search
						  pattern generated by each search method.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- BFS
					mode = 1	- DFS
					mode = 2	- A*

		(*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[10][2], int cats, int cheeses)
				- This is a pointer to one of the heuristic functions you will implement, either H_cost()
				  or H_cost_nokitty(). The driver in AI_search_core_GL will pass the appropriate pointer
				  depending on what search the user wants to run.
			
				  If the mode is 0 or 1, this pointer is NULL

				  * How to call the heuristic function from within this function : *
					- Like any other function:
						h = heuristic( x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);

   Return values:
		Your search code will directly update data passed-in as arguments:

		- path[graph_size][2]	: Your search code will update this array to contain the path from
					  the mouse to one of the cheese chunks. The order matters, so
					  path[0][:] must be the mouse's current location, path[1][:]
					  is the next move for the mouse. Each successive row will contain
					  the next move toward the cheese, and the path ends at a location
					  whose coordinates correspond to one of the cheese chunks.
					  Any entries beyond that must remain set to -1
		- visit_order[size_X][size_Y] 	:  Your search code will update this array to contain the
						   order in which each location in the grid was expanded
						   during search. This means, when that particular location
						   was checked for being a goal, and if not a goal, had its
						   neighbours added as candidates for future expansion (in
						   whatever order is dictated by the search mode).

						   Note that since there are 1024 locations, the values in 
						   this array must always be in [0, 1023]. The driver code
						   will then display search order as a yellow-colored
						   brightness map where nodes expanded earlier will look
						   brighter.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to carry out the different types of search depending on the
 *		mode.
 *
 *		You can do this by writing code within this single function (and being clever about it!)
 *		Or, you can use this function as a wrapper that calls a different search function
 *		 (BFS, DFS, A*) depending on the mode. Note that in the latter case, you will have
 *	    	  to inform your A* function somehow of what heuristic it's supposed to use.
 *
 *		Visiting Order: When adding the neighbours of a node to your list of candidates for
 *				expansion, do so in the order top, right, bottom, left.
 *
 *		NOTE: Your search functions should be smart enough to not choose a path that goes
 *		      through a cat! this is easily done without any heuristics.
 * 
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

 // Stub so that the code compiles/runs - This will be removed and replaced by your code!

 path[0][0]=mouse_loc[0][0];
 path[0][1]=mouse_loc[0][1];
 path[1][0]=mouse_loc[0][0];
 path[1][1]=mouse_loc[0][1];


/*initial the queue and counter of the visit order*/
Queue *Q = initialQ(1024);

/*nodes that have been visited already, 1 = visited
First is the visited, and second is previous, and third A* cost*/
int visited[1024][3];
for (int q = 0; q < 1024; q++){
    for (int r = 0; r < 3; r++){
        visited[q][r] = 0;    
    }
}
//counters
int neigh;
int counter = 1;
int bfsc = 0; //bfs counter also used for dfs
int current; //current node
int h; //heuristic value 

/*get mouse location*/
int x = mouse_loc[0][0];
int y = mouse_loc[0][1];

/*add it to the visit order*/
visit_order[x][y] = counter;

//mouse index
int index = (x + (y*32));
int mouse_index = (x + (y*32)); //index in the gr
    
/*Enqueue initial start*/
Enqueue(Q,mouse_index, mode);

/*loop until there is no more*/
while((Q->numOfEle) != 0){
    /*dequeue and check its neighbours are vaild locations or if its cheese*/
    current = Dequeue(Q,mode);
    x = current % 32;
    y = current / 32;
	
	//update visit order
    counter++;
    visit_order[x][y] = counter; 

    if ((cheese_loc[0][0] == x)  && (cheese_loc[0][1] == y)){
        //head to goal, do something?
        break;
    }
	
	/*for each neigh, check if its bound, not a cat and it is a movable location
	if so, then update array that it has been visited and add its previous node
	as a the current node. Then enqueue to be furthered process*/
    for (int n = 0; n <= 3; n++){
        neigh = indexCalc(n,x,y);
        if (neigh >= 0 && neigh < 1024){
            for (int d = 0; d < cats; d++){
                int cat_idx = (cat_loc[d][0] + (cat_loc[d][1]*32));
                if (cat_idx != neigh){
                    if ((gr[current][n] == 1) && (visited[neigh][0] != 1)){
						h = heuristic( x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);
                        visited[neigh][0] = 1; //been visited
                        visited[neigh][1] = current; //previous node leading
                        visited[neigh][2] = visited[current][2] + 1; //determines cost
                        //Queue, the neigh, mode and the A* cost
                        Enqueue(Q,neigh,mode, (h+ visited[neigh][2]));
                    }
                }
            }
        }
        //ignore the walls
    }
    
}

int goal = current;
//find the length 
while(current != mouse_index){
    current = visited[current][1];
    bfsc++; 
}

for (int k = bfsc; k > 0 ; k--){
    //work backwards to adjust the path for the mouse to travel
    //adjust the path
    path[k][0] = x;
    path[k][1] = y;

    x = visited[goal][1] % 32;
    y = visited[goal][1] / 32;

    //adjust the goal
    goal = visited[goal][1];
}

 return;
}


int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. Which cheese is up to you.
	Whatever you code here, your heuristic must be admissible.

	Input arguments:

		x,y - Location for which this function will compute a heuristic search cost
		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		gr - The graph's adjacency list for the maze

		These arguments are as described in the search() function above
		
		NOTE Smallest H_cost() first
 */
 
 //choose the cheese closest to the mouse's current location
 int closest = 1500; // longest diagonal distance is 1449, can't exceed
 /*x and y coords
 int cx;
 int cy;*/
 int curDistToCheese;
 
 for (int i = 0; i<cheeses; i++){
	//if it is a cat make it very large, but less than a wall and if it is the cheese make it 0
	if ((cheeses[i][0] == x) && (cheeses[i][1] == y)){
		return 0;
	}
	for (int c = 0; c<cats; c++){
		if ((cat_loc[c][0]) == x && (cat_loc[c][1] == y)){
			return 9999;
		}
	}
	 //euclidean distance from the mouse to the ith cheese.
	curDistToCheese = (int) sqrt( 
	(cheeses[i][0] - mouse_loc[0][0])*(cheeses[i][0] - mouse_loc[0][0]) +
	(cheeses[i][1] - mouse_loc[0][1])*(cheeses[i][1] - mouse_loc[0][1])
	); 
	if (curDistToCheese < closest){
		closest = curDistToCheese;
	}
 }
 //then calculate the h_cost.
 //Factors: 

 return(closest);		// <-- Evidently you will need to update this.
}

int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. 

	However - this time you want your heuristic function to help the mouse avoid being eaten.
	Therefore: You have to somehow incorporate knowledge of the cats' locations into your
	heuristic cost estimate. How well you do this will determine how well your mouse behaves
	and how good it is at escaping kitties.

	This heuristic *does not have to* be admissible.

	Input arguments have the same meaning as in the H_cost() function above.
 */

	 //choose the cheese closest to the mouse's current location
	 int closest = 999999; // longest diagonal-ish distance
	 //
	 int curDistToCheese;

	 //Calculate the distance from each cat to the mouse
	 int catToMouse[cats];
	 int catToCheese[cats];
	 int numOfCatToCheese; //number of cats to cheese
	 int TotalDist;


	 for (int i = 0; i<cheeses; i++){
		//if it is a cat make it very large, but less than a wall and if it is the cheese make it 0
		if ((cheese_loc[i][0] == x) && (cheese_loc[i][1] == y)){
			return 0;
		}
		//easy manhattan distance from the mouse to the ith cheese.
		curDistToCheese = abs(cheese_loc[i][0] - mouse_loc[0][0]) + abs(cheese_loc[i][1] - mouse_loc[0][1]);
		for (int c = 0; c<cats; c++){
			if ((cat_loc[c][0]) == x && (cat_loc[c][1] == y)){
				return 9999;
			}
			catToCheese[c] = abs(cat_loc[c][0] - cheese_loc[0][0]) + abs(cat_loc[c][1] - cheese_loc[0][1]);
			if (catToCheese[c] < curDistToCheese) {
				numOfCatToCheese++;
				TotalDist += abs(catToCheese[c] - curDistToCheese);
			}

		}

		int temp = (int)(TotalDist/numOfCatToCheese);
		if (temp < closest){
			closest = temp;
		}
		else{
			closest = curDistToCheese;
		}
		int index = (x + (y*32));
		//for the number of walls around the current position
		for (int w = 0; w < 4; w++){
			if (gr[index][w] == 0){
				closest += 77; //some arbitrary number
			}
		}
	 }
	 //then calculate the h_cost.
	 //Factors:

	 return(closest);		// <-- Evidently you will need to update this.
}

