#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value){
  for(long i = 0; i < map->capacity; i++){
    long index= (hash(key, map->capacity) + i)%map->capacity;
    struct Pair *currentPair = map->buckets[index];
    if(currentPair == NULL || currentPair->key == NULL){
      map->buckets[index] = createPair(key, value);
      map->current = index;
      map->size++;
      return;
    }
  }
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  struct Pair** bucketsO = map->buckets;
  map->capacity *= 2;
  map->buckets= (struct Pair**)malloc(sizeof(struct Pair *)* map->capacity);
  map->size = 0;
  for(long i = 0; i < map->capacity/2;i++){
    struct Pair* currentpair = bucketsO[i];
    if(currentpair != NULL && currentpair->key != NULL){
      insertMap(map,currentpair->key,currentpair->value);
    }
  }
}


HashMap * createMap(long capacity) {
  struct HashMap *map = (struct HashMap *)malloc(sizeof(struct HashMap));
  if (map == NULL) {
    exit(1);
  }
  map->buckets = (struct Pair **)malloc(sizeof(struct Pair *) * capacity);
  if (map->buckets == NULL) {
    free(map);
    exit(1);
  }
  for (long i = 0; i < capacity; i++) {
    map->buckets[i] = NULL;
  }
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  
  return map;
}

void eraseMap(HashMap * map,  char * key) {
  for(int i = 0; i < map->capacity;i++){
    int index = (hash(key,map->capacity)+i) %map->capacity;
    struct Pair* currentpair = map->buckets[index];
    if(currentpair == NULL){
      map->current = -1;
      return;
    }
    if(strcmp(currentpair->key, key) == 0){
      currentpair->key = NULL;
      map->size--;
      map->current = index;
      return;
    }
  }
}

Pair * searchMap(HashMap * map,  char * key) {
  for (long i = 0; i < map->capacity; i++) {
    long index = (hash(key, map->capacity) + i) % map->capacity;
    struct Pair *currentPair = map->buckets[index];
    if (currentPair == NULL){
      map->current = -1;
      return NULL;
    }
    if(strcmp(currentPair->key, key) == 0){
      map->current = index;
      return currentPair;
    }
  }
  map->current = -1;
  return NULL;
}

Pair * firstMap(HashMap * map) {
  for(int i = 0; i < map->capacity;i++){
    struct Pair* currentpair = map->buckets[i];
    if(currentpair != NULL && currentpair->key != NULL){
      map->current = i;
      return currentpair;
    }
  }
  map->current = -1;
  return NULL;
}

Pair * nextMap(HashMap * map) {
  for(int i = map->current +1; i < map->capacity;i++){
    struct Pair* currentpair = map->buckets[i];
    if(currentpair != NULL && currentpair->key != NULL){
      map->current = i;
      return currentpair;
    }
  }
  map->current = -1;
  return NULL;
}
