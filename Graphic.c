#ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <Windows.h>


static int _screen_index = 0;
static HANDLE _hScreen[2];

int _width;
int _height;

void init_screen(int width, int height)
{
    CONSOLE_CURSOR_INFO cci;
    _hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    _hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(_hScreen[0], &cci);
    SetConsoleCursorInfo(_hScreen[1], &cci);

    _width = width;
    _height = height;

    COORD coord;
    coord.X = _width;
    coord.Y = _height + 1;

    SMALL_RECT rect;

    rect.Top = 0;
    rect.Left = 0;
    rect.Bottom = _height - 1;
    rect.Right = _width - 1;

    SetConsoleScreenBufferSize(_hScreen[0], coord);
    SetConsoleScreenBufferSize(_hScreen[1], coord);

    SetConsoleWindowInfo(_hScreen[0], TRUE, &rect);
    SetConsoleWindowInfo(_hScreen[1], TRUE, &rect);
}

void flip_screen()
{
    SetConsoleActiveScreenBuffer(_hScreen[_screen_index]);
    _screen_index = !_screen_index;
}

void clear_screen()
{
    DWORD dw;
    COORD coord = { 0, 0 };
    FillConsoleOutputCharacter(_hScreen[_screen_index], ' ', _width * _height, coord, &dw);
}

void print_string(int x, int y, char* str)
{
    DWORD dw;
    COORD coord = { x, y };
    SetConsoleCursorPosition(_hScreen[_screen_index], coord);
    WriteFile(_hScreen[_screen_index], str, strlen(str), &dw, NULL);
}

void set_color(unsigned short text, unsigned short back)
{
    SetConsoleTextAttribute(_hScreen[_screen_index], text | (back << 4));
}

typedef struct _Shape {
    int width;
    int height;
    const char** char_map;
} Shape;

typedef struct _Object {
    int x;
    int y;
    byte color;
    DWORD dw_last_moved;
    Shape* shape;
} Object;

Object** create_object_list(int list_size)
{
    Object** list = (Object**)malloc(sizeof(Object*) * list_size);
    return list;
}

Object* create_object(int x, int y, byte color, Shape* shape)
{
    Object* obj = (Object*)malloc(sizeof(Object));
    obj->x = x;
    obj->y = y;
    obj->color = color;
    obj->shape = shape;
    return obj;
}

void free_object(Object* obj)
{
    free(obj);
}

void move_object(Object* obj, int x_step, int y_step)
{
    obj->x += x_step;
    obj->y += y_step;
}

void draw_object(Object* obj)
{
    set_color(obj->color, 0);
    for (int i = 0; i < obj->shape->height; i++)
    {
        print_string(obj->x, obj->y + i, obj->shape->char_map[i]);
    }
}
