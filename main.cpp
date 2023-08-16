    #include <allegro5/allegro5.h>
    #include <allegro5/allegro_font.h>
    #include <allegro5/allegro_image.h>
    #include <allegro5/allegro_audio.h>
    #include <allegro5/allegro_acodec.h>
    #include <allegro5/allegro_native_dialog.h>
    #include <stdio.h>
    #include <stdbool.h>
    #include<cmath>
    #define maxfilas 50
    #define maxcolumnas 30
    #define paso 1
    #define maxtiradores 4
    #define maxbalas 5
    #define maxniveles 4
    #define MAX_JUGADORES 10
    #define MAX_NOMBRE  50
    #define maxsalto  120
    #define maxenemigos 5

    struct ranking {
        char nombres[MAX_JUGADORES][MAX_NOMBRE];
        int puntos[MAX_JUGADORES];
    };


    struct proyectil {
        
        //int direccion;
        int posx; //posicion actual de donde esta la bala
        int posy; //posicion actual de donde esta la bala
        int activado; // estado que determina si la bala se mueve o no 
        int tipo; // determina la direccion de la bala 1 a la derecha 2 a la izquierda 3 arriba y 4 abajo
    };
    
    struct tirador
    {
        
        int dibposx; //posicion x de donde parte la bala
        int dibposy; //posicion y de donde parte la bala
        struct proyectil balas[maxbalas];
    };

    struct perso
    {
        int posx= 0.0;
        int posy= 0.0;
        int estado =1; // 1=quieto,2camina derecha,3camina izquierda,4 salta y 5 se agarra
        int vidas = 5;
        int direccion = 0;
        //int quieto = 0;
        //int camina = 0;
        //int salta = 0;
        //int pared = 0;
        int puntaje=0;
        struct proyectil balasjug[maxbalas];
        
    };
    struct enemigo
    {
        int posx= -100;
        int posy= -100;
        int estado ;
        int tipo = 0; // 1 inteligente 2 torpe
        int direccion=0; // 1 derecha 2 izquierda
      
    };

    

    void cargarmapaarchivo(struct perso* jugador,struct tirador* proyectil1,int* nivelactual,int* lugarportali,int* lugarportalj,struct enemigo* malos);
    void dibujamapa(ALLEGRO_BITMAP* ladrillo, ALLEGRO_BITMAP* escalera, ALLEGRO_BITMAP* trampabmp,ALLEGRO_FONT* font,ALLEGRO_BITMAP* portal,int* p,ALLEGRO_BITMAP* moneda,int* monedacont,ALLEGRO_BITMAP* llave,int* puntuacion,int* retardo_moneda,ALLEGRO_BITMAP* corazon);
    void dibujarpersonaje(ALLEGRO_BITMAP* personaje,struct perso jugador,int* pani,int* pcaminader,ALLEGRO_BITMAP* caminaderecha,int* pcaminaizq,int* retardo_personaje);
    void moverpersonaje(struct perso* jugador,struct tirador* proyectil1,ALLEGRO_SAMPLE_INSTANCE* instanciadaño);
    void acciones(struct perso* jugador,int* inercia,int* agarre,int* nivel,int* lugarportali,int* lugarportalj,int* puntuacion,ALLEGRO_SAMPLE_INSTANCE* instanciasalto,ALLEGRO_SAMPLE_INSTANCE* instanciarecoger,ALLEGRO_SAMPLE_INSTANCE* instanciallave);
    void dibujabala(struct tirador* proyectil1, ALLEGRO_BITMAP* bala_imagen);
    void diparabala(struct tirador* proyectil1, ALLEGRO_BITMAP* bala_imagen);
    bool enfriamientobala();
    void verificanivel(int* nivelactual , int* nivel,struct perso* jugador, struct tirador* proyectil1,int* lugarportali,int* lugarportalj,struct enemigo* malos);
    void menu(int* opcion);
    bool verificarColision(struct perso* jugador, struct tirador* proyectil1,ALLEGRO_SAMPLE_INSTANCE* instanciadaño);
    void ingresarNombre(char* nombre);
    void dibuja_ranking(struct ranking* puntajes,int* opcion);
    void reinicia(struct perso* jugador, int* nivelactual, int* nivel,int* puntuacion);
    void dibuja_datos(ALLEGRO_FONT* font ,int* puntuacion,struct perso* jugador);
    void leerranking(struct ranking* puntajes);
    void ordena_ranking(struct ranking* puntajes);
    void reescribirRanking(struct ranking* puntajes, char* nombre, int puntuacion);
    void dibujamoneda(ALLEGRO_BITMAP* moneda , int* monedacont);
    void dibuja_enemigo(struct enemigo* malos ,ALLEGRO_BITMAP* esqueletomalo , int* retardo_enemigo,int* enemigocont,int* enemigocont2);
    void mover_enemigos(struct enemigo* malos);
    void colision_enemigo(struct enemigo* malos,struct perso* jugador,ALLEGRO_SAMPLE_INSTANCE* instanciadaño);
    void dibuja_bala_jug(struct perso* jugador, ALLEGRO_BITMAP* bala_jugador);
    void dispararBala(struct perso* jugador,ALLEGRO_SAMPLE_INSTANCE* instanciadisparo);
    void inicia_balas_jug(struct perso* jugador);
    void colision_bala_jug(struct perso* jugador);

    const int SCREEN_WIDTH = 50*30;
    const int SCREEN_HEIGHT = 30*30;
    const float IMAGE_SPEED = 20.0f;

    int fil,col,inercia = 0;
    int agarre = 0;
    int bandera=0,bandera_jug=0;

    char mapa1[maxcolumnas][maxfilas];
    int main()
    {
        int retardo = 0 , retardo_1 = 0,retardo_2=0,retardo_moneda=0,retardo_personaje=0,retardo_enemigo=0;;
        int personaje_y = 0; 
        int camara_y = 0; 
        int nivelactual = 1 , nivel = 1;
        int p=0,pani=0,pcaminader=0,pcaminaizq=7,monedacont=0,enemigocont=4,enemigocont2=0;
        int opcion=1;
        char nombre[50];
        //char nombresranking[10][50];
        //int puntos[10];
        int lugarportali=0,lugarportalj=0,puntuacion=0;

        al_init();
        al_install_audio();
        al_init_acodec_addon();
        al_init_font_addon();
        al_install_keyboard();

        ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30);//este es para que decir cada cuanto queremos que se haga una accion = a fps aqui son 30 cada seg
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
        al_reserve_samples(1);

        ALLEGRO_BITMAP* imagen = al_load_bitmap("imagenes/fondo.bmp");
        ALLEGRO_BITMAP* ladrillo = al_load_bitmap("imagenes/ladrillo.bmp");
        ALLEGRO_BITMAP* escalera = al_load_bitmap("imagenes/escalera.png");
        ALLEGRO_BITMAP* trampabmp = al_load_bitmap("imagenes/trampa.png");
        ALLEGRO_BITMAP* bala = al_load_bitmap("imagenes/bala.png");
        ALLEGRO_BITMAP* bala_jugador = al_load_bitmap("imagenes/balajugador.png");
        ALLEGRO_BITMAP* portal = al_load_bitmap("imagenespersonaje/portal.png");
        //ALLEGRO_SAMPLE *salto = al_load_sample("imagenes/salto.wav");
        ALLEGRO_BITMAP* personajequieto = al_load_bitmap("imagenespersonaje/pquieto.png");
        ALLEGRO_BITMAP* caminaderecha = al_load_bitmap("imagenespersonaje/pcaminaderecha.png");
        ALLEGRO_BITMAP* moneda = al_load_bitmap("imagenes/moneda.png");
        ALLEGRO_BITMAP* llave = al_load_bitmap("imagenes/llave.png");
        ALLEGRO_BITMAP* maloesqueleto = al_load_bitmap("imagenes/enemigo2.png");
        ALLEGRO_BITMAP* corazon = al_load_bitmap("imagenes/corazon.png");
        ALLEGRO_SAMPLE* temamenu = al_load_sample("musica/Phantom.mp3");
        ALLEGRO_SAMPLE* nivel1 = al_load_sample("musica/nivel1.mp3");
        
        


        ALLEGRO_SAMPLE* salto = al_load_sample("musica/salta.mp3");
        ALLEGRO_SAMPLE* disparo = al_load_sample("musica/disparo.mp3");
        ALLEGRO_SAMPLE* recoger = al_load_sample("musica/efectomoneda.mp3");
        ALLEGRO_SAMPLE* daño = al_load_sample("musica/daño.mp3");
        ALLEGRO_SAMPLE* recogellave = al_load_sample("musica/agarrallave.wav");

        ALLEGRO_SAMPLE_INSTANCE* instanciamenu = al_create_sample_instance(temamenu);
        ALLEGRO_SAMPLE_INSTANCE* instancianivel1 = al_create_sample_instance(nivel1);
        ALLEGRO_SAMPLE_INSTANCE* instanciasalto = al_create_sample_instance(salto);
        ALLEGRO_SAMPLE_INSTANCE* instanciadisparo = al_create_sample_instance(disparo);
        ALLEGRO_SAMPLE_INSTANCE* instanciarecoger = al_create_sample_instance(recoger);
        ALLEGRO_SAMPLE_INSTANCE* instanciadaño = al_create_sample_instance(daño);
        ALLEGRO_SAMPLE_INSTANCE* instanciallave = al_create_sample_instance(recogellave);


        al_attach_sample_instance_to_mixer(instanciamenu,al_get_default_mixer());
        al_attach_sample_instance_to_mixer(instancianivel1,al_get_default_mixer());
        al_attach_sample_instance_to_mixer(instanciasalto,al_get_default_mixer());
        al_attach_sample_instance_to_mixer(instanciadisparo,al_get_default_mixer());
        al_attach_sample_instance_to_mixer(instanciarecoger,al_get_default_mixer());
        al_attach_sample_instance_to_mixer(instanciadaño,al_get_default_mixer());
        al_attach_sample_instance_to_mixer(instanciallave,al_get_default_mixer());
        
        al_set_sample_instance_gain(instanciamenu,0.5);
        al_set_sample_instance_gain(instancianivel1,0.5);
        al_set_sample_instance_gain(instanciasalto, 0.5); // Ajustar el volumen entre 0.0 y 1.0
        al_set_sample_instance_gain(instanciadisparo, 1.0); // Ajustar el volumen entre 0.0 y 1.0
        al_set_sample_instance_gain(instanciarecoger, 2.0); // Ajustar el volumen entre 0.0 y 1.0

        al_set_sample_instance_playmode(instanciamenu,ALLEGRO_PLAYMODE_LOOP);
        al_set_sample_instance_playmode(instancianivel1,ALLEGRO_PLAYMODE_LOOP);


        

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
        struct tirador proyectil1[maxtiradores];

        struct ranking puntajes;

        struct enemigo malos[maxenemigos];
        
        leerranking(&puntajes);
        ordena_ranking(&puntajes);
        
        while (opcion!=4)
        {
            al_start_timer(timer);
            al_wait_for_event(queue, &event);
            al_clear_to_color(al_map_rgb(0, 0, 0));
           
            //printf("\n%d vidas" , jugador.vidas);
            if (event.type == ALLEGRO_EVENT_TIMER)
                redraw = true;
            //else if ((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE))
                //break;
            if (redraw && al_is_event_queue_empty(queue))
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));

                ALLEGRO_KEYBOARD_STATE keyboard_state;
                if (opcion == 1) {
                    //al_play_sample(temamenu, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                    al_play_sample_instance(instanciamenu);
                    menu(&opcion);
                    ordena_ranking(&puntajes);
                    reescribirRanking(&puntajes, nombre, puntuacion);
                    
                    
                }

                if (opcion==3)
                {
                    dibuja_ranking(&puntajes,&opcion);
                }
                printf("\nopcion despues de salir renking %d",opcion);
                    
                if (opcion==2)
                {
                    ingresarNombre(nombre);
                    al_stop_sample_instance(instanciamenu);
                    cargarmapaarchivo(&jugador ,proyectil1,&nivelactual,&lugarportali,&lugarportalj,malos);
                    inicia_balas_jug(&jugador);
                   do {
                        
                            al_play_sample_instance(instancianivel1);
                        
                        
                        
                        al_get_keyboard_state(&keyboard_state);

                        if (al_key_down(&keyboard_state, ALLEGRO_KEY_ESCAPE)) {
                            jugador.vidas=0;
                            puntuacion += 12000;  // Salir del bucle si la tecla 'ESC' está presionada
                        }

                        al_draw_bitmap(imagen, 0, 0, 0);
                        verificanivel(&nivelactual, &nivel, &jugador, proyectil1,&lugarportali,&lugarportalj,malos);

                        dibujamapa(ladrillo, escalera, trampabmp,font,portal,&p,moneda,&monedacont,llave,&puntuacion,&retardo_moneda,corazon);
                        //if (cont==6)
                           /*dibujamoneda; cont=0*/


                        dibujarpersonaje(personajequieto,jugador,&pani,&pcaminader,caminaderecha,&pcaminaizq,&retardo_personaje);
                        //moverCamara(jugador.posy,camara_y);
                        dibujabala(proyectil1,bala);
                        dibuja_enemigo(malos, maloesqueleto, &retardo_enemigo,&enemigocont,&enemigocont2);
                        mover_enemigos(malos);
                        colision_enemigo(malos,&jugador,instanciadaño);
                        dispararBala(&jugador,instanciadisparo);
                        dibuja_bala_jug(&jugador,bala_jugador);
                        colision_bala_jug(&jugador);
                        
                        
                        
                        if (retardo==0)
                        {   
                            acciones(&jugador, &inercia,&agarre,&nivel,&lugarportali,&lugarportalj,&puntuacion,instanciasalto,instanciarecoger,instanciallave);
                            moverpersonaje(&jugador,proyectil1,instanciadaño);
                        // printf("1");
                        }

                        if(retardo_1==0)
                        {
                            diparabala(proyectil1,bala);
                            //dibuja_puntos(font,&puntuacion);
                            //printf("2");
                        }

                        if(retardo_2==0)
                        {
                            dibuja_datos(font,&puntuacion,&jugador);
                            //printf("2");
                        }

                        //retardo_1=(retardo_1+1)%2;
                        //retardo_2=(retardo_2+1)%6;
                        //retardo=(retardo+1)%2;
                        //al_rest(0.01);
                        
                        al_flip_display();

                     
                        
                       
                    } while (jugador.vidas > 0 && maxniveles>nivel); 
                        //  for (int i = 0; i < MAX_JUGADORES; i++) {
                        //      printf("entes de reescribir %s %d\n", puntajes.nombres[i], puntajes.puntos[i]);
                        //  }
                    //reescribirRanking(&puntajes, nombre, puntuacion);
                    printf("rankinh0 %s %d",puntajes.nombres[0],puntajes.puntos[0]);
                    printf("\nrankinh2 %s %d",puntajes.nombres[1],puntajes.puntos[1]);
                    reinicia(&jugador, &nivelactual, &nivel,&puntuacion);
                    opcion = 1;
                    printf("\nopcion despues de salir juego %d\n",opcion);
                    
                }
                    
                    al_stop_sample_instance(instancianivel1);
                redraw = false;
            }
        }

        al_destroy_bitmap(imagen);
        al_destroy_bitmap(ladrillo);
        al_destroy_bitmap(escalera);
        al_destroy_bitmap(trampabmp);
        al_destroy_bitmap(personajequieto);
        al_destroy_bitmap(bala);
        al_destroy_bitmap(portal);
        al_destroy_font(font);
        al_destroy_display(disp);
        al_destroy_timer(timer);
        al_destroy_event_queue(queue);
        al_destroy_sample_instance(instanciamenu);
        al_destroy_sample_instance(instancianivel1);
        al_destroy_sample_instance(instanciasalto);
        al_destroy_sample_instance(instanciadisparo);
        al_destroy_sample_instance(instanciarecoger);
        al_destroy_sample_instance(instanciadaño);
        al_destroy_sample_instance(instanciallave);

        al_uninstall_keyboard();
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_destroy_bitmap(bala_jugador);
        al_destroy_bitmap(maloesqueleto);
        al_destroy_bitmap(llave);
        al_destroy_bitmap(moneda);
        al_destroy_bitmap(caminaderecha);
        al_destroy_bitmap(personajequieto);
        al_destroy_bitmap(portal);
        al_destroy_bitmap(trampabmp);
        al_destroy_bitmap(escalera);
        al_destroy_bitmap(ladrillo);
        al_destroy_bitmap(imagen);
        // al_destroy_sample(salto);


        return 0;
    }



