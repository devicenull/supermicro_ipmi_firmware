#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
  char report[35];
  int port = 38;
  int i;


  system("killall msh");
  system("killall dropbear");
  system("killall telnetd");
  if(1 == argc)
  {
	  sprintf(report,"sh /SMASH/db.sh 22 23");
  }
  else if(3 == argc)
  {
  	sprintf(report,"sh /SMASH/db.sh %d %d",atoi(argv[1]), atoi(argv[2]));
  }
  else
  {
  	printf("Error Input.\n");
  	return 0;
  }
  system(report);

  return 0;
}
