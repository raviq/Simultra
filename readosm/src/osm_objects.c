/* 
/ osm_object.c
/
/ common OSM objects handling implementation
/
/ version  1.0, 2012 April 21
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

static void
release_internal_tag (readosm_internal_tag * tag)
{
/* freeing an internal TAG object */
    if (tag->key)
	free (tag->key);
    if (tag->value)
	free (tag->value);
}

READOSM_PRIVATE void
release_internal_tag_block (readosm_internal_tag_block * tag_blk, int destroy)
{
/* freeing an internal TAG block */
    int i_tag;
    for (i_tag = 0; i_tag < tag_blk->next_item; i_tag++)
	release_internal_tag (tag_blk->tags + i_tag);
    if (destroy)
	free (tag_blk);
    else
      {
	  tag_blk->next_item = 0;
	  tag_blk->next = NULL;
      }
}

static void
release_internal_member (readosm_internal_member * member)
{
/* freeing an internal MEMBER object */
    if (member->role)
	free (member->role);
}

READOSM_PRIVATE void
release_internal_member_block (readosm_internal_member_block * mbr_blk,
			       int destroy)
{
/* freeing an internal MEMBER block */
    int i_mbr;
    for (i_mbr = 0; i_mbr < mbr_blk->next_item; i_mbr++)
	release_internal_member (mbr_blk->members + i_mbr);
    if (destroy)
	free (mbr_blk);
    else
      {
	  mbr_blk->next_item = 0;
	  mbr_blk->next = NULL;
      }
}

READOSM_PRIVATE void
init_export_tag (readosm_export_tag * tag)
{
/* initializing an empty export TAG object */
    if (tag == NULL)
	return;
    tag->key = NULL;
    tag->value = NULL;
}

READOSM_PRIVATE void
reset_export_tag (readosm_export_tag * tag)
{
/* resetting an export TAG object to initial empty state */
    if (tag == NULL)
	return;
    if (tag->key)
	free (tag->key);
    if (tag->value)
	free (tag->value);
    init_export_tag (tag);
}

READOSM_PRIVATE void
init_internal_node (readosm_internal_node * node)
{
/* allocating an empty internal NODE object */
    node->id = READOSM_UNDEFINED;
    node->latitude = READOSM_UNDEFINED;
    node->longitude = READOSM_UNDEFINED;
    node->version = READOSM_UNDEFINED;
    node->changeset = READOSM_UNDEFINED;
    node->user = NULL;
    node->uid = READOSM_UNDEFINED;
    node->timestamp = NULL;
    node->tag_count = 0;
    node->first_tag.next_item = 0;
    node->first_tag.next = NULL;
    node->last_tag = &(node->first_tag);
}

READOSM_PRIVATE void
append_tag_to_node (readosm_internal_node * node, const char *key,
		    const char *value)
{
/* appending a TAG to a Node object */
    int len;
    readosm_internal_tag_block *tag_blk = node->last_tag;
    readosm_internal_tag *tag;
    if (tag_blk->next_item < READOSM_BLOCK_SZ)
      {
	  /* a free Tag slot is available from the current block */
	  tag = tag_blk->tags + tag_blk->next_item;
	  tag_blk->next_item += 1;
      }
    else
      {
	  /* appending a further Tag block */
	  tag_blk = malloc (sizeof (readosm_internal_tag_block));
	  tag_blk->next_item = 1;
	  tag_blk->next = NULL;
	  tag = tag_blk->tags;
	  node->last_tag->next = tag_blk;
	  node->last_tag = tag_blk;
      }

/* initializing the Tag */
    len = strlen (key);
    tag->key = malloc (len + 1);
    strcpy (tag->key, key);
    len = strlen (value);
    tag->value = malloc (len + 1);
    strcpy (tag->value, value);
}

READOSM_PRIVATE void
destroy_internal_node (readosm_internal_node * node)
{
/* destroying an internal NODE object */
    readosm_internal_tag_block *tag_blk;
    readosm_internal_tag_block *tag_blk_n;
    if (node == NULL)
	return;
    if (node->user)
	free (node->user);
    if (node->timestamp)
	free (node->timestamp);
    tag_blk = &(node->first_tag);
    while (tag_blk)
      {
	  tag_blk_n = tag_blk->next;
	  if (tag_blk == &(node->first_tag))
	      release_internal_tag_block (tag_blk, 0);
	  else
	      release_internal_tag_block (tag_blk, 1);
	  tag_blk = tag_blk_n;
      }
    node->last_tag = &(node->first_tag);
}

