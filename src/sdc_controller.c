
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "extApi.c"
#include "extApi.h"
#include "extApiPlatform.h"
#include "extApiPlatform.c"
#include "extApiInternal.h"

#define MAX_EXT_API_CONNECTIONS=255
#define NON_MATLAB_PARSING
#define DO_NOT_USE_SHARED_MEMORY
#define PI 3.14159265 
#define NUM_THREADS 2


//Global Variables
simxInt steering;
simxInt steeringWheelJoint;
simxInt steeringLeft;
simxInt steeringRight;
simxInt motorLeft;
simxInt motorRight;
simxInt rearMLeft;
simxInt rearMRight;
simxInt path;
simxInt pathFollower;
simxInt pathFollowerTarget;
double desiredSteeringAngle;
double desiredWheelRotSpeed;
double steeringAngleDx;
double wheelRotSpeedDx;
double d;
double l;
int cnter;

simxInt pf;
simxInt m3;
simxInt m3;
simxInt ppf;
simxInt ll;
simxInt pp;
simxInt v;

double correction;

int main (int argc,char* argv[])
{
	int portNb=19997;
    pthread_t pid[NUM_THREADS];
    int art rc;

	printf("Program started\n");
	int clientID= simxStart((simxChar*)"127.0.0.1",portNb,true,true,2000,5);
	

    void *(*thr_func_ptr_arr[])(void*) = {function2,function3};
    if (clientID!=-1)
	{
		printf("connected to remote api server\n");
		printf("\n Number of ID: %d",clientID);
		
		initialization();
		
        for(i=0; i < NUM_THREADS; ++i){
            if((rc=pthread_create(&pid[i], NULL, (*thr_func_ptr_arr[i], NULL)))){
                return EXIT_FAILURE;
            }
        }

        for (int i=0; i < NUM_THREADS; ++i){
            pthread_join(pid[i], NULL);
        }
        return EXIT_SUCCESS;
	
	 simxFinish(clientID);
	}
	else{
		printf("fail");
		simxFinish(clientID);
	}

    
	return 0;

	
}

void initialization(){

//This is an extremely simple control script for the car. It is meant as a SIMPLE example!
if (sim_call_type==simSyscb_init){

    //Retrieving of some handles and setting of some initial values:
    steering=simxGetObjectHandle('vw_steering');
    steeringWheelJoint=simxGetObjectHandle('vw_steeringWheelJoint');
    steeringLeft=simxGetObjectHandle('vw_steeringLeft');;
    steeringRight=simxGetObjectHandle('vw_steeringRight');
    motorLeft=simxGetObjectHandle('vw_motorLeft');
    motorRight=simxGetObjectHandle('vw_motorRight');
    rearMLeft=simxGetObjectHandle('vw_freeAxisLeft');
    rearMRight=simxGetObjectHandle('vw_freeAxisRight');
    path=simxGetObjectHandle('vw_path');
    pathFollower=simxGetObjectHandle('vw_pathFollower');
    pathFollowerTarget=simxGetObjectHandle('vw_pathFollowerTarget');
    desiredSteeringAngle=0;
    desiredWheelRotSpeed=2000*math.pi/180;
    steeringAngleDx=2*math.pi/180;
    wheelRotSpeedDx=20*math.pi/180;
    d=0.755; //2*d=distance between left and right wheels
    l=2.5772; //l=distance between front and read wheels
    //Set a fixed velocity:
    simxSetJointTargetVelocity(motorLeft,desiredWheelRotSpeed);
    simxSetJointTargetVelocity(motorRight,desiredWheelRotSpeed);
    //Make the path object orphan (because at the beginning of the simulation it is still built on the car model):
    simxSetObjectParent(path,-1,true);
    cnter=0;

    if (sim_call_type==simSyscb_actuation){
    //Retrieve the closest point to the "pathFollower" object on the path to follow:
    pf=simxGetObjectPosition(pathFollower,-1);
    m3=simxGetObjectMatrix(path,-1);
    m3=simxGetInvertedMatrix(m3);
    ppf=sim.multiplyVector(m3,pf); //ppf is the position of "pathFollower" relative to the path object reference frame
    ll=simxGetClosestPositionOnPath(path,ppf)
    pp=simxGetPositionOnPath(path,ll); //pp is the position closest to ppf, that is on the path
    simxSetObjectPosition(pathFollowerTarget,-1,pp); //set the "pathFollowerTarget" object to the pp position
    simxInt v=simxGetObjectPosition(pathFollowerTarget,pathFollower); //v is the position of "pathFollowerTarget" relative to the "pathFollower" object reference frame
    
    //Calculate the required correction:
    correction=-v[2]*0.15;
    }
   
    if (math.abs(correction)>0.05){
        correction = correction*0.05/math.abs(correction);
    }

}   

void function2(){

   desiredSteeringAngle=desiredSteeringAngle+correction
    if(desiredSteeringAngle > 45*PI/180){
        desiredSteeringAngle = 45*PI/180;
    }
    if(desiredSteeringAngle < -(45*PI/180)){
        desiredSteeringAngle= -(45*PI/180);
    }

    simxSetJointPosition(steering,desiredSteeringAngle);
    steeringAngle = simxGetJointPosition(steering);
    simxSetJointPosition(steeringWheelJoint,steeringAngle*8);

    steeringAngleLeft= atan(1/(-d+1/tan(steeringAngle)));
    steeringAngleRight = atan(1/(d+1/tan(steeringAngle)));
    simxSetJointTargetPosition(steeringLeft, steeringAngleLeft);
    simxSetJointTargetPosition(steeringRight, steeringAngleLeft);

    if (cnter>0){
        cnter=cnter-1;
        simxSetJointTargetVelocity(motorLeft, 0);
        simxSetJointTargetPosition(motorRight,0);
        simxSetJointForce(motorLeft,20000);
        simxSetJointForce(motorRight, 20000);
        simxSetObjectInt32Parameter(rearMLeft, 20000,1);
        simxSetObjectInt32Parameter(rearMRight,20000,1);
    }
    else{
        simxSetJointTargetVelocity(motorLeft,desiredWheelRotSpeed);
        simxSetJointTargetVelocity(motorRight,desiredWheelRotSpeed);
        simxSetJointForce(motorLeft,200);
        simxSetJointForce(motorRight, 200);
        simxSetObjectInt32Parameter(rearMLeft, 2000,1);
        simxSetObjectInt32Parameter(rearMRight,2000,1);

    }

}

void function3(){

    simxInt message,auxiliaryData=simGetSimulatorMessage()
    if (message !=-1){
        if (message==sim.message_keypress){ 
            cnter=40; // when a key is pressed, we brake
        }
    }
}
 