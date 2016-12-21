/*
	CSC D84 - Unit 2 - MiniMax search and adversarial games

	This file contains stubs for implementing a MiniMax search
        procedure with alpha-beta pruning. Please read the assignment
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

	Starter code: F.J.E. Sep. 15
*/
//index calculator
int indexCalc(int i,int x, int y){
    if (i == 0){
        return (x + ((y-1)*32));
    }
    if (i == 1){
        return ((x+1) + (y*32));
    }
    if (i == 2){
        return (x + ((y+1)*32));
    }

    if (i == 3){
        return ((x-1) + (y*32));
    }
    //if i isn't a number from 0-3
    return 0;
}


#include "MiniMax_search.h"

double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta)
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in MiniMax_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:

	Exactly the same as for Assignment 1 - have a look at your code if you need a reminder of how the adjacency
	list and agent positions are stored.	

	Note that in this case, the path will contain a single move - at the top level, this function will provide
	an agent with the 'optimal' mini-max move given the game state.


   IMPORTANT NOTE: Mini-max is a recursive procedure. This function will need to fill-in the mini-max values for 
	 	   all game states down to the maximum search depth specified by the user. In order to do that,
		   the function needs to be called with the correct state at each specific node in the mini-max
		   search tree.

		   The game state is composed of:

			* Mouse, cat, and cheese positions (and number of cats and cheeses)
			
		   At the top level (when this function is called by the mini-max driver code), the game state
		   correspond to the current situation of the game. But once you start recursively calling
		   this function for lower levels of the search tree the positions of agents will have changed.
		   
		   Therefore, you will need to define local variables to keep the game state at each node of the
		   mini-max search tree, and you will need to update this state when calling recursively so that
		   the search does the right thing.

		   This function *must check* whether:
			* A candidate move results in a terminal configuration (cat eats mouse, mouse eats cheese)
			  at which point it calls the utility function to get a value
	 		* Maximum search depth has been reached (depth==maxDepth), at which point it will also call
			  the utility function to get a value
			* Otherwise, call recursively using the candidate configuration to find out what happens
			  deeper into the mini-max tree.

   Arguments:
		gr[graph_size][4]   		- This is an adjacency list for the maze
		path[1][2] 			- Your function will return the optimal mini-max move in this array.
		minmax_cost[size_X][size_Y]	- An array in which your code will store the
						  minimax value for maze locations expanded by
						  the search *when called for the mouse, not
						  for the cats!*

						  This array will be used to provide a visual 
						  display of minimax values during the game.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- No alpha-beta pruning
					mode = 1	- Alpha-beta pruning

		(*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);
				- This is a pointer to the utility function which returns a value for a specific game configuration

				   NOTE: Unlike the search assignment, this utility function also gets access to the graph so you can do any processing 					 that requires knowledge of the maze for computing the utility values.

				  * How to call the utility function from within this function : *
					- Like any other function:
						u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
						
		agentId: Identifies which agent we are doing MiniMax for. agentId=0 for the mouse, agentId in [1, cats] for cats. Notice that recursive calls
                         to this function should increase the agentId to reflect the fact that the next level down corresponds to the next agent! For a game
                         with two cats and a mouse, the agentIds for the recursion should look like 0, 1, 2, 0, 1, 2, ...
	
		depth: Current search depth - whether this is a MIN or a MAX node depends both on depth and agentId.
		
		maxDepth: maximum desired search depth - once reached, your code should somehow return
			  a minimax utility value for this location.

		alpha. beta: alpha and beta values passed from the parent node to constrain search at this
			     level.

   Return values:
		Your search code will directly update data passed-in as arguments:
		
		- Mini-Max value	: Notice this function returns a double precision number. This is
					  the minimax value at this level of the tree. It will be used 
					  as the recursion backtracks filling-in the mini-max values back
					  from the leaves to the root of the search tree. 

		- path[1][2]		: Your MiniMax function will return the location for the agent's 
					  next location (i.e. the optimal move for the agent). 
		- minmax_cost[size_X][size_Y] 	:  Your search code will update this array to contain the
						   minimax value for locations that were expanded during
						   the search. This must be done *only* for the mouse.

						   Values in this array will be in the range returned by
						   your utility function.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to perform a MiniMax search. This will involve a limited-depth BFS-like
 *              expansion. Once nodes below return values, your function will propagate minimax utilities
 *		as per the minimax algorithm.
 *	
 *		Note that if alpha-beta pruning is specified, you must keep track of alphas and betas
 *		along the path.
 *
 *		You can use helper functions if it seems reasonable. Add them to the MiniMax_search.h
 *		file and explain in your code why they are needed and how they are used.
 *
 *		Recursion should appear somewhere.
 *
 *		MiniMax cost: If the agentId=0 (Mouse), then once you have a MiniMax value for a location
 *		in the maze, you must update minmax_cost[][] for that location.
 *
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

 // Stub so that the code compiles/runs - This will be removed and replaced by your code!

 /*path[0][0]=mouse_loc[0][0];
 path[0][1]=mouse_loc[0][1];*/

