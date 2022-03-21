#include <headers.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int headers_compare(const void *a, const void *b, void *udata) {
    const HTTP_Header *ha = a;
    const HTTP_Header *hb = b;
    return strcmp(ha->name, hb->name);
}

void headers_free(void *item){
  HTTP_Header *header = item;
  free(header->name);
  free(header->value);
}

uint64_t headers_hash(const void *item, uint64_t seed0, uint64_t seed1) {
  const HTTP_Header *header = item;
  return hashmap_sip(header->name, strlen(header->name), seed0, seed1);
}

struct hashmap *create_headers() {
  return hashmap_new(sizeof(HTTP_Header), 0, 0xc8763, 0xdeadbeef, headers_hash,
                     headers_compare, headers_free, NULL);
}

void set_header(struct hashmap *map, char *name, char *value) {

  char *cname = malloc(strlen(name)+1);
  strncpy(cname,name,strlen(name)+1);

  char *cvalue = malloc(strlen(value)+1);
  strncpy(cvalue,value,strlen(value)+1);

  HTTP_Header *prev = hashmap_set(map, &(HTTP_Header){.name=cname, .value=cvalue});
  if(prev){
    free(prev->name);
    free(prev->value);
  }
}

HTTP_Header *get_header(struct hashmap *map, char *name) {
  return hashmap_get(map, &(HTTP_Header){.name = name});
}

bool h_iter(const void *item, void *udata) {
  const HTTP_Header *header = item;
  printf("%s:%s\n",header->name,header->value);
  return true;
}

void list_headers(struct hashmap* map){
  hashmap_scan(map, h_iter, NULL);
}