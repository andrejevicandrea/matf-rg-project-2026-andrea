//
// Created by matfrg on 5/21/26.
//

#include "MyApp.hpp"
#include "MainController.hpp"

#include <engine/core/App.hpp>
#include <engine/core/Controller.hpp>
#include <spdlog/spdlog.h>

namespace app {

void ::app::MyApp::app_setup() {
    spdlog::info("App setup completed");
    auto main_controller = register_controller<app::MainController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
}
}// namespace app