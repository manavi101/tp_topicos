/*
    Integrantes del grupo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido: Moyano
    Nombre: Matias
    DNI: 40570385
    Entrega: si
    -----------------
    Apellido: Villalba
    Nombre: Matias
    DNI: 40932150
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

    Comentarios (opcionales) que deseen hacer al docente sobre el TP:

*/

#include "funciones_estudiante.h"

int solucion(int argc, char* argv[])
{
    /*
        Aquí deben hacer el código que solucione lo solicitado.
        Todas las funciones utilizadas deben estar declaradas en este archivo, y en su respectivo .h
    */

    char fileNameDest[TAM_MAX_FILENAME];

    if (argc < 3)
    {
        printf("Uso: %s <operacion> <archivo_original.bmp>\n", argv[0]);
        return ERROR_ARGUMENTOS;  // Define ERROR en constantes.h
    }

    FILE* archivo = abrir_archivo(argv[2], "rb");
    t_metadata metadata = leer_bmp(archivo);
    t_pixel* pixeles = leer_pixeles(archivo, &metadata); // memoria dinamica

    int cantidad_pixeles = metadata.ancho * metadata.alto;
    fclose(archivo);

    // Aplicación de negativo
    if (strcmp(argv[1], "--negativo") == 0)
    {
        aplicar_negativo(pixeles, cantidad_pixeles);
    }
    else if(strcmp(argv[1], "--escala-de-grises")== 0)
    {
        escala_de_grises(pixeles, cantidad_pixeles);
    }
    else if(strcmp(argv[1], "--reducir-contraste")== 0)
    {
        reducir_contraste(pixeles, cantidad_pixeles);
    }
    else if(strcmp(argv[1], "--aumentar-contraste")== 0)
    {
        aumentar_contraste(pixeles, cantidad_pixeles);
    }
    else if(strcmp(argv[1], "--tonalidad-azul")== 0)
    {
        tonalidad_azul(pixeles, cantidad_pixeles);
    }
    else if(strcmp(argv[1], "--tonalidad-roja")== 0)
    {
        tonalidad_roja(pixeles, cantidad_pixeles);
    }
    else if(strcmp(argv[1], "--tonalidad-verde")== 0)
    {
        tonalidad_verde(pixeles, cantidad_pixeles);
    }
    else if(strcmp(argv[1], "--recortar")== 0)
    {

    }
    else if(strcmp(argv[1], "--rotar-derecha")== 0)
    {

    }
    else if(strcmp(argv[1], "--rotar-izquierda")== 0)
    {

    }
    else if(strcmp(argv[1], "--achicar")== 0)
    {

    }
    else if(strcmp(argv[1], "--monocromo")== 0)
    {
        aplicar_monocromo(pixeles, cantidad_pixeles);
        guardar_monocromo(fileNameDest, pixeles, &metadata);

        free(pixeles); // libera

        return TODO_OK;
    }
    else
    {
        printf("Operacion no valida\n");
        free(pixeles);
        exit(ERROR_ARGUMENTOS);
    }
    asignar_nombre_archivo(fileNameDest, argv[2], argv[1]);


    // Guardo la imagen modificada
    guardar_bmp(fileNameDest, pixeles, &metadata);
    free(pixeles); // libera

    return TODO_OK;
}


t_metadata leer_bmp(FILE* archivo)
{
    t_metadata meta;
    char tipo[1];

    fseek(archivo, 0, SEEK_SET);

    fread(&tipo, 2, 1, archivo);

    if(tipo[0] != 'B' || tipo[1] != 'M')
    {
        printf("El archivo no es un BMP\n");
        fclose(archivo);
        exit(ERROR_APERTURA_ARCHIVO);
    }

    fseek(archivo, 2, SEEK_SET);  // Saltar los dos primeros bytes que son el tipo de archivo
    fread(&meta.tamArchivo, sizeof(unsigned int), 1, archivo);
    fseek(archivo, 10, SEEK_SET);
    fread(&meta.comienzoImagen, sizeof(unsigned int), 1, archivo);
    fseek(archivo, 14, SEEK_SET);
    fread(&meta.tamEncabezado, sizeof(unsigned int), 1, archivo);
    fseek(archivo, 18, SEEK_SET);
    fread(&meta.ancho, sizeof(unsigned int), 1, archivo);
    fread(&meta.alto, sizeof(unsigned int), 1, archivo);
    fseek(archivo, 28, SEEK_SET);
    fread(&meta.profundidad, sizeof(unsigned short), 1, archivo);
    return meta;
}

