lobots 0.8
-----------------------------------------------------------
- srv: It's now possible to load widgets from a XML file. Widgets can now be referred to by their names. New widget: Scrollbar (horizontal and vertical)
- srv: widgets: custom styles by code and XML



lobots 0.7
-----------------------------------------------------------
- srv: Broke all widgets' source code into separate files
- srv: Bug fix: TWidget Constructor: trying to add children before initializing nChildren count variable
- srv: Wigets can now be loaded from a XML file, read at run or added during compile time


lobots 0.6
-----------------------------------------------------------
- srv: New command line option: no_display: removes all user interface and simulation display, making it possible to run the simulation much faster when using -no_clock optio
- srv + clients: New TRobot attribute: elapsed_time: Provides the simulation time in seconds, which can be different from the real time, especially when -no_clock server option is enabled.
- Protocol: Reverse endianness when necessary, compatible with PowerPC even in a mixed Intel+PowerPC environment.


lobots 0.5
-----------------------------------------------------------
- srv: Class TPecaMultipla: Provides the ability to hold more than one brick at a time (UI function still missing)


lobots 0.4
-----------------------------------------------------------
- Server module no longer requires the GLUI library, for improved portability
- Added buttons on the user interface - Load/Save/New map, and brick movements
- Command line arguments: it is now possible to set options and start the simulation automatically from the command line without user interaction
- Added support for PowerPC machines
- New client command - Teleport robot to a location: Just write the coordinates on the Robot object
- Bug fixes


lobots 0.1
-----------------------------------------------------------
- First release

