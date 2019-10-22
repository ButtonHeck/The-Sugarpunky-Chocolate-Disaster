/*
 * Copyright 2019 Ilya Malgin
 * TextManager.cpp
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
 * Purpose: contains definitions for TextManager class and GlyphVertex struct
 * @version 0.1.0
 */

#include "TextManager"
#include "Camera"
#include "ScreenResolution"
#include "Options"
#include "BenchmarkTimer"
#include "VRAM_Monitor"
#include "Shader"
#include "MouseInputManager"

#include <sstream>
#include <iomanip>

/**
* @brief plain ctor
* @param fontFile file name with font symbol data descriptions
* @param fontTexture file name of the font texture itself
* @param shader shader program for text rendering
* @param screenResolution current resolution of the screen
*/
TextManager::TextManager( const std::string & fontFile,
						  const std::string & fontTexture,
						  Shader & shader,
						  const ScreenResolution & screenResolution )
	: fontLoader( fontFile, fontTexture )
	, shader( shader )
	, screenResolution( screenResolution )
	, resolutionRelativeOffset( screenResolution.getWidthRatioToReference(), screenResolution.getHeightRatioToReference() )
	, scale( DEFAULT_SCALE.x * resolutionRelativeOffset.x, DEFAULT_SCALE.y * resolutionRelativeOffset.y )
	, basicGLBuffers( VAO | VBO )
	, bufferOffset( 0 )
	, glyphsCount( 0 )
{
	basicGLBuffers.bind( VAO | VBO );
	glEnableVertexAttribArray( 0 );
	//although we set pointer attribute as for vec4, shader program uses component layout specifier for position and texCoords
	glVertexAttribPointer( 0, GlyphVertex::NUMBER_OF_ELEMENTS, GL_FLOAT, GL_FALSE, GlyphVertex::NUMBER_OF_ELEMENTS * sizeof( GLfloat ), 0 );
	BufferCollection::bindZero( VAO | VBO );
}

