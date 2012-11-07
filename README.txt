CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Future plans
 * Thanks
 * References

INTRODUCTION
------------

This module provides a zoomable interface for very large images.  It includes
a tile-cutting algorithm for generating tiles in [Tile Map Service][1] format
(TMS), as well as a field formatter to render these tiles in an interactive
map-style view instead of the usual simple static display of the image.  For
image fields which are multivalued, it also provides a crossfader control to
compare any two of the images layered across each other.

INSTALLATION
------------

Since this module depends on the OpenLayers library, it's highly advised to
follow the instructions for that module first.  Notably, you must have the
latest version of the OpenLayers javascript library installed in
sites/all/libraries.

 1. Install and configure the module dependencies: OpenLayers and Media.

 2. Enable media_imagezoom.

 3. Visit the display settings for node types where you'd like image fields to
use the deep zoom viewer, and select it as the formatter.

 4. From the commandline, execute the 'drush precache-media-tiles' command.
Depending on how many large image files you have, this could take a while to
complete.  If you interrupt the process, it should pick up where it left off.

FUTURE PLANS
------------

Though the module is capable of use right now, the setup could be more
configurable through the Drupal interface.

 * admin page for setting minimum dimensions to use
 * allow specifying a fallback formatter for smaller images
 * use queue or batch to generate the image tiles, instead of a drush script

THANKS
------

This module was developed at the University of Kentucky [College of Arts &
Sciences][2] for a scholarly project under the direction of Dr. Bill Endres,
documenting illuminated insular manuscripts including the St.  Chad Gospels
and the Wycliffe New Testament housed at the [Lichfield Cathedral][3] in the
United Kingdom.  More details from Dr. Endres about the project can be found
at its website, [The Manuscripts of Lichfield Cathedral][4].

REFERENCES
----------

[1]: http://wiki.osgeo.org/wiki/Tile_Map_Service_Specification
[2]: http://www.as.uky.edu/ "University of Kentucky College of Arts & Sciences"
[3]: http://www.lichfield-cathedral.org/
[4]: http://lichfield.as.uky.edu/ "Manuscripts of Lichfield Cathedral"
