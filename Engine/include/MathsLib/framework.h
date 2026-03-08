#pragma once
/**
 * @file framework.h
 * @brief Windows-specific configuration header for the MathsLib module.
 *
 * Excludes rarely-used components from Windows headers to reduce compile time.
 * This file should be included before any Windows headers in the MathsLib.
 */

/// @brief Exclude rarely-used stuff from Windows headers to speed up compilation.
#define WIN32_LEAN_AND_MEAN
