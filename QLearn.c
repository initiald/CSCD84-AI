/*
	CSC D84 - Unit 3 - Reinforcement Learning
	
	This file contains stubs for implementing the Q-Learning method
	for reinforcement learning as discussed in lecture. You have to
	complete two versions of Q-Learning.
	
	* Standard Q-Learning, based on a full-state representation and
	  a large Q-Table
	* Feature based Q-Learning to handle problems too big to allow
	  for a full-state representation
	    
	Read the assignment handout carefully, then implement the
	required functions below. Sections where you have to add code
	are marked

	**************
	*** TO DO:
	**************

	If you add any helper functions, make sure you document them
	properly and indicate in the report.txt file what you added.
	
	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Jan. 16
*/

#include "QLearn.h"

void QLearn_update(int s, int a, double r, int s_new, double *QTable)
{
 /*
   This function implementes the Q-Learning update as stated in Lecture. It 
   receives as input a <s,a,r,s'> tuple, and updates the Q-table accordingly.
   
   Your work here is to calculate the required update for the Q-table entry
   for state s, and apply it to the Q-table
     
   The update involves two constants, alpha and lambda, which are defined in QLearn.h - you should not 
   have to change their values. Use them as they are.
     
   Details on how states are used for indexing into the QTable are shown
   below, in the comments for QLearn_action. Be sure to read those as well!
 */
 

	//define a max function for s_new
	double max = -10000.0;
	for (int i = 0; i<4; i++){
		if (*(QTable+(4*s_new)+i) > max){
			max = *(QTable+(4*s_new)+i);
		}
	}
	
	*(QTable+(4*s)+a) += alpha*(r + (lambda*max) - *(QTable+(4*s)+a));
    //printf("HERE = %f and MAX = %f \n", *(QTable+(4*s)+a), max);
  
}

int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, double *QTable, int size_X, int graph_size)
{
  /*
     This function decides the action the mouse will take. It receives as inputs
     - The graph - so you can check for walls! The mouse must never move through walls
     - The mouse position
     - The cat position
     - The chees position
     - A 'pct' value in [0,1] indicating the amount of time the mouse uses the QTable to decide its action,
       for example, if pct=.25, then 25% of the time the mouse uses the QTable to choose its action,
       the remaining 75% of the time it chooses randomly among the available actions.
       
     Remember that the training process involves random exploration initially, but as training
     proceeds we use the QTable more and more, in order to improve our QTable values around promising
     actions.
     
     The value of pct is controlled by QLearn_core_GL, and increases with each round of training.
     
     This function *must return* an action index in [0,3] where
        0 - move up
        1 - move right
        2 - move down
        3 - move left
        
     QLearn_core_GL will print a warning if your action makes the mouse cross a wall, or if it makes
     the mouse leave the map - this should not happen. If you see a warning, fix the code in this
     function!
     
   The Q-table has been pre-allocated and initialized to 0. The Q-table has
   a size of
   
        graph_size^3 x 4
        
   This is because the table requires one entry for each possible state, and
   the state is comprised of the position of the mouse, cat, and cheese. 
   Since each of these agents can be in one of graph_size positions, all
   possible combinations yield graph_size^3 states.
   
   Now, for each state, the mouse has up to 4 possible moves (up, right,
   down, and left). We ignore here the fact that some moves are not possible
   from some states (due to walls) - it is up to the QLearn_action() function
   to make sure the mouse never crosses a wall. 
   
   So all in all, you have a big table.
        
   For example, on an 8x8 maze, the Q-table will have a size of
   
       64^3 x 4  entries
       
       with 
       
       size_X = 8		<--- size of one side of the maze
       graph_size = 64		<--- Total number of nodes in the graph
       
   Indexing within the Q-table works as follows:
   
     say the mouse is at   i,j
         the cat is at     k,l
         the cheese is at  m,n
         
     state = (i+(j*size_X)) * ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size)
     ** Make sure you undestand the state encoding above!
     
     Entries in the Q-table for this state are

     *(QTable+(4*state)+a)      <-- here a is the action in [0,3]
     
     (yes, it's a linear array, no shorcuts with brackets!)
     
     NOTE: There is only one cat and once cheese, so you only need to use cats[0][:] and cheeses[0][:]
   */
 //determine which turns are random and from QTable

   int move = 0; //the move to be made, 0 as default
   double max = -10000.0;
   int mouse_index = mouse_pos[0][0] + (mouse_pos[0][1]*size_X);

    //get the current state
   int state = (mouse_pos[0][0]+(mouse_pos[0][1]*size_X)) + ((cats[0][0]+(cats[0][1]*size_X))*graph_size) + ((cheeses[0][0]+(cheeses[0][1]*size_X))*graph_size*graph_size);
    


    //Choosing from QTable
    int r = rand() % 100;
    

    if (mouse_index >= 0 && mouse_index < 1024){

        if ((r <= (pct*100)) || pct == 1){
            
           //for each move
            for (int i = 0; i<4; i++){
               if (gr[mouse_index][i] == 1){

                    int value = *(QTable+(4*state)+i);

                    if (value > max){
                        max = value;
                        move = i;
                    }
	               
               } //end check for individual wall
            } //end of foor loop for wall check

        } //end if chosen from QTable

        else{
            //choose a move randomly
            int q = rand() % 4;
            while (gr[mouse_index][q] == 0){
                q = rand() % 4;
            }

            return q;

        }
    }

  return(move);		// <--- of course, you will change this!
  
}

