#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/Tema2_cam.h"

namespace m1
{
	class Tema2 : public gfxc::SimpleScene
	{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void RenderProjectiles();
		void RenderScene();
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));
		void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));
		void CheckAndResolveCollisions();

		void RenderInamic();

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		glm::mat4 modelMatrix;
		glm::mat4 viewMatrix;
		glm::vec3 lightPosition;
		unsigned int materialShininess;
		float materialKd;
		float materialKs;
		glm::vec3 tankPosition;
		float tankRotation;
		float capRotation;
		float timpCooldownJucator;
		float coolDownCurentJucator;
		float timpTotal;
		bool gameOver;

	protected:
		implemented::Camera* camera;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;
	
	};
}