static void
init_export_node (readosm_export_node * node)
{
/* initializing an empty export NODE object */
    if (node == NULL)
	return;
    node->id = READOSM_UNDEFINED;
    node->latitude = READOSM_UNDEFINED;
    node->longitude = READOSM_UNDEFINED;
    node->version = READOSM_UNDEFINED;
    node->changeset = READOSM_UNDEFINED;
    node->user = NULL;
    node->uid = READOSM_UNDEFINED;
    node->timestamp = NULL;
    node->tag_count = 0;
    node->tags = NULL;
}

static void
reset_export_node (readosm_export_node * node)
{
/* resetting an export NODE object to initial empty state */
    int i;
    if (node == NULL)
	return;
    if (node->user)
	free (node->user);
    if (node->timestamp)
	free (node->timestamp);
    for (i = 0; i < node->tag_count; i++)
      {
	  readosm_export_tag *tag = node->tags + i;
	  reset_export_tag (tag);
      }
    if (node->tags)
	free (node->tags);
    init_export_node (node);
}

READOSM_PRIVATE readosm_internal_way *
alloc_internal_way (void)
{
/* allocating an empty internal WAY object */
    readosm_internal_way *way = malloc (sizeof (readosm_internal_way));
    way->id = 0;
    way->version = 0;
    way->changeset = 0;
    way->user = NULL;
    way->uid = 0;
    way->timestamp = NULL;
    way->ref_count = 0;
    way->first_ref.next_item = 0;
    way->first_ref.next = NULL;
    way->last_ref = &(way->first_ref);
    way->tag_count = 0;
    way->first_tag.next_item = 0;
    way->first_tag.next = NULL;
    way->last_tag = &(way->first_tag);
    return way;
}

READOSM_PRIVATE void
append_reference_to_way (readosm_internal_way * way, long long node_ref)
{
/* appending a NODE-REF to a WAY object */
    readosm_internal_ref *ref = way->last_ref;
    if (ref->next_item < READOSM_BLOCK_SZ)
      {
	  /* free Ref slot is available from the current block */
	  *(ref->node_refs + ref->next_item) = node_ref;
	  ref->next_item += 1;
      }
    else
      {
	  /* appending a further Ref block */
	  ref = malloc (sizeof (readosm_internal_ref));
	  *(ref->node_refs + 0) = node_ref;
	  ref->next_item = 1;
	  ref->next = NULL;
	  way->last_ref->next = ref;
	  way->last_ref = ref;
      }
}

READOSM_PRIVATE void
append_tag_to_way (readosm_internal_way * way, const char *key,
		   const char *value)
{
/* appending a TAG to a WAY object */
    int len;
    readosm_internal_tag_block *tag_blk = way->last_tag;
    readosm_internal_tag *tag;
    if (tag_blk->next_item < READOSM_BLOCK_SZ)
      {
	  /* a free Tag slot is available from the current block */
	  tag = tag_blk->tags + tag_blk->next_item;
	  tag_blk->next_item += 1;
      }
    else
      {
	  /* appending a further Tag block */
	  tag_blk = malloc (sizeof (readosm_internal_tag_block));
	  tag_blk->next_item = 1;
	  tag_blk->next = NULL;
	  tag = tag_blk->tags;
	  way->last_tag->next = tag_blk;
	  way->last_tag = tag_blk;
      }

/* initializing the Tag */
    len = strlen (key);
    tag->key = malloc (len + 1);
    strcpy (tag->key, key);
    len = strlen (value);
    tag->value = malloc (len + 1);
    strcpy (tag->value, value);
}

READOSM_PRIVATE void
destroy_internal_way (readosm_internal_way * way)
{
/* destroying an internal WAY object */
    readosm_internal_ref *ref;
    readosm_internal_ref *ref_n;
    readosm_internal_tag_block *tag_blk;
    readosm_internal_tag_block *tag_blk_n;
    if (way == NULL)
	return;
    if (way->user)
	free (way->user);
    if (way->timestamp)
	free (way->timestamp);
    ref = way->first_ref.next;
    while (ref)
      {
	  ref_n = ref->next;
	  free (ref);
	  ref = ref_n;
      }
    tag_blk = &(way->first_tag);
    while (tag_blk)
      {
	  tag_blk_n = tag_blk->next;
	  if (tag_blk == &(way->first_tag))
	      release_internal_tag_block (tag_blk, 0);
	  else
	      release_internal_tag_block (tag_blk, 1);
	  tag_blk = tag_blk_n;
      }
    free (way);
}

