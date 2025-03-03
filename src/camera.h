#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gfc_vector.h"

/**
* @brief set camera bounds
*/
void camera_set_bounds(GFC_Rect bounds);

/**
* @brief enable or disable camera boundary checks
*/
void camera_enable_binding(Bool bindCamera);

/**
* @brief get camera position relative to world space
* @return the position
*/
GFC_Vector2D  camera_get_position();

/**
* @brief get the offset to draw things relative to the camera
* @return the offset
*/
GFC_Vector2D  camera_get_offset();

/**
* @brief set camera position relative to world space
*/
void  camera_set_position(GFC_Vector2D position);

/**
* @brief set camera size
*/
void camera_set_size(GFC_Vector2D size);

/**
* @brief snap camera to bounds
*/
void camera_apply_bounds();

/**
* @brief center camera at cener
* @param Vector2d center
*/
void camera_ceneter_at(GFC_Vector2D center);

#endif 

