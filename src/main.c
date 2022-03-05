#include <httpserver.h>

void root(HTTP_Request* req, HTTP_Response* res){
    res_redirect(res,"/bruh");
}

void bruh(HTTP_Request* req, HTTP_Response* res){
    res_set_status(res, OK);
    res_set_html(res,"public/index.html");
}

int main(int argc, char *argv[]){
    HTTP_Server* http = create_server(5000);
    handle_route(http,"/",root);
    handle_route(http,"/bruh",bruh);
    start(http);
    return 0;
}