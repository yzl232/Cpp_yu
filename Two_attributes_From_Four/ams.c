/********************************************************************
AMS Sketches
G. Cormode 2003

This version: 2003-12

This work is licensed under the Creative Commons
Attribution-NonCommercial License. To view a copy of this license,
visit http://creativecommons.org/licenses/by-nc/1.0/ or send a letter
to Creative Commons, 559 Nathan Abbott Way, Stanford, California
94305, USA. 
*********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "ams.h"
#include "prng.h"
#include "massdal.h"

AMS_type * AMS_Init(int buckets, int depth, int s1)
{
  int i,j;
  AMS_type * result;
  prng_type * prng;

  prng=prng_Init(-6321371,2);

  result=calloc(1,sizeof(AMS_type));
  if (result==NULL) exit(1);
  result->depth=depth;
  result->s1=s1;
  result->buckets=buckets;
  result->count=0;
  for (i=0;i<6;i++)
    result->test[i]=calloc(depth*s1,sizeof(long long));
  // create space for the hash functions

  result->counts=(double *) calloc(buckets*depth*s1, sizeof(double));
  if (result->counts==NULL) exit(1); 

  for (i=0;i<depth*s1;i++)
    {
      for (j=0;j<6;j++)
	{
	  result->test[j][i]=(long long) prng_int(prng);
	  if (result->test[j][i]<0) result->test[j][i]= -result->test[j][i];
	  // initialise the hash functions
	  // prng_int() should return a random integer
	  // uniformly distributed in the range 0..2^31
	}
    }
  prng_Destroy(prng);
  return (result);
}

void AMS_Update(AMS_type * ams, long long item, double diff)
{
  // update the sketch
  // hash to one bucket in each row
  // then multiply by {+1, -1} chosen at random

  int j;
  unsigned int hash;
  int mult, offset;


  offset=0;
  for (j=0;j<ams->depth * ams->s1;j++)
    {
      hash=hash31(ams->test[0][j],ams->test[1][j],item);
      hash=hash % (ams->buckets); 
      mult=fourwise(ams->test[2][j],ams->test[3][j],
		    ams->test[4][j],ams->test[5][j],item);
      if ((mult&1)==1)
	ams->counts[offset+hash]+=diff;

      else
	ams->counts[offset+hash]-=diff;
      offset+=ams->buckets;
    }
}





long double AMS_F2Est(AMS_type * ams)
{
  // estimate the F2 moment of the vector (sum of squares)

  int i,j, r, k;
  long double * estimates;
  long double result, z;


  estimates=(long double *) calloc(1+ams->depth, sizeof(long double));
  r=0;
  for (i=1;i<=ams->depth;i++)
    {
	  z=0;
	  for (k=1;k<=ams->s1;k++)
	  {

      for (j=0;j<ams->buckets;j++)

	{
    	  z+=( (ams->counts[r]  *  ams->counts[r]));
    	  r++;
    	}


	  }
          estimates[i]=z / ams->s1;

    }
  if (ams->depth==1) result=estimates[1];
  else if (ams->depth==2) result=(estimates[1]+estimates[2])/2; 
  else
    result=LLMedSelect(1+ams->depth/2,ams->depth,estimates);
  free(estimates);
  return(result);
}




void AMS_Destroy(AMS_type * ams)
{
  // destroy the data structure

  int i;

  if (ams)
    {
      for (i=0;i<6;i++)
	free(ams->test[i]);
      free(ams->counts);
      free(ams);
    }
}


int AMS_AddOn(AMS_type * dest, AMS_type * source){
  int i,j,r;

  // add one sketch to another

  r=0;
  if (AMS_Compatible(dest,source)==0) return 0;
  for (i=0;i<source->depth*source->s1;i++)
    for (j=0;j<source->buckets;j++)
      {
	dest->counts[r]+=source->counts[r];
	r++;
      }
  return 1;
}


int AMS_Compatible(AMS_type * a, AMS_type * b){

  // test whether two sketches have the same parameters
  // if so, then they can be added, subtracted, etc.


  if (!a || !b) return 0;
  if (a->buckets!=b->buckets) return 0;
  if (a->depth!=b->depth) return 0;
  if (a->s1!=b->s1) return 0;
  return 1;
}
