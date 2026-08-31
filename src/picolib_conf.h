#ifndef PICOLIB_CONF_H
#define PICOLIB_CONF_H

// Настройки по умолчанию (пользователь может отредактировать)
#define PICOLIB_WIDTH   128
#define PICOLIB_HEIGHT  128
#define PICOLIB_TITLE   "PICOLIB CLICKER"
#define PICOLIB_FPS     30


// Путь и имя файла спрайт-листа (должен быть 128x128 пикселей для 256 спрайтов 8x8)
#define PICOLIB_SS      "assets\\picolib_spritesheet.png"


// Шрифт
#define PICOLIB_FONT    "assets\\font_pico8.png"

#define PICOLIB_MAX_SOUNDS      64                 // Максимум звуков, которые можно загрузить
#define PICOLIB_USE_AUDIO       1                  // 1 включить звук для игры, 0 отключить звук
#define PICOLIB_SOUNDS_PATH     "assets\\sounds/%d.wav"    // Путь к звуковым файлам


// Мышка
#define PICOLIB_USE_MOUSE       1


// Сохранение и загрузка
#define PICOLIB_USE_SAVE        1
#define PICOLIB_SAVE_SLOTS      64
#define PICOLIB_SAVE_FILE       "storage.bin"


// Карта
#define PICOLIB_USE_MAP         1

// Размеры карты
#define SPRITE_COUNT 256
#define MAP_ROWS     32
#define MAP_COLS     128

// Путь к файлу карты (если используется)
#define PICOLIB_MAP_FILE "map.csv"

#endif
