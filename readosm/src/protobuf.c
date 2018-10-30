/* 
/ protobuf.c
/
/ Protocol Buffer (.pbf) implementation
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
#include <memory.h>
#include <time.h>

#include <zlib.h>

#if defined(_WIN32) && !defined(__MINGW32__)
/* MSVC: avoiding to include at all config.h */
#else
#include "config.h"
#endif

#include "readosm.h"
#include "readosm_internals.h"
#include "readosm_protobuf.h"

#define MAX_NODES 1024

struct pbf_params
{
/* an helper struct supporting PBF parsing */
    const void *user_data;
    readosm_node_callback node_callback;
    readosm_way_callback way_callback;
    readosm_relation_callback relation_callback;
    int stop;
};

static void
init_variant (readosm_variant * variant, int little_endian_cpu)
{
/* initializing an empty PBF Variant object */
    variant->little_endian_cpu = little_endian_cpu;
    variant->type = READOSM_VAR_UNDEFINED;
    variant->field_id = 0;
    variant->length = 0;
    variant->pointer = NULL;
    variant->valid = 0;
    variant->first = NULL;
    variant->last = NULL;
}

static void
reset_variant (readosm_variant * variant)
{
/* resetting a PBF Variant object to its initial empty state */
    variant->type = READOSM_VAR_UNDEFINED;
    variant->field_id = 0;
    variant->length = 0;
    variant->pointer = NULL;
    variant->valid = 0;
}

static void
add_variant_hints (readosm_variant * variant, unsigned char type,
		   unsigned char field_id)
{
/* adding a field hint to a PBF Variant object */
    readosm_variant_hint *hint = malloc (sizeof (readosm_variant_hint));
    hint->type = type;
    hint->field_id = field_id;
    hint->next = NULL;
    if (variant->first == NULL)
	variant->first = hint;
    if (variant->last != NULL)
	variant->last->next = hint;
    variant->last = hint;
}

static int
find_type_hint (readosm_variant * variant, unsigned char field_id,
		unsigned char type, unsigned char *type_hint)
{
/* attempting to find the type hint for some PBF Variant field */
    readosm_variant_hint *hint = variant->first;
    while (hint)
      {
	  if (hint->field_id == field_id)
	    {
		switch (type)
		  {
		  case 0:
		      switch (hint->type)
			{
			case READOSM_VAR_INT32:
			case READOSM_VAR_INT64:
			case READOSM_VAR_UINT32:
			case READOSM_VAR_UINT64:
			case READOSM_VAR_SINT32:
			case READOSM_VAR_SINT64:
			case READOSM_VAR_BOOL:
			case READOSM_VAR_ENUM:
			    *type_hint = hint->type;
			    return 1;
			}
		      break;
		  case 2:
		      if (hint->type == READOSM_LEN_BYTES)
			{
			    *type_hint = hint->type;
			    return 1;
			}
		      break;
		  };
	    }
	  hint = hint->next;
      }
    return 0;
}

static void
finalize_variant (readosm_variant * variant)
{
/* cleaning any memory allocation for a PBF Variant object */
    readosm_variant_hint *hint;
    readosm_variant_hint *hint_n;
    hint = variant->first;
    while (hint)
      {
	  hint_n = hint->next;
	  free (hint);
	  hint = hint_n;
      }
    variant->first = NULL;
    variant->last = NULL;
}

static void
init_string_table (readosm_string_table * string_table)
{
/* initializing an empty PBF StringTable object */
    string_table->first = NULL;
    string_table->last = NULL;
    string_table->count = 0;
    string_table->strings = NULL;
}

static void
append_string_to_table (readosm_string_table * string_table,
			readosm_variant * variant)
{
/* appending a string to a PBF StringTable object */
    readosm_string *string = malloc (sizeof (readosm_string));
    string->string = malloc (variant->length + 1);
    memcpy (string->string, variant->pointer, variant->length);
    *(string->string + variant->length) = '\0';
    string->next = NULL;
    if (string_table->first == NULL)
	string_table->first = string;
    if (string_table->last != NULL)
	string_table->last->next = string;
    string_table->last = string;
}

static void
array_from_string_table (readosm_string_table * string_table)
{
/* creating a pointer array supporting a StringTable object */
    int i;
    readosm_string *string = string_table->first;
    while (string != NULL)
      {
	  /* counting how many strings are into the table */
	  string_table->count++;
	  string = string->next;
      }
    if (string_table->count <= 0)
	return;

/* allocating the pointer array */
    string_table->strings =
	malloc (sizeof (readosm_string *) * string_table->count);
    i = 0;
    string = string_table->first;
    while (string != NULL)
      {
	  /* setting up pointers to strings */
	  *(string_table->strings + i) = string;
	  i++;
	  string = string->next;
      }
}

static void
finalize_string_table (readosm_string_table * string_table)
{
/* cleaning any memory allocation for a StringTable object */
    readosm_string *string;
    readosm_string *string_n;
    string = string_table->first;
    while (string)
      {
	  string_n = string->next;
	  if (string->string)
	      free (string->string);
	  free (string);
	  string = string_n;
      }
    if (string_table->strings)
	free (string_table->strings);
}

static void
init_uint32_packed (readosm_uint32_packed * packed)
{
/* initialing an empty PBF uint32 packed object */
    packed->first = NULL;
    packed->last = NULL;
    packed->count = 0;
    packed->values = NULL;
}

static void
append_uint32_packed (readosm_uint32_packed * packed, unsigned int val)
{
/* appending an uint32 value to a PBF packed object */
    readosm_uint32 *value = malloc (sizeof (readosm_uint32));
    value->value = val;
    value->next = NULL;
    if (packed->first == NULL)
	packed->first = value;
    if (packed->last != NULL)
	packed->last->next = value;
    packed->last = value;
}

static void
array_from_uint32_packed (readosm_uint32_packed * packed)
{
/* creating an array supporting an uint32 packed object */
    int i;
    readosm_uint32 *value = packed->first;
    while (value != NULL)
      {
	  /* counting how many values are into the packed list */
	  packed->count++;
	  value = value->next;
      }
    if (packed->count <= 0)
	return;

/* allocating the array */
    packed->values = malloc (sizeof (unsigned int) * packed->count);
    i = 0;
    value = packed->first;
    while (value != NULL)
      {
	  /* setting up array values */
	  *(packed->values + i) = value->value;
	  i++;
	  value = value->next;
      }
}

static void
finalize_uint32_packed (readosm_uint32_packed * packed)
{
/* cleaning any memory allocation for an uint32 packed object */
    readosm_uint32 *value;
    readosm_uint32 *value_n;
    value = packed->first;
    while (value)
      {
	  value_n = value->next;
	  free (value);
	  value = value_n;
      }
    if (packed->values)
	free (packed->values);

}

static void
reset_uint32_packed (readosm_uint32_packed * packed)
{
/* resetting an uint32 packed object to empty initial state */
    finalize_uint32_packed (packed);
    packed->first = NULL;
    packed->last = NULL;
    packed->count = 0;
    packed->values = NULL;
}

static void
init_int32_packed (readosm_int32_packed * packed)
{
/* initialing an empty PBF int32 packed object */
    packed->first = NULL;
    packed->last = NULL;
    packed->count = 0;
    packed->values = NULL;
}

static void
append_int32_packed (readosm_int32_packed * packed, int val)
{
/* appending an int32 value to a PBF packed object */
    readosm_int32 *value = malloc (sizeof (readosm_int32));
    value->value = val;
    value->next = NULL;
    if (packed->first == NULL)
	packed->first = value;
    if (packed->last != NULL)
	packed->last->next = value;
    packed->last = value;
}

static void
finalize_int32_packed (readosm_int32_packed * packed)
{
/* cleaning any memory allocation for an int32 packed object */
    readosm_int32 *value;
    readosm_int32 *value_n;
    value = packed->first;
    while (value)
      {
	  value_n = value->next;
	  free (value);
	  value = value_n;
      }
    if (packed->values)
	free (packed->values);

}

static void
reset_int32_packed (readosm_int32_packed * packed)
{
/* resetting an int32 packed object to empty initial state */
    finalize_int32_packed (packed);
    packed->first = NULL;
    packed->last = NULL;
    packed->count = 0;
    packed->values = NULL;
}

static void
init_int64_packed (readosm_int64_packed * packed)
{
/* initialing an empty PBF int64 packed object */
    packed->first = NULL;
    packed->last = NULL;
    packed->count = 0;
    packed->values = NULL;
}

