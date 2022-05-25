#ifndef AGENDA_H_INCLUDED
#define AGENDA_H_INCLUDED
#define TAM 1000
#include <string.h>

typedef struct{

    char nome[100];
    char fone[20];
    char str[17];
} Contato;

typedef struct{

    Contato contatos[TAM];
    int total;
} Agenda;

void iniciar_agenda(Agenda *a){//Inicia a agenda

    a->total = 0;
}

int ins_contato(Agenda *agenda,Contato novo){

    if (agenda->total == TAM){
        return 0;
    }

    strcpy(agenda->contatos[agenda->total].nome,novo.nome);
    strcpy(agenda->contatos[agenda->total].fone,novo.fone);
    agenda->total++;

    return 1;
}

int rmv_contato(Agenda *agenda, int pos){

    int i;

    for(i = pos; i < agenda->total - 1; i++)
        agenda->contatos[i] = agenda->contatos[i+1];

    agenda->total --;

    return 1;

}

void ordenar(Agenda* agenda){//Falta concertar a função

    int i;
    int j;
    int c = 0;
    Contato temp;
    int cond = 1;

    for (i=agenda->total-1; (i >= 1) && (cond == 1); i--) {

        cond = 0;

        for (j=0; j < i ;j++) {

            if (agenda->contatos[j+1].nome[0] < agenda->contatos[j].nome[0]) {
                temp = agenda->contatos[j];
                agenda->contatos[j] = agenda->contatos[j+1];
                agenda->contatos[j+1] = temp;
                cond = 1;
            } else if(agenda->contatos[j+1].nome[0] == agenda->contatos[j].nome[0]) {
                while (c < strlen(agenda->contatos[j+1].nome)) {
                    if (agenda->contatos[j+1].nome[c] < agenda->contatos[j].nome[c]) {
                        temp = agenda->contatos[j];
                        agenda->contatos[j] = agenda->contatos[j+1];
                        agenda->contatos[j+1] = temp;
                        cond = 1;
                        break;
                    }
                    c++;
                }
            }
        }
    }
}
void importar_contatos(Agenda *agenda){//le os contatos de um txt

    int i;

    FILE *arquivo =  fopen("contatos.txt","r");

    if(!arquivo){
        printf("Não foram encontrados dados para serem carregados.");
    }else{

        fscanf(arquivo,"%d\n",&agenda->total);

        for (i = 0; i < agenda->total; i++){

                fscanf(arquivo,"%s\n",&agenda->contatos[i].nome);

                fscanf(arquivo,"%s\n",&agenda->contatos[i].fone);

        }
    }
    int fclose(FILE *arquivo);
}

void exportar_contatos (Agenda agenda){//salva os contatos em um txt

    int i;

    FILE *arquivo =  fopen("contatos.txt","w+");

    if(!arquivo){
        printf("Não foram encontrados dados para serem carregados.");
        arquivo =  fopen("contatos.txt","w+");
    }

    fprintf(arquivo,"%d\n",agenda.total);

    for (i = 0; i < agenda.total; i++){

            fprintf(arquivo,"%s\n",agenda.contatos[i].nome);

            fprintf(arquivo,"%s\n",agenda.contatos[i].fone);

    }

    int fclose(FILE *arquivo);
}
#endif // AGENDA_H_INCLUDED
