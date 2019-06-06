# Game Of Life


## Installation

Install dependencies like a guy did in the video below.
* Install Visual Studio 2019
* Install MPI -> https://www.youtube.com/watch?v=yYlJc9REY60

## How to run

* After installing MPI make sure you RESTART Visual Studio, so it properly applies environment variables change
* Build the whole solution. (make sure you build for Debug x86)
* `debug` directory will appear from which you can run the following commands:
  * `.\SerialLauncher.exe 728 20 verticalStripes true true`
  * `mpiexec.exe -n 5 .\ParallelColumnsLauncher.exe 728 20 verticalStripes true true`
  
The correct parameters you can get from `Game of Life - documentation.pdf` or simply running exe file without any parameters. A detailed list should appear.

## Hints

* Disable logging using argument parameters. Logging takes time, so if you want to get the best results you better disable them.
* Disable image generation using argument parameters. Image generation  takes time, so if you want to get the best result you better disable it.

## Helper links

Unit tests setup -> https://www.youtube.com/watch?v=KQxf7fby6yQ

Setup MPI in Visual Studio -> https://blogs.technet.microsoft.com/windowshpc/2015/02/02/how-to-compile-and-run-a-simple-ms-mpi-program/

MPI sending structures -> https://www.sharcnet.ca/help/images/f/fa/MPI_Datatypes_with_Opaque_Types_seminar_2014.pdf