void menu(int* opcion) {
    ALLEGRO_KEYBOARD_STATE keyboard_state;
    ALLEGRO_BITMAP* menu = al_load_bitmap("imagenes/menu2.png");

    if (!menu) {
        fprintf(stderr, "Error al cargar la imagen del menú.\n");
        return;
    }

    while (*opcion == 1) {
        al_get_keyboard_state(&keyboard_state);
        al_draw_bitmap(menu, 0, 0, 0);
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_1)) {
            *opcion = 1;
        } else if (al_key_down(&keyboard_state, ALLEGRO_KEY_2)) {
            *opcion = 2;
        } else if (al_key_down(&keyboard_state, ALLEGRO_KEY_3)) {
            *opcion = 3;
        } else if (al_key_down(&keyboard_state, ALLEGRO_KEY_4)) {
            *opcion = 4;
        }
        al_flip_display(); 
    }

    
    al_destroy_bitmap(menu);
    // Aquí puedes usar el valor de "*opcion" para realizar acciones específicas según la tecla presionada
}

void reinicia(struct perso* jugador, int* nivelactual, int* nivel,int* puntuacion) {
    jugador->vidas = 5;
    *nivelactual = 1;
    *nivel = 1;
    *puntuacion = 0;
    // Add any other necessary state reset here
}

