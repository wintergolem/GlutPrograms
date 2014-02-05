#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <list>
using namespace std;
typedef list<string> List;
string enter;
list<string> nameList;


void addname(){
	cout<<"Enter names: \n";
	while(enter != "q"){
		cin>>enter;
		if(enter != "q"){
			nameList.push_back(enter);
		}
	}
	nameList.sort();
}

void displaynames(){
	for(List::iterator it = nameList.begin();it != nameList.end(); it++){
		cout<<*it<<endl;
	}
}

void removename() {
	unsigned int location;
	cout<<"Remove at which location:  ";
	cin>>location;
	List::iterator i = nameList.begin(); 
	for(unsigned int j = 0;j < location -1; j++)
		i++;
	nameList.erase(i);
}//end of removename

void removespecific(){
	string temp;
	cout<<"Remove which name:  ";
	cin>>temp;
	for (List::iterator i = nameList.begin(); i != nameList.end(); i++)
		if(*i == temp){
			nameList.erase(i);
			break;
		}
	
}

void main(){
	char in = 'd';

	while(in !='q'){
		cout<<"what what you like to do:  ";
		cin>>in;
		if((in < '1' || in > '5') || in == 'd')
			cout<<"number please \n";
		else{
			switch(in){
			case '1':
				addname();
				break;
			case '2':
				displaynames();
				break;
			case '3':
				removename();
				break;
			case '4':
				removespecific();
				break;
			case '5':
				system("cls");
				break;
			}//end switch
		}//end else
	}//end of while
	getchar();
	getchar();
}