//local variables to keep current state of current tree traversed

//check for terminal node
int term = checkForTerminal(mouse_loc,cat_loc,cheese_loc,cats,cheeses);
int neigh = 0;
/*get mouse location*/
int x = mouse_loc[0][0];
int y = mouse_loc[0][1];
int index = (x + (y*32));

//next agent to keep proper order of rec calls
int nextAgent = agentId;

//who's turn is it now
int turn = 0;

//mouse simulated
int temp[1][2];
temp[0][1] = 0;
temp[0][0] = 0;

//Cat simulated location
/*since there could be more than one cat, we have to take in account of all
current cat locations*/
int temp_cat[10][2];
for (int q = 0; q < 10; q++){
    for (int r = 0; r < 2; r++){
        temp_cat[q][r] = cat_loc[q][r];    
    }
}

//set default values for alpha and beta if not
if ((mode == 1) && (depth == 0) ){
    alpha = -9999999;
    beta = 9999999;
}

if ((depth == maxDepth) || (term == 1)){
    double u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
    if (mode == 1){
        //determine who turn it is by the depth
        //even number = mouse and odd = cat
        turn = depth % 2;
        if (turn == 0){
            //update alpha
            if (u > alpha){
                alpha = u;
            }
        }
        else{
            if (u < beta){
                beta = u;
            }
        } //end turn
        
    } //end mode = 1
    minmax_cost[x][y] = u;
    return u;
}
else{
    //a mouse turn, max turn
    if (agentId == 0){
        double max = alpha;
        double cur;
        //for each neighbour of current location
        for (int i = 0; i <= 3; i++){
            //simulate the mouse_loc for moving            
            neigh = indexCalc(i,x,y);
            if (neigh >= 0 && neigh < 1024 && (gr[index][i] == 1) ){
                temp[0][0] = neigh % 32;
                temp[0][1] = neigh / 32;

                //determine the type of rec call
                if (mode == 1){
                    cur = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, temp, mode, utility, 1, depth+1, maxDepth, max, beta);
                }
                else{
                    cur = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, temp, mode, utility, 1, depth+1, maxDepth, alpha, beta);
                }


                if (cur > max){
                    max = cur;
                    if (depth == 0){
                        path[0][0] = temp[0][0];
                        path[0][1] = temp[0][1];
                    }
                } //end check max


                if (mode == 1){
                    // if beta is less than current return
                    if (beta <= max){
                        break;
                    }
                }

            } //end valid neighbours
        } //end for loop

        minmax_cost[x][y] = max;
        return max;
    } //end mouse turn

    //a cat turn, a min turn
    else{
        double min = beta;
        double cur;
            for (int i = 0; i <= 3; i++){
                //calc new cat loc as it moves
                x = cat_loc[agentId-1][0];
                y = cat_loc[agentId-1][1];
                index = (x + (y*32));
                neigh = indexCalc(i,x,y);
                if (neigh >= 0 && neigh < 1024 && (gr[index][i] == 1)){   
                    temp_cat[agentId-1][0] = neigh % 32;
                    temp_cat[agentId-1][1] = neigh / 32;

                    //determine the type of rec call
                    if (mode == 1){
                        if (agentId == cats){
                            cur = MiniMax(gr, path, minmax_cost, temp_cat, cats, cheese_loc, cheeses, mouse_loc, mode, utility, 0, depth+1, maxDepth, alpha, min);
                            
                        }
                        else{
                           nextAgent = agentId+1;
                           cur = MiniMax(gr, path, minmax_cost, temp_cat, cats, cheese_loc, cheeses, mouse_loc, mode, utility, nextAgent, depth, maxDepth, alpha, min);
                        }
                    }


                    else{
                        if (agentId == cats){
                            cur = MiniMax(gr, path, minmax_cost, temp_cat, cats, cheese_loc, cheeses, mouse_loc, mode, utility, 0, depth+1, maxDepth, alpha, beta);
                            
                        }
                        else{
                           nextAgent = agentId+1;
                           cur = MiniMax(gr, path, minmax_cost, temp_cat, cats, cheese_loc, cheeses, mouse_loc, mode, utility, nextAgent, depth, maxDepth, alpha, beta);
                        }
                    }//mode close

                    
                    if (cur < min){
                        min = cur;
                    }
                    if (mode == 1){
                        // if beta is less than current return
                        if (min <= alpha){
                            break;
                        }
                    }
                } //end neigh check

        }//end for loop
        return min;
    }
}

 return(0.0);
}

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
 /*
	This function computes and returns the utility value for a given game configuration.
	As discussed in lecture, this should return a positive value for configurations that are 'good'
	for the mouse, and a negative value for locations that are 'bad' for the mouse.

	How to define 'good' and 'bad' is up to you. Note that you can write a utility function
	that favours your mouse or favours the cats, but that would be a bad idea... (why?)

	Input arguments:

		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		depth - current search depth
		gr - The graph's adjacency list for the maze

		These arguments are as described in A1. Do have a look at your solution!
 */


