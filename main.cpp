/*	Daulton Truett
	10/22/2023
	CS-330: Computational Graphics & Visualization
	Southern New Hampshire University
*/

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "Headers/shader.h"

// GLM: headers
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "Headers/stb_image.h"

// Global constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const char* SCREEN_TITLE = "3D Scene: Final - Daulton Truett";

// camera variables
float deltaTime = 0.0f; // Time between current frams and last frame
float lastFrame = 0.0f; // Time of last frame
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;
float lastX = 960;
float lastY = 270;

// controls how fast mouse movements are
float sensitivity = 0.05f;

// texture IDs
unsigned int planeTexture;
unsigned int wallTexture;
unsigned int bookTexture;
unsigned int bookPagesTexture;
unsigned int laptopScreenTexture;
unsigned int screenTexture;
unsigned int mugTexture;
unsigned int speakerTexture;
unsigned int lightPoleTexture;

// camera pos
glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 5.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, -0.3f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// light
glm::vec3 lightPos(-2.8f, 2.11f, -1.7f);

struct GLMesh {
	GLuint vao;
	GLuint vbo;
	GLuint numIndices;
};
// plane mesh
GLMesh planeMesh;

// book mesh
GLMesh bookMesh;

// book pages mesh
GLMesh bookPagesMesh;

// laptop obj
GLMesh laptopMesh;

// Screen mesh
GLMesh screenMesh;

// mug obj
GLMesh mugMesh;

// speaker mesh
GLMesh speakerMesh;

// light obj
GLMesh lightMesh;

// -- Function declarations ---------------------------------------------------------------------------
// - window and input
void resizeWindow(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// - objects
void createPlane(GLMesh& planeMesh, Shader planeShader);
void drawPlane(GLFWwindow* window, Shader planeShader);

void createBook(GLMesh& cubeMesh, Shader bookShader);
void drawBook(GLFWwindow* window, Shader bookShader);

void createBookPages(GLMesh& bookPagesMesh, Shader bookPagesShader);
void drawBookPages(GLFWwindow* window, Shader bookPagesShader);

void createLaptop(GLMesh& laptopMesh, Shader laptopShader);
void drawLaptop(GLFWwindow* window, Shader laptopShader);

void createScreen(GLMesh& screenMesh, Shader screenShader);
void drawScreen(GLFWwindow* window, Shader screenShader);

void createMug(GLMesh& mugMesh, Shader mugShader);
void drawMug(GLFWwindow* window, Shader mugShader);

void createSpeaker(GLMesh& speakerMesh, Shader speakerShader);
void drawSpeaker(GLFWwindow* window, Shader speakerShader);

void createLightSource(GLMesh& lightMesh, Shader lightShader);
void drawLightSource(GLFWwindow* window, Shader lightShader);

void deleteMesh(GLMesh& mesh);

// textures functions
unsigned int loadTexture(char const* path);
unsigned int loadCubeMap(std::vector<std::string> faces);
// ----------------------------------------------------------------------------------------------------

// Main -----------------------------------------------------------------------------------------------
int main() {
	// GLFW: init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif __APPLE__

	// create main window
	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create the GLFW window" << std::endl;
		return false;
	};
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, resizeWindow);

	// obtain mouse pos
	glfwSetCursorPosCallback(window, mouse_callback);

	// listen for mouse scroll input 
	glfwSetScrollCallback(window, scroll_callback);

	// capture mouse 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD: Init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Shader programs
	Shader planeShader("Shaders/vertex_shader.txt", "Shaders/fragment_shader.txt");

	Shader bookShader("Shaders/vertex_shader.txt", "Shaders/fragment_shader.txt");
	Shader bookPagesShader("Shaders/vertex_shader.txt", "Shaders/fragment_shader.txt");

	Shader laptopShader("Shaders/vertex_shader.txt", "Shaders/fragment_shader.txt");
	Shader screenShader("Shaders/vertex_shader.txt", "Shaders/fragment_shader.txt");

	Shader mugShader("Shaders/vertex_shader.txt", "Shaders/fragment_shader.txt");

	Shader speakerShader("Shaders/vertex_shader.txt", "Shaders/fragment_shader.txt");

	Shader lightShader("Shaders/lightSource_vertex_shader.txt", "Shaders/lightSource_fragment_shader.txt");

	// create plane
	createPlane(planeMesh, planeShader);

	// create book
	createBook(bookMesh, bookShader);

	// create book pages
	createBookPages(bookPagesMesh, bookPagesShader);

	// create laptop
	createLaptop(laptopMesh, laptopShader);

	// create screen
	createScreen(screenMesh, screenShader);

	// create mug
	createMug(mugMesh, mugShader);

	// create speaker
	createSpeaker(speakerMesh, speakerShader);

	// create light source
	createLightSource(lightMesh, lightShader);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		// obtain and track time for mouse input
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// process keyboard input from user
		processInput(window);

		// enable z-depth, set BG color
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set the camera viewpoint and FOV
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(fov), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

		// draw the plane that all object sit on
		planeShader.use();
		planeShader.setMat4("view", view);
		planeShader.setMat4("projection", projection);
		drawPlane(window, planeShader);

		// draw book object on the plane
		bookShader.use();
		bookShader.setMat4("view", view);
		bookShader.setMat4("projection", projection);
		drawBook(window, bookShader);

		// draw book pages onto the book
		bookPagesShader.use();
		bookPagesShader.setMat4("view", view);
		bookPagesShader.setMat4("projection", projection);
		drawBookPages(window, bookPagesShader);

		// draw laptop
		laptopShader.use();
		laptopShader.setMat4("view", view);
		laptopShader.setMat4("projection", projection);
		drawLaptop(window, laptopShader);

		// draw laptop screen
		screenShader.use();
		screenShader.setMat4("view", view);
		screenShader.setMat4("projection", projection);
		drawScreen(window, screenShader);

		// draw mug
		mugShader.use();
		mugShader.setMat4("view", view);
		mugShader.setMat4("projection", projection);
		drawMug(window, mugShader);

		// draw speaker
		speakerShader.use();
		speakerShader.setMat4("view", view);
		speakerShader.setMat4("projection", projection);
		drawSpeaker(window, speakerShader);

		// draw light source
		lightShader.use();
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		drawLightSource(window, lightShader);
		
		// switch font and back buffes
		glfwSwapBuffers(window);
		// capture pending inputs fo the next loop
		glfwPollEvents();
	};

	// free up memory from used objects
	deleteMesh(planeMesh);
	deleteMesh(bookMesh);
	deleteMesh(bookPagesMesh);
	deleteMesh(laptopMesh);
	deleteMesh(screenMesh);
	deleteMesh(mugMesh);
	deleteMesh(speakerMesh);
	deleteMesh(lightMesh);
	exit(EXIT_SUCCESS);
	return 0;
};
//------------------------------------------------------------------------------------------------------

// -- function definitions -----------------------------------------------------------------------------

// resize window on user input
void resizeWindow(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
};

// process keyboad input from user
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	};
	const float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	};
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	};
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	};
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	};
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraUp;
	};
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraUp;
	};
};

// process mouse movement from user
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	};
	
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	};
	if (pitch < -89.0f) {
		pitch = -89.0f;
	};
	// mouse directions using euler angles
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
};

// process mouse scroll wheel input
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= (float)yoffset;
	if (fov < 1.0f) {
		fov = 1.0f;
	};
	if (fov > 45.0f) {
		fov = 45.0f;
	};
};

//  -- Create & draw objects ---------------------------------------------------------------------------

// create plane
void createPlane(GLMesh& planeMesh, Shader planeShader) {
	GLfloat plane[]{
		-1.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f, // top left
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // bottom left
		 1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f, // bottom right
		 1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f, // bottom right
		 1.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // top right
		-1.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f, // top left
	};

	// plane setup
	glGenVertexArrays(1, &planeMesh.vao);
	glGenBuffers(1, &planeMesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, planeMesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeMesh.ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);

	glBindVertexArray(planeMesh.vao);

	// position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load texture
	planeTexture = loadTexture("textures/floor.jpg");
	planeShader.use();
	planeShader.setInt("material.diffuse", 0);

	// load texture
	wallTexture = loadTexture("textures/interior_wall.jpg");
	planeShader.use();
	planeShader.setInt("material.diffuse", 0);
};

