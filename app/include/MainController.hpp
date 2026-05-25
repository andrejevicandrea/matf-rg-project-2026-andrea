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

class MainController final : public engine::core::Controller {
    void initialize() override;

    bool loop() override;

    void begin_draw() override;

    void end_draw() override;

    void draw() override;

    void update() override;

    unsigned m_plane_VAO = 0;
    unsigned m_plane_VBO = 0;

    bool m_camera_mouse_enabled = false;

    engine::graphics::GraphicsController *m_graphics_controller = nullptr;
    engine::resources::ResourcesController *m_resources_controller = nullptr;
    engine::platform::PlatformController *m_platform_controller = nullptr;

    engine::resources::Shader *m_basic_shader = nullptr;
    engine::resources::Texture *m_floor_texture = nullptr;
    engine::resources::Texture *m_wall_texture = nullptr;
    engine::graphics::Camera *m_camera = nullptr;
    engine::resources::Model *m_kitchen_model = nullptr;

};

}// app

#endif //MAINCONTROLLER_HPP
