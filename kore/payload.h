
#ifndef PAYLOAD_H
#define PAYLOAD_H

#include "atypes.h"

typedef struct payload
{
        char *precision;
        int round;
		double lat, lng;
		char *name;
		command_t command;
		char *data; 
        // TODO entry for JSONDATA add here.
} payload_t;
        
void parse_payload(char *text, payload_t* payload);
void set_payload(payload_t payload, char** result);
void set_payload_latlon(payload_t payload, char **result, double lat, double lon, int round );

#endif