t_pixel* leer_pixeles(FILE* archivo, t_metadata * meta)
{
    t_pixel* pixeles = malloc(meta->ancho * meta->alto * sizeof(t_pixel));


    if(!pixeles)
    {
        printf("Error al reservar memoria\n");
        exit(ERROR_MEMORIA_DINAMICA);
    }

    fseek(archivo, meta->comienzoImagen, SEEK_SET);
    for (int i = 0; i < meta->ancho * meta->alto; i++)
    {
        pixeles[i].profundidad = meta->profundidad;
        fread(pixeles[i].pixel, 3, 1, archivo);  // Leer 3 bytes por pixel
    }

    return pixeles;
}

void guardar_bmp(const char* filename, t_pixel* pixeles, t_metadata * meta)
{
    //TODO: ver que pasa cuando el archivo falla para liberar memoria
    FILE* archivo = abrir_archivo(filename, "wb");
    // Escribe el encabezado BMP
    escribir_encabezado_bmp(archivo, meta);

    // Escribe los datos de los píxeles
    fseek(archivo, meta->comienzoImagen, SEEK_SET);
    for (t_pixel* fin = pixeles + meta->ancho * meta->alto; pixeles < fin; pixeles++)
    {
        if(pixeles->pixel[0] > 255 || pixeles->pixel[1] > 255 || pixeles->pixel[2] > 255)
        {
            printf("Error: pixel con valor mayor a 255\n");
            fclose(archivo);
            exit(ERROR);
        }
        fwrite(pixeles->pixel, 1, 1, archivo);  // Escribir 3 bytes por pixel
    }
    fclose(archivo);
}

void guardar_monocromo(const char* filename, t_pixel* pixeles, t_metadata * meta)
{
    FILE* archivo = abrir_archivo(filename, "wb");
    if (!archivo)
    {
        printf("Error al abrir archivo\n");
        return;
    }

    // Escribe el encabezado BMP para monocromo
    escribir_encabezado_monocromo(archivo, meta);

    // Escribe los datos de los píxeles
    fseek(archivo, meta->comienzoImagen, SEEK_SET);
    int widthBytes = (meta->ancho + 7) / 8;
    int paddingSize = (4 - (widthBytes % 4)) % 4;
    unsigned char* buffer = calloc(widthBytes + paddingSize, sizeof(unsigned char));

    for (int y = 0; y < meta->alto; y++)
    {
        memset(buffer, 0, widthBytes + paddingSize);  // Limpiar buffer para nueva línea
        for (int x = 0; x < meta->ancho; x++)
        {
            if (pixeles[y * meta->ancho + x].pixel[0] == 255)    // Blanco
            {
                buffer[x / 8] |= (1 << (7 - (x % 8)));
            }
        }
        fwrite(buffer, 1, widthBytes + paddingSize, archivo);  // Escribir la línea
    }

    free(buffer);
    fclose(archivo);
}

void escribir_encabezado_bmp(FILE* archivo, t_metadata* meta)
{
    fseek(archivo, 0, SEEK_SET);
    fwrite("BM", 2, 1, archivo);
    fwrite(&meta->tamArchivo, sizeof(unsigned int), 1, archivo);
    fseek(archivo, 10, SEEK_SET);
    fwrite(&meta->comienzoImagen, sizeof(unsigned int), 1, archivo);
    fseek(archivo, 14, SEEK_SET);
    fwrite(&meta->tamEncabezado, sizeof(unsigned int), 1, archivo);
    fseek(archivo, 18, SEEK_SET);
    fwrite(&meta->ancho, sizeof(unsigned int), 1, archivo);
    fwrite(&meta->alto, sizeof(unsigned int), 1, archivo);
    fseek(archivo, 28, SEEK_SET);
    fwrite(&meta->profundidad, sizeof(unsigned short), 1, archivo);
}

