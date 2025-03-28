﻿/** \file application.cpp */

#include "core/application.hpp"
#include "tracy/tracy.hpp"

void* operator new (std::size_t count)
{
	//spdlog::info("Allocating size of {}", count);
	void* ptr;
	if (Application::MainLoop && Application::memArena != nullptr) {
		spdlog::info("Allocating size of + Arena {}", count);
		ptr = ((Arena*)Application::memArena.get())->get(count);
		//TracyAllocN(ptr, count, "Arena");
	}
	else {

		ptr = malloc(count);
		TracyAlloc(ptr, count);
	}

	return ptr;
}

void operator delete (void* ptr) noexcept
{


	if (Application::MainLoop && Application::memArena != nullptr) {
		//TracyFreeN(ptr, "Arena");
		if (!((Arena*)Application::memArena.get())->release(ptr)) {
			free(ptr);
		}

	}
	else {
		TracyFree(ptr);
		free(ptr);
	}

}

Application::Application(const WindowProperties& winProps)
{
	m_window.open(winProps);

	m_window.handler.onWinClose = [this](WindowCloseEvent& e) {onClose(e);};
	m_window.handler.onKeyPress = [this](KeyPressedEvent& e) {onKeyPressed(e);};
	m_window.handler.onKeyRelease = [this](KeyReleasedEvent& e) {onKeyReleased(e);};

	auto setupTime = m_timer.reset();
	memArena = std::make_shared<Arena>();

}

void Application::run()
{
	spdlog::debug("Application running");

	glEnable(GL_DEPTH_TEST);
	Application::MainLoop = true;
	while (m_running) {
		auto timestep = m_timer.reset();


		onUpdate(timestep);
		if (m_window.isHostingImGui()) onImGuiRender();
		onRender();

		m_window.onUpdate(timestep);

	}
}

void Application::onUpdate(float timestep)
{
	if (m_layer) m_layer->onUpdate(timestep);
}

void Application::onRender() const
{
	if (m_layer) m_layer->onRender();
}

void Application::onImGuiRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (m_ImGuiOpen)
	{
		ImGui::Begin("ImGUI Window", &m_ImGuiOpen, ImGuiWindowFlags_MenuBar);
		if (!m_ImGuiOpen) spdlog::info("ImGui window closed. Press I to reopen it.");

		if (m_layer) m_layer->onImGUIRender();

		ImGui::End();
	}

	ImGui::Render();
}

void Application::onClose(WindowCloseEvent& e)
{
	m_running = false;
	if (m_layer) m_layer->onClose(e);
	e.handle();
}

void Application::onResize(WindowResizeEvent& e)
{
	if (m_layer) m_layer->onResize(e);
}

void Application::onKeyPressed(KeyPressedEvent& e)
{
	if (e.getKeyCode() == GLFW_KEY_ESCAPE) {
		spdlog::info("Exiting");
		m_running = false;
		e.handle();
		return;
	}

	else {
		if (e.getKeyCode() == GLFW_KEY_I) m_ImGuiOpen = true;
		if (m_layer) m_layer->onKeyPressed(e);
		e.handle();
	}

}

void Application::onKeyReleased(KeyReleasedEvent& e)
{
	if (m_layer) m_layer->onKeyReleased(e);
}

void Application::onMousePress(MouseButtonPressedEvent& e)
{
	if (m_layer) m_layer->onMousePress(e);
}

void Application::onMouseReleased(MouseButtonReleasedEvent& e)
{
	if (m_layer) m_layer->onMouseReleased(e);
}

void Application::onMouseMoved(MouseMovedEvent& e)
{
	if (m_layer) m_layer->onMouseMoved(e);
}

void Application::onMouseScrolled(MouseScrolledEvent& e)
{
	if (m_layer) m_layer->onMouseScrolled(e);
}

bool getBitAtOffset(const void* ptr, uint8_t bitOffset) {

	// Access the byte at the given offset
	uint8_t byte = *((char*)ptr + (bitOffset / 8));

	// Extract the bit at the specified bitOffset
	return (byte & (1 << (bitOffset % 8))) != 0;

}

