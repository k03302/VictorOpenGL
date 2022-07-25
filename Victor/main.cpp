






#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

/*
// 삼각형 vertices
GLfloat vertices[] =
{
	-0.5f,  -0.5f * float(sqrt(3)) / 3,    0.0f,    0.8f, 0.3f,  0.02f,   // Lower left corner
	0.5f,   -0.5f * float(sqrt(3)) / 3,    0.0f,    0.8f, 0.3f,  0.02f,   // Lower right corner
	0.0f,   0.5f * float(sqrt(3)) * 2 / 3, 0.0f,    1.0f, 0.6f,  0.32f,   // Upper corner
	-0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f,    0.9f, 0.45f, 0.17f,   // Inner left
	0.25f,  0.5f * float(sqrt(3)) / 6,     0.0f,    0.9f, 0.45f, 0.17f,   // Inner right
	0.0f,   -0.5f * float(sqrt(3)) / 3,    0.0f,     0.8f, 0.3f,  0.02f   // Inner down
};
*/

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

// Indices for vertices order
GLuint indices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
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


	// 셰이더 프로그램 생성
	Shader shaderProgram("Default.vert", "Default.frag");
	// 셰이더 프로그램의 scale uniform 변수 id를 가져옴
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");


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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	// 텍스쳐 생성
	Texture segu("segu256.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	segu.texUnit(shaderProgram, "tex0", 0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		// 셰이더프로그램의 uniform 변수를 변경하기 전에 셰이더프로그램을 실행해야 한다.
		glUniform1f(uniID, 0.5f);
		segu.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	segu.Delete();
	shaderProgram.Delete();


	glfwDestroyWindow(window);
	glfwTerminate();




	return 0;
}