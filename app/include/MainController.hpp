//
// Created by matfrg on 5/21/26.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <engine/resources/Texture.hpp>
#include <engine/resources/Shader.hpp>
#include <engine/graphics/Camera.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/Model.hpp>

namespace app {

enum class CookingEventState {
    Idle,
    LightBoosted
};

class MainController final : public engine::core::Controller {
    void initialize() override;

    bool loop() override;

    void begin_draw() override;

    void end_draw() override;

    void poll_events() override;

    void setup_basic_shader() const;

    void draw_room(const engine::resources::Shader *shader) const;

    void draw_kitchen(const engine::resources::Shader *shader) const;

    void draw_pug(const engine::resources::Shader *shader) const;

    void draw_lamp(const engine::resources::Shader *shader) const;

    static void draw_model(
            engine::resources::Model *model,
            const engine::resources::Shader *shader,
            const glm::vec3 &position,
            const glm::vec3 &scale
            );

    static void draw_model(
            engine::resources::Model *model,
            const engine::resources::Shader *shader,
            const glm::vec3 &position,
            const glm::vec3 &rotation,
            const glm::vec3 &scale
            );

    void setup_lighting_shader() const;

    void draw_burger(engine::resources::Shader *shader) const;

    void draw() override;

    void update() override;

    struct PointLight {
        glm::vec3 position;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;
    };

    std::vector<PointLight> m_point_lights;

    unsigned m_plane_VAO = 0;
    unsigned m_plane_VBO = 0;

    bool m_camera_mouse_enabled = false;
    float m_lamps_intensity = 1.0f;

    engine::graphics::GraphicsController *m_graphics_controller = nullptr;
    engine::resources::ResourcesController *m_resources_controller = nullptr;
    engine::platform::PlatformController *m_platform_controller = nullptr;

    engine::resources::Shader *m_basic_shader = nullptr;
    engine::resources::Shader *m_lighting_shader = nullptr;
    engine::resources::Texture *m_floor_texture = nullptr;
    engine::resources::Texture *m_wall_texture = nullptr;
    engine::graphics::Camera *m_camera = nullptr;
    engine::resources::Model *m_kitchen_model = nullptr;
    engine::resources::Model *m_pug_model = nullptr;
    engine::resources::Model *m_lamp_back_model = nullptr;
    engine::resources::Model *m_lamp_left_model = nullptr;
    engine::resources::Model *m_raw_burger_model = nullptr;
    engine::resources::Model *m_cooked_burger_model = nullptr;

    CookingEventState m_cooking_state = CookingEventState::Idle;
};

}// app

#endif //MAINCONTROLLER_HPP
