%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CSC D84 - Assignment 6 - Handwritten digit classification 
% with randomi tree forest.
%
% In this assignment you will implement a system to classify handwrotten
% digits (larger in resolution than the ones you classified in A5 with
% neural nets) 
%
% The digit data comes from the MNIST dataset.
% http://yann.lecun.com/exdb/mnist/
%
% Each digit image is 28x28 pixels.
%
% We have both a training data set with 60,000 digits and their labels,
% as well as a testing data set with 10,000 digits and their labels.
%
% You can only ever use the digits in the training set while building your
% tree forest.
%
% Your goal is to train classifiers based on decision trees and see how
% well they perform at identifying the different digits in the
% dataset.
%
% The great thing about random tree forests is that you can use them for
% all sorts of classification and regression tasks, so you could easily
% grab a different dataset or problem and use the same algorithm to
% build a pretty good classifier!
%
% Starter code: F.J.E., March 2014.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Brief notes on programming in Matlab (avoid time wasteage!)
%
% - Indexing starts at 1, not 0 ! array(1) is the first entry in an array
% - Note the use of () for indexing, instead of []
% - Useful functions on 1-d arrays (known as vectors in Marlab)
%   len, max, sum, min, find - type 'help yyyy' to find help on function yyyy
% - Slicing: You can do things like this array(1:3) gives you entries 1-3 in an array
%            array(7:end) gives you all entries from 7 to the end of the array
% - array(end+1)=vvv  is the equivalent of Python 'append' on a list
% - Vectors (1-d arrays) are either column vectors or row vectors. The can only be
%            mixed as per the regular rules of linear algebra. You can add or
%            subtract vectors only if they are the same length and shape
% - Transposing a vector is easy, use the apostrophe ('). E.g.  array'  gives you
%            the transpose of array
% - 2D arrays (matrices) are equally easy, indexing starts at 1, and 
%            A(i,j) gives you row 'i', column 'j' (this is standard math indexing,
%            but is the opposite from C array indexing!)
% - Matrix multiplication and matrix-vector operations all follow standard rules
%            of linear algebra
% - For loops:
%                for i=init:increment:bound	% initial value, increment, and max value
%			% do stuff		% and runs while i<=bound
%		 end;
% - If statements:
%		if (whatever)
%			% do stuff
%		else
%			% fo other stuff
%		end;
% - Note Matlab is a rational language and has an explicit 'end' statement to define
%	     the scope of code. Indentation does not matter.
% - Use a semicolon (;) at the end of your statements! otherwise, Matlab will print out
%            the resulting values for each statement to the console, which is a lot of
%            printing if you have large matrices.
% - Mind the size of your data. Use size(X) to tell you the size of an array/vector.
%            check that everything makes sense according to Linear Algebra rules.
% - 'return' takes no arguments. Any variables defined at the top as return values will
%            be returned automatically.
% - That's it! go play with Matlab.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Your Tasks:
%
%	- Write code to train a decision-tree using the set of training digits and
%	  corresponding labels
%	- Train a large set of random decision trees (a randomized tree forest)
%	  on the training set
%	- Use the resulting tree forest to classify new (test) data.
%	- Study the performance of the classifier as you change the number of
%	  trees in the forest, and the type of tests done within the trees
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [treeForest]=trainForest(N,levels,trials);
	% This function trains a randomized tree forest with N trees, and returns the
	% tree forest (format dicussed below). The test at each node is chosen from
	% among 'trials' random trials. Trees will have the specified number of levels
	% counting from 0

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% YOU DO NOT NEED TO WRITE CODE IN THIS FUNCTION. However you must understand what
% it's doing.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

load DigitData;
%trainImages=trainImages(1:5:end,:);		% These two lines take only one out of every 5 samples.
%trainLabels=trainLabels(1:5:end);		% Note classes are 1-10, not 0-9 because of indexing in Matlab

% Create an array for the trees in the tree forest. See the trainRandomizedDT() function for details.
% The array below will hold one tree per row. Each tree has the specified number of levels (starting
% with the root at level 0). So the number of nodes in the tree is (2^(levels+1))-1. 
% Each tree node contains 13 values.
treeForest=zeros(N,((2^(levels+1))-1)*13);	

for treeNumber=1:N	% Run the training procedure for N trees
	
	fprintf(2,'***** Training tree #%d of %d\n',treeNumber,N);
	
	% Generate random training subset from original training set
	ni=randperm(length(trainLabels));
	trainSubset=trainImages(ni(1:6000),:);					% Random 10% of training set
	trainSubclass=trainLabels(ni(1:6000));					% with training labels

	[Tree]=trainRandomizedDT([],trainSubset,trainSubclass,1,trials,levels); % You need to write code in trainRamdomizedDT()
	treeForest(treeNumber,:)=Tree(:);					% Append this tree to the forest as a single long vector.
										% To recover the tree in the same format as it was 
										% from trainRandomizedDT(), simply get the tree vector
										% from the forest and reshape: 
										% Tree=reshape(treeForest(x,:),[(2^(levels+1))-1 13]);
										% where 'x' is the index of the tree you want.
end;

s=sprintf('Tree_Forest_%d_%d_%d.mat',N,trials,levels);				% Save your trained forest. You will have to submit the
save(s,'treeForest');								% trained forest as specified in the handout.

% Done! wasn't that easy?

