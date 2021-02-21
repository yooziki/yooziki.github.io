#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
int main() {
	//获取版本信息
	int Major, Minor, Rev;
	glfwGetVersion(&Major, &Minor, &Rev);
	printf("GLFW %d.%d.%d initialized\n", Major, Minor, Rev);

	//初始化及设置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//创建一个窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "window", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);		//设置为当前窗口

	//获取系统相关的OpenGL函数指针地址
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	//设置视口大小
	glViewport(0, 0, 800, 600);

	//注册回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.34f, 0.75f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//输入
		//processInput(window);

		//渲染指令
		//...

		//检查并且调用事件，交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	
	glfwTerminate();
	return 0;
}


//改变窗口大小时使用的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		//将窗口设置为关闭，跳出循环
		glfwSetWindowShouldClose(window, true);
	}
}