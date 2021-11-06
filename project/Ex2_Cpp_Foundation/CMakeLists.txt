# Copy the exercise directory to SimpleRenderEngineProject/project
# Add to build list using add_subdirectory in the bottom of the main CMakeLists.txt file

add_executable(Exercise2-1 Exercise2-1-HelloPreprocessor/main.cpp)

add_executable(Exercise2-2 Exercise2-2-SquaredSecrets/main.cpp)

add_executable(Exercise2-3 Exercise2-3-MineSweeper/main.cpp Exercise2-3-MineSweeper/MineSweeper.cpp)

add_executable(Exercise2-3-GUI Exercise2-3-MineSweeper/main_gui.cpp Exercise2-3-MineSweeper/MineSweeper.cpp)
target_link_libraries(Exercise2-3-GUI ${all_libs})

# copy assets to binary dir
file(COPY Exercise2-3-MineSweeper/minesweeper.json Exercise2-3-MineSweeper/minesweeper.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY Exercise2-3-MineSweeper/minesweeper.json Exercise2-3-MineSweeper/minesweeper.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
file(COPY Exercise2-3-MineSweeper/minesweeper.json Exercise2-3-MineSweeper/minesweeper.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    file(COPY ${DLLFileList} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
    file(COPY ${DLLFileList} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
    file(COPY ${DLLFileList} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    set_target_properties(Exercise2-3-GUI PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)
ENDIF(WIN32)