all: linux-framebuffer-example

linux-framebuffer-example:
	cc -o linux-framebuffer-example src/*.c -ljpeg

clean:
	rm -f linux-framebuffer-example

