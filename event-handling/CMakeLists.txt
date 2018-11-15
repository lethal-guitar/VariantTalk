set(sources
    event.hpp
    handler.cpp
    handler.hpp
    main.cpp
)

add_executable(event_handling ${sources})
target_include_directories(event_handling
    PRIVATE
    ${PROJECT_SOURCE_DIR}/shared

    # For SFML <= 2.4
    ${SFML_INCLUDE_DIR}
)
target_link_libraries(event_handling
    PRIVATE
    # For SFML <= 2.4
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES}
    # For SFML >= 2.5
    sfml-window
)

