#ifndef __NET_H__
#define __NET_H__

#include "mongoose.h"

#define MAX_SIZE 1024 * 100

#define GEN_ENUM(X) X,
#define GEN_STR(X) #X,
#define NET_CMDS(X) \
    X(NET_EHLO) \
    X(NET_SCAN) \
    X(NET_LIST) \
    X(NET_CONNECT) \
    X(NET_DISCONNECT) \
    X(NET_RESET)

enum net_cmd_ids {
    NET_CMDS(GEN_ENUM)
};

enum net_conn_states {
    NET_DISCONNECTED,
    NET_FAILURE,
    NET_CONNECTED,
    NET_RECONNECT
};

int is_net_request(struct mg_connection *c);
int callback_net(struct mg_connection *c);
#endif
