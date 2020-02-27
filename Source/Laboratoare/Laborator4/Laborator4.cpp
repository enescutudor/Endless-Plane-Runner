#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h>

#include <Core/Engine.h>
#include "Transform3D.h"
#include "../Laborator3/Transform2D.h"

using namespace std;

vector<Mesh*> CreateCylinder(std::string name, float radius, float length, glm::vec3 color) {
	vector<VertexFormat> vertices1;
	vector<VertexFormat> vertices2;
	vector<VertexFormat> verticesSide;
	vector<unsigned short> indices1;
	vector<unsigned short> indices2;
	vector<unsigned short> indicesSide;
	unsigned short index = 0;

	vertices1.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
	vertices2.push_back(VertexFormat(glm::vec3(0, 0, length), color));
	index++;
	glm::vec3 point_pos1(radius, 0, 0);
	glm::vec3 point_pos2(radius, 0, length);
	glm::mat3 rotation_matrix(1);
	rotation_matrix = Transform2D::Rotate(0.01);

	for (float i = 0; i <= 2 * M_PI; i += 0.01) {
		point_pos1 = point_pos1 * rotation_matrix;
		point_pos2 = point_pos2 * rotation_matrix;
		vertices1.push_back(VertexFormat(point_pos1, color));
		vertices2.push_back(VertexFormat(point_pos2, color));
		verticesSide.push_back(VertexFormat(point_pos1, color));
		verticesSide.push_back(VertexFormat(point_pos2, color));

		indices1.push_back(index++);
		indices2.push_back(index++);
		indicesSide.push_back(index++);
		indicesSide.push_back(index++);
	}
	vector<Mesh*> cylinder;
	Mesh* circle1 = new Mesh(name + "1");
	Mesh* circle2 = new Mesh(name + "2");
	Mesh* side = new Mesh(name + "3");

	circle1->SetDrawMode(GL_TRIANGLE_FAN);
	circle2->SetDrawMode(GL_TRIANGLE_FAN);
	side->SetDrawMode(GL_TRIANGLE_STRIP);
	circle1->InitFromData(vertices1, indices1);
	circle2->InitFromData(vertices2, indices2);
	side->InitFromData(verticesSide, indicesSide);
	cylinder.push_back(circle1);
	cylinder.push_back(circle2);
	cylinder.push_back(side);

	return cylinder;

}

Laborator4::Laborator4()
{
}

Laborator4::~Laborator4()
{
}

//Mesh* plane_animation[21];
//Mesh* cloud_animation[22];