/**
* @brief assembles all necessary to render text data and manages their positioning on the screen
* @param camera player's camera
* @param options set of options
* @param mouseInput mouse input manager instance
* @param sunPosition sun current position
* @param fps current FPS value
*/
void TextManager::addDebugText( const Camera & camera,
								Options & options,
								const MouseInputManager & mouseInput,
								const glm::vec3 & sunPosition,
								unsigned int fps )
{
	BENCHMARK( "TextManager: add text", true );
	float screenHeight = (float)screenResolution.getHeight();
	glm::vec3 viewPosition = camera.getPosition();
	std::stringstream ss;
	const float CROSSLINE_OFFSET_Y = 21.0f;
	const float LEFT_BORDER_OFFSET = 10.0f;
	const float UPPER_BORDER_OFFSET = 15.0f;
	const float LOWER_BORDER_OFFSET = 20.0f;
	unsigned int lineCounter = 0;

	ss << "FPS: " << fps << std::setprecision( 3 ) << std::fixed
		<< ", Frame time: " << 1000.0f / fps << " ms";
	addString( ss.str(), LEFT_BORDER_OFFSET * resolutionRelativeOffset.x, screenHeight - ( UPPER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++ ) * resolutionRelativeOffset.y, scale );

	ss.str( "" );
	ss << "Camera pos: " << std::setprecision( 3 ) << std::setw( 3 )
		<< viewPosition.x << ": " << viewPosition.y << ": " << viewPosition.z;
	addString( ss.str(), LEFT_BORDER_OFFSET * resolutionRelativeOffset.x, screenHeight - ( UPPER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++ ) * resolutionRelativeOffset.y, scale );

	ss.str( "" );
	glm::ivec2 cameraWorldCoords = camera.getWorldCoordinates();
	ss << "Camera on map: " << cameraWorldCoords.x << ": " << cameraWorldCoords.y;
	addString( ss.str(), LEFT_BORDER_OFFSET * resolutionRelativeOffset.x, screenHeight - ( UPPER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++ ) * resolutionRelativeOffset.y, scale );

	ss.str( "" );
	ss << "View dir: " << std::setprecision( 3 ) << std::setw( 3 )
		<< camera.getDirection().x << ": " << camera.getDirection().y << ": " << camera.getDirection().z;
	addString( ss.str(), LEFT_BORDER_OFFSET * resolutionRelativeOffset.x, screenHeight - ( UPPER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++ ) * resolutionRelativeOffset.y, scale );

	ss.str( "" );
	ss << "Cursor on map: ";
	if( !options[OPT_SHOW_CURSOR] )
	{
		ss << "inactive";
	}
	else
	{
		ss << std::setprecision( 3 ) << std::setw( 3 )
			<< mouseInput.getCursorWorldX() << ": " << mouseInput.getCursorWorldZ() - 1 << ", " << mouseInput.getCursorTileName();
	}
	addString( ss.str(), LEFT_BORDER_OFFSET * resolutionRelativeOffset.x, screenHeight - ( UPPER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++ ) * resolutionRelativeOffset.y, scale );

	lineCounter = 0;
	ss.str( "" );
	ss << "Water culling: " << std::boolalpha << options[OPT_WATER_CULLING];
	addString( ss.str(), LEFT_BORDER_OFFSET * resolutionRelativeOffset.x, ( LOWER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++ ) * resolutionRelativeOffset.y, scale );

	ss.str( "" );
	ss << "Hills culling: " << std::boolalpha << options[OPT_HILLS_CULLING];
	addString( ss.str(), LEFT_BORDER_OFFSET * resolutionRelativeOffset.x, ( LOWER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++ ) * resolutionRelativeOffset.y, scale );

	ss.str( "" );
	ss << "Models Phong: " << std::boolalpha << options[OPT_MODELS_PHONG_SHADING];
	addString( ss.str(), LEFT_BORDER_OFFSET * resolutionRelativeOffset.x, ( LOWER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++ ) * resolutionRelativeOffset.y, scale );

	ss.str( "" );
	ss << "Shadow cam fixed: " << std::boolalpha << options[OPT_SHADOW_CAMERA_FIXED];
	addString( ss.str(), LEFT_BORDER_OFFSET * resolutionRelativeOffset.x, ( LOWER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++ ) * resolutionRelativeOffset.y, scale );

	ss.str( "" );
	ss << "RAM available: " << std::setprecision( 6 ) << std::setw( 8 );
	if( options[OPT_SHOW_VRAM_AVAILABLE] )
	{
		VRAM_Monitor & vram = VRAM_Monitor::getInstance();
		vram.updateAvailable();
		ss << vram.getAvailableMemory() << ", " << vram.getAvailableMemoryPercent() << "%";
	}
	else
	{
		ss << "not active";
	}
	addString( ss.str(), LEFT_BORDER_OFFSET * resolutionRelativeOffset.x, ( LOWER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++ ) * resolutionRelativeOffset.y, scale );

	ss.str( "" );
	ss << "Sun position: " << std::defaultfloat << std::setprecision( 3 ) << std::setw( 3 )
		<< sunPosition.x << ": " << sunPosition.y << ": " << sunPosition.z;
	addString( ss.str(), LEFT_BORDER_OFFSET * resolutionRelativeOffset.x, ( LOWER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++ ) * resolutionRelativeOffset.y, scale );
}

