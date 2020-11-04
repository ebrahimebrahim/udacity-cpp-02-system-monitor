# CppND-System-Monitor

---

## Forked

This is a forked assignment, to be Ebrahim's solution to project 02 of udacity's c++ course.

Processes are sorted by cpu usage,
and they resort themselves every few updates.
New processes will always show up at the top to start with,
and then they will get moved to their proper position at the next sorting.

CPU usage readings for processes seem to agree with results from htop.
But something is not right: cpu usage can be over 100% sometimes.
This also happens in htop.
I suspect it's becuase processes can spend time on multiple cpus, but we are only dividing by "uptime."

The quantity advertized as "RAM[MB]" of a process is not really memory usage,
but rather some mysterious quantity called "virtual memory size."
This is documented in "man proc," but I don't really understand what it means.
The same number shows up as "M_SIZE" (column labled "VIRT") in htop; it doesn't seem to be very related to "MEM%".




---

Starter code for System Monitor Project in the Object Oriented Programming Course of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

Follow along with the classroom lesson to complete the project!

![System Monitor](images/monitor.png)

## Udacity Linux Workspace
[Udacity](https://www.udacity.com/) provides a browser-based Linux [Workspace](https://engineering.udacity.com/creating-a-gpu-enhanced-virtual-desktop-for-udacity-497bdd91a505) for students. 

You are welcome to develop this project on your local machine, and you are not required to use the Udacity Workspace. However, the Workspace provides a convenient and consistent Linux development environment we encourage you to try.

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

Within the Udacity Workspace, `.student_bashrc` automatically installs ncurses every time you launch the Workspace.

If you are not using the Workspace, install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository: `git clone https://github.com/udacity/CppND-System-Monitor-Project-Updated.git`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`
![Starting System Monitor](images/starting_monitor.png)

4. Follow along with the lesson.

5. Implement the `System`, `Process`, and `Processor` classes, as well as functions within the `LinuxParser` namespace.

6. Submit!