static void
append_int64_packed (readosm_int64_packed * packed, long long val)
{
/* appending an int64 value to a PBF packed object */
    readosm_int64 *value = malloc (sizeof (readosm_int64));
    value->value = val;
    value->next = NULL;
    if (packed->first == NULL)
	packed->first = value;
    if (packed->last != NULL)
	packed->last->next = value;
    packed->last = value;
}

static void
array_from_int64_packed (readosm_int64_packed * packed)
{
/* creating an array supporting an int64 packed object */
    int i;
    readosm_int64 *value = packed->first;
    while (value != NULL)
      {
	  /* counting how many values are into the packed list */
	  packed->count++;
	  value = value->next;
      }
    if (packed->count <= 0)
	return;

/* allocating the array */
    packed->values = malloc (sizeof (long long) * packed->count);
    i = 0;
    value = packed->first;
    while (value != NULL)
      {
	  /* setting up array values */
	  *(packed->values + i) = value->value;
	  i++;
	  value = value->next;
      }
}

static void
finalize_int64_packed (readosm_int64_packed * packed)
{
/* cleaning any memory allocation for an int64 packed object */
    readosm_int64 *value;
    readosm_int64 *value_n;
    value = packed->first;
    while (value)
      {
	  value_n = value->next;
	  free (value);
	  value = value_n;
      }
    if (packed->values)
	free (packed->values);
}

static void
reset_int64_packed (readosm_int64_packed * packed)
{
/* resetting an int64 packed object to empty initial state */
    finalize_int64_packed (packed);
    packed->first = NULL;
    packed->last = NULL;
    packed->count = 0;
    packed->values = NULL;
}

static void
init_packed_infos (readosm_packed_infos * packed)
{
/* initialing an empty PBF  packed Infos object */
    packed->ver_count = 0;
    packed->versions = NULL;
    packed->tim_count = 0;
    packed->timestamps = NULL;
    packed->cng_count = 0;
    packed->changesets = NULL;
    packed->uid_count = 0;
    packed->uids = NULL;
    packed->usr_count = 0;
    packed->users = NULL;
}

static void
finalize_packed_infos (readosm_packed_infos * packed)
{
/* cleaning any memory allocation for a packed Infos object */
    if (packed->versions)
	free (packed->versions);
    if (packed->timestamps)
	free (packed->timestamps);
    if (packed->changesets)
	free (packed->changesets);
    if (packed->uids)
	free (packed->uids);
    if (packed->users)
	free (packed->users);
}

static unsigned char *
read_var (unsigned char *start, unsigned char *stop, readosm_variant * variant)
{
/* 
 / attempting to read a variable length base128 int 
 /
 / PBF integers are encoded as base128, i.e. using 7 bits
 / for each byte: if the most significant bit is 1, then
 / a further byte is required to get the int value, and so
 / on, until a byte having a 0 most significant bit is found.
 /
 / using this encoding little values simply require few bytes:
 / as a worst case 5 bytes are required to encode int32, and
 / 10 bytes to encode int64
 /
 / there is a further complication: negative value will always 
 / require 5 or 10 bytes: thus SINT32 and SINT64 values are
 / encoded using a "ZigZag" schema.
 /
 / for more details please see:
 / https://developers.google.com/protocol-buffers/docs/encoding
*/
    unsigned char *ptr = start;
    unsigned char c;
    unsigned int v32;
    unsigned long long v64;
    unsigned int value32 = 0x00000000;
    unsigned long long value64 = 0x0000000000000000;
    readosm_endian4 endian4;
    readosm_endian8 endian8;
    int next;
    int count = 0;
    int neg;

    while (1)
      {
	  if (ptr > stop)
	      return NULL;
	  c = *ptr++;
	  if ((c & 0x80) == 0x80)
	      next = 1;
	  else
	      next = 0;
	  c &= 0x7f;
	  switch (variant->type)
	    {
	    case READOSM_VAR_INT32:
	    case READOSM_VAR_UINT32:
	    case READOSM_VAR_SINT32:
		switch (count)
		  {
		  case 0:
		      memset (endian4.bytes, 0x00, 4);
		      if (variant->little_endian_cpu)
			  endian4.bytes[0] = c;
		      else
			  endian4.bytes[3] = c;
		      v32 = endian4.uint32_value;
		      v32 &= READOSM_MASK32_1;
		      value32 |= v32;
		      break;
		  case 1:
		      memset (endian4.bytes, 0x00, 4);
		      if (variant->little_endian_cpu)
			  endian4.bytes[0] = c;
		      else
			  endian4.bytes[3] = c;
		      v32 = endian4.uint32_value << 7;
		      v32 &= READOSM_MASK32_2;
		      value32 |= v32;
		      break;
		  case 2:
		      memset (endian4.bytes, 0x00, 4);
		      if (variant->little_endian_cpu)
			  endian4.bytes[0] = c;
		      else
			  endian4.bytes[3] = c;
		      v32 = endian4.uint32_value << 14;
		      v32 &= READOSM_MASK32_3;
		      value32 |= v32;
		      break;
		  case 3:
		      memset (endian4.bytes, 0x00, 4);
		      if (variant->little_endian_cpu)
			  endian4.bytes[0] = c;
		      else
			  endian4.bytes[3] = c;
		      v32 = endian4.uint32_value << 21;
		      v32 &= READOSM_MASK32_4;
		      value32 |= v32;
		      break;
		  case 4:
		      memset (endian4.bytes, 0x00, 4);
		      if (variant->little_endian_cpu)
			  endian4.bytes[0] = c;
		      else
			  endian4.bytes[3] = c;
		      v32 = endian4.uint32_value << 28;
		      v32 &= READOSM_MASK32_5;
		      value32 |= v32;
		      break;
		  default:
		      return NULL;
		  };
		break;
	    case READOSM_VAR_INT64:
	    case READOSM_VAR_UINT64:
	    case READOSM_VAR_SINT64:
		switch (count)
		  {
		  case 0:
		      memset (endian8.bytes, 0x00, 8);
		      if (variant->little_endian_cpu)
			  endian8.bytes[0] = c;
		      else
			  endian8.bytes[7] = c;
		      v64 = endian8.uint64_value;
		      v64 &= READOSM_MASK64_1;
		      value64 |= v64;
		      break;
		  case 1:
		      memset (endian8.bytes, 0x00, 8);
		      if (variant->little_endian_cpu)
			  endian8.bytes[0] = c;
		      else
			  endian8.bytes[7] = c;
		      v64 = endian8.uint64_value << 7;
		      v64 &= READOSM_MASK64_2;
		      value64 |= v64;
		      break;
		  case 2:
		      memset (endian8.bytes, 0x00, 8);
		      if (variant->little_endian_cpu)
			  endian8.bytes[0] = c;
		      else
			  endian8.bytes[7] = c;
		      v64 = endian8.uint64_value << 14;
		      v64 &= READOSM_MASK64_3;
		      value64 |= v64;
		      break;
		  case 3:
		      memset (endian8.bytes, 0x00, 8);
		      if (variant->little_endian_cpu)
			  endian8.bytes[0] = c;
		      else
			  endian8.bytes[7] = c;
		      v64 = endian8.uint64_value << 21;
		      v64 &= READOSM_MASK64_4;
		      value64 |= v64;
		      break;
		  case 4:
		      memset (endian8.bytes, 0x00, 8);
		      if (variant->little_endian_cpu)
			  endian8.bytes[0] = c;
		      else
			  endian8.bytes[7] = c;
		      v64 = endian8.uint64_value << 28;
		      v64 &= READOSM_MASK64_5;
		      value64 |= v64;
		      break;
		  case 5:
		      memset (endian8.bytes, 0x00, 8);
		      if (variant->little_endian_cpu)
			  endian8.bytes[0] = c;
		      else
			  endian8.bytes[7] = c;
		      v64 = endian8.uint64_value << 35;
		      v64 &= READOSM_MASK64_6;
		      value64 |= v64;
		      break;
		  case 6:
		      memset (endian8.bytes, 0x00, 8);
		      if (variant->little_endian_cpu)
			  endian8.bytes[0] = c;
		      else
			  endian8.bytes[7] = c;
		      v64 = endian8.uint64_value << 42;
		      v64 &= READOSM_MASK64_7;
		      value64 |= v64;
		      break;
		  case 7:
		      memset (endian8.bytes, 0x00, 8);
		      if (variant->little_endian_cpu)
			  endian8.bytes[0] = c;
		      else
			  endian8.bytes[7] = c;
		      v64 = endian8.uint64_value << 49;
		      v64 &= READOSM_MASK64_8;
		      value64 |= v64;
		      break;
		  case 8:
		      memset (endian8.bytes, 0x00, 8);
		      if (variant->little_endian_cpu)
			  endian8.bytes[0] = c;
		      else
			  endian8.bytes[7] = c;
		      v64 = endian8.uint64_value << 56;
		      v64 &= READOSM_MASK64_9;
		      value64 |= v64;
		      break;
		  case 9:
		      memset (endian8.bytes, 0x00, 8);
		      if (variant->little_endian_cpu)
			  endian8.bytes[0] = c;
		      else
			  endian8.bytes[7] = c;
		      v64 = endian8.uint64_value << 63;
		      v64 &= READOSM_MASK64_A;
		      value64 |= v64;
		      break;
		  default:
		      return NULL;
		  };
		break;
	    };
	  count++;
	  if (!next)
	      break;
      }

    switch (variant->type)
      {
      case READOSM_VAR_INT32:
	  variant->value.int32_value = (int) value32;
	  variant->valid = 1;
	  return ptr;
      case READOSM_VAR_UINT32:
	  variant->value.uint32_value = value32;
	  variant->valid = 1;
	  return ptr;
      case READOSM_VAR_SINT32:
	  if ((value32 & 0x00000001) == 0)
	      neg = 1;
	  else
	      neg = -1;
	  v32 = (value32 + 1) / 2;
	  variant->value.int32_value = v32 * neg;
	  variant->valid = 1;
	  return ptr;
      case READOSM_VAR_INT64:
	  variant->value.int64_value = (int) value64;
	  variant->valid = 1;
	  return ptr;
      case READOSM_VAR_UINT64:
	  variant->value.uint64_value = value64;
	  variant->valid = 1;
	  return ptr;
      case READOSM_VAR_SINT64:
	  if ((value64 & 0x0000000000000001) == 0)
	      neg = 1;
	  else
	      neg = -1;
	  v64 = (value64 + 1) / 2;
	  variant->value.int64_value = v64 * neg;
	  variant->valid = 1;
	  return ptr;
      };
    return NULL;
}

