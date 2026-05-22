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

    glBindVertexArray(m_floor_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
}// app

