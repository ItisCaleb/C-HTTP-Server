#include <headers.h>
#include <string.h>
#include <stdio.h>

int headers_compare(const void *a, const void *b, void *udata) {
    const HTTP_Header *ha = a;
    const HTTP_Header *hb = b;
    return strcmp(ha->name, hb->name);
}

uint64_t headers_hash(const void *item, uint64_t seed0, uint64_t seed1) {
  const HTTP_Header *header = item;
  return hashmap_sip(header->name, strlen(header->name), seed0, seed1);
}

struct hashmap *create_headers() {
  return hashmap_new(sizeof(HTTP_Header), 0, 0xc8763, 0xdeadbeef, headers_hash,
                     headers_compare, NULL, NULL);
}

void set_header(struct hashmap *map, char *name, char *value) {
  hashmap_set(map, &(HTTP_Header){.name = name, .value = value});
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