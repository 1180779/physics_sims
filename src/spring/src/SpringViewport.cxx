/*
 * Created by Radosław Głasek on 22.10.2026
 * radoslaw.glasek@theworldlooksback.dev
 */

#include "SpringViewport.hxx"

#include <QColor>
#include <QCoreApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QOpenGLContext>
#include <QString>

#include <filesystem>
#include <memory>
#include <utility>

#include <cad_graphics/Gl.hxx>
#include <cad_graphics/PrimitiveMeshes.hxx>

namespace {

constexpr QColor gc_viewportColor{255, 255, 255};

std::filesystem::path appPath(const std::filesystem::path& relative) {
    return std::filesystem::path(QCoreApplication::applicationDirPath().toStdString()) / relative;
}

std::optional<cadg::GpuModel> uploadPrimitive(
    cadg::Gl& gl,
    cadg::MeshData mesh,
    cadg::Material material
    ) {
    cadg::Model model;
    model.name = mesh.name;
    model.materials.push_back(std::move(material));
    model.parts.push_back(cadg::ModelPart{
        .mesh = std::make_shared<const cadg::MeshData>(std::move(mesh)),
        .materialIndex = 0,
    });

    auto gpuModel = cadg::GpuModel::upload(gl, model);
    if (!gpuModel) {
        qWarning().noquote() << "Failed to upload" << QString::fromStdString(model.name)
            << ":" << QString::fromStdString(gpuModel.error());
        return std::nullopt;
    }
    return std::move(*gpuModel);
}

} // namespace

SpringViewport::SpringViewport() {
    setFocusPolicy(Qt::StrongFocus);
}

SpringViewport::~SpringViewport() {
    if (const auto glContext = context()) {
        disconnect(glContext, &QOpenGLContext::aboutToBeDestroyed, this, nullptr);
    }
    cleanup();
}

void SpringViewport::paintGL() {
    const auto gl = cadg::getGl();
    if (!gl) {
        return;
    }

    gl->glClearColor(gc_viewportColor.redF(), gc_viewportColor.greenF(), gc_viewportColor.blueF(), 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!m_shader || (!m_box && !m_spring && !m_mass)) {
        return;
    }

    m_shader->bind(*gl);
    m_shader->setMat4(*gl, "uView", m_camera.view());
    m_shader->setMat4(*gl, "uProjection", m_camera.projection());
    m_shader->setVec3(*gl, "uCameraPos", m_camera.position());
    m_shader->setVec3(*gl, "uLightDirection", cadm::Vec3{0.4f, 0.8f, 0.5f}.normalized());
    m_shader->setVec3(*gl, "uLightColor", cadm::Vec3{1.1f, 1.08f, 1.05f});
    m_shader->setVec3(*gl, "uAmbientColor", cadm::Vec3{0.35f, 0.35f, 0.38f});

    if (m_box) {
        drawModel(*gl, *m_box, m_boxTransform);
    }
    if (m_spring) {
        drawModel(*gl, *m_spring, m_springTransform);
    }
    if (m_mass) {
        drawModel(*gl, *m_mass, m_massTransform);
    }

    cadg::ShaderProgram::unbind(*gl);
}

void SpringViewport::resizeGL(const int width, const int height) {
    if (height <= 0) {
        return;
    }
    m_camera.m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    if (const auto gl = cadg::getGl()) {
        gl->glViewport(0, 0, width, height);
    }
}

void SpringViewport::initializeGL() {
    const auto gl = cadg::getGl();
    if (!gl) {
        return;
    }

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &SpringViewport::cleanup, Qt::DirectConnection);

    gl->glEnable(GL_DEPTH_TEST);
    gl->glEnable(GL_CULL_FACE);
    gl->glCullFace(GL_BACK);
    gl->glFrontFace(GL_CCW);
    gl->glDisable(GL_BLEND);
    gl->glClearColor(gc_viewportColor.redF(), gc_viewportColor.greenF(), gc_viewportColor.blueF(), 1.0f);

    if (height() > 0) {
        m_camera.m_aspectRatio = static_cast<float>(width()) / static_cast<float>(height());
    }
    m_camera.m_radius = 5.5f;

    m_initialized = true;
    loadScene(*gl);
}

void SpringViewport::mousePressEvent(QMouseEvent* event) {
    m_lastMousePosition = event->position().toPoint();
    QOpenGLWidget::mousePressEvent(event);
}

void SpringViewport::mouseMoveEvent(QMouseEvent* event) {
    const QPoint current = event->position().toPoint();
    const QPoint delta = current - m_lastMousePosition;
    m_lastMousePosition = current;

    if (event->buttons() & Qt::LeftButton) {
        m_camera.orbit(-static_cast<float>(delta.x()) * 0.006f, -static_cast<float>(delta.y()) * 0.006f);
        update();
    } else if (event->buttons() & (Qt::RightButton | Qt::MiddleButton)) {
        m_camera.pan(static_cast<float>(delta.x()), static_cast<float>(delta.y()), height());
        update();
    }
}

