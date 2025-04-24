#include "UseImGui.h"
// HACK: Dont forget to export LD_LIBRARY_PATH=./lib before running exe becuase dynamic linker dont know where to find SFML libraries
#include <SFML/Audio.hpp>
#include "imgui.h"
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

int main(void)
{
    sf::SoundBuffer buffer("sstv.wav");
    sf::Sound sound(buffer);

    displaySoundInformation(buffer);

    // Print the samples from array bugger.getSamples()
    // const std::int16_t* samples = buffer.getSamples();

    // int sampleSegment[4800];
    // for (int i = 0; i < 4800; i++)
    // {
    //     sampleSegment[i] = samples[i];
    //     std::cout << i << ": " << sampleSegment[i] << std::endl;
    // }


    // for (std::size_t i = 0; i < buffer.getSampleCount(); i++)
    // {
    //     std::cout << samples[i] << std::endl;
    // }

    // Setting volume to 50% becuase my ears are still ringing
    sound.setVolume(5.0);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool is_playing = false;

    // Creating sfml clock variable
    // HACK: It immediately starts upon creation so we reset and stop it, upon creation to get it on 0
    sf::Clock clock;
    clock.reset();
    clock.stop();

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
    GLFWwindow* window = glfwCreateWindow(1280, 720, "My window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwSetKeyCallback(window, key_callback);
    // Make the windows context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable Vsync

    gladLoadGL(glfwGetProcAddress);

    UseImGui myimgui;
    myimgui.Init(window);
    ImPlot::CreateContext();



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
        {
            ImVec2 windowSize = ImVec2(400,400);
            ImVec2 button_size = ImVec2(100,100);

            // NOTE:
            // Creating an ImGui frame with ImGui::Begin
            // Parameter 1: is title of frame
            // Parameter 2: a bool to show window close or not... i think
            // Parameter 3: Setting window flags; use | to chain flags
            //              NoTitleBar
            //              NoResize = 2
            //              NoMove = 4
            //              NoBackground = 128
            ImGui::Begin("Hello, world!",__null,1|2|4|128);

            ImGui::SameLine(250.0);
            ImGui::Text("%.2f", floatToMinutes(elapsedTimeSeconds));
            // XXX: DO NOT TOUCH SLIDER, we just need it to visualize progress
            // NOTE: Slider Flags:
            //                      AlwaysClamp = 16

            // NOTE: %% will write a single % to the stream. Cants use escape character \ for % sign :(
            ImGui::SliderFloat(" ", &percentOfDuration, 0.0f,100.0f,"%.2f %%",16);
            ImGui::NewLine();
            ImGui::SameLine(200.0);

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

                if (ImGui::Button("Pause", button_size))
                {
                    is_playing = false;
                }
            }

            else if(!is_playing)
            {
                sound.pause();
                clock.stop();
                if (ImGui::Button("Play", button_size))
                {
                    is_playing = true;
                }
            }

            ImGui::End();
        }


        //NOTE: index reference table for buffer samples to time using the Samples per second
        //TODO: Change 4800 value to use the buffer Samples Per Second
        int index = int(clock.getElapsedTime().asSeconds() * 4800);

        static ScrollingBuffer sdata;
        sdata.AddPoint(clock.getElapsedTime().asSeconds(), buffer.getSamples()[index]);

        {
            ImGui::Begin("Spectograph");
            if (ImPlot::BeginPlot("Spectograph"))
            {

                ImPlot::PlotLine("Line",&sdata.Data[0].x,&sdata.Data[0].y,sdata.Data.size(),0,sdata.Offset,2*sizeof(float));
                ImPlot::EndPlot();
            }
            ImGui::End();

        }

        // for (int i = 0; i < index; i++)
        // {
        //     std::cout << i << ": " << buffer.getSamples()[i] << std::endl;
        // }

        // ImPlot::ShowDemoWindow();


        // {
        //     // TODO: To Fix
        //     // Render Spectograph RT
        //     ImGui::Begin("Spectograph");
        //     if (ImPlot::BeginPlot("Spectograph"))
        //     {
        //         int x_data[bufferFrame];
        //         int sampleSegment[bufferFrame];
        //
        //         for (int i = 0; i < bufferFrame; i++)
        //         {
        //             x_data[i] = i;
        //             sampleSegment[i] = buffer.getSamples()[i];
        //             std::cout << "i: " << i << '\t' << "samples: " << sampleSegment[i] << std::endl;
        //             // std::cout << sampleSegment << std::endl;
        //         }
        //         ImPlot::PlotLine("Line",x_data,sampleSegment,bufferFrame);
        //         ImPlot::EndPlot();
        //     }
        //     ImGui::End();
        // }

        // Rendering
        // (Your code clears your framebuffer, renders your other stuff etc.)
        glViewport(0,0,1280,720);
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
