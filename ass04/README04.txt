To run the program ensure that uou have z3 prover installed in your computer

Then, in your current directory type
% First we will generate the test caes and then save it in input.txt file which we will run for main solver program.

g++ genTestcases.cpp -o genTestcases

% This code will push the output to the input.txt file
./genTestcases > input.txt


% Now we will run the main assg04.cpp program. 

g++ assg04.cpp -o assg04 -lz3

./assg04 < input.txt