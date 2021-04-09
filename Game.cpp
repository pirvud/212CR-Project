#include "Game.h"


/*//////////////////////////////////////////////////

All the models are made by me

Referance:

Main inspiration https://www.youtube.com/channel/UC2i39AOpDSlO1Mrn1jQ8Xkg
research https://learnopengl.com
Camera: https://learnopengl.com/Getting-started/Camera


//////////////////////////////////////////////////*/

//Private functions
void Game::initGLFW()
{
	//INIT GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initWindow(
	const char* title,
	bool resizable)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

	if (this->window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	glfwSetFramebufferSizeCallback(window, Game::framebuffer_resize_callback);

	//glViewport(0, 0, framebufferWidth, framebufferHeight);    //canvas size

	glfwMakeContextCurrent(this->window); //IMPORTANTT!!

}

void Game::initGLEW()
{
	//INIT GLEW ( NEEDS WIDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	//Error
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initOpenGLOptions()
{
	//OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST);  //Makeing Z coordinate

	glEnable(GL_CULL_FACE);  //Doesn't draw what is behind the object
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);	 //The face is count clocwaise 

	glEnable(GL_BLEND);		 //Enable blending colors
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//input
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //fill the shape with the colo   (GL_LINE  - is showing just he wireframe)

	//Input
	//DISABLE ON-SCREEN CURSOR
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);   //put curson inside window

}

void Game::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);
}

void Game::initShaders()
{
	//SHADER INIT
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
		"vertex_core.glsl", "fragment_core.glsl"));
}

void Game::initTextures()
{
	//TEXTURE INIT ----
	//TEXTURE 0
	this->textures.push_back(new Texture("Images/car_material.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/car_specular.png", GL_TEXTURE_2D));

	//TEXTURE 1
	this->textures.push_back(new Texture("Images/path_material.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/path_specular.png", GL_TEXTURE_2D));

	this->textures.push_back(new Texture("Images/grass.png", GL_TEXTURE_2D));

	this->textures.push_back(new Texture("Images/sky.png", GL_TEXTURE_2D));

	this->textures.push_back(new Texture("Images/banner_texture.png", GL_TEXTURE_2D));

	this->textures.push_back(new Texture("Images/banner_2.png", GL_TEXTURE_2D));
}

void Game::initMaterials()
{

	//MATERIAL 0
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(6.f),				
		0, 1));																							// 0 - DIFFUSE TEXTURE      1 - SPECULAR TEXTURE

}

void Game::initOBJModels()
{

}

void Game::initModels()
{
	std::vector<Mesh*>meshes;
	std::vector<Mesh*>meshes2;
	std::vector<Mesh*>meshes3;

	meshes.push_back(								//Pyramid
		new Mesh(
			&Pyramid(),
			glm::vec3(0.f, 0.f, 0.f),				//all pyramids controller
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(0.f)
		)
	);


	meshes2.push_back(								//GROUND
		new Mesh(
			&Quad(),
			glm::vec3(0.f, 0.f, 0.f),				
			glm::vec3(0.f),
			glm::vec3(-90.f, 0.f, 0.f),
			glm::vec3(300.f)
		)
	);



	this->models.push_back(new Model(					//Ground
		glm::vec3(2.f, -0.50f, 2.f),
		this->materials[0],
		this->textures[TEX_GRASS],
		this->textures[TEX_PATH_SPECULAR],
		meshes2
	)
	);


	this->models.push_back(new Model(					//Car
		glm::vec3(0.f, 0.f, 0.f),
		this->materials[0],
		this->textures[TEX_CAR],
		this->textures[TEX_PATH_SPECULAR],
		"OBJFiles/dodgeengine.obj"
	)
	);
		
	this->models.push_back(new Model(					//Path
		glm::vec3(0.f, -0.47f, -80.f),
		this->materials[0],
		this->textures[TEX_PATH],
		this->textures[TEX_PATH_SPECULAR],
		"OBJFiles/Path.obj"
	)
	);

	this->models.push_back(new Model(					//Sky
		glm::vec3(0.f, -10.f,150.f),
		this->materials[0],
		this->textures[TEX_SKYBOX],
		this->textures[TEX_PATH_SPECULAR],
		"OBJFiles/SkyBox.obj"
	)
	);

	this->models.push_back(new Model(					//Banner_1
		glm::vec3(-10.f, +2.f, +30.f),
		this->materials[0],
		this->textures[TEX_BANNER],
		this->textures[TEX_PATH_SPECULAR],
		"OBJFiles/banner.obj"
	)
	);

	this->models.push_back(new Model(					//Banner_2
		glm::vec3(15.f, +8.f, +30.f),
		this->materials[0],
		this->textures[TEX_BANNER2],
		this->textures[TEX_PATH_SPECULAR],
		"OBJFiles/banner_2.obj"
	)
	);

	for (auto*& i : meshes)
		delete i;

	for (auto*& i : meshes2)
		delete i;
}

void Game::initPointLights()
{
	this->pointLights.push_back(new PointLight(glm::vec3(10.f)));
}

void Game::initLights()
{
	this->initPointLights();

}

void Game::initUniforms()
{
	//INIT UNIFORMS
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	for each (PointLight* pl in this->pointLights)
	{
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}
	
}

