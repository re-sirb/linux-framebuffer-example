# Linux framebuffer example
A little code example to give a starting point for playing with the Linux framebuffer. It's fun to be able to get graphic output directly with relatively little effort and without the overhead of a GUI. This becomes particularly useful in the field of embedded systems.

In order to avoid some frustration at the beginning, I would like to point out that it might not work if you are using X11. So try running it from a terminal not running X Server. It might also be possible to install a dedicated framebuffer driver for use with X Server, but I haven't looked into that too much.

libjpeg is needed to compile the program. Most Linux distributions should already ship with it.

Have fun!
