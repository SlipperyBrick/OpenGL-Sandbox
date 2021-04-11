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

	void SetAlbedo(Texture albedo);
	void SetNormal();
	void SetRoughness();
	void SetAO();
	void SetMetallic();
	void SetDisplacement();

    void Bind();

private:
	Texture* m_albedo;
	Texture* m_normal;
	Texture* m_roughness;
	Texture* m_ao;
	Texture* m_metallic;
	Texture* m_displacement;
};

