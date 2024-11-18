/* TEMPLATE.c.tpl
 * Author: Noah Meyerhans
 */

#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/netlink.h>

#include "ifaddrs.h"

int scope_to_ifname(int scope_id, char *dest) {
    int netlink_socket = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_GENERIC);
    if (netlink_socket < 0) {
	perror("socket");
	exit(EXIT_FAILURE);
    }
    struct ifreq req;
    req.ifr_ifindex = scope_id;
    if(ioctl(netlink_socket, SIOCGIFNAME, &req) < 0) {
	perror("ioctl");
	exit(EXIT_FAILURE);
    }
    strncpy(dest, req.ifr_name, IFNAMSIZ);
    return 0;
}
