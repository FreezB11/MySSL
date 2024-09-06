#include "ssl/http.h"

int main(){
    // http test("hello","20.207.73.88");
    // test.httpReq("codeload.github.com","/FreezB11/Ikigai/zip/refs/heads/main");
    // test.httpRes("outfile");

    // http ggl("ggl","142.250.192.67");
    // ggl.httpReq("google.co.in","/");
    // ggl.httpRes("resp");

    http wiki("hi","103.102.166.224");
    wiki.httpReq("en.wikipedia.org","/wiki/America");
    wiki.httpRes("resp.html");
    return 0;
}