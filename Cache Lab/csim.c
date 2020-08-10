#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

FILE *filePointer;

/*this struct holds tag, timeStamp and validity of every line*/
typedef struct{
  long long int tagNum;
  int timeStampNum;
  int validNum;
}line;

/*this struct holds commandline arguments*/
typedef struct{
  char* traceName;
  int setNum;
  int blockNum;
  int lineNum;
}argument;

int main(int argc, char** argv)
{
  line **lineRep; //represents struct line
  argument input; //represents struct argument
  char alphabet; //used in the while loop below
  int totalNumberSets; //stores 2^s number of sets

  //Helps store trace file, b, E and 2^s
  while((alphabet = getopt(argc, argv, "s:E:b:t:")) != -1)
  {
    switch(alphabet)
    {
      case 't':
	input.traceName = optarg; //get Name of Trace file
        break;
      case 'b':
        input.blockNum = atoi(optarg); //get Number of Blocks
        break;
      case 's':
	input.setNum = atoi(optarg); //get s
        totalNumberSets = pow(2, input.setNum);     
        break;
      case 'E':
	input.lineNum = atoi(optarg);//get Number of lines
	break;
      default:
	printf("Invalid Input");
	exit(1);
    }
  }

  filePointer = fopen(input.traceName, "r");
  if(filePointer == NULL)
  {
    printf("No trace file");
    exit(1);
  }

  //allocating and getting space for lines
  lineRep = malloc(totalNumberSets*8);
  int j = 0;
  for(j = 0; j < totalNumberSets; j++)
  {
    lineRep[j] = malloc(input.lineNum * sizeof(line));
  }
  j = 0;
  
  char operationType;//read from the trace file
  int eviction = 0;//evinction number
  int miss = 0;//miss number
  int hit = 0;//hit number
  unsigned long int hexAddress, tagNum, setNum;//read from trace file 

  //keep reading until hitting EOF
  while(fscanf(filePointer, " %c %lx", &operationType, &hexAddress)== 2)
  {
    j = 0;
    int firstInvalidLine = 0;//stores invalid line
    int hitCheck = 0;//check for hit
    int inValidLineCheck = 0; //check for valid line
    int evictionCheck = 0; //check for evictions
    int oldLine = -1; //hold integer for recent line
    int i = 0;
    int sizeOfTag = 64 - input.setNum - input.blockNum;
    tagNum = (hexAddress >> input.setNum) >> input.blockNum;
    setNum = (hexAddress << sizeOfTag) >> (64 - input.setNum);   
    
    if(operationType == 'S' || operationType == 'L' || operationType == 'M')//no hits or misses if operation is instruction load
    {    
      while(j < input.lineNum)
      {
	if((lineRep[setNum][j]).validNum == 0 && inValidLineCheck == 0 )//get the first invalid bit
	{
	  inValidLineCheck = 1;
	  firstInvalidLine = j;
	}
	else if ((lineRep[setNum][j]).validNum == 1 && (lineRep[setNum][j]).tagNum == tagNum)//gives us hit
	{   
	  hitCheck = 1;
	  for(i = 0; i < input.lineNum; i++)
	  {
	    if(i == j)
	    {
	      (lineRep[setNum][i]).timeStampNum = 0;//hit in this line meaning recently used
	    }
	    else
	    {
	      (lineRep[setNum][i]).timeStampNum = (lineRep[setNum][i]).timeStampNum + 1;
	    }
	  }
	  break;
	}
	j++;
      } 

      if(hitCheck == 0)//no hit
      {
	if(inValidLineCheck == 1)//if there was any invalid line
	{    
	  (lineRep[setNum][firstInvalidLine]).validNum = 1; //set specific invalidid lines of code for validation
	  (lineRep[setNum][firstInvalidLine]).tagNum = tagNum;//set tag for corresponding linne
	  for(i = 0; i < input.lineNum; i++)
	  {    
	    if(i == firstInvalidLine)
	    {
	      (lineRep[setNum][i]).timeStampNum = 0;//reset time stamp to 0
	    }
	    else
	    {
	      (lineRep[setNum][i]).timeStampNum = (lineRep[setNum][i]).timeStampNum + 1;//update set lines
	    }
	  }
	  evictionCheck = 0;//no eviction
        }
	else if (inValidLineCheck == 0)
	{
	  int s = 0;
          int leastRecentUse = 0;

     	  //for loop finds oldest line num in least recently used cache with hihgest timme stamp
	  for(s = 0; s < input.lineNum;s++)
	  {
	    if(lineRep[setNum][s].timeStampNum > oldLine)
	    {
	      oldLine = (lineRep[setNum][s]).timeStampNum;
	      leastRecentUse = s;
	    }
	  }

	  for(i = 0; i < input.lineNum; i++)
	  {
	    if(i == leastRecentUse)
	    {
	      (lineRep[setNum][i]).timeStampNum = 0;//rset time stamp num to 0
	    }
	    else
	    {
	      (lineRep[setNum][i]).timeStampNum = (lineRep[setNum][i]).timeStampNum + 1;//increase time stamp for other lines
	    }
	  }
	  (lineRep[setNum][leastRecentUse]).validNum = 1;
	  (lineRep[setNum][leastRecentUse]).tagNum = tagNum;
	  evictionCheck = 1;
      	}
      }

      //if hit
      if (hitCheck)
      {       
	hit += 1;
      }
      else if(evictionCheck)//if eviction
      {   
        miss += 1;
	eviction += 1;
      }
      else
      {
	miss +=1;
      }

      if(operationType == 'M')//if modify
      {              
	hit += 1;
      }
    }
  }

  fclose(filePointer);

  //frree memory
  int counterVar = 0;
  for(counterVar = 0; counterVar < totalNumberSets;counterVar++)
  {
    free(lineRep[counterVar]);
  }
  free(lineRep);
  printSummary(hit, miss, eviction);
  return 0;
}
