/*
 * dcap.h
 *
 * Copyright (c) 2011, DeepField Networks, Inc. <info@deepfield.net>
 * All rights reserved.
 *
 */

#ifndef __DCAP_H__
#define __DCAP_H__

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <pcap/pcap.h>
#include <event.h>

typedef void (*dcap_handler)(struct timeval *tv, int pkt_len, char *ip_pkt, void *user);

struct dcap {
	char		intf_name[128];		/* Read-only */
	uint32_t	pkts_captured;		/* Read-only */
	void		*user;			/* Read/write */
	uint16_t	sample_rate;		/* Read/write - 0 or 1 to
						   disable sampling */

	/* Private vars */
	pcap_t		*_pcap;
	struct event	_ev_pcap[1];
	dcap_handler	_callback;
};

struct dcap_stat {
	int	ps_valid;	/* pcap stats only valid for live capture. */
	/* pcap stats */
	uint32_t ps_recv;
	uint32_t ps_drop;
	uint32_t ps_ifdrop;

	uint32_t captured;
};


struct dcap * dcap_init_file(char *filename, char *filter,
		dcap_handler callback);
struct dcap * dcap_init_live(char *intf_name, int promisc, char *filter,
		dcap_handler callback);
int dcap_event_set(struct dcap *dcap);
int dcap_get_fd(struct dcap *dcap);
void dcap_loop_all(struct dcap *dcap);
void dcap_close(struct dcap *dcap);

struct dcap_stat * dcap_get_stats(struct dcap *dcap);

#endif /* __DCAP_H__*/

