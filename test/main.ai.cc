#include "http.ai.h"

int main() {
    http wiki("Connecting to Wikipedia", "103.102.166.224");
    wiki.httpsReq("en.wikipedia.org", "/wiki/America");
    wiki.httpsRes("resp.html");
    return 0;
}
