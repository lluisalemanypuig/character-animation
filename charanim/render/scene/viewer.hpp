#pragma once

// C++ inlcudes
#include <vector>

// glm includes
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

// render includes
#include <render/cameras/perspective.hpp>
#include <render/cameras/orthogonal.hpp>
#include <render/box.hpp>

/**
 * @brief Class used for rendering and navigating
 * a scene.
 *
 * This class does not contain the definition of
 * the functions GLUT needs.
 */
class viewer {
	private:

	protected:
		/// Bounding box of the models.
		box B;

		// Used only when flying.

		/// Position of the camera.
		glm::vec3 cam_pos;
		/// Camera pitch.
		float pitch;
		/// Camera yaw.
		float yaw;

		// Used only when inspecting

		/// Bounding box's diagonal length.
		float diag_length;
		/// View Reference Point.
		glm::vec3 VRP;
		/// Rotation angle theta.
		float theta;
		/// Rotation angle psi.
		float psi;

		/// Perspective camera.
		perspective pers_cam;
		/// Orthogonal camera.
		orthogonal orth_cam;

		/// Use perspective camera.
		bool use_perspective;
		/// Use orthogonal camera.
		bool use_orthogonal;

		/// Fix the camera so that we see the whole scene.
		bool inspect;
		/// The camera flies free in the scene.
		bool fly;

		/// Window  width.
		int win_width;
		/// Window height.
		int win_height;

	protected:
		/// Returns the ratio window width / window height.
		float get_aspect_ratio() const;

	public:
		/// Default constructor.
		viewer();
		/// Destructor.
		virtual ~viewer();

		// MODIFIERS

		/// Clears the models vector (see @ref ms).
		virtual void clear();

		/**
		 * @brief Initialise the perspective and orthogonal
		 * cameras.
		 *
		 * Needs:
		 * - models in simulation.
		 * - window size
		 */
		void init_cameras();

		/// Increments angle @ref theta.
		void increment_theta(float i);
		/// Increments angle @ref psi.
		void increment_psi(float i);

		/// Increments the pitch of the camera.
		void increment_pitch(float i);
		/// Increments the yaw of the camera.
		void increment_yaw(float i);

		/**
		 * @brief Increments the zoom of both cameras.
		 *
		 * In order to have the same 'zoom' for both cameras,
		 * parameter @e i is scaled with a factor of 0.142
		 * for the orthogonal camera.
		 */
		void increment_zoom(float i);

		void move_camera(float vel, float dir);
		void tilt_camera_up(float vel, float dir);

		void camera_forwards(float vel);
		void camera_backwards(float vel);
		void camera_sideways_left(float vel);
		void camera_sideways_right(float vel);

		void switch_to_perspective();
		void switch_to_orthogonal();

		void switch_to_inspection();
		void switch_to_flight();

		// SETTERS

		/**
		 * @brief Set window dimensions.
		 * @param w Width of the window.
		 * @param h Height of the window.
		 */
		void set_window_dims(int w, int h);

		void set_perspective(const perspective& p);
		void set_orthogonal(const orthogonal& o);

		void set_VRP(const glm::vec3& VRP);
		void set_theta(float t);
		void set_psi(float p);

		void set_viewer_pos(const glm::vec3& pos);
		void set_yaw(float y);
		void set_pitch(float p);

		// GETTERS

		/// Returns a reference to the perspective camera.
		perspective& get_perspective_camera();

		/// Returns a reference to the orthogonal camera.
		orthogonal& get_orthogonal_camera();

		/// Returns the position of the viewer.
		const glm::vec3& get_viewer_pos() const;
		/// Returns the yaw of the camera.
		float get_yaw() const;
		/// Returns the pitch of the camera.
		float get_pitch() const;

		const glm::vec3& get_VRP() const;
		float get_theta() const;
		float get_psi() const;

		/// Returns the bounding box of the scene.
		box& get_box();
		/// Returns the bounding box of the scene.
		const box& get_box() const;

		/// Returns the width of the window.
		int window_width() const;
		/// Returns the height of the window.
		int window_height() const;

		/// Returns whether the camera is flying.
		bool is_flying() const;
		/// Returns whether the camera is inspecting.
		bool is_inspecting() const;

		// OpenGL

		/**
		 * @brief Makes the corresponding calls to render the
		 * scene using the internal camera.
		 *
		 * If no shader is to be used, calls either gluPerspective, or glOrtho.
		 * If the renderer shaqer is used then sets the corresponding
		 * uniform values.
		 */
		void apply_projection() const;

		/// Returns the projection matrix corresponding to the active camera.
		glm::mat4 make_projection_matrix() const;

		/**
		 * @brief Move the scene according to the internal state.
		 *
		 * Either inspecting or flying.
		 */
		void apply_view() const;

		/// Returns the modelview matrix corresponding to the active viewing mode.
		glm::mat4 make_view_matrix() const;

		/// (Slow) Renders the models
		void slow_render_models() const;
};

