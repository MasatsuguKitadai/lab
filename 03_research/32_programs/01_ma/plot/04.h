// drag

fprintf(gp, "set terminal pngcairo enhanced font 'Times New Roman,15' \n");
fprintf(gp, "set output '%s'\n", graphname5);
// fprintf(gp, "set multiplot\n");
// fprintf(gp, "unset key\n");
fprintf(gp, "set key left top\n");
fprintf(gp, "set term pngcairo size 1280, 960 font ',24'\n");
fprintf(gp, "set key font ',20'\n");
// fprintf(gp, "set size ratio %lf\n", size);

fprintf(gp, "set lmargin screen 0.10\n");
fprintf(gp, "set rmargin screen 0.90\n");
fprintf(gp, "set tmargin screen 0.90\n");
fprintf(gp, "set bmargin screen 0.15\n");

fprintf(gp, "set xrange [%d:%d]\n", x_min, x_max);
fprintf(gp, "set xlabel '%s'offset 0.0,0\n", xxlabel);
fprintf(gp, "set yrange [%d:%d]\n", y_min1, y_max1);
fprintf(gp, "set ylabel '%s'offset 1,0.0\n", yylabel);
fprintf(gp, "set title '%s [Drag]'\n", label);

// fprintf(gp, "set samples 10000\n");
fprintf(gp, "plot 0 lw 2 lc 'dark-grey' notitle, '%s' using 1:2 with lines lc 'black' title 'Raw', '%s' using 1:3 with lines lw 2 lc 'orange' title 'Correction'\n", filename1, filename7);

fflush(gp); // Clean up Data

// lift

fprintf(gp, "set terminal pngcairo enhanced font 'Times New Roman,15' \n");
fprintf(gp, "set output '%s'\n", graphname6);
// fprintf(gp, "unset key\n");
fprintf(gp, "set key left top\n");
fprintf(gp, "set term pngcairo size 1280, 960 font ',24'\n");
fprintf(gp, "set key font ',20'\n");
// fprintf(gp, "set size ratio %lf\n", size);

fprintf(gp, "set lmargin screen 0.10\n");
fprintf(gp, "set rmargin screen 0.90\n");
fprintf(gp, "set tmargin screen 0.90\n");
fprintf(gp, "set bmargin screen 0.15\n");

fprintf(gp, "set xrange [%d:%d]\n", x_min, x_max);
fprintf(gp, "set xlabel '%s'offset 0.0,0\n", xxlabel);
fprintf(gp, "set yrange [%d:%d]\n", y_min2, y_max2);
fprintf(gp, "set ylabel '%s'offset 1,0.0\n", yylabel);
fprintf(gp, "set title '%s [lift]'\n", label);

// fprintf(gp, "set samples 10000\n");
fprintf(gp, "plot 0 lw 2 lc 'dark-grey' notitle, '%s' using 1:3 with lines lc 'black' title 'Raw', '%s' using 1:5 with lines lw 2 lc 'orange' title 'Correction'\n", filename1, filename7);

fflush(gp); // Clean up Data