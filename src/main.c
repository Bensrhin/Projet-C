#include "../include/decodeur.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
	fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
	return EXIT_FAILURE;
    }
    const char *filename = argv[1];

    /* On cree un jpeg_desc qui permettra de lire ce fichier. */
    struct jpeg_desc *jdesc = read_jpeg(filename);

    /* On recupere le flux des donnees brutes a partir du descripteur. */
    struct bitstream *stream = get_bitstream(jdesc);
    decodage(stream, jdesc);
    /* Nettoyage de printemps : close_jpeg ferme aussi le bitstream/*/
    close_jpeg(jdesc);
    return EXIT_SUCCESS;
  }
