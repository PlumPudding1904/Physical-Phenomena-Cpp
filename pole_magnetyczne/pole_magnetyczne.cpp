#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <math.h>

class zespolona
{
private:
    double Re, Im;
public:
    zespolona():Re(0.0), Im(0.0) {}
    zespolona(double Re, double Im):Re(Re), Im(Im) {}
    double r()const
    {
        return Re;
    }
    double i()const
    {
        return Im;
    }
    friend zespolona operator*(const zespolona  &,const zespolona &);
    friend zespolona operator+(const zespolona  &,const zespolona &);
    friend zespolona operator*(const double  &,const zespolona &);
    friend zespolona operator-(const zespolona  &,const zespolona &);
};
zespolona operator*(const zespolona  & a,const zespolona & b)
{
    double rz,ur;
    rz=a.r()*b.r()-a.i()*b.i();
    ur=a.i()*b.r()+a.r()*b.i();
    return zespolona(rz,ur);
}
zespolona operator+(const zespolona & a,const zespolona & b)
{
    double rz,ur;
    rz=a.r()+b.r();
    ur=a.i()+b.i();
    return zespolona(rz,ur);
}
zespolona operator*(const double & a,const zespolona & b)
{
    double rz,ur;
    rz=a*b.r();
    ur=a*b.i();
    return zespolona(rz,ur);
}
zespolona operator-(const zespolona & a,const zespolona & b)
{
    double rz,ur;
    rz=a.r()-b.r();
    ur=a.i()-b.i();
    return zespolona(rz,ur);
}
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

    zespolona r(0,0);
    zespolona r2(1,0);
    zespolona v(1,0);
    zespolona v2(-1,0);
    double m=1.0, c=1.0, e=-1.0, Bz=1.0, m2=4.0, e2=1.0;
    double Omega=e*Bz/(m*c);
    double Omega2=e2*Bz/(m2*c);
    double t=0;
    double dt=0.01;
    double alfa=0.5*dt*Omega;
    double alfa2=0.5*dt*Omega2;
    zespolona skalav((1.0-alfa*alfa)/(1.0+alfa*alfa),-2.0*alfa/(1.0+alfa*alfa));
    zespolona skalav2((1.0-alfa2*alfa2)/(1.0+alfa2*alfa2),-2.0*alfa2/(1.0+alfa2*alfa2));
    zespolona pom, pom2;
    double Ex=0.3, Ey=-0.5;
    zespolona E(Ex,Ey);
    zespolona skalaE(e/m*dt/(1.0+alfa*alfa),-e*alfa*dt/m/(1.0+alfa*alfa));
    zespolona skalaE2(e2/m2*dt/(1.0+alfa*alfa),-e2*alfa*dt/m2/(1.0+alfa*alfa));
    double Ek, Ep;
    FILE *plik=fopen("Ek.txt","w");
    FILE *plik1=fopen("Ep.txt","w");
    FILE *plik2=fopen("Ec.txt","w");
    zespolona roznica = r-r2;
    double diss = sqrt(roznica.r()*roznica.r()+roznica.i()*roznica.i());
    zespolona Coulomb(0,0);
    if(diss>0.1)
        Coulomb=(e*e2/(diss*diss*diss))*roznica;
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
            pom=v;
            pom2=v2;
            v=skalaE*E+skalav*v+(dt/m)*Coulomb;//EM
            v2=skalaE2*E+skalav2*v2-(dt/m2)*Coulomb;
            //v=skalav*v;//Magnetyczne
            r=r+0.5*dt*(pom+v);
            r2=r2+0.5*dt*(pom2+v2);
            //Ek=0.5*m*(v.r()*v.r()+v.i()*v.i());//dla poj
            //Ep=- e*(E.r()*r.r()+E.i()*r.i());//dla poj
            t=t+dt;
            //fprintf(plik,"%lf\t%lf\n",t,Ek);
            //fprintf(plik1,"%lf\t%lf\n",t,Ep);
            //fprintf(plik2,"%lf\t%lf\n",t,Ek+Ep);
            //printf("R: %f\n",r);
            //al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_filled_circle(400+20.0*r.r(),300+20.0*r.i(),2,al_map_rgb(255,0,0));
            al_draw_filled_circle(400+20.0*r2.r(),300+20.0*r2.i(),2,al_map_rgb(0,255,0));
            al_flip_display();
        }
    }
    fclose(plik);
    fclose(plik1);
    fclose(plik2);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}
