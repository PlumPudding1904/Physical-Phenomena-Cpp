#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <cstdlib>
#include <stdio.h>
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

    double **U;
    double **pom;
    int imax=80, jmax=60;
    U=new double*[imax];
    pom=new double*[imax];

    for(int i=0; i<imax; i++)
    {
        U[i]= new double[jmax];
    }
    for(int i=0;i<imax;i++)
    {
        pom[i]=new double[jmax];
    }

    for(int i=0; i<imax;i++)
    {
        for(int j=0; j<jmax; j++)
        {
            U[i][j]=0.0;
        }
    }
    for(int i=0;i<imax;i++)
    {
        U[i][0]=sin(2.0*M_PI*(double)i/(double)imax)*sinh(2.0*M_PI*(double)0/(double)jmax)+sinh(2.0*M_PI);
    }
    for(int i=0;i<imax;i++)
    {
        U[i][jmax-1]=sin(2.0*M_PI*(double)i/(double)imax)*sinh(2.0*M_PI*(double)(jmax-1)/(double)jmax)+sinh(2.0*M_PI);
    }
     for(int j=0;j<jmax;j++)
    {
        U[0][j]=sin(2.0*M_PI*(double)0/(double)imax)*sinh(2.0*M_PI*(double)j/(double)jmax)+sinh(2.0*M_PI);
    }
    for(int j=0;j<jmax;j++)
    {
        U[imax-1][j]=sin(2.0*M_PI*(double)(imax-1)/(double)imax)*sinh(2.0*M_PI*(double)j/(double)jmax)+sinh(2.0*M_PI);
    }

    double kolormin, kolormax, kolor;
    double blad;
    int krok=0;

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

            for(int i=1;i<imax-1;i++)
                for(int j=1;j<jmax-1;j++)
                    pom[i][j]=0.25*(U[i+1][j]+U[i-1][j]+U[i][j-1]+U[i][j+1]);

            for(int  i=1;i<imax-1;i++)
                for(int j=1;j<jmax-1;j++)
                    U[i][j]=pom[i][j];
            kolormin=U[0][0];
            kolormax=U[0][0];
            for(int i=0;i<imax;i++)
                for(int j=0;j<jmax;j++)
            {
                if(U[i][j]>kolormax)
                    kolormax=U[i][j];
                if(U[i][j]<kolormin)
                    kolormin=U[i][j];
            }
            for(int i=0;i<imax;i++)
                for(int j=0;j<jmax;j++)
                {
                    kolor=(U[i][j]-kolormin)/(kolormax-kolormin)*255.0;
                    al_draw_filled_rectangle(i*(800.0/imax),j*(600.0/jmax),(i+1)*(800.0/imax),(j+1)*(600.0/jmax),al_map_rgb(kolor,0,255.0-kolor));
                }
                krok++;
                blad=0.0;
                for(int i=0;i<imax;i++)
                for(int j=0;j<jmax;j++)
                {
                    blad+=fabs(U[i][j]-sin(2.0*M_PI*(double)i/(double)imax)*sinh(2.0*M_PI*(double)j/(double)jmax)-sinh(2.0*M_PI))/(imax*jmax);

                }
                printf("%d\t%.15lf\n",krok,blad);


            al_flip_display();
        }
    }
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}