static unsigned char *
read_bytes (unsigned char *start, unsigned char *stop,
	    readosm_variant * variant)
{
/* 
 / attempting to read some bytes from PBF
 / Strings and alike are encoded in PBF using a two steps approach:
 / - an INT32 field declares the expected length
 / - then the string (no terminating NULL char) follows
*/
    unsigned char *ptr = start;
    readosm_variant varlen;
    unsigned int len;

/* initializing an empty variant field (length) */
    init_variant (&varlen, variant->little_endian_cpu);
    varlen.type = READOSM_VAR_UINT32;

    ptr = read_var (ptr, stop, &varlen);
    if (varlen.valid)
      {
	  len = varlen.value.uint32_value;
	  if ((ptr + len - 1) > stop)
	      return NULL;
	  variant->pointer = ptr;
	  variant->length = len;
	  variant->valid = 1;
	  return ptr + len;
      }
    return NULL;
}

static int
parse_uint32_packed (readosm_uint32_packed * packed, unsigned char *start,
		     unsigned char *stop, char little_endian_cpu)
{
/* parsing a uint32 packed object */
    unsigned char *ptr = start;
    readosm_variant variant;

/* initializing an empty variant field (length) */
    init_variant (&variant, little_endian_cpu);
    variant.type = READOSM_VAR_UINT32;

    while (1)
      {
	  ptr = read_var (start, stop, &variant);
	  if (variant.valid)
	    {
		append_uint32_packed (packed, variant.value.uint32_value);
		if (ptr > stop)
		    break;
		start = ptr;
		continue;
	    }
	  return 0;
      }
    return 1;
}

static int
parse_sint32_packed (readosm_int32_packed * packed, unsigned char *start,
		     unsigned char *stop, char little_endian_cpu)
{
/* parsing an int32 packed object */
    unsigned char *ptr = start;
    readosm_variant variant;

/* initializing an empty variant field (length) */
    init_variant (&variant, little_endian_cpu);
    variant.type = READOSM_VAR_SINT32;

    while (1)
      {
	  ptr = read_var (start, stop, &variant);
	  if (variant.valid)
	    {
		append_int32_packed (packed, variant.value.int32_value);
		if (ptr > stop)
		    break;
		start = ptr;
		continue;
	    }
	  return 0;
      }
    return 1;
}

static int
parse_sint64_packed (readosm_int64_packed * packed, unsigned char *start,
		     unsigned char *stop, char little_endian_cpu)
{
/* parsing a sint64 packed object */
    unsigned char *ptr = start;
    readosm_variant variant;

/* initializing an empty variant field (length) */
    init_variant (&variant, little_endian_cpu);
    variant.type = READOSM_VAR_SINT64;

    while (1)
      {
	  ptr = read_var (start, stop, &variant);
	  if (variant.valid)
	    {
		append_int64_packed (packed, variant.value.int64_value);
		if (ptr > stop)
		    break;
		start = ptr;
		continue;
	    }
	  return 0;
      }
    return 1;
}

static unsigned int
get_header_size (unsigned char *buf, int little_endian_cpu)
{
/* 
 / retrieving the current header size 
 / please note: header sizes in PBF always are 4 bytes
 / BIG endian encoded
*/
    readosm_endian4 endian4;
    if (little_endian_cpu)
      {
	  endian4.bytes[0] = *(buf + 3);
	  endian4.bytes[1] = *(buf + 2);
	  endian4.bytes[2] = *(buf + 1);
	  endian4.bytes[3] = *(buf + 0);
      }
    else
      {
	  endian4.bytes[0] = *(buf + 0);
	  endian4.bytes[1] = *(buf + 1);
	  endian4.bytes[2] = *(buf + 2);
	  endian4.bytes[3] = *(buf + 3);
      }
    return endian4.uint32_value;
}

static unsigned char *
parse_field (unsigned char *start, unsigned char *stop,
	     readosm_variant * variant)
{
/* attempting to parse a variant field */
    unsigned char *ptr = start;
    unsigned char type;
    unsigned char field_id;
    unsigned char type_hint;

    if (ptr > stop)
	return NULL;

/*
 / any PBF field is prefixed by a single byte
 / a bitwise mask is used so to store both the
 / field-id and the field-type on a single byte
*/
    type = *ptr & 0x07;
    field_id = (*ptr & 0xf8) >> 3;

/* attempting to identify the field accordingly to declared hints */
    if (!find_type_hint (variant, field_id, type, &type_hint))
	return NULL;

    variant->type = type_hint;
    variant->field_id = field_id;
    ptr++;

/* parsing the field value */
    switch (variant->type)
      {
      case READOSM_VAR_INT32:
      case READOSM_VAR_INT64:
      case READOSM_VAR_UINT32:
      case READOSM_VAR_UINT64:
      case READOSM_VAR_SINT32:
      case READOSM_VAR_SINT64:
	  return read_var (ptr, stop, variant);
      case READOSM_LEN_BYTES:
	  return read_bytes (ptr, stop, variant);
      };
    return NULL;
}

static int
skip_osm_header (readosm_file * input, unsigned int sz)
{
/*
 / expecting to retrieve a valid OSMHeader header 
 / there is nothing really interesting here, so we'll
 / simply discard the whole block, simply advancing
 / the read file-pointer as appropriate
*/
    int ok_header = 0;
    int hdsz = 0;
    size_t rd;
    unsigned char *buf = malloc (sz);
    unsigned char *base = buf;
    unsigned char *start = buf;
    unsigned char *stop = buf + sz - 1;
    readosm_variant variant;
    if (buf == NULL)
	goto error;

/* initializing an empty variant field */
    init_variant (&variant, input->little_endian_cpu);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 1);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 2);
    add_variant_hints (&variant, READOSM_VAR_INT32, 3);

    rd = fread (buf, 1, sz, input->in);
    if (rd != sz)
	goto error;

