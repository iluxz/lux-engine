#include "lux/Application.h"
#include "lux/Shader.h"
#include "lux/Camera.h"
#include "lux/Mesh.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace lux;

class LuxApp : public Application {
public:
    LuxApp() : Application(WindowConfig{
        "lux Engine - v0.2",
        1280, 720,
        true, true, false
    }) {}

    void onInit() override {
        LUX_INFO("lux Engine starting up...");

        m_shader = Shader::fromSource(R"(
            #version 460 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;
            layout (location = 2) in vec2 aTexCoord;
            layout (location = 3) in vec3 aColor;
            out vec3 vColor;
            out vec3 vNormal;
            out vec3 vFragPos;
            uniform mat4 uProjection;
            uniform mat4 uView;
            uniform mat4 uModel;
            void main() {
                vFragPos = vec3(uModel * vec4(aPos, 1.0));
                vNormal = mat3(transpose(inverse(uModel))) * aNormal;
                gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
                vColor = aColor;
            }
        )", R"(
            #version 460 core
            in vec3 vColor;
            in vec3 vNormal;
            in vec3 vFragPos;
            out vec4 FragColor;
            uniform vec3 uLightPos;
            uniform vec3 uLightColor;
            uniform vec3 uViewPos;
            void main() {
                vec3 norm = normalize(vNormal);
                vec3 lightDir = normalize(uLightPos - vFragPos);
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * uLightColor;
                vec3 ambient = 0.2 * uLightColor;
                vec3 result = (ambient + diffuse) * vColor;
                FragColor = vec4(result, 1.0);
            }
        )");

        m_cube = Mesh::createCube(1.0f);
        m_plane = Mesh::createPlane(5.0f, 5.0f);
        m_sphere = Mesh::createSphere(0.5f, 32);

        m_camera = Camera::perspective(45.0f, 16.0f/9.0f, 0.1f, 100.0f);
        m_camera.setPosition(glm::vec3(3.0f, 2.0f, 3.0f));
        m_camera.setTarget(glm::vec3(0.0f, 0.0f, 0.0f));

        glEnable(GL_DEPTH_TEST);
        LUX_INFO("Engine initialized - 3D scene ready");
    }

    void onUpdate(f64 dt) override {
        m_time += (f32)dt;
        // Orbit camera
        f32 radius = 5.0f;
        m_camera.setPosition(glm::vec3(
            cos(m_time * 0.5f) * radius,
            2.0f,
            sin(m_time * 0.5f) * radius
        ));
        m_camera.setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    void onRender() override {
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader->bind();
        m_shader->setMat4("uProjection", m_camera.getProjectionMatrix());
        m_shader->setMat4("uView", m_camera.getViewMatrix());
        m_shader->setVec3("uLightPos", glm::vec3(2.0f, 3.0f, 2.0f));
        m_shader->setVec3("uLightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        m_shader->setVec3("uViewPos", m_camera.getPosition());

        // Draw cube
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), m_time, glm::vec3(0.0f, 1.0f, 0.0f));
        m_shader->setMat4("uModel", model);
        m_cube.draw();

        // Draw sphere
        glm::mat4 sphereModel = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.5f, 0.0f));
        sphereModel = glm::rotate(sphereModel, m_time * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        m_shader->setMat4("uModel", sphereModel);
        m_sphere.draw();

        // Draw plane
        glm::mat4 planeModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
        m_shader->setMat4("uModel", planeModel);
        m_plane.draw();
    }

    void onShutdown() override {
        LUX_INFO("Shutting down...");
    }

private:
    std::shared_ptr<Shader> m_shader;
    Mesh m_cube, m_plane, m_sphere;
    Camera m_camera;
    f32 m_time = 0.0f;
};

int main() {
    LuxApp app;
    app.run();
    return 0;
}