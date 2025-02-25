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

pushd "$BASE/../Sec_5-5_Effectiveness_Table2/targets" &> /dev/null
shopt -s nullglob
d_targets=(*)
d_targets_str="${d_targets[@]//-/_}"
declare -a DEFAULT_TARGETS="($d_targets_str)"
shopt -u nullglob

popd &> /dev/null
