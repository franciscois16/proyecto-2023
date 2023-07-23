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
    #define maxtiradores 3
    #define maxbalas 5
    #define maxniveles 3
    #define MAX_JUGADORES 10
    #define MAX_NOMBRE  50

    struct perso
    {
        int posx= 0.0;
        int posy= 0.0;
        int estado =1; // 1=quieto,2camina derecha,3camina izquierda,4 salta y 5 se agarra
        int vidas = 300;
        //int quieto = 0;
        //int camina = 0;
        //int salta = 0;
        //int pared = 0;
        
    };

    struct dispara
    {     
    };


    struct proyectil {
    int dibposx; //posicion x de donde parte la bala
    int dibposy; //posicion y de donde parte la bala
    //int direccion;
    int posx[maxbalas]; //posicion actual de donde esta la bala
    int posy[maxbalas]; //posicion actual de donde esta la bala
    int activado[maxbalas]; // estado que determina si la bala se mueve o no 
    };
    


    void cargarmapaarchivo(struct perso* jugador,struct proyectil* proyectil1,int* nivelactual);
    void dibujamapa(ALLEGRO_BITMAP* ladrillo, ALLEGRO_BITMAP* escalera, ALLEGRO_BITMAP* trampabmp,ALLEGRO_FONT* font,ALLEGRO_BITMAP* portal,int* p);
    void dibujarpersonaje(ALLEGRO_BITMAP* personaje,struct perso jugador,int* pani,int* pcaminader,ALLEGRO_BITMAP* caminaderecha,int* pcaminaizq);
    void moverpersonaje(struct perso* jugador,struct proyectil* proyectil);
    void acciones(struct perso* jugador,int* inercia,int* agarre,int* nivel);
    int moverCamara(int personaje_y, int camara_y);
    void dibujabala(struct proyectil* proyectil, ALLEGRO_BITMAP* bala_imagen);
    void diparabala(struct proyectil* proyectil, ALLEGRO_BITMAP* bala_imagen);
    bool enfriamientobala();
    void verificanivel(int* nivelactual , int* nivel,struct perso* jugador, struct proyectil* proyectil1);
    void menu(int* opcion);
    bool verificarColision(struct perso* jugador, struct proyectil* proyectil1);
    void ingresarNombre(char* nombre);
    void leerranking(char nombres[MAX_JUGADORES][MAX_NOMBRE], int puntos[MAX_JUGADORES]);
    void dibuja_ranking(char nombresranking[MAX_JUGADORES][MAX_NOMBRE], int puntos[MAX_JUGADORES]);



    const int SCREEN_WIDTH = 50*30;
    const int SCREEN_HEIGHT = 30*30;
    const float IMAGE_SPEED = 20.0f;

    int fil,col,inercia = 0;
    int agarre = 0;
    int bandera=0;
    
    
    

    char mapa1[maxcolumnas][maxfilas];
    int main()
    {
        int retardo = 0 , retardo_1 = 0;
        int personaje_y = 0; 
        int camara_y = 0; 
        int nivelactual = 1 , nivel = 1;
        int p=0,pani=0,pcaminader=0,pcaminaizq=7;
        int opcion=1;
        char nombre[50];
        char nombresranking[10][50];
        int puntos[10];

        al_init();
        al_install_audio();
        al_init_acodec_addon();
        al_init_font_addon();
        al_install_keyboard();

        ALLEGRO_TIMER *timer = al_create_timer(1.0 / 120);//este es para que decir cada cuanto queremos que se haga una accion = a fps aqui son 30 cada seg
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
        ALLEGRO_BITMAP* bala = al_load_bitmap("imagenes/pedribala.jpg");
        ALLEGRO_BITMAP* portal = al_load_bitmap("imagenespersonaje/portal.png");
        //ALLEGRO_SAMPLE *salto = al_load_sample("imagenes/salto.wav");
        ALLEGRO_BITMAP* personajequieto = al_load_bitmap("imagenespersonaje/pquieto.png");
        ALLEGRO_BITMAP* caminaderecha = al_load_bitmap("imagenespersonaje/pcaminaderecha.png");




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
        
         
        
        
        
        
        while (opcion!=4)
        {
            al_start_timer(timer);
            al_wait_for_event(queue, &event);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            printf("%d vidas" , jugador.vidas);
            if (event.type == ALLEGRO_EVENT_TIMER)
                redraw = true;
            else if ((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE))
                break;

            if (redraw && al_is_event_queue_empty(queue))
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                
            
                cargarmapaarchivo(&jugador ,proyectil1,&nivelactual);
                ALLEGRO_KEYBOARD_STATE keyboard_state;
                if (opcion == 1) {
                    menu(&opcion);
                }
                

                if (opcion==2)
                {
                    ingresarNombre(nombre);
                   do {
                        
                        al_get_keyboard_state(&keyboard_state);

                        if (al_key_down(&keyboard_state, ALLEGRO_KEY_ESCAPE)) {
                            break;  // Salir del bucle si la tecla 'ESC' está presionada
                        }

                        al_draw_bitmap(imagen, 0, 0, 0);
                        verificanivel(&nivelactual, &nivel, &jugador, proyectil1);
                        dibujamapa(ladrillo, escalera, trampabmp,font,portal,&p);
                        dibujarpersonaje(personajequieto,jugador,&pani,&pcaminader,caminaderecha,&pcaminaizq);
                        moverCamara(jugador.posy,camara_y);
                        dibujabala(proyectil1,bala);
                        if (retardo==0)
                        {
                            
                            acciones(&jugador, &inercia,&agarre,&nivel);
                            moverpersonaje(&jugador,proyectil1);
                            
                        // printf("1");
                        }

                        if(retardo_1==0)
                        {
                            
                            diparabala(proyectil1,bala);
                            //printf("2");
                        }

                        retardo_1=(retardo_1+1)%3;
                        
                        //retardo=(retardo+1)%2;
                        //al_rest(0.01);
                        
                        al_flip_display(); 
                    } while (jugador.vidas > 1); /* code */
                }

                if (opcion==3)
                {
                    al_get_keyboard_state(&keyboard_state);
                    leerranking(nombresranking,puntos);
                    while(al_key_down(&keyboard_state, ALLEGRO_KEY_BACKSPACE)){
                    dibuja_ranking(nombresranking, puntos);
                    }
                }
                
                


            

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
        // al_destroy_sample(salto);


        return 0;
    }



void menu(int* opcion) {
    ALLEGRO_KEYBOARD_STATE keyboard_state;
    ALLEGRO_BITMAP* menu = al_load_bitmap("imagenes/menu.png");

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




void cargarmapaarchivo(struct perso* jugador, struct proyectil* proyectil1,int* nivelactual) {
    ALLEGRO_FILE* mapa;
    
    if(*nivelactual==1){
        mapa = al_fopen("mapa.txt", "r");
    }

    if(*nivelactual==2){
        mapa = al_fopen("mapa2.txt", "r");
    }

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



    void dibujamapa(ALLEGRO_BITMAP* ladrillo, ALLEGRO_BITMAP* escalera, ALLEGRO_BITMAP* trampabmp,ALLEGRO_FONT* font,ALLEGRO_BITMAP* portal,int* p)//, , int vidas, int puntaje
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
                else if (mapa1[j][i] == 'n') {
                    al_draw_bitmap_region(portal,(*p)*30,0,30,30,j*30,i*30,0);
                    (*p)++;
                    if (*p>6)
                    {
                       *p=0;
                    }
                    
                }
                
            //   al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "¡Hola, mundo!");
                
                
            }
        }

    // al_draw_textf(font, al_map_rgb(255, 0, 0), 10, 10, 0, "VIDAS: %d", vidas);
    //   al_draw_textf(font, al_map_rgb(255, 0, 0), 10, 30, 0, "PUNTAJE: %d", puntaje);
    }

    void dibujarpersonaje(ALLEGRO_BITMAP* personajequieto, struct perso jugador,int* pani,int* pcaminader,ALLEGRO_BITMAP* caminaderecha,int* pcaminaizq) {

        if(jugador.estado==1){
            al_draw_bitmap_region(personajequieto,(*pani)*30,0,30,30,jugador.posx,jugador.posy,0);
            (*pani)++;
            if (*pani>4)
            {
                *pani=0;
            }
        }

        else if(jugador.estado==2){
            al_draw_bitmap_region(caminaderecha,(*pcaminader)*30,0,30,30,jugador.posx,jugador.posy,0);
            (*pcaminader)++;
            //printf("\ncamina%d",*pcaminader);
            if (*pcaminader>6)
            {
                *pcaminader=0;
            }
        }

        else if(jugador.estado==3){
            al_draw_bitmap_region(caminaderecha,(*pcaminaizq)*30,0,30,30,jugador.posx,jugador.posy,0);
            (*pcaminaizq)++;
           // printf("\nizquierda%d",*pcaminaizq);
            if (*pcaminaizq>12)
            {
                *pcaminaizq=7;
            }
        }


        //al_draw_bitmap(personajequieto, pixelPosX, pixelPosY, 0);
    }

    void moverpersonaje(struct perso* jugador,struct proyectil* proyectil1) {
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

            if (verificarColision(jugador, proyectil1)) {
                // Si hay colisión, realiza alguna acción (por ejemplo, mostrar un mensaje)
                printf("¡Has sido alcanzado por una bala! vidas %d\n",jugador->vidas);
            }

    
        }


    void acciones(struct perso* jugador, int* inercia, int* agarre , int* nivel) {
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
        if (mapa1[jugador->posx / 30][(jugador->posy + 30) / 30] == 'o' && *inercia == 0 && *agarre == 0 && mapa1[(jugador->posx+29) / 30][(jugador->posy + 30) / 30] == 'o' && *inercia == 0 && *agarre == 0 ) {
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


        // if(mapa1[jugador->posx/30][(jugador->posy)/30] == 't')
        // {
        //     jugador->vidas = jugador->vidas - 1;
        // printf("%d vidas" , jugador->vidas);
        //     jugador->posx -= 30;
        // }

        if(mapa1[jugador->posx/30][(jugador->posy)/30] == 'n')
        {
            (*nivel)++;
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
                
                if (proyectil[i].activado[j] == 1) {
                     if(enfriamientobala() == true){
                    proyectil[i].activado[j] = 0;
                    //printf("\nprueba:j%d estado%d", j, proyectil[i].activado[j]);
                    proyectil[i].posx[j] = proyectil[i].dibposx;
                    proyectil[i].posy[j] = proyectil[i].dibposy - 30;
                    }
                } else {
                    
                    proyectil[i].posy[j] -= 1;
                    //printf("\nposicioni:%d,pos%d",j, proyectil[i].posy[j]);

                    // Verificar si hay una 'x' en los próximos 30 píxeles en la dirección de avance
                    if (mapa1[proyectil[i].posx[j] / 30][proyectil[i].posy[j] / 30] == 'x') {
                        proyectil[i].activado[j] = 1;
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
               bandera=2000; 
               return true;
            }
            else{
             return false;
            }
        
        
        

    }

    void verificanivel(int* nivelactual , int* nivel,struct perso* jugador, struct proyectil* proyectil1){
        if(*nivelactual != *nivel){
            *nivelactual=*nivel;
            cargarmapaarchivo(jugador ,proyectil1,nivelactual);
        }


    }

bool verificarColision(struct perso* jugador, struct proyectil* proyectil) {
    // Definimos el tamaño de la zona de colisión adicional (en píxeles)
    int colisionAdicional = 4;

    // Coordenadas del rectángulo que representa al jugador
    int jugador_x1 = jugador->posx + colisionAdicional;
    int jugador_x2 = jugador->posx + 30 - colisionAdicional;
    int jugador_y1 = jugador->posy + colisionAdicional;
    int jugador_y2 = jugador->posy + 30 - colisionAdicional;

    // Iteramos sobre las balas y comprobamos si alguna colisiona con el jugador
    for (int i = 0; i < maxtiradores; i++) {
        for (int j = 0; j < maxbalas; j++) {
            if (proyectil[i].activado[j] == 0) {
                // Coordenadas del rectángulo que representa a la bala
                int bala_x1 = proyectil[i].posx[j] + colisionAdicional;
                int bala_x2 = proyectil[i].posx[j] + 30 - colisionAdicional;
                int bala_y1 = proyectil[i].posy[j] + colisionAdicional;
                int bala_y2 = proyectil[i].posy[j] + 30 - colisionAdicional;

                // Comprobamos si hay intersección entre los rectángulos (colisión)
                if (jugador_x1 < bala_x2 && jugador_x2 > bala_x1 && jugador_y1 < bala_y2 && jugador_y2 > bala_y1) {
                    jugador->vidas--;
                    proyectil[i].posy[j]=1;
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


void leerranking(char nombres[MAX_JUGADORES][MAX_NOMBRE], int puntos[MAX_JUGADORES]) {
    FILE *archivo = fopen("ranking.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de ranking.\n");
        return;
    }

    int i = 0;
    char nombre[MAX_NOMBRE];
    int puntaje;

    while (i < MAX_JUGADORES && fscanf(archivo, "%s %d", nombre, &puntaje) == 2) {
        strcpy(nombres[i], nombre);
        puntos[i] = puntaje;
        i++;
    }
    printf("todo leido");
    fclose(archivo);
}

void dibuja_ranking(char nombresranking[MAX_JUGADORES][MAX_NOMBRE], int puntos[MAX_JUGADORES]) {
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR bgColor = al_map_rgba(0, 0, 0, 200);
    const int textWidth = 300;
    const int textHeight = 50;

    ALLEGRO_BITMAP* textBitmap = al_create_bitmap(textWidth, textHeight * (MAX_JUGADORES + 1));
    al_set_target_bitmap(textBitmap);

    al_clear_to_color(bgColor);

    for (int i = 0; i < MAX_JUGADORES; i++) {
        char text[MAX_NOMBRE + 10];
        sprintf(text, "%d. %s - %d puntos", i + 1, nombresranking[i], puntos[i]);
        al_draw_text(font, color, textWidth / 2, i * textHeight + textHeight / 2 - al_get_font_ascent(font) / 2, ALLEGRO_ALIGN_CENTER, text);
    }

    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
    al_draw_bitmap(textBitmap, al_get_display_width(al_get_current_display()) / 2 - textWidth / 2, al_get_display_height(al_get_current_display()) / 2 - textHeight * (MAX_JUGADORES + 1) / 2, 0);
    al_flip_display();

    al_destroy_bitmap(textBitmap);

    bool done = false;
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    while (!done) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            done = true;
        }
    }

    al_destroy_event_queue(event_queue);
}
