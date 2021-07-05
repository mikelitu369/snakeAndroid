/*
 * GAME SCENE
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

    class GameScene : public basics::Scene
    {

        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;

        //Lista de posibles estados de la escena
        enum State
        {
            LOADING,
            RUNNING,
            PAUSE
        };

        // Lista de estados propios del propio gameplay, referentes a las etapas de
        enum Game_State
        {
            MOVE,
            WAIT
        };

        // Lista ed direcciones que puede llevar la serpiente
        enum Direction
        {
            UP,
            DOWN,
            RIGHT,
            LEFT
        };


    public:


        // Variables destinadas al estado de la escena
        State               state;
        bool                suspended;

        // Variables referentes a las dimensiones del canvas
        unsigned            canvas_width;
        unsigned            canvas_height;

        // Texturas
        Texture_Handle      texture, cell, food_cell, head, body, pause;

        // Variables de coordenadas del input
        float               x, y;
        float               vector_input[2];

        // Variables referentes a las propiedades del area de juego
        float               cell_size, cell_space;
        unsigned            cell_column, cell_row;

        // Coordenadas de las celdas
        float               xx[10], yy[10];

        // Casillas destacadas
        unsigned            casilla_print;
        unsigned            head_snake, food;

        // Estados del juego
        Game_State          phase;

        // Gestores de tiempo
        float               wait_time, timer;

        // Estructura de la serpiente
        std::list<unsigned> snake;
        std::list<unsigned>::iterator it;

        // Horientacion actual del snake
        Direction           direction, last_move;



    public:

        GameScene();

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

        void LostGame   ();

    private:

        void load ();
        void run  (float time);

    };

}