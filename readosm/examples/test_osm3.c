/* 
/ test_osm3.c
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
#include <stdlib.h>

#include "readosm.h"

struct osm_helper
{
/* an user defined struct */
    int read_count;
    int stop_limit;
};

static int
eval_abort (struct osm_helper *helper)
{
/* testing the stop limit */
    if (helper->read_count > helper->stop_limit)
	return 1;
    return 0;
}

static int
parse_node (const void *user_data, const readosm_node * node)
{
/* parsing a Node (callback function)*/

    /* casting the USER_DATA pointer to osm_helper */
    struct osm_helper *helper = (struct osm_helper *) user_data;

    helper->read_count++;
    if (eval_abort (helper))
	return READOSM_ABORT;
    printf ("Node#%d\n", helper->read_count);
    return READOSM_OK;
}

static int
parse_way (const void *user_data, const readosm_way * way)
{
/* parsing a Way (callback function)*/

    /* casting the USER_DATA pointer to osm_helper */
    struct osm_helper *helper = (struct osm_helper *) user_data;

    helper->read_count++;
    if (eval_abort (helper))
	return READOSM_ABORT;
    printf ("Way#%d\n", helper->read_count);
    return READOSM_OK;
}

static int
parse_relation (const void *user_data, const readosm_relation * relation)
{
/* parsing a Relation stats (callback function)*/

    /* casting the USER_DATA pointer to osm_helper */
    struct osm_helper *helper = (struct osm_helper *) user_data;

    helper->read_count++;
    if (eval_abort (helper))
	return READOSM_ABORT;
    printf ("Relation#%d\n", helper->read_count);
    return READOSM_OK;
}

int
main (int argc, char *argv[])
{
    const void *osm_handle;
    int ret;
    struct osm_helper helper;

/* initializing the helper struct */
    helper.read_count = 0;
    helper.stop_limit = 0;

    if (argc != 3)
      {
	  fprintf (stderr, "usage: test_osm3 path-to-OSM limit\n");
	  return -1;
      }

/* setting the stop limit */
    helper.stop_limit = atoi (argv[2]);

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
* this task is unbelievably simple
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
	readosm_parse (osm_handle, &helper, parse_node, parse_way,
		       parse_relation);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "PARSE error: %d\n", ret);
	  goto stop;
      }

  stop:
/*
* STEP #3: closing the OSM file
* this will release any internal memory allocation
*/
    readosm_close (osm_handle);
    return 0;
}