void SpringViewport::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_O:
        m_camera.setOrthographic(true);
        update();
        break;
    case Qt::Key_P:
        m_camera.setOrthographic(false);
        update();
        break;
    case Qt::Key_T:
        m_camera.toggleProjection();
        update();
        break;
    default:
        QOpenGLWidget::keyPressEvent(event);
        break;
    }
}

void SpringViewport::wheelEvent(QWheelEvent* event) {
    if (const float steps = static_cast<float>(event->angleDelta().y()) / 120.0f;
        steps != 0.0f) {
        m_camera.zoom(steps);
        update();
    }
}

void SpringViewport::loadScene(cadg::Gl& gl) {
    const std::filesystem::path shaderRoot = appPath("shaders");

    auto shader = cadg::ShaderProgram::fromFiles(gl, shaderRoot / "phong.vert", shaderRoot / "phong.frag");
    if (!shader) {
        qWarning().noquote() << "Failed to create Phong shader:" << QString::fromStdString(shader.error());
        return;
    }
    m_shader.emplace(std::move(*shader));

    constexpr float boxEdge = 3.5f;

    constexpr float boxWidth = boxEdge;
    constexpr float boxDepth = boxEdge;
    constexpr float boxHeight = 0.65f;
    constexpr float springHeight = 5.0f;
    constexpr float springLength = 70.0f;
    constexpr float springRadius = 0.58f;
    constexpr float wireRadius = 0.09f;
    constexpr float massRadius = springRadius * 1.6f;
    constexpr float massHeight = std::max(springRadius * 2.7f, springHeight / 3.0f);

    cadg::Material boxMaterial;
    boxMaterial.name = "box";
    boxMaterial.diffuseColor = {0.55f, 0.33f, 0.16f};
    boxMaterial.specularColor = {0.12f, 0.10f, 0.08f};
    boxMaterial.shininess = 16.0f;

    cadg::Material springMaterial;
    springMaterial.name = "spring";
    springMaterial.diffuseColor = {0.58f, 0.58f, 0.62f};
    springMaterial.specularColor = {0.8f, 0.8f, 0.8f};
    springMaterial.shininess = 64.0f;
    springMaterial.doubleSided = true;

    cadg::Material massMaterial;
    massMaterial.name = "mass";
    massMaterial.diffuseColor = springMaterial.diffuseColor;
    massMaterial.specularColor = springMaterial.specularColor;
    massMaterial.shininess = springMaterial.shininess;

    cadg::SpringParameters springParameters;
    springParameters.height = springHeight;
    springParameters.length = springLength;
    springParameters.radius = springRadius;
    springParameters.wireRadius = wireRadius;
    springParameters.segmentsAlong = 1024;
    springParameters.segmentsAround = 24;

    m_box = uploadPrimitive(
        gl,
        cadg::makeBox({boxWidth, boxHeight, boxDepth}),
        std::move(boxMaterial)
        );
    m_spring = uploadPrimitive(
        gl,
        cadg::makeSpring(springParameters),
        std::move(springMaterial)
        );
    m_mass = uploadPrimitive(
        gl,
        cadg::makeCylinder(massRadius, massHeight, 32, 1),
        std::move(massMaterial)
        );

    m_boxTransform = cadm::Mat4::translation({0.0f, boxHeight * 0.5f + springHeight + massHeight, 0.0f});
    m_springTransform = cadm::Mat4::translation({0.0f, massHeight, 0.0f});
    m_massTransform = cadm::Mat4::translation({0.0f, massHeight * 0.5f, 0.0f});
    m_camera.m_target = {0.0f, boxHeight + springHeight * 0.5f, 0.0f};
}

void SpringViewport::drawModel(
    cadg::Gl& gl,
    const cadg::GpuModel& model,
    const cadm::Mat4& modelTransform
    ) const {
    if (!m_shader) {
        return;
    }

    for (const auto& [meshIndex, materialIndex, nodeTransform] : model.parts()) {
        const auto& mesh = model.mesh(meshIndex);
        const auto& material = model.material(materialIndex);

        if (material.doubleSided()) {
            gl.glDisable(GL_CULL_FACE);
        } else {
            gl.glEnable(GL_CULL_FACE);
        }

        m_shader->setMat4(gl, "uModel", modelTransform * nodeTransform);
        material.bind(gl, *m_shader);
        mesh.draw(gl);
    }

    gl.glEnable(GL_CULL_FACE);
}

void SpringViewport::cleanup() {
    if (!m_initialized) {
        return;
    }

    if (!isValid()) {
        m_initialized = false;
        return;
    }

    makeCurrent();
    if (const auto gl = cadg::getGl()) {
        if (m_box) {
            m_box->destroy(*gl);
            m_box.reset();
        }
        if (m_spring) {
            m_spring->destroy(*gl);
            m_spring.reset();
        }
        if (m_mass) {
            m_mass->destroy(*gl);
            m_mass.reset();
        }
        if (m_shader) {
            m_shader->destroy(*gl);
            m_shader.reset();
        }
    }
    doneCurrent();

    m_initialized = false;
}