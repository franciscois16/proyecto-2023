#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include <cstdio>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const float IMAGE_SPEED = 20.0f;

int main()
{
    al_init();
    al_install_keyboard();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
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

    const char *ruta_imagen = "imagenes/pedro.jpg";
    ALLEGRO_BITMAP *imagen = al_load_bitmap(ruta_imagen);
    if (!imagen) {
        fprintf(stderr, "Error al cargar la imagen.\n");
        return -1;
    }

    float posX = SCREEN_WIDTH / 2 - al_get_bitmap_width(imagen) / 2;
    float posY = SCREEN_HEIGHT / 2 - al_get_bitmap_height(imagen) / 2;

    //float velX = IMAGE_SPEED;
    //float velY = IMAGE_SPEED;

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

        //    if (posX < 0 || posX + al_get_bitmap_width(imagen) > SCREEN_WIDTH)
      //          velX = -velX;
    //        if (posY < 0 || posY + al_get_bitmap_height(imagen) > SCREEN_HEIGHT)
  //              velY = -velY;
//
            al_draw_bitmap(imagen, posX, posY, 0);
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Pantalla de bloqueo de DVD");
            al_flip_display();

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
