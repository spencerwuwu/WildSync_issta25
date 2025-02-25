
new_fuzzers="libxmlb__xb_builder_add_fixup__0 libxmlb__xb_builder_compile__3 libxmlb__xb_builder_import_node__1 libxmlb__xb_builder_node_export__4 libxmlb__xb_builder_source_load_bytes__2 libxmlb__xb_builder_source_load_xml__0 libxmlb__xb_builder_source_load_xml__1 libxmlb__xb_builder_source_load_xml__2 libxmlb__xb_node_get_attr__0 libxmlb__xb_node_get_attr_as_uint__0 libxmlb__xb_node_get_child__0 libxmlb__xb_node_get_child__1 libxmlb__xb_node_get_children__0 libxmlb__xb_node_get_data__0 libxmlb__xb_node_get_element__2 libxmlb__xb_node_get_next__0 libxmlb__xb_node_get_parent__1 libxmlb__xb_node_get_silo__0 libxmlb__xb_node_get_tail__0 libxmlb__xb_node_query__0 libxmlb__xb_node_query_attr__0 libxmlb__xb_node_query_attr_as_uint__0 libxmlb__xb_node_query_first__0 libxmlb__xb_node_query_first_full__0 libxmlb__xb_node_query_full__2 libxmlb__xb_node_query_text__0 libxmlb__xb_node_query_text_as_uint__0 libxmlb__xb_node_query_with_context__0 libxmlb__xb_query_context_get_bindings__0 libxmlb__xb_query_new__0 libxmlb__xb_silo_get_root__0 libxmlb__xb_silo_invalidate__0 libxmlb__xb_silo_lookup_query__0 libxmlb__xb_silo_query__0 libxmlb__xb_silo_query_build_index__0 libxmlb__xb_silo_query_first__0 libxmlb__xb_silo_query_first_with_context__0"

for new_f in $new_fuzzers; do
    cp "$SRC/new_fuzzers/${new_f}.cpp" "$SRC/new_fuzzers/${new_f}.c"
    f="$SRC/new_fuzzers/${new_f}.c"
    fuzzer_name=$(basename $f .c)

    $CC $BUILD_CFLAGS $LIB_FUZZING_ENGINE \
            -I src \
            -I _builddir \
            ${f} \
            -c -o ${fuzzer_name}.o 

    if [ $? -ne 0 ]; then
        echo "Failed to compile $new_f"
        continue
    fi

    $CXX $CXXFLAGS \
            ${fuzzer_name}.o -o $OUT/${fuzzer_name} \
            -L/work/prefix/lib/x86_64-linux-gnu -lxmlb\
            $PREDEPS_LDFLAGS \
            $BUILD_LDFLAGS \
            -I src \
            -I _builddir \
            -gdwarf-4 \
            $LIB_FUZZING_ENGINE \
            -Wl,-Bdynamic

done

