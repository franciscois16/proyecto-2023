    #include <allegro5/allegro5.h>
    #include <allegro5/allegro_font.h>
    #include <allegro5/allegro_image.h>
    #include <allegro5/allegro_audio.h>
    #include <allegro5/allegro_acodec.h>
    #include <stdio.h>
    #include<cmath>
    #define maxfilas 30
    #define maxcolumnas 20
    #define paso 1
    #define maxtiradores 3
    #define maxbalas 5

    struct perso
    {
        int posx= 0.0;
        int posy= 0.0;
        int vidas = 3;
        int estado = 0;
        
    };

    struct dispara
    {

        
            
    };


    struct proyectil {
    int dibposx;
    int dibposy;
    //int direccion;
    int posx[maxbalas];
    int posy[maxbalas];
    int activado[maxbalas];
    };
    


    void cargarmapaarchivo(struct perso* jugador,struct proyectil* proyectil1);
    void dibujamapa(ALLEGRO_BITMAP* ladrillo, ALLEGRO_BITMAP* escalera, ALLEGRO_BITMAP* trampabmp,ALLEGRO_FONT* font);
    void dibujarpersonaje(ALLEGRO_BITMAP* personaje,struct perso jugador);
    void moverpersonaje(struct perso* jugador);
    void acciones(struct perso* jugador,int* inercia,int* agarre);
    int moverCamara(int personaje_y, int camara_y);
    void dibujabala(struct proyectil* proyectil, ALLEGRO_BITMAP* bala_imagen);
    void diparabala(struct proyectil* proyectil, ALLEGRO_BITMAP* bala_imagen);
    void enfriamientobala();

    const int SCREEN_WIDTH = 30*30;
    const int SCREEN_HEIGHT = 20*30;
    const float IMAGE_SPEED = 20.0f;

    int fil,col,inercia = 0;
    int agarre = 0;
    int bandera=50000;
    

    char mapa1[maxcolumnas][maxfilas];
    int main()
    {
        int retardo = 0 , retardo_1 = 0;
        int personaje_y = 0; 
        int camara_y = 0; 

        al_init();
        al_install_audio();
        al_init_acodec_addon();
        al_init_font_addon();
        al_install_keyboard();

        ALLEGRO_TIMER *timer = al_create_timer(1.0 / 1.0);//este es para que decir cada cuanto queremos que se haga una accion = a fps aqui son 30 cada seg
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
        ALLEGRO_BITMAP* trampabmp = al_load_bitmap("imagenes/trampa.png");
        ALLEGRO_BITMAP* personaje = al_load_bitmap("imagenes/enemigo.png");
        ALLEGRO_BITMAP* bala = al_load_bitmap("imagenes/pedribala.jpg");
        //ALLEGRO_SAMPLE *salto = al_load_sample("imagenes/salto.wav");

        al_reserve_samples(1);

        if (!imagen) {
            fprintf(stderr, "Error al cargar la imagen.\n");
            return -1;
        }

        float posX = SCREEN_WIDTH / 2 - al_get_bitmap_width(imagen) / 2;
        float posY = SCREEN_HEIGHT / 2 - al_get_bitmap_height(imagen) / 2;

        //float velX = IMAGE_SPEED;
        //float velY = IMAGE_SPEED;

        
        struct perso jugador;
        //struct dispara balas[3];
        struct proyectil proyectil1[maxtiradores];
        
         
        
        
        
        
        while (jugador.vidas > 1)
        {
            al_start_timer(timer);
            al_wait_for_event(queue, &event);
            printf("%d vidas" , jugador.vidas);
            if (event.type == ALLEGRO_EVENT_TIMER)
                redraw = true;
            else if ((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE))
                break;

            if (redraw && al_is_event_queue_empty(queue))
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));

            
                cargarmapaarchivo(&jugador ,proyectil1);
                ALLEGRO_KEYBOARD_STATE keyboard_state;

                do {
                    al_get_keyboard_state(&keyboard_state);

                    if (al_key_down(&keyboard_state, ALLEGRO_KEY_ESCAPE)) {
                        break;  // Salir del bucle si la tecla 'ESC' está presionada
                    }

                    al_draw_bitmap(imagen, posX, posY, 0);
                    dibujamapa(ladrillo, escalera, trampabmp,font);
                    dibujarpersonaje(personaje,jugador);
                    moverCamara(jugador.posy,camara_y);
                    dibujabala(proyectil1,bala);
                    if (retardo==0)
                    {
                        
                        acciones(&jugador, &inercia,&agarre);
                        moverpersonaje(&jugador);
                        
                       // printf("1");
                    }

                    if(retardo_1==0)
                    {
                        
                        diparabala(proyectil1,bala);
                        //printf("2");
                    }

                    retardo_1=(retardo_1+1)%3;
                    
                    retardo=(retardo+1)%2;
                    al_flip_display(); 
                } while (jugador.vidas > 1);


            

                redraw = false;
            }
        }

        al_destroy_font(font);
        al_destroy_display(disp);
        al_destroy_timer(timer);
        al_destroy_event_queue(queue);
        al_destroy_bitmap(imagen);
        al_destroy_bitmap(ladrillo);
        al_destroy_bitmap(escalera);
        al_destroy_bitmap(trampabmp);
        al_destroy_bitmap(personaje);
    // al_destroy_sample(salto);


        return 0;
    }



