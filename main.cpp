#include "SFML/System/Vector2.hpp"
#include "UseImGui.h"
// HACK: Dont forget to export LD_LIBRARY_PATH=./lib before running exe becuase dynamic linker dont know where to find SFML libraries
#include <SFML/Audio.hpp>
#include "imgui.h"
#include "implot.h"
#include <cstddef>
#include <fstream>
#include <implot.h>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>


void StyleCustom()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0;
    style.WindowRounding = 3;
    style.GrabRounding = 1;
    style.GrabMinSize = 20;
    style.FrameRounding = 3;

    style.Colors[ImGuiCol_Text]                  = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_Separator]             = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
}

// utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset  = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x,y));
        else {
            Data[Offset] = ImVec2(x,y);
            Offset =  (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset  = 0;
        }
    }
};

void displaySoundInformation(sf::SoundBuffer buffer)
{
    std::cout << " " << buffer.getDuration().asSeconds() << " seconds" << '\n'
            << " " << buffer.getSampleRate() << " samples / sec" << '\n'
            << " " << buffer.getChannelCount() << " channels" << '\n';
}

void bufferSamplesToCSV(sf::SoundBuffer buffer)
{
    const std::int16_t* samplesArray = buffer.getSamples();
    // int arrayCopy[buffer.getSampleCount()];
    std::ofstream csv("sstv.csv");
    for (std::size_t i = 0; i < buffer.getSampleCount(); i++)
    {
        // arrayCopy[i] = samplesArray[i];
        csv << samplesArray[i] << '\n';
    }
    csv.close();
}


float floatToMinutes(float secondsAsFloat)
{
    // HACK: We are converting the duration in seconds to a float value that represnts the time in minutes as float
    int durationSeconds = secondsAsFloat;
    // HACK: Getting the time in minutes and casting it as int to truncate it to just the minute values
    int minute = durationSeconds/60;
    // HACK: The seconds values is derived by using a modulo to get the float remainder and then dividing it by 100
    // and then adding the results together
    float seconds = fmod(durationSeconds, 60.0f) / 100.0f;
    float durationAsFloatMinutes = static_cast<float>(minute) + seconds;
    return durationAsFloatMinutes;
}

std::vector<float> normalizeSamples(std::vector<float> samples,std::uint64_t size)
{
    std::vector<float> r_samples;
    r_samples.resize(samples.size());
    // Normalize valuesto plot from -1 to 1
    float maximum = 0.0;
    float minimum = 0.0;

    // Get the maximum and minimum values
    for (std::uint64_t i = 0; i < size; i++)
    {
        if (samples[i] > maximum)
        {
            maximum = samples[i];
        }

        if (samples[i] < minimum)
        {
            minimum = samples[i];
        }
    }
    float range = maximum - minimum;

    for (std::uint64_t i = 0; i < size; i++)
    {
        float normalized = -1.0 + (((samples[i] - minimum) * (1.0 - -1.0))/range);
        r_samples[i] = normalized;
    }
    return r_samples;
}

static void glfw_error_callback(int error, const char* description)
{
    //HACK: Disabling "Error 65539: Invalid window attribute 0x0002000D" error message that keeps printing for some unknown reason
    if (error != 65539)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

std::vector<ImVec2> averageSamples(const std::int16_t* samples, std::uint64_t sample_size, int n_samples)
{
    std::vector<ImVec2> avg_samples;
    avg_samples.reserve(int((sample_size/n_samples) + 1));
    int pidx = 0;
    // const int spacing = 300'000;
    for (std::uint64_t i = 0; i < sample_size;) 
    {
        float sum = 0;
        float average_point;
        if(i == sample_size - (sample_size % n_samples))
        {
            for(int j = i; j < i + sample_size % n_samples ; j++)
            {
                sum += samples[j];
            }
            average_point = sum/n_samples;
            // points[pidx] = average_point;
        }
        else
        {
            for(int j = i; j < i + n_samples ; j++)
            {
                sum += samples[j];
            }
            average_point = sum/n_samples;
        }
        avg_samples.push_back(ImVec2(pidx,average_point));
        i += n_samples;
        pidx++;
    }

    return avg_samples;
}

const char* timeline_labels(int song_duration, int interval)
{
    char* labels = new char[song_duration/interval];
    for(int i = 0; i < song_duration; i += interval)
    {
        labels[i] = floatToMinutes(i);
    }
    return labels;
}

static int window_width = 1280;
static int window_height = 720;

ImVec2 window_pos = ImVec2(1280,720);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    window_pos = ImVec2(width, height);
    glViewport(0,0,width,height);

    std::cout << "(w: " << width << " , h" << height << " )" << std::endl;
}

