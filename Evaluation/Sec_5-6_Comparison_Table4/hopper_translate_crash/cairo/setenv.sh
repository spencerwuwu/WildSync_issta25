
SO_PATHS="/home/cairo/build/subprojects/pixman/pixman/:/home/cairo/build/src/:/home/cairo/build/util/cairo-gobject/:/home/cairo/build/util/cairo-script/:/home/cairo/build/util/cairo-trace/:/home/cairo/build/util/cairo-fdr/"

if [ -z "$LD_LIBRARY_PATH" ]; then
    export LD_LIBRARY_PATH="$SO_PATHS"
else
    export LD_LIBRARY_PATH="$SO_PATHS:$LD_LIBRARY_PATH"
fi