void Game::updateUniforms()
{

	//Update view matrix(camera)
	this->ViewMatrix = this->camera.getViewMatrix();

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");  //perspective and same ratio at resize window
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camera.getPosition(), "cameraPos");

	for each (PointLight * pl in this->pointLights)
	{
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}

	//Update framebuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane);

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");  //perspective and same ratio at resize window

}

//Constructors  / Destructors

Game::Game(const char* title,
	const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
	bool resizable)
	
	:
	WINDOW_WIDTH(WINDOW_WIDTH), 
	WINDOW_HEIGHT(WINDOW_HEIGHT),
	GL_VERSION_MAJOR(GL_VERSION_MAJOR), 
	GL_VERSION_MINOR(GL_VERSION_MINOR),
	camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
{
	//Init variable
	this->window = nullptr;
	this->framebufferWidth = this->WINDOW_WIDTH;
	this->framebufferHeight = this->WINDOW_HEIGHT;

	this->camPosition = glm::vec3(0.f, 0.f, 1.f);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);

	this->fov = 60.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->dt = 0.f;
	this->curTime = 0.f;
	this->lastTime = 0.f;

	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();
	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initOBJModels();
	this->initModels();
	this->initLights();
	this->initUniforms();
}

Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();

	//Clean up memory
	for (size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];

	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];

	for (size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];

	for (auto*& i : this->models)
		delete i;

	for (size_t i = 0; i < this->pointLights.size(); i++)
		delete this->pointLights[i];

}

//Accessor
int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

//Modifier
void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

//Functions

void Game::updateDt()										//delta time
{
	this->curTime = static_cast<float>(glfwGetTime());	//take time
	this->dt = this->curTime - this->lastTime;			//current time - last time
	this->lastTime = this->curTime;						//last time will be current time

}

void Game::updateMouseInput()
{
	//https://www.glfw.org/docs/3.3/input_guide.html

	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);		//get cursor position from window and add them to variables

	if (this->firstMouse)
	{
		this->lastMouseX = this->mouseX;		//last mouse pos. will be the current one
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	//Calc offset
	this->mouseOffsetX = this->mouseX - this->lastMouseX;		//calculate the offset
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	//Set last X and Y
	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;

	//Move light
	//if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	//{
	//	this->pointLights[0]->setPosition(this->camera.getPosition());
	//}

}

bool brakeCheckForward = 0;			//Check if the speed is 0
bool brakeCheckBackward = 0;

void Game::updateKeyboardInput()
{
	//Program close
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, GLFW_TRUE);
	}

	//Camera
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		this->camera.move(this->dt, FORWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->camera.move(this->dt, BACKWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		this->camera.move(this->dt, LEFT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		this->camera.move(this->dt, RIGHT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS)
	{
		this->camPosition.y -= 0.0005f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		this->camPosition.y += 0.0005f;
	}

	if (glfwGetKey(this->window, GLFW_KEY_R) == GLFW_PRESS)			//RESET car position 
	{
		this->models[1]->resetCar(glm::vec3(0.f));
	}


	//Car movement
	if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		this->models[1]->moveUp(glm::vec3(0.f));
		this->models[1]->speed += 1.f * this->dt;
		brakeCheckForward = 1;
	}

	if(brakeCheckForward == 1)
	{ 
		if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_RELEASE)			//Decelerate when the key is released
		{
			this->models[1]->moveUp(glm::vec3(0.f));
			if (this->models[1]->speed > 0)
			{
				this->models[1]->speed -= 1.f * this->dt;
			}
			if (this->models[1]->speed <= 0)
			{
				brakeCheckForward = 0;
			}
		}
	}

	if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		this->models[1]->moveDown(glm::vec3(0.f));
		brakeCheckBackward = 1;
	}

	if (brakeCheckBackward == 1)
	{
		if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_RELEASE)			//Decelerate when the key is released
		{
			this->models[1]->moveDown(glm::vec3(0.f));
			if (this->models[1]->speed > 0)
			{
				this->models[1]->speed -= 1.f * this->dt;
			}
			if (this->models[1]->speed <= 0)
			{
				brakeCheckBackward = 0;
			}
		}
	}

	if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		this->models[1]->rotate(glm::vec3(0.f, -0.05f, 0.f));
	}

	if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		this->models[1]->rotate(glm::vec3(0.f, +0.05f, 0.f));
	}
	

	//Debug mode
	if (glfwGetKey(this->window, GLFW_KEY_F) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(this->window, GLFW_KEY_G) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Game::updateInput()
{
	glfwPollEvents();

	this->updateKeyboardInput();
	this->updateMouseInput();
	this->camera.updateInput(dt, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Game::update()
{
	//UPDATE INPUT--
	this->updateDt();
	this->updateInput();



	this->models[5]->rotate(glm::vec3(0.f, 0.05f, 0.f));		//rotate BANNER2

	
	//Move the light to camera position
	this->pointLights[0]->setPosition(this->camera.getPosition());		//bring the light to the camera constant

}

void Game::render()
{

	//DRAW ---
	//Clear

			   //Red Green Blue Alpha
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  //Pixels that can be colored 

	//Update the uniforms
	this->updateUniforms();

	//Render models
	for (auto&i : this->models)
		i->render(this->shaders[SHADER_CORE_PROGRAM]);			//Mesh 1
			

	//End Draw
	glfwSwapBuffers(window);  //is beeing shown on the screen
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//Static functions
void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
};

