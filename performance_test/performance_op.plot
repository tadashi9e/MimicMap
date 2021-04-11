set title "operator[]"
set logscale xy 10
plot "performance_op.dat" using 1:2 w l title 'MimicMap', \
     "performance_op.dat" using 1:3 w l title 'MimicMap (reserved)', \
     "performance_op.dat" using 1:4 w l title 'std::unordered\_map', \
     "performance_op.dat" using 1:5 w l title 'std::unordered\_map (reserved)', \
     "performance_op.dat" using 1:6 w l title 'boost::container::flat\_map', \
     "performance_op.dat" using 1:7 w l title 'std::map'
set terminal png
set out "performance_op.png"
replot
