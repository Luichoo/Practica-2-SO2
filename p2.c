/*/////////////////////////////////////////////////////////////////////
    Integrantes del equipo:
                            Luis Antonio Blanco Conde
                            Gustavo Contreras Mejia
                            Alejandro Octavio Salas Comparan
                            Gabriel Salom Fernandez
/////////////////////////////////////////////////////////////////////*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

/////////////////////////////////////////////////////////

    /*Declaracion de argumentos maximos de arrays y variables globales*/

#define MAXDIR 50
#define MAXPATH 100
#define MAXNAME 200

/////////////////////////////////////////////////////////

            /*Declaracion de funciones*/

void crea_dir(char **,char [MAXPATH]);
void crea_path(char [MAXPATH],char **);
void crea_archivo(char [MAXPATH],char **);
void imprimeinfo();

/////////////////////////////////////////////////////////
int main(int argc, char **argv){
char path[MAXPATH]={"./"};                                  //inicializamos el path en el padre  
int fdesc;
    if(argc!=3){                                            //Verifica que sean solo 3 argumentos, si son menos o mas, el programa se sale
        printf("Datos ingresados incorrectamente. (./[ejecutable] [directorio] [Archivo sin terminacion].\n");
        exit(EXIT_FAILURE);
    }
    crea_dir(&*argv,path);
    crea_archivo(path,&*argv);
    imprimeinfo();
return 0;
}
/////////////////////////////////////////////////////////
void imprimeinfo(){                                         //Funcion que imprime los atributos de entradas del directorio de trabajo
DIR *dir;
int fdesc;
struct dirent *info;
struct stat infofile;
    dir=opendir("./");                                      //abre el directorio padre
    if(!dir)
    {
        closedir(dir);
        printf("directorio no existente\n");
        exit(EXIT_FAILURE);
    }
    printf("Atributos del directorio padre: \n");
    printf("+--------------------+---------------+\n");
    printf("|%*s|%*s|\n", -20, " Nombre", -15," Inodo");    
    printf("+--------------------+---------------+\n");
    do{
        info=readdir(dir);                                  //empieza a leer cada dato del directorio hasta que ya no queden datos
        if(info){
            fdesc=open(info->d_name, O_RDONLY);             //Saca el nombre del archivo encontrado
            if(fdesc!=-1){
                fstat(fdesc,&infofile);                     //Saca la info del inodo del archivo
                printf("|%*s|%*ld|\n",-20,info->d_name,-15,infofile.st_ino);
            }
        }
    }while(info);
    printf("+--------------------+---------------+\n");
}
/////////////////////////////////////////////////////////
void crea_archivo(char path[MAXPATH],char **argv){          //funcion que genera el archivo, sus enlaces e imprime su descripcion          
int fdesc;
char name[MAXNAME];
struct stat data;
char nombre[4][MAXNAME]={"Luis Antonio Blanco Conde\n",     //Arreglo de nombres para el write para igual sacar sus caracteres
                        "Gustavo Contreras Mejia\n",
                        "Alejandro Octavio Salas Comparan\n",
                        "Gabriel Salom Fernandez\n"};

    fdesc=open(path, O_WRONLY|O_CREAT|O_TRUNC,0777);        //Creacion del archivo en el path con permisos rwxrwxrwx
    if(fdesc!=-1){
        printf("\nArchivo creado\n");
        write(fdesc, nombre[0],strlen(nombre[0]));
        write(fdesc, nombre[1],strlen(nombre[1]));
        write(fdesc, nombre[2],strlen(nombre[2]));
        write(fdesc, nombre[3],strlen(nombre[3]));
        close(fdesc);
    }
    else{
        printf("Archivo no existente\n");
        exit(EXIT_FAILURE);
    }
    strncpy(name,*(argv+1),1);
    strcpy(name,*(argv+2));
    strcat(name,"-hardlink");
    if(link(path,name)==-1){                       //creacion del enlace fisico
        printf("\nCreacion fallida del enlace fisico");
        exit(EXIT_FAILURE);
    }
    printf("\np2-hardlink creado");
    strncpy(name,*(argv+1),1);
    strcpy(name,*(argv+2));
    strcat(name,"-symlink");
    if(symlink(path,(name))==-1){                   //creacion del enlace simbolico
        printf("\nCreacion fallida del enlace simbolico");
        exit(EXIT_FAILURE);
    }
    printf("\np2-symlink creado\n\n");
    fdesc = open(path, O_RDONLY);                           //apertura del archivo creado para sacar su informacion usando stat
    if(fdesc != -1){
        fstat(fdesc, &data);
    printf("+------------------------------------+\n");
    printf("|%*s %*s|\n", -3,"",-32, " Informacion del Archivo");    
    printf("+--------------------+---------------+\n");
    printf("|%*s|%*s|\n", -20, " Nombre", -15,*(argv+2));    
    printf("+--------------------+---------------+\n");
    printf("|%*s|%*ld|\n", -20, " Inodo", -15,data.st_ino);    
    printf("+--------------------+---------------+\n");
    printf("|%*s|%*ld|\n", -20, " Size", -15,data.st_size);    
    printf("+--------------------+---------------+\n");
    printf("|%*s|%*ld|\n", -20, " Enlaces", -15,data.st_nlink);    
    printf("+--------------------+---------------+\n\n\n");
    }
    else{
        printf("\nArchivo no encontrado");
        close(fdesc);
        exit(EXIT_FAILURE);
    }
    close(fdesc);
}
/////////////////////////////////////////////////////////
void crea_path(char path[MAXPATH],char **argv){             //funcion que genera el path dentro del directorio
    strcat(path,*(argv+1));
    strcat(path,"/");
    strcat(path,*(argv+2));
    strcat(path,".txt");
}
/////////////////////////////////////////////////////////   
void crea_dir(char **argv,char path[MAXPATH]){              //funcion que genera el directorio y verifica nombres de los argumentos dados
struct stat dirinfo = {0};
char nombre[MAXDIR];
    while(stat(*(argv+1),&dirinfo)!=-1){                    //Checa si existe el directorio, si existe, le pedira cambio de nombre al usuario
        printf("\ndirectorio ya existente, ingrese otro nombre: ");
        fflush(stdin);
        scanf("%s",nombre);
        strcpy(*(argv+1),nombre);
    }
    if(mkdir(*(argv+1),0755)==-1){                          //directorio creado con permisos rwxr-xr-x usando los por defecto
        printf("\nError en la creacion del directorio.");
        exit(EXIT_FAILURE);
    }
    crea_path(path,&*argv);
        while(stat(path,&dirinfo)!=-1){                     //Checa si existe el archivo dentro del directorio, si existe, le pedira cambio de nombre al usuario
        printf("\nnombre de archivo ya existente, ingrese otro nombre: ");
        fflush(stdin);
        scanf("%s",nombre);
        strcpy(*(argv+2),nombre);
        strcpy(path,"./");
        crea_path(path,*&argv);
    }
}