static void
init_export_way (readosm_export_way * way)
{
/* initializing an empty export WAY object */
    if (way == NULL)
	return;
    way->id = 0;
    way->version = 0;
    way->changeset = 0;
    way->user = NULL;
    way->uid = 0;
    way->timestamp = NULL;
    way->node_ref_count = 0;
    way->node_refs = NULL;
    way->tag_count = 0;
    way->tags = NULL;
}

static void
reset_export_way (readosm_export_way * way)
{
/* resetting an export WAY object to initial empty state */
    int i;
    if (way == NULL)
	return;
    if (way->user)
	free (way->user);
    if (way->timestamp)
	free (way->timestamp);
    if (way->node_refs)
	free (way->node_refs);
    for (i = 0; i < way->tag_count; i++)
      {
	  readosm_export_tag *tag = way->tags + i;
	  reset_export_tag (tag);
      }
    if (way->tags)
	free (way->tags);
    init_export_way (way);
}

static void
init_export_member (readosm_export_member * member)
{
/* initializing an empty export RELATION-MEMBER object */
    if (member == NULL)
	return;
    member->member_type = READOSM_UNDEFINED;
    member->id = 0;
    member->role = NULL;
}

static void
reset_export_member (readosm_export_member * member)
{
/* resetting an export RELATION-MEMBER object to initial empty state */
    if (member == NULL)
	return;
    if (member->role)
	free (member->role);
    init_export_member (member);
}

READOSM_PRIVATE readosm_internal_relation *
alloc_internal_relation (void)
{
/* allocating an empty internal RELATION object */
    readosm_internal_relation *rel =
	malloc (sizeof (readosm_internal_relation));
    rel->id = 0;
    rel->version = 0;
    rel->changeset = 0;
    rel->user = NULL;
    rel->uid = 0;
    rel->timestamp = NULL;
    rel->member_count = 0;
    rel->first_member.next_item = 0;
    rel->first_member.next = NULL;
    rel->last_member = &(rel->first_member);
    rel->tag_count = 0;
    rel->first_tag.next_item = 0;
    rel->first_tag.next = NULL;
    rel->last_tag = &(rel->first_tag);
    return rel;
}

READOSM_PRIVATE void
append_member_to_relation (readosm_internal_relation * relation, int type,
			   long long id, const char *role)
{
/* appending a RELATION-MEMBER to a RELATION object */
    int len;
    readosm_internal_member_block *mbr_blk = relation->last_member;
    readosm_internal_member *member;
    if (mbr_blk->next_item < READOSM_BLOCK_SZ)
      {
	  /* a free Member slot is available from the current block */
	  member = mbr_blk->members + mbr_blk->next_item;
	  mbr_blk->next_item += 1;
      }
    else
      {
	  /* appending a further Member block */
	  mbr_blk = malloc (sizeof (readosm_internal_member_block));
	  mbr_blk->next_item = 1;
	  mbr_blk->next = NULL;
	  member = mbr_blk->members;
	  relation->last_member->next = mbr_blk;
	  relation->last_member = mbr_blk;
      }

    member->member_type = type;
    member->id = id;
    len = strlen (role);
    member->role = malloc (len + 1);
    strcpy (member->role, role);
}

READOSM_PRIVATE void
append_tag_to_relation (readosm_internal_relation * relation, const char *key,
			const char *value)
{
/* appending a TAG to a RELATION object */
    int len;
    readosm_internal_tag_block *tag_blk = relation->last_tag;
    readosm_internal_tag *tag;
    if (tag_blk->next_item < READOSM_BLOCK_SZ)
      {
	  /* a free Tag slot is available from the current block */
	  tag = tag_blk->tags + tag_blk->next_item;
	  tag_blk->next_item += 1;
      }
    else
      {
	  /* appending a further Tag block */
	  tag_blk = malloc (sizeof (readosm_internal_tag_block));
	  tag_blk->next_item = 1;
	  tag_blk->next = NULL;
	  tag = tag_blk->tags;
	  relation->last_tag->next = tag_blk;
	  relation->last_tag = tag_blk;
      }

/* initializing the Tag */
    len = strlen (key);
    tag->key = malloc (len + 1);
    strcpy (tag->key, key);
    len = strlen (value);
    tag->value = malloc (len + 1);
    strcpy (tag->value, value);
}

