//----------------------------------------------------------------------------
// ZetaScale
// Copyright (c) 2016, SanDisk Corp. and/or all its affiliates.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published by the Free
// Software Foundation;
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License v2.1 for more details.
//
// A copy of the GNU Lesser General Public License v2.1 is provided with this package and
// can also be found at: http://opensource.org/licenses/LGPL-2.1
// You should have received a copy of the GNU Lesser General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place, Suite 330, Boston, MA 02111-1307 USA.
//----------------------------------------------------------------------------

/*
 * File:   name_service.c
 * Author: DO
 *
 * Created on January 15, 2008, 10:04 AM
 *
 * Copyright Schooner Information Technology, Inc.
 * http://www.schoonerinfotech.com/
 *
 * $Id: name_service.c 12335 2010-03-18 20:56:46Z briano $
 *
 */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "platform/string.h"
#include "platform/stdlib.h"
#include "platform/logging.h"
#include "cmc.h"
#include "container.h"
#include "container_props.h"
#include "name_service.h"
#include "shard_compute.h"
#include "ssd/fifo/container_meta_blob.h"
#include "init_sdf.h"
#include "api/zs.h"
#include "api/sdf_internal.h"

#define LOG_ID PLAT_LOG_ID_INITIAL
#define LOG_CAT PLAT_LOG_CAT_SDF_NAMING
#define LOG_DBG PLAT_LOG_LEVEL_DEBUG
#define LOG_TRACE PLAT_LOG_LEVEL_TRACE
#define LOG_ERR PLAT_LOG_LEVEL_ERROR
#define LOG_WARN PLAT_LOG_LEVEL_WARN
#define LOG_FATAL PLAT_LOG_LEVEL_FATAL

// Externals
extern SDF_cmc_t *theCMC; // Global CMC object
extern void UTStartDebugger(char *s); /* XXXZZZ */

// Forwards
SDFContainer
name_service_metadata_container_from_cguid( SDF_cguid_t cguid );

SDFContainer
name_service_metadata_container_from_cname( char *cname );

// =============================================================
SDF_status_t
name_service_create_meta(SDF_internal_ctxt_t *pai, const char *cname, SDF_cguid_t cguid,
			 SDF_container_meta_t *meta) {

    SDF_status_t 	status 		= SDF_FAILURE;
    int 			log_level 	= LOG_ERR;
	SDFContainer	c			= NULL;

    plat_log_msg(21579, LOG_CAT, LOG_TRACE, "%s - %lu", cname, cguid);

    if (ISEMPTY(cname)) {
		status = SDF_INVALID_PARAMETER;
    } else {
	    c = name_service_metadata_container_from_cguid( cguid );
		if ( c && (status = cmc_create_meta( pai, c, cname, cguid, meta ) ) == SDF_SUCCESS) 
			log_level = LOG_TRACE;
    }

    plat_log_msg(21610, LOG_CAT, log_level, "%s - %llu - %s", 
		 cname, (unsigned long long)cguid, SDF_Status_Strings[status]);

    return status;
}

SDF_status_t
name_service_get_meta(SDF_internal_ctxt_t *pai, SDF_cguid_t cguid, SDF_container_meta_t *meta) {

    SDF_status_t 	status 		= SDF_FAILURE;
    int 			log_level 	= LOG_ERR;
	SDFContainer	c			= NULL;

    plat_log_msg(21507, LOG_CAT, LOG_TRACE, "%llu", (unsigned long long)cguid);

    if (meta == NULL) {

		status = SDF_INVALID_PARAMETER;

    } else {

        c = name_service_metadata_container_from_cguid( cguid );
		if (c && (status = cmc_get_meta(pai, c, cguid, meta)) == SDF_SUCCESS)
			log_level = LOG_TRACE;
    }

	if ( SDF_SUCCESS != status )
		status = SDF_FAILURE;		// Hack for ZS test compatibility

    plat_log_msg(21506, LOG_CAT, log_level, "%llu - %s", (unsigned long long)cguid, SDF_Status_Strings[status]);

    return status;
}

