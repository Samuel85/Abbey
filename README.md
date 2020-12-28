# Abbey

This repository contains a new implementation of the classic video game "La abadía del crimen" or "The Abbey of Crime" for pc and the RG350 handheld. 

Based on the great work of Manuel Abadía for the VIGASOCO project, this new version reduces the computational power necessary for the emulation of the inner behavior of the Amstrad CPC on which the original video game was designed, and replace it with modern programming patterns and data structures. While the original VIGASOCO project needed four threads to emulate video, inputs, sound, and the main game, this new version reduces the need to use threads, making it easy to port to low specs machines like the RG350, the Nintendo 3ds, or Android.

# Updates
* New android port of the game. 

# Status
* At this point the game is stable, can run smoothly on the RG350 and Linux, and includes new features in the menu system, support of Forcefeedback for the RG350 and pc (using a PS4 controller), but more work needs to be done testing the game and fixing new bugs.

* There is a need to redesign some of the classes inherited from the VIGASOCO project, as while the latter was designed to create a framework for the design of games, most of these features are beyond the scope of this project and makes the maintenance of the code more difficult.


# Screenshots
# <a href="https://ibb.co/2KYmxMj"><img src="https://i.ibb.co/nk8Z91D/Abbey-Android.png" alt="Abbey-Android" border="0" /></a>
# <a href="https://ibb.co/8PGLLFJ"><img src="https://i.ibb.co/sscddLh/20201212-140648.jpg" alt="20201212-140648" border="0"></a>

# Note for Android users
* It's necessary to use a PS4 controller.
* The android package (apk) of the game is located inside of app/build/outputs/apk/debug
* If the program don't save yor progress, it is necessary to manually assign storage permisions on the game. Holding in the game icon->App info->Permissions.
* To compile the game for android, see the Readme.md file located inside of the build_directory folder.

# RG350 users
The opk file is located inside of the build_rg350 directory.