void cargarmapaarchivo(struct perso* jugador, struct tirador* proyectil1,int* nivelactual,int* lugarportali,int* lugarportalj,struct enemigo* malos) {
    ALLEGRO_FILE* mapa;
    
    if(*nivelactual==1){
        mapa = al_fopen("mapa.txt", "r");
    }

    if(*nivelactual==2){
        mapa = al_fopen("mapa2.txt", "r");
    }

    if (*nivelactual==3)
    {
        mapa = al_fopen("mapa3.txt", "r");
    }
    

    int i, j, z = 0,y=0;

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
                    proyectil1[z].balas[b].activado = 1;
                    proyectil1[z].balas[b].tipo = 3; // determina la direccion de la bala 1 a la derecha 2 a la izquierda 3 arriba y 4 abajo 
                    //printf("\nestado:%d",proyectil1[z].balas[b].activado);
                    //printf("\ntipo bala: %d", proyectil1[z].balas[b].tipo);
                }
                //printf("\nestex : %d", proyectil1[z].dibposx);
                //printf("\nestey : %d", proyectil1[z].dibposy);
                //printf("\nestez : %d", z);

                z++;
            }

            if (mapa1[j][i] == 'z') {
                malos[y].estado=0;
                malos[y].posx=j*30;
                malos[y].posy=i*30;
                malos[y].tipo=1;
                malos[y].direccion=1;
                

                y++;
            }

            else if(mapa1[j][i] == 'u') {
                proyectil1[z].dibposx = j * 30;
                proyectil1[z].dibposy = i * 30;
                for (int b = 0; b < maxbalas; b++) {
                    proyectil1[z].balas[b].activado = 1;
                    proyectil1[z].balas[b].tipo = 4; // determina la direccion de la bala 1 a la derecha 2 a la izquierda 3 arriba y 4 abajo 
                    // printf("\nestado:%d",proyectil1[z].balas[b].activado);
                    // printf("\ntipo bala: %d", proyectil1[z].balas[b].tipo);
                }
                // printf("\nestex : %d", proyectil1[z].dibposx);
                // printf("\nestey : %d", proyectil1[z].dibposy);
                
                // printf("\nestez : %d", z);

                z++;
            }

            else if(mapa1[j][i] == 'y') {
                proyectil1[z].dibposx = j * 30;
                proyectil1[z].dibposy = i * 30;
                for (int b = 0; b < maxbalas; b++) {
                    proyectil1[z].balas[b].activado = 1;
                    proyectil1[z].balas[b].tipo = 2; // determina la direccion de la bala 1 a la derecha 2 a la izquierda 3 arriba y 4 abajo 
                //     printf("\nestado:%d",proyectil1[z].balas[b].activado);
                //     printf("\ntipo bala: %d", proyectil1[z].balas[b].tipo);
                 }
                // printf("\nestex : %d", proyectil1[z].dibposx);
                // printf("\nestey : %d", proyectil1[z].dibposy);
                
                // printf("\nestez : %d", z);

                z++;
            }

            else if(mapa1[j][i] == 'i') {
                proyectil1[z].dibposx = j * 30;
                proyectil1[z].dibposy = i * 30;
                for (int b = 0; b < maxbalas; b++) {
                    proyectil1[z].balas[b].activado = 1;
                    proyectil1[z].balas[b].tipo = 1; // determina la direccion de la bala 1 a la derecha 2 a la izquierda 3 arriba y 4 abajo 
                //     printf("\nestado:%d",proyectil1[z].balas[b].activado);
                //     printf("\ntipo bala: %d", proyectil1[z].balas[b].tipo);
                }
                // printf("\nestex : %d", proyectil1[z].dibposx);
                // printf("\nestey : %d", proyectil1[z].dibposy);
                
                // printf("\nestez : %d", z);

                z++;
            }

            if(mapa1[j][i]=='n'){

                *lugarportali=i;
                *lugarportalj=j;
                // printf("\nestei : %d", *lugarportali);
                // printf("\nestej : %d", *lugarportalj);
                mapa1[j][i]='o';

            }
        }

        
        leido = al_fgetc(mapa); // Saltar al siguiente carácter de nueva línea
    }

    al_fclose(mapa);
}



    void dibujamapa(ALLEGRO_BITMAP* ladrillo, ALLEGRO_BITMAP* escalera, ALLEGRO_BITMAP* trampabmp,ALLEGRO_FONT* font,ALLEGRO_BITMAP* portal,int* p,ALLEGRO_BITMAP* moneda,int* monedacont,ALLEGRO_BITMAP* llave,int* puntuacion,int* retardo_moneda,ALLEGRO_BITMAP* corazon)//, , int vidas, int puntaje
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
                    al_draw_bitmap_region(trampabmp,0,0,30,30,j*30,i*30,0);
                }
                else if (mapa1[j][i] == 'u') {
                    al_draw_bitmap_region(trampabmp,1*30,0,30,30,j*30,i*30,0);
                }

                else if (mapa1[j][i] == 'y') {
                    al_draw_bitmap_region(trampabmp,2*30,0,30,30,j*30,i*30,0);
                }

                else if (mapa1[j][i] == 'i') {
                    al_draw_bitmap_region(trampabmp,3*30,0,30,30,j*30,i*30,0);
                }

                else if (mapa1[j][i]=='c')
                {
                    al_draw_bitmap(corazon,j*30,i*30,0);
                }
                


                else if (mapa1[j][i] == 'n') {
                    al_draw_bitmap_region(portal,(*p)*30,0,30,30,j*30,i*30,0);
                    (*p)++;
                    if (*p>6)
                    {
                       *p=0;
                    }
                    
                }
                else if (mapa1[j][i] == 'm') {
                    al_draw_bitmap_region(moneda,(*monedacont)*30,0,30,30,j*30,i*30,0);
                    if(*retardo_moneda == 100){
                        
                        (*monedacont)++;
                        if (*monedacont>8)
                        {
                        *monedacont=0;
                        }
                        *retardo_moneda=0;
                    }

                    (*retardo_moneda)++;
                    
                }

                else if (mapa1[j][i] == 'l') {
                    al_draw_bitmap(llave, j * 30, i * 30, 0);
                }

            }
        }

    // al_draw_textf(font, al_map_rgb(255, 0, 0), 10, 10, 0, "VIDAS: %d", vidas);
    //   al_draw_textf(font, al_map_rgb(255, 0, 0), 10, 30, 0, "PUNTAJE: %d", puntaje);
    }


    void dibuja_datos(ALLEGRO_FONT* font ,int* puntuacion,struct perso* jugador){

        al_draw_textf(font, al_map_rgb(0, 0, 0), 15, 5, 0, "puntuacion : %d", *puntuacion);
        al_draw_textf(font, al_map_rgb(0, 0, 0), 15, 15, 0, "vidas : %d", jugador->vidas);
    }

    void dibujarpersonaje(ALLEGRO_BITMAP* personajequieto, struct perso jugador,int* pani,int* pcaminader,ALLEGRO_BITMAP* caminaderecha,int* pcaminaizq,int* retardo_personaje) {

        if(jugador.estado==1){
            al_draw_bitmap_region(personajequieto,(*pani)*30,0,30,30,jugador.posx,jugador.posy,0);
            if(*retardo_personaje==9){
                (*pani)++;
                if (*pani>3)
                {
                    *pani=0;
                }
                *retardo_personaje=0;
            }
            (*retardo_personaje)++;
           
        }

        else if(jugador.estado==2){
            al_draw_bitmap_region(caminaderecha,(*pcaminader)*30,0,30,30,jugador.posx,jugador.posy,0);
            if(*retardo_personaje==9){
                (*pcaminader)++;
                //printf("\ncamina%d",*pcaminader);
                if (*pcaminader>5)
                {
                    *pcaminader=0;
                }
                *retardo_personaje=0;
            }
            (*retardo_personaje)++;
        }

        else if(jugador.estado==3){
            al_draw_bitmap_region(caminaderecha,(*pcaminaizq)*30,0,30,30,jugador.posx,jugador.posy,0);
            if(*retardo_personaje==9){
                (*pcaminaizq)++;
            // printf("\nizquierda%d",*pcaminaizq);
                if (*pcaminaizq>11)
                {
                    *pcaminaizq=7;
                }
                *retardo_personaje=0;
            }
            (*retardo_personaje)++;
        }


        //al_draw_bitmap(personajequieto, pixelPosX, pixelPosY, 0);
    }

    void moverpersonaje(struct perso* jugador,struct tirador* proyectil1,ALLEGRO_SAMPLE_INSTANCE* instanciadaño) {
            ALLEGRO_KEYBOARD_STATE keyboard_state;
            al_get_keyboard_state(&keyboard_state);

            int espera = 0;

            //float nuevaPosX = jugador->posx;
            //float nuevaPosY = jugador->posy;

            if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)) {
            if(mapa1[(jugador->posx+30)/30][((jugador->posy)/30)]!='x' && mapa1[(jugador->posx+30)/30][(jugador->posy+29)/30]!='x'){

                jugador->posx += paso;
                jugador->estado = 2;
                }


            }

            if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
                if(mapa1[(jugador->posx)/30][(jugador->posy)/30]!='x' && mapa1[(jugador->posx)/30][(jugador->posy+29)/30]!='x'){
                jugador->posx -= paso;
                jugador->estado=3;
                jugador->direccion=1;
                }
            }
            bool ninguna_tecla_presionada = true;

            for (int tecla = 0; tecla < ALLEGRO_KEY_MAX; tecla++) {
                if (al_key_down(&keyboard_state, tecla)) {
                    ninguna_tecla_presionada = false;
                    break;
                }
            }

            if (ninguna_tecla_presionada) {
               jugador->estado=1;
            }

            if (verificarColision(jugador, proyectil1,instanciadaño)) {
                // Si hay colisión, realiza alguna acción (por ejemplo, mostrar un mensaje)
                printf("¡Has sido alcanzado por una bala! vidas %d\n",jugador->vidas);
            }

    
        }


    void acciones(struct perso* jugador, int* inercia, int* agarre , int* nivel,int* lugarportali,int* lugarportalj,int* puntuacion,ALLEGRO_SAMPLE_INSTANCE* instanciasalto,ALLEGRO_SAMPLE_INSTANCE* instanciarecoger,ALLEGRO_SAMPLE_INSTANCE* instanciallave) {
        ALLEGRO_KEYBOARD_STATE keyboard_state;
        al_get_keyboard_state(&keyboard_state);
        int direccion = 3;
        

        // Verificar si se presiona la tecla de flecha derecha y si hay una pared en esa dirección
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)) {
            if (mapa1[(jugador->posx+30) / 30][(jugador->posy) / 30] == 'x'&& mapa1[(jugador->posx+30) / 30][(jugador->posy+30) / 30] == 'x' && *inercia > 0) {
                *agarre = 1;  // El jugador se agarra a la pared derecha
                printf("%dder", *agarre);
                jugador->direccion=2;
            // mapa1[jugador->posx / 30][(jugador->posy + 60) / 30] = 'x';
            } else {
                *agarre = 0;
            }
        }
        // Verificar si se presiona la tecla de flecha izquierda y si hay una pared en esa dirección
        else if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
            if (mapa1[(jugador->posx - 30 ) / 30][(jugador->posy) / 30] == 'x' &&mapa1[(jugador->posx - 30 ) / 30][(jugador->posy+30) / 30] == 'x'&& *inercia > 0) {
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

        if (mapa1[(jugador->posx+1)/30][(jugador->posy+1)/30]!='x'&& mapa1[(jugador->posx + 29) / 30][(jugador->posy+1) / 30] != 'x' && mapa1[(jugador->posx+1) / 30][(jugador->posy + 29) / 30] != 'x' && mapa1[(jugador->posx + 29) / 30][(jugador->posy + 30) / 30] != 'x'&& mapa1[jugador->posx / 30][(jugador->posy + 30) / 30] != 'e' &&mapa1[(jugador->posx+30) / 30][(jugador->posy + 30) / 30] != 'e'&&*inercia == 0 &&*agarre == 0)
        {
            jugador->posy += paso;
        }

        // Movimiento hacia arriba si hay una escalera y se presiona la tecla de flecha hacia arriba
         if (mapa1[jugador->posx / 30][(jugador->posy+30) / 30] == 'e'  && al_key_down(&keyboard_state, ALLEGRO_KEY_UP)|| mapa1[(jugador->posx+1) / 30][(jugador->posy) / 30] == 'e'  && al_key_down(&keyboard_state, ALLEGRO_KEY_UP)) {
            jugador->posy -= paso;
        }

        // Movimiento hacia abajo si hay una escalera y se presiona la tecla de flecha hacia abajo
        if (mapa1[jugador->posx / 30][(jugador->posy+30) / 30] == 'e' && al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN)  ) {
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
            *inercia = maxsalto;
            al_play_sample_instance(instanciasalto);
        //al_play_sample(salto, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }


        if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT) && mapa1[(jugador->posx+30) / 30][(jugador->posy) / 30] == 'x' && *agarre == 1 && mapa1[(jugador->posx+30) / 30][(jugador->posy-30) / 30] == 'x' ) {
            if (al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE) && direccion !=2)
            {
                if(mapa1[(jugador->posx)/30][(jugador->posy-30)/30]!='x'){
                    jugador->posx-=60;
                    *inercia = maxsalto;
                    direccion = 2; 
                }   
            }
        }

        if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT) && mapa1[(jugador->posx-30) / 30][(jugador->posy) / 30] == 'x' && *agarre == 1 ) {
            if (al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE) && direccion != 1)
            {
                if(mapa1[(jugador->posx+1)/30][(jugador->posy-30)/30]!='x'){
                    jugador->posx+=60;
                    *inercia = maxsalto;
                    direccion = 1;
                    printf("%d\n", direccion);
                }   
            }
        }

        if (mapa1[(jugador->posx / 30)][((jugador->posy) / 30)] == 'l'){
            mapa1[(jugador->posx / 30)][((jugador->posy) / 30)] ='o'; 
            al_play_sample_instance(instanciallave);
            mapa1[*lugarportalj][*lugarportali]='n';
            //printf("portalj%d,portali%d",*lugarportalj,*lugarportali);
        }
        
        if(mapa1[jugador->posx/30][(jugador->posy)/30] == 'n')
        {
            (*nivel)++;
        }

        if (mapa1[(jugador->posx / 30)][((jugador->posy) / 30)] == 'm'){
            al_play_sample_instance(instanciarecoger);
            mapa1[(jugador->posx / 30)][((jugador->posy) / 30)]='o'; 
            *puntuacion = *puntuacion + (jugador->vidas*50);
        }

        if (mapa1[(jugador->posx / 30)][((jugador->posy) / 30)] == 'c'){
            al_play_sample_instance(instanciarecoger);
            mapa1[(jugador->posx / 30)][((jugador->posy) / 30)]='o'; 
            jugador->vidas += 1;
        }
        

        // al_destroy_sample(salto);
    }

    void dibujabala(struct tirador* proyectil1, ALLEGRO_BITMAP* bala_imagen) {
    for (int i = 0; i < maxtiradores; i++) {
        for (int j = 0; j < maxbalas; j++) {
            if (proyectil1[i].balas[j].activado == 0) {
                al_draw_bitmap(bala_imagen, proyectil1[i].balas[j].posx, proyectil1[i].balas[j].posy, 0);
            }
        }
    }
}



    void diparabala(struct tirador* proyectil1, ALLEGRO_BITMAP* bala) {
     int i, j;

        for (i = 0; i < maxtiradores; i++) {
            for (j = 0; j < maxbalas; j++) {
                
                if (proyectil1[i].balas[j].activado == 1) {
                     if(enfriamientobala() == true){
                    proyectil1[i].balas[j].activado = 0;
                    //printf("\nprueba:j%d estado%d", j, proyectil[i].activado[j]);
                     // determina la direccion de la bala 1 a la derecha 2 a la izquierda 3 arriba y 4 abajo 
                    if (proyectil1[i].balas[j].tipo == 3)
                    {
                        proyectil1[i].balas[j].posx = proyectil1[i].dibposx;
                        proyectil1[i].balas[j].posy = proyectil1[i].dibposy - 30;
                    }

                    if (proyectil1[i].balas[j].tipo == 4)
                    {
                        proyectil1[i].balas[j].posx = proyectil1[i].dibposx;
                        proyectil1[i].balas[j].posy = proyectil1[i].dibposy + 30;
                    }

                    if (proyectil1[i].balas[j].tipo == 1)
                    {
                        proyectil1[i].balas[j].posx = proyectil1[i].dibposx+30;
                        proyectil1[i].balas[j].posy = proyectil1[i].dibposy ;
                    }

                    if (proyectil1[i].balas[j].tipo == 2)
                    {
                        proyectil1[i].balas[j].posx = proyectil1[i].dibposx-30;
                        proyectil1[i].balas[j].posy = proyectil1[i].dibposy;
                    }
                    
                    
                    }
                } else {
                    // determina la direccion de la bala 1 a la derecha 2 a la izquierda 3 arriba y 4 abajo 
                    if (proyectil1[i].balas[j].tipo == 3)
                    {
                        proyectil1[i].balas[j].posy -= 1;
                    }

                    if (proyectil1[i].balas[j].tipo == 4)
                    {
                        proyectil1[i].balas[j].posy += 1;
                    }
                    if (proyectil1[i].balas[j].tipo == 1)
                    {
                        proyectil1[i].balas[j].posx += 1;
                    }

                    if (proyectil1[i].balas[j].tipo == 2)
                    {
                        proyectil1[i].balas[j].posx -= 1;
                    }
                    //printf("\nposicioni:%d,pos%d",j, proyectil[i].posy[j]);

                    // Verificar si hay una 'x' en los próximos 30 píxeles en la dirección de avance
                    if (mapa1[proyectil1[i].balas[j].posx / 30][proyectil1[i].balas[j].posy / 30] == 'x') {
                        proyectil1[i].balas[j].activado = 1;
                    }
                }
            }
        }
    }

    bool enfriamientobala() {
            if (bandera>0)
            {
               bandera = bandera - 4; 
            }

           // printf("\nbandera %d ",bandera);
            
            if (bandera == 0)
            {
               bandera=4000; 
               return true;
            }
            else{
             return false;
            }
    }

    void verificanivel(int* nivelactual , int* nivel,struct perso* jugador, struct tirador* proyectil1,int* lugarportali,int* lugarportalj,struct enemigo* malos){
        if(*nivelactual != *nivel){
            *nivelactual=*nivel;
            cargarmapaarchivo(jugador ,proyectil1,nivelactual,lugarportali, lugarportalj,malos);
        }
    }

    bool verificarColision(struct perso* jugador, struct tirador* proyectil1,ALLEGRO_SAMPLE_INSTANCE* instanciadaño) {
        // Definimos el tamaño de la zona de colisión adicional (en píxeles)
        int colisionAdicional = 4;
        // Iteramos sobre las balas y comprobamos si alguna colisiona con el jugador
        for (int i = 0; i < maxtiradores; i++) {
            for (int j = 0; j < maxbalas; j++) {
                if (proyectil1[i].balas[j].activado == 0) {
                    // Coordenadas del rectángulo que representa a la bala
                    

                    // Comprobamos si hay intersección entre los rectángulos (colisión)
                    if (jugador->posx == proyectil1[i].balas[j].posx && jugador->posy == proyectil1[i].balas[j].posy || jugador->posx + 30 == proyectil1[i].balas[j].posx+15 && jugador->posy == proyectil1[i].balas[j].posy) {
                        jugador->vidas--;
                        al_play_sample_instance(instanciadaño);
                        proyectil1[i].balas[j].activado=1;
                        return true; // Colisión detectada
                    }
                }
            }
        }

        return false; // No hay colisión
    }

    void ingresarNombre(char* nombre) {
        ALLEGRO_FONT* font = al_create_builtin_font();
        ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);
        ALLEGRO_COLOR bgColor = al_map_rgba(0, 0, 0, 200);
        const int textWidth = 300;
        const int textHeight = 50;

        ALLEGRO_BITMAP* textBitmap = al_create_bitmap(textWidth, textHeight);
        al_set_target_bitmap(textBitmap);
        al_clear_to_color(bgColor);
        al_draw_text(font, color, textWidth / 2, textHeight / 2 - al_get_font_ascent(font) / 2, ALLEGRO_ALIGN_CENTER, "Ingresa tu nombre:");
        al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
        al_draw_bitmap(textBitmap, al_get_display_width(al_get_current_display()) / 2 - textWidth / 2, al_get_display_height(al_get_current_display()) / 2 - textHeight / 2, 0);
        al_flip_display();

        bool done = false;
        ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
        al_register_event_source(event_queue, al_get_keyboard_event_source());

        char inputText[50] = { 0 };
        int currentIndex = 0;

        while (!done) {
            ALLEGRO_EVENT ev;
            al_wait_for_event(event_queue, &ev);

            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    strcpy(nombre, inputText);
                    done = true;
                } else if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                    if (currentIndex > 0) {
                        inputText[currentIndex - 1] = '\0';
                        currentIndex--;
                    }
                } else if (ev.keyboard.keycode >= ALLEGRO_KEY_A && ev.keyboard.keycode <= ALLEGRO_KEY_Z && currentIndex < sizeof(inputText) - 1) {
                    char ch = 'a' + ev.keyboard.keycode - ALLEGRO_KEY_A;
                    inputText[currentIndex] = ch;
                    currentIndex++;
                }

                al_set_target_bitmap(textBitmap);
                al_clear_to_color(bgColor);
                al_draw_text(font, color, textWidth / 2, textHeight / 2 - al_get_font_ascent(font) / 2, ALLEGRO_ALIGN_CENTER, inputText);
                al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
                al_draw_bitmap(textBitmap, al_get_display_width(al_get_current_display()) / 2 - textWidth / 2, al_get_display_height(al_get_current_display()) / 2 - textHeight / 2, 0);
                al_flip_display();
            }
        }

        al_destroy_bitmap(textBitmap);
        al_destroy_event_queue(event_queue);
        al_destroy_font(font);
    }


    void leerranking(struct ranking* puntajes) {
        FILE *archivo = fopen("ranking.txt", "r");
        if (archivo == NULL) {
            printf("Error al abrir el archivo de ranking.\n");
            return;
        }

        int i = 0;
        char nombre[MAX_NOMBRE];
        int puntaje;

        while (i < MAX_JUGADORES && fscanf(archivo, "%s %d", nombre, &puntaje) == 2) {
            strcpy(puntajes->nombres[i], nombre);
            puntajes->puntos[i] = puntaje;
            printf("%s %d\n", puntajes->nombres[i], puntajes->puntos[i]);
            i++;
        }
        printf("todo leido\n");
        fclose(archivo);
    }




