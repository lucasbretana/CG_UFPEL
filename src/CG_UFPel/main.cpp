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
#include <glm/gtx/spline.hpp>
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
	//Model novo(FileSystem::getPath("resources/objects/rock/rock.obj"));


		// render loop
	double atualTime=0.0;
	double firstTime =0.0;
	glm::mat4 model;
	glm::vec3 inicial = glm::vec3(-2.0f, -1.0f, 0.0f);
    glm::vec3 einicial = glm::vec3(0.1f, 0.1f, 0.1f);
	//parâmetros da curva
	double delt;
	//vector<Model> models;
	while (!glfwWindowShouldClose(window))
	{
		//glm::vec3 pont = ourModel.calculoponto();
		//camera.angulocamera(pont);
		

		ourModel.calculoDelta();

		//wireframe
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//adicionaobjeto
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && ourModel.retornaDelta() > 0.5f)
		{
			ourModel.adicionaObjeto();
	
		}
		//rodar por ponto
		
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
			glm::vec3 p(-2.5f,1.0f,-2.0f);
			ourModel.roda(10,p,window,ourShader);
		}
		//troca de modelos
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && ourModel.retornaDelta() > 0.5f){
			ourModel.trocaObjetoMax();
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && ourModel.retornaDelta() > 0.5f){
			ourModel.trocaObjetoMin();
		}
		//Animação translação
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
			ourModel.translat(3,ourShader,window);

		}
		//rodaobjetoespecifico
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
			//ourModel.rodaobjetoespecifico();
		}
		// roda em tornoo do ponto 
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
			glm::vec3 ponto(0.5f, 0.5f, 0.5f);
			ourModel.rodaemponto(ponto);
		}
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
			ourModel.bezier();
		}
		//escala
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
			ourModel.escala(window,ourShader,0.8f,1);
		}
		if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && ourModel.retornaDelta() > 0.5f){
			ourModel.rodanoeixo(10,window,ourShader,1);
		}

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		//model = glm::translate(model, inicial); // translate it down so it's at the center of the scene
       // model = glm::scale(model,einicial);
		//ourShader.setMat4("model", model);
		//novo.Draw(ourShader);
		// input22
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
		

		ourModel.desenha(ourShader);
		
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
	if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.AnimationCam(deltaTime,4);
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
// Zoom
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