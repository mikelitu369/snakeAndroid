/*
 * GAME OVER SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 * Modified by Mikel Iturralde (Copyright © 2020+)
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 * rexitah@gmail.com
 */

#include "GameOver.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>
#include "Title.hpp"

using namespace basics;
using namespace std;

namespace snake
{

    GameOver::GameOver()
    {
        canvas_width  = 1280;
        canvas_height =  720;
    }

    bool GameOver::initialize ()
    {
        state     = LOADING;
        suspended = false;
        x         = 640;
        y         = 360;

        return true;
    }

    void GameOver::suspend ()
    {
        suspended = true;
    }

    void GameOver::resume ()
    {
        suspended = false;
    }

    void GameOver::handle (Event & event)
    {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started):
                    director.run_scene (shared_ptr< Scene >(new Title()));
                    break;
                case ID(touch-moved):
                    break;
                case ID(touch-ended):
                    break;
            }
        }
    }

    void GameOver::update (float time)
    {
        switch (state)
        {
            case LOADING: load ();     break;
            case RUNNING: run  (time); break;
        }
    }

    void GameOver::render (basics::Graphics_Context::Accessor & context)
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
                if (texture)
                {
                    canvas->fill_rectangle ({ x, y }, { 1280, 720 }, texture.get ());
                }
            }
        }
    }

    void GameOver::load ()
    {
        if (!suspended)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                texture = Texture_2D::create (ID(test), context, "GameOver.png");

                if (texture)
                {
                    context->add (texture);

                    state = RUNNING;
                }
            }
        }
    }

    void GameOver::run (float time)
    {
    }

}
