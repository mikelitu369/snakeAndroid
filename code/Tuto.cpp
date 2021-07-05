/*
 * TUTORIAL SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 * Modified by Mikel Iturralde (Copyright © 2020+)
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 * rexitah@gmail.com
 */

#include "Title.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>
#include "Tuto.hpp"

using namespace basics;
using namespace std;

namespace snake
{

    Tuto::Tuto()
    {
        canvas_width  = 1280;
        canvas_height =  720;
    }

    bool Tuto::initialize ()
    {
        state     = LOADING;
        suspended = false;
        x         = 640;
        y         = 360;
        step      = 0;

        return true;
    }

    void Tuto::suspend ()
    {
        suspended = true;
    }

    void Tuto::resume ()
    {
        suspended = false;
    }

    void Tuto::handle (Event & event)
    {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started):
                    if(step < 2) ++step;
                    else director.run_scene (shared_ptr< Scene >(new Title()));
                    break;
                case ID(touch-moved):
                    break;
                case ID(touch-ended):
                    break;
            }
        }
    }

    void Tuto::update (float time)
    {
        switch (state)
        {
            case LOADING: load ();     break;
            case RUNNING: run  (time); break;
        }
    }

    void Tuto::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended && state == RUNNING)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                if (fondo && tuto1 && tuto2 && tuto3)
                {
                    canvas->fill_rectangle ({ x, y }, { 1280, 720 }, fondo.get ());
                    switch(step)
                    {
                        case 0:
                            canvas->fill_rectangle ({ x, y }, { 512, 512 }, tuto1.get ());
                            break;
                        case 1:
                            canvas->fill_rectangle ({ x, y }, { 512, 512 }, tuto2.get ());
                            break;
                        case 2:
                            canvas->fill_rectangle ({ x, y }, { 512, 512 }, tuto3.get ());
                            break;
                    }
                }
            }
        }
    }

    void Tuto::load ()
    {
        if (!suspended)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                if(!tuto1)
                {
                    tuto1 = Texture_2D::create (ID(tuto1), context, "tuto1.png");
                    if(tuto1) context->add (tuto1);
                }

                else if(!fondo)
                {
                    fondo = Texture_2D::create (ID(fondo), context, "fondo.png");
                    if(fondo) context->add (fondo);
                }
                else if(!tuto2)
                {
                    tuto2 = Texture_2D::create (ID(tuto2), context, "tuto2.png");
                    if(tuto2) context->add (tuto2);
                }
                else if(!tuto3)
                {
                    tuto3 = Texture_2D::create (ID(tuto3), context, "tuto3.png");
                    if(tuto3) context->add (tuto3);
                }
                else
                {
                    state = RUNNING;
                }
            }
        }
    }

    void Tuto::run (float time)
    {
    }

}