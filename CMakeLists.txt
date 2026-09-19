# CMakeList.txt : CMake project for pong, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Required for CMAKE_MSVC_RUNTIME_LIBRARY to be honoured; the ci-windows preset
# uses it to link the CRT statically so the released .exe needs no VC++
# redistributable installed.
if (POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()

project ("pong")

set (SOURCES
	"src/pong.cpp"
	"src/game.cpp"
	"src/player.cpp"
	"src/ball.cpp"
	"src/score.cpp"
	"src/net.cpp"
	"src/clock.cpp"
	"src/fps_calculator.cpp"
	"src/sound.cpp"
)


# Add source to this project's executable.
add_executable (pong ${SOURCES})

target_include_directories(pong PRIVATE include)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET pong PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(pong PRIVATE SDL3::SDL3)

# Application icon.
if (WIN32)
  enable_language(RC)
  target_sources(pong PRIVATE "assets/pong.rc")
  # GUI subsystem; without it MSVC links the console subsystem and Windows opens
  # a console window behind the game. Requires SDL_main.h in pong.cpp, which
  # supplies the WinMain this subsystem entry point needs.
  set_target_properties(pong PROPERTIES WIN32_EXECUTABLE TRUE)
elseif (APPLE)
  set(PONG_ICNS "${CMAKE_CURRENT_SOURCE_DIR}/assets/pong.icns")
  target_sources(pong PRIVATE "${PONG_ICNS}")
  # Both halves are required: this copies the file into the bundle, and
  # MACOSX_BUNDLE_ICON_FILE below names it in Info.plist.
  set_source_files_properties("${PONG_ICNS}" PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources")
endif()

# Packaging.
if (APPLE)
  set_target_properties(pong PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME "Pong"
    MACOSX_BUNDLE_GUI_IDENTIFIER "io.github.pong"
    MACOSX_BUNDLE_ICON_FILE "pong.icns"
  )
  install(TARGETS pong BUNDLE DESTINATION .)
  set(CPACK_GENERATOR "DragNDrop")
  set(CPACK_DMG_VOLUME_NAME "Pong")
else()
  install(TARGETS pong RUNTIME DESTINATION bin)
endif()

include(CPack)