#include "picolib.h"
#include "stdio.h"
#include <math.h>
#include <time.h>

#define SAVE_FILE "save.txt"

// INIT
picolib_mouse m;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t spr;
} Cursor;

typedef struct {
    uint64_t pixels;
    uint64_t price;
    int16_t x;
    int16_t y;
    int16_t x2;
    int16_t y2;
    int16_t color;
} Pixel;

Cursor cursor = {
    .x = 0,
    .y = 0,
    .spr = 1
};

Pixel pixWhite = {
    .pixels = 0,
    .price = 0,
    .x = 48,
    .y = 48,
    .x2 = 79,
    .y2 = 79,
    .color = 7
};

Pixel pixGreen = {
    .pixels = 0,
    .price = 15,
    .x = 2,
    .y = 9,
    .x2 = 9,
    .y2 = 16,
    .color = 11
};

Pixel pixRed = {
    .pixels = 0,
    .price = 100,
    .x = 2,
    .y = 26,
    .x2 = 9,
    .y2 = 33,
    .color = 8
};

Pixel pixYellow = {
    .pixels = 0,
    .price = 500,
    .x = 2,
    .y = 43,
    .x2 = 9,
    .y2 = 50,
    .color = 10
};

Pixel pixPink = {
    .pixels = 0,
    .price = 2000,
    .x = 2,
    .y = 60,
    .x2 = 9,
    .y2 = 67,
    .color = 14
};

Pixel pixDarkBlue = {
    .pixels = 0,
    .price = 7000,
    .x = 2,
    .y = 77,
    .x2 = 9,
    .y2 = 84,
    .color = 1
};

Pixel pixDarkPurple = {
    .pixels = 0,
    .price = 50000,
    .x = 2,
    .y = 94,
    .x2 = 9,
    .y2 = 101,
    .color = 2
};

Pixel pixLightPeach = {
    .pixels = 0,
    .price = 1000000,
    .x = 2,
    .y = 111,
    .x2 = 9,
    .y2 = 118,
    .color = 15
};

Pixel pixPico = {
    .pixels = 0,
    .price = 123456789,
    .x = 48,
    .y = 48,
    .x2 = 0,
    .y2 = 0,
    .color = 0
}; // spr(2, pixPico.x, pixPico.y);

/*
    MODE
    0 - Game
    1 - Shop
    2 - Secret Shop
*/
uint8_t mode = 0;
uint8_t max_mode = 1;

// Таймер для производства
int production_timer = 0;

// Глобальные переменные для производства
double production_accumulator = 0.0;
clock_t last_time = 0;
bool first_update = true;
double production_per_second = 0.0;


int color = 2;

void save_game() {
    FILE* f = fopen(SAVE_FILE, "w");
    if (f == NULL) return;

    fprintf(f, "%llu\n", pixWhite.pixels);
    fprintf(f, "%llu\n", pixGreen.pixels);
    fprintf(f, "%llu\n", pixGreen.price);
    fprintf(f, "%llu\n", pixRed.pixels);
    fprintf(f, "%llu\n", pixRed.price);
    fprintf(f, "%llu\n", pixYellow.pixels);
    fprintf(f, "%llu\n", pixYellow.price);
    fprintf(f, "%llu\n", pixPink.pixels);
    fprintf(f, "%llu\n", pixPink.price);
    fprintf(f, "%llu\n", pixDarkBlue.pixels);
    fprintf(f, "%llu\n", pixDarkBlue.price);
    fprintf(f, "%llu\n", pixDarkPurple.pixels);
    fprintf(f, "%llu\n", pixDarkPurple.price);
    fprintf(f, "%llu\n", pixLightPeach.pixels);
    fprintf(f, "%llu\n", pixLightPeach.price);
    fprintf(f, "%llu\n", pixPico.pixels);
    fprintf(f, "%llu\n", pixPico.price);
    fprintf(f, "%f\n", production_accumulator);

    fclose(f);
    sfx(0);
}