void cargarmapaarchivo(struct perso* jugador, struct proyectil* proyectil1) {
    ALLEGRO_FILE* mapa = al_fopen("mapa.txt", "r");
    int i, j, z = 0;

    if (mapa == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    char leido;

    for (i = 0; i < maxcolumnas; i++) {
        for (j = 0; j < maxfilas; j++) {
            leido = al_fgetc(mapa);
            mapa1[j][i] = leido;
            if (mapa1[j][i] == 'p') {
                jugador->posx = j * 30;
                jugador->posy = i * 30;
            }

            if (mapa1[j][i] == 't') {
                proyectil1[z].dibposx = j * 30;
                proyectil1[z].dibposy = i * 30;
                for (int b = 0; b < maxbalas; b++) {
                    proyectil1[z].activado[b] = 1;
                    printf("\nestado:%d",proyectil1[z].activado[b]);
                }
                printf("\nestex : %d", proyectil1[z].dibposx);
                printf("\nestey : %d", proyectil1[z].dibposy);
                printf("\nestez : %d", z);

                z++;
            }
        }
        leido = al_fgetc(mapa); // Saltar al siguiente carácter de nueva línea
    }

    al_fclose(mapa);
}



    void dibujamapa(ALLEGRO_BITMAP* ladrillo, ALLEGRO_BITMAP* escalera, ALLEGRO_BITMAP* trampabmp,ALLEGRO_FONT* font)//, , int vidas, int puntaje
    {
        int i, j;
        for (i= 0; i < maxcolumnas; i++) {
            for ( j = 0; j < maxfilas; j++) {
                if (mapa1[j][i] == 'x') {
                    al_draw_bitmap(ladrillo, j * 30, i * 30, 0);
                }
                else if (mapa1[j][i] == 'e') {
                    al_draw_bitmap(escalera, j * 30, i * 30, 0);
                }
                else if (mapa1[j][i] == 't') {
                    al_draw_bitmap(trampabmp, j * 30, i * 30, 0);
                }
                
            //   al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "¡Hola, mundo!");
                
                
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

            int espera = 0;

            //float nuevaPosX = jugador->posx;
            //float nuevaPosY = jugador->posy;

            if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)) {
            if(mapa1[(jugador->posx+30)/30][((jugador->posy)/30)]!='x' && mapa1[(jugador->posx+30)/30][(jugador->posy+29)/30]!='x'){

                jugador->posx += paso;
                }


            }

            if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
                if(mapa1[(jugador->posx)/30][(jugador->posy)/30]!='x' && mapa1[(jugador->posx)/30][(jugador->posy+29)/30]!='x'){
                jugador->posx -= paso;
                }
            }

            if (al_key_down(&keyboard_state, ALLEGRO_KEY_UP)) {
                if(mapa1[(jugador->posx/30)][((jugador->posy-30)/30)]!='x'&& mapa1[jugador->posx/30][((jugador->posy+30)/30)]=='x'){
                jugador->posy -= paso;
                }
            }

            if (al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN)) {
                if(mapa1[(jugador->posx/30)][((jugador->posy+30)/30)]!='x'&& mapa1[(jugador->posx)/30][((jugador->posy+30)/30)]=='x'){
                jugador->posy += paso;
                }
            }

    
        }


    void acciones(struct perso* jugador, int* inercia, int* agarre) {
        ALLEGRO_KEYBOARD_STATE keyboard_state;
        al_get_keyboard_state(&keyboard_state);
        int direccion = 3;
        

        // Verificar si se presiona la tecla de flecha derecha y si hay una pared en esa dirección
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)) {
            if (mapa1[(jugador->posx+30) / 30][(jugador->posy) / 30] == 'x' && *inercia > 0) {
                *agarre = 1;  // El jugador se agarra a la pared derecha
                printf("%dder", *agarre);
            // mapa1[jugador->posx / 30][(jugador->posy + 60) / 30] = 'x';
            } else {
                *agarre = 0;
            }
        }
        // Verificar si se presiona la tecla de flecha izquierda y si hay una pared en esa dirección
        else if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
            if (mapa1[(jugador->posx -30 ) / 30][(jugador->posy) / 30] == 'x' && *inercia > 0) {
                *agarre = 1;  // El jugador se agarra a la pared izquierda
                printf("%dizq", *agarre);
            //mapa1[(jugador->posx) / 30][(jugador->posy + 60) / 30] = 'x';
            } else {
                *agarre = 0;
            }
        }
        // Si no se presiona ninguna tecla de flecha, no hay agarre
        else {
            *agarre = 0;
        }

        // Movimiento hacia abajo si no hay inercia, agarre o una pared en la parte inferior
        if (mapa1[jugador->posx / 29][(jugador->posy + 30) / 30] == 'o' && *inercia == 0 && *agarre == 0 && mapa1[(jugador->posx+29) / 30][(jugador->posy + 30) / 30] == 'o' && *inercia == 0 && *agarre == 0 ) {
            jugador->posy += paso;
        }


        // Movimiento hacia arriba si hay una escalera y se presiona la tecla de flecha hacia arriba
        if (mapa1[(jugador->posx ) / 30][(jugador->posy + 30) / 30] == 'e' && al_key_down(&keyboard_state, ALLEGRO_KEY_UP) || mapa1[(jugador->posx + 30) / 30][(jugador->posy + 30) / 30] == 'e' && al_key_down(&keyboard_state, ALLEGRO_KEY_UP)) {
            jugador->posy -= paso;
        }

        // Movimiento hacia abajo si hay una escalera y se presiona la tecla de flecha hacia abajo
        if (mapa1[jugador->posx / 30][(jugador->posy+30) / 30] == 'e' && al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN) || mapa1[(jugador->posx + 30) / 30][(jugador->posy) / 30] == 'e' && al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN)) {
            jugador->posy += paso;
        }

        // Movimiento hacia arriba debido a la inercia
        if (*inercia > 0) {
            if (*agarre == 0 && mapa1[(jugador->posx-30) / 30][(jugador->posy) / 30] != 'x' && mapa1[(jugador->posx + 30) / 30][(jugador->posy) / 30] != 'x') {
                jugador->posy -= paso;
                *inercia = *inercia - 1;
                
            } else {
                *inercia = 0;
            }
        }

        // Aplicar inercia hacia arriba si se presiona la tecla de espacio y hay agarre en una pared
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE) && mapa1[jugador->posx / 30][(jugador->posy + 30) / 30] == 'x' && *agarre == 0 || *agarre == 1) {
            *inercia = 400;
        //al_play_sample(salto, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }


        if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT) && mapa1[(jugador->posx+30) / 30][(jugador->posy) / 30] == 'x' && *agarre == 1 && mapa1[(jugador->posx+30) / 30][(jugador->posy-30) / 30] == 'x' ) {
            if (al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE) && direccion !=2)
            {
                if(mapa1[(jugador->posx)/30][(jugador->posy-30)/30]!='x'){
                    jugador->posx-=60;
                    *inercia += 300;
                    direccion = 2; 
                }
                
            }
            
            
        }

        if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT) && mapa1[(jugador->posx-30) / 30][(jugador->posy) / 30] == 'x' && *agarre == 1 ) {
            if (al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE) && direccion != 1)
            {
                if(mapa1[(jugador->posx+1)/30][(jugador->posy-30)/30]!='x'){
                    jugador->posx+=60;
                    *inercia += 300;
                    direccion = 1;
                    printf("%d\n", direccion);
                }
                
            }
            
            
        }


        if(mapa1[jugador->posx/30][(jugador->posy)/30] == 't')
        {
            jugador->vidas = jugador->vidas - 1;
        printf("%d vidas" , jugador->vidas);
            jugador->posx -= 30;
        }

        // al_destroy_sample(salto);
    }


    int moverCamara(int personaje_y, int camara_y) {
        int camara_nueva_y = camara_y;

        if (personaje_y > SCREEN_HEIGHT - 150) {
            camara_nueva_y = personaje_y - (SCREEN_HEIGHT - 150);
        }

        return camara_nueva_y;
    }



    void dibujabala(struct proyectil* proyectil, ALLEGRO_BITMAP* bala_imagen) {
        for (int i = 0; i < maxtiradores; i++) {
            for (int j = 0; j < maxbalas; j++) {
                if (proyectil[i].activado[j] == 0) {
                    al_draw_bitmap(bala_imagen, proyectil[i].posx[j], proyectil[i].posy[j], 0);
                }
            }
        }
    }


    void diparabala(struct proyectil* proyectil, ALLEGRO_BITMAP* bala) {
     int i, j;

        for (i = 0; i < maxtiradores; i++) {
            for (j = 0; j < maxbalas; j++) {
                enfriamientobala();
                if (proyectil[i].activado[j] == 1 ) {
                    
                    proyectil[i].activado[j] = 0;
                    printf("\nprueba:j%d estado%d", j, proyectil[i].activado[j]);
                    proyectil[i].posx[j] = proyectil[i].dibposx;
                    proyectil[i].posy[j] = proyectil[i].dibposy - 30;
                } else {
                    
                    proyectil[i].posy[j] -= 1;
                    printf("\nposicioni:%d,pos%d",j, proyectil[i].posy[j]);

                    // Verificar si hay una 'x' en los próximos 30 píxeles en la dirección de avance
                    if (mapa1[proyectil[i].posx[j] / 30][proyectil[i].posy[j] / 30] == 'x') {
                        proyectil[i].activado[j] = 1;
                    }
                }
            }
        }
    }

    void enfriamientobala() {
        do
        {
            if (bandera>0)
            {
                bandera = bandera -1;
            }
            
        } while (bandera!=0);
        bandera=50000;
        

    }



