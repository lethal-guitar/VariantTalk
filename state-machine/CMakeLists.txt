set(sources
    components.cpp
    components.hpp
    config.hpp
    enemy.cpp
    enemy.hpp
    main.cpp
    math.hpp
    player.cpp
    player.hpp
    projectiles.cpp
    projectiles.hpp
    sfml_interop.hpp
)

add_executable(state_machine ${sources})
target_include_directories(state_machine
    PRIVATE
    ${PROJECT_SOURCE_DIR}/shared

    # For SFML <= 2.4
    ${SFML_INCLUDE_DIR}
)
target_link_libraries(state_machine
    PRIVATE
    # For SFML <= 2.4
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES}
    # For SFML >= 2.5
    sfml-graphics
)
set_target_properties(state_machine PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# The game/state machine example triggers a lot of the following warnings,
# without them pointing to actual problems.
# In order to keep the code readable, we are less strict in this example
# compared to the others.
if(MSVC)
    target_compile_options(state_machine
        PRIVATE
        /wd4244
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(state_machine
        PRIVATE
        -Wno-conversion
        -Wno-float-equal
        -Wno-sign-compare
        -Wno-sign-conversion
    )
endif()

set(resource_files
    Base.png
    Circling.png
    DroidSans.ttf
    FlyOut.png
    FlyToCenter.png
    ShootingFromCenter.png
    opponent1.png
    opponent2.png
    player1.png
    player2.png
    space-bg.jpg
)

foreach(RESOURCE ${resource_files})
    configure_file(
        resources/${RESOURCE}
        ${CMAKE_BINARY_DIR}/resources/${RESOURCE}
        COPYONLY)
endforeach()
