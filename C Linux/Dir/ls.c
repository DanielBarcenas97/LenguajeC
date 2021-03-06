/** 
 * Manejo de Directorios utilizando el estándar POSIX y la estructura DIRENT (dirent.h)
 
 * struct dirent {
    ino_t          d_ino;        inode number 
    off_t          d_off;        offset to the next dirent 
    unsigned short d_reclen;     length of this record 
    unsigned char  d_type;       type of file; not supported
                                   by all file system types 
    char           d_name[256];  filename 
	};
 	
 	Esta estructura es la que describe a los directorios en el estándar POSIX.
 	Para poder operar con directorios, hacemos uso de diversas llamadas al SO.
 				
 	int mkdir(const char *pathname, mode_t mode); 
 	- Nos permite crear directorios. El modo especifica los permisos de uso.
	DIR *opendir(const char *name);
	- Abre el flujo de directorio correspondiente al nombre del directorio especificado y nos regresa un puntero a este.
	El flujo se posiciona en la primera entrada del directorio.
	struct dirent *readdir(DIR *dirp);
	- Nos permite leer el flujo de directorio abierto previamente con la función opendir(const char *name).
	Esta función retorna un puntero a una estructura DIRENT que representa a la siguiente entrada en el flujo apuntado por dirp.
	Retorna NULL al llegar al final del flujo o al ocurrir un error.
	int rmdir(const char *pathname);
	- rmdir() remueve el directorio representado por el "pathname" si está vacío. SI no está vacio el directorio, 
	entonces la función falla.
	int closedir(DIR *dirp);
	- La función closedir() cierra el flujo a un directorio asociado con dirp.
	Una llamada exitosa a la función también cierra el descriptor de archivo asociado con dirp.
	char *getcwd(char *buf, size_t size);
	- Nos permite obtener el PATH absoluto del directorio apuntado por buf de tamaño size.
	La función nos devuelve una cadena que contiene el PATH absoluto del directorio en el que nos encontramos trabajando y también devuelve esta por medio
	del argumento buf. Si la longitud de la cadena excede size bytes, se retorna NULL.
  
 **/
 

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h> // Nos provee de funciones que nos permite el manejo de directorios en un entorno POSIX.
#include <stdio.h>
#include <string.h>
#define BUFF_SIZE 255

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

// Programa que emula el comando ls.

int32_t main(int32_t argc, char** argv){
	struct dirent *dir_ptr = NULL; // Tenemos nuestro apuntador a una estructura dirent. Para inicializarlo, le asignamos un valor NULL.
	DIR *dp = NULL; // Tenemos nuestro apuntador a un flujo de directorio.
	char *buf = NULL; // Buffer de caracteres.

	buf = malloc(sizeof(char)*BUFF_SIZE); // Asignamos a nuestro buffer de caracteres BUFF_SIZE bytes de memoria.
	char* nombre_dir = getcwd(buf, BUFF_SIZE); // El nombre del directorio va a ser nuestro directorio actual de trabajo.

	if (argc > 1){
		nombre_dir = argv[1]; // Si se especifica un directorio por medio de la linea de comandos, utilizamos ese en lugar de nuestro CWD.
	}

	/* DIR *opendir(const char *name); */
	dp = opendir(nombre_dir);

	if(dp == NULL){
		printf("\npathname invalido.\n");
		printf("pathname == NULL: %i\n", nombre_dir == NULL);
		return 1;
	}

	/* struct dirent *readdir(DIR *dirp); */
	/* La lógica para este recorrido es la siguiente:
		Recordemos que cuando llamamos a readdir sobre un flujo dp, este va a llamar a la SIGUIENTE entrada en ese flujo.
		Funciona de manera análoga a una lista ligada, por lo que, si volvieramos a llamar a readdir sobre ese mismo flujo, otra vez
		readdir nos devolveria un puntero a un struct dirent pero este seria el SIGUIENTE al de la anterior vez que hicimos la llamada a la función.
		De esta manera entonces, podemos recorrer cada uno de los elementos dentro del flujo de directorio.
	*/
	while ((dir_ptr = readdir(dp)) != NULL){
		unsigned char type = dir_ptr->d_type;
		char* name = dir_ptr->d_name;

		if (type == DT_DIR) // Si el tipo de archivo es un directorio...
			printf(BLU"*%s\n"RESET, name);
		else if(type == DT_REG) // Si el tipo de archivo es un fichero regular...
			//printf(GRN"*%s\n"RESET, name);
			printf("*%s\n", name);

	}

	closedir(dp);
	free(buf);

	return 0;
}