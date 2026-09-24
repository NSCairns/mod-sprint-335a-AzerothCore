set(CMAKE_CXX_STANDARD 17)

AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/PlayerSprintHandler.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/SprintConfig.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/SprintWorldScript.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/SprintCommandScript.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/SprintLoader.cpp")

install(FILES conf/mod-sprint.conf.dist DESTINATION etc/modules)
