/**
 * This is a test application with exported nodes and ways defined in nodes.c and ways.c
 * that represent Vitoria, Esperito Santo, Brazil.
 *
 * Here we just render the image once and then write the output as a png file using libpng.
 */

#include <maprender.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include "nodes.c"
#include "ways.c"
#include <assert.h>
#include <png.h>
#include <stdio.h>
#include <string.h>

#define W 1000
#define H 1000
#define NWAYS 28663
#define NNODES 167123
#define OX -40.497
#define OY -20.516
#define ViewW .5
#define ViewH .5


static GLfloat origx = OX;
static GLfloat origy = OY;
static GLfloat view_width = ViewW;
static GLfloat view_height = ViewH;
static GLuint tex;

int store_image (unsigned int *pixels)
{
	// output file
	FILE *fp = fopen ("output.png", "wb");
	if (!fp) return 1;

	// setup png stuff

	png_structp pngp = png_create_write_struct (PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (pngp == NULL) return 1;

	png_infop infop = png_create_info_struct (pngp);
	if (infop == NULL) return 1;

	if (setjmp (png_jmpbuf (pngp))) return 1;

	png_set_IHDR (
		pngp,
		infop,
		W,
		H,
		8,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);

	// create rows
	png_byte **rows = png_malloc (pngp, H * sizeof (png_byte *));
	size_t rsize = W * sizeof (unsigned int);
	for (int y = 0; y < H; y ++)
	{
		rows[y] = png_malloc (pngp, rsize);
		memcpy (rows[y], pixels + y * rsize, rsize);
	}

	// write to file
	png_init_io (pngp, fp);
	png_set_rows (pngp, infop, rows);
	png_write_png (pngp, infop, PNG_TRANSFORM_IDENTITY, NULL);

	// free up stuff
	for (int y = 0; y < H; y ++)
		png_free (pngp, rows[y]);
	png_free (pngp, rows);
	png_destroy_write_struct (&pngp, &infop);
	fclose (fp);

	return 0;
}

int main ()
{
	assert (map_init (W, H, &tex) == 0);

	map_load_nodes (vitoria_nodes, NNODES);
	map_load_primary_ways (way_idx, way_counts, NWAYS);
	//map_load_secondary_ways (way_idx + 5000, way_counts + 5000, 5000);
	//map_load_tertiary_ways (way_idx + 10000, way_counts + 10000, NWAYS - 10000);

	unsigned int *pixels = malloc (W * H * sizeof (unsigned int));
	map_draw (origx, origy, view_width, view_height);
	glBindTexture (GL_TEXTURE_2D, tex);
	glGetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, pixels);
	glBindTexture (GL_TEXTURE_2D, 0);

	if (store_image (pixels))
		fprintf (stderr, "error storing png file\n");

	free (pixels);
}
