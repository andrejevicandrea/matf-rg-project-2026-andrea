//
// Created by matfrg on 5/21/26.
//

#include "MainController.hpp"

#include "../../engine/libs/glad/include/glad/glad.h"

#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>

namespace app {
void MainController::initialize() {
    //dodavanje poda najpre u centru ekrana kao obican beli kvadrat
    float floor_vertices[] = {-0.5f, -0.5f, 0.0f,
                              0.5f, -0.5f, 0.0f,
                              0.5f, 0.5f, 0.0f,

                              -0.5f, -0.5f, 0.0f,
                              0.5f, 0.5f, 0.0f,
                              -0.5f, 0.5f, 0.0f};
    //generisanje VAO i VBO
    glGenVertexArrays(1, &m_floor_VAO);
    glGenBuffers(1, &m_floor_VBO);

    //bindovanje
    glBindVertexArray(m_floor_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_floor_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //inicijalizacija shadera:
    m_basic_shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("basic");

    //inicijalizacija graphic controlera
    m_graphics_controller = engine::core::Controller::get<engine::graphics::GraphicsController>();

    //menjanje pozicije kamere
    auto camera = m_graphics_controller->camera();
    camera->Position = glm::vec3(0.0f, 3.0f, 5.0f);
    camera->rotate_camera(0.0f, -250.0f);
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) { return false; }
    return true;
}

void MainController::begin_draw() { engine::graphics::OpenGL::clear_buffers(); }

void MainController::end_draw() { engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers(); }

void MainController::draw() {
    m_basic_shader->use();

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(3.0f, -1.0f, -2.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(9.0f, 9.0f, 1.0f));

    m_basic_shader->set_mat4("model", model);
    m_basic_shader->set_mat4("view", m_graphics_controller->camera()->view_matrix());
    m_basic_shader->set_mat4("projection", m_graphics_controller->projection_matrix());


    glBindVertexArray(m_floor_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
}// app

