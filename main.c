/* TEMPLATE.c.tpl
 * Author: Noah Meyerhans
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <net/if.h>

#include "ifaddrs.h"

int main(int argc, char* argv[]) {
    char hostname[128];
    char formatted_addr[128];
    char ifname[IFNAMSIZ];
    int ch;

    struct addrinfo  hints;
    struct addrinfo  *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    while ((ch = getopt(argc, argv, "64")) != EOF) {
	switch(ch) {
	case '4':
	    if (hints.ai_family == AF_INET6)
		fprintf(stderr, "only one -4 or -6 option may be specified");
	    hints.ai_family = AF_INET;
	    break;
	case '6':
	    if (hints.ai_family == AF_INET)
		fprintf(stderr, "only one -4 or -6 option may be specified");
	    hints.ai_family = AF_INET6;
	    break;
	default:
	    fprintf(stderr, "Usage error\n");
	    exit(EXIT_FAILURE);
	}
    }

    argc -= optind;
    argv += optind;

    if(argc == 1) {
	strncpy(hostname, argv[0], 128);
    } else {
	fprintf(stderr, "argc is %d\n", argc);
	exit(1);
    }

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
		int scope_id = ((struct sockaddr_in6*)a)->sin6_scope_id;
		printf("%s", formatted_addr);
		if(scope_id > 0) {
		    scope_to_ifname(scope_id, ifname);
		    printf("%%%s\n", ifname);
		} else {
		    printf("\n");
		}
	    } else {
		printf("Ohno\n");
	    }
	    break;
	}
    }
    freeaddrinfo(result);
}
