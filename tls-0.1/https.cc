#include "https.h"

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

https::https(char *URI){
    // parse the uri to extract the host and its path
    char *pos;
    pos = strstr(URI,"//");
    if(!pos){log("err")}
    host = pos  + 2;
    pos = strchr(host,'/');
    if(!pos){path = NULL;}else{
        //*pos = '/0';
    path = pos + 1;}
}