set size ratio -1
set autoscale fix

set cbrange [-0.05:0.2]

set term gif animate
set output 'visual.gif'
unset tics

r(x) = (x == 0 ? 1 : sqrt(x))
g(x) = (x == 0 ? 1 : x**3)
b(x) = (x == 0 ? 1 : sin(2*pi*x))
set palette functions r(gray),g(gray),b(gray)

do for [i=0:59]{
    plot './results/visual/state_'.i.'.dat' using 1:2:3 with image
}