void Laborator4::Init()
{
	polygonMode = GL_FILL;
	srand(time(NULL));
	Mesh* mesh = new Mesh("2Dplane");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
	meshes[mesh->GetMeshID()] = mesh;

	mesh = new Mesh("obstacle");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[mesh->GetMeshID()] = mesh;

	{
		Shader* shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Laborator4/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator4/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
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
	
}


/*float OZcloud_locations[18];
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
int number_of_lives = 3;*/

void Laborator4::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator4::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
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



void Laborator4::Update(float deltaTimeSeconds)
{
	glm::mat4 modelMatrix;
	glLineWidth(3);
	glPointSize(5);
	//adaug background
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-2.5f, 0.5f, -3.0f);
		modelMatrix *= Transform3D::RotateOX(3.1415f / 2);
		modelMatrix *= Transform3D::Scale(1.0f, 1.0f, 1.0f);
		RenderSimpleMesh(meshes["2Dplane"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0, 1, 1));
	

	//adaugam marea
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Scale(4.0f, 1.0f, 1.0f);
		modelMatrix *= Transform3D::Translate(0.0f,-0.6f,-3.0f);
		RenderSimpleMesh(CreateCylinder("cyllinder", 1, 4, glm::vec3(0, 0, 1)).at(2), shaders["ShaderTema2"], modelMatrix, glm::vec3(0, 0, 1));
	

	//calculez pozitia si unghiul avionului
		float OYTargetPosition = -0.0067f * mouseYposition + 2.8991f;
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
	
	
	//calculez numarul cadrului pentru animatia avionului si pentru nori
		planeFrame += planeAnimationSpeed * deltaTimeSeconds;
		if (planeFrame > 20.0f) {
			planeFrame = 1.0f;
		}

		cloudFrame += (30.0f) * deltaTimeSeconds;
		if (cloudFrame > 21.0f) {
			cloudFrame = 1.0f;
		}
	

	//incarc modelul avionului pe ecran
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(OXposition, OYPosition, OZPosition);
		modelMatrix *= Transform3D::RotateOY(3 * 3.1415f / 2);
		modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
		modelMatrix *= Transform3D::RotateOX(verticalRotation);
		RenderSimpleMesh(plane_animation[(int) planeFrame], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));
	
	

	//generez nori
		time_to_generate -= deltaTimeSeconds;
		if (time_to_generate <= 0.0f) {
			time_to_generate = 1.8f;
			cloud_scale[number_of_clouds] = 0.03f * (rand() % 10) + 0.09f;
			OYcloud_locations[number_of_clouds] = 0.2f * (rand() % 10) + 0.55f;
			cloudSpeed[number_of_clouds] = 0.5f * (rand() % 10 + 1);
			OZcloud_locations[number_of_clouds++] = 6.0f;
		}

	

	//generez obstacole
		time_to_generate_obstacle -= deltaTimeSeconds;
		if (time_to_generate_obstacle <= 0.0f) {
			time_to_generate_obstacle = 0.9f;
			OYobstacle_locations[number_of_obstacles] = 0.2f * (rand() % 10) + 0.55f;
			OZobstacle_locations[number_of_obstacles++] = 6.0f;
		}

	

	//elimin norii care au parasit scena
		for (int i = 0; i < number_of_clouds; i++) {
			if (OZcloud_locations[i] < -6.0f) {
				for (int j = i; j < number_of_clouds - 1; j++) {
					OYcloud_locations[j] = OYcloud_locations[j + 1];
					OZcloud_locations[j] = OZcloud_locations[j + 1];
					cloud_scale[j] = cloud_scale[j + 1];
					cloudSpeed[j] = cloudSpeed[j + 1];
				}
				i--;
				number_of_clouds--;
			}
		}
	

	//elimin obstacole care au parasit scena
		for (int i = 0; i < number_of_obstacles; i++) {
			if (OZobstacle_locations[i] < -6.0f) {
				for (int j = i; j < number_of_obstacles - 1; j++) {
					OYobstacle_locations[j] = OYobstacle_locations[j + 1];
					OZobstacle_locations[j] = OZobstacle_locations[j + 1];
				}
				i--;
				number_of_obstacles--;
			}
		}
	

	//deplasez norii
		for (int i = 0; i < number_of_clouds; i++) {
			OZcloud_locations[i] -= cloudSpeed[i] * deltaTimeSeconds;
		}
	

	//deplasez obstacolele
		for (int i = 0; i < number_of_obstacles; i++) {
			OZobstacle_locations[i] -= obstacle_speed * deltaTimeSeconds;
		}
	

	//incarc norii pe ecran
		glm::vec3 cloud_color = glm::vec3(1, 1, 1);
		for (int i = 0; i < number_of_clouds; i++) {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(OZcloud_locations[i], OYcloud_locations[i], OXposition + 1.0f);
			modelMatrix *= Transform3D::RotateOY(3 * 3.1415f / 2);
			modelMatrix *= Transform3D::Scale(cloud_scale[i], cloud_scale[i], cloud_scale[i]);
			RenderSimpleMesh(cloud_animation[(int)cloudFrame], shaders["ShaderTema2"], modelMatrix, cloud_color);
		}
	

	//incarc obstacolele pe ecran
		glm::vec3 obstacle_color = glm::vec3(1, 0, 1);
		for (int i = 0; i < number_of_obstacles; i++) {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(OZobstacle_locations[i], OYobstacle_locations[i], OXposition + 1.0f);
			modelMatrix *= Transform3D::RotateOY(3 * 3.1415f / 2);
			modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
			RenderSimpleMesh(meshes["obstacle"], shaders["ShaderTema2"], modelMatrix, obstacle_color);
		}
	
}

void Laborator4::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{

}

void Laborator4::OnKeyPress(int key, int mods)
{
	
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{	
	mouseYposition = mouseY;
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator4::OnWindowResize(int width, int height)
{
}
