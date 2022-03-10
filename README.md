# A HTTP server made by C

This is just a practice project

## Check List
- ~~TCP server~~
- ~~HTTP response~~
- ~~parse request start-line~~
- parse path query
- parse request headers
- parse request data
- multi thread
- ~~more HTTP status code~~
- route matching
- HTTPS
- middleware
- static directory

## Example
```C
#include <httpserver.h>

void root(HTTP_Request* req, HTTP_Response* res){
    res_set_status(res, OK);
    res_set_html(res,"public/index.html");
}

int main(int argc, char *argv[]){
    HTTP_Server* http = create_server(5000);
    handle_route(http,"/",root);
    start(http);
    return 0;
}
```
Then type `make` in terminal
The program will get compiled and output to `out`
Then you can type `./out/main` to run it!