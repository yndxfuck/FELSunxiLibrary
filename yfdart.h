#ifndef __YFDART_H
#define __YFDART_H

#include <dart_api_dl.h>
#include <stdlib.h>

enum yfdart_packet_type {
    YFDART_PACKET_HELLO = 1, // The first packet to mark communication 
                             // as successful
    YFDART_PACKET_DATA, // Packet with user data
    YFDART_PACKET_END, // Packet which marks the end of communication
                       // and gives communication exit code
};

struct yfdart_packet {
    enum yfdart_packet_type type;
    void *data; // Hello: NULL
                // Data: NULL or pointer to data structure/type
                // End: pointer to int32_t
};

struct yfdart_connection {
    Dart_Port_DL port_id; // Dart DL port ID
    bool ready; // Ready to act
};

// FFI
intptr_t yfdart_init_dart_api(void *data);
int yfdart_attach_port(struct yfdart_connection *c, Dart_Port_DL port);

// C
int yfdart_start_connection(struct yfdart_connection *c);
int yfdart_connection_post(struct yfdart_connection *c, void *data);
int yfdart_end_connection(struct yfdart_connection **cp, int32_t end_code);

#endif /* __YFDART_H */
