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

#include <render/cameras/observer.hpp>

observer::observer() : zN(-1), zF(1) { }
observer::observer(const observer& o) {
	zN = o.zN;
	zF = o.zF;
}

observer::~observer() { }

void observer::set_znear(double zn) {
	zN = zn;
}
float observer::get_znear() const {
	return zN;
}

void observer::set_zfar(double zf) {
	zF = zf;
}
float observer::get_zfar() const {
	return zF;
}

