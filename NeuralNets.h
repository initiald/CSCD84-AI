/*
	CSC D84 - Unit 4 - Neural Networks

	This file contains the API function headers for your assignment.
	Please pay close attention to the function prototypes, and
	understand what the arguments are.

	Stubs for implementing each function are to be found in NeuralNets.c,
	along with clear ** TO DO markers to let you know where to add code.

	You are free to add helper functions within reason. But you must
	provide a prototype *in this file* as well as the implementation
	in the .c program file.

	Starter by: F.J.E., Feb. 2016
*/

#ifndef __NeuralNets_header

#define __NeuralNets_header

// Generally needed includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#include<string.h>

#define INPUTS 785			// Number of inputs 28x28 pixel digits = 784 + 1 bias term
#define OUTPUTS 10			// Output classes
#define MAX_HIDDEN 785			// Maximum number of hidden units
#define ALPHA .01			// Network learning rate
#define SIGMOID_SCALE .01		// Scaling factor for sigmoid function input <--- MIND THIS!

// Function prototypes for D84 - Unit 4 - Neural Nets
int train_1layer_net(double sample[INPUTS],int label,double (*sigmoid)(double input), double weights_io[INPUTS][OUTPUTS]);
int train_2layer_net(double sample[INPUTS],int label,double (*sigmoid)(double input), int units, double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS]);
void feedforward_1layer(double sample[INPUTS], double (*sigmoid)(double input), double weights_io[INPUTS][OUTPUTS], double activations[OUTPUTS]);
void feedforward_2layer(double sample[INPUTS], double (*sigmoid)(double input), double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS], double h_activations[MAX_HIDDEN],double activations[OUTPUTS], int units);
void backprop_1layer(double sample[INPUTS],double activations[OUTPUTS], double (*sigmoid)(double input), int label, double weights_io[INPUTS][OUTPUTS]);
void backprop_2layer(double sample[INPUTS],double h_activations[MAX_HIDDEN], double activations[OUTPUTS], double (*sigmoid)(double input), int label, double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS], int units);
int classify_1layer(double sample[INPUTS], int label, double (*sigmoid)(double input), double weights_io[INPUTS][OUTPUTS]);
int classify_2layer(double sample[INPUTS],int label,double (*sigmoid)(double input), int units, double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS]);
double logistic(double input);
// Note that hyperbolic tangent is already provided in math.h as tanh()!

// If you need to add any function prototypes yourself, you can do so *below* this line.
#endif

