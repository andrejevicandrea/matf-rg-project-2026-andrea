//
// Created by matfrg on 5/21/26.
//

#include "MainController.hpp"
#include <array>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};

    const auto textured_quad = engine::graphics::OpenGL::initialize_textured_quad(vertices, sizeof(vertices));
    m_plane_vao = textured_quad.vao;
    m_plane_vbo = textured_quad.vbo;

    //refactorinng tako da izvucem prvo kontroler resources kao graphic sto sam i onda ucitavanje teksture i shadera
    //inicijalizacija graphic, resources i platform controlera
    m_graphics_controller = engine::core::Controller::get<engine::graphics::GraphicsController>();
    m_resources_controller = engine::core::Controller::get<engine::resources::ResourcesController>();
    m_platform_controller = engine::core::Controller::get<engine::platform::PlatformController>();


    m_basic_shader = m_resources_controller->shader("basic");
    m_lighting_shader = m_resources_controller->shader("lighting_shader");
    m_point_shadow_shader = m_resources_controller->shader("point_shadow_shader");
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
    m_lamp_back_model = m_resources_controller->model("wall_lamp");
    m_lamp_left_model = m_resources_controller->model("wall_lamp");
    m_raw_burger_model = m_resources_controller->model("raw_burger");
    m_cooked_burger_model = m_resources_controller->model("cooked_burger");

    //point lights
    m_point_lights = {
            {
                    glm::vec3(3.0f, 2.0f, 0.35f),
                    glm::vec3(0.04f, 0.032f, 0.022f),
                    glm::vec3(0.75f, 0.55f, 0.35f),
                    glm::vec3(0.22f, 0.17f, 0.10f),
                    1.0f,
                    0.22f,
                    0.18f,
            },
            {
                    glm::vec3(0.35f, 2.0f, 3.0f),
                    glm::vec3(0.018f, 0.015f, 0.011f),
                    glm::vec3(0.55f, 0.42f, 0.28f),
                    glm::vec3(0.08f, 0.06f, 0.04f),
                    1.0f,
                    0.30f,
                    0.28f,
            },
    };
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) { return false; }
    return true;
}

void MainController::reset_cooking_event() {
    m_cooking_state = CookingEventState::Idle;
    m_cooking_event_elapse_time = 0.0f;
    m_lamps_intensity = 1.0f;
}

void MainController::poll_events() {
    if (m_platform_controller->key(engine::platform::KeyId::KEY_1).state() == engine::platform::Key::State::JustPressed) { m_lamps_intensity = 0.6f; }
    if (m_platform_controller->key(engine::platform::KeyId::KEY_2).state() == engine::platform::Key::State::JustPressed) { m_lamps_intensity = 1.0f; }
    if (m_platform_controller->key(engine::platform::KeyId::KEY_3).state() == engine::platform::Key::State::JustPressed) { m_lamps_intensity = 1.6f; }
    if (m_platform_controller->key(engine::platform::KeyId::KEY_0).state() == engine::platform::Key::State::JustPressed) { m_lamps_intensity = 0.0f; }
    if (m_platform_controller->key(engine::platform::KeyId::KEY_C).state() == engine::platform::Key::State::JustPressed) {
        m_cooking_state = CookingEventState::LightBoosted;
        m_lamps_intensity = 1.6f;
    }
    if (m_platform_controller->key(engine::platform::KeyId::KEY_V).state() == engine::platform::Key::State::JustPressed) { reset_cooking_event(); }
}

void MainController::setup_basic_shader() const {
    glm::mat4 view = m_graphics_controller->camera()->view_matrix();
    glm::mat4 projection = m_graphics_controller->projection_matrix();

    m_basic_shader->set_mat4("view", view);
    m_basic_shader->set_mat4("projection", projection);
}

