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


#include "GameScene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>
#include "GameOver.hpp"


using namespace basics;
using namespace std;

namespace snake
{
    GameScene::GameScene()
    {
        // Dimensiones iniciales del canvas
        canvas_width  = 1280;
        canvas_height =  720;
    }

    bool GameScene::initialize ()
    {
        // Definicion de variables en valores iniciales
        state     = LOADING;
        suspended = false;

        cell_size = 60;
        cell_space = 75;
        cell_column = 8;
        cell_row = 10;

        // Calculo de coordenadas de casillas
        for (int i = 0; i < cell_row; ++i) {
            xx[i] = cell_space * i + 300;
        }
        for (int i = 0; i < cell_column; ++i) {
            yy[i] = cell_space * i + 100;
        }

        // Colocacion de la cabeza en casilla inicial y comida inicial
        head_snake = 45;
        food = rand() % (cell_column * cell_row);

        // Inclusion de la cabeza dentro de la estructura de la serpiente
        it = snake.begin();
        snake.insert(it,head_snake);

        // Fase de movimiento en espera
        phase = WAIT;

        // Direccion inicial
        direction = UP;

        // Tiempo de movimiento (Velocidad invertida)
        wait_time = 0.5;

        return true;
    }

    void GameScene::suspend ()
    {
        // Cambio de estado y activacion del semaforo
        suspended = true;
        state = PAUSE;
    }

    void GameScene::resume ()
    {
        // Cambio de estado
        suspended = false;
    }

    void GameScene::handle (Event & event)
    {
        // Si esta en pausa retoma el juego
        if(state == PAUSE)
        {
            switch (event.id)
            {
                case ID(touch-started):
                {
                    state = RUNNING;
                }
            }
        }


        if (state == RUNNING)
        {
            switch (event.id)
            {
                // Guardado de coordenadas al inicio del input
                case ID(touch-started):
                {
                    x = *event[ID(x)].as< var::Float > ();
                    y = *event[ID(y)].as< var::Float > ();
                }
                case ID(touch-moved):
                case ID(touch-ended):
                {
                    // Nuevas coordenadas del final del input
                    vector_input[0] = *event[ID(x)].as< var::Float > () - x;
                    vector_input[1] = *event[ID(y)].as< var::Float > () - y;

                    // Segun el vector creado entre ambas posiciones se escoge la dirección seleccionada por el jugador
                    if(std::abs(vector_input[0]) > std::abs(vector_input[1]))
                    {
                        if(vector_input[0] > 0 && last_move != LEFT)
                        {
                            direction = RIGHT;
                        }
                        else if (last_move != RIGHT)
                        {
                            direction = LEFT;
                        }
                    }

                    else
                    {
                        if(vector_input[1] > 0 && last_move != DOWN)
                        {
                            direction = UP;
                        }
                        else if(last_move != UP)
                        {
                            direction = DOWN;
                        }
                    }
                    break;
                }
            }
        }
    }

    void GameScene::update (float time)
    {
        // Den fncion del estado se realiza una funcion u otra
        switch (state)
        {
            case LOADING: load ();     break;
            case RUNNING: run  (time); break;
            case PAUSE:                break;
        }
    }

