#ifndef PARSER_H
#define PARSER_H

#include "readosm.h"
#include "osmdata.h"
#include <vector>
#include <QString>

using namespace std;

int add_node (const void *user_data, const readosm_node * node);
int add_way (const void *user_data, const readosm_way * way);
int add_relation (const void *user_data, const readosm_relation * relation);
int load_map(const char *osm_map_filename, osm_world_t* world);
void setWorld(osm_world_t* w);

#endif // PARSER_H
