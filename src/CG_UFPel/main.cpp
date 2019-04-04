#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float firstTime = 0.0f;
float distancia = 0.2f;
float temp = 2.00f;
float x = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader ourShader(FileSystem::getPath("resources/cg_ufpel.vs").c_str(), FileSystem::getPath("resources/cg_ufpel.fs").c_str());

	// load models
	// -----------
	Model ourModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));

	/* varios modelos
	vector<Model> objs;
	vector<int> model;
	vector<glm::mat4> transform;
	int n = 5,i = 0;
	float position = 0.5;
	for (i = 0; i < n; i++) {
		model.push_back(i);
		glm::mat4 mat;
		mat = glm::translate(mat, glm::vec3((float)position, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		mat = glm::scale(mat, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down;
		position += 0.5f;
		transform.push_back(mat);
		*/

		// render loop
		// -----------
	glm::vec3 pinicial = glm::vec3(0.0f, -1.00f, 0.0f);
	glm::vec3 escalainicial = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 pfinal = pinicial;
	glm::vec3 patual = pinicial;
	glm::mat4 model;
	model = glm::translate(model, pinicial); // translate it down so it's at the center of the scene
	model = glm::scale(model, escalainicial);	// it's a bit too big for our scene, so scale it down 
	//modelesquerda = glm::translate(modelesquerda, glm::vec3(0.8f, -1.0f, 0.0f));
	//delesquerda = glm::scale(modelesquerda, glm::vec3(0.1f, 0.1f, 0.1f));

	float pontoinicial = 0.0f;
	float t = 0.0f;
	float delta = 0.0f;
	glm::vec3 p;
	while (!glfwWindowShouldClose(window))
	{
		//wireframe
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// a ideia seria que a cada 2s ele ande 0.2 no x exemplo, então em 10s ele vai dar 1 em x
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
			float firstTime= (float)glfwGetTime();
			pfinal.x = (10 * 0.2) / 2;
			while (t < 10.0f) {
				float atualTime = (float)glfwGetTime();
				delta = atualTime - firstTime;
				t = t + delta;
				if(t!=0.0f)
					patual.x =( pinicial.x) + ((0.2*t) / 2);
				else
					patual.x = (pinicial.x) + 0.2;
				model = glm::translate(model, patual);
				glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				ourShader.setMat4("model", model);
				ourModel.Draw(ourShader);

			}
			t = 0.0f;
		}

		// angulo da camera
		std::cout << "angulo" << std::endl;
		std::cout << glm::acos(glm::dot(camera.Position,pinicial )) << std::endl;

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
			model = glm::translate(model, glm::vec3(0.50f, 0.00f, 0.0f));
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.00f, 0.0f));
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);
		//ourShader.setMat4("model", modelesquerda);
		//ourModel.Draw(ourShader);

		/*(for (int i = 0; i < n; ++i) {
			// render the loaded model
			ourShader.setMat4("model", transform[i]);
			ourModel.Draw(ourShader);
		}*/

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}