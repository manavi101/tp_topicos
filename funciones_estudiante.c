/*
    Integrantes del grupo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido: Moyano
    Nombre: Matias
    DNI: 40570385
    Entrega: si
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------

    Comentarios (opcionales) que deseen hacer al docente sobre el TP:

*/

#include "funciones_estudiante.h"

int solucion(int argc, char* argv[])
{
    /*
        Aquí deben hacer el código que solucione lo solicitado.
        Todas las funciones utilizadas deben estar declaradas en este archivo, y en su respectivo .h
    */

    if (argc < 3)
    {
        printf("Uso: %s <operacion> <archivo_original.bmp>\n", argv[0]);
        return ERROR;  // Define ERROR en constantes.h
    }

    FILE* archivo = fopen(argv[2], "rb");
    if (!archivo)
    {
        printf("Error al abrir el archivo %s\n", argv[2]);
        return ERROR;
    }

    t_metadata imagen = leer_bmp(archivo);
    t_pixel* pixeles = leer_pixeles(archivo, imagen);
    fclose(archivo);

    // Aplicación de negativo
    if (strcmp(argv[1], "--negativo") == 0)
    {
        //aplicar_negativo(pixeles, meta.ancho * meta.alto);
    }

    // Guardo la imagen modificada
    //guardar_bmp("salida.bmp", pixeles, meta);
    free(pixeles);

    return TODO_OK;
}


t_metadata leer_bmp(FILE* archivo)
{
    t_metadata meta;
    fseek(archivo, 2, SEEK_SET);  // Saltar los dos primeros bytes que son el tipo de archivo
    fread(&meta.tamArchivo, sizeof(unsigned int), 1, archivo);
    fseek(archivo, 10, SEEK_SET);
    fread(&meta.comienzoImagen, sizeof(unsigned int), 1, archivo);
    fseek(archivo, 18, SEEK_SET);
    fread(&meta.ancho, sizeof(unsigned int), 1, archivo);
    fread(&meta.alto, sizeof(unsigned int), 1, archivo);
    fseek(archivo, 28, SEEK_SET);
    fread(&meta.profundidad, sizeof(unsigned short), 1, archivo);
    meta.tamEncabezado = meta.comienzoImagen - 14; // 14 bytes hasta la info del tamaño de imagen
    return meta;
}

t_pixel* leer_pixeles(FILE* archivo, t_metadata meta)
{
    t_pixel* pixeles = malloc(meta.ancho * meta.alto * sizeof(t_pixel));
    fseek(archivo, meta.comienzoImagen, SEEK_SET);
    for (int i = 0; i < meta.ancho * meta.alto; i++)
    {
        fread(pixeles[i].pixel, 3, 1, archivo);  // Leer 3 bytes por pixel
    }
    return pixeles;
}

void guardar_bmp(const char* filename, t_pixel* pixeles, t_metadata meta) {
    FILE* archivo = fopen(filename, "wb");
    if (archivo == NULL) {
        fprintf(stderr, "Error al abrir el archivo para escribir.\n");
        return;
    }

    // Escribe el encabezado BMP
    //escribir_encabezado_bmp(archivo, meta);

    // Escribe los datos de los píxeles
    fseek(archivo, meta.comienzoImagen, SEEK_SET);
    for (int i = 0; i < meta.ancho * meta.alto; i++) {
        fwrite(pixeles[i].pixel, 1, 3, archivo);  // Escribir 3 bytes por píxel
    }

    fclose(archivo);
}

void aplicar_negativo(t_pixel* pixeles, int cantidad)
{
    for (int i = 0; i < cantidad; i++)
    {
        pixeles[i].pixel[0] = 255 - pixeles[i].pixel[0];  // Rojo
        pixeles[i].pixel[1] = 255 - pixeles[i].pixel[1];  // Verde
        pixeles[i].pixel[2] = 255 - pixeles[i].pixel[2];  // Azul
    }
}



void abrir_archivo(FILE** archivo, const char* filename, const char* modo)
{
    *archivo = fopen(filename, modo);
    if (!*archivo)
    {
        printf("Error al abrir el archivo %s\n", filename);
        exit(ERROR_APERTURA_ARCHIVO);
    }
}