#ifndef FRONTEND_H
#define FRONTEND_H

#include <locale.h>
#include <unistd.h>
#include <wchar.h>
#include "../../brick_game/tetris/model.hpp"


/**
 * @file frontend.h Файл имеет основные функции для отрисовки поля игры,
 * тетрамино и статистики
 *
 */

/**
 * @brief Отображает основной интерфейс игры, включая игровое поле и HUD.
 *
 * Включает отображение максимального счета, текущего счета, уровня, времени
 * и тетромино, которое будет следовать.
 */
void PrintOverlay(void);

/**
 * @brief Рисует прямоугольник на экране с заданными координатами.
 *
 * @param top_y Координата Y верхней границы.
 * @param bottom_y Координата Y нижней границы.
 * @param left_x Координата X левой границы.
 * @param right_x Координата X правой границы.
 */
void PrintRectangle(int top_y, int bottom_y, int left_x, int right_x);

/**
 * @brief Выводит текущие игровые статистики на экран.
 *
 * @param stats Указатель на структуру с информацией об игре (GameInfo_t),
 * включающей максимальный счет, текущий счет и уровень.
 */
void PrintStats(GameInfo_t stats);

/**
 * @brief Печатает игровое поле на экране.
 *
 * Анимация постепенного заполнения поля используется для эффекта.
 */
void PrintField();

/**
 * @brief Печатает игровое поле (Board_t) на экране, отображая блоки и пустые
 * клетки.
 *
 * @param map Структура игрового поля (Board_t), которая хранит состояние
 * клеток.
 */
void PrintBoard(int field[ROWS_MAP][COLS_MAP]);

/**
 * @brief Отображает сообщение об окончании игры с итоговым счетом.
 *
 * @param stats Указатель на структуру с информацией об игре (GameInfo_t),
 * включающей текущий счет.
 */
void PrintGameOver(GameInfo_t stats);

/**
 * @brief Отображает сообщение о паузе.
 */
void PrintPause();

/**
 * @brief Очищает сообщение о паузе.
 */
void ClearPause();
void PrintTime(int realtime[2]);

void PrintNextTetramino(int figure[4][4]);

void UpdateView(GameInfo_t game_info);

// /**
//  * @brief Перечисление направлений движения змейки.
//  */
// enum Direction_t {
//   UP_DIRECTION = 0, /**< Начало игры */
//   RIGHT_DIRECTION,    /**< Фигура движется */
//   DOWN_DIRECTION,  /**< Сдвиг или поворот фигуры */
//   LEFT_DIRECTION,     /**< Появление новой фигуры */
// };

// /**
//  * @brief Структура, представляющая координаты на игровом поле.
//  */
// struct Position_t{
//   int x, y; /**< Координаты */
// };

// struct Snake_t {
//   std::deque<Position_t> points; /**< Координаты точек змейки */
//   Direction_t direction; /**< Направление движения змейки (0-3) */
// };

// void PrintSnake(Snake_t snake);


#endif