#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED


#include "constantes.h"
#include "estructuras.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int solucion(int argc, char* argv[]);

t_metadata leer_bmp(FILE* archivo);
t_pixel* leer_pixeles(FILE* archivo, t_metadata meta);

void guardar_bmp(const char* filename, t_pixel* pixeles, t_metadata meta);

void aplicar_negativo(t_pixel* pixeles, int cantidad);


#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
