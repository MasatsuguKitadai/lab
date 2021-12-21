/******************************************************************************
PROGRAM NAME :
AUTHER  : masatsugu Kitadai
DATE    :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>

FILE *fp, *fp_csv, *fp_dat;

// 円周率の定義
#define pi 4 * atan(1.0)

char filename_read[100];
char filename_dat[100];
char filename_csv[100];

/*********************************   MAIN   *********************************/

int phase_angle_lift_theory(char date[])
{
    /*****************************************************************************/
    // ディレクトリの作成
    char directoryname_dat[100];
    char directoryname_csv[100];

    sprintf(directoryname_dat, "../result/%s/dat/28-2_phase-angle_lift-theory", date);
    sprintf(directoryname_csv, "../result/%s/csv/28-2_phase-angle_lift-theory", date);

    mkdir(directoryname_dat, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IXOTH);
    mkdir(directoryname_csv, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IXOTH);

    // ファイルの指定
    char filename_read[100];
    char filename_csv[100];
    char filename_dat[100];

    sprintf(filename_read, "../result/%s/csv/27-1_fft-lift-theory/27-1.csv", date);
    sprintf(filename_csv, "../result/%s/csv/28-2_phase-angle_lift-theory/28-2.csv", date);
    sprintf(filename_dat, "../result/%s/dat/28-2_phase-angle_lift-theory/28-2.dat", date);

    /*****************************************************************************/

    int i;
    int buf;
    double ch0, ch1, ch2;
    double value[100][4];

    fp = fopen(filename_read, "r");

    if (filename_read == NULL)
    {
        printf("Error! I can't open the file.\n");
        exit(0);
    }

    i = 0;

    while ((fscanf(fp, "%d, %lf, %lf, %lf", &buf, &ch0, &ch1, &ch2)) != EOF)
    {
        // printf("[%d]\t%lf\t%lf\t%lf\n", buf, ch0, ch1, ch2);
        value[i][0] = buf; // 波数
        value[i][1] = ch0; // パワースペクトル
        value[i][2] = ch1; // 実部
        value[i][3] = ch2; // 虚部
        i = i + 1;
    }

    fclose(fp);

    // 計算

    double gradient;
    double radian;
    double degree;

    gradient = value[1][3] / value[1][2];
    radian = atan(gradient);
    degree = 180 * radian / pi;

    printf("[lift-Theory]\n");
    printf("Im/Re =\t%lf\n", gradient);
    printf("angle =\t%lf\t[deg]\n\n", degree);

    fp_csv = fopen(filename_csv, "w");
    fp_dat = fopen(filename_dat, "w");

    fprintf(fp_csv, "%lf\t%lf\n", gradient, degree);
    fprintf(fp_dat, "%lf\t%lf\n", gradient, degree);

    fclose(fp_csv);
    fclose(fp_dat);

    printf("28-2\t\tsuccess\n");
}

// int main()
// {
//     radian_lift("test-fft");
//     return (0);
// }