#include "cmd.h"
#include <stdio.h>
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define APPEND 3

void aff_membres_args(cmd *c)
{
  int cpt,i;
  for(i = 0; i < c -> nb_cmd_membres; i++)
  {
        printf("Commande %d : \n", i);
        for(cpt = 0; cpt < c -> nb_membres_args[i];cpt++)
        {
            printf("Arg %d : %s\n",cpt, c -> cmd_membres_args[i][cpt]);
        }

  }

}

void aff_membres(cmd *ma_cmd)
{
  int i;
  for(i = 0; i < ma_cmd -> nb_cmd_membres; i++)
  {
    printf("Commande %d : %s\n",i, ma_cmd -> cmd_membres[i]);
  }
}

void parse_membres(char *chaine,cmd *ma_cmd)
{
    char* membres, *buffer;
    int i = 0;
    ma_cmd -> cmd_membres = malloc(sizeof((char**)ma_cmd -> cmd_membres));
    buffer = strdup(chaine);
    membres = strtok(buffer,"|");
    ma_cmd -> cmd_membres[0] = strdup(membres);
    while(membres != NULL)
    {
        membres = strtok(NULL,"|");
        i++;
        ma_cmd -> cmd_membres = realloc(ma_cmd -> cmd_membres, sizeof(char **) * i);
        if(membres != NULL)
            ma_cmd -> cmd_membres[i] = strdup(membres);
    }
    ma_cmd -> nb_cmd_membres = i;
}

void parse_membres_args(cmd *c)
{
  int i,cpt_args = 1;
  char* args, *chaine;

  c -> cmd_membres_args = malloc(sizeof(char**) * c -> nb_cmd_membres);
  c -> nb_membres_args = malloc(sizeof(unsigned int) * c->nb_cmd_membres);

  for(i = 0; i < c -> nb_cmd_membres ; i++)
  {
    c -> nb_membres_args[i] = 0;
  }

  i=0;

  while (i < c -> nb_cmd_membres)
  { 
    cpt_args = 0;
    c -> cmd_membres_args = realloc(c -> cmd_membres_args, sizeof(char **) *  c -> nb_cmd_membres);
    chaine = strdup(c -> cmd_membres[i]);
    args = strtok(chaine," ");
    c -> cmd_membres_args[i] = malloc(sizeof((char*) * (c -> cmd_membres_args[i]+1)));
    c -> cmd_membres_args[i][cpt_args] = strdup(args);
    cpt_args++;
    while(args != NULL)
    {
        args = strtok(NULL," ");
        if(args != NULL)
        {
            if((strcmp(args,"<") == 0) || (strcmp(args,">>") == 0) || (strcmp(args,">") == 0) || (strcmp(args,"\0") == 0))
            {
                c -> cmd_membres_args[i] = realloc(c -> cmd_membres_args[i], sizeof(args)*(cpt_args+1));
                c -> cmd_membres_args[i][cpt_args]= NULL;
                cpt_args ++;
                break;
            }
                c -> cmd_membres_args[i] = realloc(c -> cmd_membres_args[i], sizeof(args)*(cpt_args+1));
                c -> cmd_membres_args[i][cpt_args]=strdup(args);
                cpt_args++;
        }
    }
    c -> cmd_membres_args[i][cpt_args]= NULL;
    c -> nb_membres_args[i] = cpt_args;
    i++;
  }
    free(chaine);
}
void free_membres_args(cmd *c)
{
  int i, j;
  for(i = 0; i < c->nb_cmd_membres; i++)
  {

          for(j = 0; j < c->nb_membres_args[i]; j++)
          {
            free(c->cmd_membres_args[i][j]);
          }
            free(c->cmd_membres_args[i]);
  }
  free(c->cmd_membres_args);
}

void free_membres(cmd *ma_cmd)
{
    int i;
    for(i = 0; i < ma_cmd->nb_cmd_membres; i++)
    {
        free(ma_cmd->cmd_membres[i]);
    }
    free(ma_cmd->cmd_membres);
}


void parse_redirection(unsigned int i,cmd *c)
{
    char *args;
    char *chaine;
    int compteur;
    c -> redirection = malloc(sizeof(char**) * c -> nb_cmd_membres);
    c -> redirection[i] = malloc( 3 * sizeof(char *));
    c->redirection[i][STDIN] = NULL;
    c->redirection[i][STDOUT] = NULL;
    c->redirection[i][STDERR] = NULL;
    chaine = strdup(c->cmd_membres[i]);

   for (compteur = 0; chaine[compteur] != '\0'; compteur++)
   {
      if(chaine[compteur] == '<')
      {
          args = strtok(chaine, "<");
    args = strtok(NULL, "<");
    printf("On a %s\n", args);
          c->redirection[i][STDIN] = strdup(args);
          c->redirection[i][STDOUT] = NULL;
          c->redirection[i][STDERR] = NULL;
      }
      if(chaine[compteur] == '>')
      {
          args = strtok(chaine, ">");
    args = strtok(NULL, ">");
    printf("On a %s\n", args);  
          //c -> type_redirection[i][STDOUT] = APPEND;
          c->redirection[i][STDIN] = NULL;
          c->redirection[i][STDOUT] = strdup(args);
          c->redirection[i][STDERR] = NULL;
      }
   }
   free(chaine);
}

void free_redirection(cmd *c)
{
    int i;
    for(i=0; i < c->nb_cmd_membres; i++)
    {
        free(c->redirection[i][STDIN]);
        free(c->redirection[i][STDOUT]);
    }
    free(c->redirection);
}

void aff_redirection(cmd* c, int i)
{
    int cpt;
    for(cpt  = 0; cpt < 3 ; cpt++)
    {
        printf("Redirection %d : %s\n",i, c->redirection[i][cpt]);
    }
}
/*
Faire 4 exec() , donc 4 processus fils
Le pere attend la fin de ses processus pour lancer un nouvelle commande
Communiquer : utiliser des pipe() 3 tubes
le dernier fils affiche à l'écran
créer les tube un 1er et dans chaque fils ne pas oublier ceux qu'ils n'utilisent pas
Puis faire une redirection
*/
