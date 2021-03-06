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

#include <anim/terrain/terrain.hpp>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

namespace charanim {

// PRIVATE

// PUBLIC

terrain::terrain() {
	dimX = dimY = 0.0f;
	rg = nullptr;
}

terrain::~terrain() {
	clear();
}

// MODIFIERS

void terrain::clear() {
	sgs.clear();
	if (rg != nullptr) {
		rg->clear();
		delete rg;
		rg = nullptr;
	}
}

// SETTERS

// GETTERS

const std::vector<segment>& terrain::get_segments() const {
	return sgs;
}

regular_grid *terrain::get_regular_grid() {
	return rg;
}

const regular_grid *terrain::get_regular_grid() const {
	return rg;
}

// I/O

bool terrain::read_map(const string& filename) {
	clear();

	ifstream fin;
	fin.open(filename.c_str());
	if (not fin.is_open()) {
		cerr << "terrain::read_map - Error (" << __LINE__ << "):" << endl;
		cerr << "    Could not read map file: '" << filename << "'" << endl;
		return false;
	}

	path_finder_type pf_type = path_finder_type::none;

	bool res_read = false;
	size_t resX, resY;
	resX = resY = 0;

	string keyword;

	while (fin >> keyword) {
		if (keyword == "type") {
			fin >> keyword;
			if (keyword == "regular_grid") {
				pf_type = path_finder_type::regular_grid;
			}
			else {
				cerr << "terrain::read_map - Error (" << __LINE__ << "):" << endl;
				cerr << "    Invalid type '" << keyword << "'" << endl;
				return false;
			}
		}
		else if (keyword == "resolution") {
			fin >> resX >> resY;
			res_read = true;
		}
		else if (keyword == "dimensions") {
			fin >> dimX >> dimY;
		}
		else if (keyword == "wall") {
			segment s;
			fin >> s.first.x  >> s.first.y
				>> s.second.x >> s.second.y;
			sgs.push_back(s);
		}
		else {
			cerr << "terrain::read_map - Error (" << __LINE__ << "):" << endl;
			cerr << "    Unrecognized keywork '" << keyword << "'" << endl;
			return false;
		}
	}
	fin.close();

	if (pf_type == path_finder_type::none) {
		cerr << "terrain::read_map - Error (" << __LINE__ << "):" << endl;
		cerr << "    Path finder type not found" << endl;
		cerr << "    Include a line with the following format:" << endl;
		cerr << "        type TYPE" << endl;
		cerr << "    where TYPE is one of the following:" << endl;
		cerr << "        regular_grid" << endl;
		return false;
	}

	// make walls of the quadrilateral
	segment wall1(vec2(0,0), vec2(dimX-1,0));
	segment wall2(vec2(0,0), vec2(0, dimY-1));
	segment wall3(vec2(dimX-1,0), vec2(dimX-1, dimY-1));
	segment wall4(vec2(0,dimY-1), vec2(dimX-1, dimY-1));

	if (pf_type == path_finder_type::regular_grid) {
		if (not res_read) {
			cerr << "terrain::read_map - Error (" << __LINE__ << "):" << endl;
			cerr << "    Resolution not found" << endl;
			cerr << "    Include a line with the following format:" << endl;
			cerr << "        resolution RX RY" << endl;
			cerr << "    where RX and RY are, respectively, the amount of cells" << endl;
			cerr << "    in each dimension." << endl;
			return false;
		}

		rg = new regular_grid();
		rg->init(resX, resY, dimX, dimY);
		rg->init(sgs);
		rg->expand_function_distance(segment(vec2(-1,-1), vec2(dimX, -1)));
		rg->expand_function_distance(segment(vec2(-1,-1), vec2(-1, dimY)));
		rg->expand_function_distance(segment(vec2(dimX,-1), vec2(dimX, dimY)));
		rg->expand_function_distance(segment(vec2(-1,dimY), vec2(dimX, dimY)));

		rg->make_final_state();
	}

	sgs.push_back(wall1);
	sgs.push_back(wall2);
	sgs.push_back(wall3);
	sgs.push_back(wall4);
	return true;
}

} // -- namespace charanim