SDF_status_t
name_service_get_meta_from_cname(SDF_internal_ctxt_t *pai, const char *cname,
				 SDF_container_meta_t *meta) {

    SDF_status_t 	status 		= SDF_FAILURE;
    int 			log_level 	= LOG_TRACE;
	SDFContainer	c			= NULL;

    plat_log_msg(20819, LOG_CAT, LOG_TRACE, "%s", cname);

    c = name_service_metadata_container_from_cname( (char *) cname );
    status = cmc_get_meta_from_cname(pai, c, cname, meta);

    plat_log_msg(21511, LOG_CAT, log_level, "%s - %s",
		 cname, SDF_Status_Strings[status]);

    return (status);
}

SDF_status_t
name_service_put_meta(SDF_internal_ctxt_t *pai, SDF_cguid_t cguid, SDF_container_meta_t *meta) {

    SDF_status_t 	status 		= SDF_FAILURE;
    int 			log_level 	= LOG_ERR;
	SDFContainer	c			= NULL;

    plat_log_msg(21507, LOG_CAT, LOG_TRACE, "%llu", (unsigned long long)cguid);

    if (meta == NULL) {
		status = SDF_INVALID_PARAMETER;
    } else {

        c = name_service_metadata_container_from_cguid( cguid );
		if (c && (status = cmc_put_meta(pai, c, cguid, meta)) == SDF_SUCCESS)
			log_level = LOG_TRACE;
    }

    plat_log_msg(21506, LOG_CAT, log_level, "%llu - %s",
		 (unsigned long long)cguid, SDF_Status_Strings[status]);

    return status;
}

SDF_status_t
name_service_lock_meta(SDF_internal_ctxt_t *pai, const char *cname) {

    SDF_status_t status = SDF_FAILURE;
    SDF_cguid_t cguid = SDF_NULL_CGUID;
    int log_level = LOG_ERR;

    plat_log_msg(20819, LOG_CAT, LOG_TRACE, "%s", cname);

    if ((status = name_service_get_cguid(pai, cname, &cguid)) == SDF_SUCCESS &&
		(status = cmc_lock_meta(pai, theCMC->c, cguid)) == SDF_SUCCESS) {
		log_level = LOG_TRACE;
    }

    plat_log_msg(21506, LOG_CAT, log_level, "%llu - %s",
		 (unsigned long long)cguid, SDF_Status_Strings[status]);

    return (status);
}

SDF_status_t
name_service_unlock_meta(SDF_internal_ctxt_t *pai, const char *cname) {

    SDF_status_t status = SDF_FAILURE;
    SDF_cguid_t cguid = SDF_NULL_CGUID;
    int log_level = LOG_ERR;

    plat_log_msg(20819, LOG_CAT, LOG_TRACE, "%s", cname);

    if ((status = name_service_get_cguid(pai, cname, &cguid)) == SDF_SUCCESS &&
		(status = cmc_unlock_meta(pai, theCMC->c, cguid)) == SDF_SUCCESS) {
		log_level = LOG_TRACE;
    }

    plat_log_msg(21506, LOG_CAT, log_level, "%llu - %s",
		 (unsigned long long)cguid, SDF_Status_Strings[status]);

    return (status);
}

SDF_status_t
name_service_lock_meta_by_cguid(SDF_internal_ctxt_t *pai, SDF_cguid_t cguid) {

    SDF_status_t status = SDF_FAILURE;
    int log_level = LOG_ERR;

    if ((status = cmc_lock_meta(pai, theCMC->c, cguid)) == SDF_SUCCESS) {
		log_level = LOG_TRACE;
    }

    plat_log_msg(21506, LOG_CAT, log_level, "%llu - %s",
		 (unsigned long long)cguid, SDF_Status_Strings[status]);

    return (status);
}

