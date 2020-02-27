#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h>

#include <Core/Engine.h>
#include "Transform3D.h"
#include "../Laborator3/Transform2D.h"
#include "../Laborator5/LabCamera.h"

using namespace std;


Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

Mesh* plane_animation[21];
Mesh* cloud_animation[22];

void Tema2::Init()
{

	polygonMode = GL_FILL;
	srand(time(NULL));
	Mesh* mesh = new Mesh("2Dplane");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
	meshes[mesh->GetMeshID()] = mesh;

	mesh = new Mesh("obstacle");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[mesh->GetMeshID()] = mesh;

	mesh = new Mesh("cylinder");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "cylinder.obj");
	meshes["cylinder"] = mesh;

	mesh = new Mesh("heart");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "heart.obj");
	meshes["heart"] = mesh;

	mesh = new Mesh("fuel_gauge");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "fuel_gauge.obj");
	meshes["fuel_gauge"] = mesh;

	mesh = new Mesh("gas_can");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "gas_can.obj");
	meshes["gas_can"] = mesh;

	{
		puts("Loading shader...");
		Shader* shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
		puts("DONE");
	}


	{
		puts("Loading plane...");
		for (int i = 1; i <= 20; i++) {
			char buffer[3];
			_itoa(i, buffer, 10);
			if (i < 10) {
				buffer[2] = '\0';
				buffer[1] = buffer[0];
				buffer[0] = '0';
			}
			else {
				buffer[2] = '\0';
			}
			std::string obj_name("plane_0000");
			obj_name = obj_name + buffer + ".obj";
			plane_animation[i] = new Mesh("plane" + std::string(buffer));
			plane_animation[i]->LoadMesh(RESOURCE_PATH::MODELS + "plane_animation", obj_name);
		}
		puts("DONE");
		puts("Loading clouds...");
		for (int i = 1; i <= 21; i++) {
			char buffer[3];
			_itoa(i, buffer, 10);
			if (i < 10) {
				buffer[2] = '\0';
				buffer[1] = buffer[0];
				buffer[0] = '0';
			}
			else {
				buffer[2] = '\0';
			}
			std::string obj_name("cloud_0000");
			obj_name = obj_name + buffer + ".obj";
			cloud_animation[i] = new Mesh("cloud" + std::string(buffer));
			cloud_animation[i]->LoadMesh(RESOURCE_PATH::MODELS + "cloud_animation", obj_name);
		}
		puts("DONE");
	}

	this->GetSceneCamera()->SetPositionAndRotation(glm::vec3(0, 1, 3.5f), glm::quat());
	puts("Press SPACE to start...");
}

bool start = false;


void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	shader->Use();

	int location;
	location = glGetUniformLocation(shader->program, "my_color");
	glUniform3fv(location, 1, glm::value_ptr(color));
	

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));



	// Draw the object
	mesh->Render();
}



