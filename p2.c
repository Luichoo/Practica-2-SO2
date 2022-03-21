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

#define MAXPATH 100

void crea_dir(char **,char [MAXPATH]);
void crea_path(char [MAXPATH],char **);
void crea_archivo(char [MAXPATH],char **);
void imprimeinfo();

int main(int argc, char **argv){
strcat(*(argv+2),".txt");
char path[MAXPATH]={"./"};    
int fdesc;

    if(argc!=3){
        printf("Datos ingresados incorrectamente. (./[ejecutable] [directorio] [Archivo sin terminacion].\n");
        exit(EXIT_FAILURE);
    }
    crea_dir(&*argv,path);
    crea_archivo(path,&*argv);
    imprimeinfo();
return 0;
}
/////////////////////////////////////////////////////////
void imprimeinfo(){
DIR *dir;
int fdesc;
struct dirent *info;
struct stat infofile;
    dir=opendir("./");
    if(!dir)
    {
        closedir(dir);
        printf("directorio no existente\n");
        exit(EXIT_FAILURE);
    }
    do{
        info=readdir(dir);
        if(info){
            fdesc=open(info->d_name, O_RDONLY);
            if(fdesc!=-1){
                fstat(fdesc,&infofile);
                printf("\n%s--------------------Inodo: %ld",info->d_name,infofile.st_ino);
            }
        }
    }while(info);

}
/////////////////////////////////////////////////////////
void crea_archivo(char path[MAXPATH],char **argv){
int fdesc;
struct stat data;
char nombre[4][200]={"Luis Antonio Blanco Conde\n",
                     "Gustavo Contreras Mejia\n",
                     "Alejandro Octavio Salas Comparan\n",
                     "Gabriel Salom Fernandez\n"};

    fdesc=open(path, O_WRONLY|O_CREAT|O_TRUNC,0777);
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
    if(link(path,"p2-hardlink")==-1){
        printf("\nCreacion fallida del hardlink");
        exit(EXIT_FAILURE);
    }
    if(symlink(path,"p2-symlink")==-1){
        printf("\nCreacion fallida del symlink");
        exit(EXIT_FAILURE);
    }
    fdesc = open(path, O_RDONLY);
    if(fdesc != -1){
        fstat(fdesc, &data);
        printf("Informacion:\nNombre: %s\ninodo: %ld\nSize: %ld\nenlaces: %ld\n\n\n",*(argv+2),data.st_ino,data.st_size,data.st_nlink);
    }
    else{
        printf("\nArchivo no encontrado");
        close(fdesc);
        exit(EXIT_FAILURE);
    }
    close(fdesc);
}
/////////////////////////////////////////////////////////
void crea_path(char path[MAXPATH],char **argv){
    strcat(path,*(argv+1));
    strcat(path,"/");
    strcat(path,*(argv+2));
}
/////////////////////////////////////////////////////////
void crea_dir(char **argv,char path[MAXPATH]){
struct stat dirinfo = {0};
char nombre[100];
    while(stat(*(argv+1),&dirinfo)!=-1){
        printf("\ndirectorio ya existente, ingrese otro nombre: ");
        fflush(stdin);
        scanf("%s",nombre);
        strcpy(*(argv+1),nombre);
    }
    if(mkdir(*(argv+1),0755)==-1){
        printf("\nError en la creacion del directorio.");
        exit(EXIT_FAILURE);
    }
    crea_path(path,&*argv);
        while(stat(path,&dirinfo)!=-1){
        printf("\nnombre de archivo ya existente, ingrese otro nombre: ");
        fflush(stdin);
        scanf("%s",nombre);
        strcpy(*(argv+2),nombre);
        strcat(*(argv+2),".txt");
        strcpy(path,"./");
        crea_path(path,*&argv);
    }
}