/*
 * Copyright 2019 Ilya Malgin
 * ScreenResolution.cpp
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains definitions for ScreenResolution class
 * @version 0.1.0
 */

#include "ScreenResolution"

/**
* @brief plain ctor
* @param width width of resolution
* @param height height of resolution
*/
ScreenResolution::ScreenResolution( int width, 
									int height ) noexcept
	: width( width )
	, height( height )
	, aspectRatio( (float)width / (float)height )
{}

/**
* @brief updates resolution dimensions and updates aspect ratio
* @param newWidth new width of resolution
* @param newHeight new height of resolution
*/
void ScreenResolution::updateResolution( int newWidth, 
										 int newHeight ) noexcept
{
	width = newWidth;
	height = newHeight;
	aspectRatio = (float)newWidth / (float)newHeight;
}

int ScreenResolution::getWidth() const noexcept
{
	return width;
}

int ScreenResolution::getHeight() const noexcept
{
	return height;
}

float ScreenResolution::getAspectRatio() const noexcept
{
	return aspectRatio;
}

/**
* @brief utility function needed for objects that are rendered as points whose size should be proportional
* to actual screen resolution since the point size is absolute value, e.g. on larger monitors point objects should be
* scaled according to that monitor resolution
*/
float ScreenResolution::getWidthRatioToReference() const noexcept
{
	return width / REFERENCE_WIDTH;
}

/**
* @brief utility function needed for objects that are rendered as points whose size should be proportional
* to actual screen resolution since the point size is absolute value, e.g. on larger monitors point objects should be
* scaled according to that monitor resolution
*/
float ScreenResolution::getHeightRatioToReference() const noexcept
{
	return height / REFERENCE_HEIGHT;
}
