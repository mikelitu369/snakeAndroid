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

#include <memory>
#include <basics/Scene>
#include <basics/Texture_2D>


namespace snake
{

    class Title : public basics::Scene
    {

        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;

    public:

        enum State
        {
            LOADING,
            RUNNING,
        };

        State          state;
        bool           suspended;

        unsigned       canvas_width;
        unsigned       canvas_height;

        Texture_Handle texture, fondo, help;
        float          x, y;

    public:

        Title();

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override;
        void suspend    () override;
        void resume     () override;

        void handle     (basics::Event & event) override;
        void update     (float time) override;
        void render     (basics::Graphics_Context::Accessor & context) override;

    private:

        void load ();
        void run  (float time);

    };

}