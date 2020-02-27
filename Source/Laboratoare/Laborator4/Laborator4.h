#pragma once
#include <Component/SimpleScene.h>

class Laborator4 : public SimpleScene
{
	public:
		Laborator4();
		~Laborator4();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		void Laborator4::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
		float OZcloud_locations[18];
		float OYcloud_locations[18];
		float cloud_scale[18];
		int number_of_clouds = 0;
		float cloud_speed = 2.5f;
		float time_to_generate = 1.8f;
		float OXposition = -2.5f;
		float OZPosition = -2.0f;
		float OYPosition = 0.5f; //de calculat la fiecare cadru
		float verticalRotation = 0;
		float verticalSpeed = 1.3f;
		float planeFrame = 1.0f;
		float cloudFrame = 1.0f;
		float planeAnimationSpeed = 110.0f;
		float cloudSpeed[18];
		int mouseYposition = 0;

		float OZobstacle_locations[18];
		float OYobstacle_locations[18];
		float obstacle_speed = 3.0f;
		float time_to_generate_obstacle = 0.9f;
		int number_of_obstacles = 0;
		int number_of_lives = 3;

	protected:
		glm::mat4 modelMatrix;
		float translateX, translateY, translateZ;
		float scaleX, scaleY, scaleZ;
		float angularStepOX, angularStepOY, angularStepOZ;
		GLenum polygonMode;
};
