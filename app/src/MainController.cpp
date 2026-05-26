//
// Created by matfrg on 5/21/26.
//

#include "MainController.hpp"

#include "../../engine/libs/glad/include/glad/glad.h"

#include <assimp/light.h>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>

namespace app {
void MainController::initialize() {
    //floor_vertices sam koristila i za zidove pa samo rename na vertices
    float vertices[] = {
            // pozicija + normala + tekstura koord
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 6.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 6.0f, 6.0f,

            0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 6.0f, 6.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 6.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
    };
    //generisanje VAO i VBO
    glGenVertexArrays(1, &m_plane_VAO);
    glGenBuffers(1, &m_plane_VBO);

    //bindovanje
    glBindVertexArray(m_plane_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_plane_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // pozicije
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    //normale
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // teksture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //refactorinng tako da izvucem prvo kontroler resources kao graphic sto sam i onda ucitavanje teksture i shadera
    //inicijalizacija graphic, resources i platform controlera
    m_graphics_controller = engine::core::Controller::get<engine::graphics::GraphicsController>();
    m_resources_controller = engine::core::Controller::get<engine::resources::ResourcesController>();
    m_platform_controller = engine::core::Controller::get<engine::platform::PlatformController>();


    m_basic_shader = m_resources_controller->shader("basic");
    m_floor_texture = m_resources_controller->texture("floor_texture");
    m_wall_texture = m_resources_controller->texture("wall_texture");

    //menjanje pozicije kamere
    m_camera = m_graphics_controller->camera();
    m_camera->Position = glm::vec3(3.0f, 4.0f, 9.0f);
    m_camera->rotate_camera(-115.0f, -250.0f);

    m_platform_controller->set_enable_cursor(true);
    engine::graphics::OpenGL::enable_depth_testing();

    //model
    m_kitchen_model = m_resources_controller->model("overcooked_assets");
    m_pug_model = m_resources_controller->model("overcooked_pug_character");


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

    glm::mat4 view = m_graphics_controller->camera()->view_matrix();
    glm::mat4 projection = m_graphics_controller->projection_matrix();

    m_basic_shader->set_mat4("view", view);
    m_basic_shader->set_mat4("projection", projection);

    glBindVertexArray(m_plane_VAO);

    //Pod sa teksturom
    glm::mat4 floor_model = glm::mat4(1.0f);

    floor_model = glm::translate(floor_model, glm::vec3(3.0f, 0.0f, 3.0f));
    floor_model = glm::rotate(floor_model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    floor_model = glm::scale(floor_model, glm::vec3(6.0f, 6.0f, 1.0f));

    m_basic_shader->set_mat4("model", floor_model);
    // bindovanje teksture
    m_basic_shader->set_int("texture_diffuse1", 0);
    m_floor_texture->bind(GL_TEXTURE0);


    glDrawArrays(GL_TRIANGLES, 0, 6);

    //zadnji zid

    glm::mat4 back_wall_model = glm::mat4(1.0f);

    back_wall_model = glm::translate(back_wall_model, glm::vec3(3.0f, 3.0f, 0.0f));
    back_wall_model = glm::scale(back_wall_model, glm::vec3(6.0f, 6.0f, 1.0f));

    m_basic_shader->set_mat4("model", back_wall_model);
    m_basic_shader->set_int("texture_diffuse1", 0);
    m_wall_texture->bind(GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    //levi zid

    glm::mat4 left_wall_model = glm::mat4(1.0f);
    left_wall_model = glm::translate(left_wall_model, glm::vec3(0.0f, 3.0f, 3.0f));
    left_wall_model = glm::rotate(left_wall_model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    left_wall_model = glm::scale(left_wall_model, glm::vec3(6.0f, 6.0f, 1.0f));

    m_basic_shader->set_mat4("model", left_wall_model);
    m_basic_shader->set_int("texture_diffuse1", 0);
    m_wall_texture->bind(GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLES, 0, 6);


    //model kuhinje
    glm::mat4 kitchen_model = glm::mat4(1.0f);

    kitchen_model = glm::translate(kitchen_model, glm::vec3(3.0f, 0.02f, 3.0f));
    kitchen_model = glm::scale(kitchen_model, glm::vec3(0.25f));

    m_basic_shader->set_mat4("model", kitchen_model);

    m_kitchen_model->draw(m_basic_shader);

    //model pug
    glm::mat4 pug_model = glm::mat4(1.0f);

    pug_model = glm::translate(pug_model, glm::vec3(3.0f, 0.0f, 1.5f));
    pug_model = glm::scale(pug_model, glm::vec3(0.5f));

    m_basic_shader->set_mat4("model", pug_model);

    m_pug_model->draw(m_basic_shader);


}

void MainController::update() {
    const float dt = m_platform_controller->dt();
    if (m_platform_controller->key(engine::platform::KeyId::KEY_W).is_down()) { m_camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt); }
    if (m_platform_controller->key(engine::platform::KeyId::KEY_S).is_down()) { m_camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt); }
    if (m_platform_controller->key(engine::platform::KeyId::KEY_A).is_down()) { m_camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt); }
    if (m_platform_controller->key(engine::platform::KeyId::KEY_D).is_down()) { m_camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt); }
    if (m_platform_controller->key(engine::platform::KeyId::KEY_E).is_down()) { m_camera->move_camera(engine::graphics::Camera::Movement::UP, dt); }
    if (m_platform_controller->key(engine::platform::KeyId::KEY_Q).is_down()) { m_camera->move_camera(engine::graphics::Camera::Movement::DOWN, dt); }

    const auto &mouse = m_platform_controller->mouse();

    if (mouse.scroll != 0.0f) {
        m_camera->zoom(mouse.scroll);
        m_graphics_controller->perspective_params().FOV = glm::radians(m_camera->Zoom);
    }
}
}// app