void dibuja_ranking(struct ranking* puntajes, int* opcion) {
    // Código para dibujar la pantalla de ranking
    al_clear_to_color(al_map_rgb(0, 0, 0));
    ALLEGRO_BITMAP* fondoranking=al_load_bitmap("imagenes/ranking.jpg");
    al_draw_bitmap(fondoranking,0,0,0);
    ALLEGRO_FONT* font = al_create_builtin_font();

    //al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, "RANKING");
    
    for (int i = 0; i < MAX_JUGADORES; i++) {
        if (puntajes->nombres[i][0] != '\0') {
            char texto[100];
            al_draw_textf(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 120 + i * 60, ALLEGRO_ALIGN_CENTER, "%d. %s - Puntos: %d", i + 1, puntajes->nombres[i], puntajes->puntos[i]);
        }
    }

    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, ALLEGRO_ALIGN_CENTER, "Presiona ENTER para regresar al menú");
    al_flip_display();

    al_destroy_font(font);

    ALLEGRO_KEYBOARD_STATE keyboard_state;
    al_get_keyboard_state(&keyboard_state);

    // Esperar a que se presione la tecla ENTER para regresar al menú
    while (true) {
        if (al_key_down(&keyboard_state, ALLEGRO_KEY_ENTER)) {
            *opcion = 1;
            break;
        }
        al_get_keyboard_state(&keyboard_state);
    }
}



