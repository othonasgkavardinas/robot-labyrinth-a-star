# robot-labyrith-a-star

## Robot navigation in a maze
Simple application that solves a maze. The user inputs the start and two goal states. The robot reaches the nearest goal first and then the furthest.

### /files
Assignment 1

### Installation
make install

### Example

$ make install && make run

gcc -o robot_labyrinth_a_star main.c  
./robot_labyrinth_a_star  
~~MATRIX~~  
0 0 0 0 0   
0 0 0 0 0   
1 0 0 0 0   
1 0 0 0 0   
0 1 1 0 0   

Choices:   
(0,0) (0,1) (0,2) (0,3) (0,4) (1,0) (1,1) (1,2) (1,3) (1,4) (2,1) (2,2) (2,3) (2,4) (3,1) (3,2) (3,3) (3,4) (4,0) (4,3) (4,4)   
Please insert a starting point S(i,j)  
i j: 0 0  
S(0,0)  
Insert target X(i, j), *except (0,0)  
i j: 2 3  
G1(2,3)  
Insert target Y(i, j), *except (0,0),(2,3)  
i j: 4 4  
G2(4,4)Status: S(0,0), X(2,3) , Y(4,4)  
  
2 0 0 0 0   
0 0 0 0 0   
1 0 0 3 0   
1 0 0 0 0   
0 1 1 0 4   
  
First goal is X  
5.00 4.00 3.00 2.00 3.00   
4.00 3.00 2.00 1.00 2.00   
3.00 2.00 1.00 0.00 1.00   
4.00 3.00 2.00 1.00 2.00   
5.00 4.00 3.00 2.00 3.00   
2 0 0 0 0   
7 7 0 0 0   
1 7 7 3 0   
1 0 0 0 0   
0 1 1 0 4   
cost1: 25.00  
first extensions: 10  
8.00 7.00 6.00 5.00 4.00   
7.00 6.00 5.00 4.00 3.00   
6.00 5.00 4.00 3.00 2.00   
5.00 4.00 3.00 2.00 1.00   
4.00 3.00 2.00 1.00 0.00   
2 0 0 0 0   
7 7 0 0 0   
1 7 7 3 0   
1 0 0 7 0   
0 1 1 7 4   
cost2: 9.00  
total cost: 34.00  
total extensions: 6  

