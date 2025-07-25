#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

/*--------------------------------------------------------------------*/

#define UNIT_ARRAY_SIZE 1024

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
};

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
  DB_T d; //DB_T : typedef struct DB* 
  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 1024 elements
  d->numItems = 0;
  d->pArray = (struct UserInfo *)calloc(d->curArrSize,
               sizeof(struct UserInfo));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n", d->curArrSize);   
    free(d);
    return NULL;
  }
  return d;
}
/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
  if(d == NULL) return;
  struct UserInfo* p = d->pArray;
  for(int i = 0; i < d->curArrSize; i++){
      if(p[i].name != NULL){
          free(p[i].name);
          free(p[i].id);
      }
  }
  free(d->pArray);
  free(d);
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
 if(d == NULL) return -1;
 if(id == NULL) return -1;
 if(name == NULL) return -1;
 if(purchase <= 0) return -1; 
 struct UserInfo* p = d->pArray; 
 if(d->numItems == d->curArrSize){
     struct UserInfo* tmp;
     int before = 0;
     before = d->curArrSize;
     d->curArrSize += UNIT_ARRAY_SIZE;
     tmp = (struct UserInfo* )realloc(d->pArray, sizeof(struct UserInfo) * d->curArrSize);
     if(tmp == NULL){
         fprintf(stderr,"Cannot reallocate the memory for expanded array of size %d\n", d->curArrSize);
         return -1;
     }
     d->pArray = tmp;
     p = d->pArray;
     for(int i = before; i < d->curArrSize; i++){
          p[i].name = NULL;
          p[i].id = NULL;
          p[i].purchase = 0;
     }
 }
/* SEARCH */ 
 for(int i = 0; i < d->curArrSize; i++){
     if(p[i].name != NULL){
        if(strcmp(p[i].name,name) == 0){
            return -1;
        }
        if(strcmp(p[i].id,id) == 0){
            return -1;
        }
     }
 }
 for(int i = 0; i < d->curArrSize; i++){
    if(p[i].name == NULL){
        p[i].name = strdup(name);
        p[i].id = strdup(id);
        p[i].purchase = purchase;
        d->numItems += 1;
        return 0;
    }
 } 
 return -1;
}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  if(d == NULL) return -1;
  if(id == NULL) return -1;
  struct UserInfo* p = d->pArray;
  for(int i = 0;i < d->curArrSize;i++){
    if(p[i].id != NULL){
        if(strcmp(p[i].id,id) == 0){
            free(p[i].name);
            free(p[i].id);
            p[i].name = NULL;
            p[i].id = NULL;
            p[i].purchase= 0;
            d->numItems -= 1;
            return 0;
        }
    }
  }
  return -1;
}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  if(d == NULL) return -1;
  if(name == NULL) return -1;
  struct UserInfo* p = d->pArray;
  for(int i = 0;i < d->curArrSize;i++){
    if(p[i].name != NULL){
        if(strcmp(p[i].name,name) == 0){
            free(p[i].name);
            free(p[i].id);
            p[i].name = NULL;
            p[i].id = NULL;
            p[i].purchase= 0;
            d->numItems -= 1;
            return 0;
        }
    }
  }
  return -1;
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  if(d == NULL) return -1;
  if(id == NULL) return -1;
  struct UserInfo* p = d->pArray;
  for(int i = 0;i < d->curArrSize;i++){
    if(p[i].id != NULL){
        if(strcmp(p[i].id,id) == 0){
             return p[i].purchase;
        }
    }
  }
  return -1;
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
  if(d == NULL) return -1;
  if(name == NULL) return -1;
  struct UserInfo* p = d->pArray;
  for(int i = 0;i < d->curArrSize;i++){
    if(p[i].name!= NULL){
        if(strcmp(p[i].name,name) == 0){
             return p[i].purchase;
        }
    }
  }
  return -1;
}
/*--------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
 if(d == NULL) return -1;
 if(fp == NULL) return -1;
 struct UserInfo* p = d->pArray;
 int sum = 0;
 for(int i =0; i< d-> curArrSize;i++){
     if(p[i].name != NULL || p[i].id != NULL){
         sum+=fp(p[i].id,p[i].name,p[i].purchase);
     }
 }
 return sum;
}