READOSM_PRIVATE void
destroy_internal_relation (readosm_internal_relation * relation)
{
/* destroying an internal RELATION object */
    readosm_internal_member_block *mbr_blk;
    readosm_internal_member_block *mbr_blk_n;
    readosm_internal_tag_block *tag_blk;
    readosm_internal_tag_block *tag_blk_n;
    if (relation == NULL)
	return;
    if (relation->user)
	free (relation->user);
    if (relation->timestamp)
	free (relation->timestamp);
    mbr_blk = &(relation->first_member);
    while (mbr_blk)
      {
	  mbr_blk_n = mbr_blk->next;
	  if (mbr_blk == &(relation->first_member))
	      release_internal_member_block (mbr_blk, 0);
	  else
	      release_internal_member_block (mbr_blk, 1);
	  mbr_blk = mbr_blk_n;
      }
    tag_blk = &(relation->first_tag);
    while (tag_blk)
      {
	  tag_blk_n = tag_blk->next;
	  if (tag_blk == &(relation->first_tag))
	      release_internal_tag_block (tag_blk, 0);
	  else
	      release_internal_tag_block (tag_blk, 1);
	  tag_blk = tag_blk_n;
      }
    free (relation);
}

static void
init_export_relation (readosm_export_relation * relation)
{
/* initializing an empty export RELATION object */
    if (relation == NULL)
	return;
    relation->id = 0;
    relation->version = 0;
    relation->changeset = 0;
    relation->user = NULL;
    relation->uid = 0;
    relation->timestamp = NULL;
    relation->member_count = 0;
    relation->members = NULL;
    relation->tag_count = 0;
    relation->tags = NULL;
}

static void
reset_export_relation (readosm_export_relation * relation)
{
/* resetting an export RELATION object to initial empty state */
    int i;
    if (relation == NULL)
	return;
    if (relation->user)
	free (relation->user);
    if (relation->timestamp)
	free (relation->timestamp);
    for (i = 0; i < relation->member_count; i++)
      {
	  readosm_export_member *member = relation->members + i;
	  reset_export_member (member);
      }
    if (relation->members)
	free (relation->members);
    for (i = 0; i < relation->tag_count; i++)
      {
	  readosm_export_tag *tag = relation->tags + i;
	  reset_export_tag (tag);
      }
    if (relation->tags)
	free (relation->tags);
    init_export_relation (relation);
}


READOSM_PRIVATE int
call_node_callback (readosm_node_callback node_callback,
		    const void *user_data, readosm_internal_node * node)
{
/* calling the Node-handling callback function */
    int ret;
    int len;
    readosm_internal_tag *tag;
    readosm_internal_tag_block *tag_blk;
    int i_tag;
    readosm_export_node exp_node;

/* 
 / please note: READONLY-NODE simply is the same as export 
 / NODE inteded to disabale any possible awful user action
*/
    readosm_node *readonly_node = (readosm_node *) & exp_node;

/*initialing an empty export NODE object */
    init_export_node (&exp_node);

/* setting up the export NODE object */
    exp_node.id = node->id;
    exp_node.latitude = node->latitude;
    exp_node.longitude = node->longitude;
    exp_node.version = node->version;
    exp_node.changeset = node->changeset;
    if (node->user != NULL)
      {
	  len = strlen (node->user);
	  exp_node.user = malloc (len + 1);
	  strcpy (exp_node.user, node->user);
      }
    exp_node.uid = node->uid;
    if (node->timestamp != NULL)
      {
	  len = strlen (node->timestamp);
	  exp_node.timestamp = malloc (len + 1);
	  strcpy (exp_node.timestamp, node->timestamp);
      }

/* setting up the NODE-TAGs array */
    tag_blk = &(node->first_tag);
    while (tag_blk)
      {
	  exp_node.tag_count += tag_blk->next_item;
	  tag_blk = tag_blk->next;
      }
    if (exp_node.tag_count > 0)
      {
	  int i;
	  readosm_export_tag *p_tag;
	  exp_node.tags =
	      malloc (sizeof (readosm_export_tag) * exp_node.tag_count);
	  for (i = 0; i < exp_node.tag_count; i++)
	    {
		p_tag = exp_node.tags + i;
		init_export_tag (p_tag);
	    }
	  i = 0;
	  tag_blk = &(node->first_tag);
	  while (tag_blk)
	    {
		for (i_tag = 0; i_tag < tag_blk->next_item; i_tag++)
		  {
		      tag = tag_blk->tags + i_tag;
		      p_tag = exp_node.tags + i;
		      if (tag->key != NULL)
			{
			    len = strlen (tag->key);
			    p_tag->key = malloc (len + 1);
			    strcpy (p_tag->key, tag->key);
			}
		      if (tag->value != NULL)
			{
			    len = strlen (tag->value);
			    p_tag->value = malloc (len + 1);
			    strcpy (p_tag->value, tag->value);
			}
		      i++;
		  }
		tag_blk = tag_blk->next;
	    }
      }

/* calling the user-defined NODE handling callback function */
    ret = (*node_callback) (user_data, readonly_node);

/* resetting the export WAY object */
    reset_export_node (&exp_node);
    return ret;
}

