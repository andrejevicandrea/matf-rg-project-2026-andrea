#include <engine/graphics/MultisampleFramebuffer.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <glad/glad.h>

namespace engine::graphics {
void MultisampleFramebuffer::destroy() {
    if (m_framebuffer_id != 0) {
        CHECKED_GL_CALL(glDeleteFramebuffers, 1, &m_framebuffer_id);
        m_framebuffer_id = 0;
    }

    if (m_color_texture_id != 0) {
        CHECKED_GL_CALL(glDeleteTextures, 1, &m_color_texture_id);
        m_color_texture_id = 0;
    }

    if (m_depth_stencil_renderbuffer_id != 0) {
        CHECKED_GL_CALL(glDeleteRenderbuffers, 1, &m_depth_stencil_renderbuffer_id);
        m_depth_stencil_renderbuffer_id = 0;
    }
}
}// namespace engine::graphics
