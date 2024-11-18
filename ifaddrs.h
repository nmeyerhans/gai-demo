/* TEMPLATE.h.tpl
 * Author: Noah Meyerhans
 */

#ifndef _IFADDRS_H
#define _IFADDRS_H 1

#include <sys/types.h>
#include <ifaddrs.h>

int scope_to_ifname(int scope_id, char *dest);

#endif /* _IFADDRS_H */

