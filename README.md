# Abbey

This repository contains a new implementation of the classic video game "La abadía del crimen" or "The Abbey of Crime" for pc and the RG350 handheld. 

Based on the great work of Manuel Abadía for the VIGASOCO project, this new version reduces the computational power necessary for the emulation of the inner behavior of the Amstrad CPC on which the original video game was designed, and replaced it with modern programming patterns and data structures algorithms. While the original VIGASOCO project needed four threads to emulate video, inputs, sound, and the main game, this new version reduces the need to use threads, making it easy to port to low specs machines like the RG350, the Nintendo 3ds, or Android.

# Status
* At this point the game is stable, can run smoothly on the RG350 and Linux, and includes new features in the menu system, support of Forcefeedback on the RG350 and pc (using a PS4 controller), but more work needs to be done testing the game and fixing new bugs.

* There is a need to redesign some of the classes inherited from the VIGASOCO project, as while the latter was designed to create a framework for the design of games, most of these features are beyond the scope of this project that only focuses in only one game.

# OPK file
The opk file is located inside of the build_rg350 directory.
