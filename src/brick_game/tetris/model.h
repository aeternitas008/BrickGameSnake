#ifndef BACKEND_H
#define BACKEND_H

#include <locale.h>
#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "defines.h"
#include "../../gui/cli/view.h"

/**
 * @file main.h Файл имеет основные функции по реализации игры, а именно
 * чтение и обработку ключей, перемещение тетромино по полю и реализацию базовой
 * механики игры.
 *
 */


static const unsigned int TETRAMINO_FIGURES[19][4][4] = {
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},

    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},

    {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}},

    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
    {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    // L
    {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
    {{0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    // J 11
    {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}},
    {{0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    {{0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    // T 15
    {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
};


void AddTetraminoOnBoard(Params_t *prms);
void StatsInit(GameInfo_t *stats);

void TetraminoPosInit(Position *tetramino_pos);
void InitBoard(int field[ROWS_MAP][COLS_MAP]);

int CheckNewVariant(Params_t prms);

int CheckTetramino(Params_t prms, Tetramino_t tetramino);

/**
 * @brief Возвращает указатель на статический экземпляр структуры Params_t.
 *
 * Функция инициализирует и возвращает указатель на статический объект Params_t.
 * Используется для работы с параметрами игры в течение всего времени её
 * выполнения.
 *
 * @return Указатель на структуру Params_t.
 */
Params_t *GetPrmsInstance();


/**
 * @brief Заполняет текущую фигуру тетрамино на основе его типа и варианта.
 *
 * Функция извлекает фигуру тетрамино на основе его типа и варианта,
 * и копирует её в поле figure структуры Tetramino_t.
 *
 * @param tetramino Указатель на структуру Tetramino_t, в которую будет
 * скопирована фигура.
 */
void GetTetramino(Tetramino_t *tetramino);

void StartGame(Params_t *prms);
void Spawn(Params_t *prms);
void MoveUp(Params_t *prms);

/**
 * @brief Сдвигает тетрамино вниз, если под ним нет блокирующих элементов.
 *
 * Функция проверяет, можно ли сдвинуть тетрамино вниз. Если можно, тетрамино
 * сдвигается. Если оно достигает препятствия или дна поля, тетрамино
 * добавляется на игровую доску.
 *
 * @param prms Указатель на структуру Params_t с параметрами игры.
 */
void MoveDown(Params_t *prms);

void MoveRight(Params_t *prms);
void MoveLeft(Params_t *prms);
void GameOver(Params_t *prms);
void ExitState(Params_t *prms);
void Check(Params_t *prms);
void TurnRight(Params_t *prms);
void GamePause(Params_t *prms);

/**
 * @brief Обновляет положение тетрамино на поле через определённый интервал
 * времени.
 *
 * Функция вычисляет прошедшее время и сдвигает тетрамино вниз, если прошло
 * достаточно времени с момента последнего обновления. Уровень сложности игры
 * также влияет на скорость падения.
 *
 * @param prms Указатель на структуру Params_t с параметрами игры.
 */
void Shifting(Params_t *prms);

GameInfo_t updateCurrentState();

#endif