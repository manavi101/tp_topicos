#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

// Definicion de Constantes Globales
#define TODO_OK 0
#define ARCHIVO_NO_ENCONTRADO 10
#define NO_SE_PUEDE_CREAR_ARCHIVO 20
#define TAM_CABECERA_BMP 2
#define TAM_MAX_FILENAME 150

// Definicion de Errores
#define ERROR 0
#define ERROR_ARGUMENTOS 0
#define ERROR_MEMORIA_DINAMICA 50
#define ERROR_APERTURA_ARCHIVO 100

// Definicion de Macros
#define MAXRGB(x) (x>255)? (255) : (x)

// Definicion de Modificadores
#define REDUCCION_CONTRASTE 0.75
#define AUMENTO_CONTRASTE 1.25
#define AUMENTO_TONALIDAD 1.5

#endif // CONSTANTES_H_INCLUDED
