/******************************************************************************
PROGRAM NAME :
AUTHER  : masatsugu Kitadai
DATE    :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
// #include "00_dft.c"

// 円周率の定義
// double pi = 4 * atan(1.0);

char filename_read[100];
char filename_dat[100];
char filename_csv[100];

FILE *fp, *fp_csv, *fp_dat, *gp;

/*********************************   MAIN   *********************************/

int calculate_lift_theory(char date[], int range)
{
    /*****************************************************************************/
    // ディレクトリの作成
    char directoryname_csv[100];
    char directoryname_dat[100];

    sprintf(directoryname_dat, "../result/%s/dat/27-4_dft-lift-theory", date);
    sprintf(directoryname_csv, "../result/%s/csv/27-4_dft-lift-theory", date);

    mkdir(directoryname_dat, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IXOTH);
    mkdir(directoryname_csv, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IXOTH);

    // ファイルの指定
    char filename_read[100];
    char filename_csv[100];
    char filename_dat[100];

    sprintf(filename_read, "../result/%s/csv/20_adjust-value/20.csv", date);
    sprintf(filename_csv, "../result/%s/csv/27-4_dft-lift-theory/27-4.csv", date);
    sprintf(filename_dat, "../result/%s/dat/27-4_dft-lift-theory/27-4.dat", date);

    /*****************************************************************************/

    // sin波の配列
    double angle[3600];
    double wave_lift[3600];

    // 変数の作成
    double value[range], value_i[range];

    // ファイルの読み込み (dat データ) ・格納

    int i;
    double ch0, ch1, ch2;

    for (i = 0; i < range; i++)
    {
        value[i] = 0;
        value_i[i] = 0;
    }

    fp = fopen(filename_read, "r");

    if (fp == NULL)
    {
        printf("Error! I can't open the file.\n");
        exit(0);
    }

    // printf("check\n");

    i = 0;

    while ((fscanf(fp, "%lf,%lf,%lf", &ch0, &ch1, &ch2)) != EOF)
    {
        angle[i] = ch0;
        wave_lift[i] = ch2;
        i = i + 1;
    }

    fclose(fp);

    int split = 3600 / range;

    /*****************************************************************************/

    // ファイルの読み込み (dat データ) ・格納

    int buf;
    int count = 0;

    for (i = 0; i < range; i++)
    {
        value[i] = 0;
        value_i[i] = 0;
    }

    for (i = 0; i < range; i++)
    {
        count = i * split;
        value[i] = wave_lift[count];
        // printf("angle =\t%d\tvalue[%d] =\t%.3f\n", count, i, value[i]);
    }

    // dftの適用

    double ps, as, dt;
    int fq;
    dt = 1;

    S_dft(value, value_i, range, 1);

    fp_csv = fopen(filename_csv, "w");
    fp_dat = fopen(filename_dat, "w");

    for (i = 0; i < range; i++)
    {
        ps = value[i] * value[i] + value_i[i] * value_i[i];       /* パワースペクトル  */
        as = sqrt(value[i] * value[i] + value_i[i] * value_i[i]); /* 振幅スペクトル  */
        // fq = (double)i / (dt * (double)range);
        fq = i;
        fprintf(fp_csv, "%d,%lf,%lf,%lf\n", fq, ps, value[i], value_i[i]);
        fprintf(fp_dat, "%d\t%lf\t%lf\t%lf\n", fq, ps, value[i], value_i[i]);
        // printf("[%d]\tvalue_Re = %.3f \tvalue_Im = %.3f\t pw: %.3f\tfq :%d\n", i, value[i], value_i[i], ps, fq);
    }

    fclose(fp_csv);
    fclose(fp_dat);

    // printf("\n");

    /*****************************************************************************/
    // Gnuplot //

    // ディレクトリの作成
    char directoryname_plot[100];

    sprintf(directoryname_plot, "../result/%s/plot/27", date);

    mkdir(directoryname_plot, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IXOTH);

    char filename_plot_1[100];
    char filename_plot_2[100];
    char filename_dat_2[100];

    sprintf(filename_dat, "../result/%s/dat/27-4_dft-lift-theory/27-4.dat", date);
    sprintf(filename_dat_2, "../result/%s/dat/07-4_dft-lift/07-4.dat", date);

    sprintf(filename_plot_1, "../result/%s/plot/27/27-4_dft-lift.png", date);
    sprintf(filename_plot_2, "../result/%s/plot/27/27-4_dft-lift_summary.png", date);

    /*****************************************************************************/

    // range x
    double x_min = 0;
    int x_max = range / 2;

    // range y
    double y_min = 0;
    double y_max = 100;

    // label
    char label[100] = "DFT";
    char xxlabel[100] = "Number of waves [-]";
    char yylabel[100] = "Power [-]";

    double size;

    // size
    size = 1;

    /*****************************************************************************/

    if ((gp = popen("gnuplot", "w")) == NULL)
    {
        printf("gnuplot is not here!\n");
        exit(0); // gnuplotが無い場合、異常ある場合は終了
    }

    fprintf(gp, "set terminal pngcairo enhanced font 'Times New Roman,15' \n");

    fprintf(gp, "set output '%s'\n", filename_plot_1);
    // fprintf(gp, "set multiplot\n");
    fprintf(gp, "set key left top\n");
    fprintf(gp, "set key font ',22'\n");
    fprintf(gp, "set term pngcairo size 1280, 960 font ',27'\n");
    // fprintf(gp, "set size ratio %.3f\n", size);

    fprintf(gp, "set lmargin screen 0.10\n");
    fprintf(gp, "set rmargin screen 0.90\n");
    fprintf(gp, "set tmargin screen 0.90\n");
    fprintf(gp, "set bmargin screen 0.15\n");

    fprintf(gp, "set xrange [%.3f:%d]\n", x_min, x_max);
    fprintf(gp, "set xlabel '%s'offset 0.0,0\n", xxlabel);
    fprintf(gp, "set yrange [%.3f:%.3f]\n", y_min, y_max);
    fprintf(gp, "set ylabel '%s'offset 1,0.0\n", yylabel);
    fprintf(gp, "set title '%s (lift)'\n", label);

    // fprintf(gp, "set samples 10000\n");
    fprintf(gp, "plot '%s' using 1:2 with lines lc 'black' notitle\n", filename_dat);
    fflush(gp); // Clean up Data

    /*****************************************************************************/

    fprintf(gp, "set output '%s'\n", filename_plot_2);
    // fprintf(gp, "set multiplot\n");
    fprintf(gp, "set key left top\n");
    fprintf(gp, "set key font ',22'\n");
    fprintf(gp, "set term pngcairo size 1280, 960 font ',27'\n");
    // fprintf(gp, "set size ratio %.3f\n", size);

    fprintf(gp, "set lmargin screen 0.10\n");
    fprintf(gp, "set rmargin screen 0.90\n");
    fprintf(gp, "set tmargin screen 0.90\n");
    fprintf(gp, "set bmargin screen 0.15\n");

    fprintf(gp, "set xrange [%.3f:%d]\n", x_min, x_max);
    fprintf(gp, "set xlabel '%s'offset 0.0,0\n", xxlabel);
    fprintf(gp, "set yrange [%.3f:%.3f]\n", y_min, y_max);
    fprintf(gp, "set ylabel '%s'offset 1,0.0\n", yylabel);
    fprintf(gp, "set title '%s (Lift)'\n", label);

    // fprintf(gp, "set samples 10000\n");
    fprintf(gp, "plot '%s' using 1:2 with lines lc 'red' lw 5 title 'Measured', '%s' using 1:2 with lines lc 'orange' lw 5 title 'Theoretical'\n", filename_dat_2, filename_dat);
    fflush(gp); // Clean up Data

    fprintf(gp, "exit\n"); // Quit gnuplot

    pclose(gp);

    printf("27-4\t\tsuccess\n");
}

// int main()
// {
//     calculate_lift_theory("test-dft", 16);

//     return (0);
// }