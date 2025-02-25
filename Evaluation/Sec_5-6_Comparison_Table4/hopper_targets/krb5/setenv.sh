
SO_PATHS="${BASE}/src/lib"

if [ -z "$LD_LIBRARY_PATH" ]; then
    export LD_LIBRARY_PATH="$SO_PATHS"
else
    export LD_LIBRARY_PATH="$SO_PATHS:$LD_LIBRARY_PATH"
fi