/* reading the OSMHeader header */
    while (1)
      {
	  /* resetting an empty variant field */
	  reset_variant (&variant);

	  base = parse_field (start, stop, &variant);
	  if (base == NULL && variant.valid == 0)
	      goto error;
	  start = base;
	  if (variant.field_id == 1 && variant.type == READOSM_LEN_BYTES
	      && variant.length == 9)
	    {
		if (memcmp (variant.pointer, "OSMHeader", 9) == 0)
		    ok_header = 1;
	    }
	  if (variant.field_id == 3 && variant.type == READOSM_VAR_INT32)
	      hdsz = variant.value.int32_value;
	  if (base > stop)
	      break;
      }
    free (buf);
    buf = NULL;
    if (!ok_header || !hdsz)
	goto error;

    buf = malloc (hdsz);
    base = buf;
    start = buf;
    stop = buf + hdsz - 1;
    rd = fread (buf, 1, hdsz, input->in);
    if ((int) rd != hdsz)
	goto error;

    if (buf != NULL)
	free (buf);
    finalize_variant (&variant);
    return 1;

  error:
    if (buf != NULL)
	free (buf);
    finalize_variant (&variant);
    return 0;
}

static int
unzip_compressed_block (unsigned char *zip_ptr, unsigned int zip_sz,
			unsigned char *raw_ptr, unsigned int raw_sz)
{
/* 
 / decompressing a zip compressed block 
 / please note: PBF data blocks are internally stored as
 / ZIP compessed blocks
 /
 / both the compressed and uncompressed sizes are declared
 / for each PBF ZIPped block
*/
    uLongf size = raw_sz;
    int ret = uncompress (raw_ptr, &size, zip_ptr, zip_sz);
    if (ret != Z_OK || size != raw_sz)
	return 0;
    return 1;
}

static int
parse_string_table (readosm_string_table * string_table,
		    unsigned char *start, unsigned char *stop,
		    char little_endian_cpu)
{
/* 
 / attempting to parse a StringTable 
 / 
 / Remark: each PBF compressed block includes a StringTable
 / i.e. a centralized table where any string value used within
 / the compressed block itself appears only one time.
 / This is obviously intended so to minimize storage requirements.
 /
 / Individual objects within the PBF file will never directly
 / encode string values; they'll use instead the corresponding
 / index referencing the appropriate string within the StringTable.
*/
    readosm_variant variant;
    unsigned char *base = start;

/* initializing an empty variant field */
    init_variant (&variant, little_endian_cpu);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 1);

/* reading the StringTable */
    while (1)
      {
	  /* resetting an empty variant field */
	  reset_variant (&variant);

	  base = parse_field (start, stop, &variant);
	  if (base == NULL && variant.valid == 0)
	      goto error;
	  start = base;
	  if (variant.field_id == 1 && variant.type == READOSM_LEN_BYTES)
	      append_string_to_table (string_table, &variant);
	  if (base > stop)
	      break;
      }

    finalize_variant (&variant);
    return 1;

  error:
    finalize_variant (&variant);
    return 0;
}

static int
parse_pbf_node_infos (readosm_packed_infos * packed_infos,
		      unsigned char *start, unsigned char *stop,
		      char little_endian_cpu)
{
/* 
 / attempting to parse a valid PBF DenseInfos
 /
 / Remark: PBF DenseNodes blocks require a DenseInfos block
 / this actually consists in six strings:
 / - versions
 / - timestamps
 / - changesets
 / - uids
 / - user-names (expressed as index to StringTable entries)
 /
 / each "string" in turn contains an array of INT values;
 / and individual values are usually encoded as DELTAs,
 / i.e. differences respect the immediately preceding value.
*/
    readosm_variant variant;
    unsigned char *base = start;
    readosm_uint32_packed packed_u32;
    readosm_uint32 *pu32;
    readosm_int32_packed packed_32;
    readosm_int32 *p32;
    readosm_int64_packed packed_64;
    readosm_int64 *p64;
    int count;

/* initializing empty packed objects */
    init_uint32_packed (&packed_u32);
    init_int32_packed (&packed_32);
    init_int64_packed (&packed_64);

/* initializing an empty variant field */
    init_variant (&variant, little_endian_cpu);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 1);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 2);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 3);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 4);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 5);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 6);