void ordena_ranking(struct ranking* puntajes) {
    int i, j;
    char temp_nombre[MAX_NOMBRE];
    int temp_puntos;

    for (i = 0; i < MAX_JUGADORES - 1; i++) {
        for (j = 0; j < MAX_JUGADORES - i - 1; j++) {
            if (puntajes->puntos[j] < puntajes->puntos[j + 1]) {
                // Intercambiar los puntos
                temp_puntos = puntajes->puntos[j];
                puntajes->puntos[j] = puntajes->puntos[j + 1];
                puntajes->puntos[j + 1] = temp_puntos;

                // Intercambiar los nombres
                strcpy(temp_nombre, puntajes->nombres[j]);
                strcpy(puntajes->nombres[j], puntajes->nombres[j + 1]);
                strcpy(puntajes->nombres[j + 1], temp_nombre);
            }
        }
        printf("ordenar %s %d\n", puntajes->nombres[i], puntajes->puntos[i]);
    }
}

void reescribirRanking(struct ranking* puntajes, char* nombre, int puntuacion) {
    // Verificar si el puntaje del jugador es mayor que el último puntaje en el ranking
    if (puntuacion > puntajes->puntos[MAX_JUGADORES - 1]) {
        // Reemplazar el último puntaje y nombre con los del jugador actual
        strcpy(puntajes->nombres[MAX_JUGADORES - 1], nombre);
        puntajes->puntos[MAX_JUGADORES - 1] = puntuacion;
        // Ordenar el ranking nuevamente (opcional, si el ranking no está siempre ordenado)
        
        // Reescribir el archivo con el ranking actualizado
        FILE* archivo = fopen("ranking.txt", "w");
        if (archivo == NULL) {
            printf("Error al abrir el archivo de ranking para escritura.\n");
            return;
        }
        for (int i = 0; i < MAX_JUGADORES; i++) {
            fprintf(archivo, "%s %d\n", puntajes->nombres[i], puntajes->puntos[i]);
        }
        fclose(archivo);
    }
}

