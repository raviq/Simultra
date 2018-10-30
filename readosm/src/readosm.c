/* 
/ readosm.c
/
/ ReadOSM main implementation (externally visible API)
/
/ version  1.0, 2012 April 10
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32) && !defined(__MINGW32__)
/* MSVC: avoiding to include at all config.h */
#else
#include "config.h"
#endif

#include "readosm.h"
#include "readosm_internals.h"

#ifdef _WIN32
#define strcasecmp	_stricmp
#endif /* not WIN32 */

static int
test_endianness ()
{
/* checks the current CPU endianness */
    readosm_endian4 endian4;
    endian4.bytes[0] = 0x01;
    endian4.bytes[1] = 0x00;
    endian4.bytes[2] = 0x00;
    endian4.bytes[3] = 0x00;
    if (endian4.uint32_value == 1)
	return READOSM_LITTLE_ENDIAN;
    return READOSM_BIG_ENDIAN;
}

static readosm_file *
alloc_osm_file (int little_endian_cpu, int format)
{
/* allocating and initializing the OSM input file struct */
    readosm_file *input = malloc (sizeof (readosm_file));
    if (!input)
	return NULL;
    input->magic1 = READOSM_MAGIC_START;
    input->file_format = format;
    input->little_endian_cpu = little_endian_cpu;
    input->magic2 = READOSM_MAGIC_END;
    input->in = NULL;
    return input;
}

static void
destroy_osm_file (readosm_file * input)
{
/* destroying the OSM input file struct */
    if (input)
      {
	  if (input->in)
	      fclose (input->in);
	  free (input);
      }
}

READOSM_DECLARE int
readosm_open (const char *path, const void **osm_handle)
{
/* opening and initializing the OSM input file */
    readosm_file *input;
    int len;
    int format;
    int little_endian_cpu = test_endianness ();

    *osm_handle = NULL;
    if (path == NULL || osm_handle == NULL)
	return READOSM_NULL_HANDLE;

    len = strlen (path);
    if (len > 4 && strcasecmp (path + len - 4, ".osm") == 0)
	format = READOSM_OSM_FORMAT;
    else if (len > 4 && strcasecmp (path + len - 4, ".pbf") == 0)
	format = READOSM_PBF_FORMAT;
    else
	return READOSM_INVALID_SUFFIX;

/* allocating the OSM input file struct */
    input = alloc_osm_file (little_endian_cpu, format);
    if (!input)
	return READOSM_INSUFFICIENT_MEMORY;
    *osm_handle = input;

    input->in = fopen (path, "rb");
    if (input->in == NULL)
	return READOSM_FILE_NOT_FOUND;

    return READOSM_OK;
}

READOSM_DECLARE int
readosm_close (const void *osm_handle)
{
/* attempting to destroy the OSM input file */
    readosm_file *input = (readosm_file *) osm_handle;
    if (!input)
	return READOSM_NULL_HANDLE;
    if ((input->magic1 == READOSM_MAGIC_START)
	&& input->magic2 == READOSM_MAGIC_END)
	;
    else
	return READOSM_INVALID_HANDLE;

/* destroying the workbook */
    destroy_osm_file (input);

    return READOSM_OK;
}

READOSM_DECLARE int
readosm_parse (const void *osm_handle, const void *user_data,
	       readosm_node_callback node_fnct, readosm_way_callback way_fnct,
	       readosm_relation_callback relation_fnct)
{
/* attempting to parse the OSM input file */
    int ret;
    readosm_file *input = (readosm_file *) osm_handle;
    if (!input)
	return READOSM_NULL_HANDLE;
    if ((input->magic1 == READOSM_MAGIC_START)
	&& input->magic2 == READOSM_MAGIC_END)
	;
    else
	return READOSM_INVALID_HANDLE;

    if (input->file_format == READOSM_OSM_FORMAT)
	ret =
	    parse_osm_xml (input, user_data, node_fnct, way_fnct,
			   relation_fnct);
    else if (input->file_format == READOSM_PBF_FORMAT)
	ret =
	    parse_osm_pbf (input, user_data, node_fnct, way_fnct,
			   relation_fnct);
    else
	return READOSM_INVALID_HANDLE;

    return ret;
}
