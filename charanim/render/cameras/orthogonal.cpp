#include <render/cameras/orthogonal.hpp>

void orthogonal::iguala_NEW_OLD() {
	nleft = oleft;
	nright = oright;
	nbottom = obottom;
	ntop = otop;
	total_zoom = 0;
}

orthogonal::orthogonal() : observer() {
	oleft = -1;
	oright = 1;
	obottom = -1;
	otop = 1;
	iguala_NEW_OLD();
	total_zoom = 0;
}
orthogonal::orthogonal(const orthogonal& o) : observer(o) {
	oleft = o.oleft;
	oright = o.oright;
	obottom = o.obottom;
	otop = o.otop;
	nleft = o.nleft;
	nright = o.nright;
	nbottom = o.nbottom;
	ntop = o.ntop;
}

orthogonal::~orthogonal() {}

void orthogonal::init_camera(const box& b) {
	float R = b.get_diag_length()/2.0f;
	oleft = -R;
	oright = -oleft;
	obottom = -R;
	otop = -obottom;
	iguala_NEW_OLD();
	zN = -4*R;
	zF = 4*R;
}

void orthogonal::adapt_camera_to_viewport(const box& b, int w, int h) {
	//vec3 mig = b.get_middle_point();
	float R = b.get_diag_length()/2.0f;
	float r = ((float)w)/((float)h);
	if (w > h) {
		obottom = -R;
		otop = R;
		float newWidth = get_window_height()*r;
		oleft = -newWidth/2;
		oright = newWidth/2;
	}
	else {
		oleft = -R;
		oright = R;
		float newHeight = get_window_width()/r;
		obottom = -newHeight/2;
		otop = newHeight/2;
	}
	iguala_NEW_OLD();
}

void orthogonal::set_zoom(float incr) {
	float r = (nright - nleft)/(ntop - nbottom);
	if (r < 1) {
		if (nbottom - incr < ntop + incr) {
			nbottom -= incr;
			ntop += incr;
			float newWidth = (nright - nleft)*r;
			nleft = -newWidth/2;
			nright = newWidth/2;
			total_zoom = incr;
		}
	}
	else {
		if (nleft - incr < nright + incr) {
			nleft -= incr;
			nright += incr;
			float newHeight = (nright - nleft)/r;
			nbottom = -newHeight/2;
			ntop = newHeight/2;
			total_zoom = incr;
		}
	}
}

void orthogonal::zoom(float incr) {
	float r = (nright - nleft)/(ntop - nbottom);
	if (r < 1) {
		if (nbottom - incr < ntop + incr) {
			nbottom -= incr;
			ntop += incr;
			float newWidth = (nright - nleft)*r;
			nleft = -newWidth/2;
			nright = newWidth/2;
			total_zoom += incr;
		}
	}
	else {
		if (nleft - incr < nright + incr) {
			nleft -= incr;
			nright += incr;
			float newHeight = (nright - nleft)/r;
			nbottom = -newHeight/2;
			ntop = newHeight/2;
			total_zoom += incr;
		}
	}
}

void orthogonal::reset_zoom() {iguala_NEW_OLD();}

float orthogonal::get_zoom() const {return total_zoom;}

float orthogonal::get_window_width() const {return oright - oleft;}
float orthogonal::get_window_height() const {return otop - obottom;}
float orthogonal::get_RA_w() const {return get_window_width()/get_window_height();}

void orthogonal::set_left(float l) {nleft = l;}
float orthogonal::get_left() const {return nleft;}

void orthogonal::set_right(float r) {nright = r;}
float orthogonal::get_right() const {return nright;}

void orthogonal::set_top(float t) {ntop = t;}
float orthogonal::get_top() const {return ntop;}

void orthogonal::set_bottom(float b) {nbottom = b;}
float orthogonal::get_bottom() const {return nbottom;}
