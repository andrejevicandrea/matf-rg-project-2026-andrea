//
// Created by matfrg on 5/21/26.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>
#include <engine/resources/ResourcesController.hpp>

namespace app {

class MainController final : public engine::core::Controller {
    void initialize() override;

    bool loop() override;

    void begin_draw() override;

    void end_draw() override;

    void draw() override;

    unsigned m_floor_VAO = 0;
    unsigned m_floor_VBO = 0;

    engine::resources::Shader *m_basic_shader = nullptr;

};

}// app

#endif //MAINCONTROLLER_HPP
