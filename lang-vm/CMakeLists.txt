set(sources
    interpreter.cpp
    interpreter.hpp
    main.cpp
    program.hpp
)

add_executable(lang_vm ${sources})
target_include_directories(lang_vm
    PRIVATE
    ${PROJECT_SOURCE_DIR}/shared
)
