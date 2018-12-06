#include <anim/terrain/terrain.hpp>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// charanim includes
#include <anim/terrain/regular_grid/regular_grid.hpp>

namespace charanim {

// PRIVATE

// PUBLIC

terrain::terrain() {
	dimX = dimY = 0.0f;
	pf = nullptr;
}

terrain::~terrain() {
	clear();
}

// MODIFIERS

void terrain::clear() {
	sgs.clear();
	if (pf != nullptr) {
		pf->clear();
		delete pf;
		pf = nullptr;
	}
}

// SETTERS

// GETTERS

const std::vector<segment>& terrain::get_segments() const {
	return sgs;
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
		cout << "keyword: " << keyword << endl;
		if (keyword == "type") {
			fin >> keyword;
			if (keyword == "regular_grid") {
				pf_type = path_finder_type::regular_grid;
			}
			else if (keyword == "visibility_graph") {
				pf_type = path_finder_type::visibility_graph;
			}
			else {
				cerr << "terrain::read_map - Error (" << __LINE__ << "):" << endl;
				cerr << "    Invalid type '" << keyword << "'" << endl;
				return false;
			}
		}
		else if (keyword == "resolution") {
			fin >> resX >> resY;
			cout << resX << "," << resY << endl;
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
		cerr << "        visibility_graph" << endl;
		return false;
	}

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

		pf = new regular_grid();

		regular_grid *rg = static_cast<regular_grid *>(pf);
		rg->init(resX, resY, dimX, dimY);
		rg->init(sgs);
	}
	else if (pf_type == path_finder_type::visibility_graph) {

	}

	// make walls of the quadrilateral
	segment wall1(point2D(0,0), point2D(dimX, 0));
	segment wall2(point2D(0,0), point2D(0, dimY));
	segment wall3(point2D(dimX,0), point2D(dimX, dimY));
	segment wall4(point2D(0,dimY), point2D(dimX, dimY));
	sgs.push_back(wall1);
	sgs.push_back(wall2);
	sgs.push_back(wall3);
	sgs.push_back(wall4);

	return true;
}

} // -- namespace charanim