void MainController::setup_lighting_shader() const {
    const glm::mat4 view = m_graphics_controller->camera()->view_matrix();
    const glm::mat4 projection = m_graphics_controller->projection_matrix();

    m_lighting_shader->set_mat4("view", view);
    m_lighting_shader->set_mat4("projection", projection);

    m_lighting_shader->set_vec3("viewPos", m_camera->Position);

    m_lighting_shader->set_int("material.diffuse", 0);
    m_lighting_shader->set_float("material.shininess", 32.0f);
    m_lighting_shader->set_int("depth_map", 15);
    m_lighting_shader->set_float("far_plane", m_point_shadow_far_plane);
    m_lighting_shader->set_bool("point_shadows_enabled", m_graphics_controller->point_shadow_enabled());
    if (m_graphics_controller->point_shadow_enabled()) { m_graphics_controller->bind_point_shadow_cubemap(); }

    m_lighting_shader->set_vec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    m_lighting_shader->set_vec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
    m_lighting_shader->set_vec3("dirLight.diffuse", glm::vec3(0.55f, 0.55f, 0.55f));
    m_lighting_shader->set_vec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

    //setovanje za point light
    for (std::size_t i = 0; i < m_point_lights.size(); ++i) {
        const auto &light = m_point_lights[i];
        const std::string prefix = "pointLights[" + std::to_string(i) + "]";

        m_lighting_shader->set_vec3(prefix + ".position", light.position);

        m_lighting_shader->set_vec3(prefix + ".ambient", light.ambient * m_lamps_intensity);
        m_lighting_shader->set_vec3(prefix + ".diffuse", light.diffuse * m_lamps_intensity);
        m_lighting_shader->set_vec3(prefix + ".specular", light.specular * m_lamps_intensity);

        m_lighting_shader->set_float(prefix + ".constant", light.constant);
        m_lighting_shader->set_float(prefix + ".linear", light.linear);
        m_lighting_shader->set_float(prefix + ".quadratic", light.quadratic);
    }
}

