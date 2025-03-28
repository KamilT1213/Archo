
#pragma once

#include "events/keyEvents.hpp"
#include <entt/entt.hpp>
#include "rendering/scene.hpp"
#include <memory>
#include "core/log.hpp"

/**    \class Script
*    \brief A class which exhibits runtime behavoiur.
*    Script class must be attached to Actors via a Scriptable component
*/

class Script
{
public:
	Script(entt::entity entity, std::shared_ptr<Scene> scene) : m_entity(entity), m_registry(scene->m_entities) {};//TestMemory(); }
	virtual void onUpdate(float timestep) = 0; //!< Runs on update
	virtual void onImGuiRender() {}; //!< Runs on ImGuiRender
	virtual void onKeyPress(KeyPressedEvent& e) = 0; //!< Runs on key press
	virtual void onKeyRelease(KeyReleasedEvent& e) = 0; //!< Runs on key release
protected:

	entt::registry& m_registry;
	entt::entity m_entity;
	void TestMemory() {
		spdlog::info("Script\t\t\tsizeof {}\talignof {}", sizeof(Script), alignof(Script));
		spdlog::info("m_registry\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(Script&), alignof(entt::registry&), offsetof(Script, m_registry));
		spdlog::info("m_entity\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(Script::m_entity), alignof(entt::entity), offsetof(Script, m_entity));
	};

};

/** \class ScriptComp
*    \brief A script component to be added to a entity and can have a scipt attached
*/

class ScriptComp
{
public:
	ScriptComp() = default; //!< Default constructor
	/** Runs on update */
	void onUpdate(float timestep) { if (m_script) m_script->onUpdate(timestep); }
	/** Runs on ImGuiRender */
	void onImGuiRender() { if (m_script) m_script->onImGuiRender(); }
	/** Runs on key press */
	void onKeyPress(KeyPressedEvent& e) { if (m_script) m_script->onKeyPress(e); }
	/** Runs on key release */
	void onKeyRelease(KeyReleasedEvent& e) { if (m_script) m_script->onKeyRelease(e); }
	/** Method which attaches the Script subclass to the the actor */
	template <typename T, typename ...Args> void attachScript(Args&& ...args)
	{
		T* ptr = new T(std::forward<Args>(args)...);
		m_script.reset(static_cast<Script*>(ptr));
	}
//private:
	std::shared_ptr<Script> m_script{ nullptr }; //!< Script subclass
};