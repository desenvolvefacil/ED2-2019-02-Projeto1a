/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: carlao
 * N USP 8901184
 * Created on 29 de Março de 2019, 13:03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_PAGINA 16000
#define TAMANHO_REGISTRO 80
#define nomeArqWB "wbfile.bin"

/*
 * 
 */
int main() {

    char TAG_CAMPO_CIDADE = '4';
    char TAG_CAMPO_ESCOLA = '5';

    //comando a ser lido
    //char comando[100] = "1 csv.csv";
    char comando[100] = "2 wbfile.bin";

    //varicavel que guarda a opcao selecioanda
    int opc = 0;

    //leitura do comando a ser executado
    //fgets(comando, 100, stdin);

    //pega o tamnho do comando lido
    size_t ln = strlen(comando) - 1;

    //elimina o \n caso houve para ajuda no processamento
    if (comando[ln] == '\n') {
        comando[ln] = '\0';
    }

    //verifica o inteiro digitado no comando
    opc = atoi(strtok(comando, " "));

    // exemplo de comando
    // 1 arquivo.csv 
    if (opc == 1) {

        char * nomeArquivo = strtok(NULL, " ");

        FILE * file = fopen(nomeArquivo, "r+");

        if (file) {


            //cria o arquivo para salvar os dados
            FILE * wbFile = fopen(nomeArqWB, "wb+");

            char buff[80];

            int vez = 1;

            while (!feof(file)) {
                // Lê uma linha (inclusive com o '\n')
                char * result = fgets(buff, 200, file); // o 'fgets' lê até 99 caracteres ou até o '\n'

                //verifica se e uma linha valida
                if (result != NULL) {
                    //printf("%s\n", result);

                    size_t ln = strlen(result) - 1;
                    //remove o enter e o carriage return
                    if (result[ln] == '\n') {
                        result[ln] = '\0';

                        if (result[ln - 1] == '\r') {
                            result[ln - 1] = '\0';
                        }
                    }

                    if (vez == 1) {

                        //recolhe os dados do cabecalho

                        //grava o status
                        char status = '1';
                        fwrite(&status, sizeof (status), 1, wbFile);

                        //grava o topo da pilha
                        int topoPilha = -1;
                        fwrite(&topoPilha, sizeof (topoPilha), 1, wbFile);

                        //grava os dados do campo 1
                        char tagCampo1 = '1';
                        fwrite(&tagCampo1, sizeof (tagCampo1), 1, wbFile);

                        char desCampo1[55] = "numero de inscricao do participante do ENEM\0@@@@@@@@@@@";
                        fwrite(&desCampo1, sizeof (desCampo1), 1, wbFile);

                        //grava os dados do campo 2
                        char tagCampo2 = '2';
                        fwrite(&tagCampo2, sizeof (tagCampo2), 1, wbFile);

                        char desCampo2[55] = "nota do participante do ENEM na prova de matematica\0@@@";
                        fwrite(&desCampo2, sizeof (desCampo2), 1, wbFile);

                        //grava os dados do campo 3
                        char tagCampo3 = '3';
                        fwrite(&tagCampo3, sizeof (tagCampo3), 1, wbFile);

                        char desCampo3[55] = "data\0@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
                        fwrite(&desCampo3, sizeof (desCampo3), 1, wbFile);


                        //grava os dados do campo 4
                        char tagCampo4 = '4';
                        fwrite(&tagCampo4, sizeof (tagCampo4), 1, wbFile);

                        char desCampo4[55] = "cidade na qual o participante do ENEM mora\0@@@@@@@@@@@@";
                        fwrite(&desCampo4, sizeof (desCampo4), 1, wbFile);


                        //grava os dados do campo 4
                        char tagCampo5 = '5';
                        fwrite(&tagCampo5, sizeof (tagCampo5), 1, wbFile);

                        char desCampo5[55] = "nome da escola de ensino medio\0@@@@@@@@@@@@@@@@@@@@@@@@";
                        fwrite(&desCampo5, sizeof (desCampo5), 1, wbFile);


                        char arr = '@';

                        //for para completar com @ e deixar o cabeçalho em uma pagina só
                        int i;

                        for (i = 285; i < TAMANHO_PAGINA; i++) {
                            fwrite(&arr, sizeof (arr), 1, wbFile);
                        }
                        //break;

                    } else {
                        //grava o valor de removido
                        char arr = '-';
                        fwrite(&arr, sizeof (arr), 1, wbFile);

                        //grava o encadeamento
                        int encadeamento = -1;
                        fwrite(&encadeamento, sizeof (encadeamento), 1, wbFile);

                        //começa a ler os dados
                        char * tmp = strsep(&result, ",");

                        int nroInscricao = atoi(tmp);

                        //grava no arquivo binario
                        fwrite(&nroInscricao, sizeof (nroInscricao), 1, wbFile);

                        //pega  anote
                        tmp = strsep(&result, ",");

                        double nota = -1;

                        if (strlen(tmp)) {
                            nota = strtod(tmp, NULL);
                        }
                        //grava no arquivo binario
                        fwrite(&nota, sizeof (nota), 1, wbFile);


                        //printf("%lf\n",nota);

                        tmp = strsep(&result, ",");
                        char data[10] = "\0@@@@@@@@@";

                        if (strlen(tmp)) {
                            strncpy(data, tmp, sizeof (data));
                        }

                        fwrite(&data, sizeof (data), 1, wbFile);


                        size_t totalBytes = 27;


                        char * cidade = strsep(&result, ",");

                        int tamanhoCidade = strlen(cidade) + 1;
                        if (tamanhoCidade > 1) {

                            //concatena com \0
                            cidade = strncat(cidade, "\0", tamanhoCidade);

                            //salva o tamanho do campo
                            fwrite(&tamanhoCidade, sizeof (tamanhoCidade), 1, wbFile);

                            //salva a tag do campo
                            fwrite(&TAG_CAMPO_CIDADE, sizeof (char), 1, wbFile);

                            fwrite(cidade, tamanhoCidade, 1, wbFile);

                            totalBytes += 5 + tamanhoCidade;
                        }

                        /*size_t ok = sizeof(cidade);
                        ok = strlen(cidade);
                        break;*/

                        char * nomeEscola = strsep(&result, ",");

                        int tamanhoEscola = strlen(nomeEscola) + 1;

                        if (tamanhoEscola > 1) {

                            //concatena com \0
                            nomeEscola = strncat(nomeEscola, "\0", tamanhoEscola);

                            //salva o tamanho do campo
                            fwrite(&tamanhoEscola, sizeof (tamanhoEscola), 1, wbFile);

                            //salva a tag do campo
                            fwrite(&TAG_CAMPO_ESCOLA, sizeof (char), 1, wbFile);

                            fwrite(nomeEscola, tamanhoEscola, 1, wbFile);

                            totalBytes += 5 + tamanhoEscola;
                        }


                        arr = '@';
                        for (; totalBytes < TAMANHO_REGISTRO; totalBytes++) {
                            fwrite(&arr, sizeof (arr), 1, wbFile);
                        }

                        //break;
                        /*if(vez==5){
                            int a = 10;
                            //break;
                        }*/
                    }
                }

                result = NULL;
                vez++;


            }

            //printf("%p", wbFile);
            fclose(wbFile);
            fclose(file);


        } else {
            printf("Falha no carregamento do arquivo.");
        }

    } else if (opc == 2) {
        char * nomeArquivo = strtok(NULL, " ");


        //exit(0);
        //char buff2[200];
        char arr;
        int encadeamento;
        int nroInscricao;
        double nota;
        char data[10];
        data[10] = '\0';

        char * cidade;
        char * nomeEscola;
        //char tmp[TAMANHO_CAMPOS_VARIAVEIS];


        FILE *fileWb = fopen(nomeArqWB, "rb");

        if (fileWb != NULL) {

            int vez = 0;
            int erro = 0;
            
            while (!feof(fileWb) && !erro) {
                int pular = TAMANHO_PAGINA + vez * TAMANHO_REGISTRO;
                
                fseek(fileWb, pular , 0);
                
                //while
                fread(&arr, sizeof (arr), 1, fileWb);
                fread(&encadeamento, sizeof (encadeamento), 1, fileWb);
                fread(&nroInscricao, sizeof (nroInscricao), 1, fileWb);
                fread(&nota, sizeof (nota), 1, fileWb);

                fread(&data, sizeof (data), 1, fileWb);

                printf("%d\n%lf\n%s\n", nroInscricao, nota, data);

                vez++;
                
                /*if(vez==5000){
                    int a = 10;
                }*/
            }

            fclose(fileWb);
        } else {
            printf("Falha no processamento do arquivo.");
        }



        //printf("Acabou");
    }


    return (EXIT_SUCCESS);
}

