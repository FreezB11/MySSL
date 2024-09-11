#include "https.h"
#include <cstring>
#include "hex.h"
#include "tls.h"
#include "base64.h"


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

int https::httpsConnect(const char *HOST,const char *PATH){
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (!sock){
        perror("unable to create local socket");
        return 2;
    }
    host_name = gethostbyname(host);
    if(!host_name){
        perror("error in name resolution");
        return 3;
    }
    host_address.sin_family = AF_INET;
    host_address.sin_port = htons(Ports);

    memcpy(&host_address.sin_addr, host_name->h_addr_list[0],sizeof(struct in_addr));

    if ( connect( sock, ( struct sockaddr * ) &host_address, sizeof( host_address ) ) == -1 ){perror( "Unable to connect to host" );return 4;}

    printf( "Connection complete; negotiating TLS parameters\n" );

  if ( session_id != NULL )
  {
    if ( tls_resume( sock, session_id_length,
         session_id, master_secret, &tls_context ) )
    {
      fprintf( stderr, "Error: unable to negotiate SSL connection.\n" );
      if ( close(sock) == -1 )
      {
        perror( "Error closing client connection" );
        return 2;
      }
      return 3;
    }
  }
  else
  {
    if ( tls_connect(sock, &tls_context, 0 ) )
    {
      fprintf( stderr, "Error: unable to negotiate TLS connection.\n" );
      return 3;
    }
  }

} 
int https::httpsSend(int sock, const char *path, const char *host, TLSParameters *tls_context){
  static char get_command[MAX_GET_COMMAND];
  sprintf( get_command, "GET /%s HTTP/1.1\r\n", path );

  if ( tls_send( sock, get_command, 
       strlen( get_command ), 0, tls_context ) == -1 ){return -1;}

  sprintf( get_command, "Host: %s\r\n", host );
  if ( tls_send( sock, get_command, 
       strlen( get_command ), 0, tls_context ) == -1 ){return -1;}

  sprintf( get_command, "Connection: close\r\n\r\n" );
  if ( tls_send( sock, get_command, 
       strlen( get_command ), 0, tls_context ) == -1 ){return -1;}
  return 0;   
}

void https::httpsRecv(int sock, TLSParameters *tls_context){
    int recieved = 0;
    static char recv_buf[BUFFER_SIZE + 1];

    while((recieved = tls_recv(sock, recv_buf,BUFFER_SIZE,0,tls_context)) >= 0 ){
        std::cout << recv_buf;
    }
    std::cout << std::endl;
}