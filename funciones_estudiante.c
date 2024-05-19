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
    Apellido: Rodriguez
    Nombre: Iara Sol
    DNI: 42247089
    Entrega: si
    -----------------
    Apellido: Calvo
    Nombre: Ignacio
    DNI: 41162300
    Entrega: si
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
        printf("Uso: %s <operaciones> <archivo_original.bmp>\n", argv[0]);
        return ERROR_ARGUMENTOS;
    }

    FILE* archivo = abrir_archivo(argv[argc-1], "rb");
    t_metadata metadata = leer_bmp(archivo);
    t_pixel* pixeles = leer_pixeles(archivo, &metadata);

    int cantidad_pixeles = metadata.alto * metadata.ancho;

    fclose(archivo);

    char baseFileName[TAM_MAX_FILENAME];
    extraer_nombre_base(baseFileName, argv[argc-1]);

    for (int i = 1; i < argc - 1; i++)
    {
        // Copio para no abrir el archivos nuevamente
        t_pixel* pixeles_copiados = malloc(cantidad_pixeles * sizeof(t_pixel));
        memcpy(pixeles_copiados, pixeles, cantidad_pixeles * sizeof(t_pixel));

        t_metadata metadata_copia;
        memcpy(&metadata_copia, &metadata, sizeof(t_metadata));

        procesar_operacion(argv[i], pixeles_copiados, &metadata_copia, baseFileName);
        free(pixeles_copiados);
    }

    free(pixeles);
    return TODO_OK;
}

