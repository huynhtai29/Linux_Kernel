#include <stdio.h>
#include <unistd.h>

int main(){
	
	printf("sension id B %d " ,getsid(0));
	return 0;
}