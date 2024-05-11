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
    char fileNameDest[80];
    /*
        Aquí deben hacer el código que solucione lo solicitado.
        Todas las funciones utilizadas deben estar declaradas en este archivo, y en su respectivo .h
    */

    if (argc < 3)
    {
        printf("Uso: %s <operacion> <archivo_original.bmp>\n", argv[0]);
        return ERROR_ARGUMENTOS;  // Define ERROR en constantes.h
    }

    FILE* archivo = abrir_archivo(argv[2], "rb");

    if(!es_bmp(archivo)){
        printf("El archivo no es un BMP\n");
        fclose(archivo);
        exit(ERROR_APERTURA_ARCHIVO);
    }

    t_metadata imagen = leer_bmp(archivo);

    t_pixel* pixeles = leer_pixeles(archivo, imagen); // memoria dinamica

    fclose(archivo);

    // Aplicación de negativo
    if (strcmp(argv[1], "--negativo") == 0)
    {
        aplicar_negativo(pixeles, imagen.ancho * imagen.alto);
        ;
    }else if(strcmp(argv[1], "--escala-de-grises")){

    }else if(strcmp(argv[1], "--aumentar-contraste")){

    }else if(strcmp(argv[1], "--tonalidad-azul")){

    }else if(strcmp(argv[1], "--tonalidad-roja")){
 
    }else if(strcmp(argv[1], "--tonalidad-verde")){

    }else if(strcmp(argv[1], "--recortar")){

    }else if(strcmp(argv[1], "--rotar-derecha")){

    }else if(strcmp(argv[1], "--rotar-izquierda")){

    }else if(strcmp(argv[1], "--achicar")){
        
    }else if(strcmp(argv[1], "--monocromo")){

    }else{
        printf("Operacion no valida\n");
        free(pixeles);
        exit(ERROR_ARGUMENTOS);
    }
    asignar_nombre_archivo(fileNameDest, argv[2], argv[1]);

    

    // Guardo la imagen modificada
    guardar_bmp(fileNameDest, pixeles, imagen);
    free(pixeles); // libera

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

    if(!pixeles){
        printf("Error al reservar memoria\n");
        exit(ERROR_MEMORIA_DINAMICA);
    }

    fseek(archivo, meta.comienzoImagen, SEEK_SET);
    for (int i = 0; i < meta.ancho * meta.alto; i++)
    {
        fread(pixeles[i].pixel, 3, 1, archivo);  // Leer 3 bytes por pixel
    }

    return pixeles;
}

void guardar_bmp(const char* filename, t_pixel* pixeles, t_metadata meta) {
    //TODO: ver que pasa cuando el archivo falla para liberar memoria
    FILE* archivo = abrir_archivo(filename, "wb");

    // Escribe el encabezado BMP
    //escribir_encabezado_bmp(archivo, meta);

    // Escribe los datos de los píxeles
    fseek(archivo, meta.comienzoImagen, SEEK_SET);
    for (int i = 0; i < meta.ancho * meta.alto; i++) {
        fwrite(pixeles[i].pixel, 1, 3, archivo);  // Escribir 3 bytes por pixel
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

FILE* abrir_archivo(const char* filename, const char* modo)
{
    FILE* archivo = fopen(filename, modo);
    if (!archivo)
    {
        printf("Error al abrir el archivo %s\n", filename);
        exit(ERROR_APERTURA_ARCHIVO);
    }
    return archivo;
}

int es_bmp(FILE* archivo){
    char tipo[TAM_CABECERA_BMP];
    fread(&tipo, sizeof(char)*TAM_CABECERA_BMP, 1, archivo);
    if(strcmp(tipo, "BM") != 0)
        return 0;
    return 1;
}

void reducir_contraste(t_pixel* pixeles, int cantidad) // Reduce 25% el contraste
{
    for (int i = 0; i < cantidad; i++)
    {
        pixeles[i].pixel[0] = pixeles[i].pixel[0] * REDUCCION_CONTRASTE;  // Rojo
        pixeles[i].pixel[1] = pixeles[i].pixel[1] * REDUCCION_CONTRASTE;  // Verde
        pixeles[i].pixel[2] = pixeles[i].pixel[2] * REDUCCION_CONTRASTE;  // Azul
    }   
}

void aumentar_contraste(t_pixel* pixeles, int cantidad) // Aumenta 25% el contraste
// Si es mayor a 255 queda en ese valor.
{
    for (int i = 0; i < cantidad; i++)
    {
        pixeles[i].pixel[0] = pixeles[i].pixel[0] * AUMENTO_CONTRASTE;  // Rojo
        pixeles[i].pixel[1] = pixeles[i].pixel[1] * AUMENTO_CONTRASTE;  // Verde
        pixeles[i].pixel[2] = pixeles[i].pixel[2] * AUMENTO_CONTRASTE;  // Azul
        if(pixeles[i].pixel[0] > 255)
            pixeles[i].pixel[0] = 255;
        if(pixeles[i].pixel[1] > 255)
            pixeles[i].pixel[1] = 255;
        if(pixeles[i].pixel[2] > 255)
            pixeles[i].pixel[2] = 255;
    }
}   
/*
    Asigna un nombre de archivo con sufijo a partir de un nombre de archivo original.
    Ejemplo: si el archivo original es "imagen.bmp" y el sufijo es "--negativo", el archivo destino será "imagen_negativo.bmp"
*/
void asignar_nombre_archivo(char* destino, const char* origen, const char* sufijo){
    //origen = "imagen.bmp" termina en .
    while(*origen != '.'){
        *destino = *origen;
        destino++;
        origen++;
    }
    //destino agrego el _
    destino++;
    *destino='_';
    
    //le agrego el sufijo, elimino el --
    sufijo+2;
    while(*sufijo != '\0'){
        *destino = *sufijo;
        destino++;
        sufijo++;
    }

    strcat(destino, '.bmp\0');
}