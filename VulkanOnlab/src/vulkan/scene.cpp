#include "vulkan/scene.h"
#include "vulkan/application.h"
#include "GUI/mainmenu.h"

void Scene::buildScene()
{
	camera.asp = (float)Application::WIDTH / Application::HEIGHT;
	camera.fov = glm::radians(45.0f);
	camera.fp = 200.0f;
	camera.np = 0.1f;
	camera.wForward = glm::normalize(glm::vec3(0.0f)- glm::vec3(5.0f, 5.0f, 5.0f));
	camera.wVup = glm::vec3(0.0f, 1.0f, 0.0f);
	camera.wEye = glm::vec3(5.0f, 5.0f, 5.0f);
	mainMenu = new MainMenu(this);
}

void Scene::cleanup()
{
	for (Object* object : objects) {
		object->cleanup();
	}
	delete mainMenu;
}

void Scene::addObject(const char* filename, MainMenu* mainMenu)
{
	Object* obj = new Object();
	obj->create(filename);

	objects.push_back(obj);

	ImGuiObject* imObj = new ImGuiObject(obj, filename,this, mainMenu);
	mainMenu->addObject(imObj);
}

void Scene::removeObject(Object* object)
{
	for (int i = 0; i < objects.size(); i++) {
		if (*(objects[i]) == *object) {
			vkDeviceWaitIdle(Application::device);
			object->cleanup();
			delete objects[i];
			objects.erase(objects.begin() + i);
			break;
		}
	}
}

void Scene::updateUniformBuffer(uint32_t currentImage)
{
	for (Object* object : objects) {

		object->updateUniformBuffer(currentImage,camera, lights);
	}
}

void Scene::addLight()
{
	if (lights.size() < MAX_LIGHTS) {
		lights.push_back(new Light(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.2f), glm::vec3(1)));
		ImGuiLight* imLight = new ImGuiLight(lights[lights.size() - 1], this, mainMenu);
		mainMenu->addLight(imLight);
	}
}

void Scene::deleteLight(Light* light)
{
	for (int i = 0; i < lights.size(); i++) {
		if (*(lights[i]) == *light) {
			delete lights[i];
			lights.erase(lights.begin() + i);
			break;
		}
	}
}

void Scene::drawMenu()
{
	mainMenu->draw();
}

Scene::~Scene()
{
	for (int i = 0; i < objects.size();i++) {
		delete objects[i];
	}

	for (int i = 0; i < lights.size(); i++) {
		delete lights[i];
	}

	vkDestroyDescriptorSetLayout(Application::device, Object::descriptorSetLayout, nullptr);
}
