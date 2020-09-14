#include<glew.h>
#include<glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int hellowindow();

GLfloat vertices[] = {
	-0.5f, -0.5f, -0.0f,		
	0.5f,  -0.5f,  0.0f,		
	0.0f,   0.5f,  0.0f,		
	0.8f,   0.8f,  0.0f			
};

GLuint indices[] = {
	0,  1,  2,
	2,  1,  3
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
	//glEnable(GL_CULL_FACE);//面剔除功能开启
	//glCullFace(GL_BACK);//去除背面，也就是顺时针排点画出的图形
	//glCullFace(GL_FRONT);//去除正面
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);勾边函数，面剔除功能会影响勾边。


	glewExperimental = true;

	
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "glewInit fail";
		glfwTerminate();
		return -1;
	}


	GLuint VAO;  
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);   


	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//使用标签EBO 创建一个EBO（） 
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//使用宏 GL_ELEMENT_ARRAY_BUFFER 将EBO标签指向的缓存 定义到VAO的对应栏位上。
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);




	//此处是简易的vertexshader&fragmentShader, 接下来的项目版本此处将会	 使用api加载外部文档 内的shader					 

	const GLchar* vertexShaderSource =
		"#version 330 core                                       \n"
		"layout(location = 15) in vec3 aPos;                     \n"//这条脚本的意思就是，将零号特征值的内容塞进vec3类型的aPos中
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
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	//此处使用OpenGLapi创建一个shader，并加载源代码进去

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);



	//接下来将两个shader封装成一个shader program，这样接下来就可以交付给GPU 执行你的shader了。

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


//////////////////////////////////////////////////////////////////////////////////////////////////////



	glVertexAttribPointer(15, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	
	glEnableVertexAttribArray(15);




	//不断循环绘制 ->
	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);
		//首次循环用户输入为空忽略不计，此处为处理之前帧的用户输入。


		//rendering(渲染当前帧) commands here
		//...
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);//绑定要绘制的VAO（模型）
		glUseProgram(shaderProgram);//绑定要使用的Program（shader）

			
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//通过Elements访问VAO来绘制三角形，首个参数代表要画的是三角形，
		//6代表顶点个数，GL_UNSIGNED_INT 代表使用的数据再VAO中的类型，0代表在VAO中的偏移值。
		
		//glDrawArrays(GL_TRIANGLES, 0, 6);//使用顶点数据绘制三角形

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