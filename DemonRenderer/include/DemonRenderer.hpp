#pragma once

#include <cstdint>
#include <limits>
//#include <iostream>

#include "core/application.hpp"
#include "core/layer.hpp"
#include "core/log.hpp"
#include "core/timer.hpp"
#include "core/physics.hpp"
#include "core/randomiser.hpp"

#include "assets/cubeMap.hpp"
#include "assets/managedTexture.hpp"
#include "assets/mesh.hpp"
#include "assets/shader.hpp"
#include "assets/texture.hpp"
#include "assets/textureUnitManager.hpp"

#include "buffers/FBO.hpp"
#include "buffers/FBOlayout.hpp"
#include "buffers/IBO.hpp"
#include "buffers/RBO.hpp"
#include "buffers/SSBO.hpp"
#include "buffers/UBO.hpp"
#include "buffers/UBOlayout.hpp"
#include "buffers/UBOmanager.hpp"
#include "buffers/VAO.hpp"
#include "buffers/VBO.hpp"
#include "buffers/VBOlayout.hpp"

#include "events/events.hpp"
#include "events/eventHandler.hpp"

#include "components/render.hpp"
#include "components/script.hpp"
#include "components/transform.hpp"
#include "components/order.hpp"
#include "components/collidable.hpp"

#include "rendering/camera.hpp"
#include "rendering/computePass.hpp"
#include "rendering/depthOnlyPass.hpp"
#include "rendering/lights.hpp"
#include "rendering/material.hpp"
#include "rendering/renderer.hpp"
#include "rendering/renderPass.hpp"
#include "rendering/uniformDataTypes.hpp"

#include "sound/soundManager.hpp"

#include "windows/GLFW_GL_GC.hpp"
#include "windows/GLFWSystem.hpp"
#include "windows/GLFWWindowImpl.hpp"
#include "windows/graphicsContext.hpp"
#include "windows/window.hpp"

#include "tracy/Tracy.hpp"