void dibuja_enemigo(struct enemigo* malos, ALLEGRO_BITMAP* malo, int* retardo_enemigo,int* enemigocont,int* enemigocont2) {
    int i=0,j=0;
    for (i = 0; i < maxenemigos; i++) {
        if (malos[i].estado == 0 && malos[i].direccion==1) {
            al_draw_bitmap_region(malo,(*enemigocont)*30,0,30,30,malos[i].posx,malos[i].posy,0);
            if(*retardo_enemigo==100){
               (*enemigocont)++;
               *retardo_enemigo=0;
               if (*enemigocont>7)
               {
                *enemigocont=4;
               }
               
            }
            (*retardo_enemigo)++;
            
        }

        if (malos[i].estado == 0 && malos[i].direccion==2) {
            al_draw_bitmap_region(malo,(*enemigocont2)*30,0,30,30,malos[i].posx,malos[i].posy,0);
            if(*retardo_enemigo==100){
               (*enemigocont2)++;
               *retardo_enemigo=0;
               if (*enemigocont2>3)
               {
                *enemigocont2=0;
               }
               
            }
            (*retardo_enemigo)++;
            
        }
    }
}

void mover_enemigos(struct enemigo* malos){

    for (int i = 0; i < maxenemigos; i++)
    {   if (mapa1[(malos[i].posx+30)/30][malos[i].posy/30]!='x' && malos[i].direccion==1 && mapa1[(malos[i].posx+30)/30][(malos[i].posy+30)/30]=='x' )
        {
            malos[i].posx++;
        }

        else {
         malos[i].direccion=2;

        }

        if (mapa1[(malos[i].posx)/30][malos[i].posy/30]!='x' && malos[i].direccion==2 && mapa1[(malos[i].posx)/30][(malos[i].posy+30)/30]=='x')
        {
            malos[i].posx--;
        }

        else{
            malos[i].direccion=1;
        } 
    }
}