READOSM_PRIVATE int
call_way_callback (readosm_way_callback way_callback,
		   const void *user_data, readosm_internal_way * way)
{
/* calling the Way-handling callback function */
    int ret;
    int len;
    int i;
    readosm_internal_ref *ref;
    readosm_internal_tag *tag;
    readosm_internal_tag_block *tag_blk;
    int i_tag;
    int i_ref;
    readosm_export_way exp_way;

/* 
 / please note: READONLY-WAY simply is the same as export 
 / WAY inteded to disabale any possible awful user action
*/
    readosm_way *readonly_way = (readosm_way *) & exp_way;

/*initialing an empty export WAY object */
    init_export_way (&exp_way);

    exp_way.id = way->id;
    exp_way.version = way->version;
    exp_way.changeset = way->changeset;
    if (way->user != NULL)
      {
	  len = strlen (way->user);
	  exp_way.user = malloc (len + 1);
	  strcpy (exp_way.user, way->user);
      }
    exp_way.uid = way->uid;
    if (way->timestamp != NULL)
      {
	  len = strlen (way->timestamp);
	  exp_way.timestamp = malloc (len + 1);
	  strcpy (exp_way.timestamp, way->timestamp);
      }

    ref = &(way->first_ref);
    while (ref)
      {
	  exp_way.node_ref_count += ref->next_item;
	  ref = ref->next;
      }

/* setting up the NODE-REFs array */
    if (exp_way.node_ref_count > 0)
      {
	  exp_way.node_refs =
	      malloc (sizeof (long long) * exp_way.node_ref_count);
	  i = 0;
	  ref = &(way->first_ref);
	  while (ref)
	    {
		for (i_ref = 0; i_ref < ref->next_item; i_ref++)
		  {
		      *(exp_way.node_refs + i) = *(ref->node_refs + i_ref);
		      i++;
		  }
		ref = ref->next;
	    }
      }

/* setting up the WAY-TAGs array */
    tag_blk = &(way->first_tag);
    while (tag_blk)
      {
	  exp_way.tag_count += tag_blk->next_item;
	  tag_blk = tag_blk->next;
      }
    if (exp_way.tag_count > 0)
      {
	  readosm_export_tag *p_tag;
	  exp_way.tags =
	      malloc (sizeof (readosm_export_tag) * exp_way.tag_count);
	  for (i = 0; i < exp_way.tag_count; i++)
	    {
		p_tag = exp_way.tags + i;
		init_export_tag (p_tag);
	    }
	  i = 0;
	  tag_blk = &(way->first_tag);
	  while (tag_blk)
	    {
		for (i_tag = 0; i_tag < tag_blk->next_item; i_tag++)
		  {
		      tag = tag_blk->tags + i_tag;
		      p_tag = exp_way.tags + i;
		      if (tag->key != NULL)
			{
			    len = strlen (tag->key);
			    p_tag->key = malloc (len + 1);
			    strcpy (p_tag->key, tag->key);
			}
		      if (tag->value != NULL)
			{
			    len = strlen (tag->value);
			    p_tag->value = malloc (len + 1);
			    strcpy (p_tag->value, tag->value);
			}
		      i++;
		  }
		tag_blk = tag_blk->next;
	    }
      }

/* calling the user-defined WAY handling callback function */
    ret = (*way_callback) (user_data, readonly_way);

/* resetting the export WAY object */
    reset_export_way (&exp_way);
    return ret;
}

