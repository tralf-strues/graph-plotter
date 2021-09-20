//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file space_dep_value.h
//! @date 2021-09-18
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef SPACE_DEP_VALUE_H
#define SPACE_DEP_VALUE_H

//------------------------------------------------------------------------------
//! @brief Space-dependent value, i.e. value that varies between model and 
//!        camera spaces.
//! 
//! Some values (commonly related to shape of an entity) change when 
//! transforming the entity from model space to world space and to camera space. 
//! This is exactly when SpaceDepValue should be used. 
//! 
//! @tparam T Type of value.
//------------------------------------------------------------------------------
template<typename T>
struct SpaceDepValue
{
    SpaceDepValue() = default;

    T modelSpace;
    T worldSpace;
    T cameraSpace;
};

class Camera;

class SpaceDependent
{
public:
    //--------------------------------------------------------------------------
    //! @brief Convert all space-dependent values to world space. 
    //--------------------------------------------------------------------------
    virtual void toWorldSpace() = 0;

    //--------------------------------------------------------------------------
    //! @brief Convert all space-dependent values to camera space. 
    //! @warning Supposed to be called after @ref toWorldSpace().
    //--------------------------------------------------------------------------
    virtual void toCameraSpace(const Camera& camera) = 0;
};

#endif // SPACE_DEP_VALUE_H