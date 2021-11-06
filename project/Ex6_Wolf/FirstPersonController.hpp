#include <SDL_events.h>
#include "sre/Camera.hpp"

class FirstPersonController {
public:
    explicit FirstPersonController(sre::Camera * camera);

    void update(float deltaTime);
    void onKey(SDL_Event& event);
    void onMouse(SDL_Event &event);

    void setInitialPosition(glm::vec2 position, float rotation);

    float rotation = 0;
    glm::vec3 position;
    glm::vec3 lookRotation;
private:
    sre::Camera * camera;

    // TODO you can use these to record the state of the keys and mouse movement
    //  remember to initialize if you decide to use these as is
    bool fwd = false;
    bool bwd = false;
    bool left = false;
    bool right = false;
    int xrel;

    float sensitivity = 0.5f;
    float movementSpeed = 2.0f;
};

