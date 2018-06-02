#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#include "include/decode.c"
#include "include/microtime.c"
#include "include/io.c"

#define BUFLEN 512  // max length of buffer
#define MUMBLE_PING {0x00,0x00,0x00,0x00,0x13,0x37,0x42,0x00,0x11,0x10,0x01,0x00} // 4 byte 0x00 - 8 byte ident
u_int16_t PORT=64738;   // default port
u_int8_t S_TIME=1; // default sleep
u_int16_t COUNT=-1;
bool no_c=true;
u_int16_t timeout=1000;

void handleArguments(int argc, char *argv[]) {
  u_int8_t tmp = string_in("-p", argv, argc);
  if (tmp > 0) {
    char *p;
    u_int16_t num;
    errno = 0;
    long conv = strtol(argv[tmp+1], &p, 10);
    if (!(errno != 0 || *p != '\0' || conv > INT_MAX)) {
      num = conv;
      PORT = num;
    }
  }

  tmp = string_in("-c", argv, argc);
  if (tmp > 0) {
    char *p;
    u_int16_t num;
    errno = 0;
    long conv = strtol(argv[tmp+1], &p, 10);
    if (!(errno != 0 || *p != '\0' || conv > INT_MAX)) {
      num = conv;
      COUNT = num;
			no_c=false;
    }
  }

  tmp = string_in("-s", argv, argc);
  if (tmp > 0) {
    char *p;
    u_int8_t num;
    errno = 0;
    long conv = strtol(argv[tmp+1], &p, 10);
    if (!(errno != 0 || *p != '\0' || conv > INT_MAX)) {
      num = conv;
      S_TIME = num;
    }
  }
}

int main(int argc, char *argv[])
{
  handleArguments(argc, argv);
  struct sockaddr_in si_other;
  int s, i, slen=sizeof(si_other);
  char buf[BUFLEN];
  char message[BUFLEN] = MUMBLE_PING;
  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) p_err("Error opening udp socket!\n");
  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  if (inet_aton(argv[1] , &si_other.sin_addr) == 0) p_err("inet_aton() failed\n");
  while(1) {
    memset(buf,'\0', BUFLEN);
    long time1 = getMicrotime();
    if (sendto(s, message, 12 , 0 , (struct sockaddr *) &si_other, slen)==-1) p_err("Error sending udp packet!\n");;
    while (recvfrom(s, buf, BUFLEN, MSG_DONTWAIT, (struct sockaddr *) &si_other, &slen) == -1) {
       if ((((getMicrotime() - time1)/1000.0)) >= timeout) break;
    }
    if ((((getMicrotime() - time1)/1000.0)) >= timeout) break;
    long time2 = getMicrotime();
    p_png(decode_ping(buf), (((time2 - time1)/1000.0)));
    if (no_c == false) COUNT--;
    if (no_c == false && COUNT == 0) break;
    sleep(S_TIME);
  }
  close(s);
  return 0;
}
