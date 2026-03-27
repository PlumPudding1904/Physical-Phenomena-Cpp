#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include<math.h>
#include <stdio.h>
#include<stdlib.h>

double fx(double x, double y, double vx, double vy, double t)
{
    return vx;
}

double fy(double x, double y, double vx, double vy, double t)
{
    return vy;
}

double gx(double x, double y, double vx, double vy, double t)
{
    double pierwiastek = sqrt(x*x+y*y);
    return -x/(pierwiastek*pierwiastek*pierwiastek);
}

double gy(double x, double y, double vx, double vy, double t)
{
    double pierwiastek = sqrt(x*x+y*y);
    return -y/(pierwiastek*pierwiastek*pierwiastek);
}

int main(int argc, char **argv)
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    ALLEGRO_DISPLAY *display=al_create_display(800, 600);
    bool redraw=true;
    const float FPS=1000;
    ALLEGRO_TIMER *timer=al_create_timer(1.0/FPS);
    al_start_timer(timer);
    ALLEGRO_EVENT_QUEUE *event_queue=al_create_event_queue();
    al_register_event_source(event_queue,
                             al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    double dt=0.01, t=0;
    double x, y, vx, vy;
    double x0=0.0, y0=1.0, vx0=2.0*sqrt(2), vy0=0;
    x=x0; vx=vx0; y=y0; vy=vy0;
    double kx[4], lx[4], ky[4], ly[4];
    double v;

    double tE=0;
    double xE, yE, vxE, vyE;
    xE=x0; vxE=vx0; yE=y0; vyE=vy0;
    double kxE[4], lxE[4], kyE[4], lyE[4];

    double bladE, blad;
    FILE *plik=fopen("blad_Euler.txt","w");
    FILE *plik1=fopen("blad_RK.txt","w");
    int ite=0;
    double rE, r;
    double phi;

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

            //RK4

            kx[0]=fx(x,y,vx,vy,t);
            ky[0]=fy(x,y,vx,vy,t);
            lx[0]=gx(x,y,vx,vy,t);
            ly[0]=gy(x,y,vx,vy,t);

            kx[1]=fx(x+0.5*dt*kx[0],y+0.5*dt*ky[0],vx+0.5*dt*lx[0],vy+0.5*dt*ly[0],t+0.5*dt);
            ky[1]=fy(x+0.5*dt*kx[0],y+0.5*dt*ky[0],vx+0.5*dt*lx[0],vy+0.5*dt*ly[0],t+0.5*dt);
            lx[1]=gx(x+0.5*dt*kx[0],y+0.5*dt*ky[0],vx+0.5*dt*lx[0],vy+0.5*dt*ly[0],t+0.5*dt);
            ly[1]=gy(x+0.5*dt*kx[0],y+0.5*dt*ky[0],vx+0.5*dt*lx[0],vy+0.5*dt*ly[0],t+0.5*dt);

            kx[2]=fx(x+0.5*dt*kx[1],y+0.5*dt*ky[1],vx+0.5*dt*lx[1],vy+0.5*dt*ly[1],t+0.5*dt);
            ky[2]=fy(x+0.5*dt*kx[1],y+0.5*dt*ky[1],vx+0.5*dt*lx[1],vy+0.5*dt*ly[1],t+0.5*dt);
            lx[2]=gx(x+0.5*dt*kx[1],y+0.5*dt*ky[1],vx+0.5*dt*lx[1],vy+0.5*dt*ly[1],t+0.5*dt);
            ly[2]=gy(x+0.5*dt*kx[1],y+0.5*dt*ky[1],vx+0.5*dt*lx[1],vy+0.5*dt*ly[1],t+0.5*dt);

            kx[3]=fx(x+dt*kx[2],y+dt*ky[2],vx+dt*lx[2],vy+dt*ly[2],t+dt);
            ky[3]=fy(x+dt*kx[2],y+dt*ky[2],vx+dt*lx[2],vy+dt*ly[2],t+dt);
            lx[3]=gx(x+dt*kx[2],y+dt*ky[2],vx+dt*lx[2],vy+dt*ly[2],t+dt);
            ly[3]=gy(x+dt*kx[2],y+dt*ky[2],vx+dt*lx[2],vy+dt*ly[2],t+dt);

            x=x+dt/6.0*(kx[0]+2.0*kx[1]+2.0*kx[2]+kx[3]);
            y=y+dt/6.0*(ky[0]+2.0*ky[1]+2.0*ky[2]+ky[3]);
            vx=vx+dt/6.0*(lx[0]+2.0*lx[1]+2.0*lx[2]+lx[3]);
            vy=vy+dt/6.0*(ly[0]+2.0*ly[1]+2.0*ly[2]+ly[3]);
            t=t+dt;

            //Euler

            kxE[0]=fx(xE,yE,vxE,vyE,tE);
            kyE[0]=fy(xE,yE,vxE,vyE,tE);
            lxE[0]=gx(xE,yE,vxE,vyE,tE);
            lyE[0]=gy(xE,yE,vxE,vyE,tE);

            kxE[1]=fx(xE+0.5*dt*kxE[0],yE+0.5*dt*kyE[0],vxE+0.5*dt*lxE[0],vyE+0.5*dt*lyE[0],tE+0.5*dt);
            kyE[1]=fy(xE+0.5*dt*kxE[0],yE+0.5*dt*kyE[0],vxE+0.5*dt*lxE[0],vyE+0.5*dt*lyE[0],tE+0.5*dt);
            lxE[1]=gx(xE+0.5*dt*kxE[0],yE+0.5*dt*kyE[0],vxE+0.5*dt*lxE[0],vyE+0.5*dt*lyE[0],tE+0.5*dt);
            lyE[1]=gy(xE+0.5*dt*kxE[0],yE+0.5*dt*kyE[0],vxE+0.5*dt*lxE[0],vyE+0.5*dt*lyE[0],tE+0.5*dt);

            kxE[2]=fx(xE+0.5*dt*kxE[1],yE+0.5*dt*kyE[1],vxE+0.5*dt*lxE[1],vyE+0.5*dt*lyE[1],tE+0.5*dt);
            kyE[2]=fy(xE+0.5*dt*kxE[1],yE+0.5*dt*kyE[1],vxE+0.5*dt*lxE[1],vyE+0.5*dt*lyE[1],tE+0.5*dt);
            lxE[2]=gx(xE+0.5*dt*kxE[1],yE+0.5*dt*kyE[1],vxE+0.5*dt*lxE[1],vyE+0.5*dt*lyE[1],tE+0.5*dt);
            lyE[2]=gy(xE+0.5*dt*kxE[1],yE+0.5*dt*kyE[1],vxE+0.5*dt*lxE[1],vyE+0.5*dt*lyE[1],tE+0.5*dt);

            kxE[3]=fx(xE+dt*kxE[2],yE+dt*kyE[2],vxE+dt*lxE[2],vyE+dt*lyE[2],tE+dt);
            kyE[3]=fy(xE+dt*kxE[2],yE+dt*kyE[2],vxE+dt*lxE[2],vyE+dt*lyE[2],tE+dt);
            lxE[3]=gx(xE+dt*kxE[2],yE+dt*kyE[2],vxE+dt*lxE[2],vyE+dt*lyE[2],tE+dt);
            lyE[3]=gy(xE+dt*kxE[2],yE+dt*kyE[2],vxE+dt*lxE[2],vyE+dt*lyE[2],tE+dt);

            xE=xE+dt*kxE[0];
            yE=yE+dt*kyE[0];
            vxE=vxE+dt*lxE[0];
            vyE=vyE+dt*lyE[0];
            tE=tE+dt;

            //porówanie
             //printf("Roznica (RK4-Euler) x: %f \n", x-xE);
             //printf("Roznica y: %f \n", y-yE);
             //printf("Roznica vx: %f \n", vx-vxE);
             //printf("Roznica vy: %f \n", vy-vyE);
             //printf("Roznica t: %f \n", t-tE);


            if(t>100.0)
                break;//ten sam czas dla obu symulacji, blad na wykresie skala log(Yaxis)

            ite++;
            if(ite%100==0)
            {
                v = sqrt(vx0*vx0+vy0*vy0);
                //r=sqrt(x*x+y*y);
                printf("t=%lf, y=%lf,x=%lf\n",t,y,x);
                //cosphi=2/(sqrt(x*x+y*y))-1;
                r=v*v/(1+sqrt(1+v*v*(-2+v*v))*cos(M_PI/3 + t/(3.0*M_PI)));
                blad=fabs(1.0-r);
                //fprintf(plik,"%lf\t%.15lf\n",t,r);
                fprintf(plik,"%.15lf\n",r);
            }
            //al_clear_to_color(al_map_rgb(0,0,0));
            //al_draw_filled_circle(400,300,100,al_map_rgb(255,255,255));
            al_draw_filled_circle(400+x*100,300-y*100,5,al_map_rgb(0,255,255));
            //al_draw_filled_circle(400+x*100,300-y*100,5,al_map_rgb(255,0,255));
            al_flip_display();
        }
    }
    fclose(plik);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}

