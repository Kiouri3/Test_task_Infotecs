set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(Client
  main.cpp
  client.h client.cpp
  integrator.cpp integrator.h
)


target_link_libraries(Client PRIVATE 
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Network
    Threads::Threads
)
