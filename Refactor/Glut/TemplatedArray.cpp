#include <stdio.h>
#include <string.h>

template <typename T>
class Array
{
public:
	T *pEntries;
	int capacity;
	int size;
	Array(){
		pEntries = NULL;
		size = capacity = 0;
	}
	Array(int s){
		size = capacity = s;
		Allocate(s);
	}
	void Allocate(int size)
	{
		pEntries = new int[size];
	}
	void Resize(int newsize){
		T *temp = new T [newsize*sizeof(T)];
		memcpy(temp, pEntries, size*sizeof(T));
		delete [] pEntries;
		pEntries = temp;
	}
	void Print(char *title){
		printf("%s:\n" , title);
		for(int i=0;i<size;i++)
			printf("a[%i] = %i\n", i, pEntries[i]);
	}
	void Insert(int fir, int sec){
		if(size > capacity -1)
			Resize(capacity *2);
		pEntries[size] = fir;
		pEntries[size +1] = sec;
		size += 2;
	}
	~Array(){};
};

typedef Array<int> Ints;

void main(){
	Ints a(5);
	for(int i=0; i<5; i++)
		//a[i] = i;
		a.pEntries[i] = i;
	a.Print("before");
	a.Insert(5,6);
	a.Print("after");

	getchar();
}