int main(void)
{
    sf::SoundBuffer buffer("September.wav");
    sf::Sound sound(buffer);

    displaySoundInformation(buffer);
    int song_duration = buffer.getDuration().asSeconds();
    int intervals = 15;



    //TODO: Create functions to pass buffer to new array and a function to average the arrays
    //WARN: NVM with that. have to do some weird shit to return arrays in a function. Not in the mood for that
    //TODO: Store ponts as double to get the averages
    int n_samples = 512;
    const std::uint64_t size = buffer.getSampleCount(); 
    const std::int16_t* samples = buffer.getSamples();

    std::vector<ImVec2> avg_samples = averageSamples(samples, size, n_samples);

    // Setting volume to 50% becuase my ears are still ringing
    sound.setVolume(5.0);
    ImVec4 clear_color = ImVec4(0.14f,0.16f,0.18f,1.00f);
    bool is_playing = false;

    // Creating sfml clock variable
    // HACK: It immediately starts upon creation so we reset and stop it, upon creation to get it on 0
    sf::Clock clock;
    clock.reset();
    clock.stop();


    static int index = 0;
    static ScrollingBuffer sdata;
    // sdata.AddPoint(clock.getElapsedTime().asSeconds(), buffer.getSamples()[index]);

    float durationSeconds = buffer.getDuration().asSeconds();

    // std::cout << durationFull;


    glfwSetErrorCallback(glfw_error_callback);
    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    // Create a window and its context
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "My window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Make the windows context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable Vsync

    gladLoadGL(glfwGetProcAddress);

    UseImGui myimgui;
    myimgui.Init(window);
    ImPlot::CreateContext();


    static float history = 1.0f;

    // Loop until the user closes window
    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll for and process events
        glfwPollEvents();
        myimgui.NewFrame();

        float elapsedTimeSeconds = clock.getElapsedTime().asSeconds();
        float percentOfDuration = elapsedTimeSeconds/durationSeconds * 100.0f;

        // Populates myimgui frame
        // myimgui.Update();
        //
        {


            ImVec2 timeline_window_size = ImVec2(window_width/2.0,window_height * 0.4);
            ImVec2 timeline_button_size = ImVec2(timeline_window_size.y * 0.2,timeline_window_size.y * 0.2);

            // NOTE:
            // Creating an ImGui frame with ImGui::Begin
            // Parameter 1: is title of frame
            // Parameter 2: a bool to show window close or not... i think
            // Parameter 3: Setting window flags; use | to chain flags
            //              NoTitleBar
            //              NoMove = 4
            //              NoBackground = 128
            //              //HACK: Do not use NoResize, If enabled the WindowSize wont adjust to contents, thus we wont get the window size to center the button
            //              NoResize = 2
            ImGui::Begin("TimeLine",__null,1|4|128);
            StyleCustom();
            ImGui::SetNextWindowSize(ImVec2(timeline_window_size.x,0),ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(window_pos.x/2.0 - timeline_window_size.x/2.0, window_pos.y - timeline_window_size.y),ImGuiCond_Always);
            const char* time_labels[5] = {"0","15","30","40","45"};
            if (ImPlot::BeginPlot("Spectograph",ImVec2(timeline_window_size.x,timeline_window_size.y * 0.5),ImPlotFlags_CanvasOnly))
            {

                //TODO:Set up axis labels
                // ImPlot::SetupAxisTicks(ImAxis_X1,0,1,5,time_labels,true);
                ImPlot::SetupAxes(nullptr, nullptr,ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_NoTickLabels|ImPlotAxisFlags_NoDecorations);
                ImPlot::PlotLine("Line",&avg_samples[0].x,&avg_samples[0].y,avg_samples.size(),0,0,2*sizeof(float));
                ImPlot::EndPlot();
            }

            // ImGui::SameLine(timeline_window_size.x/2.0 - timeline_button_size.x/2.0);
            // TODO: Parse info as a string and then remove the decimal '.' and replace with ':' to display time format
            // Also add if for if the time is less 10 seconds it shows a 0 in front of the values so 02:30 and not 2:30
            // XXX: DO NOT TOUCH SLIDER, we just need it to visualize progress
            // NOTE: Slider Flags:
            //                      AlwaysClamp = 16

            // NOTE: %% will write a single % to the stream. Cants use escape character \ for % sign :(
            // ImGui::Text("%.2f", floatToMinutes(elapsedTimeSeconds)); ImGui::SameLine(); 
            // ImGui::SetNextItemWidth(timeline_window_size.x);
            // ImGui::SliderFloat(" ", &percentOfDuration, 0.0f,100.0f,"%.2f %%",16); ImGui::NewLine();
            // ImGui::Text("%.2f",floatToMinutes(buffer.getDuration().asSeconds() - elapsedTimeSeconds)); ImGui::NewLine();
            //TODO: Change position of button
            //Make it Relative to center of scream center
            
            ImGui::NewLine();
            ImGui::NewLine();
            ImGui::NewLine();
            ImGui::SameLine(timeline_window_size.x/2.0 - timeline_button_size.x/2.0);

            // ImGui::SameLine(timeline_window_size.x/2.0 - timeline_button_size.x/2.0);

            // TODO: Move these up to the main to just initilise them instead of it happening every frame
            // NOTE: Sets the values derived from floats to be 'fixed' meaning no scientific notation
            // replace fixed with scientific to display scientific notation
            std::cout.setf(std::ios::fixed);
            // NOTE: sets the values of floats to 2 decimals
            std::cout.precision(2);

            // std::cout << percentOfDuration << std::endl;
            // std::cout << buffer.getChannelCount() << std::endl;

            if (is_playing)
            {
                sound.play();
                clock.start();

                if (ImGui::Button("Pause", timeline_button_size))
                {
                    is_playing = false;
                }
            }

            else if(!is_playing)
            {
                sound.pause();
                clock.stop();
                if (ImGui::Button("Play", timeline_button_size))
                {
                    is_playing = true;
                }
            }

            ImGui::End();
        }


        //NOTE: index reference table for buffer samples to time using the Samples per second
        //TODO: Change 4800 value to use the buffer Samples Per Second
        index = int(clock.getElapsedTime().asSeconds() * 4800);

        // const int spacing = 20'000;
        // if (is_playing)
        // {
        //     sdata.AddPoint(clock.getElapsedTime().asSeconds() * spacing, buffer.getSamples()[index]);
        // }

        // {
        //     ImGui::Begin("Spectograph",nullptr,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoBackground);
        //     ImGui::SetWindowSize(ImVec2(200,400));
        //     ImGui::SetWindowPos(ImVec2(0,0));
        //     // ImGui::SetWindowPos(ImVec2(window_pos.x/2.0 - windowSize.x/2.0, window_pos.y - windowSize.y/2.0));
        //
        //     if (ImPlot::BeginPlot("Spectograph"),ImVec2(-1,-1),ImPlotFlags_CanvasOnly)
        //     {
        //         ImPlot::SetupAxes(nullptr, nullptr,ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_NoTickLabels|ImPlotAxisFlags_NoDecorations);
        //         // ImPlot::SetupAxisLimits(ImAxis_X1,( ( clock.getElapsedTime().asSeconds() * spacing ) - ( history * spacing ) ), clock.getElapsedTime().asSeconds() * spacing, ImGuiCond_Always);
        //         // ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        //         // ImPlot::PlotLine("Line",&sdata.Data[0].x,&sdata.Data[0].y,sdata.Data.size(),0,sdata.Offset,2*sizeof(float));
        //         // int idx = (int)(m_samples.size());
        //         ImPlot::PlotLine("Line",&avg_samples[0].x,&avg_samples[0].y,avg_samples.size(),0,0,2*sizeof(float));
        //         // ImPlot::PlotLine("Line")
        //         ImPlot::EndPlot();
        //     }

            //TODO: Move these to seperate gui func
            // ImGui::Text("Time: %f", clock.getElapsedTime().asSeconds());
            // ImGui::Text("Index: %i", index);
            // ImGui::Text("Current Data: %i", buffer.getSamples()[index]);

            // ImGui::End();

        // }

        // Rendering
        // (Your code clears your framebuffer, renders your other stuff etc.)
        glViewport(0,0,window_width,window_height);
        glClearColor(clear_color.x,clear_color.y,clear_color.z,clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        myimgui.Render();
        // (Your code calls glfwSwapBuffers() etc.)

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    ImPlot::DestroyContext();
    myimgui.Shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
