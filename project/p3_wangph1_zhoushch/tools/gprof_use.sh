# add -g -pg to MAKEFILES

cd ..
make all
bash tools/run.sh
gprof ./lbm gmon.out > tools/gprof_report