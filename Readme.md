# CPP_Threading Practical problem1
COVID19 Vaccination Problem using POSIX C++ Threading with MUTEX (Synchronized the threading)

Simulation of the behavior of people getting vaccinated at a small vaccination center.
Each of the people getting vaccinated will be simulated by a separate pthread forked by the main program.  When they arrive at the center, they will wait until one of the staff members is ready to take them, they will get vaccinated then wait for a few minutes to check for immediate negative reactions.
