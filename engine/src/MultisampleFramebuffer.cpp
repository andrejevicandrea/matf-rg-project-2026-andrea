#include <engine/graphics/OpenGL.hpp>
#include <engine/graphics/MultisampleFramebuffer.hpp>
#include <glad/glad.h>

namespace engine::graphics {
void MultisampleFramebuffer::destroy() {
    CHECKED_GL_CALL(glDeleteFramebuffers, 1, &m_framebuffer_id);
    CHECKED_GL_CALL(glDeleteTextures, 1, &m_color_texture_id);
    CHECKED_GL_CALL(glDeleteRenderbuffers, 1, &m_depth_stencil_renderbuffer_id);
}

}