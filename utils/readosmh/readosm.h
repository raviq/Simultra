/* 
/ readosm.h
/
/ public declarations
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

/**
 \file readosm.h 
 
 Function declarations and constants for ReadOSM library
 */
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef _WIN32
#ifdef DLL_EXPORT
#define READOSM_DECLARE __declspec(dllexport)
#define READOSM_PRIVATE
#else
#define READOSM_DECLARE extern
#define READOSM_PRIVATE
#endif
#else
#define READOSM_DECLARE __attribute__ ((visibility("default")))
#define READOSM_PRIVATE __attribute__ ((visibility("hidden")))
#endif
#endif

#ifndef _READOSM_H
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define _READOSM_H
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* constants */
/** information is not available */
#define READOSM_UNDEFINED	-1234567890
/** MemberType: NODE */
#define READOSM_MEMBER_NODE	7361
/** MemberType: WAY */
#define READOSM_MEMBER_WAY 6731
/** MemberType: RELATION */
#define READOSM_MEMBER_RELATION 3671

/* Error codes */
#define READOSM_OK			0 /**< No error, success */
#define READOSM_INVALID_SUFFIX		-1 /**< not .osm or .pbf suffix */
#define READOSM_FILE_NOT_FOUND		-2 /**< .osm or .pbf file does not exist or is
						not accessible for reading */
#define READOSM_NULL_HANDLE		-3 /**< Null OSM_handle argument */
#define READOSM_INVALID_HANDLE		-4 /**< Invalid OSM_handle argument */
#define READOSM_INSUFFICIENT_MEMORY	-5 /**< some kind of memory allocation
                                                failure */
#define READOSM_CREATE_XML_PARSER_ERROR	-6 /**< cannot create the XML Parser */
#define READOSM_READ_ERROR		-7 /**< read error */
#define READOSM_XML_ERROR		-8 /**< XML parser error */
#define READOSM_INVALID_PBF_HEADER	-9 /**< invalid PBF header */
#define READOSM_UNZIP_ERROR		-10 /**< unZip error */
#define READOSM_ABORT			-11 /**< user-required parser abort */

	/**
	 a struct representing a <b>key:value</b> pair, and wrapping an XML fragment like the following:
	\verbatim
<tag key="key-value" value="some-value" />
	\endverbatim
	 */
    struct readosm_tag_struct
    {

	const char *key; /**< the KEY */
	const char *value; /**< the VALUE */
    };

	/**
     Typedef for TAG structure.
     
     \sa readosm_tag_struct
     */
    typedef struct readosm_tag_struct readosm_tag;

	/**
	 a struct representing a NODE object, and wrapping a complex XML fragment like the following:
	\verbatim
<node id="12345" lat="6.66666" lon="7.77777" version="1" changeset="54321" user="some-user" uid="66" timestamp="2005-02-28T17:45:15Z">
	<tag key="created_by" value="JOSM" />
	<tag key="tourism" value="camp_site" />
</node>
	\endverbatim
	 */
    struct readosm_node_struct
    {
	const long long id; /**< NODE-ID (expected to be a unique value) */
	const double latitude; /**< geographic latitude */
	const double longitude;	/**< geographic longitude */
	const int version; /**< object version */
	const long long changeset; /**< ChangeSet ID */
	const char *user; /**< name of the User defining this NODE */
	const int uid; /**< corresponding numeric UserID */
	const char *timestamp; /**< when this NODE was defined */
	const int tag_count; /**< number of associated TAGs (may be zero) */
	const readosm_tag *tags; /**< array of TAG objects (may be NULL) */
    };

	/**
     Typedef for NODE structure.
     
     \sa readosm_node_struct
     */
    typedef struct readosm_node_struct readosm_node;

	/**
	 a struct representing a WAY object, and wrapping a complex XML fragment like the following:
	\verbatim
<way id="12345" version="1" changeset="54321" user="some-user" uid="66" timestamp="2005-02-28T17:45:15Z">
	<nd ref="12345" />
	<nd ref="12346" />
	<nd ref="12347" />
	<tag key="created_by" value="JOSM" />
	<tag key="tourism" value="camp_site" />
</way>
	\endverbatim
	 */
    struct readosm_way_struct
    {
	const long long id; /**< WAY-ID (expected to be a unique value) */
	const int version; /**< object version */
	const long long changeset; /**< ChangeSet ID */
	const char *user; /**< name of the User defining this WAY */
	const int uid; /**< corresponding numeric UserID */
	const char *timestamp; /**< when this WAY was defined */
	const int node_ref_count; /**< number of referenced NODE-IDs (may be zero) */
	const long long *node_refs; /**< array of NODE-IDs (may be NULL) */
	const int tag_count; /**< number of associated TAGs (may be zero) */
	const readosm_tag *tags; /**< array of TAG objects (may be NULL) */
    };

	/**
     Typedef for WAY structure.
     
     \sa readosm_way_struct
     */
    typedef struct readosm_way_struct readosm_way;

	/**
	 a struct representing a RELATION-MEMBER, and wrapping an XML fragment like the following:
	\verbatim
<member type="some-type" ref="12345" role="some-role" />
	\endverbatim
	 */
    struct readosm_member_struct
    {

	const int member_type; /**< can be one of: READOSM_MEMBER_NODE, READOSM_MEMBER_WAY or READOSM_MEMBER_RELATION */
	const long long id; /**< ID-value identifying the referenced object */
	const char *role; /**< intended role for this reference */
    };

	/**
     Typedef for MEMBER structure.
     
     \sa readosm_member_struct
     */
    typedef struct readosm_member_struct readosm_member;


	/**
	 a struct representing a RELATION object, and wrapping a complex XML fragment like the following:
	\verbatim
<relation id="12345" version="1" changeset="54321" user="some-user" uid="66" timestamp="2005-02-28T17:45:15Z">
	<member type="way" ref="12345" role="outer" />
	<member type="way" ref="12346" role="inner" />
	<tag key="created_by" value="JOSM" />
	<tag key="tourism" value="camp_site" />
</relation>
	\endverbatim
	 */
    struct readosm_relation_struct
    {
	const long long id; /**< RELATION-ID (expected to be a unique value) */
	const int version; /**< object version */
	const long long changeset; /**< ChangeSet ID */
	const char *user; /**< name of the User defining this RELATION */
	const int uid; /**< corresponding numeric UserID */
	const char *timestamp; /**< when this RELATION was defined */
	const int member_count;	/**< number of associated MEMBERs (may be zero) */
	const readosm_member *members; /**< array of MEMBER objects (may be NULL) */
	const int tag_count; /**< number of associated TAGs (may be zero) */
	const readosm_tag *tags; /**< array of TAG objects (may be NULL) */
    };

	/**
     Typedef for RELATION structure.
     
     \sa readosm_relation_struct
     */
    typedef struct readosm_relation_struct readosm_relation;

