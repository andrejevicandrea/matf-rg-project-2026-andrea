

#ifndef POINTSHADOWFRAMEBUFFER_HPP
#define POINTSHADOWFRAMEBUFFER_HPP

#include <cstdint>

namespace engine::graphics {
class GraphicsController;

class PointShadowFramebuffer {
    friend class GraphicsController;

public:
    void destroy();

    uint32_t id() { return m_framebuffer_id; }
    uint32_t depth_cubemap_id() const { return m_depth_cubemap_id; }

    void bind_depth_cubemap() const;

private:
    PointShadowFramebuffer() = default;

    uint32_t m_framebuffer_id{0};
    uint32_t m_depth_cubemap_id{0};

    PointShadowFramebuffer(const uint32_t framebuffer_id, const uint32_t depth_cubemap_id): m_framebuffer_id(framebuffer_id)
                                                                                        , m_depth_cubemap_id(depth_cubemap_id) {}

};
}

#endif //POINTSHADOWFRAMEBUFFER_HPP
