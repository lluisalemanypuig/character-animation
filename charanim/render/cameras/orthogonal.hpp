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

// render includes
#include <render/cameras/observer.hpp>
#include <render/box.hpp>
using namespace std;

class orthogonal : public observer {
	private:
		float oleft, oright, obottom, otop;
		float nleft, nright, nbottom, ntop;

		void iguala_NEW_OLD();
		float total_zoom;

	public:
		orthogonal();
		orthogonal(const orthogonal& o);
		~orthogonal();

		void init_camera(const box& b);

		void adapt_camera_to_viewport(const box& b, int w, int h);

		void set_zoom(float inc);
		void zoom(float inc);
		void reset_zoom();

		float get_zoom() const;

		float get_window_width() const;
		float get_window_height() const;
		float get_RA_w() const;

		void set_left(float l);
		float get_left() const;

		void set_right(float r);
		float get_right() const;

		void set_top(float t);
		float get_top() const;

		void set_bottom(float b);
		float get_bottom() const;
};