double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function computes and returns a reward for the state represented by the input mouse, cat, and
    cheese position. 
    
    You can make this function as simple or as complex as you like. But it should return positive values
    for states that are favorable to the mouse, and negative values for states that are bad for the 
    mouse.
    
    I am providing you with the graph, in case you want to do some processing on the maze in order to
    decide the reward. 
        
    This function should return a maximim/minimum reward when the mouse eats/gets eaten respectively.      
   */

    //using euclidean distance for both the distance to the cat and the cheese respectfully, same ish like the utility function
    //one cat only and cheese only?
    int mouseToCat = (int) sqrt( 
	(cats[0][0] - mouse_pos[0][0])*(cats[0][0] - mouse_pos[0][0]) +
	(cats[0][1] - mouse_pos[0][1])*(cats[0][1] - mouse_pos[0][1])
	);

    int mouseToCheese = (int) sqrt( 
	(cheeses[0][0] - mouse_pos[0][0])*(cheeses[0][0] - mouse_pos[0][0]) +
	(cheeses[0][1] - mouse_pos[0][1])*(cheeses[0][1] - mouse_pos[0][1])
	);

    int catToCheese = (int) sqrt( 
	(cheeses[0][0] - cats[0][0])*(cheeses[0][0] - cats[0][0]) +
	(cheeses[0][1] - cats[0][1])*(cheeses[0][1] - cats[0][1])
	);

    int mouse_index = mouse_pos[0][0] + (mouse_pos[0][1]*size_X);
    
    int movability = -10;
    int found_cat = 0;

    for (int i = 0; i < 4; i++){
        if (gr[mouse_index][i] == 1){
            movability +=2;
        }
        if ((mouseToCheese < 2) && (gr[mouse_index][i] == 1)){
            //printf("CHEESE\n"); 
            return 100.0;
        }

        if ((mouseToCat < 2) && (gr[mouse_index][i] == 1)){
        //printf("CAT\n");
            found_cat = 1;
        }
    }

    //If Either one is right beside it, return a large value right away, favoring
    // cheese first
    if (found_cat == 1){
        return -100.0;
    }
    

    //Otherwise calc some reward other wise
    double reward = (mouseToCat - mouseToCheese)+ (catToCheese - mouseToCheese) + movability;

    
  return(reward);		// <--- of course, you will change this as well!     
}

