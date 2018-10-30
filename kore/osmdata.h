//
// OSM data definitions.
//

#ifndef OSMDATA_H
#define OSMDATA_H

#include <vector>

using namespace std;

// Nodes

typedef struct osm_tag_struct
{
    char *key;
    char *value;
} osm_tag_t;

typedef struct osm_node_struct
{
    long long id;           // NODE-ID (expected to be a unique value)
    double latitude; 		// geographic latitude
    double longitude;		// geographic longitude
    int version;            // object version
    long long changeset; 	// ChangeSet ID
    char *user;             // name of the User defining this NODE
    int uid;                // corresponding numeric UserID
    char *timestamp; 		// when this NODE was defined
    int tag_count;          // number of associated TAGs (may be zero)
    osm_tag_t *tags; 		// array of TAG objects (may be NULL)

} osm_node_t;

// Relations

typedef struct osm_member_struct
{
    int member_type; 	// can be one of: READOSM_MEMBER_NODE, READOSM_MEMBER_WAY or READOSM_MEMBER_RELATION
    long long id;		// ID-value identifying the referenced object
    char *role; 		// intended role for this reference
} osm_member_t;

typedef struct osm_relation_struct
{
    long long id;       	// RELATION-ID (expected to be a unique value)
    int version;        	// object version
    long long changeset; 	// ChangeSet ID
    char *user;             // name of the User defining this RELATION
    int uid;                // corresponding numeric UserID
    char *timestamp; 		// when this RELATION was defined
    int member_count;		// number of associated MEMBERs (may be zero)
    osm_member_t *members;  // array of MEMBER objects (may be NULL)
    int tag_count;          // number of associated TAGs (may be zero)
    osm_tag_t *tags;		// array of TAG objects (may be NULL)
} osm_relation_t;

// Ways

typedef struct osm_way_struct
{
    long long id;           // WAY-ID (expected to be a unique value)
    int version;            // object version
    long long changeset; 	// ChangeSet ID
    char *user;             // name of the User defining this WAY
    int uid;                // corresponding numeric UserID
    char *timestamp; 		// when this WAY was defined
    int node_ref_count; 	// number of referenced NODE-IDs (may be zero)
    long long *node_refs; 	// array of NODE-IDs (may be NULL)
    int tag_count;          // number of associated TAGs (may be zero)
    osm_tag_t *tags; 		// array of TAG objects (may be NULL)
} osm_way_t;

// World

typedef struct osm_world_struct
{
    vector<osm_node_t>      nodes;
    vector<osm_way_t>       ways;
    vector<osm_relation_t>  relations;

} osm_world_t;

#endif // OSMDATA_H
