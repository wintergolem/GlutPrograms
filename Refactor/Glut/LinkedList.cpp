#include <stdio.h>
//Globals
int count = 0;
int nodevalue = 0;
int nodeNumberOf = 0;
template <typename T> 
class Node {
public:
	T value;
	Node *next;
};

template <typename T>
class LinkedList {
public:
	Node<T> *start;

	LinkedList(){
		start = NULL;
	}
	LinkedList(T value){
		start = NULL;
	}

	void add(T value)
	{
		Node<T> *n = new Node<T>;
		n->value = value;
		n->next = start;
		start = n;
	}

	void remove(){
		Node<T> *n = start;
		/*for(int i =1; n && i <pos;i++)
			n = n->next;
		if(n && n->next){*/
			Node<T> *r = n->next;
			n->next = r->next;
			delete r;
	}

	void insert(){
		Node<T> *n = start;
//		for(int i =1; n && i <pos;i++)
//			n = n->next;
		if(n && n->next){
			Node<T> *r = new Node<T>;
			r->next = n->next;
			n->next = r;
			}
	}

	void listinsert(){
		int value;
		for(int i = 0; i<5; i++){
			Node<T> *n = new Node<T>;
			value = getvalue();
			n->value = value;
			n->next = start;
			start = n;
		}
	}

	int Count(){
		Node<T> *n = start;
		count = 0;
		for(int i =0;;i++){
			n = n->next;
			if(n){
				count= count +1;
			}
			else
				return count;
		}
	}

	int getvalue(){
		nodevalue++;
		return nodevalue;
	}

};




void main(){
	LinkedList<int> list;
	char c;
	do{
		scanf("%c",&c);
		switch (c){
			case 'a':
				list.add(list.getvalue());
				break;
			case 'c':
				list.remove();
				break;
			case 'g':
				list.insert();
				break;
			case 'h':
				list.listinsert();
				break;
			case 'k':
				printf("%i", list.Count());
				break;
		}
	}while(c != 'q');
	nodeNumberOf = list.Count();
 	Node<int> *n = list.start;

	for(int i=0;i<nodeNumberOf;i++){
		printf("\n%i", n->value);
		printf("\nNumber of Node = %i", nodeNumberOf);
		n = n->next;
	}
		getchar();
		getchar();
}