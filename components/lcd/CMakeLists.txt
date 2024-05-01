idf_component_register(SRCS "lcd.c" "fontx.c" "lcd_test.c"
                       INCLUDE_DIRS "."
                       REQUIRES driver)
target_compile_options(${COMPONENT_LIB} PRIVATE -O3)
