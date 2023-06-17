How to compile my program:
Run the following commands
Step 1- gcc --std=gnu99 -o movies main.c
Step 2- ./movies movies_sample_1.csv

Thats it :D

This program will:
1.Reads a CSV file with movie data that is provided to the program as an argument
2.Processes the data in the file to create structs to hold data for each movie
3.Creates a linked list of all these structs
4.Gives user choices to ask questions about the movies in the data
5.Prints out the data about the movies per user choice
