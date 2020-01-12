PROJECT(bingo)

SET(PROJECT_SRCS
    bingo.cpp
    card.cpp
    generator.cpp
    lua.cpp
    objective_event.cpp
    objective_fail.cpp
    objective_generic.cpp
    objective_item.cpp
    objective_population.cpp
    objective_survival.cpp
    persistence.cpp
    screen.cpp
    square.cpp
    util.cpp
    weblegends.cpp
    win_condition_generic.cpp
)

SET(PROJECT_HDRS
    bingo.h
    card.h
    generator.h
    square.h
    util.h
    weblegends-plugin.h
)

SET_SOURCE_FILES_PROPERTIES(${PROJECT_HDRS} PROPERTIES HEADER_FILE_ONLY TRUE)

LIST(APPEND PROJECT_SRCS ${PROJECT_HDRS})

LIST(APPEND PROJECT_LIBS jsoncpp_lib_static lua)

INSTALL(DIRECTORY lua/
        DESTINATION ${DFHACK_LUA_DESTINATION}/plugins
        FILES_MATCHING PATTERN "*.lua")

DFHACK_PLUGIN(bingo ${PROJECT_SRCS} LINK_LIBRARIES ${PROJECT_LIBS} COMPILE_FLAGS_GCC "-Wall -Wextra -Werror" COMPILE_FLAGS_MSVC "/W3")
