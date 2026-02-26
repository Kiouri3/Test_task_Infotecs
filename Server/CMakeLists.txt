set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(Server
  main.cpp
  server.h server.cpp
  ConsoleReader.h
)

target_link_libraries(Server PRIVATE 
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Network
)
