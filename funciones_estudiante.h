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
t_metadata leer_bmp(FILE* archivo);
t_pixel* leer_pixeles(FILE* archivo, t_metadata* meta);
void asignar_nombre_archivo(char* destino, const char* origen, const char* sufijo);
void guardar_bmp(const char* filename, t_pixel* pixeles, t_metadata * meta);
void escribir_encabezado_bmp(FILE* archivo, t_metadata* meta);

//cambios pixeles BMP
void aplicar_negativo(t_pixel* pixeles, const int cantidad);
void aumentar_contraste(t_pixel* pixeles, const int cantidad);
void reducir_contraste(t_pixel* pixeles, const int cantidad);
void tonalidad_azul(t_pixel* pixeles, const int cantidad);
void tonalidad_roja(t_pixel* pixeles, const int cantidad);
void tonalidad_verde(t_pixel* pixeles, const int cantidad);
void aumentar_tonalidad(t_pixel* pixeles, const int cantidad, const int color);
void escala_de_grises(t_pixel* pixeles, const int cantidad);
void rotar_izquierda(t_pixel** pixeles, t_metadata *meta);
void rotar_derecha(t_pixel** pixeles, t_metadata *meta);
void aplicar_monocromo(t_pixel* pixeles, const int cantidad);
void escribir_encabezado_monocromo(FILE* archivo, t_metadata* meta);
void guardar_monocromo(const char* filename, t_pixel* pixeles, t_metadata * meta);
int promedio_colores(t_pixel* pixel);

void achicar(t_pixel* pixeles_originales, t_metadata* meta);
void extraer_nombre_base(char* dest, const char* fullFileName);
void generar_nombre_archivo(char* destino, const char* origen, const char* sufijo);
int procesar_operacion(char* operacion, t_pixel* pixeles, t_metadata* metadata, char* baseFileName);


#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
