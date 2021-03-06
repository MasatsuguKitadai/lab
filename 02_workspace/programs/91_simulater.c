/******************************************************************************
PROGRAM NAME : sin_wave_maker.c
AUTHER  : masatsugu Kitadai
DATE    :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>

FILE *fp, *fp_csv, *fp_dat, *gp;

// 円周率の定義
// #define pi 4 * atan(1.0)

/*********************************   MAIN   *********************************/
int simulater(char date[], double Theta_1, double Theta_2)
{
    // ファイルの指定
    char filename_csv_1[100];
    char filename_csv_2[100];
    char filename_csv_3[100];

    char filename_dat_1[100];
    char filename_dat_2[100];
    char filename_dat_3[100];

    sprintf(filename_csv_1, "../result/%s/csv/wave/wave.csv", date);
    sprintf(filename_dat_1, "../result/%s/dat/wave/wave.dat", date);

    sprintf(filename_csv_2, "../result/%s/csv/average/average.csv", date);
    sprintf(filename_dat_2, "../result/%s/dat/average/average.dat", date);

    sprintf(filename_csv_3, "../result/%s/csv/dft/dft.csv", date);
    sprintf(filename_dat_3, "../result/%s/dat/dft/dft.dat", date);

    /*****************************************************************************/

    int range = 24;
    int i = 0;
    int j = 0;
    double buf, angle;
    int angle_int;

    // sin波の配列
    double wave_net[3900];
    double wave_drag_1[3900];
    double wave_drag_2[3900];
    double wave_drag_sum[3900];
    double wave_lift_1[3900];
    double wave_lift_2[3900];
    double wave_lift_sum[3900];

    double theta[2];
    double phi[3900];

    double sum[3];
    double ave[3];

    for (i = 0; i < 3; i++)
    {
        sum[i] = 0;
        ave[i] = 0;
    }

    /*****************************************************************************/

    for (i = 0; i < 3900; i++)
    {
        buf = i;
        phi[i] = pi / 1800 * (i - 300);
        theta[0] = pi / 180 * (Theta_1);
        theta[1] = pi / 180 * (Theta_2);

        wave_drag_1[i] = -0.5 * cos(phi[i]);
        wave_drag_2[i] = -0.5 * cos(phi[i] + theta[0]);
        wave_drag_sum[i] = wave_drag_1[i] + wave_drag_2[i];

        wave_lift_1[i] = -0.5 * sin(phi[i]);
        wave_lift_2[i] = -0.5 * sin(phi[i] + theta[1]);
        wave_lift_sum[i] = wave_lift_1[i] + wave_lift_2[i];

        wave_net[i] = sqrt(wave_drag_sum[i] * wave_drag_sum[i] + wave_lift_sum[i] * wave_lift_sum[i]);
    }

    /*****************************************************************************/

    // double degree = 0;

    // for (i = 0; i < 3900; i++)
    // {
    //     sum[0] = sum[0] + wave_drag_sum[i];
    //     sum[1] = sum[1] + wave_lift_sum[i];
    //     sum[2] = sum[2] + wave_net[i];

    //     ave[0] = sum[0] / 3900;
    //     ave[1] = sum[1] / 3900;
    //     ave[2] = sum[2] / 3900;
    // }

    // fp_csv = fopen(filename_csv_1, "w");
    // fp_dat = fopen(filename_dat_1, "w");

    // for (i = 0; i < 3900; i++)
    // {
    //     buf = i;
    //     angle = (buf - 300) / 10;

    //     fprintf(fp_csv, "%.1f,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", angle, wave_drag_1[i], wave_drag_2[i], wave_drag_sum[i], wave_lift_1[i], wave_lift_2[i], wave_lift_sum[i], wave_net[i]);
    //     fprintf(fp_dat, "%.1f\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", angle, wave_drag_1[i], wave_drag_2[i], wave_drag_sum[i], wave_lift_1[i], wave_lift_2[i], wave_lift_sum[i], wave_net[i]);
    // }

    // fclose(fp_csv);
    // fclose(fp_dat);

    // /*****************************************************************************/

    // fp_csv = fopen(filename_csv_2, "w");
    // fp_dat = fopen(filename_dat_2, "w");

    // fprintf(fp_csv, "%.lf,%.lf,%.lf\n", ave[0], ave[1], ave[2]);
    // fprintf(fp_dat, "-30\t%.1f\t%lf\t%lf\n", ave[0], ave[1], ave[2]);
    // fprintf(fp_dat, "360\t%.1f\t%lf\t%lf\n", ave[0], ave[1], ave[2]);

    // fclose(fp_csv);
    // fclose(fp_dat);

    /*****************************************************************************/

    // printf("\n【Net voltage】\n\n");

    double wave_net_2[24];

    for (i = 0; i < 24; i++)
    {
        buf = i * 150;
        angle = buf / 10;
        angle_int = angle;
        wave_net_2[i] = wave_net[angle_int * 10];

        // printf("[%d]\t%d\t%.3f\n", i, angle_int, wave_net_2[i]);
    }

    /*****************************************************************************/

    // printf("\n---------------------------------------------------------------------------\n\n");
    // printf("\n【DFT】\n");

    double ps[24], as;
    double value_i[24];
    int fq;

    // Drag

    S_dft(wave_net_2, value_i, range, 1);

    for (i = 0; i < range; i++)
    {
        if (wave_net_2[i] == -0)
        {
            wave_net_2[i] = -1 * wave_net_2[i];
        }
        else if (value_i[i] == -0)
        {
            value_i[i] = -1 * value_i[i];
        }

        ps[i] = wave_net_2[i] * wave_net_2[i] + value_i[i] * value_i[i];    /* パワースペクトル  */
        as = sqrt(wave_net_2[i] * wave_net_2[i] + value_i[i] * value_i[i]); /* 振幅スペクトル  */
        // fq = (double)i / (dt * (double)range);
        fq = i;

        // printf("[%d]\tvalue_Re = %.3f \tvalue_Im = %.3f\t pw: %.3f\tfq :%d\n", i, wave_net_2[i], value_i[i], ps, fq);
    }

    /*****************************************************************************/

    double target_1 = 0.0554;
    double target_2 = 0.0575;

    double target_3 = 0.0505;
    double target_4 = 0.0514;

    double target_5 = 0.2324;
    double target_6 = 0.2315;

    // if (ps[2] > target_1 && ps[2] < target_2)
    // {
    if (wave_net_2[2] > target_3 && wave_net_2[2] < target_4)
    {
        if (value_i[2] > target_5 && value_i[2] < target_6)
        {
            printf("\t[%.2f]\tHit!!\n", Theta_2);

            fp_csv = fopen(filename_csv_3, "a");
            fp_dat = fopen(filename_dat_3, "a");

            fprintf(fp_csv, "%.2f,%.2f,%lf,%lf,%lf\n", Theta_1, Theta_2, wave_net_2[2], value_i[2], ps[2]);
            fprintf(fp_dat, "%.2f\t%.2f\t%lf\t%lf\t%lf\n", Theta_1, Theta_2, wave_net_2[2], value_i[2], ps[2]);

            fclose(fp_csv);
            fclose(fp_dat);
        }
    }
    // }

    /*****************************************************************************/

    // // Gnuplot //

    // // ディレクトリの作成
    // char directoryname_plot_1[100];
    // char directoryname_plot_2[100];

    // sprintf(directoryname_plot_1, "../result/%s/plot/wave", date);
    // mkdir(directoryname_plot_1, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IXOTH);
    // sprintf(directoryname_plot_2, "../result/%s/plot/net", date);
    // mkdir(directoryname_plot_2, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IXOTH);

    // /*****************************************************************************/

    // // filename
    // char filename_plot_1[100];
    // char filename_plot_2[100];
    // char filename_plot_3[100];

    // sprintf(filename_dat_1, "../result/%s/dat/wave/wave.dat", date);
    // sprintf(filename_dat_2, "../result/%s/dat/average/average.dat", date);
    // sprintf(filename_plot_1, "../result/%s/plot/wave/drag.png", date);
    // sprintf(filename_plot_2, "../result/%s/plot/wave/lift.png", date);
    // sprintf(filename_plot_3, "../result/%s/plot/net/net.png", date);

    // /*****************************************************************************/

    // // range x
    // double x_min = -30;
    // double x_max = 360;
    // double interval = 30;

    // // range y
    // double y_min = -1.2;
    // double y_max = 1.2;

    // // label
    // const char *xxlabel = "Angle [deg]";
    // const char *yylabel_1 = "Gradient of voltage [V/V]";
    // const char *yylabel_2 = "Net output voltage [V/V]";
    // char label_1[100] = "Gradient value";
    // char label_2[100] = "Output voltage";

    // double size;

    // /*****************************************************************************/

    // // size
    // size = 1;

    // if ((gp = popen("gnuplot", "w")) == NULL)
    // {
    //     printf("gnuplot is not here!\n");
    //     exit(0); // gnuplotが無い場合、異常ある場合は終了
    // }

    // fprintf(gp, "set terminal pngcairo enhanced font 'Times New Roman,15' \n");

    // fprintf(gp, "set output '%s'\n", filename_plot_1);
    // // fprintf(gp, "set multiplot\n");
    // fprintf(gp, "set key left top\n");
    // fprintf(gp, "set key font ',22'\n");
    // fprintf(gp, "set term pngcairo size 1280, 960 font ',27'\n");
    // // fprintf(gp, "set size ratio %.3f\n", size);

    // fprintf(gp, "set lmargin screen 0.10\n");
    // fprintf(gp, "set rmargin screen 0.90\n");
    // fprintf(gp, "set tmargin screen 0.90\n");
    // fprintf(gp, "set bmargin screen 0.15\n");

    // fprintf(gp, "set xrange [%.3f:%.3f]\n", x_min, x_max);
    // fprintf(gp, "set xlabel '%s'offset 0.0,0\n", xxlabel);
    // fprintf(gp, "set xtics %.3f\n", interval);
    // fprintf(gp, "set yrange [%.3f:%.3f]\n", y_min, y_max);
    // fprintf(gp, "set ylabel '%s'offset 1,0.0\n", yylabel_1);
    // fprintf(gp, "set title '%s'\n", label_1);

    // // fprintf(gp, "set samples 10000\n");
    // fprintf(gp, "plot '%s' using 1:2 with lines lc 'royalblue' title 'Drag (1)', '%s' using 1:3 with lines lc 'orangered' title 'Drag (2)', '%s' using 1:4 with lines lc 'magenta' title 'Drag sum'\n", filename_dat_1, filename_dat_1, filename_dat_1);
    // fflush(gp); // Clean up Data

    // // 2枚目

    // fprintf(gp, "set output '%s'\n", filename_plot_2);
    // // fprintf(gp, "set multiplot\n");
    // fprintf(gp, "set key left top\n");
    // fprintf(gp, "set key font ',22'\n");
    // fprintf(gp, "set term pngcairo size 1280, 960 font ',27'\n");
    // // fprintf(gp, "set size ratio %.3f\n", size);

    // fprintf(gp, "set lmargin screen 0.10\n");
    // fprintf(gp, "set rmargin screen 0.90\n");
    // fprintf(gp, "set tmargin screen 0.90\n");
    // fprintf(gp, "set bmargin screen 0.15\n");

    // fprintf(gp, "set xrange [%.3f:%.3f]\n", x_min, x_max);
    // fprintf(gp, "set xlabel '%s'offset 0.0,0\n", xxlabel);
    // fprintf(gp, "set xtics %.3f\n", interval);
    // fprintf(gp, "set yrange [%.3f:%.3f]\n", y_min, y_max);
    // fprintf(gp, "set ylabel '%s'offset 1,0.0\n", yylabel_1);
    // fprintf(gp, "set title '%s'\n", label_1);

    // // fprintf(gp, "set samples 10000\n");
    // fprintf(gp, "plot '%s' using 1:5 with lines lc 'royalblue' title 'Lift (1)', '%s' using 1:6 with lines lc 'orangered' title 'Lift (2)', '%s' using 1:7 with lines lc 'magenta' title 'Lift sum'\n", filename_dat_1, filename_dat_1, filename_dat_1);
    // fflush(gp); // Clean up Data

    // // 3枚目

    // fprintf(gp, "set output '%s'\n", filename_plot_3);
    // // fprintf(gp, "set multiplot\n");
    // fprintf(gp, "set key left top\n");
    // fprintf(gp, "set key font ',22'\n");
    // fprintf(gp, "set term pngcairo size 1280, 960 font ',27'\n");
    // // fprintf(gp, "set size ratio %.3f\n", size);

    // fprintf(gp, "set lmargin screen 0.10\n");
    // fprintf(gp, "set rmargin screen 0.90\n");
    // fprintf(gp, "set tmargin screen 0.90\n");
    // fprintf(gp, "set bmargin screen 0.15\n");

    // fprintf(gp, "set xrange [%.3f:%.3f]\n", x_min, x_max);
    // fprintf(gp, "set xlabel '%s'offset 0.0,0\n", xxlabel);
    // fprintf(gp, "set xtics %.3f\n", interval);
    // fprintf(gp, "set yrange [0.8:1.2]\n");
    // fprintf(gp, "set ylabel '%s'offset 1,0.0\n", yylabel_2);
    // fprintf(gp, "set ytics 0.1\n");
    // fprintf(gp, "set title '%s'\n", label_2);

    // // fprintf(gp, "set samples 10000\n");
    // fprintf(gp, "plot '%s' using 1:8 with lines lc 'green' title 'Net voltage', '%s' using 1:4 with lines lc 'grey' title 'Average'\n", filename_dat_1, filename_dat_2);
    // fflush(gp); // Clean up Data

    // fprintf(gp, "exit\n"); // Quit gnuplot

    // pclose(gp);
}

// int main()
// {
//     char dataname[50];
//     double theta_x, theta_y;

//     printf("[Parameters]\n");
//     printf("Theta x = ");
//     scanf("%lf", &theta_x);
//     printf("Theta y = ");
//     scanf("%lf", &theta_y);
//     printf("\n");

//     simulater("simulation_sensors", theta_x, theta_y);

//     return (0);
// }