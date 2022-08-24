/* Copyright (c) 2022, The YNDXFuck project */

#include "yfdart.h"
#include "yflog.h"
#include <stdio.h>

/*
 * Dart DL API wrappers
 */

intptr_t yfdart_init_dart_api(void *data) {
    LOG_clean();
    return Dart_InitializeApiDL(data);
}

static int send_packet_to_dart(struct yfdart_packet *p, Dart_Port_DL port)
{
    if (!port) {
        LOG("port is NULL\n");
        return -1;
    }

    if (!p->type || (p->type == YFDART_PACKET_END && p->data == NULL)) {
        LOG("invalid packet\n");
        return -2;
    }

    Dart_CObject obj;
    obj.type = Dart_CObject_kInt64;
    obj.value.as_int64 = (uint64_t)p;
    Dart_PostCObject_DL(port, &obj);
    return 0;
}

int yfdart_attach_port(struct yfdart_connection *c, Dart_Port_DL port) {
    if (!c) {
        LOG("c is NULL\n");
        return -1;
    }

    if (c->port_id == port) {
        LOG("c has got same port already\n");
        return -2;
    }

    c->port_id = port;
    c->ready = true;
    return 0;
}

int yfdart_start_connection(struct yfdart_connection *c) {
    if (!c) {
        LOG("c is NULL\n");
        return -1;
    }

    if (!c->ready || !c->port_id) {
        LOG("c not ready\n");
        return -1;
    }

    struct yfdart_packet *p = malloc(sizeof(struct yfdart_packet)); 
    p->type = YFDART_PACKET_HELLO;
    p->data = "placeholder";

    return send_packet_to_dart(p, c->port_id);
}

int yfdart_connection_post(struct yfdart_connection *c, void *data) {
    if (!c) {
        LOG("c is NULL\n");
        return -1;
    }

    if (!c->ready || !c->port_id) {
        LOG("c not ready\n");
        return -1;
    }

    struct yfdart_packet p = {
        YFDART_PACKET_DATA,
        data,
    };

    return send_packet_to_dart(&p, c->port_id);
}

int yfdart_end_connection(struct yfdart_connection **cp, int32_t end_code) {
    struct yfdart_connection *c = *cp;

    if (!c) {
        LOG("c is NULL\n");
        return -1;
    }

    if (!c->ready || !c->port_id) {
        LOG("c not ready\n");
        return -1;
    }

    int32_t *end_code_p = malloc(sizeof(int32_t));
    if (!end_code_p)
        return -3;
    *end_code_p = end_code;

    struct yfdart_packet p = {
        YFDART_PACKET_END,
        end_code_p,
    };

    int ret = send_packet_to_dart(&p, c->port_id);
    free(*cp);
    return ret;
}
