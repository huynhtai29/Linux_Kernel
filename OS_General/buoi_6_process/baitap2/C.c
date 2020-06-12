#include <stdio.h>
#include <unistd.h>

int main(){
	
	printf("sension id C %d " ,getsid(0));
	return 0;
}