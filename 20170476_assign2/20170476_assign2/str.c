#include <assert.h> /* to use assert() */
#include <stdio.h>
#include "str.h"
/* Your task is: 
   1. Rewrite the body of "Part 1" functions - remove the current
      body that simply calls the corresponding C standard library 
      function. 
   2. Write appropriate comment per each function 
*/

/* Part 1 */
/*------------------------------------------------------------------------*/
size_t StrGetLength(const char* pcSrc)
{
  const char *pcEnd;
  assert(pcSrc); /* NULL address, 0, and FALSE are identical. */
  pcEnd = pcSrc;
  while (*pcEnd) /* null character and FALSE are identical. */
    pcEnd++;
  return (size_t)(pcEnd - pcSrc);
}
/*------------------------------------------------------------------------*/
char *StrCopy(char *pcDest, const char* pcSrc)
{
  /* TODO: fill this function */
  assert(pcSrc);
  assert(pcDest);
  int i=0;
  while(*(pcSrc+i)){
    *(pcDest+i)=*(pcSrc+i);
    i++;
  }
  *(pcDest+i)='\0';
  return pcDest;
}
/*------------------------------------------------------------------------*/
int StrCompare(const char* pcS1, const char* pcS2)
{
  /* TODO: fill this function */
  assert(pcS2);
  assert(pcS1);
  int i=0;
  for(i=0;*(pcS1+i)==*(pcS2+i);i++){
    if(*(pcS1+i)=='\0')return 0;
  }
  return *(pcS1+i)-*(pcS2+i);
}
/*------------------------------------------------------------------------*/
char *StrSearch(const char* pcHaystack, const char *pcNeedle)
{
  /* TODO: fill this function */ 
  assert(pcNeedle);
  assert(pcHaystack);
  int i=0;
  int len=StrGetLength(pcNeedle);
  if(*pcNeedle=='\0')return(char *) pcHaystack;
  while(*pcHaystack){
    if(*pcHaystack==*pcNeedle){
      for(i=0;*(pcHaystack+i)==*(pcNeedle+i);i++);
      if(i==len)return(char *)pcHaystack;
    }
    pcHaystack++;
  }
  return NULL;
}
/*------------------------------------------------------------------------*/
char *StrConcat(char *pcDest, const char* pcSrc)
{
  /* TODO: fill this function */
  assert(pcDest);
  assert(pcSrc);
  int i=0;
  int j=0;
  while(*(pcDest+i)!=0){
    i++;
  }
  while(*(pcSrc+j)!=0){
    *(pcDest+i)=*(pcSrc+j);
    i++;
    j++;
  }
  *(pcDest+i)='\0';
  return pcDest;
  }
