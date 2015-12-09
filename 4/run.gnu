reset
set terminal png
set output "mcat.png"
set title "Performance"
set xlabel "Taille du tampon (en octets)"
set logscale x
set ylabel "Temps (en secondes)"
set logscale y
set style data linespoints
plot "mcat-tm.dat" using 1:2 title "Temps global", \
	 "mcat-tm.dat" using 1:3 title "Temps utilisateur", \
	 "mcat-tm.dat" using 1:4 title "Temps système"
pause -1  "Hit return to continue"