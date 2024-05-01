idf_component_register(SRCS "pin.c" "pin_test.c"
                       INCLUDE_DIRS "."
                       REQUIRES driver)
# target_compile_options(${COMPONENT_LIB} PRIVATE -O0)
