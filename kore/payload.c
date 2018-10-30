/*
 *  Mobility payloads for the agents
 */

#include <stdio.h>
#include <stdlib.h>
#include "json.h"
#include<string.h>

#include "payload.h"
#include "debug.h"
	
/*
 * input: payload as string.
 * output: parse the payload string to JSON, then render back to text, and print.
 */

void parse_payload(char *text, payload_t* payload)
{
	char* out;
        cJSON *json=cJSON_Parse(text);
	int i;
	
	if (!json)
	{
        debug("Error before: <%s>\n",cJSON_GetErrorPtr());
	}
	else
	{		
		cJSON *item = json->child;

		for ( i = 0 ; i < cJSON_GetArraySize(item) ; i++ )
		{
			cJSON* subitem = cJSON_GetArrayItem(item, i);

            /* Populating variables */
			if (!strcmp(subitem->string, "Precision"))
			{
				out = cJSON_Print(subitem);
				payload->precision = malloc(strlen(out) + 1);
				sscanf(out, "%s", payload->precision);
			}
			else if (!strcmp(subitem->string, "Round"))
			{
				out = cJSON_Print(subitem);
				sscanf(out, "%d", &payload->round);
			}
			else if (!strcmp(subitem->string, "Latitude"))
			{
				out = cJSON_Print(subitem);
				sscanf(out, "%lf", &payload->lat);
			}
			else if (!strcmp(subitem->string, "Longitude"))
			{
				out = cJSON_Print(subitem);
				sscanf(out, "%lf", &payload->lng);
			}
			else if (!strcmp(subitem->string, "Name"))
			{
				out = cJSON_Print(subitem);
				payload->name = malloc(strlen(out) + 1);
				sscanf(out, "%s", payload->name);
			}
			else if (!strcmp(subitem->string, "Command"))
			{
				out = cJSON_Print(subitem);
				sscanf(out, "%d", &payload->command);
			}
			else if (!strcmp(subitem->string, "Data"))
			{
				out = cJSON_Print(subitem);
				payload->data = malloc(strlen(out) + 1);
				sscanf(out, "%s", payload->data);
			}
                /* JSONDATA add here ... */
		}		
	/*
        debug(" Precision = %s\n Round = %d\n Lat = %lf\n Lng = %lf\n Name = %s\n Command = %s\n Data = %s\n",
		       payload->precision,
		       payload->round,
		       payload->lat,
		       payload->lng,
		       payload->name,
		       payload->command,
		       payload->data
		       );
	 */
		
		free(out);
		cJSON_Delete(json);
	}
}

/*
 * input: payload structure
 * output: payload as string
 */

void set_payload(payload_t payload, char** result)
{

	cJSON *root, *fld;
	
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "Mobility", fld=cJSON_CreateObject());
    cJSON_AddStringToObject(fld, "Precision", payload.precision);
    cJSON_AddNumberToObject(fld, "Round", payload.round);
    cJSON_AddNumberToObject(fld, "Latitude", payload.lat);
    cJSON_AddNumberToObject(fld, "Longitude", payload.lng);
    cJSON_AddStringToObject(fld, "Name", payload.name);
    cJSON_AddNumberToObject(fld, "Command", payload.command);
    cJSON_AddStringToObject(fld, "Data", payload.data);
	
    /* JSONDATA add here ... */
	
    char* out = cJSON_Print(root);
    if ( (*result = malloc(sizeof(char) * (strlen(out) + 1))) != NULL )
    {
        strcpy(*result, out);
    }
    else
    {
        debug ("Cannot allocate *result !\n");
    }

    cJSON_Delete(root);
    free(out);
}

/*
 * input: latitude, longitude
 * output: payload as string
 */

void set_payload_latlon(payload_t payload, char **result, double lat, double lon, int round )
{
	cJSON *root, *fld;

    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "Mobility", fld=cJSON_CreateObject());
    cJSON_AddStringToObject(fld, "Precision", payload.precision);
    cJSON_AddNumberToObject(fld, "Round", round);
    cJSON_AddNumberToObject(fld, "Latitude", lat);
    cJSON_AddNumberToObject(fld, "Longitude", lon);
    cJSON_AddStringToObject(fld, "Name", payload.name);
    cJSON_AddNumberToObject(fld, "Command", payload.command);
    cJSON_AddStringToObject(fld, "Data", payload.data);
	
    /* JSONDATA add here ... */
	
    char *out = cJSON_Print(root);
	
	if ( (*result = malloc(sizeof(char) * (strlen(out) + 1))) != NULL )
    {
        strcpy(*result, out);
    }
    else
    {
        debug ("Cannot allocate *result !\n");
    }

    cJSON_Delete(root);
	free(out);
}

int payload_test ()
{
    payload_t payload = {"precType", 1, 37.371, -1.220, "", MOVING, "myCar"};
    char* mobility = NULL;
    set_payload(payload, &mobility);
    payload_t new_payload = {"precType", 3, 37.371, -1.220, "", MOVING, "myCar2"};
    parse_payload(mobility, &new_payload);
	return 0;
}
