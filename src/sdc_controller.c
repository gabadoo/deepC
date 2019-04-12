#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "extApiPlatform.h"
#define MAX_EXT_API_CONNECTIONS=255
#define NON_MATLAB_PARSING

extern "C"{
#include "extApi.h"
}

int main (int argc,char* argv[])
{

	printf("Program started\n");
	int clientID= simxStart((simxChar*)"127.0.0.1",19999,true,true,2000,5);
	if (clientID!=-1)
	{
		printf("connected to remote api server\n");
	}
	simxFinish(clientID);
	return 0;
}


