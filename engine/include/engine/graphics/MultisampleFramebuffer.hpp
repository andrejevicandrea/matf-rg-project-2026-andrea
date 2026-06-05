//
// Created by matfrg on 6/1/26.
//

#ifndef MULTISAMPLEFRAMEBUFFER_HPP
#define MULTISAMPLEFRAMEBUFFER_HPP

#include <cstdint>

namespace engine::graphics {
class GraphicsController;

class MultisampleFramebuffer {
    friend class GraphicsController;

public:
    void destroy();

    uint32_t id() const { return m_framebuffer_id; }

private:
    MultisampleFramebuffer() = default;

    uint32_t m_framebuffer_id{0};
    uint32_t m_color_texture_id{0};
    uint32_t m_depth_stencil_renderbuffer_id{0};

    MultisampleFramebuffer(const uint32_t framebuffer_id, const uint32_t color_texture_id, const uint32_t depth_stencil_renderbuffer_id): m_framebuffer_id(framebuffer_id)
                                                                                                                                      , m_color_texture_id(color_texture_id)
                                                                                                                                      , m_depth_stencil_renderbuffer_id(depth_stencil_renderbuffer_id) {}

};
}

#endif //MULTISAMPLEFRAMEBUFFER_HPP
