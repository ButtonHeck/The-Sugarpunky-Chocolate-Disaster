/*
 * Copyright 2019 Ilya Malgin
 * ModelRenderer.h
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
 * Purpose: contains declaration for ModelRenderer class
 * @version 0.1.0
 */

#pragma once

#include "ModelIndirectBufferTypes.h"

#include <GL/glew.h>

class BufferCollection;

/**
* @brief renderer for model for both onscreen and depthmap modes.
* Has two options: render only one model and instanced indirect rendering
*/
class ModelRenderer
{
public:
	ModelRenderer( BufferCollection & basicGLBuffers,
				   BufferCollection & depthmapDIBO,
				   BufferCollection & reflectionDIBO ) noexcept;
	void render( MODEL_INDIRECT_BUFFER_TYPE type, 
				 GLsizei primCount );
	void renderOneInstance( GLsizei numIndices );

private:
	BufferCollection & basicGLBuffers;
	BufferCollection & depthmapDIBO;
	BufferCollection & reflectionDIBO;
};
