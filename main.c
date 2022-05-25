#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Agenda.h"
#include <string.h>

const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;

ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *fundo_menu = NULL;
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

char str[17];

void manipular_entrada(ALLEGRO_EVENT evento);
void exibir_texto_centralizado();

int inicializar();
int carregar_arquivos();
void finalizar();

int inserir(Agenda *agenda);

void exibir_contatos(Agenda *agenda);

int remover_contato(Agenda *agenda);

int main(void)
{
    int sair = 0;
    int concluido = 0;

    int controle;
    Agenda agenda;

    iniciar_agenda(&agenda);
    importar_contatos(&agenda);

    if (!inicializar())
    {
        return -1;
    }

    strcpy(str, "");

    if (!carregar_arquivos())
    {
        return -1;
    }

    while (!sair)
    {
        while (!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if (!concluido)
            {
                manipular_entrada(evento);

                 if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    sair = 1;
                }

                if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
                    if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                        sair = 1;
                    }
                    if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
                        concluido = 1;
                    }
                    if(evento.keyboard.keycode == ALLEGRO_KEY_1){
                        al_draw_bitmap(fundo, 0, 0, 0);

                        controle = inserir(&agenda);
                        if (controle == 1)
                            fprintf(stderr,"Contato adicionado com sucesso!");
                        else
                            printf("O contato não foi adicionado, agenda cheia!");
                    }
                    if(evento.keyboard.keycode == ALLEGRO_KEY_2){
                        controle = remover_contato(&agenda);
                        if (controle == 1)
                            printf("Contato removido com sucesso!");
                        else
                            printf("O contato não foi removido, agenda vazia!");

                    }
                    if(evento.keyboard.keycode == ALLEGRO_KEY_3){
                        sair = 1;
                    }
                    if(evento.keyboard.keycode == ALLEGRO_KEY_4){
                        sair = 1;
                    }
                    if(evento.keyboard.keycode == ALLEGRO_KEY_5){
                        exibir_contatos(&agenda);
                    }
                    if(evento.keyboard.keycode == ALLEGRO_KEY_6){
                        sair = 1;
                    }

                }
            }

        }

        al_draw_bitmap(fundo_menu, 0, 0, 0);

        exibir_texto_centralizado();

        al_flip_display();
    }

    finalizar();

    return 0;
}

int inicializar()
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a biblioteca Allegro.\n");
        return 0;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar teclado.\n");
        return 0;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar allegro_image.\n");
        return 0;
    }

    al_init_font_addon();

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar allegro_ttf.\n");
        return 0;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar a janela.\n");
        return 0;
    }

    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        return 0;
    }

    al_set_window_title(janela, "Entrada de Texto");

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    return 1;
}

int carregar_arquivos()
{
    fundo_menu = al_load_bitmap("Agenda_menu.jpg");
    if (!fundo_menu)
    {
        fprintf(stderr, "Falha ao carregar \"bg.jpg\".\n");
        return 0;
    }

    fundo = al_load_bitmap("Agenda_fundo.jpg");
    if (!fundo)
    {
        fprintf(stderr, "Falha ao carregar \"bg.jpg\".\n");
        return 0;
    }

    fonte = al_load_font("arial.ttf", 42, 0);
    if (!fonte)
    {
        fprintf(stderr, "Falha ao carregar \"comic.ttf\".\n");
        return 0;
    }

    return 1;
}
void manipular_entrada(ALLEGRO_EVENT evento)
{
    if (evento.type == ALLEGRO_EVENT_KEY_CHAR)
    {
        if (strlen(str) <= 16)
        {
            char temp[] = {evento.keyboard.unichar, '\0'};
            if (evento.keyboard.unichar == ' ')
            {
                strcat(str, temp);
            }
            else if (evento.keyboard.unichar >= '0' &&
                     evento.keyboard.unichar <= '9')
            {
                strcat(str, temp);
            }
            else if (evento.keyboard.unichar >= 'A' &&
                     evento.keyboard.unichar <= 'Z')
            {
                strcat(str, temp);
            }
            else if (evento.keyboard.unichar >= 'a' &&
                     evento.keyboard.unichar <= 'z')
            {
                strcat(str, temp);
            }
        }

        if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0)
        {
            str[strlen(str) - 1] = '\0';
        }
    }
}

void exibir_texto_centralizado()
{
    if (strlen(str) > 0)
    {
        al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2,
                     (ALTURA_TELA - al_get_font_ascent(fonte)) / 2,
                     ALLEGRO_ALIGN_CENTRE, str);
    }
}

void finalizar()
{
    al_destroy_bitmap(fundo);
    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);
}

int inserir(Agenda *agenda){

    //Retorna 0 pra avisar que não foi possivel inserir o item, ou 1 se foi
    int i;
    Contato novo;
    al_draw_text(fonte, al_map_rgb(255, 255, 255),100, 50, ALLEGRO_ALIGN_CENTER, "Nome:");
    al_flip_display();

    printf("Digite o nome do novo contato: ");
    fflush(stdin);
    fgets(novo.nome,100,stdin);

    novo.nome[strlen(novo.nome)-1]= '\0';//Remove o caractere new line

    for (i = 0; i < strlen(novo.nome);i++)
        novo.nome[i] = toupper(novo.nome[i]);

    printf("Digite o numero do novo contato: ");
    fflush(stdin);
    fgets(novo.fone,20,stdin);
    novo.fone[strlen(novo.fone)-1]= '\0';

    return ins_contato(agenda,novo);
}

void exibir_contatos(Agenda *agenda){

    int i;

    if (agenda->total == 0)
        printf("Agenda vazia!");
    else{
        for (i = 0; i < agenda->total; i++){
            printf("%d  - Nome: %s  |  Fone: %s \n",i+1,agenda->contatos[i].nome,agenda->contatos[i].fone);
        }
    }
}

int remover_contato(Agenda *agenda){

    int i;

    if (agenda->total == 0){
        printf("\t\tAgenda vazia!\n\tNão há contatos para serem removidos!");
        return 0;
    }

    exibir_contatos(agenda);
    printf("Digite o número do contato a ser removido: ");
    scanf(" %d",&i);

    return rmv_contato(agenda,i-1);

}

