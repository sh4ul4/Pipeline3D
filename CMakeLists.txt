# CMakeList.txt : projet CMake pour Pipeline3d, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.9)

project ("Pipeline3d")

project(Pipeline3d LANGUAGES CUDA CXX)

enable_language(CUDA) 

# Utiliser C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(CUDA 10.1 REQUIRED)

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
INCLUDE_DIRECTORIES( ${CUDA_INCLUDE_DIRS})

#SET(GCC_COVERAGE_COMPILE_FLAGS "--expt-relaxed-constexpr")
#SET(GCC_COVERAGE_LINK_FLAGS    "--expt-relaxed-constexpr")

#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
# SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE})

set(SRCS	"app/src/main.cu")

#add_compile_options(--expt-relaxed-constexpr)
#set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --expt-relaxed-constexpr")
#set( CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS}" "--expt-relaxed-constexpr" )

set(HEADERS app/headers/rasterization/Bitmap.hpp
			app/headers/geometry/Camera.hpp
			app/headers/display/Chrono.hpp
			app/headers/display/Color.hpp
			app/headers/display/Draw.hpp
			app/headers/userinteraction/ErrorsHandler.hpp
			app/headers/display/FrameRate.hpp
			app/headers/rasterization/GlobalTexture.hpp
			app/headers/headers.hpp
			app/headers/userinteraction/InputEvent.hpp
			app/headers/userinteraction/Keyboard.hpp
			app/headers/geometry/Maths.hpp
			app/headers/geometry/Matrix.hpp
			app/headers/userinteraction/Mouse.hpp
			app/headers/geometry/Physics.hpp
			app/headers/geometry/Point.hpp
			app/headers/display/Render.hpp
			app/headers/geometry/Shape.hpp
			app/headers/geometry/ShapeManager.hpp
			app/headers/userinteraction/TextBox.hpp
			app/headers/userinteraction/TextInput.hpp
			app/headers/display/Texture.hpp
			app/headers/rasterization/TextureManager.hpp
			app/headers/geometry/Triangle.hpp
			app/headers/geometry/Vector.hpp
			app/headers/geometry/Vertex.hpp
			app/headers/display/Window.hpp
			app/headers/userinteraction/Button.hpp
			app/headers/userinteraction/ScrollArea.hpp
			)



# Ajoutez une source à l'exécutable de ce projet.
add_executable (Pipeline3d  ${SRCS} ${HEADERS})

target_include_directories(Pipeline3d PRIVATE headers)

target_link_libraries(Pipeline3d ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${CUDA_INCLUDE_DIRS})

set_target_properties( Pipeline3d
                       PROPERTIES CUDA_SEPARABLE_COMPILATION ON)


if (WIN32)
	file( 
	COPY ${SDL2_DLL} ${SDL2_mixer_DLL} ${SDL2_ttf_DLL} ${SDL2_image_DLL}
	DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
endif()
# TODO: Ajoutez des tests et installez des cibles si nécessaire.
