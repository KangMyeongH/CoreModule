#include "Transform.h"

namespace GameEngine
{
	std::unordered_map<int, Transform*> Transform::s_TransformMap;
	std::unordered_map<int, std::vector<Transform*>> Transform::s_ChildTransformMap;
}
