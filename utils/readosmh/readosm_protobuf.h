/* 
/ readosm_protobuf.h
/
/ internal declarations (Protocol Buffer specific)
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

/*
 * PBF stands for: Protocol Buffer Format
 * a serialization notation documented here:
 * http://wiki.openstreetmap.org/wiki/PBF_Format
 * and here too:
 * https://developers.google.com/protocol-buffers/docs/encoding
 */

/* PBF variant validity */
#define READOSM_VAR_INVALID	0
#define READOSM_VAR_VALID	1

/* PBF variant types */
#define READOSM_VAR_UNDEFINED	0
#define READOSM_VAR_INT32	1
#define READOSM_VAR_INT64	2
#define READOSM_VAR_UINT32	3
#define READOSM_VAR_UINT64	4
#define READOSM_VAR_SINT32	5
#define READOSM_VAR_SINT64	6
#define READOSM_VAR_BOOL	7
#define READOSM_VAR_ENUM	8
#define READOSM_LEN_BYTES	9

/* PBF bitmasks used for 32 bit VarInts */
#define READOSM_MASK32_1	0x0000007f
#define READOSM_MASK32_2	0x00003f80
#define READOSM_MASK32_3	0x001fc000
#define READOSM_MASK32_4	0x0fe00000
#define READOSM_MASK32_5	0xf0000000

/* PBF bitmasks used for 64 bit VarInts */
#define READOSM_MASK64_1	0x000000000000007f
#define READOSM_MASK64_2	0x0000000000003f80
#define READOSM_MASK64_3	0x00000000001fc000
#define READOSM_MASK64_4	0x000000000fe00000
#define READOSM_MASK64_5	0x00000007f0000000
#define READOSM_MASK64_6	0x000003f800000000
#define READOSM_MASK64_7	0x0001fc0000000000
#define READOSM_MASK64_8	0x00fe000000000000
#define READOSM_MASK64_9	0x7f00000000000000
#define READOSM_MASK64_A	0x8000000000000000

typedef struct readosm_packed_infos_struct
{
/* a struct supporting DenseInfos parsing */
    int ver_count;		/* how many Version values are there */
    int *versions;		/* Version values array */
    int tim_count;		/* how many Timestamp values are there */
    int *timestamps;		/* Timestamp values array */
    int cng_count;		/* how many Changeset values are there */
    long long *changesets;	/* Changeset values array */
    int uid_count;		/* how many UID values are there */
    int *uids;			/* UID values array */
    int usr_count;		/* how many User values are there */
    int *users;			/* indexes to access corresponding Strings in StringTable */
} readosm_packed_infos;

typedef struct readosm_variant_hint_struct
{
/* VariantHints for PBF parsing */
    unsigned char field_id;	/* field ID */
    unsigned char type;		/* expected type */
    struct readosm_variant_hint_struct *next;	/* supporting linked list */
} readosm_variant_hint;

typedef struct readosm_variant_struct
{
/* a PBF Variant type wrapper */
    char little_endian_cpu;	/* actual CPU endianness */
    unsigned char type;		/* current type */
    unsigned char field_id;	/* field ID */
    union variant_value
    {
	int int32_value;	/* signed int32 */
	unsigned int uint32_value;	/* unsigned int32 */
	long long int64_value;	/* signed int64 */
	unsigned long long uint64_value;	/* unsigned int64 */
	float float_value;	/* floating point, single precision */
	double double_value;	/* floating point, double precision */
    } value;			/* numeric variant value */
    size_t length;		/* length in bytes [for strings] */
    unsigned char *pointer;	/* pointer to String value */
    char valid;			/* valid value */
    readosm_variant_hint *first;	/* pointers supporting a linked list */
    readosm_variant_hint *last;	/* of VariantHints items */
} readosm_variant;

typedef struct readosm_string_struct
{
/* a String into a PBF StringTable */
    char *string;		/* pointer to string value (NULL terminated string) */
    struct readosm_string_struct *next;	/*  supporting linked list */
} readosm_string;

typedef struct readosm_string_table_struct
{
/* a PBF StringTable
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
    readosm_string *first;	/* pointers supporting a linked list */
    readosm_string *last;	/* of PBF string objects */
    int count;			/* how many TAG items are there */
    readosm_string **strings;	/* array of PBF string objects */
} readosm_string_table;

typedef struct readosm_uint32_struct
{
/* a uint32 value into the PBF packed object */
    unsigned int value;
    struct readosm_uint32_struct *next;
} readosm_uint32;

typedef struct readosm_uint32_packed_struct
{
/* a PBF uint32 packed object */
    readosm_uint32 *first;
    readosm_uint32 *last;
    int count;
    unsigned int *values;
} readosm_uint32_packed;

typedef struct readosm_int32_struct
{
/* an int32 value into the PBF packed object */
    int value;
    struct readosm_int32_struct *next;
} readosm_int32;

typedef struct readosm_int32_packed_struct
{
/* a PBF int32 packed object */
    readosm_int32 *first;
    readosm_int32 *last;
    int count;
    int *values;
} readosm_int32_packed;

typedef struct readosm_int64_struct
{
/* an int64 value into the PBF packed object */
    long long value;
    struct readosm_int64_struct *next;
} readosm_int64;

typedef struct readosm_int64_packed_struct
{
/* a PBF int64 packed object */
    readosm_int64 *first;
    readosm_int64 *last;
    int count;
    long long *values;
} readosm_int64_packed;
