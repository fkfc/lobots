lobots server / lobots client
2009 - 2014 Felipe Kermentz Ferraz Costa (felipekfcosta@gmail.com)
Licensed under the GNU General Public License


lobots_srv: Lobots Server

COMPILE AND RUN:
make
./lobots_srv



COMMAND LINE ARGUMENTS
lobots_srv [MAP NAME] [OPTIONS]

OPTIONS:
  -pop <POPULATION SIZE> : Automatically starts the simulation when number of connected robots reaches the defined size
  
  -no_clock : Run the simulation as fast as the computer can (but don't skip frames)
  
  -clock_fps : Relevant only when using -no_clock. Quantity of frames per seconds in which the simulation will run. The default value (when not using this option) is 10. When not using -no_clock, this depends on how fast your computer is.
  
  -skip_draw : Don't draw the map (makes the simulation run faster when using -no_clock). Same as pressing 'l' during execution
  
  -port <PORT> : Set a custom listening port for the server. The default value is 7123
  
  -no_display : No display (user interface + simulation display) - windowless mode
  
  