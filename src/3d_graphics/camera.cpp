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

Camera::Camera(const ViewFrustum& viewFrustum,const Vec3<float>& pos, const Vec3<float>& forward) : 
               viewFrustum(viewFrustum)
{
    setForward(forward);
    setPos(pos);
}

const SpaceDepValue<Vec3<float>>& Camera::getPos() const
{
    return pos;
}

void Camera::setPos(const Vec3<float>& pos)
{
    this->pos.worldSpace = pos;
    updateViewMatrix();
}

const SpaceDepValue<Vec3<float>>& Camera::getForward() const
{
    return forward;
}

void Camera::setForward(const Vec3<float>& forward)
{
    this->forward.worldSpace = normalize(forward);
    updateViewMatrix();

    updateVerticalAngle();
}

float Camera::getVerticalAngle() const
{
    return verticalAngle;
}

void Camera::setVerticalAngle(float verticalAngle)
{
    this->verticalAngle = verticalAngle;
    updateForward();
}

const Mat4<float>& Camera::getViewMatrix() const
{
    return viewMatrix;
}

const ViewFrustum& Camera::getViewFrustum() const
{
    return viewFrustum;
}

void Camera::setViewFrustum(const ViewFrustum& viewFrustum)
{
    this->viewFrustum = viewFrustum;
}

void Camera::updateForward()
{
    Vec3<float> newForward(forward.worldSpace.x, 0, forward.worldSpace.z);
    newForward.y = tanf(verticalAngle) * length(newForward);
    normalize(newForward);
    
    forward.worldSpace = newForward;
}

void Camera::updateVerticalAngle()
{
    Vec3<float> forwardXZ(forward.worldSpace.x, 0, forward.worldSpace.z);
    normalize(forwardXZ);

    verticalAngle = acosf(dotProduct(forward.worldSpace, forwardXZ));
}

void Camera::updateViewMatrix()
{
    viewMatrix = lookAt(pos.worldSpace, forward.worldSpace);
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

    point.x = frustum.left + (frustum.right - frustum.left) * pixel.x / width;
    point.y = frustum.bottom + (frustum.top - frustum.bottom) * (height - pixel.y) / height;
    point.z = frustum.near;

    return point; 
}