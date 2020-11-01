------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------  Projet C - JPEG ---------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------

Organisation
-------------
Aprés la premiere étape de décodage des images données ( mode séquentiel ) de jpeg to ppm, chacun du membres de groupes a attaqué l'implémentation d' un des modules à implémentés.



Etapes réalisées:
-----------------

   1 // Décodeur JPEG mode séquentiel + Tests (pour differents facteurs échantionnages "non traité par les images données", voir images)  .
   2 // Implémentations des modules: bitstream / huffman / jpeg_reader.
   3 // Algorithme de Loeffler " IDCT rapide " ( temps d'exucution de 5 min à 2.89 secondes pour biiiig.jpeg !!! ).
   4 // Implémentation de makefile.
   5 // test: generation des images : avec gimp, et différents cas d'échantillonnage avec la commande:
	cjpeg -sample h1xv1, h2xv2, h3xv3 image_input.ppm > image_output.jpg

Valgrind:
---------

   --> Conditional jump or move depends on uninitialised value(s).
	by main ( ligne 19 ) at close_jpeg ( module jpeg_reader )

    + On a essayé de résoude ce probleme qui apparait au dernier moment ( et qui nous a appris beaucoup de temps a essayé de le résoudre 😞 ) aprés le rassemblement des différents parties de notre codes final.

Execution de code:
------------------

     ---> make : compiler le code
