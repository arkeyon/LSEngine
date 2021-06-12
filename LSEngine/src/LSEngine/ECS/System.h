#pragma once

#include "LSEngine/Core/Core.h"

namespace LSE {

	template <class T>
	class EntitySystem
	{
	public:
		virtual void Create(Ref<T> component) = 0;
		virtual void Update(Ref<T> component) = 0;
		virtual void Delete(Ref<T> component) = 0;
	};
}

class Renderable;

class RendererSystem : public LSE::EntitySystem<Renderable>
{
	virtual void Create(LSE::Ref<Renderable> component) override;

	virtual void Update(LSE::Ref<Renderable> component) override;

	virtual void Delete(LSE::Ref<Renderable> component) override;
};
