#include "Graphic.h"
#include "shapes.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int g_screen_width = 150;
const int g_screen_height = 40;

Object** g_snow_list[100];

int g_snow_list_length;
int g_snow_list_idx = 0;

DWORD g_dw_last_snowed = 0;

int random_range(int start, int end)
{
    return rand() % (end + 1 - start) + start;
}


void pull_object_list(Object** list, int start_idx, int end_idx)
{
    for (int i = start_idx; i < end_idx - 1; i++)
        list[i] = list[i + 1];
}

void create_snow()
{
    if (g_snow_list_idx >= g_snow_list_length) return;
    int snow_list_idx = g_snow_list_idx;
    for (int i = snow_list_idx; i < snow_list_idx + 5; i++)
    {
        if (g_dw_last_snowed && GetTickCount() - g_dw_last_snowed < 2000) return;
        int pos_x = random_range(0, g_screen_width - 1);
        Object* snow = create_object(pos_x, 0, 15, &shape_snow);
        snow->dw_last_moved = GetTickCount();
        g_snow_list[i] = snow;
        g_snow_list_idx++;
    }
    g_dw_last_snowed = GetTickCount();
}

void move_snow()
{
    for (int i = 0; i < g_snow_list_idx; i++)
    {
        Object* snow = g_snow_list[i];
        if (GetTickCount() - snow->dw_last_moved < 1000) continue;
        short direction_x = random_range(-1, 1);
        if (snow->y + 1 >= g_screen_height || snow->x + direction_x < 0 || snow->x + direction_x >= g_screen_width)
        {
            free_object(snow);
            pull_object_list(g_snow_list, i, g_snow_list_idx);
            i--;
            g_snow_list_idx--;
            continue;
        }
        move_object(snow, direction_x, 1);
        snow->dw_last_moved = GetTickCount();
    }
}

void draw_snow()
{
    for (int i = 0; i < g_snow_list_idx; i++)
    {
        draw_object(g_snow_list[i]);
    }
}

int main(void) {
    srand(time(NULL));
    init_screen(g_screen_width, g_screen_height);
    Object* obj_merry_christmas = create_object(23, 33, 12, &shape_merry_christmas);
    Object* obj_snowman = create_object(100, 15, 15, &shape_snowman);
    Object* obj_tree = create_object(5, 13, 10, &shape_tree);
    Object* obj_slide = create_object(80, 3, 15, &shape_slide);
    g_snow_list_length = 100;
    while (1)
    {
        clear_screen();
        create_snow();
        move_snow();
        draw_object(obj_snowman);
        draw_object(obj_merry_christmas);
        draw_object(obj_tree);
        draw_object(obj_slide);
        draw_snow();
        flip_screen();
    }
    return 0;
}
