/******************************************************************************
PROGRAM NAME :plot.c
AUTHER  : Masatsugu Kitadai
DATE    :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "files/fp.h"

// 移動平均 範囲
int range_ma = 11;

// range x
int x_min = -50;
int x_max = 750;

// label
const char *xxlabel = "Time [s]";
const char *yylabel = "Output voltage [V]";
char label[100];

double size;
FILE *gp;

/*********************************   gnuplot   *********************************/
int plot_ma(char name[], char date[], char label_name[])
{
#include "files/moving_average.h"

    // 移動平均
    sprintf(label, "%s  Moving average (n=%d)", label_name, range_ma);

    // size
    size = 1;

    if ((gp = popen("gnuplot", "w")) == NULL)
    {
        printf("gnuplot is not here!\n");
        exit(0); // gnuplotが無い場合、異常ある場合は終了
    }

#include "plot/01.h"
// #include "plot/02.h"
// #include "plot/03.h"
#include "plot/04.h"
#include "plot/05.h"
    // #include "plot/06.h"

    fprintf(gp, "exit\n"); // Quit gnuplot

    pclose(gp);
}

int main()
{
    // y軸範囲の設定に注意!!

    plot_ma("C1", "210806", "C1");
    plot_ma("Groove_A", "210806", "Groove A");
    plot_ma("Groove_B", "210806", "Groove B");
    plot_ma("Groove_C", "210806", "Groove C");
    plot_ma("Groove_D", "210806", "Groove D");
    plot_ma("Normal", "210806", "Normal");
    plot_ma("R1_17.9", "210806", "R1 17.9");
    plot_ma("R1_18.6", "210806", "R1 18.6");
    plot_ma("R1_19.3", "210806", "R1 19.3");

    return (0);
}
