//
// Created by matfrg on 5/21/26.
//

#include "MyApp.hpp"

#include <engine/core/App.hpp>
#include <spdlog/spdlog.h>

namespace app {

void ::app::MyApp::app_setup() { spdlog::info("App setup completed"); }
}// app