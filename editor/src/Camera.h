#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <glm/glm.hpp>

#ifndef PI
#define PI 3.1415926536
#endif

namespace boxel {
namespace system {
namespace render
{
	class Camera
	{
		private:
			glm::vec3 m_position;
			glm::vec3 m_rotation;

			bool m_initialized;

		public:
			Camera(void);
			~Camera(void);

			glm::mat4 viewMatrix(void);

			void setRotationX(float angle) { m_rotation[0] = angle; }
			void setRotationY(float angle)	{ m_rotation[1] = angle; }
			void setRotationZ(float angle)	{ m_rotation[2] = angle; }

			//void setRotation(float x, float y, float z){m_rotation[0] = x; m_rotation[1] = y; m_rotation[2] = z;}
			//void setRotation(const glm::vec3 &rotationVector) { m_rotation = rotationVector;}

			float xRotation(void) {return m_rotation[0];}
			float yRotation(void) {return m_rotation[1];}
			float zRotation(void) {return m_rotation[2];}

			void setPosition(float x, float y, float z) {m_position[0] = x; m_position[1] = y; m_position[2] = z;}
			glm::vec3 position(void) {return m_position;}
			float xPosition(void) {return m_position[0];}
			float yPosition(void) {return m_position[1];}
			float zPosition(void) {return m_position[2];}
	};
}
}
}

using namespace boxel::system::render;

#endif //#ifndef _CAMERA_H_
