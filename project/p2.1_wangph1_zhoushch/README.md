# Project 2.1: ALU and Regfile

## testing locally
./test.sh

do not use windows which will result some '\r' problem

use dos2unix to convert this

This repo contains the framework code for your Project 2.1.

In this project, you will create an ALU that supports all the operations needed by the instructions in our ISA, and a RegFile which should be able to write to or read from these registers specified in a given RISC-V instruction. 

We hope this project will help you know how a CPU is built, and prepare you for what's to come later in this course.


Check if you already have the following files after pulling.

```
.
├── README.md
├── autolab.txt
├── alu.circ
├── alu_harness.circ
├── regfile.circ
├── regfile_harness.circ
├── tests
│   ├── alu_tests
│   │   └── ...
│   ├── regfile_tests
│   │   └── ...
│   ├── reference_output
│   │   └── ...
│   ├── binary_to_hex_alu.py
│   ├── binary_to_hex_regfile.py
│   └── test.py
├── logisim-evolution.jar
└── test.sh
```

## Warning

Changes made out of `alu.circ` and `regfile.circ` will NOT be considered by autograder.
