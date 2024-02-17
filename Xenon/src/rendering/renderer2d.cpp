#include "renderer2d.h"
#include "../dataStructures.h"

#include"renderer2d.h"
#include<glad.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// StaticLayer2D
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Xenon::StaticLayer2D::StaticLayer2D()
{
	
}

Xenon::StaticLayer2D::~StaticLayer2D()
{

}

void Xenon::StaticLayer2D::addQuad(glm::vec2 position, glm::vec2 scale, float rotation, Xenon::Color color, Xenon::Texture* texture)
{
	m_quadList.push_back({ position, scale, rotation, color, texture });
	if (!m_testureList.contains(texture))
	{ m_testureList[texture] = m_textureCounter++; }
}
void Xenon::StaticLayer2D::addLine(glm::vec2 pos0, glm::vec2 pos1, Xenon::Color color) 
{
	m_lineList.push_back({ pos0, pos1, color });
}
void Xenon::StaticLayer2D::compute()
{

}