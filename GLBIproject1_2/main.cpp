#include<glew.h>
#include<glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int hellowindow();

GLfloat vertices[] = {
	-0.5f, -0.5f, -0.0f,		// 0
	0.5f,  -0.5f,  0.0f,		// 1
	0.0f,   0.5f,  0.0f,		// 2
	//逆时针排点					
	0.5f,  -0.5f,  0.0f,		
	0.0f,   0.5f,  0.0f,		
	0.8f,   0.8f,  0.0f			// 3  ，为点标号后，发现除重复点外，只有四个非重复点
	//顺时针排点，

	//按照逆时针的绘制方式将画不出来图形

};//三角形的缺省绘制是按照逆时针绘点

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


	glewExperimental = true;

	
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "glewInit fail";
		glfwTerminate();
		return -1;
	}

	//加载VAO，将此VAO设置为正在使用的VAO
	GLuint VAO;  
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);   //此函数用于绑定或解绑一个 顶点数组对象，
							  //当传入的既不是顶点数组对象，又不是“0”时，则返回错误值：GL_INVALID_OPERATION
							  //当传入的是“0”时，解绑当前指定的VAO

	//使用标签VBO 创建一个VBO（数据缓冲区对象）, 
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//绑定该VBO（缓冲区对象）到标签：GL_ARRAY_BUFFER 上
	//宏标签 GL_ARRAY_BUFFER 的定义：vertax attribu顶点属性，因此此标识用于标识 该VBO指向的 缓冲区用于存放vertex attribute。

	//将模型数据（此处仅有三点）暂存到 GL_ARRAY_BUFFER绑定的VBO 指向的一段数据缓冲区中。
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



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
	//此处函数即是用于处理： 
	//		利用 GL_ARRAY_BUFFER 传入 VBO 的VertexAttributes , 并将其指向VAO
	//		将GL_ARRAY_BUFFER指向的VBO 数据转移到 VAO中，并标注特征值，特征值范围为 [0,15]
	//		上条代码作用为：
	//				在VAO中，创建一个 以0号特征值 标注的栏位，每三份创建一份资料，
	//				每份都是一个GL_FLOAT类型，不需要规范化到正负1（也可以选择规划到0到1，或-1到0），
	//				挖掘间隔，本次挖掘的起始偏移量。
	//
	//		具体特征值设置使用 详见jianghuaiyue@outlook.com -> Onenote -> OpenGL -> OpenGL基本知识 -> VAO,VBO&以及shader的简单使用流程

	glEnableVertexAttribArray(15);
	//开启此特征值，注意，只有开启了这个特征值，之后opengl_api以及shader脚本 才能使用这个特征值，
	//详见: 
	//	shader/OpenGLPersetVertexShader(simple).gls 
	//	line->2: “layout (location = 0) in vec3 aPos;\n”



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

		glDrawArrays(GL_TRIANGLES, 0, 6);//使用数据绘制三角形

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