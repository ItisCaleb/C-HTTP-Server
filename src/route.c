#include <routes.h>
#include <string.h>

typedef struct HTTP_Route {
  char *path;
  Request_Handler handler;
} HTTP_Route;

int routes_compare(const void *a, const void *b, void *udata) {
  const HTTP_Route *ra = a;
  const HTTP_Route *rb = b;
  return strcmp(ra->path, rb->path);
}

uint64_t routes_hash(const void *item, uint64_t seed0, uint64_t seed1) {
  const HTTP_Route *route = item;
  return hashmap_sip(route->path, strlen(route->path), seed0, seed1);
}

struct hashmap *create_router() {
  return hashmap_new(sizeof(HTTP_Route), 0, 0xc8763, 0xdeadbeef, routes_hash,
                     routes_compare, NULL, NULL);
};

void add_route(struct hashmap *routes, char *path, Request_Handler handler) {
  hashmap_set(routes, &(HTTP_Route){.path = path, .handler = handler});
}

bool execute_handler(struct hashmap *routes, char *path, HTTP_Request *req,
                     HTTP_Response *res) {
  HTTP_Route *route = hashmap_get(routes, &(HTTP_Route){.path = path});
  if (route == NULL) {
    req->_status = Not_Found;
    return false;
  } else {
    route->handler(req, res);
    return true;
  }
}