void load_game() {
    FILE* f = fopen(SAVE_FILE, "r");
    if (f == NULL) return;

    uint64_t w, g, pr, r, rp, y, yp, p, pp, b, bp, pu, pup, pe, pep, pico, picop;
    double acc;
    // Теперь 17 целых (15 старых + 2 для pixPico) + 1 дробное = 18 аргументов
    if (fscanf(f, "%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%llu\n%lf\n",
               &w, &g, &pr, &r, &rp, &y, &yp, &p, &pp, &b, &bp, &pu, &pup, &pe, &pep, &pico, &picop, &acc) == 18) {
        pixWhite.pixels = w;
        pixGreen.pixels = g;
        pixGreen.price = pr;
        pixRed.pixels = r;
        pixRed.price = rp;
        pixYellow.pixels = y;
        pixYellow.price = yp;
        pixPink.pixels = p;
        pixPink.price = pp;
        pixDarkBlue.pixels = b;
        pixDarkBlue.price = bp;
        pixDarkPurple.pixels = pu;
        pixDarkPurple.price = pup;
        pixLightPeach.pixels = pe;
        pixLightPeach.price = pep;
        pixPico.pixels = pico;
        pixPico.price = picop;
        production_accumulator = acc;
    }
    fclose(f);

    last_time = clock();
    first_update = false;
    sfx(0);
}

