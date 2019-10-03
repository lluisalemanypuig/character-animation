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

class perspective : public observer {
	private:
		float raw;
		float d;

		// angles (radians)
		float alpha;
		float initial_alpha;

	public:
		perspective();
		perspective(const perspective& p);
		~perspective();

		void init_camera(const box& b);
		void make_Zs(const box& b);

		void adapt_camera_to_viewport(int w, int h);

		// a: angle (degrees)
		void set_zoom(float a);
		void zoom(float a);
		void reset_zoom();

		float get_zoom() const;

		// retorna la distancia de la posicio de la camera al centre de l'escena
		float getD() const;

		float getRAw() const;
		// returns the Field Of View in degrees
		float get_FOV() const;
		float get_window_height() const;
		float get_window_width() const;
};