void MainController::setup_point_shadow_shader() const {
    const glm::vec3 light_position = m_point_lights[0].position;

    const float aspect = 1.0f;
    const float near_plane = 1.0f;

    const glm::mat4 shadow_projection = glm::perspective(glm::radians(90.0f), aspect, near_plane, m_point_shadow_far_plane);
    const std::array<glm::mat4, 6> shadow_transforms = {
            shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

    for (std::size_t i = 0; i < shadow_transforms.size(); ++i) { m_point_shadow_shader->set_mat4("shadow_matrices[" + std::to_string(i) + "]", shadow_transforms[i]); }
    m_point_shadow_shader->set_vec3("light_position", light_position);
    m_point_shadow_shader->set_float("far_plane", m_point_shadow_far_plane);
}

void MainController::draw_room(const engine::resources::Shader *shader) const {

    //Pod sa teksturom
    glm::mat4 floor_model = glm::mat4(1.0f);

    floor_model = glm::translate(floor_model, glm::vec3(3.0f, 0.0f, 3.0f));
    floor_model = glm::rotate(floor_model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    floor_model = glm::scale(floor_model, glm::vec3(6.0f, 6.0f, 1.0f));

    shader->set_mat4("model", floor_model);
    // bindovanje teksture
    shader->set_int("material.diffuse", 0);


    engine::graphics::OpenGL::draw_textured_quad(m_plane_vao, m_floor_texture->id(), 0);

    //zadnji zid

    glm::mat4 back_wall_model = glm::mat4(1.0f);

    back_wall_model = glm::translate(back_wall_model, glm::vec3(3.0f, 3.0f, 0.0f));
    back_wall_model = glm::scale(back_wall_model, glm::vec3(6.0f, 6.0f, 1.0f));

    shader->set_mat4("model", back_wall_model);
    shader->set_int("material.diffuse", 0);

    engine::graphics::OpenGL::draw_textured_quad(m_plane_vao, m_wall_texture->id(), 0);

    //levi zid

    glm::mat4 left_wall_model = glm::mat4(1.0f);
    left_wall_model = glm::translate(left_wall_model, glm::vec3(0.0f, 3.0f, 3.0f));
    left_wall_model = glm::rotate(left_wall_model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    left_wall_model = glm::scale(left_wall_model, glm::vec3(6.0f, 6.0f, 1.0f));

    shader->set_mat4("model", left_wall_model);
    shader->set_int("material.diffuse", 0);

    engine::graphics::OpenGL::draw_textured_quad(m_plane_vao, m_wall_texture->id(), 0);
}

void MainController::draw_kitchen(const engine::resources::Shader *shader) const { draw_model(m_kitchen_model, shader, glm::vec3(3.0f, 0.02f, 3.0f), glm::vec3(0.25f)); }

void MainController::draw_pug(const engine::resources::Shader *shader) const { if (m_cooking_state == CookingEventState::RawBurgerShown || m_cooking_state == CookingEventState::BurgerCooked) { draw_model(m_pug_model, shader, glm::vec3(5.5f, 0.02f, 3.5f), glm::vec3(0.0f, -25.0f, 0.0f), glm::vec3(0.5f)); } else if (m_cooking_state == CookingEventState::BurgerServed) { draw_model(m_pug_model, shader, glm::vec3(0.5f, 0.02f, 3.5f), glm::vec3(0.0f, 25.0f, 0.0f), glm::vec3(0.5f)); } else { draw_model(m_pug_model, shader, glm::vec3(3.0f, 0.02f, 1.5f), glm::vec3(0.5f)); } }

void MainController::draw_lamp(const engine::resources::Shader *shader) const {
    draw_model(m_lamp_back_model, shader, glm::vec3(3.0f, 2.0f, 0.0f), glm::vec3(0.02f));
    draw_model(m_lamp_left_model, shader, glm::vec3(0.0f, 2.0f, 3.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.02f));
}

void MainController::draw_burger(engine::resources::Shader *shader) const {
    if (m_cooking_state == CookingEventState::RawBurgerShown) { draw_model(m_raw_burger_model, shader, glm::vec3(5.0f, 0.3f, 3.9f), glm::vec3(0.08f, 0.08f, 0.08f)); }
    if (m_cooking_state == CookingEventState::BurgerCooked) { draw_model(m_cooked_burger_model, shader, glm::vec3(5.0f, 0.3f, 3.9f), glm::vec3(0.08f, 0.08f, 0.08f)); }
    if (m_cooking_state == CookingEventState::BurgerServed) { draw_model(m_cooked_burger_model, shader, glm::vec3(1.0f, 0.3f, 3.9f), glm::vec3(0.08f, 0.08f, 0.08f)); }
}

void MainController::draw_scene(engine::resources::Shader *shader) const {
    draw_room(shader);
    draw_kitchen(shader);
    draw_pug(shader);
    draw_lamp(shader);
    draw_burger(shader);
}

void MainController::draw_model(engine::resources::Model *model, const engine::resources::Shader *shader, const glm::vec3 &position, const glm::vec3 &scale) {
    glm::mat4 model_matrix = glm::mat4(1.0f);

    model_matrix = glm::translate(model_matrix, position);
    model_matrix = glm::scale(model_matrix, scale);

    shader->set_mat4("model", model_matrix);
    model->draw(shader);
}

void MainController::draw_model(engine::resources::Model *model, const engine::resources::Shader *shader, const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) {
    glm::mat4 model_matrix = glm::mat4(1.0f);

    model_matrix = glm::translate(model_matrix, position);
    model_matrix = glm::rotate(model_matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model_matrix = glm::scale(model_matrix, scale);

    shader->set_mat4("model", model_matrix);
    model->draw(shader);
}

void MainController::draw() {
    if (m_graphics_controller->point_shadow_enabled()) {
        m_graphics_controller->begin_point_shadow_pass();

        m_point_shadow_shader->use();
        setup_point_shadow_shader();
        draw_scene(m_point_shadow_shader);

        m_graphics_controller->end_point_shadow_pass();
    }

    m_lighting_shader->use();
    setup_lighting_shader();
    draw_scene(m_lighting_shader);
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

    if (m_cooking_state != CookingEventState::Idle) { m_cooking_event_elapse_time += dt; }

    if (m_cooking_state == CookingEventState::LightBoosted && m_cooking_event_elapse_time >= 2.0f) { m_cooking_state = CookingEventState::RawBurgerShown; }
    if (m_cooking_state == CookingEventState::RawBurgerShown && m_cooking_event_elapse_time >= 5.0f) { m_cooking_state = CookingEventState::BurgerCooked; }
    if (m_cooking_state == CookingEventState::BurgerCooked && m_cooking_event_elapse_time >= 6.5f) { m_cooking_state = CookingEventState::BurgerServed; }
}
}// namespace app