void feat_QLearn_update(double gr[max_graph_size][4],double weights[25], double reward, int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function performs the Q-learning adjustment to all the weights associated with your
    features. Unlike standard Q-learning, you don't receive a <s,a,r,s'> tuple, instead,
    you receive the current state (mouse, cats, and cheese potisions), and the reward 
    associated with this action (this is called immediately after the mouse makes a move,
    so implicit in this is the mouse having selected some action)
    
    Your code must then evaluate the update and apply it to the weights in the weight array.    
   */
  
   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/  

   //Simulate New move and get the max move
	double *U; //maxu, using this for max
	int *A; // maxA
	
	//make copies of the mouse,cats, and cheese
	int mouse[1][2] ;
	mouse[0][0] = mouse_pos[0][0];
	mouse[0][1] = mouse_pos[0][1];
	
	int catCopy[5][2];
	int cheeseCopy[5][2];
	
	for (int d = 0; d<5; d++){
		for (int k = 0; k<2; k++){
			catCopy[d][k] = cats[d][k];
			cheeseCopy[d][k] = cheeses[d][k];
		}
	}
	
	double f[numFeatures];
	//fake feature list i nonexit
	if (weights[0] == 0){
		
		for (int k = 0; k < numFeatures; k++){
			f[k] = 0.01;
		}
	}
	maxQsa(gr,weights,mouse, catCopy, cheeseCopy, size_X, graph_size, U, A);
	evaluateFeatures(gr,f, mouse, catCopy, cheeseCopy, size_X, graph_size);
	//get current state
	double Q = Qsa(weights, f);
	
   for (int i = 0; i < numFeatures; i++){
	   //if there is no weight meaning no feature?
	   if (weights[i] == 0){
		   weights[i] += 0.01; //stub value for testing
	   }
        weights[i] += alpha*(reward + lambda*(*U) - Q)*(f[i]);
   }
}

int feat_QLearn_action(double gr[max_graph_size][4],double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, int size_X, int graph_size)
{
  /*
    Similar to its counterpart for standard Q-learning, this function returns the index of the next
    action to be taken by the mouse.
    
    Once more, the 'pct' value controls the percent of time that the function chooses an optimal
    action given the current policy.
    
    E.g. if 'pct' is .15, then 15% of the time the function uses the current weights and chooses
    the optimal action. The remaining 85% of the time, a random action is chosen.
    
    As before, the mouse must never select an action that causes it to walk through walls or leave
    the maze.    
   */

  
  int move = 0; //the move to be made, 0 as default
   double max = -10000.0;
   int mouse_index = mouse_pos[0][0] + (mouse_pos[0][1]*size_X);

    //get the current state
   double *U; //maxu,
	int *A; // maxA, using this for the action
	

	//Choosing from QTable
	int r = rand() % 100;
	if (mouse_index >= 0 && mouse_index < 1024){
		if ((r <= (pct*100)) || pct == 1){
		   maxQsa(gr,weights,mouse_pos, cats, cheeses, size_X, graph_size, U, A);
		   move = *A;
		} //end if chosen from QTable

		else{
			//choose a move randomly until a valid move appears
			move = rand() % 4;
			while (gr[mouse_index][move] == 0){
				move = rand() % 4;
			}
		}
	} // end of valid mouse

  return(move);	

}