void escribir_encabezado_monocromo(FILE* archivo, t_metadata* meta)
{
    int widthBytes = (meta->ancho + 7) / 8;  // Convertir ancho de píxeles a bytes
    int paddingSize = (4 - (widthBytes % 4)) % 4;  // Alineación a 4 bytes
    int sizeImage = (widthBytes + paddingSize) * meta->alto;

    meta->tamArchivo = 62 + sizeImage; // 54 bytes del encabezado + 8 bytes de la tabla de colores + tamaño de datos
    meta->comienzoImagen = 54 + 8; // Encabezado + tabla de colores
    meta->profundidad = 1;  // 1 bit por píxel

    fseek(archivo, 0, SEEK_SET);
    fwrite("BM", 2, 1, archivo);
    fwrite(&meta->tamArchivo, sizeof(unsigned int), 1, archivo);
    unsigned int reserved = 0;
    fwrite(&reserved, sizeof(unsigned int), 1, archivo);
    fwrite(&meta->comienzoImagen, sizeof(unsigned int), 1, archivo);
    unsigned int tamEncabezado = 40;  // Tamaño del encabezado DIB
    fwrite(&tamEncabezado, sizeof(unsigned int), 1, archivo);
    fwrite(&meta->ancho, sizeof(unsigned int), 1, archivo);
    fwrite(&meta->alto, sizeof(unsigned int), 1, archivo);
    unsigned short planes = 1;
    fwrite(&planes, sizeof(unsigned short), 1, archivo);
    fwrite(&meta->profundidad, sizeof(unsigned short), 1, archivo);
    unsigned int compression = 0;
    fwrite(&compression, sizeof(unsigned int), 1, archivo);
    fwrite(&sizeImage, sizeof(unsigned int), 1, archivo);
    int res = 2835;  // 72 DPI en pixels por metro
    fwrite(&res, sizeof(unsigned int), 1, archivo);
    fwrite(&res, sizeof(unsigned int), 1, archivo);
    unsigned int numColors = 2;
    fwrite(&numColors, sizeof(unsigned int), 1, archivo);
    unsigned int importantColors = 0;
    fwrite(&importantColors, sizeof(unsigned int), 1, archivo);

    // Escribir la tabla de colores (paleta)
    unsigned char black[4] = {0, 0, 0, 0};  // Color negro
    unsigned char white[4] = {255, 255, 255, 0};  // Color blanco
    fwrite(black, 4, 1, archivo);
    fwrite(white, 4, 1, archivo);
}


FILE* abrir_archivo(const char* filename, const char* modo)
{
    FILE* archivo = fopen(filename, modo);
    if (!archivo)
    {
        printf("Error al abrir el archivo %s\n", filename);
        fclose(archivo);
        exit(ERROR_APERTURA_ARCHIVO);
    }
    return archivo;
}

/*
    Asigna un nombre de archivo con sufijo a partir de un nombre de archivo original.
    Ejemplo: si el archivo original es "imagen.bmp" y el sufijo es "--negativo", el archivo destino será "imagen_negativo.bmp"
*/
void asignar_nombre_archivo(char* destino, const char* origen, const char* sufijo)
{
    //origen = "imagen.bmp" termina en .
    while(*origen != '.')
    {
        *destino = *origen;
        destino++;
        origen++;
    }
    //agrego el "_" en destino
    *destino='_';
    destino++;

    //le agrego el sufijo, elimino el --
    sufijo+=2;
    while(*sufijo != '\0')
    {
        *destino = *sufijo;
        destino++;
        sufijo++;
    }

    strcat(destino, ".bmp\0");
}


void aplicar_negativo(t_pixel* pixeles, const int cantidad)
{
    t_pixel* fin = pixeles + cantidad;

    while(pixeles < fin)
    {
        pixeles->pixel[0] = 255 - pixeles->pixel[0];  // Rojo
        pixeles->pixel[1] = 255 - pixeles->pixel[1];  // Verde
        pixeles->pixel[2] = 255 - pixeles->pixel[2];  // Azul
        pixeles++;
    }
}

