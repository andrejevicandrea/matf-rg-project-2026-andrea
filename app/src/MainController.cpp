//
// Created by matfrg on 5/21/26.
//

#include "MainController.hpp"

#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>

namespace app {
void MainController::initialize() {}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) { return false; }
    return true;
}

void MainController::begin_draw() { engine::graphics::OpenGL::clear_buffers(); }

void MainController::end_draw() { engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers(); }
}// app