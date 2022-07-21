
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"




GLfloat vertices[] =
{
	-0.5f,  -0.5f * float(sqrt(3)) / 3,    0.0f,    0.8f, 0.3f,  0.02f,   // Lower left corner
	0.5f,   -0.5f * float(sqrt(3)) / 3,    0.0f,    0.8f, 0.3f,  0.02f,   // Lower right corner
	0.0f,   0.5f * float(sqrt(3)) * 2 / 3, 0.0f,    1.0f, 0.6f,  0.32f,   // Upper corner
	-0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f,    0.9f, 0.45f, 0.17f,   // Inner left
	0.25f,  0.5f * float(sqrt(3)) / 6,     0.0f,    0.9f, 0.45f, 0.17f,   // Inner right
	0.0f,   -0.5f * float(sqrt(3)) / 3,    0.0f,     0.8f, 0.3f,  0.02f   // Inner down
};

// Indices for vertices order
GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};






int main() {
	glfwInit();


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "MyOpenGL", nullptr, nullptr);
	if (!window) {
		std::cout << "failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	gladLoadGL();
	glViewport(0, 0, 800, 800);


	Shader shaderProgram("Default.vert", "Default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));



	/*
	LinkAttrib 설명:
	주어진 VBO를 link한다.
	나머지는 vertex data를 해석하는 방법에 대한 인자이다.

	layout : 데이터 구분 0: 좌표 / 1: 색 
	numComponents : layout에 컴포넌트의 수 layout0, 1 모두 3개이다.
	type : layout의 컴포넌트의 타입
	stride : 첫 컴포넌트와 마지막 컴포넌트 사이의 바이트 수
	offset : layout이 시작하는 자리 지정
	*/
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();



	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");



	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();


	glfwDestroyWindow(window);
	glfwTerminate();




	return 0;
}