# Fork information
This fork adds support for **Quake 2** maps reachability calculation.

Bot clips and cluster portal brushes are supported. To make them set the following surface flags:
- **trans33** + **skip** flags = ***bot clip brush***
- **trans66** + **skip** flags = ***cluster portal brush***

The **skip** flag is needed here to make it non-blocking and invisible, **trans33/66** flags are used to distinguish between those two brush types.

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
