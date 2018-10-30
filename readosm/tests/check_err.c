/* 
/ check_err.c
/
/ Test cases for BAD file
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
#include <memory.h>

#include "readosm.h"

struct osm_count
{
    int nodes;
    int nd_tags;
    int ways;
    int way_nds;
    int way_tags;
    int relations;
    int rel_members;
    int rel_tags;
};

static int
zero_count (struct osm_count *cnt)
{
/* resetting the osm_count struct */
    cnt->nodes = 0;
    cnt->nd_tags = 0;
    cnt->ways = 0;
    cnt->way_nds = 0;
    cnt->way_tags = 0;
    cnt->relations = 0;
    cnt->rel_members = 0;
    cnt->rel_tags = 0;
}

static int
parse_node (const void *user_data, const readosm_node * node)
{
/* Node callback function */
    struct osm_count *cnt = (struct osm_count *) user_data;
    cnt->nodes++;
    cnt->nd_tags += node->tag_count;
    return READOSM_OK;
}

static int
parse_way (const void *user_data, const readosm_way * way)
{
/* Way callback function */
    struct osm_count *cnt = (struct osm_count *) user_data;
    cnt->ways++;
    cnt->way_nds += way->node_ref_count;
    cnt->way_tags += way->tag_count;
    return READOSM_OK;
}

static int
parse_relation (const void *user_data, const readosm_relation * relation)
{
/* Relation callback function */
    struct osm_count *cnt = (struct osm_count *) user_data;
    cnt->relations++;
    cnt->rel_members += relation->member_count;
    cnt->rel_tags += relation->tag_count;
    return READOSM_OK;
}

