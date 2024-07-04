#pragma once
#include "vulkan/mesh/mesh.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <imnodes.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <set>
#include <optional>
#include <limits> 
#include <algorithm>
#include <fstream>
#include "vulkan/mesh/vertex.h"
#include "uniformbufferobject.h"
#include "vulkan/scene.h"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

class Application {
public:
	void run();
	const static int MAX_FRAMES_IN_FLIGHT = 2;
	const static uint32_t WIDTH = 800;
	const static uint32_t HEIGHT = 600;
	const static int MAX_OBJECTS_IN_SCENE = 1000;
	const static int MAX_TEXTURES_PER_OBJECT = 1000;

	static VkDescriptorSetLayout globalDescriptorSetLayout;
	static std::vector<VkDescriptorSet> globalDescriptorSets;
	static VkDescriptorPool descriptorPool;
	static VkDevice device;
	static VkPhysicalDevice physicalDevice;
	static VkCommandPool commandPool;
	static VkQueue graphicsQueue;
	static VkExtent2D swapChainExtent;
	static VkRenderPass renderPass;
	static VkSurfaceKHR surface;

	static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

private:
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();

	void createSurface();
	void createInstance();
	bool checkValidationLayerSupport();
	void selectPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	void createSwapChain();
	void recreateSwapChain();
	void cleanupSwapChain();
	void createImageViews();
	void createRenderPass();
	void createFrameBuffer();
	void createCommandPool();
	void createDescriptorPool();
	void createDepthResources();
	void createScene();
	void createCommandBuffer();
	void recordCommandBuffer(uint32_t imageIndex, uint32_t currentFrame);
	void createSyncObjects();
	void drawFrame();
	void initDearImgui();
	void imGuiRenders();
	void updateUniformBuffer();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);

	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void recordMousePos();
	void movement();
	bool forward = false;
	bool backwards = false;
	bool left = false;
	bool right = false;
	bool mousePressed = false;
	double lastX, lastY;

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	void createLogicalDevice();

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
	
	uint32_t currentFrame = 0;

	GLFWwindow* window;
	VkInstance instance;
	
	Scene* scene;

	VkQueue presentQueue;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	std::vector<VkImageView> swapChainImageViews;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;
	
	VkDescriptorPool g_DescriptorPool;


	void createDescriptorSetLayout();
	void createDescriptorSets();

	void createUniformBuffers();
	std::vector<VkBuffer> globalUniformBuffers;
	std::vector<VkDeviceMemory> globalUniformBuffersMemory;
	std::vector<void*> globalUniformBuffersMapped;

	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;
	bool imGuiInitialized = false;


	VkRenderPass imGuiRenderPass; 
	VkCommandPool imGuiCommandPool;
	std::vector<VkCommandBuffer> imGuiCommandBuffers;
	std::vector<VkFramebuffer> imGuiFrameBuffers;
	
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	bool framebufferResized = false;


	static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);	

	std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now();
	float time = 0;

};