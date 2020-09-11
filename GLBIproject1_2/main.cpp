#include<glew.h>
#include<glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int hellowindow();

GLfloat vertices[] = {
	-0.5f, -0.5f, -0.0f,
	0.5f,  -0.5f,  0.0f,
	0.0f,   0.5f,  0.0f
};


int main() {
	hellowindow();
	return 0;
}

int hellowindow(){
	std::cout << "HELLO! WINDOW";

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(1000,700,"test",NULL,NULL);

	if (window == NULL)
	{
		std::cerr << "window create fail!";
		glfwTerminate();
		return -1;
	}



	
	glfwMakeContextCurrent(window);
	

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	glewExperimental = true;

	
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "glewInit fail";
		glfwTerminate();
		return -1;
	}

	GLuint VAO;  //VAOID存放地址。
	glGenVertexArrays(1, &VAO);
	//这条方法可以产生多个VAO,并返还VAOID，（数量，VAO地址，因为是取址，所以可以传入指针常量哦）最终填充到VAOID容器中。
	//GLuint VAO[n]
	//glGenVertexArrays(n, &VAO);

	//接下来呼叫OpenGL加载(绑定)这些VAO，将GL_ARRAY_BUFFER 链接到 VAO
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	//GLuint VBO[n];
	//glGenBuffers(n, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//利用VBOID将VBO绑定在 GL_ARRAY_BUFFER 上，此时VAO已经在前一步加载到了GL_ARRAY_BUFFER上
	//也就是说通过 GL_ARRAY_BUFFER，VBO顺利链接到了VAO。

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//注意：此处并没有使用VBO，而是直接将vertices装载到了直属VAO的 GL_ARRAY_BUFFER 结构
	//最后的参数 代表 此（模型的）数据是否会在接下来无穷多帧的绘制之中 经常改动，因为我们本次画的是一个静态的三角形，因此选择GL_STATIC_DRAW
	//最后的参数包括：
	//GL_STATIC_DRAW 几乎没有变动
	//GL_DYNAMIC_DRAW 会改动很多次
	//GL_STREAM_DRAW 几乎总在变动

	//此处是简易的vertexshader&fragmentShader, 接下来的项目版本此处将会	 使用api加载外部文档 内的shader					 

	const GLchar* vertexShaderSource =
		"#version 330 core                                       \n"
		"layout(location = 0) in vec3 aPos;                      \n"
		"void main() {											 \n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}		 \n";
	//这样会自动在这个地址内 存：const GLUbyte的二维聚合类型。																    

	const GLchar* fragmentShaderSource =
		"#version 330 core                                       \n"
		"out vec4 FragColor;                      				 \n"
		"void main() {											 \n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);}				 \n";





	/*接下来是  添加shader代码 以及编译链接shader代码 的部分*/
	/*此部分整体需要补充一定的纠错代码，因为OpenGL自身不提供，也无义务提供完整的纠错体系，*/

	/*因此shader源代码编译链接发生错误的时候无法进行细节分析，需要人工补充一定的纠错*/
	/*但本次先掠过，之后细节理顺shader知识点的时候再补充*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////

			 //此处使用OpenGLapi创建一个shader，并加载源代码进去

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader;//创建shaderID
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//创建shader实体，并挂载到指定ID
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//接入shader源代码到shaderID
	glCompileShader(fragmentShader);//编译shader



	//接下来将两个shader封装成一个shader program，这样接下来就可以交付给GPU 执行你的shader了。

	GLuint shaderProgram;//申请一个ProgramID
	shaderProgram = glCreateProgram();//初始化一个Program，并返回该Program的ID
	glAttachShader(shaderProgram, vertexShader);//为shaderProgram指定的Program 添加编译过的（shader）目标代码。
	glAttachShader(shaderProgram, fragmentShader);//为shaderProgram指定的Program 添加编译过的（shader）目标代码。
	glLinkProgram(shaderProgram);//链接shaderProgram指定的Program


//////////////////////////////////////////////////////////////////////////////////////////////////////



	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);


	//不断循环绘制 ->
	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);
		//首次循环用户输入为空忽略不计，此处接受当前帧的用户输入，待到下一帧渲染结束并swap到前台后响应


		//rendering(渲染) commands here
		//...

		glClearColor(1.0f, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		//攫取新的用户输入，以及其他事件
		glfwPollEvents();
		//swap buffers
		glfwSwapBuffers(window);
	}
	//结束循环绘制 <-

	glfwTerminate();



}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)	
	{
		glfwSetWindowShouldClose(window, true);
	}
}; 