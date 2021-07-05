/*
 * TITLE SCENE
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
#include "GameScene.hpp"
#include "Tuto.hpp"

using namespace basics;
using namespace std;

namespace snake
{

    Title::Title()
    {
        canvas_width  = 1280;
        canvas_height =  720;
    }

    bool Title::initialize ()
    {
        state     = LOADING;
        suspended = false;
        x         = 640;
        y         = 360;

        return true;
    }

    void Title::suspend ()
    {
        suspended = true;
    }

    void Title::resume ()
    {
        suspended = false;
    }

    void Title::handle (Event & event)
    {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started):
                    x = *event[ID(x)].as< var::Float > ();
                    y = *event[ID(y)].as< var::Float > ();

                    if(x < 120 && y < 120) director.run_scene (shared_ptr< Scene >(new Tuto()));
                    else director.run_scene (shared_ptr< Scene >(new GameScene()));
                    break;
                case ID(touch-moved):
                    break;
                case ID(touch-ended):
                    break;
            }
        }
    }

    void Title::update (float time)
    {
        switch (state)
        {
            case LOADING: load ();     break;
            case RUNNING: run  (time); break;
        }
    }

    void Title::render (basics::Graphics_Context::Accessor & context)
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
                if (texture && fondo && help)
                {
                    canvas->fill_rectangle ({ 640, 360 }, { 1280, 720 }, fondo.get ());
                    canvas->fill_rectangle ({ 640, 360 }, { 256, 144 }, texture.get ());
                    canvas->fill_rectangle ({ 50, 50 }, { 100, 100 }, help.get ());
                }
            }
        }
    }

    void Title::load ()
    {
        if (!suspended)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                if(!texture)
                {
                    texture = Texture_2D::create (ID(test), context, "snake.png");
                    if(texture) context->add (texture);
                }

                else if(!fondo)
                {
                    fondo = Texture_2D::create (ID(fondo), context, "fondo.png");
                    if(fondo) context->add (fondo);
                }
                else if(!help)
                {
                    help = Texture_2D::create (ID(help), context, "help.png");
                    if(help) context->add (help);
                }
                else
                {
                    state = RUNNING;
                }
            }
        }
    }

    void Title::run (float time)
    {
    }

}