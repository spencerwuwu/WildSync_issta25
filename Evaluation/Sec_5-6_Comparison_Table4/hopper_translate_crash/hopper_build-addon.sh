##########################

mkdir -p $HOPPER_RESULTS_DIR

echo "================="
echo "Start hopper compile at $HOPPER_RESULTS_DIR "
time HOPPER_INCLUDE_SEARCH_PATH=$LIB_PATH \
    hopper compile \
    --header $HEADER \
    --library $SHARED_LIB \
    --output $HOPPER_RESULTS_DIR