int
main (int argc, char *argv[])
{
    const void *handle;
    int ret;
    struct osm_count count;
    char buffer[128];
    memset (buffer, '\0', 128);

    ret = readosm_open ("testdata/not-exists.osm", &handle);
    readosm_close (handle);
    if (ret != READOSM_FILE_NOT_FOUND)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_FILE_NOT_FOUND, ret);
	  return -1;
      }

    ret = readosm_open ("testdata/test.foo", &handle);
    readosm_close (handle);
    if (ret != READOSM_INVALID_SUFFIX)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_INVALID_SUFFIX, ret);
	  return -2;
      }

    ret = readosm_open ("testdata/test.osm", &handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "OPEN ERROR: %d\n", ret);
	  return -3;
      }

    ret = readosm_close (NULL);
    if (ret != READOSM_NULL_HANDLE)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_NULL_HANDLE, ret);
	  return -4;
      }

    ret = readosm_close (buffer);
    if (ret != READOSM_INVALID_HANDLE)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_INVALID_HANDLE, ret);
	  return -5;
      }

    ret = readosm_parse (NULL, NULL, NULL, NULL, NULL);
    if (ret != READOSM_NULL_HANDLE)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_NULL_HANDLE, ret);
	  return -6;
      }

    ret = readosm_parse (buffer, NULL, NULL, NULL, NULL);
    if (ret != READOSM_INVALID_HANDLE)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_INVALID_HANDLE, ret);
	  return -7;
      }

    zero_count (&count);
    ret = readosm_parse (handle, &count, parse_node, parse_way, parse_relation);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_OK, ret);
	  return -8;
      }
    if (count.nodes != 1060 || count.nd_tags != 1052 || count.ways != 112
	|| count.way_nds != 785 || count.way_tags != 241
	|| count.relations != 13 || count.rel_members != 66
	|| count.rel_tags != 199)
      {
	  fprintf (stderr,
		   "XML-ALL: unexpected results: expected 1060/1052/112/785/241/13/66/19, found %d/%d/%d/%d/%d/%d/%d/%d\n",
		   count.nodes, count.nd_tags, count.ways, count.way_nds,
		   count.way_tags, count.relations, count.rel_members,
		   count.rel_tags);
	  return -9;
      }

    ret = readosm_close (handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "CLOSE ERROR: %d\n", ret);
	  return -10;
      }

    ret = readosm_open ("testdata/test.osm", &handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "OPEN ERROR: %d\n", ret);
	  return -11;
      }

    zero_count (&count);
    ret = readosm_parse (handle, &count, parse_node, NULL, NULL);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_OK, ret);
	  return -12;
      }
    if (count.nodes != 1060 || count.nd_tags != 1052 || count.ways != 0
	|| count.way_nds != 0 || count.way_tags != 0 || count.relations != 0
	|| count.rel_members != 0 || count.rel_tags != 0)
      {
	  fprintf (stderr,
		   "XML-NODE: unexpected results: expected 1060/1052/0/0/0/0/0/0, found %d/%d/%d/%d/%d/%d/%d/%d\n",
		   count.nodes, count.nd_tags, count.ways, count.way_nds,
		   count.way_tags, count.relations, count.rel_members,
		   count.rel_tags);
	  return -13;
      }

    ret = readosm_close (handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "CLOSE ERROR: %d\n", ret);
	  return -14;
      }

    ret = readosm_open ("testdata/test.osm", &handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "OPEN ERROR: %d\n", ret);
	  return -15;
      }

    zero_count (&count);
    ret = readosm_parse (handle, &count, NULL, parse_way, NULL);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_OK, ret);
	  return -16;
      }
    if (count.nodes != 0 || count.nd_tags != 0 || count.ways != 112
	|| count.way_nds != 785 || count.way_tags != 241 || count.relations != 0
	|| count.rel_members != 0 || count.rel_tags != 0)
      {
	  fprintf (stderr,
		   "XML-WAY: unexpected results: expected 0/0/113/785/241/0/0/0, found %d/%d/%d/%d/%d/%d/%d/%d\n",
		   count.nodes, count.nd_tags, count.ways, count.way_nds,
		   count.way_tags, count.relations, count.rel_members,
		   count.rel_tags);
	  return -17;
      }

    ret = readosm_close (handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "CLOSE ERROR: %d\n", ret);
	  return -18;
      }

    ret = readosm_open ("testdata/test.osm", &handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "OPEN ERROR: %d\n", ret);
	  return -19;
      }

    zero_count (&count);
    ret = readosm_parse (handle, &count, NULL, NULL, parse_relation);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_OK, ret);
	  return -20;
      }
    if (count.nodes != 0 || count.nd_tags != 0 || count.ways != 0
	|| count.way_nds != 0 || count.way_tags != 0 || count.relations != 13
	|| count.rel_members != 66 || count.rel_tags != 199)
      {
	  fprintf (stderr,
		   "XML-RELATION: unexpected results: expected 0/0/0/0/0/13/66/19, found %d/%d/%d/%d/%d/%d/%d/%d\n",
		   count.nodes, count.nd_tags, count.ways, count.way_nds,
		   count.way_tags, count.relations, count.rel_members,
		   count.rel_tags);
	  return -21;
      }

    ret = readosm_close (handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "CLOSE ERROR: %d\n", ret);
	  return -22;
      }

    ret = readosm_open ("testdata/test.osm", &handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "OPEN ERROR: %d\n", ret);
	  return -23;
      }

    zero_count (&count);
    ret = readosm_parse (handle, &count, NULL, NULL, NULL);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_OK, ret);
	  return -24;
      }
    if (count.nodes != 0 || count.nd_tags != 0 || count.ways != 0
	|| count.way_nds != 0 || count.way_tags != 0 || count.relations != 0
	|| count.rel_members != 0 || count.rel_tags != 0)
      {
	  fprintf (stderr,
		   "XML-NONE: unexpected results: expected 0/0/0/0/0/0/0/0, found %d/%d/%d/%d/%d/%d/%d/%d\n",
		   count.nodes, count.nd_tags, count.ways, count.way_nds,
		   count.way_tags, count.relations, count.rel_members,
		   count.rel_tags);
	  return -25;
      }

    ret = readosm_close (handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "CLOSE ERROR: %d\n", ret);
	  return -26;
      }

    ret = readosm_open ("testdata/test.osm.pbf", &handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "OPEN ERROR: %d\n", ret);
	  return -27;
      }

    zero_count (&count);
    ret = readosm_parse (handle, &count, parse_node, parse_way, parse_relation);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_OK, ret);
	  return -28;
      }
    if (count.nodes != 8000 || count.nd_tags != 3162 || count.ways != 12336
	|| count.way_nds != 221627 || count.way_tags != 24904
	|| count.relations != 1520 || count.rel_members != 5723
	|| count.rel_tags != 10081)
      {
	  fprintf (stderr,
		   "PBF-ALL: unexpected results: expected 8000/3162/12336/221627/24904/1520/5723/10081, found %d/%d/%d/%d/%d/%d/%d/%d\n",
		   count.nodes, count.nd_tags, count.ways, count.way_nds,
		   count.way_tags, count.relations, count.rel_members,
		   count.rel_tags);
	  return -29;
      }

    ret = readosm_close (handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "CLOSE ERROR: %d\n", ret);
	  return -30;
      }

    ret = readosm_open ("testdata/test.osm.pbf", &handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "OPEN ERROR: %d\n", ret);
	  return -31;
      }

    zero_count (&count);
    ret = readosm_parse (handle, &count, parse_node, NULL, NULL);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_OK, ret);
	  return -32;
      }
    if (count.nodes != 8000 || count.nd_tags != 3162 || count.ways != 0
	|| count.way_nds != 0 || count.way_tags != 0
	|| count.relations != 0 || count.rel_members != 0
	|| count.rel_tags != 0)
      {
	  fprintf (stderr,
		   "PBF-NODE: unexpected results: expected 8000/3162/0/0/0/0/0/0, found %d/%d/%d/%d/%d/%d/%d/%d\n",
		   count.nodes, count.nd_tags, count.ways, count.way_nds,
		   count.way_tags, count.relations, count.rel_members,
		   count.rel_tags);
	  return -33;
      }

    ret = readosm_close (handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "CLOSE ERROR: %d\n", ret);
	  return -34;
      }

    ret = readosm_open ("testdata/test.osm.pbf", &handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "OPEN ERROR: %d\n", ret);
	  return -35;
      }

    zero_count (&count);
    ret = readosm_parse (handle, &count, NULL, parse_way, NULL);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_OK, ret);
	  return -36;
      }
    if (count.nodes != 0 || count.nd_tags != 0 || count.ways != 12336
	|| count.way_nds != 221627 || count.way_tags != 24904
	|| count.relations != 0 || count.rel_members != 0
	|| count.rel_tags != 0)
      {
	  fprintf (stderr,
		   "PBF-WAY: unexpected results: expected 0/0/12336/221627/24904/0/0/0, found %d/%d/%d/%d/%d/%d/%d/%d\n",
		   count.nodes, count.nd_tags, count.ways, count.way_nds,
		   count.way_tags, count.relations, count.rel_members,
		   count.rel_tags);
	  return -37;
      }

    ret = readosm_close (handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "CLOSE ERROR: %d\n", ret);
	  return -38;
      }

    ret = readosm_open ("testdata/test.osm.pbf", &handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "OPEN ERROR: %d\n", ret);
	  return -39;
      }

    zero_count (&count);
    ret = readosm_parse (handle, &count, NULL, NULL, parse_relation);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_OK, ret);
	  return -40;
      }
    if (count.nodes != 0 || count.nd_tags != 0 || count.ways != 0
	|| count.way_nds != 0 || count.way_tags != 0
	|| count.relations != 1520 || count.rel_members != 5723
	|| count.rel_tags != 10081)
      {
	  fprintf (stderr,
		   "PBF-RELATION: unexpected results: expected 0/0/0/0/0/1520/5723/10081, found %d/%d/%d/%d/%d/%d/%d/%d\n",
		   count.nodes, count.nd_tags, count.ways, count.way_nds,
		   count.way_tags, count.relations, count.rel_members,
		   count.rel_tags);
	  return -41;
      }

    ret = readosm_close (handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "CLOSE ERROR: %d\n", ret);
	  return -42;
      }

    ret = readosm_open ("testdata/test.osm.pbf", &handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "OPEN ERROR: %d\n", ret);
	  return -43;
      }

    zero_count (&count);
    ret = readosm_parse (handle, &count, NULL, NULL, NULL);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "Unexpected result: expected %d, found %d\n",
		   READOSM_OK, ret);
	  return -44;
      }
    if (count.nodes != 0 || count.nd_tags != 0 || count.ways != 0
	|| count.way_nds != 0 || count.way_tags != 0
	|| count.relations != 0 || count.rel_members != 0
	|| count.rel_tags != 0)
      {
	  fprintf (stderr,
		   "PBF-NONE: unexpected results: expected 0/0/0/0/0/0/0/0, found %d/%d/%d/%d/%d/%d/%d/%d\n",
		   count.nodes, count.nd_tags, count.ways, count.way_nds,
		   count.way_tags, count.relations, count.rel_members,
		   count.rel_tags);
	  return -45;
      }

    ret = readosm_close (handle);
    if (ret != READOSM_OK)
      {
	  fprintf (stderr, "CLOSE ERROR: %d\n", ret);
	  return -46;
      }

    return 0;
}
