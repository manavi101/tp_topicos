#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED


#include "constantes.h"
#include "estructuras.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int solucion(int argc, char* argv[]);

FILE* abrir_archivo(const char* filename, const char* modo);

//Archivo BMP
int es_bmp(FILE* archivo);
t_metadata leer_bmp(FILE* archivo);
t_pixel* leer_pixeles(FILE* archivo, t_metadata meta);
void asignar_nombre_archivo(char* destino, const char* origen, const char* sufijo);
void guardar_bmp(const char* filename, t_pixel* pixeles, t_metadata meta);

//cambios pixeles BMP
void aplicar_negativo(t_pixel* pixeles, int cantidad);
void aumentar_contraste(t_pixel* pixeles, int cantidad);
void reducir_contraste(t_pixel* pixeles, int cantidad);
void tonalidad_azul(t_pixel* pixeles, int cantidad);
void tonalidad_roja(t_pixel* pixeles, int cantidad);
void tonalidad_verde(t_pixel* pixeles, int cantidad);
void aumentar_tonalidad(t_pixel* pixeles, int cantidad, int color);


#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
