#include "SpriteAnimation.h"
#include "Vertex.h"
#include "GL\glew.h"

SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col)
	: Mesh(meshName)
	, m_row(row)
	, m_col(col)
	, m_currentTime(0)
	, m_currentFrame(0)
	, m_playCount(0)
{
	m_anim = NULL;
}

SpriteAnimation::~SpriteAnimation()
{
	if (m_anim)
	{
		//delete goes to location of pointer and deletes what is stored there
		delete m_anim;
		m_anim = NULL; //pointer still exists, so set it to NULL
	}
}

void SpriteAnimation::Update(double dt)
{
	if (m_anim == NULL)
		return;

	if (m_anim->animActive == false) // If animation doesn't happen
		return;

	//-----------------------If animation happens-----------------------------//

	// Get Animation current time, m_currentTime
	// Animation current time increments with dt
	m_currentTime += static_cast<float>(dt); //(float)dt

	// Find int numFrame
	int numFrame = Math::Max(1, m_anim->endFrame - m_anim->startFrame + 1);

	// Get float frameTime
	//FrameTime = animTime / numFrame
	float frameTime = m_anim->animTime / numFrame;

	// Get Animation current frame, m_currentFrame.
	m_currentFrame = Math::Min(m_anim->endFrame, 
						m_anim->startFrame + static_cast<int>(m_currentTime / frameTime));

	// Check if m_currentTime is greater than or equal animTime
	if (m_currentTime >= m_anim->animTime)
	{
		if (m_anim->repeatCount == 0)
		{
			m_anim->animActive = false;
			m_currentTime = 0.0f;
			m_currentFrame = m_anim->startFrame;
			m_anim->ended = true;
		}

		if (m_anim->repeatCount == 1)
		{
			m_currentTime = 0.0f;
			m_currentFrame = m_anim->startFrame;
		}
	}
}

void SpriteAnimation::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if (mode == DRAW_LINES)
		glDrawElements(GL_LINES, indexSize, GL_UNSIGNED_INT, 
						(void*)(m_currentFrame * 6 * sizeof(GLint))); //6 vertices for 2 triangles
	else if (mode == DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT,
						(void*)(m_currentFrame * 6 * sizeof(GLint)));
	else
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT,
						(void*)(m_currentFrame * 6 * sizeof(GLint)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}