void evaluateFeatures(double gr[max_graph_size][4],double features[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
   This function evaluates all the features you defined for the game configuration given by the input
   mouse, cats, and cheese positions. You are free to define up to 25 features. This function will
   evaluate each, and return all the feature values in the features[] array.
   
   Take some time to think about what features would be useful to have, the better your features, the
   smarter your mouse!
   
   Note that instead of passing down the number of cats and the number of cheese chunks (too many parms!)
   the arrays themselves will tell you what are valid cat/cheese locations.
   
   You can have up to 5 cats and up to 5 cheese chunks, and array entries for the remaining cats/cheese
   will have a value of -1 - check this when evaluating your features!
  */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/      
   double featureList[numFeatures];
   //initialize feature list
   for (int f = 0; f < numFeatures; f++){
	   featureList[f] = 0;
   }
   int x = mouse_pos[0][0];
   int y = mouse_pos[0][1];
   int cur_cat = 0;
   
   /*Feature 1: Survival Rate
   Find out what is rate the mouse would be eaten*/
   double avg = 0;
   int max = -1000;
   int num = 0;
   for (int c = 0; c < 5; c++){
	   if ((cats[c][0] == -1) || (cats[c][1] == -1)){
		   //calculate euclidean distance
		   cur_cat = (int) sqrt( 
			(cats[0][0] - mouse_pos[0][0])*(cats[0][0] - mouse_pos[0][0]) +
			(cats[0][1] - mouse_pos[0][1])*(cats[0][1] - mouse_pos[0][1])
			);
			if (cur_cat < max){
				max = cur_cat;
			}
			avg += cur_cat;
			num += 1;
	   } //end valid cats
   }//end for loop cats
   avg = avg / num; // average distance of cats
   
   if (avg > (size_X/2)){
	   //mouse is generally safe
	   featureList[0] = 1.0;
   }
   else{
	   //mouse isn't safe
	   featureList[0] = -1*(max%2);
   }
   
   
   /*Feature 2: Closest Cheese!
   Finding the closest cheese is the most important to the mouse*/
   avg = 0;
   int cur_cheese = 0;
   int max_cheese = -1000;
   for (int c = 0; c < 5; c++){
	   if ((cheeses[c][0] == -1) || (cheeses[c][1] == -1)){
		   //calculate euclidean distance
		   cur_cat = (int) sqrt( 
			(cheeses[0][0] - mouse_pos[0][0])*(cheeses[0][0] - mouse_pos[0][0]) +
			(cheeses[0][1] - mouse_pos[0][1])*(cheeses[0][1] - mouse_pos[0][1])
			);
			if (cur_cheese < max_cheese){
				max_cheese = cur_cheese;
			}
			avg += cur_cheese;
			num += 1;
	   } //end valid cats
   }//end for loop cats
   
   avg = avg / num; // average distance of cats
   
   if (avg > 3){
	   //mouse is generally safe
	   featureList[1] = -1.0;
   }
   else{
	   //mouse is really close
	   featureList[1] = 1-(max*0.25);
   }
   
   
   /*Feature 3: Navi
   The ability to navigate and not run into deadends or travel to places before?*/
   
}

double Qsa(double weights[25], double features[25])
{
  /*
    Compute and return the Qsa value given the input features and current weights
   */

  /***********************************************************************************************
  * TO DO: Complete this function
  ***********************************************************************************************/  

    double sum = 0.0;      
   for (int i = 0; i < numFeatures; i++){
        sum += weights[i]*features[i];
   }
    
  
  return(sum);		// <--- stub! compute and return the Qsa value
}

void maxQsa(double gr[max_graph_size][4],double weights[25],int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size, double *maxU, int *maxA)
{
 /*
   Given the state represented by the input positions for mouse, cats, and cheese, this function evaluates
   the Q-value at all possible neighbour states and returns the max. The maximum value is returned in maxU
   and the index of the action corresponding to this value is returned in maxA.
   
   You should make sure the function does not evaluate moves that would make the mouse walk through a
   wall. 
  */
 
   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/  

    //get the current state
	double max = -1000.0;
	int move = 0;
	int x = mouse_pos[0][0];
	int y = mouse_pos[0][1];
	
	double featureList[numFeatures];
   //initialize feature list
   for (int f = 0; f < numFeatures; f++){
	   featureList[f] = 0;
   }
	
	int mouse_index = x + (y*size_X);
	
	int moveL[4];
   
   for (int i = 0; i<4; i++){
	   if (i == 0){
        moveL[i] = (x + ((y-1)*32));
		}
		//right
		if (i == 1){
			moveL[i] = ((x+1) + (y*32));
		}
		//bottom
		if (i == 2){
			moveL[i] = (x + ((y+1)*32));
		}
		//left
		if (i == 3){
			moveL[i]  = ((x-1) + (y*32));
		}
   }
    
    //for each move (index,value)
            for (int i = 0; i<4; i++){
               if (gr[mouse_index][i] == 1){
				   int temp = moveL[i];
				   int new_mouse_pos[1][2];
				   new_mouse_pos[0][0] = temp % 32;
				   new_mouse_pos[0][1] = temp / 32;
				   
				   evaluateFeatures(gr, featureList, new_mouse_pos, cats, cheeses, size_X, graph_size);
				   
                    int value = Qsa(weights,  featureList);
					

                    if (value > max){
                        max = value;
                        move = i;
                    }
	               
               } //end check for individual wall
            } //end of foor loop for wall check
		
		*maxU = max;
		*maxA = move;

  return;
   
}

/***************************************************************************************************
 *  Add any functions needed to compute your features below 
 *                 ---->  THIS BOX <-----
 * *************************************************************************************************/
