#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> //fork()
#include <time.h> //for date
#include <string.h> //command comparisons

#include <stdio.h>

#include "mypro.h"

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

int styles = 0;//varíavel para definir o "state" do prompt

void showDate() { //função pra retornar data atual
    
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        printf("Data atual: %d/%d/%d\n", tm.tm_mday, (tm.tm_mon+1), tm.tm_year+1900);
}

int launch(char **args) //pega a lista de argumentos passada anteriormente, forka o processo e salva o valor de retorno.

{
        pid_t pid;

        //uma vez que fork retorna, temos 2 processos rodando concurrentemente.
        pid = fork();

        //O filho entra aqui
        if (pid == 0) {
                //queremos executar o comando passado pelo usuário
                //se retornar -1 saberei que deu erro. Execvp n deveria retornar
                execvp(args[0], args);
        } 

        //Checa se o fork deu algum erro. Se sim, apresenta o erro e vaza
        else if (pid < 0) {
                fprintf(stderr, "Fork Failed");
                return 1;
        } 
        //Fork funcionou
        else {
        //Aqui entra o processo pai
                wait(NULL);
        }

        return 1;
}

int executar(char **args)
{
        //Procurar se as funções são built-in. 
        //Se não forem, vão para os ifs específicos

        int i;

        if (args[0] == NULL) {
        // Sem comandos
                return 1;
        }

        for (i = 0; i < 2; i++) {

                if ((strcmp(args[0], "date") == 0) && (args[1] == NULL)){
                        showDate();
                        return 1;
                }

                else if ((strcmp(args[0], "style") == 0) && (args[1] == NULL)){

                }

                else if (strcmp(args[0], "style") == 0){

                        if (strcmp(args[1], "sequential")==0){
                                styles = 0;
                                return 1;
                        }

                        else if (strcmp(args[1], "parallel")==0){
                                styles = 1;
                                return 1;
                        }

                        else{
                                fprintf(stderr, "Error: Style inválido.\n");
                                break;
                        }


                } 
                else if ((strcmp(args[0], "exit")==0) && (args[1] == NULL)){
                    return 0;
                }

        }          

        return launch(args);
}

char **divide_linha(char *linha)
{
        //Buffer que não é fixo, vai se expandindo de acordo com as necessidades.
        //Array de ponteiros terminado em NULL
        int bufsize = LSH_TOK_BUFSIZE, position = 0;
        char **tokens = malloc(bufsize * sizeof(char*));
        char *token, **tokens_pivo;

        if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
        }

        //strtok() retorna os ponteiros  de acordo com a string que você deu, e coloca bytes \0 no fim de cada token
        //guardamos cada ponteior em um array (buffer) de pointeros caracteres
        token = strtok(linha, LSH_TOK_DELIM);
        
        //esse processo continua até que nenhum token é retornado de strtok
        while (token != NULL) {
                tokens[position] = token;
                position++;

                //realocação do array de ponteiros (se necessário)
                if (position >= bufsize) {
                        bufsize += LSH_TOK_BUFSIZE;
                        tokens_pivo = tokens;
                        tokens = realloc(tokens, bufsize * sizeof(char*));
                        
                        if (!tokens) {
    
                                free(tokens_pivo);
                                fprintf(stderr, "lsh: allocation error\n");
                                exit(EXIT_FAILURE);
                        }
                }

                token = strtok(NULL, LSH_TOK_DELIM);
        }
  
        tokens[position] = NULL;
        return tokens;
}//ao fim desta função eu tenho um array de tokens

//Loop de execução
void prompt_loop() {

        char *linha;
        char **args;
        int status = 1;

        while (status) {
        
                if (styles == 0)
                        printf("myPrompt seq > ");
                else
                        printf("myPrompt par > ");

                ssize_t bufsize = 0; //Alocação de buffer

                if (getline(&linha, &bufsize, stdin) == -1){
                        if (feof(stdin)) {
                                exit(EXIT_SUCCESS);  //Chegou em EOF
                        } else  {
                                perror("readline");
                                exit(EXIT_FAILURE);
                        }
                }

                args = divide_linha(linha);
                status = executar(args);

        }

        
}