//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file camera.cpp
//! @date 2021-09-16
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "camera.h"

ViewFrustum::ViewFrustum(float left, float right, float bottom, float top, float near, float far) :
                         left(left), right(right), bottom(bottom), top(top), near(near), far(far)
{}

ViewFrustum::ViewFrustum(float verticalFOV, float aspect, float near, float far) :
                         near(near), far(far)
{
    float tanFOV = tanf(verticalFOV / 2);

    top    = tanFOV * near;
    bottom = -top;

    right  = aspect * top; 
    left   = -right; 
}

Camera::Camera(const ViewFrustum& viewFrustum, const Vec3<float>& pos, 
               float pitchVertical, float yawHorizontal) :
               m_ViewFrustum(viewFrustum)
{
    setPitchVertical(pitchVertical);
    setYawHorizontal(yawHorizontal);
    setPos(pos);
}

const SpaceDepValue<Vec3<float>>& Camera::getPos() const
{
    return m_Pos;
}

void Camera::setPos(const Vec3<float>& pos)
{
    m_Pos.worldSpace = pos;
    updateViewMatrix();
}

float Camera::getPitchVertical() const
{
    return m_PitchVertical;
}

void Camera::setPitchVertical(float pitchVertical)
{
    m_PitchVertical = pitchVertical;
    updateViewMatrix();
}

float Camera::getYawHorizontal() const
{
    return m_YawHorizontal;
}

void Camera::setYawHorizontal(float yawHorizontal)
{
    m_YawHorizontal = yawHorizontal;
    updateViewMatrix();
}

const Mat4<float>& Camera::getViewMatrix() const
{
    return m_ViewMatrix;
}

const ViewFrustum& Camera::getViewFrustum() const
{
    return m_ViewFrustum;
}

void Camera::setViewFrustum(const ViewFrustum& viewFrustum)
{
    m_ViewFrustum = viewFrustum;
}

void Camera::updateViewMatrix()
{
    static const Vec3<float> originalDirection = {1.0f, 0.0f, 0.0f};
    Mat4<float> rotationMatrix = createRotationMatrix(m_PitchVertical, m_YawHorizontal, 0);

    Vec3<float> direction = rotationMatrix * originalDirection;

    m_ViewMatrix = lookAt(m_Pos.worldSpace, direction);
}

Vec2<float> toPixel(const Vec3<float>& point,
                     float width,
                     float height,
                     const ViewFrustum& frustum)
{
    Vec2<float> pixel{};

    pixel.x = width * (point.x - frustum.left) / (frustum.right - frustum.left);
    pixel.y = height - height * (point.y - frustum.bottom) / (frustum.top - frustum.bottom);

    return pixel;
}

Vec3<float> toViewFrustumPoint(const Vec2<float>& pixel,
                               float width,
                               float height,
                               const ViewFrustum& frustum)
{
    Vec3<float> point{};

    point.x = frustum.left   + (frustum.right - frustum.left)   * pixel.x            / width;
    point.y = frustum.bottom + (frustum.top   - frustum.bottom) * (height - pixel.y) / height;
    point.z = frustum.near;

    return point; 
}