void update(void) {
    m = mousep();

    cursor.x = m.x;
    cursor.y = m.y;

    if (pixLightPeach.pixels > 0) max_mode = 2;

    // Хтьбоксы для столкновения
    Rect cursor_rect = { cursor.x, cursor.y, 8, 8} ;
    Rect white_rect = { pixWhite.x, pixWhite.y, 32, 32 };

    Rect green_rect = { pixGreen.x, pixGreen.y, 8, 8 };
    Rect red_rect = { pixRed.x, pixRed.y, 8, 8 };
    Rect yellow_rect = { pixYellow.x, pixYellow.y, 8, 8 };
    Rect pink_rect = { pixPink.x, pixPink.y, 8, 8 };
    Rect darkBlue_rect = { pixDarkBlue.x, pixDarkBlue.y, 8, 8 };
    Rect darkPurple_rect = { pixDarkPurple.x, pixDarkPurple.y, 8, 8 };
    Rect lightPeach_rect = { pixLightPeach.x, pixLightPeach.y, 8, 8 };

    Rect pico_rect = { pixPico.x, pixPico.y, 32, 32 };


    Rect btnSave_rect = { 111, 2, 15, 5 };
    Rect btnLoad_rect = { 111, 25, 15, 5 };

    // --- Вычисляем delta_time (всегда) ---
    clock_t current_time = clock();
    if (first_update) {
        last_time = current_time;
        first_update = false;
    }
    double delta_time = (double)(current_time - last_time) / CLOCKS_PER_SEC;
    last_time = current_time;
    if (delta_time > 0.1) delta_time = 0.1;

    // --- Переключаем окно ---
    if (m.right) mode ++;
    if (mode > max_mode) {
        mode = 0;
    }

    if (mode == 0) {
        if (col_rect(&cursor_rect, &white_rect) && m.left) { sfx(0); pixWhite.pixels++; }

        if (col_rect(&cursor_rect, &btnSave_rect) && m.left) {
            save_game();
        } else if (col_rect(&cursor_rect, &btnLoad_rect) && m.left) {
            load_game();
        }
    } else if (mode == 1) {
        // Зеленый пиксель
        if (col_rect(&cursor_rect, &green_rect) && m.left) {
            sfx(0);
            if (pixWhite.pixels >= pixGreen.price) {
                pixWhite.pixels -= pixGreen.price;  // списываем белые
                pixGreen.pixels++;                  // покупаем один зелёный
                
                // Увеличиваем цену по формуле (как в Cookie Clicker)
                // Цена = базовая цена * 1.1 ^ количество купленных
                // Используем целочисленный расчёт, чтобы избежать float
                // Можно сделать так: price = 15 + (count * 2) (линейный рост),
                // но лучше экспоненциальный:
                pixGreen.price = (uint64_t)(15 * pow(1.1, pixGreen.pixels));
                // Для целых чисел можно использовать умножение на 1.1 с округлением:
                // pixGreen.price = (uint64_t)(pixGreen.price * 1.1);
                // Но это даст погрешность. Проще завести базовую цену и считать по формуле каждый раз.
            }
        }

        // Красный пиксель
        if (col_rect(&cursor_rect, &red_rect) && m.left) {
            sfx(0);
            if (pixWhite.pixels >= pixRed.price) {
                pixWhite.pixels -= pixRed.price;
                pixRed.pixels++;
                pixRed.price = (uint64_t)(100 * pow(1.1, pixRed.pixels));
            }
        }

        // Желтый пиксель
        if (col_rect(&cursor_rect, &yellow_rect) && m.left) {
            sfx(0);
            if (pixWhite.pixels >= pixYellow.price) {
                pixWhite.pixels -= pixYellow.price;
                pixYellow.pixels++;
                pixYellow.price = (uint64_t)(500 * pow(1.1, pixYellow.pixels));
            }
        }

        // Розовый пиксель
        if (col_rect(&cursor_rect, &pink_rect) && m.left) {
            sfx(0);
            if (pixWhite.pixels >= pixPink.price) {
                pixWhite.pixels -= pixPink.price;
                pixPink.pixels++;
                pixPink.price = (uint64_t)(2000 * pow(1.1, pixPink.pixels));
            }
        }

        // Темный синий пиксель
        if (col_rect(&cursor_rect, &darkBlue_rect) && m.left) {
            sfx(0);
            if (pixWhite.pixels >= pixDarkBlue.price) {
                pixWhite.pixels -= pixDarkBlue.price;
                pixDarkBlue.pixels++;
                pixDarkBlue.price = (uint64_t)(7000 * pow(1.1, pixDarkBlue.pixels));
            }
        }

        // Темный фиолетовый пиксель
        if (col_rect(&cursor_rect, &darkPurple_rect) && m.left) {
            sfx(0);
            if (pixWhite.pixels >= pixDarkPurple.price) {
                pixWhite.pixels -= pixDarkPurple.price;
                pixDarkPurple.pixels++;
                pixDarkPurple.price = (uint64_t)(50000 * pow(1.1, pixDarkPurple.pixels));
            }
        }

        // Персиковый (Портал)
        if (col_rect(&cursor_rect, &lightPeach_rect) && m.left) {
            sfx(0);
            if (pixWhite.pixels >= pixLightPeach.price) {
                pixWhite.pixels -= pixLightPeach.price;
                pixLightPeach.pixels++;
                pixLightPeach.price = (uint64_t)(1000000 * pow(1.1, pixLightPeach.pixels));
            }
        }
    // Секретный магазин
    } else if (mode == 2) {
        // pixPico
        if (col_rect(&cursor_rect, &pico_rect) && m.left) {
            sfx(0);
            if (pixWhite.pixels >= pixPico.price) {
                pixWhite.pixels -= pixPico.price;
                pixPico.pixels++;
                pixPico.price = (uint64_t)(123456789 * pow(1.1, pixPico.pixels));
            }
        }
    }
    // --- Логика производства
    production_per_second = pixGreen.pixels * 0.2 +
                        pixRed.pixels * 0.8 +
                        pixYellow.pixels * 4.0 +
                        pixPink.pixels * 10.0 +
                        pixDarkBlue.pixels * 20.0 +
                        pixDarkPurple.pixels * 100.0 +
                        pixLightPeach.pixels * 1333.2 +
                        pixPico.pixels * 24691.2;

    // Накопление времени
    production_accumulator += production_per_second * delta_time;

    // Если накопилось целое число, добавляем белые пиксели
    while (production_accumulator >= 1.0) {
        pixWhite.pixels += 1;
        production_accumulator -= 1.0;
    }
}

