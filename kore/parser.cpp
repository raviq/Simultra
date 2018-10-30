//
// Parsing the OSM map and populating world object.
//

#include <iostream>
#include <vector>
#include <QString>
#include <QFile>
#include <QResource>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QTemporaryDir>
#include <QFileInfo>
#include <QDataStream>

#include "parser.h"
#include "readosm.h"
#include "osmdata.h"
#include "config.h"
#include "debug.h"
#include "utils.h"

using namespace std;

// Sets the world from the map.

int load_map(const char * osm_map_filename, osm_world_t* world)
{
    int ret;
    const void *osm_handle = NULL;

    QString osm_map = QDir::currentPath() + "/../../../../" + osm_map_filename; // TODO
    const char* osmmap = osm_map.toStdString().c_str();

    if (!file_exists(osmmap) )
    {
        debug("Map file '%s' does not exist!", osmmap);
        ::exit(-1);
    }

    cout << "...." << osm_map.toStdString() << endl;

    // open the OSM file
    if ((ret = readosm_open (osmmap, &osm_handle)) != READOSM_OK)
    {

      debug("Cannot open '%s'.", osm_map_filename);
      readosm_close (osm_handle);
      exit(-1);
    }
    else
    {
        debug("Map file '%s' opened.", osm_map_filename);
    }

    osm_world_t* w = new osm_world_t;

    // Parsing the OSM file
    if ((ret = readosm_parse (osm_handle, (const void *)  w, add_node, add_way, add_relation)) != READOSM_OK)
    {
          debug (".osm PARSE error.\n");
          exit(-1);
    }

    (*world).nodes.swap(w->nodes);
    (*world).ways.swap(w->ways);

    debug("OSM input file successfully parsed.");

    if ((ret = readosm_close (osm_handle)) != READOSM_OK)
    {
        perror ("CLOSE ERROR: %d\n");
        exit(-1);
    }
    else
    {
        debug( "OSM handle closed.");
    }

    delete w;

    return 0;
}

// Calback function handling nodes

int add_node (const void *user_data, const readosm_node * node)
{
    // populating a new node
    osm_node_t* new_node = (osm_node_t *) malloc (sizeof(osm_node_t));

    // members
    new_node->id        = node->id;
    new_node->latitude  = node->latitude;
    new_node->longitude = node->longitude;
    new_node->version   = node->version;
    new_node->changeset = node->changeset;
    new_node->changeset = node->longitude;
    new_node->uid       = node->uid;
    new_node->tag_count = node->tag_count;

    // user
    if (node->user != NULL)
    {
        new_node->user = (char*) malloc(sizeof(char) * (strlen(node->user) + 1) );
        strcpy(new_node->user, node->user);
    }
    else
    {
        new_node->user = NULL;
    }

    // timestamp
    if (node->timestamp != NULL)
    {
        new_node->timestamp = (char*) malloc(sizeof(char) *  (strlen(node->timestamp) + 1) );
        strcpy(new_node->timestamp, node->timestamp);
    }
    else
    {
        new_node->timestamp = NULL;
    }

    // populating tags
    if (new_node->tag_count == 0)
    {
       new_node->tags = NULL;
    }
    else
    {
       new_node->tags = (osm_tag_t*) malloc(new_node->tag_count * sizeof(osm_tag_t));

       for (int i = 0 ; i < new_node->tag_count ; i++)
        {
            new_node->tags[i].key   = (char*) malloc (sizeof(char) * (strlen(node->tags[i].key) + 1)  ) ;
            new_node->tags[i].value = (char*) malloc (sizeof(char) * (strlen(node->tags[i].value) + 1));
            strcpy(new_node->tags[i].key, node->tags[i].key);
            strcpy(new_node->tags[i].value, node->tags[i].value);         
        }
    }

    // Finally, adding
    ((osm_world_t *) user_data)->nodes.push_back(*new_node);
    free(new_node);
    return READOSM_OK;
}

// Calback function handling ways

