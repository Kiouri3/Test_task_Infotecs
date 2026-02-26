cmake_minimum_required(VERSION 3.16)
project(IntegralDistributedSystem)

# Установка стандарта C++17 для всего проекта [cite: 10]
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск необходимых компонентов Qt для всех подпроектов
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

# Поиск библиотеки для работы с потоками (необходимо для std::thread на Linux) [cite: 16]
find_package(Threads REQUIRED)

# Подключаем папки с подпроектами
add_subdirectory(Server)
add_subdirectory(Client)