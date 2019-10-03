/*********************************************************************
 * charanim - Character Animation Project
 * Copyright (C) 2018 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#pragma once

/*
 * As whoever is reading this may know, std_image.h is a file that
 * is part of a well-know C++ header-only library.
 *
 * The library can be downloaded from the repository
 *		https://github.com/nothings/stb
 *
 * The file added to this project was downloaded and copied 'as is'
 * from this repository. No changes were made to the file.
 *
 * This file (std_image.hpp) was created following the instructions
 * in:
 *		https://learnopengl.com/Getting-started/Textures
 * in the section "Loading and creating textures".
 */

#define STB_IMAGE_IMPLEMENTATION
#include <render/textures/stb_image.h>
