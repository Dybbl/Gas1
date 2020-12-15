#include "TXlib.h"
#include <cstdlib>
#include <iostream>
#include <cassert>
#include "math.h"

struct General
{

    double Screen_height;
    double Screen_width;

    int Game_is_over;

    int Game_level;

    int Numb_of_particles;

};

struct Game_object
{

    double x;
    double y;
    double r;

    double light_point_x;
    double light_point_y;

    int Red;
    int Green;
    int Blue;

    int drawing_level;

    double Vx;
    double Vy;

};

bool CheckColossionTwoSphere(Game_object Sphere1, Game_object Sphere2)
{

    if((pow((Sphere1.x - Sphere2.x),2)) + pow((Sphere1.y - Sphere2.y),2) <= pow((Sphere1.r + Sphere2.r),2))
    {
            return true;
    }

}

void Colission(Game_object* Sphere1, Game_object* Sphere2)
{

    double Bufer = 0;

    Bufer = Sphere1->Vx;
    Sphere1->Vx = Sphere2->Vx;
    Sphere2->Vx = Bufer;

    Bufer = Sphere1->Vy;
    Sphere1->Vy = Sphere2->Vy;
    Sphere2->Vy = Bufer;

}

void CheckColission(Game_object* Sphere, General Game_options)
{

    if (Sphere->x <= Sphere->r or Sphere->x >= Game_options.Screen_height - Sphere->r)
    {
            Sphere->Vx = -Sphere->Vx;
    }

    if (Sphere->y <= Sphere->r or Sphere->y >= Game_options.Screen_width - Sphere->r)
    {
            Sphere->Vy = -Sphere->Vy;
    }

}

void MoveSphere(Game_object* Sphere)
{

    Sphere->x += Sphere->Vx;
    Sphere->y += Sphere->Vy;
    Sphere->light_point_x += Sphere->Vx;
    Sphere->light_point_y += Sphere->Vy;

}

void MoveParticles(Game_object* Particles, General Game_options)
{

   for(int i = 0; i < Game_options.Numb_of_particles; i++)
    {
       for(int j = i + 1; j < Game_options.Numb_of_particles; j++)
       {
           if(CheckColossionTwoSphere(Particles[i], Particles[j]))
            {
                    Colission(&Particles[i],&Particles[j]);
            }
       }
    }

    for(int i = 0; i < Game_options.Numb_of_particles; i++)
    {
       CheckColission(&Particles[i], Game_options);
    }

    for(int i = 0; i < Game_options.Numb_of_particles; i++)
    {
       MoveSphere(&Particles[i]);
    }
}

void DrowSphere(Game_object Sphere)
{

    COLORREF OldFillColor = txGetFillColor();
    COLORREF OldColor = txGetColor();
    for(int i = 0; i <= Sphere.drawing_level; i++)
    {
        txSetFillColor(RGB(Sphere.Red * i / Sphere.drawing_level, Sphere.Green * i / Sphere.drawing_level, Sphere.Blue * i / Sphere.drawing_level));
        txSetColor(RGB(Sphere.Red * i / Sphere.drawing_level, Sphere.Green * i / Sphere.drawing_level ,Sphere.Blue * i / Sphere.drawing_level));
        txCircle(Sphere.x + (Sphere.light_point_x - Sphere.x) * i / Sphere.drawing_level, Sphere.y + (Sphere.light_point_y - Sphere.y) * i / Sphere.drawing_level, Sphere.r - ((Sphere.r * i) / Sphere.drawing_level));

    }
    txSetFillColor(OldFillColor);
    txSetColor(OldColor);

}

void DrowParticles(Game_object* Particles, int Numb_of_particles)
{
    for(int i = 0; i < Numb_of_particles; i++)
    {
       DrowSphere(Particles[i]);
    }

}


int main()
{
    General Game_options;

    Game_options = {1600, 900, 0, 10, 500};

    Game_object* Particles = new Game_object[Game_options.Numb_of_particles];

    for(int i = 0; i < Game_options.Numb_of_particles; i++)
    {
        int x = rand()%(1550 - 20 + 1);
        int y = rand()%(850- 20 + 1);
        Particles[i].x = x;
        Particles[i].y = y;
        Particles[i].light_point_x = x + 2;
        Particles[i].light_point_y = y + 2;
        Particles[i].r = 10;
        Particles[i].Vx = rand()%(10 + 10 + 1);
        Particles[i].Vy = rand()%(10 + 10 + 1);
        Particles[i].Red = 255;
        Particles[i].Blue = 0;
        Particles[i].Green = 0;
        Particles[i].drawing_level = 2;

    }

    txCreateWindow(Game_options.Screen_height, Game_options.Screen_width);
    txSetFillColor(RGB(0,0,0));
    txSetColor(RGB(0,0,0));
    txClear();

    while(!GetAsyncKeyState (VK_ESCAPE))
    {
        txBegin();
        txClear();
        DrowParticles(Particles, Game_options.Numb_of_particles);
        txEnd();
        MoveParticles(Particles, Game_options);

    }
    return 0;
}
