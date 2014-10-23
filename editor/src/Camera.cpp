#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp> 

Camera::Camera(void)
{
	m_position =  glm::vec3(0.f, 0.f, 0.f);//glm::vec3(-8.f, -5.f, 0.f);
	m_rotation = glm::vec3(0.f, 0.f, 0.f);
}

Camera::~Camera(void)
{
	
}

glm::mat4 Camera::viewMatrix(void)
{
	glm::mat4 view = glm::mat4(1.0f);

	view *= glm::rotate(glm::mat4(1.0f), m_rotation[0], glm::vec3(1.f, 0.f, 0.f));
	view *= glm::rotate(glm::mat4(1.0f), m_rotation[1], glm::vec3(0.f, 1.f, 0.f));
	view *= glm::rotate(glm::mat4(1.0f), m_rotation[2], glm::vec3(0.f, 0.f, 1.f));
	view *= glm::translate(glm::mat4(1.0f), m_position);

	return view;
}