SDF_status_t
name_service_unlock_meta_by_cguid(SDF_internal_ctxt_t *pai, SDF_cguid_t cguid) {

    SDF_status_t status = SDF_FAILURE;
    int log_level = LOG_ERR;

    if ((status = cmc_unlock_meta(pai, theCMC->c, cguid)) == SDF_SUCCESS) {
		log_level = LOG_TRACE;
    }

    plat_log_msg(21506, LOG_CAT, log_level, "%llu - %s",
		 (unsigned long long)cguid, SDF_Status_Strings[status]);

    return (status);
}

SDF_status_t
name_service_meta_exists(SDF_internal_ctxt_t *pai, const char *cname) {

    SDF_status_t    status      = SDF_FAILURE;

    plat_log_msg(20819, LOG_CAT, LOG_TRACE, "%s", cname);

    if ((status = cmc_meta_exists(pai, theCMC->c, cname)) != SDF_SUCCESS)
        ;

/*    plat_log_msg(21511, LOG_CAT, log_level, "%s - %s",
         cname, SDF_Status_Strings[status]); */

    return (status);
}

SDF_status_t
name_service_cguid_exists(SDF_internal_ctxt_t *pai, SDF_cguid_t cguid) {

    SDF_status_t 	status 		= SDF_FAILURE;
	SDFContainer	c			= NULL;

    plat_log_msg(21630, LOG_CAT, LOG_TRACE, "%lu", cguid);

	c = name_service_metadata_container_from_cguid( cguid );
	if ((status = cmc_cguid_exists(pai, c, cguid)) != SDF_SUCCESS)
                ;

    plat_log_msg(150021, LOG_CAT, LOG_DBG, "%lu - %s",
		 cguid, SDF_Status_Strings[status]);

    return (status);
}

SDF_status_t
name_service_remove_meta(SDF_internal_ctxt_t *pai, const char *cname) {

    SDF_status_t 	status 		= SDF_FAILURE;
    SDF_cguid_t 	cguid 		= SDF_NULL_CGUID;
    int 			log_level 	= LOG_ERR;
	SDFContainer	c			= NULL;

    plat_log_msg(21507, LOG_CAT, LOG_TRACE, "%llu",
		 (unsigned long long)cguid);

    if ((status = name_service_get_cguid(pai, cname, &cguid)) == SDF_SUCCESS) {
		c = name_service_metadata_container_from_cguid( cguid );
		if ((status = cmc_remove_meta(pai, c, cguid)) == SDF_SUCCESS) 
			log_level = LOG_TRACE;
    }

    plat_log_msg(21506, LOG_CAT, log_level, "%llu - %s",
		 (unsigned long long)cguid, SDF_Status_Strings[status]);

    return (status);
}

SDF_status_t
name_service_get_props(SDF_internal_ctxt_t *pai, const char *cname, SDF_container_props_t *props) {

    SDF_status_t 	status 		= SDF_FAILURE;
    SDF_cguid_t 	cguid 		= SDF_NULL_CGUID;
    int 			log_level 	= LOG_TRACE;
	SDFContainer	c			= NULL;

    plat_log_msg(20819, LOG_CAT, LOG_TRACE, "%s", cname);

    if (ISEMPTY(cname) || props == NULL) {
		status = SDF_INVALID_PARAMETER;
		log_level = LOG_ERR;
    } else if ((status = name_service_get_cguid(pai, cname, &cguid)) == SDF_SUCCESS) {
		c = name_service_metadata_container_from_cguid( cguid );
		status = cmc_get_props(pai, c, cguid, props);
    }

    plat_log_msg(21511, LOG_CAT, log_level, "%s - %s",
		 cname, SDF_Status_Strings[status]);

    return (status);
}

