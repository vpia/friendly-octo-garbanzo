#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include "shell_fct.h"
#define MYSHELL_CMD_OK 0
#define MYSHELL_FCT_EXIT 1

int main(int argc, char** argv)
{
	//..........
	int ret = MYSHELL_CMD_OK;
	cmd mycmd;
	char* readlineptr;
	struct passwd* infos;
	char str[1024];
	char hostname[256];
	char workingdirectory[256];

	//..........
	while(ret != MYSHELL_FCT_EXIT)
	{
		infos=getpwuid(getuid());
		gethostname(hostname, 256);
		getcwd(workingdirectory, 256);

		sprintf(str, "\n{myshell}%s@%s:%s$ ", infos->pw_name, hostname, workingdirectory);
		readlineptr = readline(str);

		//..........
		parse_membres(readlineptr, &mycmd);
		aff_membres(&mycmd);
		parse_membres_args(&mycmd);
		aff_membres_args(&mycmd);
		int i;
		for (i = 0; i < mycmd.nb_cmd_membres; i++)
		{	
			parse_redirection(i, &mycmd);
			aff_redirection(&mycmd,i);
		}
		exec_commande(&mycmd);
		//..........

		free_membres_args(&mycmd);
		free_membres(&mycmd);
		free_redirection(&mycmd);
		free(readlineptr);
		//..........

	}
	//..........
	return 0;
}