void colision_enemigo(struct enemigo* malos,struct perso* jugador,ALLEGRO_SAMPLE_INSTANCE* instanciadaño){
    for (int i = 0; i < maxenemigos; i++)
    {
        if(malos[i].estado==0){
            if(((jugador->posx)/30) == ((malos[i].posx)/30) && ((jugador->posy)/30) == ((malos[i].posy)/30)){
                malos[i].estado=1;
                jugador->vidas--;
                al_play_sample_instance(instanciadaño);
                //printf("\nllegaste aqu i este es i %d",i);
            }
            for (int j = 0; j < maxbalas; j++)
            {
               if(((jugador->balasjug[j].posx)/30) == ((malos[i].posx)/30) && ((jugador->balasjug[j].posy)/30) == ((malos[i].posy)/30) && jugador->balasjug[j].activado==0){
                    malos[i].estado=1;
                    jugador->balasjug[j].activado=1;
                } 
            }
            
            

            
        }
    }
    
}

    bool enfriamientobala_jug() {
            if (bandera_jug>0)
            {
               bandera_jug = bandera_jug - 4; 
            }

           // printf("\nbandera %d ",bandera);
            
            if (bandera_jug == 0)
            {
               bandera_jug=600; 
               return true;
            }
            else{
             return false;
            }
    }

