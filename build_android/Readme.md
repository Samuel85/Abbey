Notes for Android compilation

Compiling SDL projects for Android is not as clean as compiling for the pc. The sources can't be located in separete folders, so in order to compile the program, run the script "configure" to create a local copy of all the sources and assets of the game, and unzip the sources for SDL2, SDL2_image, SDL2_mixer, and SDL2_ttf inside of the directory "app/jni".Finnally run "./gradlew build" to generate the apk that will be located in "app/build/outputs/apk/debug". The compilation time is about 10 minutes even in a modern laptop, so a good coffe cup or beer will be necessary.


