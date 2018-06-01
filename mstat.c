#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h> 
#include <limits.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>


#define BUFLEN 512  // max length of buffer
#define MUMBLE_PING {0x00,0x00,0x00,0x00,0x13,0x37,0x42,0x00,0x11,0x10,0x01,0x00} // 4 byte 0x00 - 8 byte ident
int PORT=64738;   // default port
int S_TIME=3; // default sleep
int COUNT=-1;
int timeout=1;

int string_in(char *my_str, char *string_list[], size_t num_strings)
{
    for ( int i = 0; i < num_strings; i++ ) if (strcmp(my_str, string_list[i]) == 0 ) return i;
    return 0;
}

void handleArguments(int argc, char *argv[]) {
	int tmp = string_in("-p", argv, argc);
	if (tmp > 0) {
		char *p;
		int num;
		errno = 0;
		long conv = strtol(argv[tmp+1], &p, 10);
		if (!(errno != 0 || *p != '\0' || conv > INT_MAX)) {
			num = conv;
			PORT = num;
		}
	}
	tmp = string_in("-s", argv, argc);
  if (tmp > 0) {
    char *p;
    int num;
    errno = 0;
    long conv = strtol(argv[tmp+1], &p, 10);
    if (!(errno != 0 || *p != '\0' || conv > INT_MAX)) {
      num = conv;
      S_TIME = num;
    }
  }
	tmp = string_in("-c", argv, argc);
	if (tmp > 0) {
    char *p;
    int num;
    errno = 0;
    long conv = strtol(argv[tmp+1], &p, 10);
    if (!(errno != 0 || *p != '\0' || conv > INT_MAX)) {
      num = conv;
      COUNT = num;
    }
  }
  tmp = string_in("-h", argv, argc);
  if (tmp > 0) {
   	printf("where you are, there is no help. i'm sorry.\n");
	  exit(0);
  }
}

int * decodePingResponse(char buf[]) {
	static int decoded[2];
	unsigned char a[4];
	a[0] = buf[15];
	a[1] = buf[14];
	a[2] = buf[13];
	a[3] = buf[12];
	unsigned char b[4];
	b[0] = buf[19];
	b[1] = buf[18];
	b[2] = buf[17];
	b[3] = buf[16];
	int users = *(int *)a;
	int maxusers = *(int *)b;
	decoded[0] = users;
	decoded[1] = maxusers;
	return decoded;
}

uint64_t get_posix_clock_time ()
{
    struct timespec ts;
    if (clock_gettime (CLOCK_MONOTONIC, &ts) == 0) return (uint64_t) (ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
    else return 0;
}

void die(char *s)
{
    perror(s);
    exit(1);
}


int main(int argc, char *argv[])
{
    handleArguments(argc, argv);
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN] = MUMBLE_PING;
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    if (inet_aton(argv[1] , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    while(1 && COUNT != 0)
    {
        uint64_t time1 = get_posix_clock_time();
        if (sendto(s, message, 12 , 0 , (struct sockaddr *) &si_other, slen)==-1) {
	        die("sendto()");
        }
        memset(buf,'\0', BUFLEN);
        sleep(timeout);
        if (recvfrom(s, buf, BUFLEN, MSG_DONTWAIT, (struct sockaddr *) &si_other, &slen) == -1) break;
	      uint64_t time2 = get_posix_clock_time()-(1000000*timeout);
	      float ping = ((time2 - time1)/1000.0);
	      int *response = decodePingResponse(buf);
	      printf("Users : %i/%i - %.2f ms\n", *(response + 0), *(response + 1), ping);
	      COUNT--;
        if (COUNT == 0) break;
        sleep(S_TIME);
    }
    close(s);
    return 0;
}