READOSM_PRIVATE int
call_relation_callback (readosm_relation_callback relation_callback,
			const void *user_data,
			readosm_internal_relation * relation)
{
/* calling the Relation-handling callback function */
    int ret;
    int len;
    int i;
    readosm_internal_member *member;
    readosm_internal_member_block *mbr_blk;
    int i_mbr;
    readosm_internal_tag *tag;
    readosm_internal_tag_block *tag_blk;
    int i_tag;
    readosm_export_relation exp_relation;

/* 
 / please note: READONLY-RELATION simply is the same as export 
 / RELATION inteded to disabale any possible awful user action
*/
    readosm_relation *readonly_relation = (readosm_relation *) & exp_relation;

/*initialing an empty export RELATION object */
    init_export_relation (&exp_relation);

    exp_relation.id = relation->id;
    exp_relation.version = relation->version;
    exp_relation.changeset = relation->changeset;
    if (relation->user != NULL)
      {
	  len = strlen (relation->user);
	  exp_relation.user = malloc (len + 1);
	  strcpy (exp_relation.user, relation->user);
      }
    exp_relation.uid = relation->uid;
    if (relation->timestamp != NULL)
      {
	  len = strlen (relation->timestamp);
	  exp_relation.timestamp = malloc (len + 1);
	  strcpy (exp_relation.timestamp, relation->timestamp);
      }

/* setting up the RELATION-MEMBERs array */
    mbr_blk = &(relation->first_member);
    while (mbr_blk)
      {
	  exp_relation.member_count += mbr_blk->next_item;
	  mbr_blk = mbr_blk->next;
      }
    if (exp_relation.member_count > 0)
      {
	  readosm_export_member *p_member;
	  exp_relation.members =
	      malloc (sizeof (readosm_export_member) *
		      exp_relation.member_count);
	  for (i = 0; i < exp_relation.member_count; i++)
	    {
		p_member = exp_relation.members + i;
		init_export_member (p_member);
	    }
	  i = 0;
	  mbr_blk = &(relation->first_member);
	  while (mbr_blk)
	    {
		for (i_mbr = 0; i_mbr < mbr_blk->next_item; i_mbr++)
		  {
		      member = mbr_blk->members + i_mbr;
		      p_member = exp_relation.members + i;
		      p_member->member_type = member->member_type;
		      p_member->id = member->id;
		      if (member->role != NULL)
			{
			    len = strlen (member->role);
			    p_member->role = malloc (len + 1);
			    strcpy (p_member->role, member->role);
			}
		      i++;
		  }
		mbr_blk = mbr_blk->next;
	    }
      }

/* setting up the RELATION-TAGs array */
    tag_blk = &(relation->first_tag);
    while (tag_blk)
      {
	  exp_relation.tag_count += tag_blk->next_item;
	  tag_blk = tag_blk->next;
      }
    if (exp_relation.tag_count > 0)
      {
	  readosm_export_tag *p_tag;
	  exp_relation.tags =
	      malloc (sizeof (readosm_export_tag) * exp_relation.tag_count);
	  for (i = 0; i < exp_relation.tag_count; i++)
	    {
		p_tag = exp_relation.tags + i;
		init_export_tag (p_tag);
	    }
	  i = 0;
	  tag_blk = &(relation->first_tag);
	  while (tag_blk)
	    {
		for (i_tag = 0; i_tag < tag_blk->next_item; i_tag++)
		  {
		      tag = tag_blk->tags + i_tag;
		      p_tag = exp_relation.tags + i;
		      if (tag->key != NULL)
			{
			    len = strlen (tag->key);
			    p_tag->key = malloc (len + 1);
			    strcpy (p_tag->key, tag->key);
			}
		      if (tag->value != NULL)
			{
			    len = strlen (tag->value);
			    p_tag->value = malloc (len + 1);
			    strcpy (p_tag->value, tag->value);
			}
		      i++;
		  }
		tag_blk = tag_blk->next;
	    }
      }

/* calling the user-defined RELATION handling callback function */
    ret = (*relation_callback) (user_data, readonly_relation);

/* resetting the export RELATION object */
    reset_export_relation (&exp_relation);
    return ret;
}