void flipBitAtOffset(void* ptr, uint8_t bitOffset) {

	// Access the byte at the given offset
	char* a = ((char*)ptr + (bitOffset / 8));
	char byte = *a;
	// Change the bit
	byte ^= (1 << (bitOffset % 8));

	*a = byte;

}
struct extraSize {
	int x{ 0 };
	int y{ 0 };
	int z{ 0 };
	int w{ 0 };
};
Arena::Arena()
{

	std::size_t totalMemNeeded = 0;


	int currentOffset = 0;
	for (int i = 0; i < m_sizes.size(); i++) {
		totalMemNeeded += m_sizes[i] * m_counts[i];
		m_availability[i] = (char*)malloc(m_counts[i] / 8);
		std::vector<char> o(m_counts[i] / 8);
		std::memcpy(((char*)m_availability[i]), o.data(), m_counts[i] / 8);

	}
	m_base.reset(malloc(totalMemNeeded));
	n_size = totalMemNeeded;
	for (int i = 0; i < m_basePtrs.size(); i++) {
		m_basePtrs[i] = (void*)((char*)m_base.get() + currentOffset);
		currentOffset += m_sizes[i] * m_counts[i];
	}

}



void* Arena::get(size_t count)
{
	std::size_t temp = -1;
	for (int i = 0; i < m_sizes.size(); i++) {
		if (count <= m_sizes[i]) {
			temp = i;
			break;
		}
	}

	//spdlog::info("Geting for Size: {}", m_sizes[temp]);
	while (temp < m_sizes.size()) {
		for (int i = 0; i < m_counts[temp] / 8; i++) {
			if (m_availability[temp] == nullptr) {
				break;
			}
			char* selected = (char*)m_availability[temp] + i;
			if (*selected != 'ÿ') { // checks if byte is 11111111
				for (int j = 0; j < 8; j++) {
					if (!getBitAtOffset(selected, j)) {
						flipBitAtOffset(selected, j);
						void* outptr = (void*)((char*)m_basePtrs[temp] + (j * m_sizes[temp]));
						//spdlog::info("Added At Offset: {}", (char*)outptr - (char*)m_base.get());
						return outptr;
					}
				}
			}
		}
		temp++;
	}
	return nullptr;
}

bool Arena::release(void* ptr)
{

	uint32_t GlobalOffset = (char*)ptr - (char*)m_base.get();
	if (GlobalOffset > n_size || GlobalOffset < 0) {
		return false;
	}

	uint32_t offset = 0;
	uint32_t counter;
	for (int i = 0; i < m_basePtrs.size(); i++)
	{
		offset = (char*)ptr - (char*)m_basePtrs[i];
		if (offset >= 0) {
			counter = i;
			break;
		}
	}

	if ((unsigned long long)m_availability[counter] == 0xdddddddddddddddd) {
		return false;
	}
	flipBitAtOffset((char*)m_availability[counter], offset / m_sizes[counter]);
	//spdlog::info("Freed At Offset: {}", offset);
	return true;

	//uint32_t counter2 = 0;
	//for (int i = 0; i < m_counts.size(); i++) {
	//	counter2 += m_counts[i] * m_sizes[i];
	//	if (counter2 >= offset) {
	//		counter2 -= m_counts[i] * m_sizes[i];
	//		offset -= counter2;
	//		

	//	}
	//}

}

void Arena::clear()
{
	std::size_t totalMemNeeded = 0;
	for (int i = 0; i < m_sizes.size(); i++) {
		totalMemNeeded += m_sizes[i] + m_counts[i];
		if (m_availability[i] != nullptr) free(m_availability[i]);
		m_availability[i] = (char*)malloc(m_counts[i] / 8);
	}
	m_base.reset(malloc(totalMemNeeded));

	int currentOffset = 0;
	for (int i = 0; i < m_basePtrs.size(); i++) {
		m_basePtrs[i] = (void*)((char*)m_base.get() + currentOffset);
		currentOffset += m_sizes[i] * m_counts[i];
	}
}