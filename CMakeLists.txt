PROJECT(bingo)

SET(PROJECT_SRCS
    bingo.cpp
    goal.cpp
    goal_generic.cpp
    lua.cpp
    persistence.cpp
    rule.cpp
    rule_generic.cpp
    screen.cpp
    weblegends.cpp
)

SET(PROJECT_HDRS
    bingo.h
    goal.h
    rule.h
    weblegends-plugin.h
)

SET_SOURCE_FILES_PROPERTIES(${PROJECT_HDRS} PROPERTIES HEADER_FILE_ONLY TRUE)

LIST(APPEND PROJECT_SRCS ${PROJECT_HDRS})

LIST(APPEND PROJECT_LIBS jsoncpp_lib_static)

DFHACK_PLUGIN(bingo ${PROJECT_SRCS} LINK_LIBRARIES ${PROJECT_LIBS} COMPILE_FLAGS_GCC "-Wall -Wextra -Werror" COMPILE_FLAGS_MSVC "/W3")
