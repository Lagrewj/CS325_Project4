/******************************************************************************************************
CS325 Project4_TSP
Filename: tsp.cpp
Group 3: Yunsik Choi, Tri-Solomon Huynh, Jonathan Lagrew 
Date: 8/8/2015
Description: This program is our implementation of the Traveling Salesman Problem (TSP) using 2-opt
		     and 3-opt algorithms. To run 2-opt or 3-opt instead of just the greedy algorithm, type 
			 2 or 3 after the input file name on the command line. If number of lines is greater than 250 
			 then 2-opt or 3-opt algorithms are ran. 

Input: textfile 

Output: The length of the tour followed by the order of cities travelled. 
******************************************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <time.h>

using namespace std;

ifstream inFile;
ofstream outFile;

//reading file, counting number of lines
int fileLineCount(char *infileName)
{
  int fileLineCount = 0;
  inFile.open(infileName);

  string outfileName = infileName;
  outfileName += ".tour";
  outFile.open(outfileName);

	//counting number of lines in file
	for (string line; getline(inFile, line); ++fileLineCount);
	cout << "# of lines in file: " << fileLineCount << endl;
	inFile.clear();
	inFile.seekg(0, ios::beg);
	return fileLineCount;
}

//populating arraycoord with name, x, y from input file
int populateArraycoord(string names[], int xcoord[], int ycoord[], int lines)
{
	string str;
	int i = 0;
	while (i < lines)
	{
		//get city name from input file
		inFile >> str;
		names[i] = str;
		//get x coord from input file
		inFile >> str;
		xcoord[i] = stoi(str);
		//get y coord from input file
		inFile >> str;
		ycoord[i] = stoi(str);
		i++;//inc
	}
}

//return true if test in arr
bool isIn(int arr[], int test, int length){
  for (int i = 0; i < length; i++) {
    if (arr[i] == test) return true;
  }
  return false;
}

//returns distance between two points to the nearest integer
int distance(float x1, float y1, float x2, float y2) {
  return (int) (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)) + .5);
}

int tourLength(int xcoord[], int ycoord[], int order[], int points) {
  int length = 0;

  for (int i = 0; i < points - 1; i++) {
    length += distance(xcoord[order[i]], ycoord[order[i]], xcoord[order[i+1]], ycoord[order[i+1]]);
  }
  length += distance(xcoord[order[points - 1]], ycoord[order[points - 1]], xcoord[order[0]], ycoord[order[0]]);

  return length;
}

//////////////////Greedy Algorithm/////////////////////////////
int greedyTour(int xcoord[], int ycoord[], int order[], int points)
{
  int length = 0, current, bestPoint = 0, bestDist, nextDist;
  bool firstDist;
  //compare each point to each of the other points that haven't been to yet.
  //Then picks the point with the smallest distance.
  for (int i = 0; i < points; i++) {
    current = bestPoint;
    order[i] = current;
    firstDist = true;
    bestDist = 0;
    for (int j = 0; j < points; j++) {
      if (isIn(order, j, points)) continue;
      nextDist = distance(xcoord[current], ycoord[current], xcoord[j], ycoord[j]);
      if (nextDist < bestDist || firstDist) {
	bestDist = nextDist;
	bestPoint = j;
	firstDist = false;
      }
    }
      length += bestDist;
  }
  //add up the array lengths
  length += distance(xcoord[order[points - 1]], ycoord[order[points - 1]], xcoord[order[0]], ycoord[order[0]]);
  cout << "Greedy tour length: " << length << endl;
  return length;
}

///////////////////////2-OPT Algorithm///////////////////////////////
int opt2Tour(int xcoord[], int ycoord[], int order[], int points, string names[])
{
  int length = 0, bestPointIdx, bestDist, nextDist;
  bool isChange;
  //at each point it considers switching, then stores the best solution until
  for (int i = 0; i < points - 1; i++) {
    //setting best to current dist and point
    bestDist = tourLength(xcoord, ycoord, order, points);
    bestPointIdx = i;
    for (int j = i+1; j < points; j++) {
      //getting tour distance
      swap(order[i], order[j]);
      nextDist = tourLength(xcoord, ycoord, order, points);
      swap(order[i], order[j]);
      if (nextDist < bestDist) {
  	bestDist = nextDist;
	bestPointIdx = j;
      }
    }
    //swapping if better tour
    if (bestPointIdx != i) {
      swap(order[i], order[bestPointIdx]);
      isChange = true;
    }
  }
  length = tourLength(xcoord, ycoord, order, points);
  cout << "2-OPT length: " << length << endl;
  return length;
}

///////////////////////////3-OPT Algorithm///////////////////////////////////
int opt3Tour(int xcoord[], int ycoord[], int order[], int points, string names[])
{
  int length = 0, bestPointIdx, bestPointIdx2, bestDist, nextDist;
  bool isChange; //reruns algorithm if swap was made
  //consider swapping at each point to store the best solution and until all
  //points have been compared 
  for (int i = 0; i < points - 1; i++) {
    bestDist = tourLength(xcoord, ycoord, order, points);
    bestPointIdx = i;
    bestPointIdx2 = i;
    for (int j = i+1; j < points; j++) {
      swap(order[i], order[j]);
      nextDist = tourLength(xcoord, ycoord, order, points);
      swap(order[i], order[j]);
      if (nextDist < bestDist) {
  	bestDist = nextDist;
	bestPointIdx = j;
        bestPointIdx2 = i;
      }
      for (int k = j+1; k < points; k++) {
      swap(order[i], order[j]); 
      swap(order[j], order[k]); 
      nextDist = tourLength(xcoord, ycoord, order, points);
      swap(order[j], order[k]);
      swap(order[i], order[j]);
      if (nextDist < bestDist) {
  	bestDist = nextDist;
	bestPointIdx = j;
	bestPointIdx2 = k;
      }
      swap(order[i], order[k]); 
      swap(order[j], order[k]);
      nextDist = tourLength(xcoord, ycoord, order, points);
      swap(order[j], order[k]);
      swap(order[i], order[k]);
      if (nextDist < bestDist) {
  	bestDist = nextDist;
	bestPointIdx = k;
	bestPointIdx2 = j;
      }
      }
    }
    //swapping points if better tour
    if (bestPointIdx != i) {
      if (bestPointIdx2 != i) {
	swap(order[i], order[bestPointIdx]);
	swap(order[bestPointIdx], order[bestPointIdx2]);
      } else {
        swap(order[i], order[bestPointIdx]);
      }
      isChange = true;
    }
  }
  length = tourLength(xcoord, ycoord, order, points);
  cout << "3-OPT length: " << length << endl;
  return length;
}

//output tour length and cities visited to output file 
void outputTour(int length, int order[], int points, string names[]) {
  outFile << length << endl;
  for (int i = 0; i < points; i++) {
    string identifier = names[order[i]];
    outFile << identifier << endl;
  }
}

int main(int argc, char *argv[])
{
	int lines = fileLineCount(argv[1]);//counting lines in file
	double runTime = 0.0; //timer
    clock_t start, stop;//start and stop the timer
	// To find better solutions but taking more time
	int fast = 1;//default command line argument if user does not enter '2' or '3'
	start = clock();//Start timer.
	if (argc > 2) fast = atoi(argv[2]);
	//populate arrays with cities and coordinates
	string names[lines];
	int xcoord[lines];
	int ycoord[lines];
	int order[lines];
  	for (int i = 0; i < lines; i++) order[i] = -1;
	populateArraycoord(names, xcoord, ycoord, lines);

	int totalDist = greedyTour(xcoord, ycoord, order, lines);
	int tempDist;
 	if (fast == 2) {
		do {
			tempDist = totalDist;
			totalDist = opt2Tour(xcoord, ycoord, order, lines, names);
		} while (tempDist != totalDist);
	} else if (fast == 3) {
		do {
			tempDist = totalDist;
			totalDist = opt3Tour(xcoord, ycoord, order, lines, names);
		} while (tempDist != totalDist);
	} else if (lines <= 250) {
		opt3Tour(xcoord, ycoord, order, lines, names);
		opt3Tour(xcoord, ycoord, order, lines, names);
		totalDist = opt3Tour(xcoord, ycoord, order, lines, names);
	} else if (lines <= 1000) {
 		opt2Tour(xcoord, ycoord, order, lines, names);
 		opt2Tour(xcoord, ycoord, order, lines, names);
		opt2Tour(xcoord, ycoord, order, lines, names);
		totalDist = opt2Tour(xcoord, ycoord, order, lines, names);
	} else if (lines <= 2000) {
		totalDist = opt2Tour(xcoord, ycoord, order, lines, names);
	}
	stop = clock();
	  runTime = ((double)(stop - start) / CLOCKS_PER_SEC);
	  std::cout << "Total Runtime: " << runTime << std::endl;//display run time
	outputTour(totalDist, order, lines, names);//output to file 
	return 0;
}
