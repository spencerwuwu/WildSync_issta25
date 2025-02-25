
# Issues
- `gdk-pixbuf` doesn't work, adding `gdwarf-4` not helping
  - Officially less priority [link](https://github.com/ossf/fuzz-introspector/issues/30)
```
++ pkg-config --static --libs gmodule-2.0 glib-2.0 gio-2.0 gobject-2.0 gdk-pixbuf-2.0
+ BUILD_LDFLAGS='-Wl,-static --verbose -L/work/prefix/lib -Wl,--export-dynamic -lgdk_pixbuf-2.0 -lm -lpng16 -ljpeg -lgio-2.0 -pthread -lresolv -lgobject-2.0 -pthread -lffi -lgmodule-2.0 -pthread -lglib-2.0 -pthread -lz -gdwarf-4'
++ find /src/fuzz/ -name '*_fuzzer.c'
+ fuzzers='/src/fuzz/animation_fuzzer.c
/src/fuzz/pixbuf_cons_fuzzer.c
/src/fuzz/pixbuf_file_fuzzer.c
/src/fuzz/pixbuf_scale_fuzzer.c
/src/fuzz/stream_fuzzer.c
/src/fuzz/gdk-pixbuf__gdk_pixbuf_animation_new_from_stream__fuzzer.c'
+ for f in $fuzzers
++ basename /src/fuzz/animation_fuzzer.c .c
+ fuzzer_name=animation_fuzzer
+ clang -O1 -fno-omit-frame-pointer -gline-tables-only -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION -O0 -flto -fno-inline-functions -fuse-ld=gold -Wno-unused-command-line-argument -fsanitize=fuzzer-no-link -g -gdwarf-4 -O1 -fno-omit-frame-pointer -gline-tables-only -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION -O0 -flto -fno-inline-functions -fuse-ld=gold -Wno-unused-command-line-argument -fsanitize=fuzzer-no-link -g -gdwarf-4 -pthread -I/work/prefix/include -I/work/prefix/include/gdk-pixbuf-2.0 -I/work/prefix/include -I/work/prefix/include/glib-2.0 -I/work/prefix/lib/glib-2.0/include -c /src/fuzz/animation_fuzzer.c -o /work/animation_fuzzer.o
+ clang++ -O1 -fno-omit-frame-pointer -gline-tables-only -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION -O0 -flto -fno-inline-functions -fuse-ld=gold -Wno-unused-command-line-argument -fsanitize=fuzzer-no-link -stdlib=libc++ -g -gdwarf-4 /work/animation_fuzzer.o -o /out/animation_fuzzer -Wl,-Bdynamic -ldl -lm -pthread -lrt -lpthread -lc -gdwarf-4 -Wl,-static --verbose -L/work/prefix/lib -Wl,--export-dynamic -lgdk_pixbuf-2.0 -lm -lpng16 -ljpeg -lgio-2.0 -pthread -lresolv -lgobject-2.0 -pthread -lffi -lgmodule-2.0 -pthread -lglib-2.0 -pthread -lz -gdwarf-4 -fsanitize=fuzzer -Wl,-Bdynamic
clang version 15.0.0 (https://github.com/llvm/llvm-project.git bf7f8d6fa6f460bf0a16ffec319cd71592216bf4)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Candidate multilib: .;@m64
Candidate multilib: 32;@m32
Candidate multilib: x32;@mx32
Selected multilib: .;@m64
 "/usr/bin/ld.gold" -z relro --hash-style=gnu --eh-frame-hdr -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o /out/animation_fuzzer /lib/x86_64-linux-gnu/crt1.o /lib/x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/9/crtbegin.o -L/work/prefix/lib -L/usr/lib/gcc/x86_64-linux-gnu/9 -L/usr/lib/gcc/x86_64-linux-gnu/9/../../../../lib64 -L/lib/x86_64-linux-gnu -L/lib/../lib64 -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib64 -L/usr/local/bin/../lib -L/lib -L/usr/lib -plugin /usr/local/bin/../lib/LLVMgold.so -plugin-opt=mcpu=x86-64 -plugin-opt=O0 --whole-archive /usr/local/lib/clang/15.0.0/lib/linux/libclang_rt.fuzzer-x86_64.a --no-whole-archive --whole-archive /usr/local/lib/clang/15.0.0/lib/linux/libclang_rt.fuzzer_interceptors-x86_64.a --no-whole-archive -lc++ --whole-archive /usr/local/lib/clang/15.0.0/lib/linux/libclang_rt.ubsan_standalone-x86_64.a --no-whole-archive --dynamic-list=/usr/local/lib/clang/15.0.0/lib/linux/libclang_rt.ubsan_standalone-x86_64.a.syms --whole-archive /usr/local/lib/clang/15.0.0/lib/linux/libclang_rt.ubsan_standalone_cxx-x86_64.a --no-whole-archive --dynamic-list=/usr/local/lib/clang/15.0.0/lib/linux/libclang_rt.ubsan_standalone_cxx-x86_64.a.syms /work/animation_fuzzer.o -Bdynamic -ldl -lm -lrt -lpthread -lc -static --export-dynamic -lgdk_pixbuf-2.0 -lm -lpng16 -ljpeg -lgio-2.0 -lresolv -lgobject-2.0 -lffi -lgmodule-2.0 -lglib-2.0 -lz -Bdynamic -lc++ -lm --no-as-needed -lpthread -lrt -lm -ldl -lgcc_s -lgcc -lpthread -lc -lgcc_s -lgcc /usr/lib/gcc/x86_64-linux-gnu/9/crtend.o /lib/x86_64-linux-gnu/crtn.o
/usr/bin/ld.gold: error: cannot find -lgcc_s
/usr/bin/ld.gold: error: cannot find -lgcc_s
[Log level 1] : 19:50:47 : Fuzz introspector is running
[Log level 1] : 19:50:47 : Running introspector on ld-temp.o
[Log level 1] : 19:50:47 : This is a fuzzer, performing analysis
[Log level 2] : 19:50:47 : Using default configuration
[Log level 1] : 19:50:48 : Logging next yaml tile to /src/fuzzerLogFile-0-25OkRoXSNQ.data.yaml
[Log level 1] : 19:50:48 : Wrapping all functions
[Log level 1] : 19:50:53 : Ended wrapping all functions
[Log level 1] : 19:50:55 : Finished introspector module
/usr/bin/ld.gold: internal error in read_header_prolog, at ../../gold/dwarf_reader.cc:1678
clang-15: error: linker command failed with exit code 1 (use -v to see invocation)
root@ce3c1c485f7a:/src/gdk-pixbuf#
```
