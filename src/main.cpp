#include "lux/Application.h"
#include "lux/Shader.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace lux;

class LuxApp : public Application {
public:
    LuxApp() : Application(WindowConfig{
        "Lux Engine - v0.1",
        1280, 720,
        true, true, false
    }) {}

    void onInit() override {
        LUX_INFO("Lux Engine starting up...");

        // Create shader
        m_shader = Shader::fromSource(R"(
            #version 460 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aColor;
            out vec3 vColor;
            uniform mat4 uProjection;
            uniform mat4 uView;
            uniform mat4 uModel;
            void main() {
                gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
                vColor = aColor;
            }
        )", R"(
            #version 460 core
            in vec3 vColor;
            out vec4 FragColor;
            void main() {
                FragColor = vec4(vColor, 1.0);
            }
        )");

        // Triangle vertices
        f32 vertices[] = {
            // positions          // colors
             0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
             0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
        };

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)(3 * sizeof(f32)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        LUX_INFO("Engine initialized successfully");
    }

    void onUpdate(f64 dt) override {
        // Rotate triangle
        m_rotation += (f32)dt * 1.0f;
    }

    void onRender() override {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader->bind();

        glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));

        m_shader->setMat4("uProjection", projection);
        m_shader->setMat4("uView", view);
        m_shader->setMat4("uModel", model);

        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

    void onShutdown() override {
        LUX_INFO("Shutting down...");
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
    }

private:
    std::shared_ptr<Shader> m_shader;
    u32 m_vao = 0;
    u32 m_vbo = 0;
    f32 m_rotation = 0.0f;
};

int main() {
    LuxApp app;
    app.run();
    return 0;
}