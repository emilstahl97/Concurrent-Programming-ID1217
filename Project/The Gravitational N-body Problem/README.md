# Programming Project: The Gravitational N-body Problem

## Objectives
The main objectives of this assignment can be summarized as follows:

To understand how to develop a parallel program and optimize it for performance;
To understand how use shared memory or message programming models in developing a parallel program:
how to use threads in a parallel program and how to use shared memory for thread interaction;
how to create a parallel program for a distributed memory platform and how to use message passing in a parallel program
To learn how to use parallel programming environments such as MPI, Pthreads, OpenMP;
To implement a parallel application for a shared-memory or/and a distributed memory multiprocessor.

## Task

In this project you are to develop and to evaluate efficient parallel programs for a particle computation to solve the gravitational N-body problem, conduct evaluation experiments to analyze the performance of your programs or/and the amount of communication, and write a report describing your results and what you have learned. The gravitational N-body problem and approaches to its solution are described in Section 11.2 of the G.Andrews textbook "Foundations of Multithreaded, Parallel, and Distributed Programming".  

### Programs to Develop and to Evaluate

You are to develop and to evaluate the following four programs

A sequential N2 program, as in Figure 11.9Förhandsgranska dokumentet (page 556)
A parallel N2program, as in Figure 11.11Förhandsgranska dokumentet (pp. 560-561)
A sequential N logN program using the Barnes-Hut method.
A parallel N logN program using the Barnes-Hut method.
You may write the programs in either of the following five programming environments (it's your choice):

C/C++ with MPI library using message passing programming model
C/C++ with Pthreads library using shared-memory programming model
C/C++ with the OpenMP API (with or without tasks) using shared-memory programming model
Java with Java RMI or Java socket classes using message passing programming model
Java with Java threads, monitors or/and concurrent utilities using shared-memory programming model
Develop your programs on any machines at KTH or at home, but run performance experiments on severl computers, if avialble, or a mulicore server (Länkar till en externa sida.). As an alternative evaluation task you may estimate an amount of communication (the number of messages) instead of performance (the execution time). In this case, you can run a parallel program on ONE computer (e.g. use the -all-local option for mpirun).

The Barnes-Hut method is described in Section 11.2.4 (page 569) of the G.Andrews textbook "Foundations of Multithreaded, Parallel, and Distributed Programming", but an actual code is not given. It is up to you to figure out the details for the sequential program and to figure out how to parallelize that program. There is lots of information on the Web that might be helpful. For example, a search for "Barnes-Hut" using Google returns many links. 

It is up to you to figure out how to initialize data in such a way that you can tell that your programs are correct. You also want to be sure that bodies do not get too close to each other. You need to implement an efficient barrier. Finally, you should make the sequential parts of your programs as efficient as possible; see the discussion on pages 539-540 of the G.Andrews textbook "Foundations of Multithreaded, Parallel, and Distributed Programming" for ideas on things you can do to the source code so that the C compiler generates faster assembly code.

### Input Parameters

Your programs should have three command-line arguments in the following order:

gnumBodies	the number of bodies
numSteps	the number of time steps in the simulation
far	the distance used to decide when to approximate (Barnes-Hut programs only)
numWorkers	the number of workers (parallel programs only)
Note: if you use MPI, numWorkers can be specified with -np option for mpirun.
Feel free to use other arguments in your programs, such as for the value of G and the initial mass of the bodies. However, put them after the above arguments and assign default values in your programs so that we can test programs using just the above arguments. You may also read data from an input file if you wish, otherwise you may simply use "canned" data that you can generate algorithmically..

### Output
The output from your programs should be the command-line arguments and  the execution time for the computational part or/and the number of messages sent. You may also want to write the final data values to a file—at least while your are debugging your programs—but write only the above results (the execution time or/and the number of messages) to standard out. If you will you may display moving bodies in a graphical user interface.

To calculate the execution time, read the clock after you have initialized all variables and just before you create the processes (in the parallel programs). Read the clock again as soon as you have completed the time steps and the worker processes have terminated (in the parallel programs).

### Evaluation Experiments

Your second task is to evaluate your programs with a series of experiments. First experiment with your first program (sequential N2) to determine how many time steps to use so that the computation time of the program is about 15 seconds for 120 bodies. In short, figure out what value to use for numSteps so that your program computes for 15 seconds for 120 bodies.

Next run a series of evaluation experiments to calculate the computation time or/and communication overhead (the number of messages) for all of the following
combinations of programs and values for the command-line arguments:

program 1 for 120, 180, and 240 bodies.
program 2 for 120, 180, and 240 bodies and 1-4 workers.
program 3 for 120, 180, and 240 bodies.
program 4 for 120, 180 and 240 bodies and 1-4 workers.
There are a total of 30 different runs—3 for program 1, 12 for program 2, 3 for program 3, and 12 for program 4. In order to get more precise timing estimates, we recommend you to run a program at least 5 times and take the median (rather than the mean) of the timing values.

In all programs, set numSteps to the value you calculated in the first step. In the Barnes-Hut programs, set far to a value so that most bodies (80 to 90%) are considered to be far enough away to use approximations. You will, of course, have to experiment with program 3 to determine a reasonable value to use.  To estimate the execution time, you can use different functions depending on which environment you use to develop the program:

If you develop your programs using Pthreads, use the times function (read a man page: man -s 2 times, see times (Länkar till en externa sida.)).
If you develop your programs using OpenMP, use the omp_get_wtime function (see omp_get_wtime (Länkar till en externa sida.))
If you develop your programs using MPI, use the MPI_Wtime function (see MPI_Wtime (Länkar till en externa sida.))
If you develop your program in Java, use the System.nanoTime() method (see System.nanoTime() (Länkar till en externa sida.))

### Reports

In a report you should explain what you have done and what you have learned. Your report should be a few pages of text plus tables and figures. It should have four or five sections, as follows:

Introduction. Briefly describe the problem and what your report will show.
Programs. Describe your programs, stating what each does and how. Explain the program-level optimizations you have implemented.
Evaluation. Present the results from the evaluation experiments. Use tables to present the raw data and graphs to show speedups and comparisons. Also explain your results. Do not just present the output data! What do the results show? Why?
Conclusion. Briefly summarize what your report has shown, and describe what you have learned from this project.
(Optional, not required) You can also submit a makefile and a README file with an instructions on how to build and run your programs.