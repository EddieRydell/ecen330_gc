idf_component_register(
        SRCS "main.c" "sd_card_file_system.c" "fseq.c" "interface.c"
        "."
        PRIV_REQUIRES fatfs driver pin esp_timer my_lcd
        )
# PRIV_REQUIRES "..."
# target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
