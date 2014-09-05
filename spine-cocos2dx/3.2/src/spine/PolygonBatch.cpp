/******************************************************************************
 * Spine Runtimes Software License
 * Version 2.1
 * 
 * Copyright (c) 2013, Esoteric Software
 * All rights reserved.
 * 
 * You are granted a perpetual, non-exclusive, non-sublicensable and
 * non-transferable license to install, execute and perform the Spine Runtimes
 * Software (the "Software") solely for internal use. Without the written
 * permission of Esoteric Software (typically granted by licensing Spine), you
 * may not (a) modify, translate, adapt or otherwise create derivative works,
 * improvements of the Software or develop new applications using the Software
 * or (b) remove, delete, alter or obscure any trademarks or any copyright,
 * trademark, patent or other intellectual property or proprietary rights
 * notices on or in the Software, including any copy thereof. Redistributions
 * in binary or source form must include this license and terms.
 * 
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include <spine/PolygonBatch.h>
#include <spine/extension.h>

USING_NS_CC;

namespace spine {

PolygonBatch* PolygonBatch::createWithCapacity (ssize_t capacity) {
	PolygonBatch* batch = new PolygonBatch();
	batch->initWithCapacity(capacity);
	batch->autorelease();
	return batch;
}

PolygonBatch::PolygonBatch () :
	_capacity(0),
	_vertices(nullptr), _verticesCount(0),
	_triangles(nullptr), _trianglesCount(0),
	_texture(nullptr), _hasSetupVBOVAO(false)
{}

bool PolygonBatch::initWithCapacity (ssize_t capacity) {
	// 32767 is max index, so 32767 / 3 - (32767 / 3 % 3) = 10920.
	CCASSERT(capacity <= 10920, "capacity cannot be > 10920");
	CCASSERT(capacity >= 0, "capacity cannot be < 0");
	_capacity = capacity;
	_vertices = MALLOC(V2F_C4B_T2F, capacity);
	_triangles = MALLOC(GLushort, capacity * 3);
	return true;
}

PolygonBatch::~PolygonBatch () {
	FREE(_vertices);
	FREE(_triangles);
}

void PolygonBatch::add (const Texture2D* addTexture,
		const float* addVertices, const float* uvs, int addVerticesCount,
		const int* addTriangles, int addTrianglesCount,
		Color4B* color) {

	if (
		addTexture != _texture
		|| _verticesCount + (addVerticesCount >> 1) > _capacity
		|| _trianglesCount + addTrianglesCount > _capacity * 3) {
		this->flush();
		_texture = addTexture;
	}

	for (int i = 0; i < addTrianglesCount; ++i, ++_trianglesCount){
		_triangles[_trianglesCount] = addTriangles[i] + _verticesCount;
    }

	for (int i = 0; i < addVerticesCount; i += 2, ++_verticesCount) {
		V2F_C4B_T2F* vertex = _vertices + _verticesCount;
		vertex->vertices.x = addVertices[i];
		vertex->vertices.y = addVertices[i + 1];
		vertex->colors = *color;
		vertex->texCoords.u = uvs[i];
		vertex->texCoords.v = uvs[i + 1];
	}
}
    
void PolygonBatch::setupVBOAndVAO(){
    if(_hasSetupVBOVAO || !useVAO())
    {
        return;
    }
    
    glGenVertexArrays(1, &_spineVAO);
    GL::bindVAO(_spineVAO);
    
    glGenBuffers(2, &_buffersVBO[0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, _buffersVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F) * _capacity, NULL, GL_DYNAMIC_DRAW);
    
    // vertices
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
    GLvoid* ptr = (GLvoid*)offsetof( V2F_C4B_T2F, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), ptr);
    
    // colors
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid*) offsetof( V2F_C4B_T2F, colors));
    
    // tex coords
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid*) offsetof( V2F_C4B_T2F, texCoords));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffersVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*_capacity * 3, NULL, GL_STATIC_DRAW);
    
    // Must unbind the VAO before changing the element buffer.
    GL::bindVAO(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    CHECK_GL_ERROR_DEBUG();
    
    _hasSetupVBOVAO = true;
}
    

void PolygonBatch::flush () {
#if USE_VBO
    flushVBO();
#else
    flushWithoutVBO();
#endif
}
    
void PolygonBatch::flushVBO() {
    if (!_verticesCount) return;
    
     GL::bindTexture2D(_texture->getName());
    
    
    if(!glIsBuffer(_buffersVBO[0]))
    {
        glGenBuffers(2, &_buffersVBO[0]);
    }
     
     //Set VBO data
     glBindBuffer(GL_ARRAY_BUFFER, _buffersVBO[0]);
     glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F) * _verticesCount, _vertices, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffersVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * _trianglesCount, _triangles, GL_DYNAMIC_DRAW);

    
    if (useVAO())
    {
        //Bind VAO
        GL::bindVAO(_spineVAO);
    }
    else
    {
        // vertices
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid*)offsetof( V2F_C4B_T2F, vertices));
        
        // colors
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid*) offsetof( V2F_C4B_T2F, colors));
        
        // tex coords
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid*) offsetof( V2F_C4B_T2F, texCoords));
    }
    
    glDrawElements(GL_TRIANGLES, _trianglesCount, GL_UNSIGNED_SHORT, (GLvoid*)0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _verticesCount);
    
    if (useVAO())
    {
        //Unbind VAO
        GL::bindVAO(0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    _verticesCount = 0;
    _trianglesCount = 0;
    
    CHECK_GL_ERROR_DEBUG();
}
    
bool PolygonBatch::useVAO()
{
#if USE_VAO
    return Configuration::getInstance()->supportsShareableVAO();
#else
    return false;
#endif
}
    
void PolygonBatch::flushWithoutVBO() {
    if (!_verticesCount) return;
    
    GL::bindTexture2D(_texture->getName());
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORDS);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), &_vertices[0].vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), &_vertices[0].colors);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), &_vertices[0].texCoords);
    
    glDrawElements(GL_TRIANGLES, _trianglesCount, GL_UNSIGNED_SHORT, _triangles);
    
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _verticesCount);
    
	_verticesCount = 0;
	_trianglesCount = 0;
    
	CHECK_GL_ERROR_DEBUG();
}


#if CULLING
//reset index if the sprite doesnt need to be drawed.
void PolygonBatch::setVerticesTrianglesCount(int verticesCount, int trianglesCount) {
    _verticesCount = verticesCount;
    _trianglesCount = trianglesCount;
}

int PolygonBatch::getVerticesCount() {
    return _verticesCount;
}

int PolygonBatch::getTrianglesCount() {
    return _trianglesCount;
}
#endif
}
