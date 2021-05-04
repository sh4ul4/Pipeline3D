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

set(SRCS	src/main.cpp)

add_compile_options(-Wall -g)

set(HEADERS headers/rasterization/Bitmap.hpp
			headers/geometry/Camera.hpp
			headers/display/Chrono.hpp
			headers/display/Color.hpp
			headers/display/Draw.hpp
			headers/userinteraction/ErrorsHandler.hpp
			headers/display/FrameRate.hpp
			headers/rasterization/GlobalTexture.hpp
			headers/headers.hpp
			headers/userinteraction/InputEvent.hpp
			headers/userinteraction/Keyboard.hpp
			headers/geometry/Maths.hpp
			headers/geometry/Matrix.hpp
			headers/userinteraction/Mouse.hpp
			headers/geometry/Physics.hpp
			headers/geometry/Point.hpp
			headers/display/Render.hpp
			headers/geometry/Shape.hpp
			headers/geometry/ShapeManager.hpp
			headers/userinteraction/TextBox.hpp
			headers/userinteraction/TextInput.hpp
			headers/display/Texture.hpp
			headers/rasterization/TextureManager.hpp
			headers/geometry/Triangle.hpp
			headers/geometry/Vector.hpp
			headers/geometry/Vertex.hpp
			headers/display/Window.hpp
			headers/userinteraction/Button.hpp
			headers/Luminary.hpp
			headers/Planet.hpp
			headers/Star.hpp
			headers/starSystem.hpp
			)



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
