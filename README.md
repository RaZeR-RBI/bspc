# Fork information
This fork adds support for **Quake 2** maps reachability calculation.

Botclip, cluster portal and 'do not enter' brushes are supported, along with a new brush type 'navsplit', which can be used to split large AAS areas into multiple parts.
To use these brushes you need to use the following **surface flags** in addition to **skip** flag (to make them invisible and non-solid):
- **0x1000** makes a **botclip brush** (marks area inside brush as unreachable by bot)
- **0x2000** makes a **cluster portal brush** (divides map into clusters, recommended to place in doorways)
- **0x4000** makes a **navsplit brush**, which essentially adds another content flag which doesn't change anything except how AAS BSP splits
- **0x8000** makes a **'do not enter' brush** which marks an area for bot to avoid unless necessary

Also this fork adds additional options to '-bsp2aas' mode:

- **-forcedportals** - disables automatic cluster portal creation - only forced (defined by cluster portal brushes) portals will be used
- **-onlystatic** - use only worldspawn brushes for AAS calculation (no doors, triggers, etc.)


---

# bspc

This is the [Quake III: Arena](http://www.idsoftware.com/games/quake/quake3-arena/) BSP-to-AAS compiler.

## Downloading

You can download the latest version [here](https://github.com/bnoordhuis/bspc).

## Compiling

Dead simple:

	make

## Usage

Straight from the source:

	Usage:   bspc [-<switch> [-<switch> ...]]
	Example 1: bspc -bsp2aas /quake3/baseq3/maps/mymap?.bsp
	Example 2: bspc -bsp2aas /quake3/baseq3/pak0.pk3/maps/q3dm*.bsp

	Switches:
	   bsp2aas  <[pakfilter/]filter.bsp>    = convert BSP to AAS
	   reach    <filter.bsp>                = compute reachability & clusters
	   cluster  <filter.aas>                = compute clusters
	   aasopt   <filter.aas>                = optimize aas file
	   aasinfo  <filter.aas>                = show AAS file info
	   output   <output path>               = set output path
	   threads  <X>                         = set number of threads to X
	   cfg      <filename>                  = use this cfg file
	   optimize                             = enable optimization
	   noverbose                            = disable verbose output
	   breadthfirst                         = breadth first bsp building
	   nobrushmerge                         = don't merge brushes
	   noliquids                            = don't write liquids to map
	   freetree                             = free the bsp tree
	   nocsg                                = disables brush chopping
	   forcesidesvisible                    = force all sides to be visible
	   grapplereach                         = calculate grapple reachabilities

## Support

[File a bug report](https://github.com/bnoordhuis/bspc/issues) if you run into issues.

## License

This program is licensed under the GNU Public License v2.0 and any later version.
