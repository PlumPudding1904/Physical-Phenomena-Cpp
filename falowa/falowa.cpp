#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
int main(int argc, char **argv)
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    ALLEGRO_DISPLAY *display=al_create_display(800, 600);
    bool redraw=true;
    const float FPS=60;
    ALLEGRO_TIMER *timer=al_create_timer(1.0/FPS);
    al_start_timer(timer);
    ALLEGRO_EVENT_QUEUE *event_queue=al_create_event_queue();
    al_register_event_source(event_queue,
                             al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    double psireal[1280];
    double psiimag[1280];

    double psireal_m1[1280];
    double psiimag_m1[1280];

    double psireal_p1[1280];
    double psiimag_p1[1280];

    double dx=1.0/1280.0;
    double dt=dx*dx*0.5;

    double x;

    for(int i=0; i<1280; i++)
    {
        x=dx*i;
        psireal_m1[i]=cos(4.0*M_PI*x);
        psiimag_m1[i]=sin(2.0*M_PI*x);
    }

    psireal[0]=psireal_m1[0]-dt*(psiimag_m1[1]-2.0*psiimag_m1[0]+psiimag_m1[1279])/(2.0*dx*dx);
    psiimag[0]=psiimag_m1[0]+dt*(psireal_m1[1]-2.0*psireal_m1[0]+psireal_m1[1279])/(2.0*dx*dx);

    for(int i=1; i<1280-1; i++)
    {
        psireal[i]=psireal_m1[i]-dt*(psiimag_m1[i+1]-2.0*psiimag_m1[i]+psiimag_m1[i-1])/(2.0*dx*dx);
        psiimag[i]=psiimag_m1[i]+dt*(psireal_m1[i+1]-2.0*psireal_m1[i]+psireal_m1[i-1])/(2.0*dx*dx);
    }

    psireal[1279]=psireal_m1[1279]-dt*(psiimag_m1[0]-2.0*psiimag_m1[1279]+psiimag_m1[1278])/(2.0*dx*dx);
    psiimag[1279]=psiimag_m1[1279]+dt*(psireal_m1[0]-2.0*psireal_m1[1279]+psireal_m1[1278])/(2.0*dx*dx);

    while(1)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue,&ev);
        if(ev.type==ALLEGRO_EVENT_TIMER)
        {
            redraw=true;
        }
        else if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            al_clear_to_color(al_map_rgb(0,0,0));

            for(int j=0; j<1000; j++)
            {
                psireal_p1[0]=psireal_m1[0]-dt*(psiimag[1]-2.0*psiimag[0]+psiimag[1279])/(dx*dx);
                psiimag_p1[0]=psiimag_m1[0]+dt*(psireal[1]-2.0*psireal[0]+psireal[1279])/(dx*dx);

                for(int i=1; i<1280-1; i++)
                {
                    psireal_p1[i]=psireal_m1[i]-dt*(psiimag[i+1]-2.0*psiimag[i]+psiimag[i-1])/(dx*dx);
                    psiimag_p1[i]=psiimag_m1[i]+dt*(psireal[i+1]-2.0*psireal[i]+psireal[i-1])/(dx*dx);
                }

                psireal_p1[1279]=psireal_m1[1279]-dt*(psiimag[0]-2.0*psiimag[1279]+psiimag[1278])/(dx*dx);
                psiimag_p1[1279]=psiimag_m1[1279]+dt*(psireal[0]-2.0*psireal[1279]+psireal[1278])/(dx*dx);

                for(int i=0; i<1280; i++)
                {
                    psireal_m1[i]=psireal[i];
                    psiimag_m1[i]=psiimag[i];
                }
                for(int i=0; i<1280; i++)
                {
                    psireal[i]=psireal_p1[i];
                    psiimag[i]=psiimag_p1[i];
                }
            }

            for(int i=0; i<1279; i++)
            {
                al_draw_line(i,360+psireal[i]*100,i+1,360+psireal[i+1]*100.0,al_map_rgb(255,0,0),2);
                al_draw_line(i,360+psiimag[i]*100,i+1,360+psiimag[i+1]*100.0,al_map_rgb(0,0,255),2);
            }

            al_flip_display();
        }
    }
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}
