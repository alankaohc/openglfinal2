#pragma once

#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw.h>
#include <imgui\imgui_impl_opengl3.h>

#include <string>

class MyImGuiPanel
{
public:
	MyImGuiPanel();
	virtual ~MyImGuiPanel();

public:
	void update();
	void setAvgFPS(const double avgFPS);
	void setAvgFrameTime(const double avgFrameTime);
	bool isNormalMappingEnabled() const { return m_normalMappingEnabled; }
	bool isCascadeEnabled() const { return m_cascadeEnabled; }
	int getMode() const { return mode; }
	int getModeT() const { return modeT; }

private:
	double m_avgFPS;
	double m_avgFrameTime;
	bool m_normalMappingEnabled = false;
	bool m_cascadeEnabled = false;
	int mode = 1;
	int modeT = 1;


};