double ut; //The value to return
int cloCheese = 99999999; //cheese to positive
int curCheese = 0;
int averageDepthCheese = 0; //average distance to any cheese from current location

int cloCat = 9999999; //cats to be negative
int curCat = 0;
int averageDepthCat = 0; //average distance to any cat from current location

//mouses_index
int mouse_index = (mouse_loc[0][0] + (32*mouse_loc[0][1]));
int cat_index;
int free = 0;

//number of cats that are close
int numOfCats = 0;

/*the number of avaliable moves left for the mouse
*/
int movability = -10;
for (int i = 0; i < 4; i++){
    if (gr[mouse_index][i] == 1){
        movability += 2; 
    }
}


//find the closest cheese and the average distance to all cheeses left
for (int d = 0; d < cheeses; d++){
curCheese = (int) sqrt( 
	(cheese_loc[d][0] - mouse_loc[0][0])*(cheese_loc[d][0] - mouse_loc[0][0]) +
	(cheese_loc[d][1] - mouse_loc[0][1])*(cheese_loc[d][1] - mouse_loc[0][1]));
    if (curCheese < cloCheese){
        cloCheese = curCheese;
    }
    averageDepthCheese += curCheese;
    //if it is the cheese is beside it
    if (cloCheese == 0){
        return (1250);
    }
}

//similar for the cats
for (int c = 0; c < cats; c++){
	cat_index = (cat_loc[c][0] + (32*cat_loc[c][1]));
    curCat =(int) sqrt( 
	    (cat_loc[c][0] - mouse_loc[0][0])*(cat_loc[c][0] - mouse_loc[0][0]) +
	    (cat_loc[c][1] - mouse_loc[0][1])*(cat_loc[c][1] - mouse_loc[0][1]));
    if (curCat <  cloCat){
            cloCat = curCat;
        }
    averageDepthCat += curCat;
	
	//check around if location isn't being taken by a cat and for an offset
	for (int i = 0; i < 4; i++){
		if (gr[mouse_index][i] != cat_index){
			free += 1; 
		}
		else{
			free-= 5;
		}
	}
	
    //no matter if cat there, run!
    if (cloCat <= 0 ){
        //return (-1000);
		numOfCats -= 100;
    }
}

//With the current values determine which one is more important to
double closest = (cloCat - cloCheese);
//if either within depth range, give more influence
if (abs(closest) < (10-depth)){
    closest = (1.5)*closest;
}

ut =  (averageDepthCat - averageDepthCheese) + closest + movability + free + numOfCats;
//printf("UT = %d\n", ut);
return ut;
}

int checkForTerminal(int mouse_loc[1][2],int cat_loc[10][2],int cheese_loc[10][2],int cats,int cheeses)
{
 /* 
   This function determines whether a given configuration constitutes a terminal node.
   Terminal nodes are those for which:
     - A cat eats the mouse
     or
     - The mouse eats a cheese
   
   If the node is a terminal, the function returns 1, else it returns 0
 */

 // Check for cats having lunch
 for (int i=0; i<cats; i++)
  if (mouse_loc[0][0]==cat_loc[i][0]&&mouse_loc[0][1]==cat_loc[i][1]) return(1);

 // Check for mouse having lunch
 for (int i=0; i<cheeses; i++)
  if (mouse_loc[0][0]==cheese_loc[i][0]&&mouse_loc[0][1]==cheese_loc[i][1]) return(1);

 return(0);

}