void inicia_balas_jug(struct perso* jugador){
   int i = 0;
    for(i = 0; i < maxbalas; i++) {
        jugador->balasjug[i].activado = 1;
        jugador->balasjug[i].posx = -1;
        jugador->balasjug[i].posy = -1;
    }

}

void dispararBala(struct perso* jugador,ALLEGRO_SAMPLE_INSTANCE* instanciadisparo) {
    for (int i = 0; i < maxbalas; i++) {
            ALLEGRO_KEYBOARD_STATE keyboard_state;
            al_get_keyboard_state(&keyboard_state);
        if (jugador->balasjug[i].activado==1 && al_key_down(&keyboard_state, ALLEGRO_KEY_X) ) {
            if (enfriamientobala_jug() == true)
            {
                al_play_sample_instance(instanciadisparo);
              // Disparar una bala en la dirección actual del jugador
                // Puedes ajustar las posiciones iniciales según la dirección del jugador

                if(al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)){
                    jugador->balasjug[i].posx = jugador->posx; // Puedes ajustar las posiciones iniciales según la dirección del jugador
                    jugador->balasjug[i].posy = jugador->posy; 
                    jugador->balasjug[i].tipo = 1;// 1 a la derecha 2 a la izquierda
                    jugador->balasjug[i].activado = 0; 
                }
                else if(al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)){
                    jugador->balasjug[i].posx = jugador->posx-30; // Puedes ajustar las posiciones iniciales según la dirección del jugador
                    jugador->balasjug[i].posy = jugador->posy; 
                    jugador->balasjug[i].tipo = 2;// 1 a la derecha 2 a la izquierda
                    jugador->balasjug[i].activado = 0; 
                }
                
                 
            }
            
                
                
        }
        else if(jugador->balasjug[i].activado==0) {
            if (jugador->balasjug[i].tipo==1)
            {
               jugador->balasjug[i].posx += 1;
            }
            
            if (jugador->balasjug[i].tipo==2)
            {
               jugador->balasjug[i].posx -= 1;
            }
            // printf("\nla pos de la balax %d",jugador->balasjug[i].posx);
            // printf("\nla pos de la balay %d",jugador->balasjug[i].posy);
            
        }
    }
}

void dibuja_bala_jug(struct perso* jugador, ALLEGRO_BITMAP* bala_jugador) {
    int j=0;
        for (j = 0; j < maxbalas; j++) {
            if (jugador->balasjug[j].activado == 0) {
                al_draw_bitmap(bala_jugador, jugador->balasjug[j].posx, jugador->balasjug[j].posy, 0);
            }
        }
    
}

 void colision_bala_jug(struct perso* jugador){
    int i=0;
    int posx,posy;
    for(i=0;i<maxbalas;i++){
        // posx=jugador->balasjug[i].posx;
        // posy=jugador->balasjug[i].posy;
        if (mapa1[(jugador->balasjug[i].posx) / 30][(jugador->balasjug[i].posy) / 30] == 'x'&& jugador->balasjug[i].activado==0) {
                    jugador->balasjug[i].activado = 1;
                }
    }
 }


