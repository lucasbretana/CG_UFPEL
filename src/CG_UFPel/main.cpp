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
vector<Camera> cameras;
int cameraCorrente = 0;
int quantidadeCamera =0;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float distancia = 0.2f;
float temp = 2.00f;
float x = 0.0f;

int main()
{
	//camera
	Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
	cameras.push_back(cam);
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
	Model planet(FileSystem::getPath("resources/objects/planet/planet.obj"));
	//Model rock(FileSystem::getPath("resources/objects/rock/rock.obj"));
	//Model cyborg(FileSystem::getPath("resources/objects/cyborg/cyborg.obj"));
	//Model novo(FileSystem::getPath("resources/objects/rock/rock.obj"));

	/*vector<Model> models;
	int modelcorrent=0;
	models.push_back(nano);
	models.push_back(planet);
	models.push_back(rock);
	models.push_back(cyborg);*/
		// render loop
	const double duracao =3.0;
	glm::mat4 model;
	glm::vec3 inicial = glm::vec3(-20.37f,0.0f,-1.0f);
    glm::vec3 einicial = glm::vec3(0.1f, 0.1f, 0.1f);
	
	//parametros de time
	double delta_Time=0.0;
	double atualTime=0.0;
	double first_Time =0.0;
	int n,aux=1;
	glm::mat4 view;
	bool playAnimationLinear = false;
	bool playAnimationLinearCam = false;
	bool playRotationCam = false;
	bool playRotation = false;
	float radius = 10.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	float valx,valy,valz;
	while (!glfwWindowShouldClose(window))
	{
		view = cameras[cameraCorrente].GetViewMatrix();
		ourModel.calculoDelta();
		//models[modelcorrent].calculoDelta();

		//wireframe
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/*troca de modelo
		if ((glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)) {
			if(aux<4)
				++aux;
			if ((modelcorrent + 1) >= models.size())
				modelcorrent = 0;
			else
				++modelcorrent;
		}*/
		//adicionaobjeto
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && ourModel.retornaDelta() > 0.5f){
			 ourModel.adicionaObjeto();
			
		}

	
		//rodar por ponto
		
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
			glm::vec3 p(-2.5f,1.0f,-2.0f);
			ourModel.roda(10,inicial,window,ourShader);
			//models[modelcorrent].roda(10,p,window,ourShader);
		}
		//troca de objeto
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && ourModel.retornaDelta() > 0.5f)
			ourModel.trocaObjetoMax();
			//models[modelcorrent].trocaObjetoMax();
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && ourModel.retornaDelta() > 0.5f)
			ourModel.trocaObjetoMin();
			//models[modelcorrent].trocaObjetoMin();
		//Animação translação
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
			playAnimationLinear = true;
			first_Time = glfwGetTime();
			delta_Time =0.0;
		}
		if(playAnimationLinear){
			if(delta_Time<10){
				delta_Time = glfwGetTime() - first_Time;
					ourModel.translat(delta_Time,ourShader,window);		
			}else{
				playAnimationLinear = false;
				delta_Time=0.0;
			}
		}
		// roda em tornoo do ponto 
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
			glm::vec3 ponto(1.5f,-4.0f, 0.0f);
			ourModel.rodaemponto(ponto,ourShader,window);
			//models[modelcorrent].rodaemponto(ponto);
		}
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
			ourModel.bezier();
		//escala
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && ourModel.retornaDelta() > 0.5f)
			ourModel.escala(1.3f,1);
			//models[modelcorrent].escala(window,ourShader,0.8f,1);
		if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && ourModel.retornaDelta() > 0.5f){
			playRotation = true;
			first_Time = glfwGetTime();
			delta_Time =0.0;
		}

			
			//models[modelcorrent].rodanoeixo(10,window,ourShader,1);
		if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS ){
			glm::vec3 aux = ourModel.retornaPositionObj();
			 std::cout << "ponto: x" << aux.x << "y: " << aux.y << " z: "<< aux.z << std::endl;
		}	

		if(playRotation){
			if(delta_Time<10){
				delta_Time = glfwGetTime() - first_Time;
					ourModel.rodanoeixo(delta_Time,window,ourShader,1);
			}else{
				playRotation = false;
				delta_Time=0.0;
			}
		}

		//CAMERA
		
		//adicioma camera nova
		if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			Camera cam(glm::vec3(0.0f, 3.0f, 0.0f));
			cameras.push_back(cam);
		}
		//troca camera
		if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS ){
			if(cameraCorrente < cameras.size()-1)
				++cameraCorrente;
			else
				cameraCorrente = 0;
		}
		if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS ){
			
		}
		//troca de camera
		
		
		//olhar para o ponto
		if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS )
			 view = cameras[cameraCorrente].trocaLook(glm::vec3(-2.0f,0.0f,0.0f));
		//segue modelo não funciona
		if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS ){
			playAnimationLinearCam=true;
			first_Time = glfwGetTime();
			delta_Time =0.0;
		}
		if(glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS )
			cameras[cameraCorrente].AnimationCam(10,deltaTime);
		//ruido simula caminhada
		if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS){
				float x =( sin(glfwGetTime()) / 2);
				if(x<0)
					x = -x;
				cameras[cameraCorrente].Position.y = x;
			}
			
			if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS  ){
                    view = glm::rotate(view,  glm::radians(45.0f),  glm::vec3(0.0f, 0.0f, 1.0f) );
                    view = glm::translate(view,glm::vec3(0.0f,0.0f,1.0f));
				
			}
		//segue modelo 
		if(playAnimationLinearCam){
				if(delta_Time<2){
					delta_Time = glfwGetTime() - first_Time;
			 		cameras[cameraCorrente].acompanha(ourModel.retornaPositionObj(),delta_Time);
			 	}else{
				playAnimationLinearCam = false;
				delta_Time=0.0;
			}
		}

		

		
			 
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		// render
		// ------
		model = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f,0.1f,0.1f)), inicial);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		
		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		/* roda em torno do eixo x,y,z
		valx = cameras[cameraCorrente].Position.x;
		valy = cameras[cameraCorrente].Position.y;
		valz = cameras[cameraCorrente].Position.z;
		*/
		//roda em ponto talvez
		if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
			view = glm::lookAt(glm::vec3(camX, 0.0 , camZ) , glm::vec3(0.0, 0.0,0.0), glm::vec3(0.0, 1.0,0.0));
		
		/*if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
			view = glm::lookAt(glm::vec3(valx, camX, valz), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0,0.0));
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
			view = glm::lookAt(glm::vec3(camX, valy,  camX), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0,0.0));
		
		*/

		glm::mat4 projection = glm::perspective(glm::radians(cameras[cameraCorrente].Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.8f, 100.0f);
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		ourShader.setMat4("model", model);
		planet.Draw(ourShader);
		

		
		/*for (  n= 0; n <aux; ++n)
		{
			models[n].desenha(ourShader);
		}
		*/
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
		cameras[cameraCorrente].AnimationCam(deltaTime,4);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameras[cameraCorrente].ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameras[cameraCorrente].ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameras[cameraCorrente].ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameras[cameraCorrente].ProcessKeyboard(RIGHT, deltaTime);

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

	cameras[cameraCorrente].ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameras[cameraCorrente].ProcessMouseScroll(yoffset);
}