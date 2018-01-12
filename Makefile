output: test_matrix.o 
	g++ test_matrix.o -o output 
test_matrix.o: test_matrix.cpp 
	g++ -g -c -Wall test_matrix.cpp 
clean: 
	rm *.o output
