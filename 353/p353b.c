#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint load_shaders(const char *vshader_path, const char *fshader_path){
	GLuint vshaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint res = GL_FALSE;
	int log_len = 0;
	FILE *shader_file = fopen(vshader_path, "r");

	if(!shader_file){
		fprintf(stderr, "\e[1;31mERROR: Could not open vertex shader file \"%s\"!\e[0m\n", vshader_path);
		return 0;
	}
	fseek(shader_file, 0, SEEK_END);
	long file_size = ftell(shader_file);
	fseek(shader_file, 0, SEEK_SET);
	char *text_buf = malloc(file_size + 1), *tmp;
	if(!text_buf){
		fprintf(stderr, "\e[1;31mERROR: Could not allocate buffer!\e[0m\n");
		return 0;
	}
	fread(text_buf, 1, file_size, shader_file);
	text_buf[file_size] = 0;
	fclose(shader_file);
	glShaderSource(vshaderID, 1, (const char**)&text_buf, NULL);
	glCompileShader(vshaderID);
	glGetShaderiv(vshaderID, GL_COMPILE_STATUS, &res);
	glGetShaderiv(vshaderID, GL_INFO_LOG_LENGTH, &log_len);
	if(log_len > 0){
		if(log_len > file_size){
			tmp = realloc(text_buf, log_len + 1);
			if(!tmp){
				fprintf(stderr, "\e[1;31mERROR: Could not allocate buffer!\e[0m\n");
				free(text_buf);
				return 0;
			}
			text_buf = tmp;
		}
		fprintf(stderr, "\e[1;33mOpenGL info log:\n%s\e[0m\n", text_buf);
	}
	if(res == GL_FALSE){
		fprintf(stderr, "\e[1;31mERROR: Could not compile vertex shader!\e[0m\n");
		free(text_buf);
		return 0;
	}

	shader_file = fopen(fshader_path, "r");
	if(!shader_file){
		fprintf(stderr, "\e[1;31mERROR: Could not open fragment shader file \"%s\"!\e[0m\n", fshader_path);
	}
	fseek(shader_file, 0, SEEK_END);
	file_size = ftell(shader_file);
	fseek(shader_file, 0, SEEK_SET);
	if(!(tmp = realloc(text_buf, file_size + 1))){
		fprintf(stderr, "\e[1;31mERROR: Could not allocate buffer!\e[0m\n");
		free(text_buf);
		return 0;
	}
	text_buf = tmp;
	fread(text_buf, 1, file_size, shader_file);
	text_buf[file_size] = 0;
	fclose(shader_file);
	glShaderSource(fshaderID, 1, (const char**)&text_buf, NULL);
	glCompileShader(fshaderID);
	glGetShaderiv(fshaderID, GL_COMPILE_STATUS, &res);
	glGetShaderiv(fshaderID, GL_INFO_LOG_LENGTH, &log_len);
	if(log_len > 0){
		if(log_len > file_size){
			tmp = realloc(text_buf, log_len + 1);
			if(!tmp){
				fprintf(stderr, "\e[1;31mERROR: Could not allocate buffer!\e[0m\n");
				free(text_buf);
				return 0;
			}
			text_buf = tmp;
		}
		fprintf(stderr, "\e[1;33mOpenGL info log:\n%s\e[0m\n", text_buf);
	}
	if(res == GL_FALSE){
		fprintf(stderr, "\e[1;31mERROR: Could not compile fragment shader!\e[0m\n");
		free(text_buf);
		return 0;
	}

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vshaderID);
	glAttachShader(programID, fshaderID);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &res);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &log_len);
	if(log_len > 0){
		if(log_len > file_size){
			tmp = realloc(text_buf, log_len + 1);
			if(!tmp){
				fprintf(stderr, "\e[1;31mERROR: Could not allocate buffer!\e[0m\n");
				free(text_buf);
				return 0;
			}
			text_buf = tmp;
		}
		fprintf(stderr, "\e[1;33mOpenGL info log:\n%s\e[0m\n", text_buf);
	}
	if(res == GL_FALSE){
		fprintf(stderr, "\e[1;31mERROR: Could not link shaders!\e[0m\n");
		free(text_buf);
		return 0;
	}

	glDetachShader(programID, vshaderID);
	glDetachShader(programID, fshaderID);

	glDeleteShader(vshaderID);
	glDeleteShader(fshaderID);

	return programID;
}

int main(){
	glewExperimental = true;
	if(!glfwInit()){
		const char *desc;
		glfwGetError(&desc);
		fprintf(stderr, "\e[1;31mERROR: Could not initialize GLFW: %s!\e[0m\n", desc);
		return EXIT_FAILURE;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);// antialising
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(1024, 768, "Risky Moon", NULL, NULL);
	if(!window){
		const char *desc;
		glfwGetError(&desc);
		fprintf(stderr, "\e[1;31mERROR: Could not open window: %s!\e[0m\n", desc);
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	if(glewInit() != GLEW_OK){
		fprintf(stderr, "\e[1;31mERROR: Could not initialize GLEW!\e[0m\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	GLuint programID = load_shaders("vertex.glsl", "fragment.glsl");

	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	static const GLfloat vbuf_data[] = {
		-1, -1, 0,
		1, -1, 0,
		0, 1, 0
	};

	GLuint vbufID;
	glGenBuffers(1, &vbufID);
	glBindBuffer(GL_ARRAY_BUFFER, vbufID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbuf_data), vbuf_data, GL_STATIC_DRAW);

	glClearColor(0, 0, .4, 0);

	while(!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbufID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