int procesar_operacion(char* operacion, t_pixel* pixeles, t_metadata* metadata, char* baseFileName)
{
    char fileNameDest[TAM_MAX_FILENAME];
    generar_nombre_archivo(fileNameDest, baseFileName, operacion);

    int cantidad_pixeles = metadata->ancho * metadata->alto;
    // Aplicación de negativo
    if (strcmp(operacion, "--negativo") == 0)
    {
        aplicar_negativo(pixeles, cantidad_pixeles);
    }
    else if(strcmp(operacion, "--escala-de-grises")== 0)
    {
        escala_de_grises(pixeles, cantidad_pixeles);
    }
    else if(strcmp(operacion, "--reducir-contraste")== 0)
    {
        reducir_contraste(pixeles, cantidad_pixeles);
    }
    else if(strcmp(operacion, "--aumentar-contraste")== 0)
    {
        aumentar_contraste(pixeles, cantidad_pixeles);
    }
    else if(strcmp(operacion, "--tonalidad-azul")== 0)
    {
        tonalidad_azul(pixeles, cantidad_pixeles);
    }
    else if(strcmp(operacion, "--tonalidad-roja")== 0)
    {
        tonalidad_roja(pixeles, cantidad_pixeles);
    }
    else if(strcmp(operacion, "--tonalidad-verde")== 0)
    {
        tonalidad_verde(pixeles, cantidad_pixeles);
    }
    else if(strcmp(operacion, "--recortar")== 0)
    {
        recortar(&pixeles, metadata);
    }
    else if(strcmp(operacion, "--rotar-derecha")== 0)
    {
        rotar_derecha(&pixeles, metadata);
    }
    else if(strcmp(operacion, "--rotar-izquierda")== 0)
    {
        rotar_izquierda(&pixeles, metadata);
    }
    else if(strcmp(operacion, "--achicar")== 0)
    {
        achicar(pixeles, metadata);
    }
    else if(strcmp(operacion, "--monocromo")== 0)
    {
        aplicar_monocromo(pixeles, cantidad_pixeles);
        guardar_monocromo(fileNameDest, pixeles, metadata);

        return TODO_OK;
    }
    else
    {
        printf("Operacion no valida\n");
        exit(ERROR_ARGUMENTOS);
    }

    guardar_bmp(fileNameDest, pixeles, metadata);

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
        fwrite(pixeles->pixel, 3, 1, archivo);  // Escribir 3 bytes por pixel
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
void generar_nombre_archivo(char* destino, const char* origen, const char* sufijo)
{
    while (*origen != '.' && *origen != '\0')
    {
        *destino = *origen;
        destino++;
        origen++;
    }
    *destino = '_';
    destino++;
    sufijo += 2;
    while (*sufijo != '\0')
    {
        if (*sufijo == '-')
        {
            *destino = '_';
        }
        else
        {
            *destino = *sufijo;
        }
        destino++;
        sufijo++;
    }
    strcpy(destino, ".bmp");
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
    aumentar_tonalidad(pixeles, cantidad, 0);
}

void tonalidad_verde(t_pixel* pixeles, const int cantidad)
{
    aumentar_tonalidad(pixeles, cantidad, 1);
}

void tonalidad_roja(t_pixel* pixeles, const int cantidad)
{
    aumentar_tonalidad(pixeles, cantidad, 2);
}

void aumentar_tonalidad(t_pixel* pixeles, const int cantidad, const int color)
{
    for (int i = 0; i < cantidad; i++)
    {
        int x= MAXRGB((int)(pixeles[i].pixel[color]*AUMENTO_TONALIDAD));
        pixeles[i].pixel[color] = x;
    }
}

//Rotar 90 grados a la izquierda
void rotar_izquierda(t_pixel** pixeles, t_metadata *meta)
{
    int nuevo_ancho = meta->alto;
    int nuevo_alto = meta->ancho;
    t_pixel* pixeles_rotados = malloc(nuevo_ancho * nuevo_alto * sizeof(t_pixel));
    if (!pixeles_rotados)
    {
        printf("Error al reservar memoria\n");
        exit(ERROR_MEMORIA_DINAMICA);
    }

    for (int i = 0; i < meta->alto; i++)
    {
        for (int j = 0; j < meta->ancho; j++)
        {
            pixeles_rotados[j * nuevo_ancho + (nuevo_ancho - i - 1)] = (*pixeles)[i * meta->ancho + j];
        }
    }

    *pixeles = pixeles_rotados;

    // Intercambiar ancho y alto usando XOR
    meta->ancho ^= meta->alto;
    meta->alto ^= meta->ancho;
    meta->ancho ^= meta->alto;
}

//Rotar 90 grados a la derecha
void rotar_derecha(t_pixel** pixeles, t_metadata *meta)
{
    int nuevo_ancho = meta->alto;  // El nuevo ancho es el alto original
    int nuevo_alto = meta->ancho;  // El nuevo alto es el ancho original
    t_pixel* pixeles_rotados = (t_pixel*)malloc(nuevo_ancho * nuevo_alto * sizeof(t_pixel));
    if (!pixeles_rotados)
    {
        printf("Error al reservar memoria\n");
        exit(ERROR_MEMORIA_DINAMICA);
    }

    for (int i = 0; i < meta->alto; i++) {
        for (int j = 0; j < meta->ancho; j++) {
            pixeles_rotados[j * nuevo_ancho + i] = (*pixeles)[i * meta->ancho + j];
        }
    }

    // Paso 2: Invertir el orden de las columnas
    for (int j = 0; j < nuevo_alto; j++) {
        for (int i = 0; i < nuevo_ancho; i++) {
            t_pixel temp = pixeles_rotados[i * nuevo_alto + j];
            pixeles_rotados[i * nuevo_alto + j] = pixeles_rotados[(nuevo_ancho - i - 1) * nuevo_alto + j];
            pixeles_rotados[(nuevo_ancho - i - 1) * nuevo_alto + j] = temp;
        }
    }

    free(*pixeles);
    *pixeles = pixeles_rotados;
    meta->ancho = nuevo_ancho;
    meta->alto = nuevo_alto;
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

void achicar(t_pixel* pixeles_originales, t_metadata* meta)
{
    int nuevo_ancho = meta->ancho / 2;
    int nuevo_alto = meta->alto / 2;

    for (int y = 0; y < nuevo_alto; y++)
    {
        for (int x = 0; x < nuevo_ancho; x++)
        {
            int pixel_index_original = (2 * y * meta->ancho) + (2 * x);
            int pixel_index_nuevo = (y * nuevo_ancho) + x;
            pixeles_originales[pixel_index_nuevo] = pixeles_originales[pixel_index_original];
        }
    }

    meta->ancho = nuevo_ancho;
    meta->alto = nuevo_alto;
    meta->tamArchivo = meta->comienzoImagen + (nuevo_ancho * nuevo_alto * sizeof(t_pixel));
}

void extraer_nombre_base(char* dest, const char* fullFileName)
{
    const char* lastDot = strrchr(fullFileName, '.');
    if (lastDot)
    {
        size_t nameLength = lastDot - fullFileName;
        strncpy(dest, fullFileName, nameLength);
        dest[nameLength] = '\0';
    }
    else
    {
        strcpy(dest, fullFileName);
    }
}

void recortar(t_pixel** pixeles, t_metadata* metadata)
{
    // Calcular las nuevas dimensiones
    unsigned int nuevo_ancho = metadata->ancho / 2;
    unsigned int nuevo_alto = metadata->alto / 2;

    // Crear un nuevo array para los píxeles recortados
    t_pixel* pixeles_recortados = malloc(nuevo_ancho * nuevo_alto * sizeof(t_pixel));
    if (!pixeles_recortados)
    {
        printf("Error al reservar memoria para pixeles recortados\n");
        exit(ERROR_MEMORIA_DINAMICA);
    }

    // Copiar los píxeles de la parte superior izquierda de la imagen original
    for (int y = 0; y < nuevo_alto; y++)
    {
        for (int x = 0; x < nuevo_ancho; x++)
        {
            int idx_original = y * metadata->ancho + x;
            int idx_nuevo = y * nuevo_ancho + x;
            pixeles_recortados[idx_nuevo] = (*pixeles)[idx_original];
        }
    }

    // Actualizar el puntero de píxeles al nuevo array recortado
    *pixeles = pixeles_recortados;

    // Actualizar metadata para reflejar el nuevo tamaño
    metadata->ancho = nuevo_ancho;
    metadata->alto = nuevo_alto;
}
