#include "Scene1.h"
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>
#include <algorithm>
#include "Colormap.h"

glm::mat3 rotationMatrixFromEulerAngles(float pitch, float roll, float yaw) {
    glm::mat4 mat = glm::toMat4(glm::quat(glm::vec3(pitch, roll, yaw)));
    return glm::mat3(mat);
}

void Scene1::onDraw(Renderer &renderer) {
    Scene::onDraw(renderer);
    glm::mat3 rotation = rotationMatrixFromEulerAngles(pitch, roll, yaw);
    glm::vec3 forward = rotation * glm::vec3(0, 0, 1);
    glm::vec3 up = rotation * glm::vec3(0, 1, 0);
    glm::vec3 right = -glm::cross(forward, up);
    renderer.drawCube(glm::vec3(0, 0, 0), glm::quat(glm::vec3(pitch, roll, yaw)), glm::vec3(1), glm::vec4(1, 0, 0, 1));
    renderer.drawLine(glm::vec3(0, 0, 0), forward, glm::vec4(1, 0, 0, 1));
    renderer.drawLine(glm::vec3(0, 0, 0), up, glm::vec4(0, 0, 1, 1));
    renderer.drawLine(glm::vec3(0, 0, 0), right, glm::vec4(0, 1, 0, 1));
    Colormap cmap("viridis");
    for (Particle &p : particles) {
        glm::vec4 color = colorByLifetime ? glm::vec4(cmap(p.lifetime / maxLifetime), 1) : p.color;
        renderer.drawSphere(p.position, 0.03f, color);
    }
}

void Scene1::simulateStep() {
    dt = ImGui::GetIO().DeltaTime;
    if (autoRotate) {
        pitch += pitch_increment;
        roll += roll_increment;
        yaw += yaw_increment;
    }
    for (Particle &p : particles) {
        p.position += p.velocity * dt;
        p.velocity += gravity * dt;
        p.lifetime += dt;
    }
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [this](const Particle &p) { return p.lifetime > maxLifetime; }),
        particles.end());

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
        ImVec2 drag = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
        if (drag.x != 0 || drag.y != 0) {
            glm::vec3 push = (drag.x * cameraRight - drag.y * cameraUp) * 0.01f;
            for (Particle &p : particles)
                p.velocity += push;
        }
    }

    if (ImGui::IsKeyDown(ImGuiKey_Space))
        launchSphere();
    if (ImGui::IsKeyDown(ImGuiKey_W))
        pitch += pitch_increment;
    if (ImGui::IsKeyDown(ImGuiKey_S))
        pitch -= pitch_increment;
    if (ImGui::IsKeyDown(ImGuiKey_A))
        roll += roll_increment;
    if (ImGui::IsKeyDown(ImGuiKey_D))
        roll -= roll_increment;
    if (ImGui::IsKeyDown(ImGuiKey_Q))
        yaw += yaw_increment;
    if (ImGui::IsKeyDown(ImGuiKey_E))
        yaw -= yaw_increment;
    lastLaunch++;
}

void Scene1::launchSphere() {
    if (lastLaunch < launch_delay)
        return;
    lastLaunch = 0;
    glm::mat3 rotation = rotationMatrixFromEulerAngles(pitch, roll, yaw);
    glm::vec3 forward = rotation * glm::vec3(0, 0, 1);
    glm::vec3 right = rotation * glm::vec3(1, 0, 0);
    glm::vec3 up = rotation * glm::vec3(0, 1, 0);

    glm::vec4 color = glm::vec4(dis(gen), dis(gen), dis(gen), 1);
    float speed = 4.5f + dis(gen);
    glm::vec3 velocity = forward * speed;
    velocity += right * (dis(gen) - 0.5f) * 2.f;
    velocity += up * (dis(gen) - 0.5f) * 2.f;

    particles.push_back(Particle{glm::vec3(0), velocity, color, 0.f});
}

void Scene1::onGUI() {
    ImGui::SliderFloat("Yaw Increment", &yaw_increment, -0.01f, 0.01f);
    ImGui::SliderFloat("Pitch Increment", &pitch_increment, -0.01f, 0.01f);
    ImGui::SliderFloat("Roll Increment", &roll_increment, -0.01f, 0.01f);
    ImGui::SliderInt("Launch Delay", &launch_delay, 0, 100);
    ImGui::Checkbox("Auto Rotate", &autoRotate);
    ImGui::Checkbox("Color by Lifetime", &colorByLifetime);
    if (ImGui::Button("Launch"))
        launchSphere();
}