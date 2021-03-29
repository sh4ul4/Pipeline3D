# CMakeList.txt : projet CMake pour Pipeline3d, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

project ("Pipeline3d")

if (WIN32)
	list(APPEND CMAKE_PREFIX_PATH ./SDL)
	list(APPEND CMAKE_PREFIX_PATH ./SDL/Release)
	list(APPEND CMAKE_PREFIX_PATH ./SDL/Debug)
	find_package(SDL2 REQUIRED)
	find_package(SDL2_ttf REQUIRED)
	find_package(SDL2_mixer REQUIRED)
	find_package(SDL2_image REQUIRED)
else()
	find_package(PkgConfig REQUIRED) 
	pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_ttf SDL2_mixer SDL2_image)
endif()
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE})

set(SRCS	src/main.cpp
			headers/Point.cpp
			headers/Bitmap.cpp)

set(HEADERS headers/Camera.h
			headers/Draw.h
			headers/library.h
			headers/Matrix.h
			headers/Physics.h
			headers/Shape.h
			headers/ShapeManager.h
			headers/Vertex.h
			headers/Render.h
			headers/FrameRate.h
			headers/Window.h
			headers/Triangle.h
			#headers/Point.h
			headers/Input.h
			headers/Bitmap.hpp
			headers/Texture.h
			headers/Point.hpp)



# Ajoutez une source à l'exécutable de ce projet.
add_executable (Pipeline3d  ${SRCS} ${HEADERS})

target_include_directories(Pipeline3d PRIVATE headers)

target_link_libraries(Pipeline3d ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})


if (WIN32)
	file( 
	COPY ${SDL2_DLL} ${SDL2_mixer_DLL} ${SDL2_ttf_DLL} ${SDL2_image_DLL}
	DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
endif()
# TODO: Ajoutez des tests et installez des cibles si nécessaire.
