/* TEMPLATE.c.tpl
 * Author: Noah Meyerhans
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char* argv[]) {
    char hostname[128];
    char formatted_addr[128];

    if(argc >= 2) {
	strncpy(hostname, argv[1], 128);
    } else {
	exit(1);
    }

    struct addrinfo  hints;
    struct addrinfo  *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    int s = getaddrinfo(hostname, NULL, &hints, &result);
    if (s != 0) {
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
	exit(EXIT_FAILURE);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
	switch(rp->ai_family) {
	case AF_INET:
	    printf("Got IPv4 address ");
	    struct sockaddr* x = rp->ai_addr;
	    struct in_addr src = ((struct sockaddr_in*)x)->sin_addr;
	    if(inet_ntop(rp->ai_family, &src, formatted_addr, rp->ai_addrlen)) {
		printf("%s\n", formatted_addr);
	    } else {
		printf("Ohno\n");
	    }
	    break;
	case AF_INET6:
	    printf("Got IPv6 address ");
	    struct sockaddr* a = rp->ai_addr;
	    struct in6_addr src6 = ((struct sockaddr_in6*)a)->sin6_addr;
	    if(inet_ntop(rp->ai_family, &src6, formatted_addr, rp->ai_addrlen)) {
		printf("%s\n", formatted_addr);
	    } else {
		printf("Ohno\n");
	    }
	    break;
	}
    }
    freeaddrinfo(result);
}


