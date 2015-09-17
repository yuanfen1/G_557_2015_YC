//
//  main.cpp
//  C++ file test
//
//  Created by Yuanfen on 9/14/15.
//  Copyright (c) 2015 -. All rights reserved.
//



#include "stdafx.h" // include stdafx, which describes both standard system and project specific include files that are used frequently but hardly ever change

#include <iostream>// include iostream, a header that defines the standard input/output stream objects



using namespace std; //std namespace (where features of the C++ Standard Library, such as string or vector, are declared).After you write this instruction, 
                     //if the compiler sees string it will know that you may be referring to std::string, and if it sees vector, it will know that you may be referring to std::vector.

int myFunction(int n) //define a function
{
	int i = 0, j = 1, t, k; //define the type of the parameter i,j,t,k
	for (k = 0; k <= n; ++k) // a for loop, start from k=0, if k<=n, do the following{}, then K=K+1; if k>n, exit the for loop
	{
		t = i + j; //t equal to i plus j
		i = j; //i equal to j
		j = t; // j equal to t
	}
	return j; // returen j to the main function
}





int main(int argc, const char * argv[]) //defien a main fuction

{
	int p = myFunction(10); //10 is assgined to the variable n in myfuction, then p equal to the value return by myfuction, which is j.
	cout << p << endl; // output p, then end the main fuction.
}
