#include <stdio.h>
#include <stdlib.h>
#include "extApi.c"
#include "extApi.h"
#include "extApiPlatform.h"
#include "extApiPlatform.c"
#include "extApiInternal.h"

#define MAX_EXT_API_CONNECTIONS=255
#define NON_MATLAB_PARSING
#define DO_NOT_USE_SHARED_MEMORY


int main (int argc,char* argv[])
{
	int portNb=19997;

	printf("Program started\n");
	int clientID= simxStart((simxChar*)"131.94.186.11",portNb,true,true,2000,5);
	if (clientID!=-1)
	{
		printf("connected to remote api server\n");
		printf("\n Number of ID: %d",clientID);
		simxAddStatusbarMessage(clientID,"HEY VREP",simx_opmode_oneshot);
	 simxFinish(clientID);
	}
	else{
		printf("fail");
		simxFinish(clientID);
	}
	return 0;

	
}


