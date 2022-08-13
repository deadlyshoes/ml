#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

#include "nn.h"

class Render {
public:
	Render();
	void draw(std::vector<Layer> layers);
private:
	struct NodeInfo {
		bool in;
		unsigned int nLayer;
		unsigned int nNode;
	};

	GLFWwindow* window;
	float xRadius, yRadius;
	float nodeScale;
	std::vector<std::vector<std::pair<float, float>>> nodesCenter;

	void updateActiveNode(NodeInfo* nodeInfo);
};
