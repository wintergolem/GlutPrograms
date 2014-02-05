#include <math.h>



int foo(){
	int u = 0;
	int v = 1;
	int i,t;

	for(i=2;i<7;i++){
		t = u+v;
		u=v;
		v=t;
	}
	return v;
}


void main(){
	foo();
}