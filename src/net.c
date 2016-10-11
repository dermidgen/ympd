
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>

#include "net.h"
#include "config.h"
#include "json_encode.h"

const char * net_cmd_strs[] = {
    NET_CMDS(GEN_STR)
};


static inline enum net_cmd_ids get_cmd_id(char *cmd)
{
    for(int i = 0; i < sizeof(net_cmd_strs)/sizeof(net_cmd_strs[0]); i++)
        if(!strncmp(cmd, net_cmd_strs[i], strlen(net_cmd_strs[i])))
            return i;

    return -1;
}

int is_net_request(struct mg_connection *c)
{
    enum net_cmd_ids cmd_id = get_cmd_id(c->content);
    
    if (cmd_id == -1)
        return MG_FALSE;
    else
        return MG_TRUE;
}

int callback_net(struct mg_connection *c)
{
    enum net_cmd_ids cmd_id = get_cmd_id(c->content);
    size_t n = 0;

    FILE *fp;
    char wsres[10000];
    char resbuf[10000];
    char resbuf_cat[10000];

    if(cmd_id == -1)
        return MG_TRUE;

    // printf("Entering callback_net:cmd_switch %i\n", cmd_id);
    switch(cmd_id)
    {
        case NET_EHLO:
            fp = popen("HELO", "r");
            break;
        case NET_SCAN:
            fp = popen("iw dev wlan0 scan ap-force", "r");
            break;
        case NET_LIST:
            fp = popen("echo \"[`iw dev wlan0 scan ap-force | grep SSID | cut -d ' ' -f 2 | sed -e 's/\\(.*\\)/\"\\1\"/' | tr \"\n\" \",\" | sed 's/,$//'`]\"", "r");
            break;
        case NET_CONNECT:
            fp = popen("connmanctl", "r");
            break;
        case NET_DISCONNECT:
            fp = popen("connmanctl", "r");
            break;
        case NET_RESET:
            fp = popen("connmanctl", "r");
            break;
    }

    // if(fp == NULL) {
    //     resbuf = "Command Failed";
    // }

    resbuf[0] = 0;
    while(fgets(resbuf_cat, 1024, fp) != NULL) {
        strcat(resbuf,resbuf_cat);
    }

    // /* close */
    pclose(fp);

    // printf("Response %s\n", resbuf);
    n = snprintf(wsres, MAX_SIZE, "{\"type\":\"net\", \"data\": \"%s\"}", resbuf);
    mg_websocket_write(c, 1, wsres, n);

    return MG_TRUE;
}