SDF_status_t
name_service_put_props(SDF_internal_ctxt_t *pai, const char *cname, SDF_container_props_t props) {

    SDF_status_t 	status 		= SDF_FAILURE;
    SDF_cguid_t 	cguid 		= SDF_NULL_CGUID;
    int 			log_level 	= LOG_ERR;

    plat_log_msg(20819, LOG_CAT, LOG_TRACE, "%s", cname);

    if (ISEMPTY(cname)) {
		status = SDF_INVALID_PARAMETER;
    } else if ((status = name_service_get_cguid(pai, cname, &cguid)) == SDF_SUCCESS) {
		name_service_metadata_container_from_cguid( cguid );
	    if ((status = cmc_put_props(pai, theCMC, cguid, props) == SDF_SUCCESS))
			log_level = LOG_TRACE;
    }

    plat_log_msg(21511, LOG_CAT, log_level, "%s - %s", 
		 cname, SDF_Status_Strings[status]);

    return (status);
}

/* Need to change this to use cguid + objkey to find shard ID */
SDF_shardid_t
name_service_get_shard(SDF_cguid_t cguid, const char *objkey, 
                       SDF_shardid_t first_shard, uint32_t shard_count) {

    SDF_shardid_t shard = SDF_SHARDID_INVALID;
    SDF_status_t status = SDF_FAILURE;
    int log_level = LOG_TRACE;

    plat_log_msg(21506, LOG_CAT, LOG_TRACE, "%llu - %s",
		 (unsigned long long)cguid, objkey);

    if (cguid == CMC_CGUID) {
	// Handle CMC
	shard = theCMC->meta.shard;
	status = SDF_SUCCESS;
    } else if ((shard = shard_compute_get_shard(cguid, objkey, first_shard, shard_count)) 
               >= SDF_SHARDID_LIMIT) {
	log_level = LOG_ERR;
    } else {
	status = SDF_SUCCESS;
    }

    plat_log_msg(21611, LOG_CAT, log_level, "cguid: %lu (0x%lx) - shard: %lu (0x%lx)" 
                 "\n objkey: %s  status: %s",
		 cguid, cguid, shard, shard, objkey, SDF_Status_Strings[status]);

    return (shard);
}

SDF_status_t
name_service_put_shard(SDF_internal_ctxt_t *pai, SDF_cguid_t cguid, SDF_shardid_t shard) {

    SDF_status_t 	status 		= SDF_FAILURE;
    int 			log_level 	= LOG_ERR;
	SDFContainer	c			= NULL;

    plat_log_msg(21612, LOG_CAT, LOG_TRACE, "%lu - %lu", 
		 cguid, shard);

	c = name_service_metadata_container_from_cguid( cguid );
    if ((status = cmc_put_shard(pai, c, cguid, shard) == SDF_SUCCESS)) {
		log_level = LOG_TRACE;
    }

    plat_log_msg(21613, LOG_CAT, log_level, "%lu - %lu - %s",
		 cguid, shard, SDF_Status_Strings[status]);

    return (status);
}

SDF_status_t
name_service_delete_shards(SDF_internal_ctxt_t *pai, const char *cname) {

    SDF_status_t 	status 		= SDF_FAILURE;
    int 			log_level 	= LOG_ERR;
	SDFContainer	c			= NULL;

    plat_log_msg(20819, LOG_CAT, LOG_TRACE, "%s", cname);

 	c = name_service_metadata_container_from_cname( (char *) cname );
	if ( NULL == c ) {
		status = SDF_FAILURE;
	} else if ((status = cmc_delete_shards(pai, c, cname) == SDF_SUCCESS)) {
		log_level = LOG_TRACE;
	}

    plat_log_msg(21511, LOG_CAT, log_level, "%s - %s",
		 cname, SDF_Status_Strings[status]);

    return (status);
}

