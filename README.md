# rmoClient
Client application for a radar station with an all-round visibility indicator. Connects to the server via TCP, sends messages about the keys pressed to it and receives the current antenna position, rotation speed and radiation state, then displays these parameters in the window.
The file with the settings.ini settings must be copied to the folder with the compiled file.

Implemented in C++ version 14 and QT version 5.12 using the CMake builder for Linux platforms.
