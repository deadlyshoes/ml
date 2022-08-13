#include "render.h"
#include "Shader.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

const float SRC_WIDTH = 800.0f;
const float SRC_HEIGHT = 600.0f;

double mouseXPos, mouseYPos;
bool clicked = false;
bool ignoreMouse = false;

void Render::updateActiveNode(Render::NodeInfo* nodeInfo)
{
	for (int i = 0; i < nodesCenter.size(); i++)
	{
		for (int j = 0; j < nodesCenter[i].size(); j++)
		{
			float x = (nodesCenter[i][j].first + 1.0f) * SRC_WIDTH / 2 - mouseXPos;
			float y = (nodesCenter[i][j].second + 1.0f) * SRC_HEIGHT / 2 - mouseYPos;

			float radius = xRadius * (SRC_WIDTH / 2) * nodeScale;

			std::cout << "x: " << x << " y: " << y << std::endl;
			std::cout << "radius: " << radius << std::endl;

			if (x * x + y * y <= radius * radius)
			{
				nodeInfo->in = true;
				nodeInfo->nLayer = i;
				nodeInfo->nNode = j;
				return;
			}
		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !ignoreMouse)
	{
		glfwGetCursorPos(window, &mouseXPos, &mouseYPos);
		clicked = true;
	}
}

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

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	ImGui::CreateContext();

	ImGui::StyleColorsDark();
	
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

void Render::draw(std::vector<Layer> layers) {
	Shader nodeShader("shaders/node.vert", "shaders/node.frag");

	unsigned int VBO, EBO, VAO;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	// if x is smaller, then y should be changed
	xRadius = std::min(1.0f, SRC_HEIGHT / SRC_WIDTH);
	yRadius = std::min(1.0f, SRC_WIDTH / SRC_HEIGHT);

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
	}

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

	ImGuiIO& io = ImGui::GetIO();

	NodeInfo nodeInfo;
	nodeInfo.in = false;

	while (!glfwWindowShouldClose(window)) {
		if (io.WantCaptureMouse)
			ignoreMouse = true;
		glfwPollEvents();
		ignoreMouse = false;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Node info");
			if (!nodeInfo.in)
			{
				ImGui::Text("Click on a node");
			}
			else
			{
				const Node& node = layers[nodeInfo.nLayer].nodes[nodeInfo.nNode];
				ImGui::Text("delta: %f", node.delta);
				ImGui::Text("output: %f", node.output);
				if (ImGui::TreeNode("w"))
				{
					for (const float& x : node.w)
					{
						ImGui::Text("%f", x);
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("error gradient"))
				{
					for (const float& x : node.error_gradient)
					{
						ImGui::Text("%f", x);
					}
					ImGui::TreePop();
				}
			}

			ImGui::End();
		}

		ImGui::Render();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		nodeShader.use();
		glBindVertexArray(VAO);

		this->nodesCenter.clear();

		const int nLayers = layers.size();
		float layerStride = 2.0f / (float)nLayers;
		//std::cout << nLayers << " " << layerStride << std::endl;
		nodeScale = 1.0f / (2 * nLayers);
		for (int i = 0; i < nLayers; i++) {
			const int nNodes = layers[i].nodes.size();
			float nodeStride = 2.0f / nNodes;
			this->nodesCenter.push_back(std::vector<std::pair<float, float>>());
			for (int j = 0; j < nNodes; j++) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-1.0f + layerStride / 2.0f, 0.0f, 0.0f));
				model = glm::translate(model, glm::vec3(i * layerStride, 0.0f, 0.0f));
				model = glm::translate(model, glm::vec3(0.0f, -yRadius + nodeStride / 2.0f, 0.0f));
				model = glm::translate(model, glm::vec3(0.0f, j * nodeStride, 0.0f));
				this->nodesCenter.back().push_back({-1.0f + layerStride / 2.0f + i * layerStride, -1.0f + nodeStride / 2.0f + j * nodeStride});
				model = glm::scale(model, glm::vec3(nodeScale));
				nodeShader.setMat4("model", model);
				glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, (void*)0);
			}
		}
		for (int i = 0; i < nLayers - 1; i++) {
			for (int j = 0; j < layers[i].nodes.size(); j++) {
				for (int k = 0; k < layers[i+1].nodes.size(); k++) {
					std::pair<float, float> A = this->nodesCenter[i][j];
					std::pair<float, float> B = this->nodesCenter[i+1][k];

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

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (clicked)
		{
			updateActiveNode(&nodeInfo);
			std::cout << "layer: " << nodeInfo.nLayer << " node: " << nodeInfo.nNode << std::endl;
			std::cout << "node updated" << std::endl;
			clicked = false;
		}

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}
