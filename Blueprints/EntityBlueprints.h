#pragma once

enum class VX_E_EntityTypes {
	GROUND,
	AIR,
	NONE
};

namespace VX_N_Blueprints {
	using namespace VX_N_Blueprints;
	// Blueprints for making an entity
	struct VX_E_EntityBlueprint {
		bool gravity;
		bool physics;
		VX_E_EntityTypes entityType;
		VX_E_PhysicsState physState = VX_E_PhysicsState::NONE;
		RenderLayer layer = RenderLayer::NONE;
		VX_E_EntityBlueprint(bool g, bool p, VX_E_EntityTypes t, VX_E_PhysicsState ps, RenderLayer l) {
			gravity = g;
			physics = p;
			entityType = t;
			physState = ps;
			layer = l;
		}
	};

	// Textures for entities
	static class VX_C_EntityTextures {
	public:
		static VX_C_Texture getAirTexture(VX_C_Window* targetWindow) {
			VX_C_Texture a;
			a.loadFromFile("SKY2020.png", targetWindow->fetchRender());
			return a;
		}
		static VX_C_Texture getGroundTexture(VX_C_Window* targetWindow) {
			VX_C_Texture g;
			g.loadFromFile("GROUND2020.png", targetWindow->fetchRender());
			return g;
		}
	};
}
