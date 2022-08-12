#include <string>
#pragma once

#include <iostream>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);

	unsigned int ID;

	void use();

	void setVec3(const std::string& name, glm::vec3 v);
	void setMat4(const std::string& name, glm::mat4 m);
};