/** callback function handling NODE objects */
    typedef int (*readosm_node_callback) (const void *user_data,
					  const readosm_node * node);

/** callback function handling WAY objects */
    typedef int (*readosm_way_callback) (const void *user_data,
					 const readosm_way * way);

/** callback function handling RELATION objects */
    typedef int (*readosm_relation_callback) (const void *user_data,
					      const readosm_relation *
					      relation);

    /**
     Open the .osm or .pbf file, preparing for future functions
     
     \param path full or relative pathname of the input file.
     \param osm_handle an opaque reference (handle) to be used in each
     subsequent function (return value).

     \return READOSM_OK will be returned on success, otherwise any appropriate
     error code on failure.

     \note You are expected to readosm_close() even on failure, so as to
     correctly release any dynamic memory allocation.
     */
    READOSM_DECLARE int readosm_open (const char *path,
				      const void **osm_handle);

    /** 
     Close the .osm or .pbf file and release any allocated resource

    \param osm_handle the handle previously returned by readosm_open()

    \return READOSM_OK will be returned on success, otherwise any appropriate
     error code on failure.
    
    \note After calling readosm_close() any related resource will be released,
    and the handle will no longer be valid.
    */
    READOSM_DECLARE int readosm_close (const void *osm_handle);

    /** 
     Close the .osm or .pbf file and release any allocated resource

    \param osm_handle the handle previously returned by readosm_open()
\param user_data pointer to some user-supplied data struct
	\param node_fnct pointer to callback function intended to consume NODE objects 
	(may be NULL if processing NODEs is not an interesting option)
	\param way_fnct pointer to callback function intended to consume WAY objects 
	(may be NULL if processing WAYs is not an interesting option)
	\param relation_fnct pointer to callback function intended to consume RELATION objects 
	(may be NULL if processing RELATIONs is not an interesting option)

    \return READOSM_OK will be returned on success, otherwise any appropriate
     error code on failure.
    
    \note After calling readosm_close() any related resource will be released,
    and the handle will no longer be valid.
    */
    READOSM_DECLARE int readosm_parse (const void *osm_handle,
				       const void *user_data,
				       readosm_node_callback node_fnct,
				       readosm_way_callback way_fnct,
				       readosm_relation_callback relation_fnct);

#ifdef __cplusplus
}
#endif

#endif				/* _READOSM_H */
