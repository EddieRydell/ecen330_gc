idf_component_register(
        SRCS "main.c"
        "."
        PRIV_REQUIRES driver
        )
# PRIV_REQUIRES "..."
# target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