/* reading the DenseInfo block */
    while (1)
      {
	  /* resetting an empty variant field */
	  reset_variant (&variant);

	  base = parse_field (start, stop, &variant);
	  if (base == NULL && variant.valid == 0)
	      goto error;
	  start = base;
	  if (variant.field_id == 1 && variant.type == READOSM_LEN_BYTES)
	    {
		/* versions: *not* delta encoded */
		if (!parse_uint32_packed
		    (&packed_u32, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		count = 0;
		pu32 = packed_u32.first;
		while (pu32)
		  {
		      count++;
		      pu32 = pu32->next;
		  }
		packed_infos->ver_count = count;
		if (packed_infos->versions != NULL)
		  {
		      free (packed_infos->versions);
		      packed_infos->versions = NULL;
		  }
		if (count > 0)
		  {
		      packed_infos->versions = malloc (sizeof (int) * count);
		      count = 0;
		      pu32 = packed_u32.first;
		      while (pu32)
			{
			    *(packed_infos->versions + count) = pu32->value;
			    count++;
			    pu32 = pu32->next;
			}
		  }
		reset_uint32_packed (&packed_u32);
	    }
	  if (variant.field_id == 2 && variant.type == READOSM_LEN_BYTES)
	    {
		/* timestamps: delta encoded */
		int delta = 0;
		if (!parse_sint32_packed
		    (&packed_32, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		count = 0;
		p32 = packed_32.first;
		while (p32)
		  {
		      count++;
		      p32 = p32->next;
		  }
		packed_infos->tim_count = count;
		if (packed_infos->timestamps != NULL)
		  {
		      free (packed_infos->timestamps);
		      packed_infos->timestamps = NULL;
		  }
		if (count > 0)
		  {
		      packed_infos->timestamps = malloc (sizeof (int) * count);
		      count = 0;
		      p32 = packed_32.first;
		      while (p32)
			{
			    delta += p32->value;
			    *(packed_infos->timestamps + count) = delta;
			    count++;
			    p32 = p32->next;
			}
		  }
		reset_int32_packed (&packed_32);
	    }
	  if (variant.field_id == 3 && variant.type == READOSM_LEN_BYTES)
	    {
		/* changesets: delta encoded */
		long long delta = 0;
		if (!parse_sint64_packed
		    (&packed_64, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		count = 0;
		p64 = packed_64.first;
		while (p64)
		  {
		      count++;
		      p64 = p64->next;
		  }
		packed_infos->cng_count = count;
		if (packed_infos->changesets != NULL)
		  {
		      free (packed_infos->changesets);
		      packed_infos->changesets = NULL;
		  }
		if (count > 0)
		  {
		      packed_infos->changesets =
			  malloc (sizeof (long long) * count);
		      count = 0;
		      p64 = packed_64.first;
		      while (p64)
			{
			    delta += p64->value;
			    *(packed_infos->changesets + count) = delta;
			    count++;
			    p64 = p64->next;
			}
		  }
		reset_int64_packed (&packed_64);
	    }
	  if (variant.field_id == 4 && variant.type == READOSM_LEN_BYTES)
	    {
		/* uids: delta encoded */
		int delta = 0;
		if (!parse_sint32_packed
		    (&packed_32, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		count = 0;
		p32 = packed_32.first;
		while (p32)
		  {
		      count++;
		      p32 = p32->next;
		  }
		packed_infos->uid_count = count;
		if (packed_infos->uids != NULL)
		  {
		      free (packed_infos->uids);
		      packed_infos->uids = NULL;
		  }
		if (count > 0)
		  {
		      packed_infos->uids = malloc (sizeof (int) * count);
		      count = 0;
		      p32 = packed_32.first;
		      while (p32)
			{
			    delta += p32->value;
			    *(packed_infos->uids + count) = delta;
			    count++;
			    p32 = p32->next;
			}
		  }
		reset_int32_packed (&packed_32);
	    }
	  if (variant.field_id == 5 && variant.type == READOSM_LEN_BYTES)
	    {
		/* user-names: delta encoded (index to StringTable) */
		int delta = 0;
		if (!parse_sint32_packed
		    (&packed_32, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		count = 0;
		p32 = packed_32.first;
		while (p32)
		  {
		      count++;
		      p32 = p32->next;
		  }
		packed_infos->usr_count = count;
		if (packed_infos->users != NULL)
		  {
		      free (packed_infos->users);
		      packed_infos->users = NULL;
		  }
		if (count > 0)
		  {
		      packed_infos->users = malloc (sizeof (int) * count);
		      count = 0;
		      p32 = packed_32.first;
		      while (p32)
			{
			    delta += p32->value;
			    *(packed_infos->users + count) = delta;
			    count++;
			    p32 = p32->next;
			}
		  }
		reset_int32_packed (&packed_32);
	    }
	  if (base > stop)
	      break;
      }
    finalize_uint32_packed (&packed_u32);
    finalize_int32_packed (&packed_32);
    finalize_int64_packed (&packed_64);
    finalize_variant (&variant);
    return 1;

  error:
    finalize_variant (&variant);
    finalize_uint32_packed (&packed_u32);
    finalize_int32_packed (&packed_32);
    finalize_int64_packed (&packed_64);
    return 0;
}

static int
parse_pbf_nodes (readosm_string_table * strings,
		 unsigned char *start, unsigned char *stop,
		 char little_endian_cpu, struct pbf_params *params)
{
/* 
 / attempting to parse a valid PBF DenseNodes 
 /
 / Remark: a PBF DenseNodes block consists in five strings:
 / - ids
 / - DenseInfos
 / - longitudes
 / - latitudes
 / - packed-keys (*)
 /
 / each "string" in turn contains an array of INT values;
 / and individual values are usually encoded as DELTAs,
 / i.e. differences respect the immediately preceding value.
 /
 / (*) packed keys actually are encoded as arrays of index
 / to StringTable entries.
 / alternatively we have a key-index and then a value-index;
 / any 0 value means that the current Node stops: next index
 / will be a key-index for the next Node item
*/
    readosm_variant variant;
    unsigned char *base = start;
    readosm_uint32_packed packed_keys;
    readosm_int64_packed packed_ids;
    readosm_int64_packed packed_lats;
    readosm_int64_packed packed_lons;
    readosm_packed_infos packed_infos;
    readosm_internal_node *nodes = NULL;
    int nd_count = 0;
    int valid = 0;
    int fromPackedInfos = 0;

/* initializing empty packed objects */
    init_uint32_packed (&packed_keys);
    init_int64_packed (&packed_ids);
    init_int64_packed (&packed_lats);
    init_int64_packed (&packed_lons);
    init_packed_infos (&packed_infos);

/* initializing an empty variant field */
    init_variant (&variant, little_endian_cpu);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 1);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 5);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 8);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 9);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 10);

/* reading the Node */
    while (1)
      {
	  /* resetting an empty variant field */
	  reset_variant (&variant);

	  base = parse_field (start, stop, &variant);
	  if (base == NULL && variant.valid == 0)
	      goto error;
	  start = base;
	  if (variant.field_id == 1 && variant.type == READOSM_LEN_BYTES)
	    {
		/* NODE IDs */
		if (!parse_sint64_packed
		    (&packed_ids, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		array_from_int64_packed (&packed_ids);
	    }
	  if (variant.field_id == 5 && variant.type == READOSM_LEN_BYTES)
	    {
		/* DenseInfos */
		if (!parse_pbf_node_infos (&packed_infos,
					   variant.pointer,
					   variant.pointer + variant.length - 1,
					   variant.little_endian_cpu))
		    goto error;
	    }
	  if (variant.field_id == 8 && variant.type == READOSM_LEN_BYTES)
	    {
		/* latitudes */
		if (!parse_sint64_packed
		    (&packed_lats, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		array_from_int64_packed (&packed_lats);
	    }
	  if (variant.field_id == 9 && variant.type == READOSM_LEN_BYTES)
	    {
		/* longitudes */
		if (!parse_sint64_packed
		    (&packed_lons, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		array_from_int64_packed (&packed_lons);
	    }
	  if (variant.field_id == 10 && variant.type == READOSM_LEN_BYTES)
	    {
		/* packes-keys */
		if (!parse_uint32_packed
		    (&packed_keys, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		array_from_uint32_packed (&packed_keys);
	    }
	  if (base > stop)
	      break;
      }
    if (packed_ids.count == packed_lats.count
	&& packed_ids.count == packed_lons.count)
      {
	  /* not using PackedInfos */
	  valid = 1;
      }
    if (packed_ids.count == packed_lats.count
	&& packed_ids.count == packed_lons.count
	&& packed_ids.count == packed_infos.ver_count
	&& packed_ids.count == packed_infos.tim_count
	&& packed_ids.count == packed_infos.cng_count
	&& packed_ids.count == packed_infos.uid_count
	&& packed_ids.count == packed_infos.usr_count)
      {
	  /* from PackedInfos */
	  valid = 1;
	  fromPackedInfos = 1;
      }
    if (!valid)
	goto error;
    else
      {
	  /*
	     / all right, we now have the same item count anywhere
	     / we can now go further away attempting to reassemble
	     / individual Nodes 
	   */
	  readosm_internal_node *nd;
	  int i;
	  int i_keys = 0;
	  long long delta_id = 0;
	  long long delta_lat = 0;
	  long long delta_lon = 0;
	  int max_nodes;
	  int base = 0;
	  nd_count = packed_ids.count;
	  while (base < nd_count)
	    {
		/* processing about 1024 nodes at each time */
		max_nodes = MAX_NODES;
		if ((nd_count - base) < MAX_NODES)
		    max_nodes = nd_count - base;
		nodes = malloc (sizeof (readosm_internal_node) * max_nodes);
		for (i = 0; i < max_nodes; i++)
		  {
		      /* initializing an array of empty internal Nodes */
		      nd = nodes + i;
		      init_internal_node (nd);
		  }
		for (i = 0; i < max_nodes; i++)
		  {
		      /* reassembling internal Nodes */
		      const char *key = NULL;
		      const char *value = NULL;
		      time_t xtime;
		      struct tm *times;
		      int s_id;
		      nd = nodes + i;
		      delta_id += *(packed_ids.values + base + i);
		      delta_lat += *(packed_lats.values + base + i);
		      delta_lon += *(packed_lons.values + base + i);
		      nd->id = delta_id;
		      /* latitudes and longitudes require to be rescaled as DOUBLEs */
		      nd->latitude = delta_lat / 10000000.0;
		      nd->longitude = delta_lon / 10000000.0;
		      if (fromPackedInfos)
			{
			    nd->version = *(packed_infos.versions + base + i);
			    xtime = *(packed_infos.timestamps + base + i);
			    times = gmtime (&xtime);
			    if (times)
			      {
				  /* formatting Timestamps */
				  char buf[64];
				  int len;
				  sprintf (buf,
					   "%04d-%02d-%02dT%02d:%02d:%02dZ",
					   times->tm_year + 1900,
					   times->tm_mon + 1, times->tm_mday,
					   times->tm_hour, times->tm_min,
					   times->tm_sec);
				  if (nd->timestamp)
				      free (nd->timestamp);
				  len = strlen (buf);
				  nd->timestamp = malloc (len + 1);
				  strcpy (nd->timestamp, buf);
			      }
			    nd->changeset =
				*(packed_infos.changesets + base + i);
			    if (*(packed_infos.uids + base + i) >= 0)
				nd->uid = *(packed_infos.uids + base + i);
			    s_id = *(packed_infos.users + base + i);
			    if (s_id > 0)
			      {
				  /* retrieving user-names as strings (by index) */
				  readosm_string *s_ptr =
				      *(strings->strings + s_id);
				  int len = strlen (s_ptr->string);
				  if (nd->user != NULL)
				      free (nd->user);
				  if (len > 0)
				    {
					nd->user = malloc (len + 1);
					strcpy (nd->user, s_ptr->string);
				    }
			      }
			}
		      for (; i_keys < packed_keys.count; i_keys++)
			{
			    /* decoding packed-keys */
			    int is = *(packed_keys.values + i_keys);
			    if (is == 0)
			      {
				  /* next Node */
				  i_keys++;
				  break;
			      }
			    if (key == NULL)
			      {
				  readosm_string *s_ptr =
				      *(strings->strings + is);
				  key = s_ptr->string;
			      }
			    else
			      {
				  readosm_string *s_ptr =
				      *(strings->strings + is);
				  value = s_ptr->string;
				  append_tag_to_node (nd, key, value);
				  key = NULL;
				  value = NULL;
			      }
			}
		  }
		base += max_nodes;

		/* processing each Node in the block */
		if (params->node_callback != NULL && params->stop == 0)
		  {
		      int ret;
		      readosm_internal_node *nd;
		      int i;
		      for (i = 0; i < max_nodes; i++)
			{
			    nd = nodes + i;
			    ret =
				call_node_callback (params->node_callback,
						    params->user_data, nd);
			    if (ret != READOSM_OK)
			      {
				  params->stop = 1;
				  break;
			      }
			}
		  }

		/* memory cleanup: destroying Nodes */
		if (nodes != NULL)
		  {
		      readosm_internal_node *nd;
		      int i;
		      for (i = 0; i < max_nodes; i++)
			{
			    nd = nodes + i;
			    destroy_internal_node (nd);
			}
		      free (nodes);
		  }
	    }
      }

/* memory cleanup */
    finalize_uint32_packed (&packed_keys);
    finalize_int64_packed (&packed_ids);
    finalize_int64_packed (&packed_lats);
    finalize_int64_packed (&packed_lons);
    finalize_packed_infos (&packed_infos);
    finalize_variant (&variant);
    return 1;

  error:
    finalize_uint32_packed (&packed_keys);
    finalize_int64_packed (&packed_ids);
    finalize_int64_packed (&packed_lats);
    finalize_int64_packed (&packed_lons);
    finalize_packed_infos (&packed_infos);
    finalize_variant (&variant);
    if (nodes != NULL)
      {
	  readosm_internal_node *nd;
	  int i;
	  for (i = 0; i < nd_count; i++)
	    {
		nd = nodes + i;
		destroy_internal_node (nd);
	    }
	  free (nodes);
      }
    return 0;
}

static int
parse_pbf_way_info (readosm_internal_way * way, readosm_string_table * strings,
		    unsigned char *start, unsigned char *stop,
		    char little_endian_cpu)
{
/* attempting to parse a valid PBF Way-Info */
    readosm_variant variant;
    unsigned char *base = start;

/* initializing an empty variant field */
    init_variant (&variant, little_endian_cpu);
    add_variant_hints (&variant, READOSM_VAR_INT32, 1);
    add_variant_hints (&variant, READOSM_VAR_INT32, 2);
    add_variant_hints (&variant, READOSM_VAR_INT64, 3);
    add_variant_hints (&variant, READOSM_VAR_INT32, 4);
    add_variant_hints (&variant, READOSM_VAR_INT32, 5);
    add_variant_hints (&variant, READOSM_VAR_INT32, 6);

/* reading the WayInfo */
    while (1)
      {
	  /* resetting an empty variant field */
	  reset_variant (&variant);

	  base = parse_field (start, stop, &variant);
	  if (base == NULL && variant.valid == 0)
	      goto error;
	  start = base;
	  if (variant.field_id == 1 && variant.type == READOSM_VAR_INT32)
	    {
		/* version */
		way->version = variant.value.int32_value;
	    }
	  if (variant.field_id == 2 && variant.type == READOSM_VAR_INT32)
	    {
		/* timestamp */
		const time_t xtime = variant.value.int32_value;
		struct tm *times = gmtime (&xtime);
		if (times)
		  {
		      char buf[64];
		      int len;
		      sprintf (buf, "%04d-%02d-%02dT%02d:%02d:%02dZ",
			       times->tm_year + 1900, times->tm_mon + 1,
			       times->tm_mday, times->tm_hour, times->tm_min,
			       times->tm_sec);
		      if (way->timestamp)
			  free (way->timestamp);
		      len = strlen (buf);
		      way->timestamp = malloc (len + 1);
		      strcpy (way->timestamp, buf);
		  }
	    }
	  if (variant.field_id == 3 && variant.type == READOSM_VAR_INT64)
	    {
		/* changeset */
		way->changeset = variant.value.int64_value;
	    }
	  if (variant.field_id == 4 && variant.type == READOSM_VAR_INT32)
	    {
		/* uid */
		way->uid = variant.value.int32_value;
	    }
	  if (variant.field_id == 5 && variant.type == READOSM_VAR_INT32)
	    {
		/* user-name: index to StringTable entry */
		int userid;
		if (way->user)
		    free (way->user);
		way->user = NULL;
		userid = variant.value.int32_value;
		if (userid > 0 && userid < strings->count)
		  {
		      readosm_string *string = *(strings->strings + userid);
		      int len = strlen (string->string);
		      way->user = malloc (len + 1);
		      strcpy (way->user, string->string);
		  }
	    }
	  if (base > stop)
	      break;
      }
    finalize_variant (&variant);
    return 1;

  error:
    finalize_variant (&variant);
    return 0;
}

static int
parse_pbf_way (readosm_string_table * strings,
	       unsigned char *start, unsigned char *stop,
	       char little_endian_cpu, struct pbf_params *params)
{
/* attempting to parse a valid PBF Way */
    readosm_variant variant;
    unsigned char *base = start;
    readosm_uint32_packed packed_keys;
    readosm_uint32_packed packed_values;
    readosm_int64_packed packed_refs;
    readosm_internal_way *way = alloc_internal_way ();

/* initializing empty packed objects */
    init_uint32_packed (&packed_keys);
    init_uint32_packed (&packed_values);
    init_int64_packed (&packed_refs);

/* initializing an empty variant field */
    init_variant (&variant, little_endian_cpu);
    add_variant_hints (&variant, READOSM_VAR_INT64, 1);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 2);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 3);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 4);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 8);

/* reading the Way */
    while (1)
      {
	  /* resetting an empty variant field */
	  reset_variant (&variant);

	  base = parse_field (start, stop, &variant);
	  if (base == NULL && variant.valid == 0)
	      goto error;
	  start = base;
	  if (variant.field_id == 1 && variant.type == READOSM_VAR_INT64)
	    {
		/* WAY ID */
		way->id = variant.value.int64_value;
	    }
	  if (variant.field_id == 2 && variant.type == READOSM_LEN_BYTES)
	    {
		/* KEYs are encoded as an array of StringTable index */
		if (!parse_uint32_packed
		    (&packed_keys, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		array_from_uint32_packed (&packed_keys);
	    }
	  if (variant.field_id == 3 && variant.type == READOSM_LEN_BYTES)
	    {
		/* VALUEs are encoded as an array of StringTable index  */
		if (!parse_uint32_packed
		    (&packed_values, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		array_from_uint32_packed (&packed_values);
	    }
	  if (variant.field_id == 4 && variant.type == READOSM_LEN_BYTES)
	    {
		/* WAY-INFO block */
		if (!parse_pbf_way_info
		    (way, strings, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
	    }
	  if (variant.field_id == 8 && variant.type == READOSM_LEN_BYTES)
	    {
		/* NODE-REFs */
		long long delta = 0;
		readosm_int64 *value;
		/* KEYs are encoded as an array of StringTable index */
		if (!parse_sint64_packed
		    (&packed_refs, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		value = packed_refs.first;
		while (value != NULL)
		  {
		      /* appending Node references to Way */
		      delta += value->value;
		      append_reference_to_way (way, delta);
		      value = value->next;
		  }
	    }
	  if (base > stop)
	      break;
      }

/* reassembling a WAY object */
    if (packed_keys.count == packed_values.count)
      {
	  int i;
	  for (i = 0; i < packed_keys.count; i++)
	    {
		int i_key = *(packed_keys.values + i);
		int i_val = *(packed_values.values + i);
		readosm_string *s_key = *(strings->strings + i_key);
		readosm_string *s_value = *(strings->strings + i_val);
		append_tag_to_way (way, s_key->string, s_value->string);
	    }
      }
    else
	goto error;

    finalize_uint32_packed (&packed_keys);
    finalize_uint32_packed (&packed_values);
    finalize_int64_packed (&packed_refs);
    finalize_variant (&variant);

/* processing the WAY */
    if (params->way_callback != NULL && params->stop == 0)
      {
	  int ret =
	      call_way_callback (params->way_callback, params->user_data, way);
	  if (ret != READOSM_OK)
	      params->stop = 1;
      }
    destroy_internal_way (way);
    return 1;

  error:
    finalize_uint32_packed (&packed_keys);
    finalize_uint32_packed (&packed_values);
    finalize_int64_packed (&packed_refs);
    finalize_variant (&variant);
    destroy_internal_way (way);
    return 0;
}

static int
parse_pbf_relation_info (readosm_internal_relation * relation,
			 readosm_string_table * strings, unsigned char *start,
			 unsigned char *stop, char little_endian_cpu)
{
/* attempting to parse a valid PBF RelationInfo */
    readosm_variant variant;
    unsigned char *base = start;

/* initializing an empty variant field */
    init_variant (&variant, little_endian_cpu);
    add_variant_hints (&variant, READOSM_VAR_INT32, 1);
    add_variant_hints (&variant, READOSM_VAR_INT32, 2);
    add_variant_hints (&variant, READOSM_VAR_INT64, 3);
    add_variant_hints (&variant, READOSM_VAR_INT32, 4);
    add_variant_hints (&variant, READOSM_VAR_INT32, 5);
    add_variant_hints (&variant, READOSM_VAR_INT32, 6);

/* reading the RelationInfo */
    while (1)
      {
	  /* resetting an empty variant field */
	  reset_variant (&variant);

	  base = parse_field (start, stop, &variant);
	  if (base == NULL && variant.valid == 0)
	      goto error;
	  start = base;
	  if (variant.field_id == 1 && variant.type == READOSM_VAR_INT32)
	    {
		/* version */
		relation->version = variant.value.int32_value;
	    }
	  if (variant.field_id == 2 && variant.type == READOSM_VAR_INT32)
	    {
		/* timestamp */
		const time_t xtime = variant.value.int32_value;
		struct tm *times = gmtime (&xtime);
		if (times)
		  {
		      char buf[64];
		      int len;
		      sprintf (buf, "%04d-%02d-%02dT%02d:%02d:%02dZ",
			       times->tm_year + 1900, times->tm_mon + 1,
			       times->tm_mday, times->tm_hour, times->tm_min,
			       times->tm_sec);
		      if (relation->timestamp)
			  free (relation->timestamp);
		      len = strlen (buf);
		      relation->timestamp = malloc (len + 1);
		      strcpy (relation->timestamp, buf);
		  }
	    }
	  if (variant.field_id == 3 && variant.type == READOSM_VAR_INT64)
	    {
		/* changeset */
		relation->changeset = variant.value.int64_value;
	    }
	  if (variant.field_id == 4 && variant.type == READOSM_VAR_INT32)
	    {
		/* uid */
		relation->uid = variant.value.int32_value;
	    }
	  if (variant.field_id == 5 && variant.type == READOSM_VAR_INT32)
	    {
		/* user-name: index to StringTable entry */
		int userid;
		if (relation->user)
		    free (relation->user);
		relation->user = NULL;
		userid = variant.value.int32_value;
		if (userid > 0 && userid < strings->count)
		  {
		      readosm_string *string = *(strings->strings + userid);
		      int len = strlen (string->string);
		      relation->user = malloc (len + 1);
		      strcpy (relation->user, string->string);
		  }
	    }
	  if (base > stop)
	      break;
      }
    finalize_variant (&variant);
    return 1;

  error:
    finalize_variant (&variant);
    return 0;
}

static int
parse_pbf_relation (readosm_string_table * strings,
		    unsigned char *start, unsigned char *stop,
		    char little_endian_cpu, struct pbf_params *params)
{
/* attempting to parse a valid PBF Relation */
    readosm_variant variant;
    unsigned char *base = start;
    readosm_uint32_packed packed_keys;
    readosm_uint32_packed packed_values;
    readosm_uint32_packed packed_roles;
    readosm_uint32_packed packed_types;
    readosm_int64_packed packed_refs;
    readosm_internal_relation *relation = alloc_internal_relation ();

/* initializing empty packed objects */
    init_uint32_packed (&packed_keys);
    init_uint32_packed (&packed_values);
    init_uint32_packed (&packed_roles);
    init_uint32_packed (&packed_types);
    init_int64_packed (&packed_refs);

/* initializing an empty variant field */
    init_variant (&variant, little_endian_cpu);
    add_variant_hints (&variant, READOSM_VAR_INT64, 1);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 2);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 3);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 4);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 8);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 9);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 10);

/* reading the Relation */
    while (1)
      {
	  /* resetting an empty variant field */
	  reset_variant (&variant);

	  base = parse_field (start, stop, &variant);
	  if (base == NULL && variant.valid == 0)
	      goto error;
	  start = base;
	  if (variant.field_id == 1 && variant.type == READOSM_VAR_INT64)
	    {
		/* RELATION ID */
		relation->id = variant.value.int64_value;
	    }
	  if (variant.field_id == 2 && variant.type == READOSM_LEN_BYTES)
	    {
		/* KEYs are encoded as an array of StringTable index */
		if (!parse_uint32_packed
		    (&packed_keys, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		array_from_uint32_packed (&packed_keys);
	    }
	  if (variant.field_id == 3 && variant.type == READOSM_LEN_BYTES)
	    {
		/* VALUEs are encoded as an array of StringTable index */
		if (!parse_uint32_packed
		    (&packed_values, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		array_from_uint32_packed (&packed_values);
	    }
	  if (variant.field_id == 4 && variant.type == READOSM_LEN_BYTES)
	    {
		/* RELATION-INFO block */
		if (!parse_pbf_relation_info
		    (relation, strings, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
	    }
	  if (variant.field_id == 8 && variant.type == READOSM_LEN_BYTES)
	    {
		/* MEMBER-ROLEs are encoded as an array of StringTable index */
		if (!parse_uint32_packed
		    (&packed_roles, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		array_from_uint32_packed (&packed_roles);
	    }
	  if (variant.field_id == 9 && variant.type == READOSM_LEN_BYTES)
	    {
		/* MEMBER-REFs are encoded as an array */
		if (!parse_sint64_packed
		    (&packed_refs, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		array_from_int64_packed (&packed_refs);
	    }
	  if (variant.field_id == 10 && variant.type == READOSM_LEN_BYTES)
	    {
		/* MEMBER-TYPEs are encoded as an array */
		if (!parse_uint32_packed
		    (&packed_types, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		array_from_uint32_packed (&packed_types);
	    }
	  if (base > stop)
	      break;
      }

/* reassembling a RELATION object */
    if (packed_keys.count == packed_values.count)
      {
	  int i;
	  for (i = 0; i < packed_keys.count; i++)
	    {
		int i_key = *(packed_keys.values + i);
		int i_val = *(packed_values.values + i);
		readosm_string *s_key = *(strings->strings + i_key);
		readosm_string *s_value = *(strings->strings + i_val);
		append_tag_to_relation (relation, s_key->string,
					s_value->string);
	    }
      }
    else
	goto error;
    if (packed_roles.count == packed_refs.count
	&& packed_roles.count == packed_types.count)
      {
	  int i;
	  long long delta = 0;
	  for (i = 0; i < packed_roles.count; i++)
	    {
		int xtype = READOSM_UNDEFINED;
		int i_role = *(packed_roles.values + i);
		readosm_string *s_role = *(strings->strings + i_role);
		int type = *(packed_types.values + i);
		delta += *(packed_refs.values + i);
		if (type == 0)
		    xtype = READOSM_MEMBER_NODE;
		else if (type == 1)
		    xtype = READOSM_MEMBER_WAY;
		else if (type == 2)
		    xtype = READOSM_MEMBER_RELATION;
		append_member_to_relation (relation, xtype, delta,
					   s_role->string);
	    }
      }
    else
	goto error;

    finalize_uint32_packed (&packed_keys);
    finalize_uint32_packed (&packed_values);
    finalize_uint32_packed (&packed_roles);
    finalize_uint32_packed (&packed_types);
    finalize_int64_packed (&packed_refs);
    finalize_variant (&variant);

/* processing the RELATION */
    if (params->relation_callback != NULL && params->stop == 0)
      {
	  int ret = call_relation_callback (params->relation_callback,
					    params->user_data, relation);
	  if (ret != READOSM_OK)
	      params->stop = 1;
      }
    destroy_internal_relation (relation);
    return 1;

  error:
    finalize_uint32_packed (&packed_keys);
    finalize_uint32_packed (&packed_values);
    finalize_uint32_packed (&packed_roles);
    finalize_uint32_packed (&packed_types);
    finalize_int64_packed (&packed_refs);
    finalize_variant (&variant);
    destroy_internal_relation (relation);
    return 0;
}

static int
parse_primitive_group (readosm_string_table * strings,
		       unsigned char *start, unsigned char *stop,
		       char little_endian_cpu, struct pbf_params *params)
{
/* 
 / attempting to parse a valid Primitive Group 
 /
 / each PBF PrimitiveGroup can store only one type:
 / - NODEs
 / - WAYs
 / - RELATIONs
*/
    readosm_variant variant;
    unsigned char *base = start;

/* initializing an empty variant field */
    init_variant (&variant, little_endian_cpu);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 1);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 2);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 3);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 4);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 5);

/* reading the Primitive Group */
    while (1)
      {
	  /* resetting an empty variant field */
	  reset_variant (&variant);

	  base = parse_field (start, stop, &variant);
	  if (base == NULL && variant.valid == 0)
	      goto error;
	  start = base;
	  if (variant.field_id == 2 && variant.type == READOSM_LEN_BYTES)
	    {
		/* DenseNodes */
		if (params->node_callback == NULL)
		    goto skip;	/* skipping: no node-callback */
		if (!parse_pbf_nodes
		    (strings, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu, params))
		    goto error;
	    }
	  if (variant.field_id == 3 && variant.type == READOSM_LEN_BYTES)
	    {
		/* Way */
		if (params->way_callback == NULL)
		    goto skip;	/* skipping: no way-callback */
		if (!parse_pbf_way
		    (strings, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu, params))
		    goto error;
	    }
	  if (variant.field_id == 4 && variant.type == READOSM_LEN_BYTES)
	    {
		/* Relation */
		if (params->relation_callback == NULL)
		    goto skip;	/* skipping: no relation-callback */
		if (!parse_pbf_relation
		    (strings, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu, params))
		    goto error;
	    }
	skip:
	  if (base > stop)
	      break;
      }
    finalize_variant (&variant);
    return 1;

  error:
    finalize_variant (&variant);
    return 0;
}

static int
parse_osm_data (readosm_file * input, unsigned int sz,
		struct pbf_params *params)
{
/* expecting to retrieve a valid OSMData header */
    int ok_header = 0;
    int hdsz = 0;
    size_t rd;
    unsigned char *buf = malloc (sz);
    unsigned char *base = buf;
    unsigned char *start = buf;
    unsigned char *stop = buf + sz - 1;
    unsigned char *zip_ptr = NULL;
    int zip_sz = 0;
    unsigned char *raw_ptr = NULL;
    int raw_sz = 0;
    readosm_variant variant;
    readosm_string_table string_table;
    if (buf == NULL)
	goto error;

/* initializing an empty string list */
    init_string_table (&string_table);

/* initializing an empty variant field */
    init_variant (&variant, input->little_endian_cpu);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 1);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 2);
    add_variant_hints (&variant, READOSM_VAR_INT32, 3);

    rd = fread (buf, 1, sz, input->in);
    if (rd != sz)
	goto error;

/* reading the OSMData header */
    while (1)
      {
	  /* resetting an empty variant field */
	  reset_variant (&variant);

	  base = parse_field (start, stop, &variant);
	  if (base == NULL && variant.valid == 0)
	      goto error;
	  start = base;
	  if (variant.field_id == 1 && variant.type == READOSM_LEN_BYTES
	      && variant.length == 7)
	    {
		if (memcmp (variant.pointer, "OSMData", 7) == 0)
		    ok_header = 1;
	    }
	  if (variant.field_id == 3 && variant.type == READOSM_VAR_INT32)
	      hdsz = variant.value.int32_value;
	  if (base > stop)
	      break;
      }
    free (buf);
    buf = NULL;
    if (!ok_header || !hdsz)
	goto error;

    buf = malloc (hdsz);
    base = buf;
    start = buf;
    stop = buf + hdsz - 1;
    rd = fread (buf, 1, hdsz, input->in);
    if ((int) rd != hdsz)
	goto error;

/* uncompressing the OSMData zipped */
    finalize_variant (&variant);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 1);
    add_variant_hints (&variant, READOSM_VAR_INT32, 2);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 3);
    while (1)
      {
	  /* resetting an empty variant field */
	  reset_variant (&variant);

	  base = parse_field (start, stop, &variant);
	  if (base == NULL && variant.valid == 0)
	      goto error;
	  start = base;
	  if (variant.field_id == 1 && variant.type == READOSM_LEN_BYTES)
	    {
		/* found an uncompressed block */
		raw_sz = variant.length;
		raw_ptr = malloc (raw_sz);
		memcpy (raw_ptr, variant.pointer, raw_sz);
	    }
	  if (variant.field_id == 2 && variant.type == READOSM_VAR_INT32)
	    {
		/* expected size of unZipped block */
		raw_sz = variant.value.int32_value;
	    }
	  if (variant.field_id == 3 && variant.type == READOSM_LEN_BYTES)
	    {
		/* found a ZIP-compressed block */
		zip_ptr = variant.pointer;
		zip_sz = variant.length;
	    }
	  if (base > stop)
	      break;
      }
    if (zip_ptr != NULL && zip_sz != 0 && raw_sz != 0)
      {
	  /* unZipping a compressed block */
	  raw_ptr = malloc (raw_sz);
	  if (!unzip_compressed_block (zip_ptr, zip_sz, raw_ptr, raw_sz))
	      goto error;
      }
    free (buf);
    buf = NULL;
    if (raw_ptr == NULL || raw_sz == 0)
	goto error;

/* parsing the PrimitiveBlock */
    base = raw_ptr;
    start = raw_ptr;
    stop = raw_ptr + raw_sz - 1;
    finalize_variant (&variant);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 1);
    add_variant_hints (&variant, READOSM_LEN_BYTES, 2);
    add_variant_hints (&variant, READOSM_VAR_INT32, 17);
    add_variant_hints (&variant, READOSM_VAR_INT32, 18);
    add_variant_hints (&variant, READOSM_VAR_INT64, 19);
    add_variant_hints (&variant, READOSM_VAR_INT64, 20);
    while (1)
      {
	  /* resetting an empty variant field */
	  reset_variant (&variant);

	  base = parse_field (start, stop, &variant);
	  if (base == NULL && variant.valid == 0)
	      goto error;
	  start = base;
	  if (variant.field_id == 1 && variant.type == READOSM_LEN_BYTES)
	    {
		/* the StringTable */
		if (!parse_string_table
		    (&string_table, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu))
		    goto error;
		array_from_string_table (&string_table);
	    }
	  if (variant.field_id == 2 && variant.type == READOSM_LEN_BYTES)
	    {
		/* the PrimitiveGroup to be parsed */
		if (!parse_primitive_group
		    (&string_table, variant.pointer,
		     variant.pointer + variant.length - 1,
		     variant.little_endian_cpu, params))
		    goto error;
	    }
	  if (variant.field_id == 17 && variant.type == READOSM_VAR_INT32)
	    {
		/* assumed to be a termination marker (???) */
		break;
	    }
	  if (base > stop)
	      break;
      }

    if (buf != NULL)
	free (buf);
    if (raw_ptr != NULL)
	free (raw_ptr);
    finalize_variant (&variant);
    finalize_string_table (&string_table);
    return 1;

  error:
    if (buf != NULL)
	free (buf);
    if (raw_ptr != NULL)
	free (raw_ptr);
    finalize_variant (&variant);
    finalize_string_table (&string_table);
    return 0;
}

READOSM_PRIVATE int
parse_osm_pbf (readosm_file * input, const void *user_data,
	       readosm_node_callback node_fnct, readosm_way_callback way_fnct,
	       readosm_relation_callback relation_fnct)
{
/* parsing the input file [OSM PBF format] */
    size_t rd;
    unsigned char buf[8];
    unsigned int hdsz;
    struct pbf_params params;

/* initializing the PBF helper structure */
    params.user_data = user_data;
    params.node_callback = node_fnct;
    params.way_callback = way_fnct;
    params.relation_callback = relation_fnct;
    params.stop = 0;

/* reading BlobHeader size: OSMHeader */
    rd = fread (buf, 1, 4, input->in);
    if (rd != 4)
	return READOSM_INVALID_PBF_HEADER;
    hdsz = get_header_size (buf, input->little_endian_cpu);

/* testing OSMHeader */
    if (!skip_osm_header (input, hdsz))
	return READOSM_INVALID_PBF_HEADER;

/* 
 / the PBF file is internally organized as a collection
 / of many subsequent OSMData blocks 
*/
    while (1)
      {
	  /* reading BlobHeader size: OSMData */
	  if (params.stop)
	      return READOSM_ABORT;
	  rd = fread (buf, 1, 4, input->in);
	  if (rd == 0 && feof (input->in))
	      break;
	  if (rd != 4)
	      return READOSM_INVALID_PBF_HEADER;
	  hdsz = get_header_size (buf, input->little_endian_cpu);

	  /* parsing OSMData */
	  if (!parse_osm_data (input, hdsz, &params))
	      return READOSM_INVALID_PBF_HEADER;
      }
    return READOSM_OK;
}
