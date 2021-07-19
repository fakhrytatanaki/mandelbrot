main:
	gcc -Wall -pedantic bmplib.c mandelbrot.c -lm -o mandelbrot
	@echo compilation done! run using the generated binary file ./mandelbrot
