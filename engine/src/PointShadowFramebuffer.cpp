#include <engine/graphics/OpenGL.hpp>
#include <engine/graphics/PointShadowFramebuffer.hpp>
#include <glad/glad.h>

namespace engine::graphics {

void PointShadowFramebuffer::destroy() {
    if (m_framebuffer_id != 0) {
        CHECKED_GL_CALL(glDeleteFramebuffers, 1, &m_framebuffer_id);
        m_framebuffer_id = 0;
    }

    if (m_depth_cubemap_id != 0) {
        CHECKED_GL_CALL(glDeleteTextures, 1, &m_depth_cubemap_id);
        m_depth_cubemap_id = 0;
    }
}

void PointShadowFramebuffer::bind_depth_cubemap() const {
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE15);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, m_depth_cubemap_id);
}

}