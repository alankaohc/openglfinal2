#include "MyImGuiPanel.h"

#include <iostream>


MyImGuiPanel::MyImGuiPanel()
{
	this->m_avgFPS = 0.0;
	this->m_avgFrameTime = 0.0;
}


MyImGuiPanel::~MyImGuiPanel()
{
}

void MyImGuiPanel::update() {
	// performance information
	const std::string FPS_STR = "FPS: " + std::to_string(this->m_avgFPS);
	ImGui::TextColored(ImVec4(0, 220, 0, 255), FPS_STR.c_str());
	const std::string FT_STR = "Frame: " + std::to_string(this->m_avgFrameTime);
	ImGui::TextColored(ImVec4(0, 220, 0, 255), FT_STR.c_str());

	
	ImGui::Checkbox("Normal Mapping", &m_normalMappingEnabled);
	ImGui::Checkbox("Visualize cascade", &m_cascadeEnabled);


	if (ImGui::RadioButton("Final", mode == 1)) {
		mode = 1;
	}
	if (ImGui::RadioButton("Normal", mode == 2)) {
		mode = 2;		
	}
	if (ImGui::RadioButton("Specular", mode == 3)) {
		mode = 3;
	}
	if (ImGui::RadioButton("Diffuse", mode == 4)) {
		mode = 4;
	}
	if (ImGui::RadioButton("Vertex", mode == 5)) {
		mode = 5;
	}
	
	if (ImGui::RadioButton("Teleport 1", modeT == 1)) {
		modeT = 1;

	}
	if (ImGui::RadioButton("Teleport 2", modeT == 2)) {
		modeT = 2;

	}
	if (ImGui::RadioButton("Teleport 3", modeT == 3)) {
		modeT = 3;

	}
	

}

void MyImGuiPanel::setAvgFPS(const double avgFPS){
	this->m_avgFPS = avgFPS;
}
void MyImGuiPanel::setAvgFrameTime(const double avgFrameTime){
	this->m_avgFrameTime = avgFrameTime;
}