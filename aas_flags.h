#ifndef __AASFLAGS_H__
#define __AASFLAGS_H__

/* Defines flags used by AAS implementation */

#define AAS_CONTENTS_SOLID		1
#define	AAS_CONTENTS_WINDOW		2 /* Q2 */
#define AAS_CONTENTS_LAVA		8
#define AAS_CONTENTS_SLIME		16
#define AAS_CONTENTS_WATER		32


#define AAS_CONTENTS_NOTTEAM1		0x0080 /* Q3 */
#define AAS_CONTENTS_NOTTEAM2		0x0100 /* Q3 */

#define AAS_CONTENTS_AREAPORTAL		0x8000
#define AAS_CONTENTS_PLAYERCLIP		0x10000
#define AAS_CONTENTS_TELEPORTER		0x40000 /* Q3 */
#define AAS_CONTENTS_JUMPPAD		0x80000 /* Q3 */

#define AAS_CONTENTS_CLUSTERPORTAL	0x100000 /* Q3 */
#define AAS_CONTENTS_DONOTENTER		0x200000 /* Q3 */
#define AAS_CONTENTS_BOTCLIP		0x400000 /* Q3 */
#define AAS_CONTENTS_MOVER		0x800000 /* Q3 */

#define AAS_CONTENTS_DETAIL		0x8000000	// brushes not used for the bsp

#define AAS_MASK_LIQUID (AAS_CONTENTS_LAVA | AAS_CONTENTS_SLIME | AAS_CONTENTS_WATER)
#define AAS_MASK_PLAYERSOLID (AAS_CONTENTS_SOLID | AAS_CONTENTS_WINDOW | AAS_CONTENTS_PLAYERCLIP)
#define AAS_MASK_Q2 (AAS_MASK_PLAYERSOLID | AAS_MASK_LIQUID | AAS_CONTENTS_AREAPORTAL | AAS_CONTENTS_DETAIL | AAS_CONTENTS_LADDER | 0x1000000)

/* Q2 has this brush content flag, Q3 has it on faces instead */
#define AAS_CONTENTS_LADDER		0x20000000

/* same in both Q2 and Q3 */
#define	SURF_HINT			0x100	// make a primary bsp splitter
#define SURF_SKIP			0x200	// completely ignore, allowing non-closed brushes

// 0-2 are axial planes
#define	PLANE_X			0
#define	PLANE_Y			1
#define	PLANE_Z			2

// 3-5 are non-axial planes snapped to the nearest
#define	PLANE_ANYX		3
#define	PLANE_ANYY		4
#define	PLANE_ANYZ		5
#endif