void reducir_contraste(t_pixel* pixeles, const int cantidad)
{
    t_pixel* fin = pixeles + cantidad;

    while(pixeles < fin)
    {
        pixeles->pixel[0] = MAXRGB((int)(pixeles->pixel[0] * REDUCCION_CONTRASTE));  // Rojo
        pixeles->pixel[1] = MAXRGB((int)(pixeles->pixel[1] * REDUCCION_CONTRASTE));  // Verde
        pixeles->pixel[2] = MAXRGB((int)(pixeles->pixel[2] * REDUCCION_CONTRASTE));  // Azul
        pixeles++;
    }
}

void aumentar_contraste(t_pixel* pixeles, const int cantidad)  // Si es mayor a 255 queda en ese valor.
{
    t_pixel* fin = pixeles + cantidad;

    while(pixeles < fin)
    {
        pixeles->pixel[0] = MAXRGB((int)(pixeles->pixel[0] * AUMENTO_CONTRASTE));  // Rojo
        pixeles->pixel[1] = MAXRGB((int)(pixeles->pixel[1] * AUMENTO_CONTRASTE));  // Verde
        pixeles->pixel[2] = MAXRGB((int)(pixeles->pixel[2] * AUMENTO_CONTRASTE));  // Azul
        pixeles++;
    }
}

void tonalidad_azul(t_pixel* pixeles, const int cantidad)
{
    aumentar_tonalidad(pixeles, cantidad, 2);
}

void tonalidad_verde(t_pixel* pixeles, const int cantidad)
{
    aumentar_tonalidad(pixeles, cantidad, 1);
}

void tonalidad_roja(t_pixel* pixeles, const int cantidad)
{
    aumentar_tonalidad(pixeles, cantidad, 0);
}

void aumentar_tonalidad(t_pixel* pixeles, const int cantidad, const int color)
{
    for (int i = 0; i < cantidad; i++)
    {
        int x= MAXRGB((int)(pixeles[i].pixel[color]*AUMENTO_TONALIDAD));
        pixeles[i].pixel[color] = x;
    }
}

//Rotar 90 grados a la derecha
void rotar_derecha(t_pixel* pixeles, const int cantidad, const int ancho, const int alto)
{
    t_pixel* pixeles_rotados = malloc(cantidad * sizeof(t_pixel));
    if(!pixeles_rotados)
    {
        printf("Error al reservar memoria\n");
        exit(ERROR_MEMORIA_DINAMICA);
    }
    for(int i = 0; i < ancho; i++)
    {
        for(int j = 0; j < alto; j++)
        {
            pixeles_rotados[j * ancho + (ancho - i - 1)] = pixeles[i * alto + j];
        }
    }
    free(pixeles);
    pixeles = pixeles_rotados;
}

void escala_de_grises(t_pixel* pixeles, const int cantidad)
{
    t_pixel* fin=pixeles+cantidad;
    int prom;
    while(pixeles<fin)
    {
        prom = promedio_colores(pixeles);
        pixeles->pixel[0] = prom;
        pixeles->pixel[1] = prom;
        pixeles->pixel[2] = prom;
        pixeles++;
    }
}

int promedio_colores(t_pixel* pixeles)
{
    return (int)((pixeles->pixel[0]+pixeles->pixel[1]+pixeles->pixel[2])/3);
}

void aplicar_monocromo(t_pixel* pixeles, const int cantidad)
{
    t_pixel* fin = pixeles + cantidad;  // Puntero al final del arreglo de píxeles
    for (t_pixel* p = pixeles; p < fin; p++)
    {
        int promedio = promedio_colores(p);  // Calcula el promedio de colores usando el puntero
        if (promedio <= 127)
        {
            p->pixel[0] = 0;  // Asigna el color negro a cada componente del píxel
            p->pixel[1] = 0;
            p->pixel[2] = 0;
        }
        else
        {
            p->pixel[0] = 255;  // Asigna el color blanco a cada componente del píxel
            p->pixel[1] = 255;
            p->pixel[2] = 255;
        }
    }
}
