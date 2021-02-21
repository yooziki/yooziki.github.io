#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
int main() {
	//设置窗口大小
	int width = 1280;
	int height = 720;
	//获取版本信息
	int Major, Minor, Rev;
	glfwGetVersion(&Major, &Minor, &Rev);
	printf("GLFW %d.%d.%d initialized\n", Major, Minor, Rev);

	//初始化及设置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Major); //主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Minor); //副版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //核心模式（无需向后兼容性）
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //是否可以改变窗口大小
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //macOS启用
	//创建一个窗口
	GLFWwindow* window = glfwCreateWindow(width, height, "window", NULL, NULL);
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
	glViewport(0, 0, width, height);

	//注册回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//三角形顶点数据
	const float triangle[] = {
		-0.5f,-0.5f,0.0f,	//左下
		0.5f,-0.5f,0.0f,	//右下
		0.0f,0.5f,0.0f		//正上
	};
	//生成并绑定VBO
	GLuint vertex_buffer_object;
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
		//将顶点数据绑定至当前默认缓冲中
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	//生成并绑定VAO
	GLuint vertex_array_object;
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
	//设置顶点属性指针（GPU如何解释数据）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  //最后一个是数据偏移量
	glEnableVertexAttribArray(0);  //开启0的这个通道
	//VBO和VAO解除绑定
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	//着色器
	//顶点着色器
	const char* vertex_shader_source =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main() \n"
		"{\n"
		"	gl_Position = vec4(aPos, 1.0);\n"
		"}\n\0";
	//片段着色器
	const char* fragment_shader_source =
		"#version 330 core\n"
		"out vec4 FragColor;\n" //输出的颜色向量
		"void main()\n"
		"{\n"
		"	FragColor = vec4(0.0f,0.5f,0.2f,1.0f);\n"
		"}\n\0";
	//编译顶点着色器
	int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	int success;
	char info_log[512];
	//检查着色器是否成功编译，如果编译失败，打印错误信息
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		cout << "ERROR::SHADER::VERTEX::COMPLIATION_FAILED\n" << info_log << endl;
	}
	//编译片段着色器
	int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(vertex_shader);
	//int success;
	//char info_log[512];
	//检查着色器是否编译成功，如果失败，打印错误信息
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << endl;
	}
	//连接到着色器程序
	int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	//检查着色器时候成功连接，如果失败，打印错误信息
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << endl;
	}
	//删除顶点和片段着色器
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//输入
		//processInput(window);

		//渲染指令
		//使用着色器
		glUseProgram(shader_program);
		//绘制三角形
		glBindVertexArray(vertex_array_object);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		//检查并且调用事件，交换缓冲
		glfwSwapBuffers(window);
		glfwPollEvents();
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