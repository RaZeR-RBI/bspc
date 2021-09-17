#define Q2_CM
#include "cm_local.h"
#include "q2files.h"

#ifdef BSPC

#include "l_qfiles.h"

#endif

void CMod_LoadSurfaces(lump_t *l)
{
	texinfo_t *in;
	dshader_t *out;
	int i, count;

	in = (void *)(cmod_base + l->fileofs);

	if (l->filelen % sizeof(*in))
	{
		Com_Error(ERR_DROP, "Mod_LoadSurfaces: funny lump size");
	}

	count = l->filelen / sizeof(*in);

	if (count < 1)
	{
		Com_Error(ERR_DROP, "Map with no surfaces");
	}

	if (count > MAX_MAP_TEXINFO)
	{
		Com_Error(ERR_DROP, "Map has too many surfaces");
	}

	cm.shaders = Hunk_Alloc(count * sizeof(*cm.shaders), h_high);
	cm.numShaders = count;
	out = cm.shaders;

	for (i = 0; i < count; i++, in++, out++)
	{
		Q_snprintf(&out->shader, MAX_QPATH, "surface_%i\0", i);
		// TODO: Fill contents from owning brushes
		out->surfaceFlags = LittleLong(in->flags);
	}
}

static void
CMod_LoadLeafs(lump_t *l)
{
	int i;
	cLeaf_t *out;
	dleaf_t *in;
	int count;

	in = (void *)(cmod_base + l->fileofs);

	if (l->filelen % sizeof(*in))
	{
		Com_Error(ERR_DROP, "Mod_LoadLeafs: funny lump size");
	}

	count = l->filelen / sizeof(*in);

	if (count < 1)
	{
		Com_Error(ERR_DROP, "Map with no leafs");
	}

	/* need to save space for box planes */
	if (count > MAX_MAP_PLANES)
	{
		Com_Error(ERR_DROP, "Map has too many planes");
	}

	cm.leafs = Hunk_Alloc( ( BOX_LEAFS + count ) * sizeof( *cm.leafs ), h_high );
	out = cm.leafs;
	cm.numLeafs = count;
	cm.numClusters = 0;

	for (i = 0; i < count; i++, in++, out++)
	{
		// out->contents = LittleLong(in->contents);
		out->cluster = LittleShort(in->cluster);
		out->area = LittleShort(in->area);
		out->firstLeafBrush = LittleShort(in->firstleafbrush);
		out->numLeafBrushes = LittleShort(in->numleafbrushes);
		// TODO: Fill firstleafsurface and numleafsurfaces

		if (out->cluster >= cm.numClusters) {
			cm.numClusters = out->cluster + 1;
		}
		if (out->area >= cm.numAreas) {
			cm.numAreas = out->area + 1;
		}
	}
	cm.areas = Hunk_Alloc( cm.numAreas * sizeof( *cm.areas ), h_high );
	cm.areaPortals = Hunk_Alloc( cm.numAreas * cm.numAreas * sizeof( *cm.areaPortals ), h_high );
}

