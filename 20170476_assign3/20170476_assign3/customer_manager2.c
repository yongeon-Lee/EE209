#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

/*--------------------------------------------------------------------*/
#define UNIT_ARRAY_SIZE 1024
#define HASH_MULTIPLIER 65599
struct UserInfo {
  char *name;                  // customer name
  char *id;                    // customer id
  int purchase;                // purchase amount (> 0)
  struct UserInfo* next_id;    // pointer to next node linked with id hash
  struct UserInfo* next_name;  // pointer to next node linked with name hash
};

struct DB {
  struct UserInfo** pArray_ID;   // pointer to hash table id
  struct UserInfo** pArray_NAME; // pointer to hash table name 
  int curArrSize;            // current array size
  int numItems;              // # of stored items 
};

static int hash_function(const char *pcKey, int iBucketCount)
    /* Return a hash code for pcKey that is between 0 and iBucketCount-1,
     *    inclusive. Adapted from the EE209 lecture notes. */
{
       int i;
       unsigned int uiHash = 0U;
       for (i = 0; pcKey[i] != '\0'; i++){
            uiHash = uiHash * (unsigned int)HASH_MULTIPLIER + (unsigned int)pcKey[i];
       }
       return (int)(uiHash % (unsigned int)iBucketCount);
}
/*-------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
  DB_T d;//DB_T : typedef struct DB* 
  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  /* Make pointer array saving user information */
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 1024 elements
  d->numItems = 0;
  d->pArray_ID = (struct UserInfo **)calloc(d->curArrSize,
               sizeof(struct UserInfo*));
  d->pArray_NAME = (struct UserInfo **)calloc(d->curArrSize,
               sizeof(struct UserInfo*));
  if (d->pArray_ID == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n", d->curArrSize);   
    free(d);
    return NULL;
  }
  if (d->pArray_NAME == NULL) {
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
  struct UserInfo* nextp;
  for(int i = 0; i < d->curArrSize; i++){
      if(d->pArray_ID[i] != NULL){
          for(struct UserInfo* p = d->pArray_ID[i]; p != NULL; p = nextp){
              nextp = p->next_id;
              free(p->id);
              free(p->name);
              free(p);
          }
      }
  }
  free(d->pArray_NAME);
  free(d->pArray_ID);
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
  struct UserInfo** hash_id;
  struct UserInfo** hash_name;
   /* ---------------------------Hash Expansion-------------------------- */
  if((d->curArrSize * 0.75 <= d->numItems) && (d->numItems <= 1048576)){
      struct UserInfo** tmp_id;
      struct UserInfo** tmp_name;
      struct UserInfo* nextp;
      int before = 0;
      before = d->curArrSize;
      d->curArrSize += UNIT_ARRAY_SIZE;
      tmp_id = (struct UserInfo**)calloc(d->curArrSize,sizeof(struct UserInfo*));
      tmp_name = (struct UserInfo**)calloc(d->curArrSize,sizeof(struct UserInfo*));
      if(tmp_id == NULL){
          fprintf(stderr,"Can't allocate a memory for expanded hash of size %d\n",d->curArrSize);
          return -1;
      }
      if(tmp_name == NULL){
          fprintf(stderr,"Can't allocate a memory for expanded hash of size %d\n",d->curArrSize);
          return -1;
      }
      for(int i = 0; i < before ; i++){
          if(d->pArray_ID[i] != NULL){
              for(struct UserInfo* p = d->pArray_ID[i]; p != NULL; p = nextp){
                  int new_hash_id = hash_function(p->id,d->curArrSize);
                  int new_hash_name = hash_function(p->name,d->curArrSize);
                  nextp = p->next_id;
                  p->next_name = tmp_name[new_hash_name];
                  tmp_name[new_hash_name] = p;
                  p->next_id = tmp_id[new_hash_id];
                  tmp_id[new_hash_id] = p;
              }
          }
      }
      free(d->pArray_ID);
      free(d->pArray_NAME);
      d->pArray_ID = NULL;
      d->pArray_NAME = NULL; 
      d->pArray_ID = tmp_id;
      d->pArray_NAME = tmp_name;
  }
  /*--------------------------------------------------------------------*/
  hash_id = d->pArray_ID;
  hash_name = d->pArray_NAME;
  int key_NAME = hash_function(name,d->curArrSize);
  int key_ID = hash_function(id,d->curArrSize);
  /* SEARCH */
  for(struct UserInfo* p = hash_id[key_ID]; p != NULL; p = p->next_id){
      if(strcmp(id,p->id) == 0){
          return -1;
       }
  }
  for(struct UserInfo* p = hash_name[key_NAME]; p != NULL; p = p->next_name){
      if(strcmp(name,p->name) == 0){
          return -1;
      }
  }
  /* Make Node */
  struct UserInfo* p = (struct UserInfo*)malloc(sizeof(struct UserInfo));
  /* Put Information and Linking */
  p->name = strdup(name);
  p->id = strdup(id);
  p->purchase = purchase;
  p->next_name = hash_name[key_NAME];
  hash_name[key_NAME] = p;
  p->next_id = hash_id[key_ID];
  hash_id[key_ID] = p;
  d->numItems += 1;
  return 0;
}
/*-------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  if(d == NULL) return -1;
  if(id == NULL) return -1; 
  int key_ID = hash_function(id,d->curArrSize);
  struct UserInfo* p1; // target node pointer
  struct UserInfo* p2; // target node pointer
  struct UserInfo* front_id; // node in front of target node 
  struct UserInfo* front_name; // node in front of target node
  struct UserInfo** t1 = d->pArray_ID; // target hash table
  struct UserInfo** t2 = d->pArray_NAME; // target hash table
  p1 = t1[key_ID];
  if(p1 == NULL) return -1; // no such id that match with parameter
  /* CASE 1 : If target node is at the Entry of Hash Table_ID */
  if(strcmp(id,p1->id) == 0){
      t1[key_ID] = p1->next_id;
      int key_NAME = hash_function(p1->name,d->curArrSize);
      p2 = t2[key_NAME];
      /* CASE 1-1 : If target node is at the Entry of Hash Table_NAME */
      if(strcmp(p1->name,p2->name) == 0){
         t2[key_NAME] = p2->next_name;
         free(p1->name);
         free(p1->id);
         free(p1);
         d->numItems -= 1;
         return 0;
      }
      /* CASE 1-2 : If target node isn't at the Entry of Hash Table_NAME */
      for(front_name = p2; p2 != NULL; front_name = p2, p2 = p2->next_name){
          if(strcmp(p1->name,p2->name) == 0){
              front_name->next_name = p2->next_name;
              break;
          }
      }
      free(p1->name);
      free(p1->id);
      free(p1);
      d->numItems -= 1;
      return 0;
  }
  /* CASE 2 :  if target node isn't at the entry of Hash Table_ID */
  for(front_id = p1 ; p1 != NULL; front_id = p1,p1 = p1->next_id){
          if(strcmp(id,p1->id) == 0){
              front_id->next_id = p1->next_id;
              break;
          }
  }
  if(p1 == NULL) return -1; // no such id that match with parameter
  int key_NAME = hash_function(p1->name,d->curArrSize);
  p2 = t2[key_NAME];
  /* CASE 2-1 : if target node is at the entry of Hash Table_NAME */
  if(strcmp(p1->name,p2->name) == 0){
        t2[key_NAME] = p2->next_name;
        free(p1->name);
        free(p1->id);
        free(p1);
        d->numItems -= 1;
        return 0;
      }
  /* CASE 2-2 : if target node isn't at the entry of Hash Table_NAME*/
  for(front_name = p2; p2 != NULL; front_name = p2, p2 =p2->next_name){
          if(strcmp(p1->name,p2->name) == 0){
              front_name->next_name = p1->next_name;
              break;
          }
  }
  free(p1->name);
  free(p1->id);
  free(p1);
  d->numItems -= 1;
  return 0;
}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  if(d == NULL) return -1;
  if(name == NULL) return -1;
  int key_NAME = hash_function(name,d->curArrSize);
  struct UserInfo* p1; // target node pointer
  struct UserInfo* p2; // target node pointer
  struct UserInfo* front_id; // node in front of target node 
  struct UserInfo* front_name; // node in front of target node
  struct UserInfo** t1 = d->pArray_ID; // target hash table
  struct UserInfo** t2 = d->pArray_NAME; // target hash table
  p2 = t2[key_NAME];
  if(p2 == NULL) return -1;// no such name that match with parameter
  /* CASE 1 : If target node is at the Entry of Hash Table_NAME */
  if(strcmp(name,p2->name) == 0){
      t2[key_NAME] = p2->next_name;
      int key_ID = hash_function(p2->id,d->curArrSize);
      p1 = t1[key_ID];
  /* CASE 1-1 : If target node is at the Entry of Hash Table_ID */
      if(strcmp(p1->id,p2->id) == 0){
          t1[key_ID] = p1->next_id;
          free(p2->name);
          free(p2->id);
          free(p2);
          d->numItems -= 1;
          return 0;
      }
      /* CASE 1-2 : If target node isn't at the Entry of Hash Table_ID */
      for(front_id = p1; p1 != NULL; front_id = p1, p1 = p1->next_id){
          if(strcmp(p1->id,p2->id) == 0){
              front_id->next_id = p1->next_id;
              break;
          }
      }
      free(p2->name);
      free(p2->id);
      free(p2);
      d->numItems -= 1;
      return 0;
  }

 /* CASE 2 :  if target node  isn't at the entry of Hash Table_NAME */
  for(front_name = p2 ; p2 != NULL; front_name = p2,p2 = p2->next_name){
          if(strcmp(name,p2->name) == 0){
              front_name->next_name = p2->next_name;
              break;
          }
  }
  if(p2 == NULL) return -1;// no such name that match with parameter
  int key_ID = hash_function(p2->id,d->curArrSize);
  p1 = t1[key_ID];
 /* CASE 2-1 : if target node is at the entry of Hash Table_ID */
  if(strcmp(p1->id,p2->id) == 0){
        t1[key_ID] = p1->next_id;
        free(p2->name);
        free(p2->id);
        free(p2);
        d->numItems -= 1;
        return 0;
      }
 /* CASE 2-2 : if target node isn't at the entry of Hash Table_ID*/
  for(front_id = p1; p1 != NULL; front_id = p1, p1 = p1->next_id){
          if(strcmp(p1->id,p2->id) == 0){
              front_id->next_id = p2->next_id;
              break;
          }
  }  
  free(p2->name);
  free(p2->id);
  free(p2);
  d->numItems -= 1;
  return 0;

}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  if(d == NULL) return -1;
  if(id == NULL) return -1;
  int key_ID = hash_function(id,d->curArrSize);
  struct UserInfo * p;
  p = d->pArray_ID[key_ID];
  if(p == NULL) return -1; // no such id
  for(; p != NULL; p=p->next_id){
      if(strcmp(id,p->id) == 0){
          return p->purchase;
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
 int key_NAME = hash_function(name,d->curArrSize);
 struct UserInfo* p;
 p = d->pArray_NAME[key_NAME];
 if(p == NULL) return -1; // no such name
 for(; p != NULL; p=p->next_name){
     if(strcmp(name,p->name) == 0){
         return p->purchase;
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
  int sum = 0;
  for(int i = 0; i< d->curArrSize; i++){
    if(d->pArray_ID[i] != NULL){
        for(struct UserInfo* p = d->pArray_ID[i]; p != NULL; p=p->next_id){
            sum += fp(p->id,p->name,p->purchase);
        } 
    }
  }
  return sum;
}

