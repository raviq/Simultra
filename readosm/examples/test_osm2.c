/* 
/ test_osm2.c
/
/ libreadosm Sample code
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ ------------------------------------------------------------------------------
/ 
/ Version: MPL 1.1/GPL 2.0/LGPL 2.1
/ 
/ The contents of this file are subject to the Mozilla Public License Version
/ 1.1 (the "License"); you may not use this file except in compliance with
/ the License. You may obtain a copy of the License at
/ http://www.mozilla.org/MPL/
/ 
/ Software distributed under the License is distributed on an "AS IS" basis,
/ WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
/ for the specific language governing rights and limitations under the
/ License.
/
/ The Original Code is the ReadOSM library
/
/ The Initial Developer of the Original Code is Alessandro Furieri
/ 
/ Portions created by the Initial Developer are Copyright (C) 2012
/ the Initial Developer. All Rights Reserved.
/ 
/ Contributor(s):
/ 
/ Alternatively, the contents of this file may be used under the terms of
/ either the GNU General Public License Version 2 or later (the "GPL"), or
/ the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
/ in which case the provisions of the GPL or the LGPL are applicable instead
/ of those above. If you wish to allow use of your version of this file only
/ under the terms of either the GPL or the LGPL, and not to allow others to
/ use your version of this file under the terms of the MPL, indicate your
/ decision by deleting the provisions above and replace them with the notice
/ and other provisions required by the GPL or the LGPL. If you do not delete
/ the provisions above, a recipient may use your version of this file under
/ the terms of any one of the MPL, the GPL or the LGPL.
/ 
*/

#include <stdio.h>

#include "readosm.h"

struct osm_statistics
{
/* a struct intended to collect general OSM statistics */
    int node_count;
    int node_tag_count;
    int way_count;
    int way_ndref_count;
    int way_tag_count;
    int relation_count;
    int relation_member_node_count;
    int relation_member_way_count;
    int relation_member_relation_count;
    int relation_tag_count;
    double min_longitude;
    double max_longitude;
    double min_latitude;
    double max_latitude;
};

static int
node_stats (const void *user_data, const readosm_node * node)
{
/* updating OSM Node stats (callback function)*/

    /* casting the USER_DATA pointer to osm_statistics */
    struct osm_statistics *stats = (struct osm_statistics *) user_data;

    stats->node_count++;
    stats->node_tag_count += node->tag_count;
    if (node->latitude != READOSM_UNDEFINED)
      {
	  if (node->latitude > stats->max_latitude)
	      stats->max_latitude = node->latitude;
	  if (node->latitude < stats->min_latitude)
	      stats->min_latitude = node->latitude;
      }
    if (node->longitude != READOSM_UNDEFINED)
      {
	  if (node->longitude > stats->max_longitude)
	      stats->max_longitude = node->longitude;
	  if (node->longitude < stats->min_longitude)
	      stats->min_longitude = node->longitude;
      }
    return READOSM_OK;
}

static int
way_stats (const void *user_data, const readosm_way * way)
{
/* updating OSM Way stats (callback function)*/

    /* casting the USER_DATA pointer to osm_statistics */
    struct osm_statistics *stats = (struct osm_statistics *) user_data;

    stats->way_count++;
    stats->way_ndref_count += way->node_ref_count;
    stats->way_tag_count += way->tag_count;
    return READOSM_OK;
}

static int
relation_stats (const void *user_data, const readosm_relation * relation)
{
/* updating OSM Relation stats (callback function)*/
    int i;
    const readosm_member *member;

    /* casting the USER_DATA pointer to osm_statistics */
    struct osm_statistics *stats = (struct osm_statistics *) user_data;

    stats->relation_count++;
    for (i = 0; i < relation->member_count; i++)
      {
	  member = relation->members + i;
	  switch (member->member_type)
	    {
	    case READOSM_MEMBER_NODE:
		stats->relation_member_node_count++;
		break;
	    case READOSM_MEMBER_WAY:
		stats->relation_member_way_count++;
		break;
	    case READOSM_MEMBER_RELATION:
		stats->relation_member_relation_count++;
		break;
	    };
      }
    stats->relation_tag_count += relation->tag_count;
    return READOSM_OK;
}

int
main (int argc, char *argv[])
{
    const void *osm_handle;
    int ret;
    struct osm_statistics infos;

/* initializing the statistics struct */
    infos.node_count = 0;
    infos.node_tag_count = 0;
    infos.way_count = 0;
    infos.way_ndref_count = 0;
    infos.way_tag_count = 0;
    infos.relation_count = 0;
    infos.relation_member_node_count = 0;
    infos.relation_member_way_count = 0;
    infos.relation_member_relation_count = 0;
    infos.relation_tag_count = 0;
    infos.min_longitude = 180.0;
    infos.max_longitude = -180.0;
    infos.min_latitude = 90.0;
    infos.max_latitude = -90.0;

    if (argc != 2)
      {
	  fprintf (stderr, "usage: test_osm2 path-to-OSM-file\n");
	  return -1;
      }

/*
* STEP #1: opening the OSM file
* this can indifferently be an OSM XML encoded file (.osm)
* or an OSM Protocol Buffer encoded file (.pbf)
* the library will transparently perform any required
* action in both cases.
*/
    ret = readosm_open (argv[1], &osm_handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "OPEN error: %d\n", ret);
	  goto stop;
      }

/*
* STEP #2: parsing the OSM file
* this task is unbelievebly simple
*
* you are simply required to pass the appropriate
* pointers for callback funtions respectively intended
* to process Node-objects, Way-objects and Relation-objects
*
* the library will then parse the whole input file, calling
* the appropriate callback handling function for each OSM object
* found: please see the callback functions implementing code
* to better understand how it works
*
* important notice: this second example is mainly focused on
* using the USER_DATA pointer. in this example we'll pass the 
* address of the osm_statistics struct so to gather some
* general infos.
*/
    ret =
	readosm_parse (osm_handle, &infos, node_stats, way_stats,
		       relation_stats);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "PARSE error: %d\n", ret);
	  goto stop;
      }

/* printing OSM statistics */
    printf ("Longitude    range: %1.7f / %1.7f\n", infos.min_longitude,
	    infos.max_longitude);
    printf ("Latitude     range: %1.7f / %1.7f\n\n", infos.min_latitude,
	    infos.max_latitude);
    printf ("Nodes             : %d\n", infos.node_count);
    printf ("              tags: %d\n\n", infos.node_tag_count);
    printf ("Ways              : %d\n", infos.way_count);
    printf ("             ndref: %d\n", infos.way_ndref_count);
    printf ("              tags: %d\n\n", infos.way_tag_count);
    printf ("Relations         : %d\n", infos.relation_count);
    printf ("  member.nodes    : %d\n", infos.relation_member_node_count);
    printf ("  member.ways     : %d\n", infos.relation_member_way_count);
    printf ("  member.relations: %d\n", infos.relation_member_relation_count);
    printf ("              tags: %d\n", infos.relation_tag_count);

  stop:
/*
* STEP #3: closing the OSM file
* this will release any internal memory allocation
*/
    readosm_close (osm_handle);
    return 0;
}
