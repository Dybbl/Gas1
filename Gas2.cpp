#include "TXlib.h"
#include <cstdlib>
#include <iostream>
#include <cassert>
#include "math.h"

struct General
{
    double Screen_height;
    double Screen_width;

    int Numb_of_particles;
};

class Sphere
{
    double Screen_height = 1600;
    double Screen_width = 900;

    public:
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

            bool CheckColossionTwoSphere(Sphere Sphere)
            {
                if((pow((x - Sphere.x),2)) + pow((y - Sphere.y),2) <= pow((r + Sphere.r),2))
                {
                    return true;
                }
            }

            void ColissionSpheres(Sphere* Sphere)
            {
                double Bufer = 0;

                Bufer = Vx;
                Vx = Sphere->Vx;
                Sphere->Vx = Bufer;

                Bufer = Vy;
                Vy = Sphere->Vy;
                Sphere->Vy = Bufer;
            }

            void ColissionWalls()
            {
                if (x <= r or x >= Screen_height - r)
                {
                       Vx = -Vx;
                }

                if (y <= r or y >= Screen_width - r)
                {
                        Vy = -Vy;
                }
            }

            void Move()
            {
                x += Vx;
                y += Vy;
                light_point_x += Vx;
                light_point_y += Vy;
            }

            void Draw()
            {
                COLORREF OldFillColor = txGetFillColor();
                COLORREF OldColor = txGetColor();
                for(int i = 0; i <= drawing_level; i++)
                {
                    txSetFillColor(RGB(Red * i / drawing_level, Green * i / drawing_level, Blue * i / drawing_level));
                    txSetColor(RGB(Red * i / drawing_level, Green * i / drawing_level ,Blue * i / drawing_level));
                    txCircle(x + (light_point_x - x) * i / drawing_level, y + (light_point_y - y) * i / drawing_level, r - ((r * i) / drawing_level));
                }
                txSetFillColor(OldFillColor);
                txSetColor(OldColor);
            }
};


void MoveParticles(Sphere* Particles, General Options)
{
   for(int i = 0; i < Options.Numb_of_particles; i++)
    {
       for(int j = i + 1; j < Options.Numb_of_particles; j++)
       {
           if(Particles[i].CheckColossionTwoSphere(Particles[j]))
            {
                    Particles[i].ColissionSpheres(&Particles[j]);
            }
       }
    }

    for(int i = 0; i < Options.Numb_of_particles; i++)
    {
       Particles[i].ColissionWalls();
    }

    for(int i = 0; i < Options.Numb_of_particles; i++)
    {
       Particles[i].Move();
    }
}

void DrawParticles(Sphere* Particles, int Numb_of_particles)
{

    for(int i = 0; i < Numb_of_particles; i++)
    {
       Particles[i].Draw();
    }
}


int main()
{
    General Options;

    Options = {1600, 900, 500};

    Sphere* Particles = new Sphere[Options.Numb_of_particles];

    for(int i = 0; i < Options.Numb_of_particles; i++)
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

    txCreateWindow(Options.Screen_height, Options.Screen_width);
    txSetFillColor(RGB(0,0,0));
    txSetColor(RGB(0,0,0));
    txClear();

    while(!GetAsyncKeyState (VK_ESCAPE))
    {
        txBegin();
        txClear();
        DrawParticles(Particles, Options.Numb_of_particles);
        txEnd();
        MoveParticles(Particles, Options);
    }
    return 0;
}
