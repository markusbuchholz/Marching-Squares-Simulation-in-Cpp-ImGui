// Markus Buchholz, 2023

#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include <tuple>
#include <vector>
#include <math.h>
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>

//---------------------------------------------------------------

int w = 1000;
int h = 1000;

float dt = 50.0f;

int grid_size = 100;
int step = 50;
ImVec2 offset(0.0f, 0.0f);

//---------------------------------------------------------------

int generateRandomBinary()
{

	std::random_device engine;
	std::mt19937 generator(engine());
	std::uniform_int_distribution<int> distrib(0, 1);
	return distrib(generator);
}

//---------------------------------------------------------------

struct Grid
{

	std::vector<std::vector<int>> mesh;

	Grid()
	{
		std::vector<int> i_grid(grid_size, 0);
		mesh = std::vector<std::vector<int>>(grid_size, i_grid);

		for (int ii = 0; ii < grid_size; ii++)
		{
			for (int jj = 0; jj < grid_size; jj++)
			{

				mesh[ii][jj] = generateRandomBinary();
			}
		}
	}

	void evolution()
	{

		for (int ii = 0; ii < grid_size; ii++)
		{
			for (int jj = 0; jj < grid_size; jj++)
			{

				this->mesh[ii][jj] = generateRandomBinary();
			}
		}
	}

	int lookUpTable(int a, int b, int c, int d)
	{

		if (a == 0 && b == 0 && c == 0 && d == 0)
		{
			return 0;
		}
		else if (a == 0 && b == 0 && c == 0 && d == 1)
		{
			return 1;
		}
		else if (a == 0 && b == 0 && c == 1 && d == 0)
		{
			return 2;
		}
		else if (a == 0 && b == 0 && c == 1 && d == 1)
		{
			return 3;
		}
		else if (a == 0 && b == 1 && c == 0 && d == 0)
		{
			return 4;
		}
		else if (a == 0 && b == 1 && c == 0 && d == 1)
		{
			return 5;
		}
		else if (a == 0 && b == 1 && c == 1 && d == 0)
		{
			return 6;
		}
		else if (a == 0 && b == 1 && c == 1 && d == 1)
		{
			return 7;
		}
		else if (a == 1 && b == 0 && c == 0 && d == 0)
		{
			return 8;
		}
		else if (a == 1 && b == 0 && c == 0 && d == 1)
		{
			return 9;
		}
		else if (a == 1 && b == 0 && c == 1 && d == 0)
		{
			return 10;
		}
		else if (a == 1 && b == 0 && c == 1 && d == 1)
		{
			return 11;
		}
		else if (a == 1 && b == 1 && c == 0 && d == 0)
		{
			return 12;
		}
		else if (a == 1 && b == 1 && c == 0 && d == 1)
		{
			return 13;
		}
		else if (a == 1 && b == 1 && c == 1 && d == 0)
		{
			return 14;
		}
		else
		{
			return 15;
		}
	}
};

//---------------------------------------------------------------

int main(int argc, char const *argv[])
{

	std::string title = "Marching Squares";
	initImgui(w, h, title);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.0f / 255.0, 0.0f / 255.0, 0.0f / 255.0, 1.00f);
	ImVec4 white_color = ImVec4(255.0f / 255.0, 255.0f / 255.0, 255.0f / 255.0, 1.00f);
	ImVec4 blue_color = ImVec4(0.0f / 255.0, 0.0f / 255.0, 170.0f / 255.0, 1.00f);
	ImVec4 pink_color = ImVec4(179.0f / 255.0, 12.0f / 255.0, 130.0f / 255.0, 1.00f);
	ImVec4 gray_color = ImVec4(150.0f / 255.0, 10.0f / 160.0, 170.0f / 255.0, 1.00f);
	ImVec4 black_color = ImVec4(0.0f / 255.0, 0.0f / 255.0, 0.0f / 255.0, 1.00f);

	bool flag = true;

	Grid grid;

	while (!glfwWindowShouldClose(window) && flag == true)
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = 0;
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_FirstUseEver);
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::Begin("Simulation", nullptr, window_flags);
		ImDrawList *draw_list = ImGui::GetWindowDrawList();

		for (int ii = 0; ii < grid.mesh.size(); ii++)
		{
			for (int jj = 0; jj < grid.mesh.size(); jj++)
			{

				auto color = grid.mesh[ii][jj] == 0 ? black_color : white_color;
				// draw_list->AddCircleFilled({offset.x + ii * step, offset.y + jj * step}, 2.0f, ImColor(color));
			}
		}

		for (int ii = 0; ii < grid.mesh.size() - 1; ii++) // y
		{
			for (int jj = 0; jj < grid.mesh.size() - 1; jj++) // x
			{
				ImVec2 a_2((float)ii * step + (float)step / 2, (float)jj * step);
				ImVec2 b_2((float)(ii + 1) * step, (float)jj * step + (float)step / 2);
				ImVec2 c_2((float)ii * step + (float)step / 2, (float)(jj + 1) * step);
				ImVec2 d_2((float)ii * step, (float)jj * step + (float)step / 2);

				auto decision = grid.lookUpTable(grid.mesh[jj][ii], grid.mesh[jj][ii + 1], grid.mesh[jj + 1][ii + 1], grid.mesh[jj + 1][ii]);

				switch (decision)
				{

				case 0:
					break;
				case 1:
					draw_list->AddLine(c_2, d_2, ImColor(pink_color), 2.0f);
					break;
				case 2:
					draw_list->AddLine(b_2, c_2, ImColor(pink_color), 2.0f);
					break;
				case 3:
					draw_list->AddLine(b_2, d_2, ImColor(pink_color), 2.0f);
					break;
				case 4:
					draw_list->AddLine(a_2, b_2, ImColor(pink_color), 2.0f);
					break;
				case 5:
					draw_list->AddLine(a_2, d_2, ImColor(pink_color), 2.0f);
					draw_list->AddLine(b_2, c_2, ImColor(pink_color), 2.0f);
					break;
				case 6:
					draw_list->AddLine(a_2, c_2, ImColor(pink_color), 2.0f);
					break;
				case 7:
					draw_list->AddLine(a_2, d_2, ImColor(pink_color), 2.0f);
					break;
				case 8:
					draw_list->AddLine(a_2, d_2, ImColor(pink_color), 2.0f);
					break;
				case 9:
					draw_list->AddLine(a_2, c_2, ImColor(pink_color), 2.0f);
					break;
				case 10:
					draw_list->AddLine(a_2, b_2, ImColor(pink_color), 2.0f);
					draw_list->AddLine(c_2, d_2, ImColor(pink_color), 2.0f);
					break;
				case 11:
					draw_list->AddLine(a_2, b_2, ImColor(pink_color), 2.0f);
					break;
				case 12:
					draw_list->AddLine(b_2, d_2, ImColor(pink_color), 2.0f);
					break;
				case 13:
					draw_list->AddLine(b_2, c_2, ImColor(pink_color), 2.0f);
					break;
				case 14:
					draw_list->AddLine(c_2, d_2, ImColor(pink_color), 2.0f);
					break;
				case 15:
					break;
				}
			}
		}

		grid.evolution();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	termImgui();
	return 0;
}
