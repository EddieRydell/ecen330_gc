idf_component_register(SRCS "tone.c" "tone_test.c"
                       INCLUDE_DIRS "."
                       REQUIRES sound lcd)
# target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
