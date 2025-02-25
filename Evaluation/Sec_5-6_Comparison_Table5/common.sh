export TARBALL_BASENAME="ball"

echo_time() {
    date "+[%F %R] $*"
}
export -f echo_time

get_var_or_default() {
    ##
    # Pre-requirements:
    # - $1..N: placeholders
    ##
    function join_by { local IFS="$1"; shift; echo "$*"; }
    pattern=$(join_by _ "${@}")

    name="$(eval echo ${pattern})"
    name="${name}[@]"
    value="${!name}"
    if [ -z "$value" ] || [ ${#value[@]} -eq 0 ]; then
        set -- "DEFAULT" "${@:2}"
        pattern=$(join_by _ "${@}")
        name="$(eval echo ${pattern})"
        name="${name}[@]"
        value="${!name}"
        if [ -z "$value" ] || [ ${#value[@]} -eq 0 ]; then
            set -- "${@:2}"
            pattern=$(join_by _ "${@}")
            name="$(eval echo ${pattern})"
            name="${name}[@]"
            value="${!name}"
        fi
    fi
    echo "${value[@]}"
}
export -f get_var_or_default

get_programs_from_file() {
    file="$1"
    if [ ! -f "$file" ]; then
        echo ""
    else
        programs=""
        while read -r line; do
            [[ "$line" =~ ^#.*$ ]] && continue
            programs="$programs $line"
        done < "$file"
        echo "$programs"
    fi
}
export -f get_programs_from_file

pushd "$BASE/targets" &> /dev/null
shopt -s nullglob
d_targets=(*)
d_targets_str="${d_targets[@]//-/_}"
declare -a DEFAULT_TARGETS="($d_targets_str)"
shopt -u nullglob

for ITARGET in "${DEFAULT_TARGETS[@]}"; do
    export TARGET="${ITARGET//_/-}"
    orig_programs=$(get_programs_from_file "$BASE/targets/$TARGET/orig_fuzzers.txt")
    new_programs=$(get_programs_from_file "$BASE/targets/$TARGET/new_fuzzers.txt")
    if [ $ONLY_CATEGORY -eq 0 ]; then
        PROGRAMS_str="$orig_programs $new_programs"
    elif [ $ONLY_CATEGORY -eq 1 ]; then
        PROGRAMS_str="$new_programs"
    elif [ $ONLY_CATEGORY -eq 2 ]; then
        PROGRAMS_str="$orig_programs"
    fi
    declare -a DEFAULT_${ITARGET}_PROGRAMS="($PROGRAMS_str)"
done

popd &> /dev/null
