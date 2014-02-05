// =======================================
// Space Invaders.cpp
// Copyright (c) Steven Hoover, 2012
// All rights reserved
// ---------------------------------------


//--INCLUDE FILES--
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <string>
#include "SimpleFramework.h"
#include "gl/freeglut.h"
#include <time.h>
#include <math.h>
#include <vector>
#include <list>
#include <iostream>

using namespace std;

//typedef vector<person*> Vector;
//--GLOBALS

class person
{
public:
	person(const char *name){

	}

	//const char *GetName(){}
	void AddAsFriend(person *p){
		friends.push_back(p);
	}
	unsigned int FriendCount (){
		int i = 0;
		for(vector<person*>::iterator it = friends.begin(); it != friends.end(); it++){
			i++;
		}
		return i;
	}
	//person *GetFriend(unsigned int index){}
	bool IsAFriend(person *p){
		for (vector<person*>::iterator it = friends.begin(); it != friends.end(); it++)
			if(*it == p){
				return true;
		}
			return false;
	}

private:
	char name[32];
	vector<person*> friends;
};//end of Person class
person steven("steven"),deven("Deven"),jason("Jason"), jerk("Jerk"), wench("wench");

int DegOfSep(person &pA, person &pB)
{
	
}

void AddFriends(person &pA, person &pB){
	//add pB to pA's friends
	pA.AddAsFriend(&pB);
	//add pA to pB's Friends
	pB.AddAsFriend(&pA);
}

void AllocatePeople(){
	AddFriends(steven, deven);
	AddFriends(steven,jason);
}

void writetruefalse(person &pA, person &pB){
	if(pA.IsAFriend(&pB) == true)
		cout<<"True"<<endl;
	else
		cout<<"False";
}

void main(){
	AllocatePeople();
	
	//test it
	cout<<"Are Steven and Deven Friends?";
	writetruefalse(steven,deven);
	cout<<"Are Steven and Jerk Friends?";
	writetruefalse(steven,jerk);

	cout<<"test DegOfSep: Deven to Jason (2) = "<<DegOfSep(deven,jason)<<endl;
	getchar();
}