int add_way (const void *user_data, const readosm_way * way)
{
    // Populating a new way
    osm_way_t* my_way = (osm_way_t*) malloc(sizeof(osm_way_t));

    // vars
    my_way->id = way->id;
    my_way->version = way->version;
    my_way->changeset = way->changeset;
    my_way->uid = way->uid;
    my_way->node_ref_count = way->node_ref_count;
    my_way->tag_count = way->tag_count;

    if (way->node_ref_count == 0)
    {
         my_way->node_refs = NULL;
    }
    else
    {
        my_way->node_refs = (long long*) malloc( way->node_ref_count * sizeof(long long));
        for (int i = 0; i < way->node_ref_count; i++)
            my_way->node_refs[i] = way->node_refs[i];
    }

    // user
    if (way->user != NULL)
    {
        my_way->user = (char*) malloc(sizeof(char) * (strlen(way->user)+1) );
        strcpy(my_way->user, way->user);
    }
    else
    {
        my_way->user = NULL;
    }
    //timestamp
    if (way->timestamp != NULL)
    {
        my_way->timestamp = (char*) malloc(sizeof(char) * (strlen(way->timestamp)+1) );
        strcpy(my_way->timestamp, way->timestamp);
    }
    else
    {
        my_way->timestamp = NULL;
    }

    //tags
    if (my_way->tag_count == 0)
    {
       my_way->tags = NULL;
    }
    else
    {
        my_way->tags = (osm_tag_t*) malloc(sizeof(osm_tag_t) *way->tag_count );

        for (int i = 0 ; i < my_way->tag_count ; i++)
        {
            my_way->tags[i].key   = (char*) malloc ( sizeof(char) * (strlen(way->tags[i].key) + 1)) ;
            my_way->tags[i].value = (char*) malloc ( sizeof(char) * (strlen(way->tags[i].value) + 1) );
            strcpy(my_way->tags[i].key, way->tags[i].key);
            strcpy(my_way->tags[i].value, way->tags[i].value);
        }
    }

    ((osm_world_t *) user_data)->ways.push_back(*my_way);
    free(my_way);

    return READOSM_OK;
}

// Calback function handling relations

int add_relation (const void *user_data, const readosm_relation * relation)
{
    UNUSED(user_data);

    osm_relation_t* r = (osm_relation_t*) malloc(sizeof(osm_relation_t));

    r->id = relation->id;
    r->version = relation->version;
    r->changeset = relation->changeset;
    r->uid = relation->uid;
    r->member_count = relation->member_count;
    r->tag_count = relation->tag_count;

    // user
    if (relation->user != NULL)
    {
        r->user = (char*) malloc(sizeof(char) * (strlen(relation->user) + 1));
        strcpy(r->user, relation->user);
    }
    else
    {
        r->user = NULL;
    }

    //timestamp
    if (relation->timestamp != NULL)
    {
        r->timestamp = (char*) malloc(sizeof(char) * (strlen(relation->timestamp)+1));
        strcpy(r->timestamp, relation->timestamp);
    }
    else
    {
        r->timestamp = NULL;
    }


    //tags
    if (relation->tag_count == 0)
    {
       r->tags = NULL;
    }
    else
    {
        r->tags = (osm_tag_t*) malloc(sizeof(osm_tag_t) * relation->tag_count);

        for (int i = 0 ; i < r->tag_count ; i++)
        {
            r->tags[i].key   = (char*) malloc ( sizeof(char) * (strlen(relation->tags[i].key) + 1)  ) ;
            r->tags[i].value = (char*) malloc ( sizeof(char) * (strlen(relation->tags[i].value) + 1)   );
            strcpy(r->tags[i].key, relation->tags[i].key);
            strcpy(r->tags[i].value, relation->tags[i].value);
        }
    }

    // members
    if (relation->members == 0)
    {
       r->members = NULL;
    }
    else
    {
        r->members = (osm_member_t*) malloc(sizeof(osm_member_t) * r->member_count);

        for (int i = 0 ; i < r->member_count ; i++)
        {
            r->members[i].id   = relation->members[i].id;
            r->members[i].member_type   = relation->members[i].member_type;
            r->members[i].role   = (char*) malloc ( sizeof(char) * (strlen(relation->members[i].role) + 1)  ) ;
            strcpy(r->members[i].role, relation->members[i].role);
        }
    }

    free(r);

    return READOSM_OK;
}
// End.
