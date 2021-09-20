//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file camera.h
//! @date 2021-09-16
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef CAMERA_H
#define CAMERA_H

#include "../core/math/_core_math.h"
#include "space_dep_value.h"

struct ViewFrustum
{
    //--------------------------------------------------------------------------
    //! @brief Construct a new View Frustum object, based on coordinates of the 
    //!        frustum's near plane.
    //! 
    //! @note Planes' coordinates are supposed to be given in camera space.
    //! 
    //! @param left
    //! @param right 
    //! @param bottom 
    //! @param top 
    //! @param near 
    //! @param far 
    //--------------------------------------------------------------------------
    ViewFrustum(float left, float right, float bottom, float top, float near, float far);

    //--------------------------------------------------------------------------
    //! @brief Construct a new View Frustum object, based on field of view and
    //!        aspect ratio.
    //! 
    //! @note Planes' coordinates are supposed to be given in camera space.
    //! 
    //! @param verticalFOV Full (not half) vertical field of view.
    //! @param aspect      Aspect ratio width/height.
    //! @param near
    //! @param far
    //--------------------------------------------------------------------------
    ViewFrustum(float verticalFOV, float aspect, float near, float far);

    float left,   right;
    float bottom, top;
    float near,   far;
};

static const Vec3<float> CAMERA_DEFAULT_POS     = {0, 0, 0};
static const Vec3<float> CAMERA_DEFAULT_FORWARD = {0, 0, 1};

class Camera
{
public:
    Camera(const ViewFrustum& viewFrustum,
           const Vec3<float>& pos     = CAMERA_DEFAULT_POS, 
           const Vec3<float>& forward = CAMERA_DEFAULT_FORWARD);

    const SpaceDepValue<Vec3<float>>& getPos() const;
    void                              setPos(const Vec3<float>& pos);

    const SpaceDepValue<Vec3<float>>& getForward() const;
    void                              setForward(const Vec3<float>& forward);

    float                             getVerticalAngle() const;
    void                              setVerticalAngle(float verticalAngle);

    const Mat4<float>&                getViewMatrix() const;

    const ViewFrustum&                getViewFrustum() const;
    void                              setViewFrustum(const ViewFrustum& viewFrustum);

private:
    /** Position in model/world/camera spaces. */
    SpaceDepValue<Vec3<float>> pos;
    
    /** Forward vector, specifying where Camera is directed. */
    SpaceDepValue<Vec3<float>> forward;

    /** 
     * The angle between forward vector and XZ plane. Needed for easier and faster computation of 
     * camera movement. 
     */
    float verticalAngle;

    /** View matrix for converting vertices from world to camera space. */
    Mat4<float> viewMatrix;

    /** Camera's view frustum */
    ViewFrustum viewFrustum;

    //--------------------------------------------------------------------------
    //! @brief Update forward vector based on verticalAngle.
    //--------------------------------------------------------------------------
    void updateForward();

    //--------------------------------------------------------------------------
    //! @brief Update verticalAngle based on forward vector.
    //--------------------------------------------------------------------------
    void updateVerticalAngle();

    //--------------------------------------------------------------------------
    //! @brief Update viewMatrix based on pos and forward vectors.
    //--------------------------------------------------------------------------
    void updateViewMatrix();
};

//------------------------------------------------------------------------------
//! @brief Convert point's coordinates from view frustum's near plane to screen
//!        coordinates.
//! 
//! @attention Origin of the screen is supposed to be at the screen's top-left 
//!            corner.
//! 
//! @param point
//! @param width
//! @param height
//! @param frustum 
//!
//! @return Point's coordinates in screen coordinates.
//------------------------------------------------------------------------------
Vec2<float> toPixel(const Vec3<float>& point,
                    float width,
                    float height,
                    const ViewFrustum& frustum);

//------------------------------------------------------------------------------
//! @brief Convert pixel's coordinates from screen coordinates to view frustum's 
//!        near plane.
//! 
//! @attention Origin of the screen is supposed to be at the screen's top-left 
//!            corner.
//! 
//! @param pixel
//! @param width
//! @param height
//! @param frustum 
//!
//! @return Point's coordinates on view frustum's near plane
//------------------------------------------------------------------------------
Vec3<float> toViewFrustumPoint(const Vec2<float>& pixel,
                               float width,
                               float height,
                               const ViewFrustum& frustum);

#endif // CAMERA_H