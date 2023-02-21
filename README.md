# Linux framebuffer example
A little code example to give a starting point for playing with the Linux framebuffer. It's fun to be able to get graphic output directly with relatively little effort and without the overhead of a GUI. It's also quite fascinating how elegantly the Linux framebuffer glosses over the nightmare of interacting with graphics hardware.

This code is intended as a starting point for playing around with the Linux framebuffer, so it doesn't contain any real error handling.

In order to avoid some frustration at the beginning, I would like to point out that it might not work if you are using X11. So run it from a terminal not running X Server. It might also be possible to install a dedicated framebuffer driver for use with X Server, but I haven't looked into that too much.

libjpeg is needed to compile this example. Most Linux distributions should already ship with it.

Have fun!
