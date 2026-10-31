#include "Scene.h"
#include <vector>
#include <random>

class Scene1 : public Scene
{
    public:
        void onDraw(Renderer &renderer) override;
        float yaw = 0;
        float pitch = 0;
        float roll = 0;
        float pitch_increment = 0.005f;
        float yaw_increment = 0.005f;
        float roll_increment = 0.005f;
        int32_t launch_delay = 8;
        int32_t lastLaunch = 0;
        void launchSphere();
        float dt = 0.01f;
        glm::vec3 gravity = glm::vec3(0, 0, -9.81f);
        float maxLifetime = 1.5f;
        bool autoRotate = true;
        bool colorByLifetime = false;
        void simulateStep() override;
        void onGUI() override;
        struct Particle {
            glm::vec3 position;
            glm::vec3 velocity;
            glm::vec4 color;
            float lifetime;
        };
        std::vector<Particle> particles;
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<float> dis;
        Scene1() : gen(rd()), dis(0.f, 1.f) {}
};