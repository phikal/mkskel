mkskel
======

mkskel is a command-line utility to quickly create a skeleton structure
for projects or just singular files.

Bug reports and patches should be sent to my [public inbox].

How to use
----------

The default usage of mkskel is

	$ mkskel [skeleton name]

This will attempt to copy and process all the files from the specified
skeleton. If more than one skeleton will be listed, they will be
generated in order.

Command line flags modifying the default behaviour are listed
in the man page.

Skeletons
---------

Skeletons are stored in directories that are listed in the `SKELPATH`
environmental variable. A collection of skeletons can be found in the
[mkskel-data][collection] repository. Information on skeleton file
format can also be found there.

Copying
-------

`mkskel` is distributed under the [CC0 1.0 Universal (CC0 1.0) Public
Domain Dedication][cc0] license.

[collection]: https://git.sr.ht/~zge/mkskel-data
[public inbox]: https://lists.sr.ht/~zge/public-inbox
[cc0]: https://creativecommons.org/publicdomain/zero/1.0/deed
