set title 'Fluid Velocity'
set xlabel 'cell # along x-dimension'
set ylabel 'cell # along y-dimension'
set size ratio -1
set autoscale fix


set terminal png
set output 'final_state.png'

set cbrange [-0.05:0.2]

r(x) = (x == 0 ? 1 : sqrt(x))
g(x) = (x == 0 ? 1 : x**3)
b(x) = (x == 0 ? 1 : sin(2*pi*x))
set palette functions r(gray),g(gray),b(gray)

plot './results/final_state.dat' using 1:2:3 with image