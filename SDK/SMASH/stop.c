#include"SFCC_API/inc/SFCC_API.h"
#include<stdio.h>

int main(int argc, char* argv[])
{
	int test;
    test = SFCC_API_Stop("root/cimv2",argv[1], 1, argv[2]);
	return 0;
}
