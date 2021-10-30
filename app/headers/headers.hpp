#pragma once

#define _CUDA
#define _CUDA_DEBUG

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_thread.h>

#include <list>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>

#include <string>

#include <fstream>
#include <iostream>
#include <stdio.h>

#include <algorithm>
#include <math.h>
#include <climits>

#include <assert.h>

#include <filesystem>

#include "userinteraction/ErrorsHandler.hpp"

#include "geometry/Point.hpp"

#include "geometry/Vertex.hpp"
#include "geometry/Vector.hpp"

#include "display/Color.hpp"

#include "display/Draw.hpp"

#include "userinteraction/TextBox.hpp"

#include "display/Window.hpp"

#include "display/Chrono.hpp"
#include "display/FrameRate.hpp"

#include "rasterization/Bitmap.hpp"

#include "geometry/Matrix.hpp"
#include "geometry/Maths.hpp"

#include "display/Texture.hpp"
#include "rasterization/GlobalTexture.hpp"
#include "rasterization/TextureManager.hpp"

#include "userinteraction/Mouse.hpp"
#include "userinteraction/Keyboard.hpp"
#include "userinteraction/InputEvent.hpp"

#include "geometry/Camera.hpp"

#include "geometry/Triangle.hpp"

#include "geometry/Shape.hpp"

#include "geometry/ShapeManager.hpp"

#include "geometry/Physics.hpp"

#include "display/Render.hpp"

#include "userinteraction/TextInput.hpp"

#include "userinteraction/Button.hpp"

#include "userinteraction/ScrollArea.hpp"
