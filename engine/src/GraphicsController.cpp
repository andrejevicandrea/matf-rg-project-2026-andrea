
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/Skybox.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <engine/util/Configuration.hpp>

namespace engine::graphics {

void GraphicsController::begin_draw() {
    if (m_multisample_enabled) { OpenGL::bind_framebuffer(m_multisample_framebuffer.id()); } else { OpenGL::bind_framebuffer(0); }
    OpenGL::clear_buffers();
}

void GraphicsController::end_draw() {
    auto platform = engine::core::Controller::get<platform::PlatformController>();

    if (m_multisample_enabled) { OpenGL::resolve_framebuffer(m_multisample_framebuffer.id(), platform->window()->width(), platform->window()->height()); }

    platform->swap_buffers();
}

void GraphicsController::resize_multisample_framebuffer(int width, int height) {
    if (!m_multisample_enabled || width <= 0 || height <= 0) { return; }

    m_multisample_framebuffer.destroy();

    OpenGL::initialize_multisample_framebuffer(m_multisample_framebuffer.m_framebuffer_id, m_multisample_framebuffer.m_color_texture_id, m_multisample_framebuffer.m_depth_stencil_renderbuffer_id, width, height, m_multisample_samples);

}

void GraphicsController::initialize() {
    const int opengl_initialized = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    RG_GUARANTEE(opengl_initialized, "OpenGL failed to init!");

    const auto &config = engine::util::Configuration::config();

    if (config.contains("anti_aliasing")) {
        const auto &anti_aliasing = config["anti_aliasing"];
        if (anti_aliasing.contains("enabled")) { m_multisample_enabled = anti_aliasing["enabled"].get<bool>(); }
        if (anti_aliasing.contains("samples")) { m_multisample_samples = anti_aliasing["samples"].get<int32_t>(); }
    }

    if (config.contains("point_shadows")) {
        const auto &point_shadows = config["point_shadows"];
        if (point_shadows.contains("enabled")) { m_point_shadow_enabled = point_shadows["enabled"].get<bool>(); }
        if (point_shadows.contains("resolution")) { m_point_shadow_resolution = point_shadows["resolution"].get<int32_t>(); }
    }

    auto platform = engine::core::Controller::get<platform::PlatformController>();
    auto handle = platform->window()->handle_();
    m_perspective_params.FOV = glm::radians(m_camera.Zoom);
    m_perspective_params.Width = static_cast<float>(platform->window()->width());
    m_perspective_params.Height = static_cast<float>(platform->window()->height());
    m_perspective_params.Near = 0.1f;
    m_perspective_params.Far = 100.f;
    m_ortho_params.Bottom = 0.0f;
    m_ortho_params.Top = static_cast<float>(platform->window()->height());
    m_ortho_params.Left = 0.0f;
    m_ortho_params.Right = static_cast<float>(platform->window()->width());
    m_ortho_params.Near = 0.1f;
    m_ortho_params.Far = 100.0f;

    platform->register_platform_event_observer(std::make_unique<GraphicsPlatformEventObserver>(this));
    CHECKED_GL_CALL(glViewport, 0, 0, platform->window()->width(), platform->window()->height());
    //OpenGL::initialize_multisample_framebuffer(m_multisample_framebuffer.m_framebuffer_id, m_multisample_framebuffer.m_color_texture_id, m_multisample_framebuffer.m_depth_stencil_renderbuffer_id, platform->window()->width(), platform->window()->height(), 4);
    if (m_multisample_enabled) { resize_multisample_framebuffer(platform->window()->width(), platform->window()->height()); }
    if (m_point_shadow_enabled) { OpenGL::initialize_point_shadow_framebuffer(m_point_shadow_framebuffer.m_framebuffer_id, m_point_shadow_framebuffer.m_depth_cubemap_id, m_point_shadow_resolution, m_point_shadow_resolution); }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    RG_GUARANTEE(ImGui_ImplGlfw_InitForOpenGL(handle, true), "ImGUI failed to initialize for OpenGL");
    RG_GUARANTEE(ImGui_ImplOpenGL3_Init("#version 330 core"), "ImGUI failed to initialize for OpenGL");
}

void GraphicsController::terminate() {
    m_multisample_framebuffer.destroy();
    if (ImGui::GetCurrentContext()) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}

void GraphicsPlatformEventObserver::on_window_resize(int width, int height) {
    if (width <= 0 || height <= 0) { return; }


    m_graphics->perspective_params().Width = static_cast<float>(width);
    m_graphics->perspective_params().Height = static_cast<float>(height);
    m_graphics->orthographic_params().Right = static_cast<float>(width);
    m_graphics->orthographic_params().Top = static_cast<float>(height);
    CHECKED_GL_CALL(glViewport, 0, 0, width, height);

    m_graphics->resize_multisample_framebuffer(width, height);
}

std::string_view GraphicsController::name() const { return "GraphicsController"; }

void GraphicsController::begin_gui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GraphicsController::end_gui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GraphicsController::draw_skybox(const resources::Shader *shader, const resources::Skybox *skybox) {
    glm::mat4 view = glm::mat4(glm::mat3(m_camera.view_matrix()));
    shader->use();
    shader->set_mat4("view", view);
    shader->set_mat4("projection", projection_matrix<>());
    CHECKED_GL_CALL(glDepthFunc, GL_LEQUAL);
    CHECKED_GL_CALL(glBindVertexArray, skybox->vao());
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, skybox->texture());
    CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLES, 0, 36);
    CHECKED_GL_CALL(glBindVertexArray, 0);
    CHECKED_GL_CALL(glDepthFunc, GL_LESS);// set depth function back to default
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, 0);
}
}// namespace engine::graphics
