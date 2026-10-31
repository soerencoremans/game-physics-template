#include "Scene.h"
#include <imgui.h>

void Scene::onDraw(Renderer &renderer)
{
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    cameraMatrix = renderer.camera.viewMatrix;
    glm::mat4 cameraToWorld = glm::inverse(cameraMatrix);
    cameraFwd = cameraToWorld * glm::vec4(0, 0, 1, 0);
    cameraRight = cameraToWorld * glm::vec4(1, 0, 0, 0);
    cameraUp = cameraToWorld * glm::vec4(0, 1, 0, 0);
}
