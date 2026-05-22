//
// Created by matfrg on 5/21/26.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {

class MainController final : public engine::core::Controller {
    void initialize() override;

    bool loop() override;

    void begin_draw() override;

    void end_draw() override;

};

}// app

#endif //MAINCONTROLLER_HPP