SDF_status_t
name_service_get_cguid(SDF_internal_ctxt_t *pai, const char *cname, SDF_cguid_t *cguid) {

    SDF_status_t 	status 		= SDF_FAILURE;
    int 			log_level 	= LOG_TRACE;

    plat_log_msg(20819, LOG_CAT, LOG_TRACE, "%s", cname);

    *cguid = SDF_NULL_CGUID;

    if (ISEMPTY(cname)) {
		status = SDF_INVALID_PARAMETER;
		log_level = LOG_ERR;
    } else if (strcmp(cname, CMC_PATH) == 0) {
		*cguid = CMC_CGUID;
		status = SDF_SUCCESS;
    } else {
 		if ((*cguid = cmc_get_cguid_from_cname(pai, theCMC->c, cname)) != SDF_NULL_CGUID) 
 		    status = SDF_SUCCESS;
    }

    plat_log_msg(21610, LOG_CAT, log_level, "%s - %llu - %s",
		 cname, (unsigned long long)*cguid, SDF_Status_Strings[status]);

    return (status);
}

SDF_status_t
name_service_get_home_node(SDF_internal_ctxt_t *pai, SDF_cguid_t cguid, SDF_simple_key_t *pkey, uint32_t *node) {

    SDF_status_t 			status 		= SDF_FAILURE;
    SDF_container_meta_t 	meta;
    int 					log_level 	= LOG_TRACE;

    plat_log_msg(21507, LOG_CAT, LOG_TRACE, "%llu", (unsigned long long)cguid);

    if (node == NULL) {
		status = SDF_INVALID_PARAMETER; 
		log_level = LOG_ERR;
		/* XXX: This needs to change to go with a replicated CMC */
    } else if (cguid == CMC_CGUID) {
		// Handle CMC 
		*node = CMC_HOME;
		status = SDF_SUCCESS;
    } else if ((status = name_service_get_meta(pai, cguid, &meta)) == SDF_SUCCESS) {
		*node = meta.node;
    }

    if (node != NULL) {
		plat_log_msg(21615, LOG_CAT, log_level, "%llu - %u - %s", 
		     	(unsigned long long)cguid, *node, SDF_Status_Strings[status]);
    } else {
		plat_log_msg(21616, LOG_CAT, log_level, "%llu - NULL - %s", 
		     	(unsigned long long)cguid, SDF_Status_Strings[status]);
    }

    return (status);
}

SDF_status_t
name_service_flush_inval_object_container(SDF_internal_ctxt_t *pai, const char *cname) {
    return (cmc_flush_inval_object_container(pai, cname));
}

SDF_status_t
name_service_inval_object_container(SDF_internal_ctxt_t *pai, const char *cname) {
    return (cmc_inval_object_container(pai, cname));
}

SDFContainer
name_service_metadata_container_from_cguid( SDF_cguid_t cguid ) {
    SDFContainer    c       = NULL;
    cntr_map_t     *cmap    = NULL;
    
    if ( cguid <= LAST_PHYSICAL_CGUID ) {
        c = theCMC->c;
    } else { 
	    cmap = zs_cmap_get_by_cguid( VMC_CGUID );
	    if ( cmap && !isContainerNull( cmap->sdf_container )) { 
	        c = internal_serverToClientContainer( cmap->sdf_container );
        } else {
            plat_log_msg( 150058, LOG_CAT, LOG_ERR, "Failed to find metadata container for %lu", cguid);
        }
    }
    
    return c;
}

SDFContainer
name_service_metadata_container_from_cname( char *cname ) {
    SDFContainer    c       = NULL;
    cntr_map_t     *cmap    = NULL;

    if ( strcmp( CMC_PATH, cname ) == 0 ||
         strcmp( VMC_PATH, cname ) == 0 ||
         strcmp( VDC_PATH, cname ) == 0 ) {
        c = theCMC->c;
    } else {
	    cmap = zs_cmap_get_by_cname( VMC_PATH );
	    if ( cmap && !isContainerNull( cmap->sdf_container )) { 
	        c = internal_serverToClientContainer( cmap->sdf_container );
        } else {
            plat_log_msg( 150063, LOG_CAT, LOG_ERR, "Failed to find metadata container for %s", cname);
        }
    }

    return c;
}
