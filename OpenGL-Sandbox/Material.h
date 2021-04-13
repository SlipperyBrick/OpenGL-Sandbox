#pragma once

#include "Texture.h"

class Material
{
public:
	Material();
	Material(Texture& albedo, Texture& normal, Texture& roughness, Texture& ao, Texture& metallic);
	Material(Texture& albedo, Texture& normal, Texture& roughness, Texture& ao);
	Material(Texture& albedo, Texture& normal, Texture& roughness, Texture& ao, Texture& metallic, Texture& displacement);
    ~Material();

	void Create();

#pragma region Get/Set
	inline void SetAlbedo(Texture* albedo) { m_albedo = albedo; };
	inline Texture* GetAlbedo() { return m_albedo; };

	inline void SetNormal(Texture* normal) { m_normal = normal; };
	inline Texture* GetNormal() { return m_normal; };

	inline void SetRoughness(Texture* roughness) { m_roughness = roughness; };
	inline Texture* GetRoughness() { return m_roughness; };

	inline void SetAO(Texture* ao) { m_ao = ao; };
	inline Texture* GetAO() { return m_ao; };
	 
	inline void SetMetallic(Texture* metallic) { m_metallic = metallic; };
	inline Texture* GetMetallic() { return m_metallic; };
	 
	inline void SetDisplacement(Texture* displacement) { m_displacement = displacement; };
	inline Texture* GetDisplacement() { return m_displacement; };
#pragma endregion

    void Bind();

private:
	Texture* m_albedo;
	Texture* m_normal;
	Texture* m_roughness;
	Texture* m_ao;
	Texture* m_metallic;
	Texture* m_displacement;
};

