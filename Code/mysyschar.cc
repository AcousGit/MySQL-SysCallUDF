#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <mysql/mysql.h>
 

extern "C" my_bool mysychar_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{

	my_bool maybe_null=0;

	if(args->arg_count !=1)
        {
                strcpy(message, "1 seul argument (mettre commande sous la forme \"commande1 option1 && commande2 option2 ...\")");
                return 1;
        }
        if(args->arg_type[0] != STRING_RESULT)
        {
                strcpy(message, "exec requires exactly one string argument");
                return 1;
        }
	
	initid->max_length=65000*sizeof(char);
        return 0; /* true */


	
}

extern "C" char *mysyschar(UDF_INIT *initid, UDF_ARGS *args,char *result, unsigned long *length,char *is_null, char *error)
{
	int std=dup(1); /*backup of stdout -- backup de la sortie standard */
        close(1); /*We close stdout -- On ferme la sortie standart  */
	/*
	When we open a file, its file descriptor is the first founded in descriptor table (/proc/1/fd). Here it is 1 (ie stdout). 
	NB: In /proc/1/fd, the "1" means current process. 
	-- 
 	Quand on ouvre un fichier, le descripteur de fichier est le premier descripteur disponible dans la table /proc/1/fd/ ici 1 (ie stdout).
	NB: Dans /proc/1/fd, le "1" signifie le processus en cours d'exécution.
	 */
        int file = open("/run/mysqld/.mysqlSysExec.txt", O_CREAT | O_RDWR, S_IRWXU); /*file becomes stdout -- file devient la sortie standard */
	if(file<0){ /* if you can't open the file let me know -- On est prévenu si on ne peut pas ouvrir le fichier */
                strcpy(result,"error open(): ");
                strcat(result,strerror(errno));
                *length=strlen(result);
                return result;

	}
	if(system(args->args[0])!=0){ /*let me know if you can't run system() -- On est prévenu si il y a une erreur avec system()*/
		strcpy(result,"error system(): ");
                strcat(result,strerror(errno));
                *length=strlen(result);
                return result;

	}
	else{ /* We print the result of system() in the mysql answere -- On affiche le résultat de system() dans la réponse de mysql*/
		char *p=NULL;
		p=(char *)mmap(NULL,65100*sizeof(char),PROT_READ,MAP_PRIVATE,file,(off_t)0);
		if(*p<0){
	                strcpy(result,"error mmap(): ");
        	        strcat(result,strerror(errno));
                	*length=strlen(result);
                	return result;
		}

		close(file);
		dup(std);
		close(std); /*stdout is reset to default value -- le terminal redevient la sortie standard*/

		if(remove("/run/mysqld/.mysqlSysExec.txt")<0){ /* We delete temporary files -- On supprime les fichiers temporaires */
	                strcpy(result,"error remove(): ");
                        strcat(result,strerror(errno));
                        *length=strlen(result);
                        return result;
		}

                strcpy(result,p);
                *length=strlen(result);
                return result;
	}
        		
}


extern "C" void mysyschar_deinit(UDF_INIT *initid)
{
	return;

}
