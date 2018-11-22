#pragma once

class observer {
	protected:
		float zN, zF;

	public:
		observer();
		observer(const observer& o);
		~observer();

		void set_znear(double zn);
		float get_znear() const;

		void set_zfar(double zf);
		float get_zfar() const;
};