/**
* @brief creates renderable data from the string and buffers it to local storage
* @param text text string to buffer
* @param x absolute X coordinate (in range [0; screenWidth] left to right) to start drawing from
* @param y absolute Y coordinate (in range [0; screenHeight] bottom to top) to start drawing from
* @param scale scale value for a glyph quad
* @todo may be more efficient to use element buffer objects for text buffers
*/
void TextManager::addString( const std::string & text, 
							 GLfloat x, 
							 GLfloat y, 
							 const glm::vec2 & scale )
{
	std::string::const_iterator characterIterator;
	std::unordered_map<char, Character> & alphabet = fontLoader.getAlphabet();
	float textureWidth = (float)fontLoader.getTextureWidth();
	float textureHeight = (float)fontLoader.getTextureHeight();

	for( characterIterator = text.begin(); characterIterator != text.end(); ++characterIterator )
	{
		//create vertices for a character's bounding box
		Character & character = alphabet[*characterIterator];
		GlyphVertex lowLeft( glm::vec2( x + character.xoffset * scale.x, y - character.yoffset * scale.y ),
							 glm::vec2( (float)character.x / textureWidth, (float)character.y / textureHeight ) );
		GlyphVertex lowRight( glm::vec2( x + ( character.width + character.xoffset ) * scale.x, y - character.yoffset * scale.y ),
							  glm::vec2( (float)( character.x + character.width ) / textureWidth, (float)character.y / textureHeight ) );
		GlyphVertex upRight( glm::vec2( x + ( character.width + character.xoffset ) * scale.x, y - ( character.height + character.yoffset ) * scale.y ),
							 glm::vec2( (float)( character.x + character.width ) / textureWidth, (float)( character.y + character.height ) / textureHeight ) );
		GlyphVertex upLeft( glm::vec2( x + character.xoffset * scale.x, y - ( character.height + character.yoffset ) * scale.y ),
							glm::vec2( (float)character.x / textureWidth, (float)( character.y + character.height ) / textureHeight ) );

		//buffer to local storage
		vertexData[bufferOffset++] = lowLeft.position.x;
		vertexData[bufferOffset++] = lowLeft.position.y;
		vertexData[bufferOffset++] = lowLeft.texCoords.x;
		vertexData[bufferOffset++] = lowLeft.texCoords.y;

		vertexData[bufferOffset++] = upLeft.position.x;
		vertexData[bufferOffset++] = upLeft.position.y;
		vertexData[bufferOffset++] = upLeft.texCoords.x;
		vertexData[bufferOffset++] = upLeft.texCoords.y;

		vertexData[bufferOffset++] = upRight.position.x;
		vertexData[bufferOffset++] = upRight.position.y;
		vertexData[bufferOffset++] = upRight.texCoords.x;
		vertexData[bufferOffset++] = upRight.texCoords.y;

		vertexData[bufferOffset++] = upRight.position.x;
		vertexData[bufferOffset++] = upRight.position.y;
		vertexData[bufferOffset++] = upRight.texCoords.x;
		vertexData[bufferOffset++] = upRight.texCoords.y;

		vertexData[bufferOffset++] = lowRight.position.x;
		vertexData[bufferOffset++] = lowRight.position.y;
		vertexData[bufferOffset++] = lowRight.texCoords.x;
		vertexData[bufferOffset++] = lowRight.texCoords.y;

		vertexData[bufferOffset++] = lowLeft.position.x;
		vertexData[bufferOffset++] = lowLeft.position.y;
		vertexData[bufferOffset++] = lowLeft.texCoords.x;
		vertexData[bufferOffset++] = lowLeft.texCoords.y;

		//update X coordinate for next character quad
		x += character.xadvance * scale.x;
	}

	//keep track on the number of characters to render from buffer
	glyphsCount += text.size();
}

/**
* @brief buffers text data to GPU and sends draw call to OpenGL with blending enabled
*/
void TextManager::drawText()
{
	BENCHMARK( "TextManager: draw", true );
	shader.use();
	basicGLBuffers.bind( VAO | VBO );
	glBufferData( GL_ARRAY_BUFFER, bufferOffset * sizeof( GLfloat ), vertexData, GL_STATIC_DRAW );

	glEnable( GL_BLEND );
	glDrawArrays( GL_TRIANGLES, 0, glyphsCount * VERTICES_PER_QUAD );
	glDisable( GL_BLEND );

	//do not forget to nullify these guys to protect from out-of-bounds indexing and overdrawing unnecessary stuff
	bufferOffset = 0;
	glyphsCount = 0;
}

/**
* @brief plain ctor
*/
TextManager::GlyphVertex::GlyphVertex( glm::vec2 pos, 
									   glm::vec2 texCoords ) noexcept
	: position{ pos.x, pos.y }
	, texCoords{ texCoords.x, texCoords.y }
{}
