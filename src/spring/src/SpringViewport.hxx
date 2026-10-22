/*
 * Created by Radosław Głasek on 22.10.2026
 * radoslaw.glasek@theworldlooksback.dev
 */

#ifndef PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_SPRING_VIEWPORT_HXX
#define PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_SPRING_VIEWPORT_HXX

#include <QOpenGLWidget>
#include <QPoint>

#include <optional>

#include <cad_graphics/Camera.hxx>
#include <cad_graphics/GpuModel.hxx>
#include <cad_graphics/ShaderProgram.hxx>

class QMouseEvent;
class QKeyEvent;
class QWheelEvent;

class SpringViewport final : public QOpenGLWidget {
    Q_OBJECT

public:
    SpringViewport();
    ~SpringViewport() override;

protected:
    void paintGL() override;

    void resizeGL(int width, int height) override;

    void initializeGL() override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

private:
    void loadScene(cadg::Gl& gl);

    void drawModel(cadg::Gl& gl, const cadg::GpuModel& model, const cadm::Mat4& modelTransform) const;

    void cleanup();

    cadg::Camera m_camera{};
    std::optional<cadg::ShaderProgram> m_shader;
    std::optional<cadg::GpuModel> m_box;
    std::optional<cadg::GpuModel> m_spring;
    std::optional<cadg::GpuModel> m_mass;
    cadm::Mat4 m_boxTransform = cadm::Mat4::identity();
    cadm::Mat4 m_springTransform = cadm::Mat4::identity();
    cadm::Mat4 m_massTransform = cadm::Mat4::identity();
    QPoint m_lastMousePosition{};
    bool m_initialized = false;
};

#endif // PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_SPRING_VIEWPORT_HXX