    void GameScene::render (basics::Graphics_Context::Accessor & context)
    {
        // Referencia al canvas
        Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

        // Durante la pausa se uestra una imagen ara indicar que el juego se ha detenido
        if(state == PAUSE)
        {
            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }
            else canvas->fill_rectangle ({ 640, 360 }, { 640, 360 }, pause.get ());
        }
        if (!suspended && state == RUNNING)
        {

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                canvas->clear        ();
                canvas->set_color    (1, 1, 1);


                if (texture && cell && food_cell && head && body)
                {
                    // Fondo
                    canvas->fill_rectangle ({ 640, 360 }, { 1280, 720 }, texture.get ());


                    // Dibujado de las casillas jugables
                    casilla_print = 0;

                    for (int i = 0; i < cell_row; ++i)
                    {
                        for (int j = 0; j < cell_column; ++j)
                        {
                            // Si la casilla pertenece a la serpiente
                            if(std::find(snake.begin(), snake.end(), casilla_print) != snake.end())
                            {
                                // Si la casilla es la cabeza de la serpiente
                                if(casilla_print == head_snake)
                                {
                                    canvas->fill_rectangle ({ xx[i], yy[j] }, { cell_size, cell_size }, head.get ());
                                }
                                // si la casilla es aprte del cuerpo
                                else
                                {
                                    canvas->fill_rectangle ({ xx[i], yy[j] }, { cell_size, cell_size }, body.get ());
                                }
                            }
                            // Si la casilla es la comida activa
                            else if(food == casilla_print)
                            {
                                canvas->fill_rectangle ({ xx[i], yy[j] }, { cell_size, cell_size }, food_cell.get ());

                            }
                            // Si s una casilla vacia
                            else
                            {
                                canvas->fill_rectangle ({ xx[i], yy[j] }, { cell_size, cell_size }, cell.get ());
                            }

                            ++casilla_print;
                        }
                    }
                }
            }
        }
    }

    void GameScene::load ()
    {
        // Carga iterativa de graficos
        if (!suspended)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();
            if (context)
            {
                if(!texture)
                {
                    texture = Texture_2D::create (ID(test), context, "fondo2.png");
                    if(texture) context->add (texture);

                }

                else if(!food_cell)
                {
                    food_cell = Texture_2D::create (ID(food), context, "food.png");
                    if(food_cell) context->add (food_cell);

                }
                else if(!cell)
                {
                    cell = Texture_2D::create (ID(cell), context, "base_cell.png");
                    if(cell) context->add (cell);

                }
                else if(!head)
                {
                    head = Texture_2D::create (ID(head), context, "head.png");
                    if(head) context->add (head);

                }else if(!body)
                {
                    body = Texture_2D::create (ID(body), context, "body.png");
                    if(body) context->add (body);

                }else if(!pause)
                {
                    pause = Texture_2D::create (ID(pause), context, "snake.png");
                    if(pause) context->add (pause);

                }
                else
                {
                    state = RUNNING;
                }
            }
        }
    }

    void GameScene::run (float time)
    {
        // Discrecion de la fase de juego
        switch(phase){
            // Si la fase es de movimiento se cambia la cabeza y luego se actualiza la nueva serpiente
            case MOVE:
                timer = 0;
                last_move = direction;
                //Cambio de cabeza en funcion de la direccion
                switch(direction)
                {
                    case LEFT:
                        if(head_snake >= cell_column)
                        {
                            head_snake -= cell_column;
                        }
                        break;
                    case RIGHT:
                        if(head_snake < cell_row * cell_column - cell_column)
                        {
                            head_snake += cell_column;
                        }
                        break;
                    case UP:
                        if(head_snake % cell_column != cell_column -1)
                        {
                            ++head_snake;
                        }
                        break;
                    case DOWN:
                        if(head_snake % cell_column != 0)
                        {
                            --head_snake;
                        }
                        break;
                }


                /*
                 * Caso de derrota por choque con la propia serpiente
                 *
                 * (en caso de chocar contra el borde al no haber movido la cabeza
                 * esta seguira siendo parte de la antigua serpiente por lo que la derrota se considera igualmente)
                 */

                if(std::find(snake.begin(), snake.end(), head_snake) != snake.end())
                {
                    basics::log.d("Choque serpientes");
                    LostGame();
                }

                //Actualizacion de la serpiente
                else
                {
                    // Añadir la nueva casilla
                    it = snake.begin();
                    snake.insert(it,head_snake);

                    // Si no ha llegado a la comida eliminacion de la ultima parte
                    if(head_snake != food)
                    {
                        snake.pop_back();
                    }
                    // Si se ha alcanzado la comida generacion de nueva comida en una casilla vacia al azar
                    else
                    {
                        while(std::find(snake.begin(), snake.end(), food) != snake.end())
                        {
                            food = rand() % (cell_column * cell_row);
                        }
                    }
                }
                // Cambio de fase a espera
                phase = WAIT;
                break;

            // Si la fase es de espera aumentar el contador hasta que se cambia a la fase de movimiento nuevamente
            case WAIT:
                timer += time;
                if(timer >= wait_time) phase = MOVE;
                break;
        }


    }

    // Funcion de end game
    void GameScene::LostGame()
    {
        director.run_scene (shared_ptr< Scene >(new GameOver()));
    }

}

