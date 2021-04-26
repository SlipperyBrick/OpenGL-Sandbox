#pragma once

#include "Texture.h"

class Material
{
public:
	Material();
	Material(Texture& albedo);
	Material(glm::vec3 albedo, Texture& normal, Texture& roughness, Texture& ao, Texture& metallic);
	Material(Texture& albedo, Texture& normal, Texture& roughness, Texture& ao, Texture& metallic);
	Material(Texture& albedo, Texture& normal, Texture& roughness, Texture& ao, float metallic);
	Material(Texture& albedo, Texture& normal, Texture& roughness, Texture& ao, Texture& metallic, Texture& displacement);
    ~Material();

#pragma region Get/Set
	inline void SetAlbedo(Texture* albedo) {  m_albedo = albedo; };
	inline void SetAlbedo(glm::vec3 albedo) { m_albedoColour = albedo; };
	inline Texture* GetAlbedo() const { return m_albedo; };

	inline void SetNormal(Texture* normal) { m_normal = normal; };
	inline Texture* GetNormal() const { return m_normal; };

	inline void SetRoughness(Texture* roughness) { m_roughness = roughness; };
	inline void SetRoughness(float roughness) { m_roughnessValue = roughness; };
	inline Texture* GetRoughness() { return m_roughness; };

	inline void SetAO(Texture* ao) { m_ao = ao; };
	inline void SetAO(float ao) { m_aoValue = ao; };
	inline Texture* GetAO() { return m_ao; };
	 
	inline void SetMetallic(Texture* metallic) { m_metallic = metallic; };
	inline void SetMetallic(float metallic) { m_metallicValue = metallic; }
	inline Texture* GetMetallic() { return m_metallic; };
	 
	inline void SetDisplacement(Texture* displacement) { m_displacement = displacement; };
	inline Texture* GetDisplacement() { return m_displacement; };

	inline bool HasAlbedoTexture() const { return m_hasAlbedoTexture; };
	inline bool HasNormalTexture() const { return m_hasNormalTexture; };
	inline bool HasRoughnessTexture() const { return m_hasRoughnessTexture; };
	inline bool HasAOTexture() const { return m_hasAOTexture; };
	inline bool HasMetallicTexture() const { return m_hasMetallicTexture; };
	inline bool HasDisplacementTexture() const { return m_hasDisplacementTexture; };
	
	inline void UseAlbedoTexture(bool use) { m_hasAlbedoTexture = use; };
	inline void UseNormalTexture(bool use) { m_hasNormalTexture = use; };
	inline void UseRoughnessTexture(bool use) { m_hasRoughnessTexture = use; };
	inline void UseAOTexture(bool use) { m_hasAOTexture = use; };
	inline void UseMetallicTexture(bool use) { m_hasMetallicTexture = use; };
	inline bool UseDisplacementTexture(bool use) { m_hasDisplacementTexture = use; };

#pragma endregion

    void Bind(Shader* shader);

private:

	Texture* m_albedo;
	Texture* m_normal;
	Texture* m_roughness;
	Texture* m_ao;
	Texture* m_metallic;
	Texture* m_displacement;

	glm::vec3 m_albedoColour;
	float m_roughnessValue;
	float m_aoValue;
	float m_metallicValue;

	bool m_hasAlbedoTexture;
	bool m_hasNormalTexture;
	bool m_hasRoughnessTexture;
	bool m_hasAOTexture;
	bool m_hasMetallicTexture;
	bool m_hasDisplacementTexture;

};

