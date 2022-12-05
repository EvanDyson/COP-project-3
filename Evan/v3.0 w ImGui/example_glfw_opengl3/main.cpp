// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <stdlib.h>
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>


// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    //GLFWwindow* window = glfwCreateWindow(1920, 1080, "Testing ImGui", glfwGetPrimaryMonitor(), NULL);
    
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Testing ImGui", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMaximizeWindow(window);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    io.ConfigFlags |= ImGuiWindowFlags_NoMove;


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 6.0f;
        style.FrameRounding = 6.0f;
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.60f);
        //style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.0f, 0.0f, 1.0f);
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    bool ret = LoadTextureFromFile("century tower.png", &my_image_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
    //loading tahoma font
    io.Fonts->AddFontFromFileTTF("../../misc/fonts/tahoma.ttf", 30.0f);

    // Our state
    bool show_demo_window = false;
    bool show_loading = false;
    bool show_results = false;
    bool loadImage = true;
    bool testingmer = false;
    bool testingrad = false;
    
    //BACKGROUND COLOR
    ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    int population_, size_, avgHousingCost_, avgIncome_;
    float density_;
    int i1 = 0, i2 = 0, i4 = 0, i5 = 0;
    float i3 = 0.00f;
    static int e = 0;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        
        static bool no_titlebar = true;
        static bool no_scrollbar = true;
        static bool no_menu = true;
        static bool no_move = true;
        static bool no_resize = true;
        static bool no_collapse = true;
        static bool no_close = false;
        static bool no_nav = false;
        static bool no_background = false;
        static bool no_bring_to_front = true;
        static bool no_docking = true;
        static bool unsaved_document = false;

        ImGuiWindowFlags window_flags = 0;
        if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
        if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
        if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
        if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
        if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
        if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
        if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
        if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
        if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        if (no_docking)         window_flags |= ImGuiWindowFlags_NoDocking;
        if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
        
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            // load a background image
            if (loadImage)
            {
                static bool use_work_area = true;
                static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
                ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
                ImGui::Begin("Image", &show_loading, flags);
                ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));
                ImGui::End();
            }

            static float f = 0.0f;
            static int counter = 0;

            const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 320, main_viewport->WorkPos.y + 140), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(1280, 800), ImGuiCond_Once);
            

            ImGui::Begin("Getting user input", NULL, window_flags);                          // Create a window called "Hello, world!" and append into it.
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::Button("Exit"))
                    return EXIT_SUCCESS;
                
                ImGui::Checkbox("Demo Window", &show_demo_window);
                //ImGui::Checkbox("Picture", &show_loading);
                ImGui::EndMainMenuBar();
            }
            //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

            // i1 - i5 will contain all of the user inputs

            ImGui::Text("");
            ImGui::Text("  Welcome to City Rankers!");
            ImGui::Text("");
            ImGui::Text("      Please slide the sliders to your desired input!");
            ImGui::Text("      Leave the slider at 0 if you do not wish to use that input.");
            ImGui::Text("");
            ImGui::Text("                ");
            ImGui::SameLine();
            ImGui::SliderInt("Population", &i1, 500, 4800000);
            ImGui::Text("");
            ImGui::Text("                ");
            ImGui::SameLine();
            ImGui::SliderInt("City Size", &i2, 10, 1400);
            ImGui::Text("");
            ImGui::Text("                ");
            ImGui::SameLine();
            ImGui::SliderFloat("Population density", &i3, 0.48f, 400000);
            ImGui::Text("");
            ImGui::Text("                ");
            ImGui::SameLine();
            ImGui::SliderInt("Avg. income", &i4, 10000, 950000);
            ImGui::Text("");
            ImGui::Text("                ");
            ImGui::SameLine();
            ImGui::SliderInt("Avg. housing costs", &i5, 10000, 2400000);

            population_ = i1;
            size_ = i2;
            avgHousingCost_ = i5;
            avgIncome_ = i4;
            density_ = i3;
            
            ImGui::SetCursorPos(ImVec2(main_viewport->WorkPos.x + 135, main_viewport->WorkPos.y + 540));
            if (ImGui::Button("Load Results"))
                show_loading = true;
            
            ImGui::SetCursorPos(ImVec2(main_viewport->WorkPos.x + 135, main_viewport->WorkPos.y + 630));
            if (ImGui::Button("Reset inputs"))
            {
                i1 = 0, i2 = 0, i4 = 0, i5 = 0;
                i3 = 0.0f;
            }

            ImGui::SetCursorPos(ImVec2(main_viewport->WorkPos.x + 985, main_viewport->WorkPos.y + 540));
            ImGui::RadioButton("Merge Sort", &e, 0);
            ImGui::SetCursorPos(ImVec2(main_viewport->WorkPos.x + 985, main_viewport->WorkPos.y + 630));
            ImGui::RadioButton("Radix Sort", &e, 1);
            //MERGE SORT = 1
            //RADIX SORT = 2
            
            ImGui::End();
        }
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        if (show_loading)
        {
            const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 640, main_viewport->WorkPos.y + 300), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_Once);
            ImGui::Begin("Loading...", &show_loading);
            ImGui::Text("The user has selected...");
            ImGui::Text("population = %i", population_);
            ImGui::Text("size = %i", size_);
            ImGui::Text("density = %f", density_);
            ImGui::Text("income = %i", avgIncome_);
            ImGui::Text("housing = %i", avgHousingCost_);
            std::string temp = "";
            if (e == 0)
                ImGui::Text("button selected = merge sort");
            else if (e == 1)
                ImGui::Text("button selected = radix sort");
            
            ImGui::End();
        }
        if (show_results)
        {
            ImGui::Begin("Result Window", &show_results);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Results!");
            if (ImGui::Button("Close Me"))
                show_results = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    	
        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
