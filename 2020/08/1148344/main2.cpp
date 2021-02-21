#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
int main() {
	//���ô��ڴ�С
	int width = 1280;
	int height = 720;
	//��ȡ�汾��Ϣ
	int Major, Minor, Rev;
	glfwGetVersion(&Major, &Minor, &Rev);
	printf("GLFW %d.%d.%d initialized\n", Major, Minor, Rev);

	//��ʼ��������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Major); //���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Minor); //���汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //����ģʽ�������������ԣ�
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //�Ƿ���Ըı䴰�ڴ�С
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //macOS����
	//����һ������
	GLFWwindow* window = glfwCreateWindow(width, height, "window", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);		//����Ϊ��ǰ����

	//��ȡϵͳ��ص�OpenGL����ָ���ַ
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	//�����ӿڴ�С
	glViewport(0, 0, width, height);

	//ע��ص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//�����ζ�������
	const float triangle[] = {
		-0.5f,-0.5f,0.0f,	//����
		0.5f,-0.5f,0.0f,	//����
		0.0f,0.5f,0.0f		//����
	};
	//���ɲ���VBO
	GLuint vertex_buffer_object;
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
		//���������ݰ�����ǰĬ�ϻ�����
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	//���ɲ���VAO
	GLuint vertex_array_object;
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
	//���ö�������ָ�루GPU��ν������ݣ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  //���һ��������ƫ����
	glEnableVertexAttribArray(0);  //����0�����ͨ��
	//VBO��VAO�����
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	//��ɫ��
	//������ɫ��
	const char* vertex_shader_source =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main() \n"
		"{\n"
		"	gl_Position = vec4(aPos, 1.0);\n"
		"}\n\0";
	//Ƭ����ɫ��
	const char* fragment_shader_source =
		"#version 330 core\n"
		"out vec4 FragColor;\n" //�������ɫ����
		"void main()\n"
		"{\n"
		"	FragColor = vec4(0.0f,0.5f,0.2f,1.0f);\n"
		"}\n\0";
	//���붥����ɫ��
	int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	int success;
	char info_log[512];
	//�����ɫ���Ƿ�ɹ����룬�������ʧ�ܣ���ӡ������Ϣ
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		cout << "ERROR::SHADER::VERTEX::COMPLIATION_FAILED\n" << info_log << endl;
	}
	//����Ƭ����ɫ��
	int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(vertex_shader);
	//int success;
	//char info_log[512];
	//�����ɫ���Ƿ����ɹ������ʧ�ܣ���ӡ������Ϣ
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << endl;
	}
	//���ӵ���ɫ������
	int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	//�����ɫ��ʱ��ɹ����ӣ����ʧ�ܣ���ӡ������Ϣ
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << endl;
	}
	//ɾ�������Ƭ����ɫ��
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//����
		//processInput(window);

		//��Ⱦָ��
		//ʹ����ɫ��
		glUseProgram(shader_program);
		//����������
		glBindVertexArray(vertex_array_object);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		//��鲢�ҵ����¼�����������
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}


//�ı䴰�ڴ�Сʱʹ�õĻص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		//����������Ϊ�رգ�����ѭ��
		glfwSetWindowShouldClose(window, true);
	}
}