// draw plane
void drawPlane(GLFWwindow* window, Shader planeShader) {
	// Activate VBOs in the mesh VAO
	glBindVertexArray(planeMesh.vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, planeTexture);

	planeShader.use();
	planeShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	planeShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	planeShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	planeShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	planeShader.setVec3("pointLight.position", cameraPos);
	planeShader.setVec3("pointLight.ambient", 0.1f, 0.1f, 0.1f);
	planeShader.setVec3("pointLight.diffuse", 1.0f, 1.0f, 1.0f); // set to the light source color
	planeShader.setVec3("pointLight.specular", 0.5f, 0.5f, 0.5f); // intensity @ 10%
	planeShader.setFloat("pointLight.constant", 1.0f);
	planeShader.setFloat("pointLight.linear", 0.09f);
	planeShader.setFloat("pointLight.quadratic", 0.032f);

	// material
	planeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	planeShader.setFloat("material.shininess", 32.0f);

	// scale obj
	glm::mat4 scale = glm::scale(glm::vec3(4.0f, 4.0f, 4.0f));
	// Rotates object 
	glm::mat4 rotation = glm::rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// Place obj  
	glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, -1.0f));

	// Model matrix
	// Transformation applied right-to-left
	glm::mat4 model = translation * rotation * scale;
	// Send transform info to the Vertex shader
	planeShader.setMat4("model", model);

	// Draw objects
	glDrawArrays(GL_TRIANGLES, 0, 18);

	// Deactivate the VAO
	glBindVertexArray(0);

	// ----------- walls ---------------------------------------------

	// -- back wall

	// Activate VBOs in the mesh VAO
	glBindVertexArray(planeMesh.vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wallTexture);

	planeShader.use();
	planeShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	planeShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	planeShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	planeShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	planeShader.setVec3("pointLight.position", cameraPos);
	planeShader.setVec3("pointLight.ambient", 0.1f, 0.1f, 0.1f);
	planeShader.setVec3("pointLight.diffuse", 1.0f, 1.0f, 1.0f); // set to the light source color
	planeShader.setVec3("pointLight.specular", 0.5f, 0.5f, 0.5f); // intensity @ 10%
	planeShader.setFloat("pointLight.constant", 1.0f);
	planeShader.setFloat("pointLight.linear", 0.09f);
	planeShader.setFloat("pointLight.quadratic", 0.032f);

	// material
	planeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	planeShader.setFloat("material.shininess", 32.0f);

	// scale obj
	scale = glm::scale(glm::vec3(4.0f, 4.0f, 4.0f));
	// Rotates object 
	rotation = glm::rotate(glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	// Place obj  
	translation = glm::translate(glm::vec3(0.0f, 3.0f, -3.0f));

	// Model matrix
	// Transformation applied right-to-left
	model = translation * rotation * scale;
	// Send transform info to the Vertex shader
	planeShader.setMat4("model", model);

	// Draw objects
	glDrawArrays(GL_TRIANGLES, 0, 18);

	// Deactivate the VAO
	glBindVertexArray(0);

	// -- left wall 

	// Activate VBOs in the mesh VAO
	glBindVertexArray(planeMesh.vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wallTexture);

	// scale obj
	scale = glm::scale(glm::vec3(4.0f, 4.0f, 4.0f));
	// Rotates object 
	rotation = glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// Place obj  
	translation = glm::translate(glm::vec3(-4.0f, 3.0f, -1.0f));

	// Model matrix
	// Transformation applied right-to-left
	model = translation * rotation * scale;
	// Send transform info to the Vertex shader
	planeShader.setMat4("model", model);

	// Draw objects
	glDrawArrays(GL_TRIANGLES, 0, 18);

	// Deactivate the VAO
	glBindVertexArray(0);

	// -- Right wall

	// Activate VBOs in the mesh VAO
	glBindVertexArray(planeMesh.vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wallTexture);

	// scale obj
	scale = glm::scale(glm::vec3(4.0f, 4.0f, 4.0f));
	// Rotates object 
	rotation = glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// Place obj  
	translation = glm::translate(glm::vec3(4.0f, 3.0f, -1.0f));

	// Model matrix
	// Transformation applied right-to-left
	model = translation * rotation * scale;
	// Send transform info to the Vertex shader
	planeShader.setMat4("model", model);

	// Draw objects
	glDrawArrays(GL_TRIANGLES, 0, 18);

	// Deactivate the VAO
	glBindVertexArray(0);
};

