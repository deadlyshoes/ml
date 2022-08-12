#include "render.h"
#include "Shader.h"

const float SRC_WIDTH = 800.0f;
const float SRC_HEIGHT = 600.0f;

Render::Render()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "ML", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
}

void Render::draw(std::vector<Layer> layers) {
	Shader nodeShader("shaders/node.vert", "shaders/node.frag");

	unsigned int VBO, EBO, VAO;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	// if x is smaller, then y should be changed
	const float xRadius = std::min(1.0f, SRC_HEIGHT / SRC_WIDTH);
	const float yRadius = std::min(1.0f, SRC_WIDTH / SRC_HEIGHT);

	unsigned int SEGMENTS = 64;
	float PI = 3.14159265359f;
	std::vector<float> positions;
	std::vector<unsigned int> indices;
	positions.push_back(0.0f);
	positions.push_back(0.0f);
	positions.push_back(0.0f);
	for (int i = 0; i <= SEGMENTS; i++) {
		float segment = (float)i / (float)SEGMENTS;
		float xPos = xRadius * std::cos(segment * PI);
		float yPos = yRadius * std::sin(segment * PI);

		positions.push_back(xPos);
		positions.push_back(yPos);
		positions.push_back(0.0f);

		positions.push_back(xPos);
		positions.push_back(-yPos);
		positions.push_back(0.0f);
	}

	for (unsigned int i = 1; i <= 2 * SEGMENTS; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 2);

		//std::cout << positions[0].x << " {" << positions[i].x << ", " << positions[i].y << "} " << positions[i + 1].x << ", " << positions[i + 1].y << "}" << std::endl;
	}

	/*for (auto i : indices) {
		std::cout << i << std::endl;
	}*/

	//std::cout << positions.size() << " " << indices.size() << std::endl;
	//std::reverse(positions.begin(), positions.end());

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	unsigned int edgeVAO, edgeVBO;
	glGenVertexArrays(1, &edgeVAO);
	glGenBuffers(1, &edgeVBO);

	glBindVertexArray(edgeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, edgeVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Popup popup(0, 0, 300, 150, "content");

    Text textRenderer = Text();

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		nodeShader.use();
		glBindVertexArray(VAO);

		std::vector<std::vector<std::pair<float, float>>> nodesCenter;

		const int nLayers = layers.size();
		float layerStride = 2.0f / (float)nLayers;
		//std::cout << nLayers << " " << layerStride << std::endl;
		float nodeScale = 1.0f / (2 * nLayers);
		for (int i = 0; i < nLayers; i++) {
			const int nNodes = layers[i].nodes.size();
			float nodeStride = 2.0f / nNodes;
			nodesCenter.push_back(std::vector<std::pair<float, float>>());
			for (int j = 0; j < nNodes; j++) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-1.0f + layerStride / 2.0f, 0.0f, 0.0f));
				model = glm::translate(model, glm::vec3(i * layerStride, 0.0f, 0.0f));
				model = glm::translate(model, glm::vec3(0.0f, -yRadius + nodeStride / 2.0f, 0.0f));
				model = glm::translate(model, glm::vec3(0.0f, j * nodeStride, 0.0f));
				nodesCenter.back().push_back({-1.0f + layerStride / 2.0f + i * layerStride, -1.0f + nodeStride / 2.0f + j * nodeStride});
				model = glm::scale(model, glm::vec3(nodeScale));
				nodeShader.setMat4("model", model);
				glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, (void*)0);
			}
		}
		for (int i = 0; i < nLayers - 1; i++) {
			for (int j = 0; j < layers[i].nodes.size(); j++) {
				for (int k = 0; k < layers[i+1].nodes.size(); k++) {
					std::pair<float, float> A = nodesCenter[i][j];
					std::pair<float, float> B = nodesCenter[i+1][k];

					std::vector<float> center(6);
					center[0] = A.first;
					center[1] = A.second;
					center[2] = 0;
					center[3] = B.first;
					center[4] = B.second;
					center[5] = 0;

					glm::mat4 model = glm::mat4(1.0f);
					nodeShader.setMat4("model", model);

					glBindVertexArray(edgeVAO);
					glBindBuffer(GL_ARRAY_BUFFER, edgeVBO);
					glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(float), &center[0]);
					glDrawArrays(GL_LINES, 0, 2);
				}
			}
		}

		for (int i = 0; i < nLayers; i++) {
			for (int j = 0; j < layers[i].nodes.size(); j++) {
				const std::string text = std::to_string(layers[i].nodes[j].output);

				float x = (nodesCenter[i][j].first + 1) * SRC_WIDTH / 2;
				float y = (nodesCenter[i][j].second + 1) * SRC_HEIGHT / 2;

                textRenderer.draw(text, x, y, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));
			}
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

        popup.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}
