/* NAT table implementation in C
 * Uma Shankar - ushankar@usc.edu
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#define MAXLINEBUF 100

typedef struct natentry
{
	uint32_t int_ip;
	uint16_t int_port;
	int int_flag;
	uint32_t ext_ip;
	uint16_t ext_port;
}natentry;

typedef struct natmap
{
	struct natentry newnatentry;
	struct natmap* left;
	struct natmap* right;
}natmap;
