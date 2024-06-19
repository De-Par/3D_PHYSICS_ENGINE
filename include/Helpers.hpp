#pragma once

#include <string>

struct Constants
{
	static inline const std::string model_folder{ R"(C:\source\3D_PHYSICS_ENGINE\models\)" };
	static inline const std::string shader_folder{ R"(C:\source\3D_PHYSICS_ENGINE\shaders\)" };
};

namespace helpers
{
	// ������� rigidbody ������, ��� �������� ���� �� ������
	std::shared_ptr<gra::RigidBody> CreateMeshHelper(std::string filename, Shader *shader)
	{
		return std::make_shared<gra::RigidBody>((Constants::model_folder + filename).c_str(), *shader);
	}

	// ������� ������, ��� ����� ��� ����������
	Shader* CreateShaderHelper(std::string filename)
	{
		return new Shader((Constants::shader_folder + filename + ".vs").c_str(), (Constants::shader_folder + filename + ".fs").c_str());
	}
}