void draw(void) {
    cls(12);

    if (mode == 0) {
        // rectfill(111, 2, 125, 16, color);
        print("SAVE", 111, 2, 7);
        // rectfill(111, 25, 125, 41, color);
        print("LOAD", 111, 25, 7);

        rectfill(2, 2, 9, 9, 7);
        print("%llu", 12, 2, 7, pixWhite.pixels);
        print("PIXELS/SEC: %.1f", 2, 121, 7, production_per_second);

        rectfill(pixWhite.x, pixWhite.y, pixWhite.x2, pixWhite.y2, pixWhite.color);
    } else if (mode == 1) {
        rectfill(pixGreen.x, pixGreen.y, pixGreen.x2, pixGreen.y2, pixGreen.color);
        print("$ %llu", pixGreen.x, pixGreen.y-7, 7, pixGreen.price);
        print("%llu", pixGreen.x2+3, pixGreen.y2-7, 7, pixGreen.pixels);

        rectfill(pixRed.x, pixRed.y, pixRed.x2, pixRed.y2, pixRed.color);
        print("$ %llu", pixRed.x, pixRed.y-7, 7, pixRed.price);
        print("%llu", pixRed.x2+3, pixRed.y2-7, 7, pixRed.pixels);

        rectfill(pixYellow.x, pixYellow.y, pixYellow.x2, pixYellow.y2, pixYellow.color);
        print("$ %llu", pixYellow.x, pixYellow.y - 7, 7, pixYellow.price);
        print("%llu", pixYellow.x2 + 3, pixYellow.y2 - 7, 7, pixYellow.pixels);

        rectfill(pixPink.x, pixPink.y, pixPink.x2, pixPink.y2, pixPink.color);
        print("$ %llu", pixPink.x, pixPink.y - 7, 7, pixPink.price);
        print("%llu", pixPink.x2 + 3, pixPink.y2 - 7, 7, pixPink.pixels);

        rectfill(pixDarkBlue.x, pixDarkBlue.y, pixDarkBlue.x2, pixDarkBlue.y2, pixDarkBlue.color);
        print("$ %llu", pixDarkBlue.x, pixDarkBlue.y - 7, 7, pixDarkBlue.price);
        print("%llu", pixDarkBlue.x2 + 3, pixDarkBlue.y2 - 7, 7, pixDarkBlue.pixels);

        rectfill(pixDarkPurple.x, pixDarkPurple.y, pixDarkPurple.x2, pixDarkPurple.y2, pixDarkPurple.color);
        print("$ %llu", pixDarkPurple.x, pixDarkPurple.y - 7, 7, pixDarkPurple.price);
        print("%llu", pixDarkPurple.x2 + 3, pixDarkPurple.y2 - 7, 7, pixDarkPurple.pixels);

        rectfill(pixLightPeach.x, pixLightPeach.y, pixLightPeach.x2, pixLightPeach.y2, pixLightPeach.color);
        print("$ %llu", pixLightPeach.x, pixLightPeach.y - 7, 7, pixLightPeach.price);
        print("%llu", pixLightPeach.x2 + 3, pixLightPeach.y2 - 7, 7, pixLightPeach.pixels);
    } else if (mode == 2) {
        spr_pro(2, pixPico.x, pixPico.y, 4, 4, false, false);
        print("$ %llu", pixPico.x, pixPico.y - 7, 7, pixPico.price);
        print("%llu", pixPico.x, pixPico.y - 7 - 7, 7, pixPico.pixels);
    }

    spr(cursor.spr, cursor.x, cursor.y);
}
