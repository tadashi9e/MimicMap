set title "find"
set logscale xy 10
plot "performance_find.dat" using 1:2 w l title 'MimicMap', \
     "performance_find.dat" using 1:3 w l title 'std::unordered\_map', \
     "performance_find.dat" using 1:4 w l title 'boost::container::flat\_map', \
     "performance_find.dat" using 1:5 w l title 'std::map'
set terminal png
set out "performance_find.png"
replot
