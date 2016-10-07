#include "shell_fct.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int exec_commande(cmd* ma_cmd)
{
	pid_t* Child = malloc(sizeof(pid_t) * ma_cmd->nb_cmd_membres) ;
	int i;
	int STDin, STDout;
	STDin=dup(STDIN_FILENO);
	STDout=dup(STDOUT_FILENO);
	int **tabTube;

	tabTube = malloc(sizeof(int *) * ma_cmd -> nb_cmd_membres);

	for (i = 0; i < ma_cmd -> nb_cmd_membres; i++)
	{
		tabTube[i] = malloc(sizeof(int) * 2);
	}

    char buffer[100];
    int *tabStatut = malloc(sizeof(int) * ma_cmd->nb_cmd_membres) ;
    int fd;
    for(i = 0; i< ma_cmd->nb_cmd_membres; i++)
    {
		pipe(tabTube[i]);
        Child[i] = fork();
        
		if (Child[i] == 0)
		{
		    if(i == 0)
		    {
		        if(ma_cmd -> redirection[i][STDIN] != NULL)
		    	{        				
					fd = open(ma_cmd -> redirection[i][STDIN], O_RDONLY, 0);
			        dup2(fd,0);
        			close(fd);
        		}
        		else if(ma_cmd -> redirection[i][STDOUT] != NULL)
		    	{        				
					fd = open(ma_cmd -> redirection[i][STDOUT], O_WRONLY, 0);
			        dup2(fd,1);
        			close(fd);
        		}
        		else
        		{
		         // Redirection entrée
		        close(tabTube[i][0]);
		        dup2(tabTube[i][1],1);
		        close(tabTube[i][1]);
		    	}

		        if(execvp(ma_cmd->cmd_membres_args[i][0], ma_cmd->cmd_membres_args[i]) == -1)
		        {
		            perror("execvp");
		            exit(errno);
		        }
		        exit(0);
		    }

		    else
		    {
		        // Redirection entrée
		        close(tabTube[i - 1][1]);
		        dup2(tabTube[i - 1][0],0);
		        close(tabTube[i - 1][0]);

		        if(ma_cmd -> redirection[i][STDIN] != NULL)
		    	{        				
					fd = open(ma_cmd -> redirection[i][STDIN], O_RDONLY, 0);
			        dup2(fd,0);
        			close(fd);
        		}
        		else if(ma_cmd -> redirection[i][STDOUT] != NULL)
		    	{        				
					fd = open(ma_cmd -> redirection[i][STDOUT], O_WRONLY, 0);
			        dup2(fd,1);
        			close(fd);
        		}
        		else
        		{
		        	//Redirection Sortie
		        	close(tabTube[i][0]);
		        	dup2(tabTube[i][1],1);
		        	close(tabTube[i][1]);
		        }
		        if(execvp(ma_cmd->cmd_membres_args[i][0], ma_cmd->cmd_membres_args[i]) == -1)
		        {
		            perror("execvp");
		            exit(errno);
		        }
		        exit(0);
		    }
		}
        else
        {
			if (i != 0)
			{
				//Fermeture du pipe 
		        close(tabTube[i - 1][0]);
		        close(tabTube[i - 1][1]);
			}	
			if (i == ma_cmd->nb_cmd_membres - 1)
			{
		
		        close(tabTube[i][1]);
		        dup2(tabTube[i][0], 0);
		        close(tabTube[i][0]);
		        
		        //Synchronisation
		        waitpid(Child[i], &tabStatut[i], 0);

		        //Affichage
		        while(fgets(buffer, 100, stdin) != NULL)
		        {
		            printf("%s", buffer);
		        }
	
			}
        }
    }
    dup2(0,STDin);
    close(0);
    dup2(1,STDout);
    close(1);
    

	free(Child);
	for (i = 0; i < ma_cmd -> nb_cmd_membres; i++)
	{
		free(tabTube[i]);
	}
	
	free(tabTube);
	free(tabStatut);
 
    return 0;

}