static void CMod_LoadLeafBrushes (lump_t *l)
{
	int			i;
	int			*out;
	unsigned short	 	*in;
	int			count;

	in = (void *)(cmod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		Com_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size");
	count = l->filelen / sizeof(*in);

	cm.leafbrushes = Hunk_Alloc( (count + BOX_BRUSHES) * sizeof( *cm.leafbrushes ), h_high );
	cm.numLeafBrushes = count;

	out = cm.leafbrushes;

	for ( i=0 ; i<count ; i++, in++, out++) {
		*out = LittleLong (*in);
	}
}

static void CMod_LoadPlanes (lump_t *l)
{
	int			i, j;
	cplane_t	*out;
	dplane_t 	*in;
	int			count;
	int			bits;

	in = (void *)(cmod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		Com_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size");
	count = l->filelen / sizeof(*in);

	if (count < 1)
		Com_Error (ERR_DROP, "Map with no planes");
	cm.planes = Hunk_Alloc( ( BOX_PLANES + count ) * sizeof( *cm.planes ), h_high );
	cm.numPlanes = count;

	out = cm.planes;

	for ( i=0 ; i<count ; i++, in++, out++)
	{
		bits = 0;
		for (j=0 ; j<3 ; j++)
		{
			out->normal[j] = LittleFloat (in->normal[j]);
			if (out->normal[j] < 0)
				bits |= 1<<j;
		}

		out->dist = LittleFloat (in->dist);
		out->type = PlaneTypeForNormal( out->normal );
		out->signbits = bits;
	}
}

static void CMod_LoadBrushes( lump_t *l ) {
	dbrush_t	*in;
	cbrush_t	*out;
	int			i, count;

	in = (void *)(cmod_base + l->fileofs);
	if (l->filelen % sizeof(*in)) {
		Com_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size");
	}
	count = l->filelen / sizeof(*in);

	cm.brushes = Hunk_Alloc( ( BOX_BRUSHES + count ) * sizeof( *cm.brushes ), h_high );
	cm.numBrushes = count;

	out = cm.brushes;

	for ( i=0 ; i<count ; i++, out++, in++ ) {
		out->sides = cm.brushsides + LittleLong(in->firstside);
		out->numsides = LittleLong(in->numsides);

		// TODO: Set shadernum?
		out->shaderNum = 0; //LittleLong( in->shaderNum );
		if ( out->shaderNum < 0 || out->shaderNum >= cm.numShaders ) {
			Com_Error( ERR_DROP, "CMod_LoadBrushes: bad shaderNum: %i", out->shaderNum );
		}
		// TODO: Set brush contents using Q2_BrushContents?
		out->contents = in->contents;

		CM_BoundBrush( out );
	}

}

static void CMod_LoadBrushSides (lump_t *l)
{
	int				i;
	cbrushside_t	*out;
	dbrushside_t 	*in;
	int				count;
	int				num;

	in = (void *)(cmod_base + l->fileofs);
	if ( l->filelen % sizeof(*in) ) {
		Com_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size");
	}
	count = l->filelen / sizeof(*in);

	cm.brushsides = Hunk_Alloc( ( BOX_SIDES + count ) * sizeof( *cm.brushsides ), h_high );
	cm.numBrushSides = count;

	out = cm.brushsides;

	for ( i=0 ; i<count ; i++, in++, out++) {
		num = LittleLong( in->planenum );
		out->plane = &cm.planes[num];
		out->shaderNum = LittleLong( in->texinfo );
		if ( out->shaderNum < 0 || out->shaderNum >= cm.numShaders ) {
			// Log_Print("CMod_LoadBrushSides: brushside %i bad texInfo: %i\n", i, out->shaderNum);
			out->surfaceFlags = 0;
			continue;
		}
		out->surfaceFlags = cm.shaders[out->shaderNum].surfaceFlags;
	}
}

static void CMod_LoadSubmodels(lump_t *l)
{
	dmodel_t *in;
	cmodel_t *out;
	int i, j, count;

	in = (void *)(cmod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		Com_Error (ERR_DROP, "CMod_LoadSubmodels: funny lump size");
	count = l->filelen / sizeof(*in);

	if (count < 1)
		Com_Error (ERR_DROP, "Map with no models");
	cm.cmodels = Hunk_Alloc( count * sizeof( *cm.cmodels ), h_high );
	cm.numSubModels = count;

	if ( count > MAX_SUBMODELS ) {
		Com_Error( ERR_DROP, "MAX_SUBMODELS exceeded" );
	}

	for (i = 0; i < count; i++, in++, out++)
	{
		out = &cm.cmodels[i];

		for (j = 0; j < 3; j++)
		{
			/* spread the mins / maxs by a pixel */
			out->mins[j] = LittleFloat(in->mins[j]) - 1;
			out->maxs[j] = LittleFloat(in->maxs[j]) + 1;
			// out->[j] = LittleFloat(in->origin[j]);
		}
		// TODO: Allocate a leaf for all brushes referenced by model
		cLeaf_t leaf = {
			in->headnode, // cluster
			0, // area
			0, // TODO: firstLeafBrush
			0, // TODO: numLeafBrushes
			in->firstface,
			in->numfaces
		};
		out->leaf = leaf;
	}
}

static void CMod_LoadNodes( lump_t *l ) {
	dnode_t		*in;
	int			child;
	cNode_t		*out;
	int			i, j, count;

	in = (void *)(cmod_base + l->fileofs);
	if (l->filelen % sizeof(*in))
		Com_Error (ERR_DROP, "MOD_LoadBmodel: funny lump size");
	count = l->filelen / sizeof(*in);

	if (count < 1)
		Com_Error (ERR_DROP, "Map has no nodes");
	cm.nodes = Hunk_Alloc( count * sizeof( *cm.nodes ), h_high );
	cm.numNodes = count;

	out = cm.nodes;

	for (i=0 ; i<count ; i++, out++, in++)
	{
		out->plane = cm.planes + LittleLong( in->planenum );
		for (j=0 ; j<2 ; j++)
		{
			child = LittleLong (in->children[j]);
			out->children[j] = child;
		}
	}

}

static void CM_LoadVisibility(lump_t *l) {
	// don't load vis, assume that everything is seen
	cm.clusterBytes = ( cm.numClusters + 31 ) & ~31;
	cm.visibility = Hunk_Alloc( cm.clusterBytes, h_high );
	Com_Memset( cm.visibility, 255, cm.clusterBytes );
}

void CM_LoadMap_Q2(const char *name, qboolean clientload, int *checksum) {
	int				*buf;
	int				i;
	dheader_t		header;
	int				length;
	static unsigned	last_checksum;
	int             raven;

	if ( !name || !name[0] ) {
		Com_Error( ERR_DROP, "CM_LoadMap: NULL name" );
	}

#ifndef BSPC
	cm_noAreas = Cvar_Get ("cm_noAreas", "0", CVAR_CHEAT);
	cm_noCurves = Cvar_Get ("cm_noCurves", "0", CVAR_CHEAT);
	cm_playerCurveClip = Cvar_Get ("cm_playerCurveClip", "1", CVAR_ARCHIVE|CVAR_CHEAT );
#endif
	Com_DPrintf( "CM_LoadMap( %s, %i )\n", name, clientload );

	if ( !strcmp( cm.name, name ) && clientload ) {
		*checksum = last_checksum;
		return;
	}

	// free old stuff
	Com_Memset( &cm, 0, sizeof( cm ) );

	if ( !name[0] ) {
		cm.numLeafs = 1;
		cm.numClusters = 1;
		cm.numAreas = 1;
		cm.cmodels = Hunk_Alloc( sizeof( *cm.cmodels ), h_high );
		*checksum = 0;
		return;
	}

	//
	// load the file
	//
#ifndef BSPC
	length = FS_ReadFile( name, (void **)&buf );
#else
	length = LoadQuakeFile((quakefile_t *) name, (void **)&buf);
#endif

	if ( !buf ) {
		Com_Error (ERR_DROP, "Couldn't load %s", name);
	}

	last_checksum = LittleLong (Com_BlockChecksum (buf, length));
	*checksum = last_checksum;

	header = *(dheader_t *)buf;
	for (i=0 ; i<sizeof(dheader_t)/4 ; i++) {
		((int *)&header)[i] = LittleLong ( ((int *)&header)[i]);
	}
	if (header.ident != IDBSPHEADER) {
		Com_Error(ERR_DROP, "CM_LoadMap_Q2: %s has wrong BSP identifier (%i should be %i)",
		name, header.ident, IDBSPHEADER);
	}
	if (header.version != BSPVERSION) {
		Com_Error(ERR_DROP, "CM_LoadMap_Q2: %s has wrong BSP version (%i should be %i)",
		name, header.version, BSPVERSION);
	}
	cmod_base = (byte *)buf;
	/* load into heap */
	CMod_LoadSurfaces(&header.lumps[LUMP_TEXINFO]);
	CMod_LoadLeafs(&header.lumps[LUMP_LEAFS]);
	CMod_LoadLeafBrushes(&header.lumps[LUMP_LEAFBRUSHES]);
	CMod_LoadPlanes(&header.lumps[LUMP_PLANES]);
	CMod_LoadBrushSides(&header.lumps[LUMP_BRUSHSIDES]);
	CMod_LoadBrushes(&header.lumps[LUMP_BRUSHES]);
	CMod_LoadSubmodels (&header.lumps[LUMP_MODELS]);
	CMod_LoadNodes(&header.lumps[LUMP_NODES]);
	CMod_LoadEntityString(&header.lumps[LUMP_ENTITIES], name);
	CMod_LoadVisibility( &header.lumps[LUMP_VISIBILITY] );

	// we are NOT freeing the file, because it is cached for the ref
	FS_FreeFile (buf);

	CM_InitBoxHull ();

	CM_FloodAreaConnections ();

	// allow this to be cached if it is loaded by the server
	if ( !clientload ) {
		Q_strncpyz( cm.name, name, sizeof( cm.name ) );
	}
}