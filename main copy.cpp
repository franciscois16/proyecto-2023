#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include <cstdio>
#include<cmath>
#define maxfilas 30
#define maxcolumnas 20
#define paso 1

struct perso
{
    int posx= 0.0;
    int posy= 0.0;
    int direccion = 0;
    
};


void cargarmapaarchivo(struct perso* jugador);
void dibujamapa(ALLEGRO_BITMAP* ladrillo, ALLEGRO_BITMAP* escalera, ALLEGRO_BITMAP* trampabmp);
void dibujarpersonaje(ALLEGRO_BITMAP* personaje,struct perso jugador);
void moverpersonaje(struct perso* jugador);

const int SCREEN_WIDTH = maxfilas*30;
const int SCREEN_HEIGHT = maxcolumnas*30;
const float IMAGE_SPEED = 20.0f;

int fil,col,i,j,z;	

char mapa1[maxcolumnas][maxfilas];
int main()
{
    al_init();
    al_install_keyboard();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);//este es para que decir cada cuanto queremos que se haga una accion = a fps aqui son 30 cada seg
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_FONT *font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    al_init_image_addon();

    ALLEGRO_BITMAP* imagen = al_load_bitmap("imagenes/fondo.bmp");
    ALLEGRO_BITMAP* ladrillo = al_load_bitmap("imagenes/ladrillo.bmp");
    ALLEGRO_BITMAP* escalera = al_load_bitmap("imagenes/escalera.bmp");
    ALLEGRO_BITMAP* trampabmp = al_load_bitmap("imagenes/trampa.bmp");
    ALLEGRO_BITMAP* personaje = al_load_bitmap("imagenes/enemigo.png");


    if (!imagen) {
        fprintf(stderr, "Error al cargar la imagen.\n");
        return -1;
    }

    float posX = SCREEN_WIDTH / 2 - al_get_bitmap_width(imagen) / 2;
    float posY = SCREEN_HEIGHT / 2 - al_get_bitmap_height(imagen) / 2;

    //float velX = IMAGE_SPEED;
    //float velY = IMAGE_SPEED;

    
    struct perso jugador;
    
    
    while (1)
    {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER)
            redraw = true;
        else if ((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE))
            break;

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // posX += velX;
           // posY += velY;

            //if (posX < 0 || posX + al_get_bitmap_width(imagen) > SCREEN_WIDTH)
            //velX = -velX;
            //if (posY < 0 || posY + al_get_bitmap_height(imagen) > SCREEN_HEIGHT)
            //velY = -velY;
            // 
            cargarmapaarchivo(&jugador);
            ALLEGRO_KEYBOARD_STATE keyboard_state;

            do {
                al_get_keyboard_state(&keyboard_state);

                if (al_key_down(&keyboard_state, ALLEGRO_KEY_ESCAPE)) {
                    break;  // Salir del bucle si la tecla 'ESC' está presionada
                }

                al_draw_bitmap(imagen, posX, posY, 0);
                dibujamapa(ladrillo, escalera, trampabmp);
                dibujarpersonaje(personaje,jugador);
                moverpersonaje(&jugador);
                al_flip_display();
            } while (true);


           

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_bitmap(imagen);

    return 0;
}




void cargarmapaarchivo(struct perso* jugador)
{
    ALLEGRO_FILE* mapa = al_fopen("mapa.txt", "r");

    if (mapa == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    char leido;

    for (i = 0; i < maxcolumnas; i++) {
        for (j = 0; j < maxfilas; j++) {
            leido = al_fgetc(mapa);
            mapa1[j][i] = leido;
            if (mapa1[j][i] == 'p')
            {
                jugador->posx = j * 30;
                jugador->posy = i * 30;
            }
        }
        leido = al_fgetc(mapa); // Saltar al siguiente carácter de nueva línea
    }

    al_fclose(mapa);
}


void dibujamapa(ALLEGRO_BITMAP* ladrillo, ALLEGRO_BITMAP* escalera, ALLEGRO_BITMAP* trampabmp)//, ALLEGRO_FONT* font, int vidas, int puntaje
{
    for (int i= 0; i < maxcolumnas; i++) {
        for (int j = 0; j < maxfilas; j++) {
            if (mapa1[j][i] == 'x') {
                al_draw_bitmap(ladrillo, j * 30, i * 30, 0);
            }
            else if (mapa1[j][i] == 'e') {
                al_draw_bitmap(escalera, j * 30, i * 30, 0);
            }
            else if (mapa1[j][i] == 't') {
                al_draw_bitmap(trampabmp, j * 30, i * 30, 0);
            }
            
               
            
        }
    }

   // al_draw_textf(font, al_map_rgb(255, 0, 0), 10, 10, 0, "VIDAS: %d", vidas);
 //   al_draw_textf(font, al_map_rgb(255, 0, 0), 10, 30, 0, "PUNTAJE: %d", puntaje);
}

void dibujarpersonaje(ALLEGRO_BITMAP* personaje, struct perso jugador) {
    float pixelPosX = jugador.posx;
    float pixelPosY = jugador.posy;

    al_draw_bitmap(personaje, pixelPosX, pixelPosY, 0);
}





void moverpersonaje(struct perso* jugador) {
    ALLEGRO_KEYBOARD_STATE keyboard_state;
    al_get_keyboard_state(&keyboard_state);

    float nuevaPosX = jugador->posx;
    float nuevaPosY = jugador->posy;

    if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)) {
        if(mapa1[jugador->posx+30/30][jugador->posy/30]!='x')
         jugador->posx += 1;

        
    }

    if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
        if(mapa1[jugador->posx-30/30][jugador->posy/30]!='x')
         jugador->posx -= 1;
    }

    if (al_key_down(&keyboard_state, ALLEGRO_KEY_UP)) {
        if(mapa1[jugador->posx+60/30][jugador->posy/30]!='x')
         jugador->posy -= 1;
    }

    if (al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN)) {
        jugador->posy += 1;
    }

    // Verificar si hay una 'x' en la nueva posición


}


/*

    if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
        int nuevaPosi = (int)(nuevaPosY / 30);
        int nuevaPosj = (int)(nuevaPosX / 30);

        if (nuevaPosj - 1 >= 0 && mapa1[nuevaPosj - 1][nuevaPosi] == 'x') {
            // No se mueve hacia la izquierda si hay una 'x' delante
        } else {
            nuevaPosX -= 0.5;
        }
    }

    if (al_key_down(&keyboard_state, ALLEGRO_KEY_UP)) {
        int nuevaPosi = (int)(nuevaPosY / 30);
        int nuevaPosj = (int)(nuevaPosX / 30);

        if (nuevaPosi - 1 >= 0 && mapa1[nuevaPosj][nuevaPosi - 1] == 'x') {
            // No se mueve hacia arriba si hay una 'x' delante
        } else {
            nuevaPosY -= 0.5;
        }
    }

    if (al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN)) {
        int nuevaPosi = (int)(nuevaPosY / 30);
        int nuevaPosj = (int)(nuevaPosX / 30);

        if (nuevaPosi + 1 < maxfilas && mapa1[nuevaPosj][nuevaPosi + 1] == 'x') {
            // No se mueve hacia abajo si hay una 'x' delante
        } else {
            nuevaPosY += 0.5;
        }
    }

    // Actualizar la posición del jugador si no hay obstáculos
    jugador->posx = nuevaPosX;
    jugador->posy = nuevaPosY;


*/