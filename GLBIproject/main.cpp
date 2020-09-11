#include<glew.h>
#include<glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int hellowindow();

//int main() {
//	hellowindow();
//	return 0;
//}

int hellowindow(){
	std::cout << "HELLO! WINDOW";

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //初始化窗口需要使用的OpenGL主版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //初始化窗口需要使用的OpenGL副版本，最终也就是3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//选定设置项为PROFILE，
	//OpenGL旧版使用固定管道，现今使用可编程管道
	//设置OpenGL将要使用的配置文件类型为CORE_PROFILE，这是OPENGL可编程管道中最精简的配置文件类型。

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TURE);
	//如果要进行macos的开发才需要使用这条设置，来进行opengl的前项设置。

	//OpenGL GLFW Window Create  ->
	GLFWwindow* window = glfwCreateWindow(1000,700,"test",NULL,NULL);

	if (window == NULL)
	{
		std::cerr << "window create fail!";
		glfwTerminate();//终止glfw，清理资源;
		return -1;
	}



	//return 0;
	glfwMakeContextCurrent(window);
	//关联窗口进程上下文；

	//OpenGL GLFW Window Create  <-

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//Init GLEW ->
	glewExperimental = true;//开启glew实验性功能；

	//验证GLEW是否成功初始化->
		//glew函数库设定，某些glew函数如果成功返回，则返回值会是GLEW_OK;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "glewInit fail";
		glfwTerminate();//终止glfw，清理资源;
		return -1;
	}
	//Init GLEW <-
	//glViewport(0, 0, 1000, 700);
	//确定需要渲染的矩形区域，前两个参数为该区域左下角坐标，以及宽高。


	//不断循环绘制 ->
	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);
		//首次循环用户输入为空忽略不计，此处接受当前帧的用户输入，待到下一帧渲染结束并swap到前台后响应


		//rendering(渲染) commands here
		//...

		glClearColor(1.0f, 0, 0, 1.0f);//选取要替换的颜色。
		glClear(GL_COLOR_BUFFER_BIT);//选取你要清除的缓冲区。


		//攫取新的用户输入，以及其他事件
		glfwPollEvents();
		//swap buffers
		glfwSwapBuffers(window);
	}
	//结束循环绘制 <-

	glfwTerminate();//终止glfw，清理资源;

	return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)	
	{//意思就是，接受这个窗口的窗口过程接收的用户输入，GLFW_KEY_ESCAPE键值对应的按键接受的事件是：被press按住了。
		glfwSetWindowShouldClose(window, true);
	}
}; 