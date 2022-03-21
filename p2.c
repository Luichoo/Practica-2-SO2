//
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

void crea_dir(char **argv);

int main(int argc, char **argv){
    if(argc!=3){
        printf("Datos ingresados incorrectamente. (./[ejecutable] [directorio] [Archivo sin terminacion].\n");
        exit(EXIT_FAILURE);
    }
    crea_dir(&*argv);

return 0;
}
/////////////////////////////////////////////////////////
void crea_dir(char **argv){
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

}