void Tema2::Update(float deltaTimeSeconds)
{
	glm::mat4 modelMatrix;
	glLineWidth(3);
	glPointSize(5);
	//adaug background
	{
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-2.5f, 0.5f, -3.0f);
		modelMatrix *= Transform3D::RotateOX(3.1415f / 2);
		modelMatrix *= Transform3D::Scale(1.0f, 1.0f, 1.0f);
		RenderSimpleMesh(meshes["2Dplane"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0, 1, 1));
	}

	//adaugam marea
	{
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Scale(4.0f, 1.0f, 4.0f);
		modelMatrix *= Transform3D::Translate(0.0f, -0.6f, 0.0f);
		RenderSimpleMesh(meshes["cylinder"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0, 0, 1));
	}

	//calculez pozitia si unghiul avionului
	{
		float OYTargetPosition = -0.008f * mouseYposition + 3.8f;
		if (OYTargetPosition > OYPosition) {
			float difference = OYTargetPosition - OYPosition;
			float distance = verticalSpeed * deltaTimeSeconds;
			if (distance > difference) {
				OYPosition = OYTargetPosition;
			}
			else {
				OYPosition += distance;
			}
			if (difference >= 1) {
				verticalRotation = 3.1415f / 4;
			}
			else {
				verticalRotation = 3.1415 / 4 * difference;
			}
		}
		else {
			float difference = OYPosition - OYTargetPosition;
			float distance = verticalSpeed * deltaTimeSeconds;
			if (distance > difference) {
				OYPosition = OYTargetPosition;
			}
			else {
				OYPosition -= distance;
			}
			if (difference >= 1) {
				verticalRotation = -3.1415f / 4;
			}
			else {
				verticalRotation = -3.1415 / 4 * difference;
			}
		}

		if (OYPosition > 2.35f) {
			OYPosition = 2.35f;
		}
		else {
			if (OYPosition < -0.9f) {
				OYPosition = -0.9f;
			}
		}
	}
	
	//calculez numarul cadrului pentru animatia avionului si pentru nori
	{
		planeFrame += planeAnimationSpeed * deltaTimeSeconds;
		if (planeFrame > 20.0f) {
			planeFrame = 1.0f;
		}

		cloudFrame += (30.0f) * deltaTimeSeconds;
		if (cloudFrame > 21.0f) {
			cloudFrame = 1.0f;
		}
	}
	
	
	//incarc modelul avionului pe ecran
	{
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(OXposition, OYPosition, OZPosition);
		modelMatrix *= Transform3D::RotateOY(3 * 3.1415f / 2);
		modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
		modelMatrix *= Transform3D::RotateOX(verticalRotation);
		modelMatrix *= Transform3D::RotateOY(0.2f);
		RenderSimpleMesh(plane_animation[(int)planeFrame], shaders["ShaderTema2"], modelMatrix, glm::vec3(0, 1.0f, 0.15f));
	}
	

	//generez nori
	{
		time_to_generate -= deltaTimeSeconds;
		if (time_to_generate <= 0.0f) {
			time_to_generate = 1.8f;
			cloud_scale[number_of_clouds] = 0.03f * (rand() % 10) + 0.09f;
			OYcloud_locations[number_of_clouds] = 0.2f * (rand() % 10) + 0.55f;
			cloudSpeed[number_of_clouds] = 0.5f * (rand() % 10 + 1);
			OXcloud_locations[number_of_clouds++] = 6.0f;
		}
	}
	

	//generez obstacole 
	if(start){
		time_to_generate_obstacle -= deltaTimeSeconds;
		if (time_to_generate_obstacle <= 0.0f) {
			time_to_generate_obstacle = 0.4f;
			OYobstacle_locations[number_of_obstacles] = 0.2f * (rand() % 10) + 0.55f;
			OXobstacle_locations[number_of_obstacles++] = 6.0f;
		}
	}
	
	//generez combustibil 
	if(start){
		time_to_generate_fuel -= deltaTimeSeconds;
		if (time_to_generate_fuel <= 0.0f) {
			time_to_generate_fuel = 4.0f;
			OYfuel_location = 0.2f * (rand() % 10) + 0.55f;
			OXfuel_location = 5.5f;
		}
	}

	//elimin norii care au parasit scena
	{
		for (int i = 0; i < number_of_clouds; i++) {
			if (OXcloud_locations[i] < -6.0f) {
				for (int j = i; j < number_of_clouds - 1; j++) {
					OYcloud_locations[j] = OYcloud_locations[j + 1];
					OXcloud_locations[j] = OXcloud_locations[j + 1];
					cloud_scale[j] = cloud_scale[j + 1];
					cloudSpeed[j] = cloudSpeed[j + 1];
				}
				i--;
				number_of_clouds--;
			}
		}
	}

	//elimin obstacole care au parasit scena
	if(start){
		for (int i = 0; i < number_of_obstacles; i++) {
			if (OXobstacle_locations[i] < -6.0f) {
				for (int j = i; j < number_of_obstacles - 1; j++) {
					OYobstacle_locations[j] = OYobstacle_locations[j + 1];
					OXobstacle_locations[j] = OXobstacle_locations[j + 1];
				}
				i--;
				number_of_obstacles--;
			}
		}
	}

	//deplasez norii
	{
		for (int i = 0; i < number_of_clouds; i++) {
			OXcloud_locations[i] -= cloudSpeed[i] * deltaTimeSeconds;
		}
	}

	//deplasez obstacolele 
	if(start){
		for (int i = 0; i < number_of_obstacles; i++) {
			OXobstacle_locations[i] -= obstacle_speed * deltaTimeSeconds;
		}
	}

	//deplasez combustibilul 
	if(start){
		OXfuel_location -= 2.55f * deltaTimeSeconds;
	}

	//incarc norii pe ecran
	{
		glm::vec3 cloud_color = glm::vec3(1, 1, 1);
		for (int i = 0; i < number_of_clouds; i++) {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(OXcloud_locations[i], OYcloud_locations[i], OZPosition);
			modelMatrix *= Transform3D::RotateOY(3 * 3.1415f / 2);
			modelMatrix *= Transform3D::Scale(cloud_scale[i], cloud_scale[i], cloud_scale[i]);
			RenderSimpleMesh(cloud_animation[(int)cloudFrame], shaders["ShaderTema2"], modelMatrix, cloud_color);
		}
	}

	//incarc obstacolele pe ecran
	if(start){
		glm::vec3 obstacle_color = glm::vec3(1, 0, 1);
		for (int i = 0; i < number_of_obstacles; i++) {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(OXobstacle_locations[i], OYobstacle_locations[i], OZPosition);
			modelMatrix *= Transform3D::RotateOY(3 * 3.1415f / 2);
			modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
			RenderSimpleMesh(meshes["obstacle"], shaders["ShaderTema2"], modelMatrix, obstacle_color);
		}
	}

	//incarc combustibilul pe ecran
	if(start){
		
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(OXfuel_location, OYfuel_location, OZPosition);
		modelMatrix *= Transform3D::RotateOY(3 * 3.1415f / 2);
		modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
		RenderSimpleMesh(meshes["obstacle"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1.0f, 0.5f, 0));
		
	}

	//incarc pe ecran numarul de vieti 
	{
		if (number_of_lives == 3 && !firstPerson) {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(OXposition - 0.2f, OYPosition + 0.25f, OZPosition);
			modelMatrix *= Transform3D::Scale(0.0009f, 0.0009f, 0.0009f);
			modelMatrix *= Transform3D::RotateOY(3.1415f / 2);
			RenderSimpleMesh(meshes["heart"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));

			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(OXposition - 0.5f, OYPosition + 0.25f, OZPosition);
			modelMatrix *= Transform3D::Scale(0.0009f, 0.0009f, 0.0009f);
			modelMatrix *= Transform3D::RotateOY(3.1415f / 2);
			RenderSimpleMesh(meshes["heart"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));

			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(OXposition + 0.1f, OYPosition + 0.25f, OZPosition);
			modelMatrix *= Transform3D::Scale(0.0009f, 0.0009f, 0.0009f);
			modelMatrix *= Transform3D::RotateOY(3.1415f / 2);
			RenderSimpleMesh(meshes["heart"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));
		}
		else if (number_of_lives == 2 && !firstPerson) {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(OXposition - 0.05f, OYPosition + 0.25f, OZPosition);
			modelMatrix *= Transform3D::Scale(0.0009f, 0.0009f, 0.0009f);
			modelMatrix *= Transform3D::RotateOY(3.1415f / 2);
			RenderSimpleMesh(meshes["heart"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));

			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(OXposition - 0.35f, OYPosition + 0.25f, OZPosition);
			modelMatrix *= Transform3D::Scale(0.0009f, 0.0009f, 0.0009f);
			modelMatrix *= Transform3D::RotateOY(3.1415f / 2);
			RenderSimpleMesh(meshes["heart"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));

		}
		else if (number_of_lives == 1 && !firstPerson) {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(OXposition - 0.2f, OYPosition + 0.25f, OZPosition);
			modelMatrix *= Transform3D::Scale(0.0009f, 0.0009f, 0.0009f);
			modelMatrix *= Transform3D::RotateOY(3.1415f / 2);
			RenderSimpleMesh(meshes["heart"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));
		}
	}


	if(start){//verific daca avionul se ciocneste de un obstacol sau intra in apa
		
		{
			float XA = OXposition;
			float YA = OYPosition;
			float XB = OXposition + 0.12f;
			float YB = OYPosition;
			float alpha = verticalRotation;
			float XO = XA + sqrt((XA - XB) * (XA - XB) + (YA - YB) * (YA - YB))
				- 2 * ((XA - XB) * (XA - XB) + (YA - YB) * (YA - YB)) * cos(3.1415f / 2 - alpha) * (1 - cos(alpha));
			float YO = YA + 2 * ((XA - XB) * (XA - XB) + (YA - YB) * (YA - YB)) * (1 - cos(alpha));
			for (int i = 0; i < number_of_obstacles; i++) {
				float distance = sqrt((XO - OXobstacle_locations[i]) * (XO - OXobstacle_locations[i])
					+ (YO - OYobstacle_locations[i]) * (YO - OYobstacle_locations[i]));
				if (distance < 0.11f) {
					number_of_lives--;
					OYobstacle_locations[i] = -6.0f;
					break;
				}
			}
			if (YO < 0.42f) {
				puts("Game over!(the plane crashed in the sea)");
				exit(0);
			}
			{
				float distance = sqrt((XO - OXfuel_location) * (XO - OXfuel_location)
					+ (YO - OYfuel_location) * (YO - OYfuel_location));
				if (distance < 0.11f) {
					fuel = 1.0f;
					OXfuel_location = -6.0f;
				}
			}
		}
		{
			float XA = OXposition;
			float YA = OYPosition;
			float XB = OXposition - 0.4f;
			float YB = OYPosition;
			float alpha = verticalRotation;
			float XO = XA - sqrt((XA - XB) * (XA - XB) + (YA - YB) * (YA - YB))
				+ 2 * ((XA - XB) * (XA - XB) + (YA - YB) * (YA - YB)) * cos(3.1415f / 2 - alpha) * (1 - cos(alpha));
			float YO = YA - 2 * ((XA - XB) * (XA - XB) + (YA - YB) * (YA - YB)) * (1 - cos(alpha));
			for (int i = 0; i < number_of_obstacles; i++) {
				float distance = sqrt((XO - OXobstacle_locations[i]) * (XO - OXobstacle_locations[i])
					+ (YO - OYobstacle_locations[i]) * (YO - OYobstacle_locations[i]));
				if (distance < 0.1f) {
					number_of_lives--;
					OYobstacle_locations[i] = -6.0f;
					break;
				}
			}
			if (YO < 0.42f) {
				puts("Game over!(the plane crashed int the sea)");
				exit(0);
			}
			{
				float distance = sqrt((XO - OXfuel_location) * (XO - OXfuel_location)
					+ (YO - OYfuel_location) * (YO - OYfuel_location));
				if (distance < 0.11f) {
					fuel = 1.0f;
					OXfuel_location = -6.0f;
				}
			}
		}
	}
	//verific daca am ramas fara vieti
	{	
		if (number_of_lives <= 0) {
			puts("Game over!(out of lives)");
			exit(0);
		}
	}
	

	//actuzlizez combustibilul
	if(start){
		fuel -= (deltaTimeSeconds) / 25;
	}

	//verific daca am ramas fara combustibil 
	{
		if (fuel <= 0.0f) {
			puts("Game over!(out of fuel)");
			exit(0);
		}
	}
	//incarc pe ecran bara de combustibil
	{
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(OXposition - 0.11f, OYPosition + 0.1f, OZPosition);
		modelMatrix *= Transform3D::Scale(0.065f * fuel, 0.065f, 0.065f);
		modelMatrix *= Transform3D::RotateOY(3.1415f / 2);
		RenderSimpleMesh(meshes["fuel_gauge"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1.0f, 0.5f, 0));
	}

	if (firstPerson) {
		this->GetSceneCamera()->SetPosition(glm::vec3(OXposition + 0.3f, OYPosition, OZPosition));

	}
	else {
		this->GetSceneCamera()->SetPositionAndRotation(glm::vec3(0, 1, 3.5f), glm::quat());
	}
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(5.5f, 0.0f, 0.0f);
	modelMatrix *= Transform3D::RotateOZ(3.1415f / 2);
	RenderSimpleMesh(meshes["2Dplane"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0, 1, 1));
}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2 ::OnInputUpdate(float deltaTime, int mods)
{

}

void Tema2::OnKeyPress(int key, int mods)
{
	{
		if (key == GLFW_KEY_F) {
			firstPerson = !firstPerson;
			if (firstPerson) {
				this->GetSceneCamera()->SetPosition(glm::vec3(OXposition + 0.3f, OYPosition, OZPosition));
				this->GetSceneCamera()->RotateOY(-900);
			}
			else {
				this->GetSceneCamera()->SetPositionAndRotation(glm::vec3(0, 1, 3.5f), glm::quat());
			}
		}
		else if (key == GLFW_KEY_SPACE && !start) {
			start = true;
		} 
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{	
	mouseYposition = mouseY;
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