// create book
void createBook(GLMesh& cubeMesh, Shader bookShader) {

	GLfloat cube[] = {
		// positions			// normals				// texture coords
		-0.5f, -0.2f, -0.3f,	0.0f,  0.0f, -1.0f,		0.0f,  0.0f,
		 0.5f, -0.2f, -0.3f,	0.0f,  0.0f, -1.0f,		1.0f,  0.0f,
		 0.5f,  0.2f, -0.3f,	0.0f,  0.0f, -1.0f,		1.0f,  1.0f,
		 0.5f,  0.2f, -0.3f,	0.0f,  0.0f, -1.0f,		1.0f,  1.0f,
		-0.5f,  0.2f, -0.3f,	0.0f,  0.0f, -1.0f,		0.0f,  1.0f,
		-0.5f, -0.2f, -0.3f,	0.0f,  0.0f, -1.0f,		0.0f,  0.0f,

		-0.5f, -0.2f,  0.3f,	0.0f,  0.0f,  1.0f,		0.0f,  0.0f,
		 0.5f, -0.2f,  0.3f,	0.0f,  0.0f,  1.0f,		1.0f,  0.0f,
		 0.5f,  0.2f,  0.3f,	0.0f,  0.0f,  1.0f,		1.0f,  1.0f,
		 0.5f,  0.2f,  0.3f,	0.0f,  0.0f,  1.0f,		1.0f,  1.0f,
		-0.5f,  0.2f,  0.3f,	0.0f,  0.0f,  1.0f,		0.0f,  1.0f,
		-0.5f, -0.2f,  0.3f,	0.0f,  0.0f,  1.0f,		0.0f,  0.0f,

		-0.5f,  0.2f,  0.3f,	-1.0f,  0.0f,  0.0f,	1.0f,  0.0f,
		-0.5f,  0.2f, -0.3f,	-1.0f,  0.0f,  0.0f,	1.0f,  1.0f,
		-0.5f, -0.2f, -0.3f,	-1.0f,  0.0f,  0.0f,	0.0f,  1.0f,
		-0.5f, -0.2f, -0.3f,	-1.0f,  0.0f,  0.0f,	0.0f,  1.0f,
		-0.5f, -0.2f,  0.3f,	-1.0f,  0.0f,  0.0f,	0.0f,  0.0f,
		-0.5f,  0.2f,  0.3f,	-1.0f,  0.0f,  0.0f,	1.0f,  0.0f,

		 0.5f,  0.2f,  0.3f,	1.0f,  0.0f,  0.0f,		1.0f,  0.0f,
		 0.5f,  0.2f, -0.3f,	1.0f,  0.0f,  0.0f,		1.0f,  1.0f,
		 0.5f, -0.2f, -0.3f,	1.0f,  0.0f,  0.0f,		0.0f,  1.0f,
		 0.5f, -0.2f, -0.3f,	1.0f,  0.0f,  0.0f,		0.0f,  1.0f,
		 0.5f, -0.2f,  0.3f,	1.0f,  0.0f,  0.0f,		0.0f,  0.0f,
		 0.5f,  0.2f,  0.3f,	1.0f,  0.0f,  0.0f,		1.0f,  0.0f,

		-0.5f, -0.2f, -0.3f,	0.0f, -1.0f,  0.0f,		0.0f,  1.0f,
		 0.5f, -0.2f, -0.3f,	0.0f, -1.0f,  0.0f,		1.0f,  1.0f,
		 0.5f, -0.2f,  0.3f,	0.0f, -1.0f,  0.0f,		1.0f,  0.0f,
		 0.5f, -0.2f,  0.3f,	0.0f, -1.0f,  0.0f,		1.0f,  0.0f,
		-0.5f, -0.2f,  0.3f,	0.0f, -1.0f,  0.0f,		0.0f,  0.0f,
		-0.5f, -0.2f, -0.3f,	0.0f, -1.0f,  0.0f,		0.0f,  1.0f,

		-0.5f,  0.2f, -0.3f,	0.0f,  1.0f,  0.0f,		0.0f,  1.0f,
		 0.5f,  0.2f, -0.3f,	0.0f,  1.0f,  0.0f,		1.0f,  1.0f,
		 0.5f,  0.2f,  0.3f,	0.0f,  1.0f,  0.0f,		1.0f,  0.0f,
		 0.5f,  0.2f,  0.3f,	0.0f,  1.0f,  0.0f,		1.0f,  0.0f,
		-0.5f,  0.2f,  0.3f,	0.0f,  1.0f,  0.0f,		0.0f,  0.0f,
		-0.5f,  0.2f, -0.3f,	0.0f,  1.0f,  0.0f,		0.0f,  1.0f
	};

	glGenVertexArrays(1, &bookMesh.vao);
	glGenBuffers(1, &bookMesh.vbo);

	glBindBuffer(GL_ARRAY_BUFFER, bookMesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glBindVertexArray(bookMesh.vao);

	// position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load texture
	bookTexture = loadTexture("textures/red_book_cover.jpg");
	bookShader.use();
	bookShader.setInt("material.diffuse", 0);
};

// draw book
void drawBook(GLFWwindow* window, Shader bookShader) {

	glBindVertexArray(bookMesh.vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bookTexture);


	bookShader.use();
	bookShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	bookShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	bookShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	bookShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	bookShader.setVec3("pointLight.position", cameraPos);
	bookShader.setVec3("pointLight.ambient", 0.5f, 0.5f, 0.5f);
	bookShader.setVec3("pointLight.diffuse", 1.0f, 1.0f, 1.0f); // set to the light source color
	bookShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f); // intensity @ 10%
	bookShader.setFloat("pointLight.constant", 1.0f);
	bookShader.setFloat("pointLight.linear", 0.09f);
	bookShader.setFloat("pointLight.quadratic", 0.032f);

	// material
	bookShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	bookShader.setFloat("material.shininess", 32.0f);


	// Scales shape, makes object smaller or larger
	glm::mat4 scale = glm::scale(glm::vec3(1.0f, 0.5f, 1.0f));
	// Rotates object 
	glm::mat4 rotation = glm::rotate(glm::radians(75.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// Place obj in center/origin of screen 
	glm::mat4 translation = glm::translate(glm::vec3(-2.0f, 0.05f, -1.5f));

	// Model matrix
	// Transformation applied right-to-left
	glm::mat4 model = translation * rotation * scale;
	bookShader.setMat4("model", model);

	// Draw objects
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// Deactivate the VAO
	glBindVertexArray(0);
};

// create book pages
void createBookPages(GLMesh& bookPagesMesh, Shader bookPagesShader) {
	GLfloat plane[]{
-0.3f,  0.2f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f, // top left
-0.3f, -0.2f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // bottom left
 0.3f, -0.2f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f, // bottom right
 0.3f, -0.2f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f, // bottom right
 0.3f,  0.2f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // top right
-0.3f,  0.2f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f, // top left
	};

	// plane setup
	glGenVertexArrays(1, &bookPagesMesh.vao);
	glGenBuffers(1, &bookPagesMesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, bookPagesMesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeMesh.ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);

	glBindVertexArray(bookPagesMesh.vao);

	// position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load texture
	bookPagesTexture = loadTexture("textures/book_pages_texture.png");
	bookPagesShader.use();
	bookPagesShader.setInt("material.diffuse", 0);
};

void drawBookPages(GLFWwindow *window, Shader bookPagesShader) {
	// Activate VBOs in the mesh VAO
	glBindVertexArray(bookPagesMesh.vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bookPagesTexture);

	bookPagesShader.use();
	bookPagesShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	bookPagesShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	bookPagesShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	bookPagesShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	bookPagesShader.setVec3("pointLight.position", cameraPos);
	bookPagesShader.setVec3("pointLight.ambient", 0.5f, 0.5f, 0.5f);
	bookPagesShader.setVec3("pointLight.diffuse", 1.0f, 1.0f, 1.0f); // set to the light source color
	bookPagesShader.setVec3("pointLight.specular", 0.5f, 0.5f, 0.5f); // intensity @ 10%
	bookPagesShader.setFloat("pointLight.constant", 1.0f);
	bookPagesShader.setFloat("pointLight.linear", 0.09f);
	bookPagesShader.setFloat("pointLight.quadratic", 0.032f);

	// material
	bookPagesShader.setVec3("material.specular", 0.5f, 0.4f, 0.5f);
	bookPagesShader.setFloat("material.shininess", 32.0f);

	// scale obj
	glm::mat4 scale = glm::scale(glm::vec3(1.67f, 0.5f, 1.0f));
	// Rotates object 
	glm::mat4 rotation = glm::rotate(glm::radians(75.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// Place obj  
	glm::mat4 translation = glm::translate(glm::vec3(-1.7f, 0.05f, -1.415f));

	// Model matrix
	// Transformation applied right-to-left
	glm::mat4 model = translation * rotation * scale;
	// Send transform info to the Vertex shader
	bookPagesShader.setMat4("model", model);

	// Draw objects
	glDrawArrays(GL_TRIANGLES, 0, 18);

	
	scale = glm::scale(glm::vec3(1.0f, 0.5f, 1.0f));
	rotation = glm::rotate(glm::radians(-15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	translation = glm::translate(glm::vec3(-2.12f, 0.05f, -1.01f));
	model = translation * rotation * scale;

	bookPagesShader.setMat4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, 18);


	// Deactivate the VAO
	glBindVertexArray(0);
}

// create laptop
void createLaptop(GLMesh& laptopMesh, Shader laptopShader) {
	GLfloat cube[] = {
		// positions			// normals				// texture coords
		-0.5f, -0.05f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f,  0.0f,
		 0.5f, -0.05f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f,  0.0f,
		 0.5f,  0.05f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f,  1.0f,
		 0.5f,  0.05f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f,  1.0f,
		-0.5f,  0.05f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f,  1.0f,
		-0.5f, -0.05f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f,  0.0f,

		-0.5f, -0.05f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f,  0.0f,
		 0.5f, -0.05f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f,  0.0f,
		 0.5f,  0.05f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f,  1.0f,
		 0.5f,  0.05f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f,  1.0f,
		-0.5f,  0.05f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f,  1.0f,
		-0.5f, -0.05f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f,  0.0f,

		-0.5f,  0.05f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f,  0.0f,
		-0.5f,  0.05f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f,  1.0f,
		-0.5f, -0.05f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f,  1.0f,
		-0.5f, -0.05f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f,  1.0f,
		-0.5f, -0.05f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f,  0.0f,
		-0.5f,  0.05f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f,  0.0f,

		 0.5f,  0.05f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f,  0.0f,
		 0.5f,  0.05f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f,  1.0f,
		 0.5f, -0.05f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f,  1.0f,
		 0.5f, -0.05f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f,  1.0f,
		 0.5f, -0.05f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f,  0.0f,
		 0.5f,  0.05f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f,  0.0f,

		-0.5f, -0.05f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f,  1.0f,
		 0.5f, -0.05f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f,  1.0f,
		 0.5f, -0.05f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f,  0.0f,
		 0.5f, -0.05f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f,  0.0f,
		-0.5f, -0.05f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0f,  0.0f,
		-0.5f, -0.05f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f,  1.0f,

		-0.5f,  0.05f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f,  1.0f,
		 0.5f,  0.05f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f,  1.0f,
		 0.5f,  0.05f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f,  0.0f,
		 0.5f,  0.05f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f,  0.0f,
		-0.5f,  0.05f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f,  0.0f,
		-0.5f,  0.05f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f,  1.0f
	};



	glGenVertexArrays(1, &laptopMesh.vao);
	glGenBuffers(1, &laptopMesh.vbo);

	glBindBuffer(GL_ARRAY_BUFFER, laptopMesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glBindVertexArray(laptopMesh.vao);

	// position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load texture
	laptopScreenTexture = loadTexture("textures/laptop_base.jpg");
	laptopShader.use();
	laptopShader.setInt("material.diffuse", 0);

};

// draw laptop
void drawLaptop(GLFWwindow* window, Shader laptopShader) {
	glBindVertexArray(laptopMesh.vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, laptopScreenTexture);


	laptopShader.use();
	laptopShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	laptopShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	laptopShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	laptopShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	laptopShader.setVec3("pointLight.position", cameraPos);
	laptopShader.setVec3("pointLight.ambient", 0.5f, 0.5f, 0.5f);
	laptopShader.setVec3("pointLight.diffuse", 1.0f, 1.0f, 1.0f); // set to the light source color
	laptopShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f); // intensity @ 10%
	laptopShader.setFloat("pointLight.constant", 1.0f);
	laptopShader.setFloat("pointLight.linear", 0.09f);
	laptopShader.setFloat("pointLight.quadratic", 0.032f);

	// material
	laptopShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	laptopShader.setFloat("material.shininess", 32.0f);


	// Scales shape, makes object smaller or larger
	glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	// Rotates object 
	glm::mat4 rotation = glm::mat4(1.0f);
	// Place obj in center/origin of screen 
	glm::mat4 translation = glm::mat4(1.0f);
	// Model matrix
	// Transformation applied right-to-left
	glm::mat4 model = glm::mat4(1.0f);

	rotation = rotate(glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	translation = glm::translate(glm::vec3(0.0f, 0.0f, -1.5f));
	model = translation * rotation * scale;
	laptopShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	rotation = rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	translation = glm::translate(glm::vec3(0.0f, 0.27f, -2.0f));
	model = translation * rotation * scale;
	laptopShader.setMat4("model", model);
	// Draw objects
	glDrawArrays(GL_TRIANGLES, 0, 36);


	// Deactivate the VAO
	glBindVertexArray(0);
};

// create laptop screen
void createScreen(GLMesh& screenMesh, Shader screenShader) {
	GLfloat plane[]{
	-1.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f, // top left
	-1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // bottom left
	 1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f, // bottom right
	 1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f, // bottom right
	 1.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // top right
	-1.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f, // top left
	};

	// plane setup
	glGenVertexArrays(1, &screenMesh.vao);
	glGenBuffers(1, &screenMesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, screenMesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeMesh.ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);

	glBindVertexArray(screenMesh.vao);

	// position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load texture
	screenTexture = loadTexture("textures/screen.jpg");
	screenShader.use();
	screenShader.setInt("material.diffuse", 0);
};

// draw laptop screen
void drawScreen(GLFWwindow* window, Shader screenShader) {
	// Activate VBOs in the mesh VAO
	glBindVertexArray(screenMesh.vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTexture);

	screenShader.use();
	screenShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	screenShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	screenShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	screenShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	screenShader.setVec3("pointLight.position", cameraPos);
	screenShader.setVec3("pointLight.ambient", 0.5f, 0.5f, 0.5f);
	screenShader.setVec3("pointLight.diffuse", 1.0f, 1.0f, 1.0f); // set to the light source color
	screenShader.setVec3("pointLight.specular", 0.5f, 0.5f, 0.5f); // intensity @ 10%
	screenShader.setFloat("pointLight.constant", 1.0f);
	screenShader.setFloat("pointLight.linear", 0.09f);
	screenShader.setFloat("pointLight.quadratic", 0.032f);

	// material
	screenShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	screenShader.setFloat("material.shininess", 32.0f);

	// scale obj
	glm::mat4 scale = glm::scale(glm::vec3(0.4f, 0.4f, 0.4f));
	// Rotates object 
	glm::mat4 rotation = glm::rotate(glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	// Place obj  
	glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.5f, -1.94f));

	// Model matrix
	// Transformation applied right-to-left
	glm::mat4 model = translation * rotation * scale;
	// Send transform info to the Vertex shader
	screenShader.setMat4("model", model);

	// Draw objects
	glDrawArrays(GL_TRIANGLES, 0, 18);

	// Deactivate the VAO
	glBindVertexArray(0);
}

// create coffee mug
void createMug(GLMesh& mugMesh, Shader mugShader) {
	GLfloat coneVerts[] = {
			// cone bottom			// normals			// texture coords
			1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f,1.0f,
			.98f, 0.0f, -0.17f,		0.0f, -1.0f, 0.0f,	0.41f, 0.983f,
			.94f, 0.0f, -0.34f,		0.0f, -1.0f, 0.0f,	0.33f, 0.96f,
			.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.92f,
			.77f, 0.0f, -0.64f,		0.0f, -1.0f, 0.0f,	0.17f, 0.87f,
			.64f, 0.0f, -0.77f,		0.0f, -1.0f, 0.0f,	0.13f, 0.83f,
			.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.77f,
			.34f, 0.0f, -0.94f,		0.0f, -1.0f, 0.0f,	0.04f, 0.68f,
			.17f, 0.0f, -0.98f,		0.0f, -1.0f, 0.0f,	0.017f, 0.6f,
			0.0f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,	0.0f,0.5f,
			-.17f, 0.0f, -0.98f,	0.0f, -1.0f, 0.0f,	0.017f, 0.41f,
			-.34f, 0.0f, -0.94f,	0.0f, -1.0f, 0.0f,	0.04f, 0.33f,
			-.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.25f,
			-.64f, 0.0f, -0.77f,	0.0f, -1.0f, 0.0f,	0.13f, 0.17f,
			-.77f, 0.0f, -0.64f,	0.0f, -1.0f, 0.0f,	0.17f, 0.13f,
			-.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.08f,
			-.94f, 0.0f, -0.34f,	0.0f, -1.0f, 0.0f,	0.33f, 0.04f,
			-.98f, 0.0f, -0.17f,	0.0f, -1.0f, 0.0f,	0.41f, 0.017f,
			-1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f, 0.0f,
			-.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.017f,
			-.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.04f,
			-.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.08f,
			-.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.13f,
			-.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.17f,
			-.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.25f,
			-.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.33f,
			-.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.41f,
			0.0f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.5f,
			.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.6f,
			.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.68f,
			.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.77f,
			.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.83f,
			.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.87f,
			.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.92f,
			.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.96f,
			.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.983f,

			// cone sides		// normals									// texture coords
			1.0f, 0.0f, 0.0f,		0.993150651f, 0.0f, 0.116841137f, 		0.0f,0.0f,
			0.0f, 1.0f, 0.0f,		0.993150651f, 0.0f, 0.116841137f, 		0.5f, 1.0f,
			.98f, 0.0f, -0.17f,		0.993150651f, 0.0f, 0.116841137f, 		0.0277,0.0,
			.98f, 0.0f, -0.17f,		0.973417103f, 0.0f, 0.229039446f, 		0.0277,0.0,
			0.0f, 1.0f, 0.0f,		0.973417103f, 0.0f, 0.229039446f, 		0.5f, 1.0f,
			.94f, 0.0f, -0.34f,		0.973417103f, 0.0f, 0.229039446f, 		0.0554,0.0f,
			.94f, 0.0f, -0.34f,		0.916157305f, 0.0f, 0.400818795f, 		0.0554,0.0f,
			0.0f, 1.0f, 0.0f,		0.916157305f, 0.0f, 0.400818795f, 		0.5f, 1.0f,
			.87f, 0.0f, -0.5f,		0.916157305f, 0.0f, 0.400818795f, 		0.0831,0.0f,
			.87f, 0.0f, -0.5f,		0.813733339f, 0.0f, 0.581238329f, 		0.0831,0.0f,
			0.0f, 1.0f, 0.0f,		0.813733339f, 0.0f, 0.581238329f, 		0.5f, 1.0f,
			.77f, 0.0f, -0.64f,		0.813733339f, 0.0f, 0.581238329f, 		0.1108f, 0.0f,
			.77f, 0.0f, -0.64f,		0.707106769f, 0.0f, 0.707106769f, 		0.1108f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.707106769f, 0.0f, 0.707106769f, 		0.5f, 1.0f,
			.64f, 0.0f, -0.77f,		0.707106769f, 0.0f, 0.707106769f, 		0.1385f, 0.0f,
			.64f, 0.0f, -0.77f,		0.581238329f, 0.0f, 0.813733339f, 		0.1385f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.581238329f, 0.0f, 0.813733339f, 		0.5f, 1.0f,
			.5f, 0.0f, -0.87f,		0.581238329f, 0.0f, 0.813733339f, 		0.1662f, 0.0f,
			.5f, 0.0f, -0.87f,		0.400818795f, 0.0f, 0.916157305f, 		0.1662f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.400818795f, 0.0f, 0.916157305f, 		0.5f, 1.0f,
			.34f, 0.0f, -0.94f,		0.400818795f, 0.0f, 0.916157305f, 		0.1939f, 0.0f,
			.34f, 0.0f, -0.94f,		0.229039446f, 0.0f, 0.973417103f, 		0.1939f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.229039446f, 0.0f, 0.973417103f, 		0.5f, 1.0f,
			.17f, 0.0f, -0.98f,		0.229039446f, 0.0f, 0.973417103f, 		0.2216f, 0.0f,
			.17f, 0.0f, -0.98f,		0.116841137f, 0.0f, 0.993150651f, 		0.2216f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.116841137f, 0.0f, 0.993150651f, 		0.5f, 1.0f,
			0.0f, 0.0f, -1.0f,		0.116841137f, 0.0f, 0.993150651f, 		0.2493f, 0.0f,

			0.0f, 0.0f, -1.0f,		-0.116841137f, 0.0f, 0.993150651f, 		0.2493f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.116841137f, 0.0f, 0.993150651f, 		0.5f, 1.0f,
			-.17f, 0.0f, -0.98f,	-0.116841137f, 0.0f, 0.993150651f, 		0.277f, 0.0f,
			-.17f, 0.0f, -0.98f,	-0.229039446f, 0.0f, 0.973417103f, 		0.277f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.229039446f, 0.0f, 0.973417103f, 		0.5f, 1.0f,
			-.34f, 0.0f, -0.94f,	-0.229039446f, 0.0f, 0.973417103f, 		0.3047f, 0.0f,
			-.34f, 0.0f, -0.94f,	-0.400818795f, 0.0f, 0.916157305f, 		0.3047f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.400818795f, 0.0f, 0.916157305f, 		0.5f, 1.0f,
			-.5f, 0.0f, -0.87f,		-0.400818795f, 0.0f, 0.916157305f, 		0.3324f, 0.0f,
			-.5f, 0.0f, -0.87f,		-0.581238329f, 0.0f, 0.813733339f, 		0.3324f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.581238329f, 0.0f, 0.813733339f, 		0.5f, 1.0f,
			-.64f, 0.0f, -0.77f,	-0.581238329f, 0.0f, 0.813733339f, 		0.3601f, 0.0f,
			-.64f, 0.0f, -0.77f,	-0.707106769f, 0.0f, 0.707106769f, 		0.3601f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.707106769f, 0.0f, 0.707106769f, 		0.5f, 1.0f,
			-.77f, 0.0f, -0.64f,	-0.707106769f, 0.0f, 0.707106769f, 		0.3878f, 0.0f,
			-.77f, 0.0f, -0.64f,	-0.813733339f, 0.0f, 0.581238329f, 		0.3878f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.813733339f, 0.0f, 0.581238329f, 		0.5f, 1.0f,
			-.87f, 0.0f, -0.5f,		-0.813733339f, 0.0f, 0.581238329f, 		0.4155f, 0.0f,
			-.87f, 0.0f, -0.5f,		-0.916157305f, 0.0f, 0.400818795f, 		0.4155f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.916157305f, 0.0f, 0.400818795f, 		0.5f, 1.0f,
			-.94f, 0.0f, -0.34f,	-0.916157305f, 0.0f, 0.400818795f, 		0.4432f, 0.0f,
			-.94f, 0.0f, -0.34f,	-0.973417103f, 0.0f, 0.229039446f, 		0.4432f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.973417103f, 0.0f, 0.229039446f, 		0.5f, 1.0f,
			-.98f, 0.0f, -0.17f,	-0.973417103f, 0.0f, 0.229039446f, 		0.4709f, 0.0f,
			-.98f, 0.0f, -0.17f,	-0.993150651f, 0.0f, 0.116841137f, 		0.4709f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.993150651f, 0.0f, 0.116841137f, 		0.5f, 1.0f,
			-1.0f, 0.0f, 0.0f,		-0.993150651f, 0.0f, 0.116841137f, 		0.4986f, 0.0f,
			-1.0f, 0.0f, 0.0f,		-0.993150651f, 0.0f, -0.116841137f, 		0.4986f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.993150651f, 0.0f, -0.116841137f, 		0.5f, 1.0f,
			-.98f, 0.0f, 0.17f,		-0.993150651f, 0.0f, -0.116841137f, 		0.5263f, 0.0f,
			-.98f, 0.0f, 0.17f,		-0.973417103f, 0.0f, -0.229039446f, 		0.5263f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.973417103f, 0.0f, -0.229039446f, 		0.5f, 1.0f,
			-.94f, 0.0f, 0.34f,		-0.973417103f, 0.0f, -0.229039446f, 		0.554f, 0.0f,
			-.94f, 0.0f, 0.34f,		-0.916157305f, 0.0f, -0.400818795f, 		0.554f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.916157305f, 0.0f, -0.400818795f, 		0.5f, 1.0f,
			-.87f, 0.0f, 0.5f,		-0.916157305f, 0.0f, -0.400818795f, 		0.5817f, 0.0f,
			-.87f, 0.0f, 0.5f,		-0.813733339f, 0.0f, -0.581238329f, 		0.5817f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.813733339f, 0.0f, -0.581238329f, 		0.5f, 1.0f,
			-.77f, 0.0f, 0.64f,		-0.813733339f, 0.0f, -0.581238329f, 		0.6094f, 0.0f,
			-.77f, 0.0f, 0.64f,		-0.707106769f, 0.0f, -0.707106769f, 		0.6094f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.707106769f, 0.0f, -0.707106769f, 		0.5f, 1.0f,
			-.64f, 0.0f, 0.77f,		-0.707106769f, 0.0f, -0.707106769f, 		0.6371f, 0.0f,
			-.64f, 0.0f, 0.77f,		-0.581238329f, 0.0f, -0.813733339f, 		0.6371f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.581238329f, 0.0f, -0.813733339f, 		0.5f, 1.0f,
			-.5f, 0.0f, 0.87f,		-0.581238329f, 0.0f, -0.813733339f, 		0.6648f, 0.0f,
			-.5f, 0.0f, 0.87f,		-0.400818795f, 0.0f, -0.916157305f, 		0.6648f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.400818795f, 0.0f, -0.916157305f, 		0.5f, 1.0f,
			-.34f, 0.0f, 0.94f,		-0.400818795f, 0.0f, -0.916157305f, 		0.6925f, 0.0f,
			-.34f, 0.0f, 0.94f,		-0.229039446f, 0.0f, -0.973417103f, 		0.6925f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.229039446f, 0.0f, -0.973417103f, 		0.5f, 1.0f,
			-.17f, 0.0f, 0.98f,		-0.229039446f, 0.0f, -0.973417103f, 		0.7202f, 0.0f,
			-.17f, 0.0f, 0.98f,		-0.116841137f, 0.0f, -0.993150651f, 		0.7202f, 0.0f,
			0.0f, 1.0f, 0.0f,		-0.116841137f, 0.0f, -0.993150651f, 		0.5f, 1.0f,
			0.0f, 0.0f, 1.0f,		-0.116841137f, 0.0f, -0.993150651f, 		0.7479f, 0.0f,

			0.0f, 0.0f, 1.0f,		0.116841137f, 0.0f, -0.993150651f, 	0.7479f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.116841137f, 0.0f, -0.993150651f, 	0.5f, 1.0f,
			.17f, 0.0f, 0.98f,		0.116841137f, 0.0f, -0.993150651f, 	0.7756f, 0.0f,
			.17f, 0.0f, 0.98f,		0.229039446f, 0.0f, -0.973417103f, 	0.7756f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.229039446f, 0.0f, -0.973417103f, 	0.5f, 1.0f,
			.34f, 0.0f, 0.94f,		0.229039446f, 0.0f, -0.973417103f, 	0.8033f, 0.0f,
			.34f, 0.0f, 0.94f,		0.400818795f, 0.0f, -0.916157305f, 	0.8033f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.400818795f, 0.0f, -0.916157305f, 	0.5f, 1.0f,
			.5f, 0.0f, 0.87f,		0.400818795f, 0.0f, -0.916157305f, 	0.831f, 0.0f,
			.5f, 0.0f, 0.87f,		0.581238329f, 0.0f, -0.813733339f, 	0.831f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.581238329f, 0.0f, -0.813733339f, 	0.5f, 1.0f,
			.64f, 0.0f, 0.77f,		0.581238329f, 0.0f, -0.813733339f, 	0.8587f, 0.0f,
			.64f, 0.0f, 0.77f,		0.707106769f, 0.0f, -0.707106769f, 	0.8587f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.707106769f, 0.0f, -0.707106769f, 	0.5f, 1.0f,
			.77f, 0.0f, 0.64f,		0.707106769f, 0.0f, -0.707106769f, 	0.8864f, 0.0f,
			.77f, 0.0f, 0.64f,		0.813733339f, 0.0f, -0.581238329f, 	0.8864f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.813733339f, 0.0f, -0.581238329f, 	0.5f, 1.0f,
			.87f, 0.0f, 0.5f,		0.813733339f, 0.0f, -0.581238329f, 	0.9141f, 0.0f,
			.87f, 0.0f, 0.5f,		0.916157305f, 0.0f, -0.400818795f, 	0.9141f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.916157305f, 0.0f, -0.400818795f, 	0.5f, 1.0f,
			.94f, 0.0f, 0.34f,		0.916157305f, 0.0f, -0.400818795f, 	0.9418f, 0.0f,
			.94f, 0.0f, 0.34f,		0.973417103f, 0.0f, -0.229039446f, 	0.9418f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.973417103f, 0.0f, -0.229039446f, 	0.5f, 1.0f,
			.98f, 0.0f, 0.17f,		0.973417103f, 0.0f, -0.229039446f, 	0.9695f, 0.0f,
			.98f, 0.0f, 0.17f,		0.993150651f, 0.0f, -0.116841137f, 	0.9695f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.993150651f, 0.0f, -0.116841137f, 	0.5f, 1.0f,
			1.0f, 0.0f, 0.0f,		0.993150651f, 0.0f, -0.116841137f, 	0.0f, 0.0f
		};

	glGenVertexArrays(1, &mugMesh.vao);
	glGenBuffers(1, &mugMesh.vbo);

	// vertex data
	glBindBuffer(GL_ARRAY_BUFFER, mugMesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coneVerts), coneVerts, GL_STATIC_DRAW);

	glBindVertexArray(mugMesh.vao);

	// position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	mugTexture = loadTexture("textures/blue_mug.jpg");
	mugShader.use();
	mugShader.setInt("material.diffuse", 0);
};

// draw coffee mug
void drawMug(GLFWwindow* window, Shader mugShader) {
	glBindVertexArray(mugMesh.vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mugTexture);


	mugShader.use();
	mugShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	mugShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	mugShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	mugShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	mugShader.setVec3("pointLight.position", cameraPos);
	mugShader.setVec3("pointLight.ambient", 0.5f, 0.5f, 0.5f);
	mugShader.setVec3("pointLight.diffuse", 1.0f, 1.0f, 1.0f); // set to the light source color
	mugShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f); 
	mugShader.setFloat("pointLight.constant", 1.0f);
	mugShader.setFloat("pointLight.linear", 0.09f);
	mugShader.setFloat("pointLight.quadratic", 0.032f);

	// material
	mugShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	mugShader.setFloat("material.shininess", 32.0f);


	// Scales shape, makes object smaller or larger
	glm::mat4 scale = glm::scale(glm::vec3(0.15f, 0.8f, 0.15f));
	// Rotates object 
	glm::mat4 rotation = glm::mat4(1.0f);
	// Place obj in center/origin of screen 
	glm::mat4 translation = glm::mat4(1.0f);
	// Model matrix
	// Transformation applied right-to-left
	glm::mat4 model = glm::mat4(1.0f);

	rotation = rotate(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	translation = glm::translate(glm::vec3(1.1f, 0.4f, -1.5f));
	model = translation * rotation * scale;
	mugShader.setMat4("model", model);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
	glDrawArrays(GL_TRIANGLE_FAN, 36, 36);		//top
	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides
	// Deactivate the VAO
	glBindVertexArray(0);
}

// create speaker
void createSpeaker(GLMesh &speakerMesh, Shader speakerShader){
	GLfloat verts[] = {
		// cylinder bottom		// normals			// texture coords
		1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f,1.0f,
		.98f, 0.0f, -0.17f,		0.0f, -1.0f, 0.0f,	0.41f, 0.983f,
		.94f, 0.0f, -0.34f,		0.0f, -1.0f, 0.0f,	0.33f, 0.96f,
		.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.92f,
		.77f, 0.0f, -0.64f,		0.0f, -1.0f, 0.0f,	0.17f, 0.87f,
		.64f, 0.0f, -0.77f,		0.0f, -1.0f, 0.0f,	0.13f, 0.83f,
		.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.77f,
		.34f, 0.0f, -0.94f,		0.0f, -1.0f, 0.0f,	0.04f, 0.68f,
		.17f, 0.0f, -0.98f,		0.0f, -1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,	0.0f,0.5f,
		-.17f, 0.0f, -0.98f,	0.0f, -1.0f, 0.0f,	0.017f, 0.41f,
		-.34f, 0.0f, -0.94f,	0.0f, -1.0f, 0.0f,	0.04f, 0.33f,
		-.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.25f,
		-.64f, 0.0f, -0.77f,	0.0f, -1.0f, 0.0f,	0.13f, 0.17f,
		-.77f, 0.0f, -0.64f,	0.0f, -1.0f, 0.0f,	0.17f, 0.13f,
		-.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.08f,
		-.94f, 0.0f, -0.34f,	0.0f, -1.0f, 0.0f,	0.33f, 0.04f,
		-.98f, 0.0f, -0.17f,	0.0f, -1.0f, 0.0f,	0.41f, 0.017f,
		-1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f, 0.0f,
		-.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.017f,
		-.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.04f,
		-.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.08f,
		-.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.13f,
		-.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.17f,
		-.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.25f,
		-.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.33f,
		-.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.5f,
		.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.6f,
		.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.68f,
		.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.77f,
		.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.83f,
		.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.87f,
		.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.92f,
		.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.96f,
		.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.983f,

		// cylinder top			// normals			// texture coords
		1.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.5f,1.0f,
		.98f, 1.0f, -0.17f,		0.0f, 1.0f, 0.0f,	0.41f, 0.983f,
		.94f, 1.0f, -0.34f,		0.0f, 1.0f, 0.0f,	0.33f, 0.96f,
		.87f, 1.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.25f, 0.92f,
		.77f, 1.0f, -0.64f,		0.0f, 1.0f, 0.0f,	0.17f, 0.87f,
		.64f, 1.0f, -0.77f,		0.0f, 1.0f, 0.0f,	0.13f, 0.83f,
		.5f, 1.0f, -0.87f,		0.0f, 1.0f, 0.0f,	0.08f, 0.77f,
		.34f, 1.0f, -0.94f,		0.0f, 1.0f, 0.0f,	0.04f, 0.68f,
		.17f, 1.0f, -0.98f,		0.0f, 1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f,	0.0f,0.5f,
		-.17f, 1.0f, -0.98f,	0.0f, 1.0f, 0.0f,	0.017f, 0.41f,
		-.34f, 1.0f, -0.94f,	0.0f, 1.0f, 0.0f,	0.04f, 0.33f,
		-.5f, 1.0f, -0.87f,		0.0f, 1.0f, 0.0f,	0.08f, 0.25f,
		-.64f, 1.0f, -0.77f,	0.0f, 1.0f, 0.0f,	0.13f, 0.17f,
		-.77f, 1.0f, -0.64f,	0.0f, 1.0f, 0.0f,	0.17f, 0.13f,
		-.87f, 1.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.25f, 0.08f,
		-.94f, 1.0f, -0.34f,	0.0f, 1.0f, 0.0f,	0.33f, 0.04f,
		-.98f, 1.0f, -0.17f,	0.0f, 1.0f, 0.0f,	0.41f, 0.017f,
		-1.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.5f, 0.0f,
		-.98f, 1.0f, 0.17f,		0.0f, 1.0f, 0.0f,	0.6f, 0.017f,
		-.94f, 1.0f, 0.34f,		0.0f, 1.0f, 0.0f,	0.68f, 0.04f,
		-.87f, 1.0f, 0.5f,		0.0f, 1.0f, 0.0f,	0.77f, 0.08f,
		-.77f, 1.0f, 0.64f,		0.0f, 1.0f, 0.0f,	0.83f, 0.13f,
		-.64f, 1.0f, 0.77f,		0.0f, 1.0f, 0.0f,	0.87f, 0.17f,
		-.5f, 1.0f, 0.87f,		0.0f, 1.0f, 0.0f,	0.92f, 0.25f,
		-.34f, 1.0f, 0.94f,		0.0f, 1.0f, 0.0f,	0.96f, 0.33f,
		-.17f, 1.0f, 0.98f,		0.0f, 1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.5f,
		.17f, 1.0f, 0.98f,		0.0f, 1.0f, 0.0f,	0.983f, 0.6f,
		.34f, 1.0f, 0.94f,		0.0f, 1.0f, 0.0f,	0.96f, 0.68f,
		.5f, 1.0f, 0.87f,		0.0f, 1.0f, 0.0f,	0.92f, 0.77f,
		.64f, 1.0f, 0.77f,		0.0f, 1.0f, 0.0f,	0.87f, 0.83f,
		.77f, 1.0f, 0.64f,		0.0f, 1.0f, 0.0f,	0.83f, 0.87f,
		.87f, 1.0f, 0.5f,		0.0f, 1.0f, 0.0f,	0.77f, 0.92f,
		.94f, 1.0f, 0.34f,		0.0f, 1.0f, 0.0f,	0.68f, 0.96f,
		.98f, 1.0f, 0.17f,		0.0f, 1.0f, 0.0f,	0.6f, 0.983f,

		// cylinder body		// normals				// texture coords
		1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0,1.0,
		1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0,0.0,
		.98f, 0.0f, -0.17f,		1.0f, 0.0f, 0.0f,		0.0277,0.0,
		1.0f, 1.0f, 0.0f,		0.92f, 0.0f, -0.08f,	0.0,1.0,
		.98f, 1.0f, -0.17f,		0.92f, 0.0f, -0.08f,	0.0277,1.0,
		.98f, 0.0f, -0.17f,		0.92f, 0.0f, -0.08f,	0.0277,0.0,
		.94f, 0.0f, -0.34f,		0.83f, 0.0f, -0.17f,	0.0554,0.0,
		.98f, 1.0f, -0.17f,		0.83f, 0.0f, -0.17f,	0.0277,1.0,
		.94f, 1.0f, -0.34f,		0.83f, 0.0f, -0.17f,	0.0554,1.0,
		.94f, 0.0f, -0.34f,		0.75f, 0.0f, -0.25f,	0.0554,0.0,
		.87f, 0.0f, -0.5f,		0.75f, 0.0f, -0.25f,	0.0831,0.0,
		.94f, 1.0f, -0.34f,		0.75f, 0.0f, -0.25f,	0.0554,1.0,
		.87f, 1.0f, -0.5f,		0.67f, 0.0f, -0.33f,	0.0831,1.0,
		.87f, 0.0f, -0.5f,		0.67f, 0.0f, -0.33f,	0.0831,0.0,
		.77f, 0.0f, -0.64f,		0.67f, 0.0f, -0.33f,	0.1108,0.0,
		.87f, 1.0f, -0.5f,		0.58f, 0.0f, -0.42f,	0.0831,1.0,
		.77f, 1.0f, -0.64f,		0.58f, 0.0f, -0.42f,	0.1108,1.0,
		.77f, 0.0f, -0.64f,		0.58f, 0.0f, -0.42f,	0.1108,0.0,
		.64f, 0.0f, -0.77f,		0.5f, 0.0f, -0.5f,		0.1385,0.0,
		.77f, 1.0f, -0.64f,		0.5f, 0.0f, -0.5f,		0.1108,1.0,
		.64f, 1.0f, -0.77f,		0.5f, 0.0f, -0.5f,		0.1385,1.0,
		.64f, 0.0f, -0.77f,		0.42f, 0.0f, -0.58f,	0.1385,0.0,
		.5f, 0.0f, -0.87f,		0.42f, 0.0f, -0.58f,	0.1662,0.0,
		.64f, 1.0f, -0.77f,		0.42f, 0.0f, -0.58f,	0.1385, 1.0,
		.5f, 1.0f, -0.87f,		0.33f, 0.0f, -0.67f,	0.1662, 1.0,
		.5f, 0.0f, -0.87f,		0.33f, 0.0f, -0.67f,	0.1662, 0.0,
		.34f, 0.0f, -0.94f,		0.33f, 0.0f, -0.67f,	0.1939, 0.0,
		.5f, 1.0f, -0.87f,		0.25f, 0.0f, -0.75f,	0.1662, 1.0,
		.34f, 1.0f, -0.94f,		0.25f, 0.0f, -0.75f,	0.1939, 1.0,
		.34f, 0.0f, -0.94f,		0.25f, 0.0f, -0.75f,	0.1939, 0.0,
		.17f, 0.0f, -0.98f,		0.17f, 0.0f, -0.83f,	0.2216, 0.0,
		.34f, 1.0f, -0.94f,		0.17f, 0.0f, -0.83f,	0.1939, 1.0,
		.17f, 1.0f, -0.98f,		0.17f, 0.0f, -0.83f,	0.2216, 1.0,
		.17f, 0.0f, -0.98f,		0.08f, 0.0f, -0.92f,	0.2216, 0.0,
		0.0f, 0.0f, -1.0f,		0.08f, 0.0f, -0.92f,	0.2493, 0.0,
		.17f, 1.0f, -0.98f,		0.08f, 0.0f, -0.92f,	0.2216, 1.0,
		0.0f, 1.0f, -1.0f,		0.0f, 0.0f, -1.0f,		0.2493, 1.0,
		0.0f, 0.0f, -1.0f,		0.0f, 0.0f, -1.0f,		0.2493, 0.0,
		-.17f, 0.0f, -0.98f,	0.0f, 0.0f, -1.0f,		0.277, 0.0,
		0.0f, 1.0f, -1.0f,		0.08f, 0.0f, -1.08f,	0.2493, 1.0,
		-.17f, 1.0f, -0.98f,	-0.08f, 0.0f, -0.92f,	0.277, 1.0,
		-.17f, 0.0f, -0.98f,	-0.08f, 0.0f, -0.92f,	0.277, 0.0,
		-.34f, 0.0f, -0.94f,	-0.08f, 0.0f, -0.92f,	0.3047, 0.0,
		-.17f, 1.0f, -0.98f,	-0.08f, 0.0f, -0.92f,	0.277, 1.0,
		-.34f, 1.0f, -0.94f,	-0.17f, 0.0f, -0.83f,	0.3047, 1.0,
		-.34f, 0.0f, -0.94f,	-0.17f, 0.0f, -0.83f,	0.3047, 0.0,
		-.5f, 0.0f, -0.87f,		-0.17f, 0.0f, -0.83f,	0.3324, 0.0,
		-.34f, 1.0f, -0.94f,	-0.25f, 0.0f, -0.75f,	0.3047, 1.0,
		-.5f, 1.0f, -0.87f,		-0.25f, 0.0f, -0.75f,	0.3324, 1.0,
		-.5f, 0.0f, -0.87f,		-0.25f, 0.0f, -0.75f,	0.3324, 0.0,
		-.64f, 0.0f, -0.77f,	-0.33f, 0.0f, -0.67f,	0.3601, 0.0,
		-.5f, 1.0f, -0.87f,		-0.33f, 0.0f, -0.67f,	0.3324, 1.0,
		-.64f, 1.0f, -0.77f,	-0.33f, 0.0f, -0.67f,	0.3601, 1.0,
		-.64f, 0.0f, -0.77f,	-0.42f, 0.0f, -0.58f,	0.3601, 0.0,
		-.77f, 0.0f, -0.64f,	-0.42f, 0.0f, -0.58f,	0.3878, 0.0,
		-.64f, 1.0f, -0.77f,	-0.42f, 0.0f, -0.58f,	0.3601, 1.0,
		-.77f, 1.0f, -0.64f,	-0.5f, 0.0f, -0.5f,		0.3878, 1.0,
		-.77f, 0.0f, -0.64f,	-0.5f, 0.0f, -0.5f,		0.3878, 0.0,
		-.87f, 0.0f, -0.5f,		-0.5f, 0.0f, -0.5f,		0.4155, 0.0,
		-.77f, 1.0f, -0.64f,	-0.58f, 0.0f, -0.42f,	0.3878, 1.0,
		-.87f, 1.0f, -0.5f,		-0.58f, 0.0f, -0.42f,	0.4155, 1.0,
		-.87f, 0.0f, -0.5f,		-0.58f, 0.0f, -0.42f,	0.4155, 0.0,
		-.94f, 0.0f, -0.34f,	-0.67f, 0.0f, -0.33f,	0.4432, 0.0,
		-.87f, 1.0f, -0.5f,		-0.67f, 0.0f, -0.33f,	0.4155, 1.0,
		-.94f, 1.0f, -0.34f,	-0.67f, 0.0f, -0.33f,	0.4432, 1.0,
		-.94f, 0.0f, -0.34f,	-0.75f, 0.0f, -0.25f,	0.4432, 0.0,
		-.98f, 0.0f, -0.17f,	-0.75f, 0.0f, -0.25f,	0.4709, 0.0,
		-.94f, 1.0f, -0.34f,	-0.75f, 0.0f, -0.25f,	0.4432, 1.0,
		-.98f, 1.0f, -0.17f,	-0.83f, 0.0f, -0.17f,	0.4709, 1.0,
		-.98f, 0.0f, -0.17f,	-0.83f, 0.0f, -0.17f,	0.4709, 0.0,
		-1.0f, 0.0f, 0.0f,		-0.83f, 0.0f, -0.17f,	0.4986, 0.0,
		-.98f, 1.0f, -0.17f,	-0.92f, 0.0f, -0.08f,	0.4709, 1.0,
		-1.0f, 1.0f, 0.0f,		-0.92f, 0.0f, -0.08f,	0.4986, 1.0,
		-1.0f, 0.0f, 0.0f,		-0.92f, 0.0f, -0.08f,	0.4986, 0.0,
		-.98f, 0.0f, 0.17f,		-1.0f, 0.0f, 0.0f,		0.5263, 0.0,
		-1.0f, 1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,		0.4986, 1.0,
		-.98f, 1.0f, 0.17f,		-1.0f, 0.0f, 0.0f,		0.5263, 1.0,
		-.98f, 0.0f, 0.17f,		-0.92f, 0.0f, 0.08f,	0.5263, 0.0,
		-.94f, 0.0f, 0.34f,		-0.92f, 0.0f, 0.08f,	0.554, 0.0,
		-.98f, 1.0f, 0.17f,		-0.92f, 0.0f, 0.08f,	0.5263, 1.0,
		-.94f, 1.0f, 0.34f,		-0.83f, 0.0f, 0.17f,	0.554, 1.0,
		-.94f, 0.0f, 0.34f,		-0.83f, 0.0f, 0.17f,	0.554, 0.0,
		-.87f, 0.0f, 0.5f,		-0.83f, 0.0f, 0.17f,	0.5817, 0.0,
		-.94f, 1.0f, 0.34f,		-0.75f, 0.0f, 0.25f,	0.554, 1.0,
		-.87f, 1.0f, 0.5f,		-0.75f, 0.0f, 0.25f,	0.5817, 1.0,
		-.87f, 0.0f, 0.5f,		-0.75f, 0.0f, 0.25f,	0.5817, 0.0,
		-.77f, 0.0f, 0.64f,		-0.67f, 0.0f, 0.33f,	0.6094, 0.0,
		-.87f, 1.0f, 0.5f,		-0.67f, 0.0f, 0.33f,	0.5817, 1.0,
		-.77f, 1.0f, 0.64f,		-0.67f, 0.0f, 0.33f,	0.6094, 1.0,
		-.77f, 0.0f, 0.64f,		-0.58f, 0.0f, 0.42f,	0.6094, 0.0,
		-.64f, 0.0f, 0.77f,		-0.58f, 0.0f, 0.42f,	0.6371, 0.0,
		-.77f, 1.0f, 0.64f,		-0.58f, 0.0f, 0.42f,	0.6094, 1.0,
		-.64f, 1.0f, 0.77f,		-0.5f, 0.0f, 0.5f,		0.6371, 1.0,
		-.64f, 0.0f, 0.77f,		-0.5f, 0.0f, 0.5f,		0.6371, 0.0,
		-.5f, 0.0f, 0.87f,		-0.5f, 0.0f, 0.5f,		0.6648, 0.0,
		-.64f, 1.0f, 0.77f,		-0.42f, 0.0f, 0.58f,	0.6371, 1.0,
		-.5f, 1.0f, 0.87f,		-0.42f, 0.0f, 0.58f,	0.6648, 1.0,
		-.5f, 0.0f, 0.87f,		-0.42f, 0.0f, 0.58f,	0.6648, 0.0,
		-.34f, 0.0f, 0.94f,		-0.33f, 0.0f, 0.67f,	0.6925, 0.0,
		-.5f, 1.0f, 0.87f,		-0.33f, 0.0f, 0.67f,	0.6648, 1.0,
		-.34f, 1.0f, 0.94f,		-0.33f, 0.0f, 0.67f,	0.6925, 1.0,
		-.34f, 0.0f, 0.94f,		-0.25f, 0.0f, 0.75f,	0.6925, 0.0,
		-.17f, 0.0f, 0.98f,		-0.25f, 0.0f, 0.75f,	0.7202, 0.0,
		-.34f, 1.0f, 0.94f,		-0.25f, 0.0f, 0.75f,	0.6925, 1.0,
		-.17f, 1.0f, 0.98f,		-0.17f, 0.0f, 0.83f,	0.7202, 1.0,
		-.17f, 0.0f, 0.98f,		-0.17f, 0.0f, 0.83f,	0.7202, 0.0,
		0.0f, 0.0f, 1.0f,		-0.17f, 0.0f, 0.83f,	0.7479, 0.0,
		-.17f, 1.0f, 0.98f,		-0.08f, 0.0f, 0.92f,	0.7202, 1.0,
		0.0f, 1.0f, 1.0f,		-0.08f, 0.0f, 0.92f,	0.7479, 1.0,
		0.0f, 0.0f, 1.0f,		-0.08f, 0.0f, 0.92f,	0.7479, 0.0,
		.17f, 0.0f, 0.98f,		-0.0f, 0.0f, 1.0f,		0.7756, 0.0,
		0.0f, 1.0f, 1.0f,		-0.0f, 0.0f, 1.0f,		0.7479, 1.0,
		.17f, 1.0f, 0.98f,		-0.0f, 0.0f, 1.0f,		0.7756, 1.0,
		.17f, 0.0f, 0.98f,		0.08f, 0.0f, 0.92f,		0.7756, 0.0,
		.34f, 0.0f, 0.94f,		0.08f, 0.0f, 0.92f,		0.8033, 0.0,
		.17f, 1.0f, 0.98f,		0.08f, 0.0f, 0.92f,		0.7756, 1.0,
		.34f, 1.0f, 0.94f,		0.17f, 0.0f, 0.83f,		0.8033, 1.0,
		.34f, 0.0f, 0.94f,		0.17f, 0.0f, 0.83f,		0.8033, 0.0,
		.5f, 0.0f, 0.87f,		0.17f, 0.0f, 0.83f,		0.831, 0.0,
		.34f, 1.0f, 0.94f,		0.25f, 0.0f, 0.75f,		0.8033, 1.0,
		.5f, 1.0f, 0.87f,		0.25f, 0.0f, 0.75f,		0.831, 1.0,
		.5f, 0.0f, 0.87f,		0.25f, 0.0f, 0.75f,		0.831, 0.0,
		.64f, 0.0f, 0.77f,		0.33f, 0.0f, 0.67f,		0.8587, 0.0,
		.5f, 1.0f, 0.87f,		0.33f, 0.0f, 0.67f,		0.831, 1.0,
		.64f, 1.0f, 0.77f,		0.33f, 0.0f, 0.67f,		0.8587, 1.0,
		.64f, 0.0f, 0.77f,		0.42f, 0.0f, 0.58f,		0.8587, 0.0,
		.77f, 0.0f, 0.64f,		0.42f, 0.0f, 0.58f,		0.8864, 0.0,
		.64f, 1.0f, 0.77f,		0.42f, 0.0f, 0.58f,		0.8587, 1.0,
		.77f, 1.0f, 0.64f,		0.5f, 0.0f, 0.5f,		0.8864, 1.0,
		.77f, 0.0f, 0.64f,		0.5f, 0.0f, 0.5f,		0.8864, 0.0,
		.87f, 0.0f, 0.5f,		0.5f, 0.0f, 0.5f,		0.9141, 0.0,
		.77f, 1.0f, 0.64f,		0.58f, 0.0f, 0.42f,		0.8864, 1.0,
		.87f, 1.0f, 0.5f,		0.58f, 0.0f, 0.42f,		0.9141, 1.0,
		.87f, 0.0f, 0.5f,		0.58f, 0.0f, 0.42f,		0.9141, 0.0,
		.94f, 0.0f, 0.34f,		0.67f, 0.0f, 0.33f,		0.9418, 0.0,
		.87f, 1.0f, 0.5f,		0.67f, 0.0f, 0.33f,		0.9141, 1.0,
		.94f, 1.0f, 0.34f,		0.67f, 0.0f, 0.33f,		0.9418, 1.0,
		.94f, 0.0f, 0.34f,		0.75f, 0.0f, 0.25f,		0.9418, 0.0,
		.98f, 0.0f, 0.17f,		0.75f, 0.0f, 0.25f,		0.9695, 0.0,
		.94f, 1.0f, 0.34f,		0.75f, 0.0f, 0.25f,		0.9418, 0.0,
		.98f, 1.0f, 0.17f,		0.83f, 0.0f, 0.17f,		0.9695, 1.0,
		.98f, 0.0f, 0.17f,		0.83f, 0.0f, 0.17f,		0.9695, 0.0,
		1.0f, 0.0f, 0.0f,		0.83f, 0.0f, 0.17f,		1.0, 0.0,
		.98f, 1.0f, 0.17f,		0.92f, 0.0f, 0.08f,		0.9695, 1.0,
		1.0f, 1.0f, 0.0f,		0.92f, 0.0f, 0.08f,		1.0, 1.0,
		1.0f, 0.0f, 0.0f,		0.92f, 0.0f, 0.08f,		1.0, 0.0
	};

	glGenVertexArrays(1, &speakerMesh.vao);
	glGenBuffers(1, &speakerMesh.vbo);

	glBindBuffer(GL_ARRAY_BUFFER, speakerMesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glBindVertexArray(speakerMesh.vao);

	// position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load texture
	speakerTexture = loadTexture("textures/speaker.png");
	speakerShader.use();
	speakerShader.setInt("material.diffuse", 0);

	// load texture
	lightPoleTexture = loadTexture("textures/laptop_base.jpg");
	speakerShader.use();
	speakerShader.setInt("material.diffuse", 0);
};

// draw speaker
void drawSpeaker(GLFWwindow* window, Shader speakerShader) {
	glBindVertexArray(speakerMesh.vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, speakerTexture);


	speakerShader.use();
	speakerShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	speakerShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	speakerShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	speakerShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	speakerShader.setVec3("pointLight.position", cameraPos);
	speakerShader.setVec3("pointLight.ambient", 0.5f, 0.5f, 0.5f);
	speakerShader.setVec3("pointLight.diffuse", 1.0f, 1.0f, 1.0f); // set to the light source color
	speakerShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f); // intensity @ 10%
	speakerShader.setFloat("pointLight.constant", 1.0f);
	speakerShader.setFloat("pointLight.linear", 0.09f);
	speakerShader.setFloat("pointLight.quadratic", 0.032f);

	// material
	speakerShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	speakerShader.setFloat("material.shininess", 32.0f);


	// Scales shape, makes object smaller or larger
	glm::mat4 scale = glm::scale(glm::vec3(0.15f, 0.7f, 0.15f));
	// Rotates object 
	glm::mat4 rotation = glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	// Place obj in center/origin of screen 
	glm::mat4 translation = glm::translate(glm::vec3(2.2f, 0.15f, -1.5f));

	// Model matrix
	// Transformation applied right-to-left
	glm::mat4 model = translation * rotation * scale;
	speakerShader.setMat4("model", model);

	// Draws the triangles
	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
	glDrawArrays(GL_TRIANGLE_FAN, 36, 36);		//top
	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides

	glBindVertexArray(0);

	// -------------- light pole ------------------------

	glBindVertexArray(speakerMesh.vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lightPoleTexture);


	speakerShader.use();
	speakerShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	speakerShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	speakerShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	speakerShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	speakerShader.setVec3("pointLight.position", cameraPos);
	speakerShader.setVec3("pointLight.ambient", 0.5f, 0.5f, 0.5f);
	speakerShader.setVec3("pointLight.diffuse", 1.0f, 1.0f, 1.0f); // set to the light source color
	speakerShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f); // intensity @ 10%
	speakerShader.setFloat("pointLight.constant", 1.0f);
	speakerShader.setFloat("pointLight.linear", 0.09f);
	speakerShader.setFloat("pointLight.quadratic", 0.032f);

	// material
	speakerShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	speakerShader.setFloat("material.shininess", 32.0f);


	// Scales shape, makes object smaller or larger
	scale = glm::scale(glm::vec3(0.05f, 2.1f, 0.05f));
	// Rotates object 
	rotation = glm::rotate(glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	// Place obj in center/origin of screen 
	translation = glm::translate(glm::vec3(-2.8f, 0.0f, -1.7f));

	// Model matrix
	// Transformation applied right-to-left
	model = translation * rotation * scale;
	speakerShader.setMat4("model", model);

	// Draws the triangles
	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
	glDrawArrays(GL_TRIANGLE_FAN, 36, 36);		//top
	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides

	glBindVertexArray(0);

}

// create light source
void createLightSource(GLMesh& lightMesh, Shader lightShader) {
	GLfloat cubeVerts[] = {
		// positions			// normals				// texture coords
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f,  1.0f
	};
	glGenVertexArrays(1, &lightMesh.vao);
	// create VBO
	glGenBuffers(1, &lightMesh.vbo);

	// vertex data
	glBindBuffer(GL_ARRAY_BUFFER, lightMesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

	glBindVertexArray(lightMesh.vao);

	// position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

};

// draw light source
void drawLightSource(GLFWwindow* window, Shader lightShader) {

	glBindVertexArray(lightMesh.vao);

	lightShader.use();
	// scale obj
	glm::mat4 scale = glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
	// Rotates object 
	glm::mat4 rotation = glm::rotate(glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	// Place obj  
	glm::mat4 translation = glm::translate(lightPos);

	// Model matrix
	// Transformation applied right-to-left
	glm::mat4 model = translation * rotation * scale;
	// Send transform info to the Vertex shader
	lightShader.setMat4("model", model);
	lightShader.setVec3("lightcolor.color", 1.0f, 1.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

// delete mesh data
void deleteMesh(GLMesh& mesh) {
	glDeleteVertexArrays(1, &mesh.vao);
	glDeleteBuffers(1, &mesh.vbo);
};

// load texture
unsigned int loadTexture(char const* path) {
	
	unsigned int textureID;
	// load and create texture
	glGenTextures(1, &textureID);

	// load image, gen texture, generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data) {
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set texture wrapping parameteres
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
		stbi_image_free(data);
	};
	return textureID;
};

// load cubeMap texture
unsigned int loadCubeMap(std::vector<std::string> faces) {

	unsigned int textureID;
	//bind texture 
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		};

	};


	return textureID;
};
//-----------------------------------------------------------------------------------------------------