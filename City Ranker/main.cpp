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
#ifndef IMGUI_CDECL
#ifdef _MSC_VER
#define IMGUI_CDECL __cdecl
#else
#define IMGUI_CDECL
#endif
#endif
#include "stb_image.h"
#include <string>
#include <chrono>
#include "CityRanker.h"

namespace
{
    enum MyItemColumnID
    {
        MyItemColumnID_ID,
        /*
        MyItemColumnID_Name,
        MyItemColumnID_Action,
        MyItemColumnID_Quantity,
        MyItemColumnID_Description*/
        MyItemColumnID_Name,
        MyItemColumnID_population,
        MyItemColumnID_size,
        MyItemColumnID_density,
        MyItemColumnID_avgHousingCost,
        MyItemColumnID_avgIncome,
        MyItemColumnID_weightScore
    };

    struct MyItem
    {
        int         ID;
        //const char* Name;
        //int         Quantity;
        const char* name;
        int population;
        int size;
        double density;
        int avgHousingCost;
        int avgIncome;
        int weightScore;

        // We have a problem which is affecting _only this demo_ and should not affect your code:
        // As we don't rely on std:: or other third-party library to compile dear imgui, we only have reliable access to qsort(),
        // however qsort doesn't allow passing user data to comparing function.
        // As a workaround, we are storing the sort specs in a static/global for the comparing function to access.
        // In your own use case you would probably pass the sort specs to your sorting/comparing functions directly and not use a global.
        // We could technically call ImGui::TableGetSortSpecs() in CompareWithSortSpecs(), but considering that this function is called
        // very often by the sorting algorithm it would be a little wasteful.
        static const ImGuiTableSortSpecs* s_current_sort_specs;

        // Compare function to be used by qsort()
        static int IMGUI_CDECL CompareWithSortSpecs(const void* lhs, const void* rhs)
        {
            const MyItem* a = (const MyItem*)lhs;
            const MyItem* b = (const MyItem*)rhs;
            for (int n = 0; n < s_current_sort_specs->SpecsCount; n++)
            {
                // Here we identify columns using the ColumnUserID value that we ourselves passed to TableSetupColumn()
                // We could also choose to identify columns based on their index (sort_spec->ColumnIndex), which is simpler!
                const ImGuiTableColumnSortSpecs* sort_spec = &s_current_sort_specs->Specs[n];
                int delta = 0;
                switch (sort_spec->ColumnUserID)
                {
                    /*case MyItemColumnID_ID:             delta = (a->ID - b->ID);                break;
                    case MyItemColumnID_Name:           delta = (strcmp(a->Name, b->Name));     break;
                    case MyItemColumnID_Quantity:       delta = (a->Quantity - b->Quantity);    break;
                    case MyItemColumnID_Description:    delta = (strcmp(a->Name, b->Name));     break;*/
                case MyItemColumnID_ID:             delta = (a->ID - b->ID);                break;
                case MyItemColumnID_Name:           delta = (strcmp(a->name, b->name));     break;
                case MyItemColumnID_population:     delta = (a->population - b->population);    break;
                case MyItemColumnID_size:           delta = (a->size - b->size);            break;
                case MyItemColumnID_density:        delta = (a->density - b->density);      break;
                case MyItemColumnID_avgHousingCost: delta = (a->avgHousingCost - b->avgHousingCost);    break;
                case MyItemColumnID_avgIncome:      delta = (a->avgIncome - b->avgIncome);  break;
                case MyItemColumnID_weightScore:    delta = (a->weightScore - b->weightScore);  break;

                default: IM_ASSERT(0); break;
                }
                if (delta > 0)
                    return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? +1 : -1;
                if (delta < 0)
                    return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? -1 : +1;
            }

            // qsort() is instable so always return a way to differenciate items.
            // Your own compare function may want to avoid fallback on implicit sort specs e.g. a Name compare if it wasn't already part of the sort specs.
            return (a->ID - b->ID);
        }
    };
    const ImGuiTableSortSpecs* MyItem::s_current_sort_specs = NULL;
}

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
        style.FrameRounding = 7.0f;
        style.GrabRounding = 15.0f;
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.7f);
        //style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.0f, 0.0f, 1.0f);
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);



    //loading my background image
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    bool ret = LoadTextureFromFile("Tower.jpg", &my_image_texture, &my_image_width, &my_image_height);
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
    io.Fonts->AddFontFromFileTTF("../../misc/fonts/tahoma.ttf", 29.5f);






    // Our state
    bool show_demo_window = false;
    bool show_loading = false;
    bool show_results = false;
    bool backgroundImage = true;
    bool userInput = true;
    bool scannedYet = false;
    bool showwhatuinputted = false;
    
    //BACKGROUND COLOR
    ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    int i1 = 0, i2 = 0, i4 = 0, i5 = 0;
    float i3 = 0.00f;
    int tableSize = 15;
    static int e = 0;
    long long int mergeTime = 0, radixTime = 0;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

    //setting up our merge and radix vectors

    vector<City*> mergeVector;
    vector<City*> radixVector;


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

        ImGuiWindowFlags resultWindow = 0;
        if (no_titlebar)        resultWindow |= ImGuiWindowFlags_NoTitleBar;
        if (no_scrollbar)       resultWindow |= ImGuiWindowFlags_NoScrollbar;
        if (!no_menu)           resultWindow |= ImGuiWindowFlags_MenuBar;
        if (no_move)            resultWindow |= ImGuiWindowFlags_NoMove;
        if (no_resize)          resultWindow |= ImGuiWindowFlags_NoResize;
        if (no_collapse)        resultWindow |= ImGuiWindowFlags_NoCollapse;
        if (no_nav)             resultWindow |= ImGuiWindowFlags_NoNav;
        if (no_background)      resultWindow |= ImGuiWindowFlags_NoBackground;
        if (no_bring_to_front)  resultWindow |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        if (no_docking)         resultWindow |= ImGuiWindowFlags_NoDocking;
        if (unsaved_document)   resultWindow |= ImGuiWindowFlags_UnsavedDocument;

        static ImGuiTableFlags tableFlags =
            ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollY;
        
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            // load a background image
            if (backgroundImage)
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

            if (userInput)
            {
                ImGui::Begin("Getting user input", NULL, window_flags);
                if (ImGui::BeginMainMenuBar())
                {
                    ImGui::Text(" ");
                    if (ImGui::Button("Exit"))
                        return EXIT_SUCCESS;

                    //ImGui::Checkbox("Demo Window", &show_demo_window);
                    ImGui::EndMainMenuBar();
                }
                //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

                // i1 - i5 will contain all of the user inputs

                ImGui::Text("");
                ImGui::Text("  Welcome to City Rankers!");
                ImGui::Text("");
                ImGui::Text("      Please move the sliders to your desired input!");
                ImGui::Text("      Leave the slider at 0 if you do not wish to use that input.");
                ImGui::Text("      Select which sort you would like to use on the bottom right!");
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

                ImGui::SetCursorPos(ImVec2(main_viewport->WorkPos.x + 135, main_viewport->WorkPos.y + 555));
                if (ImGui::Button("Load Results") && e != 0)
                    show_results = true;

                ImGui::SetCursorPos(ImVec2(main_viewport->WorkPos.x + 135, main_viewport->WorkPos.y + 645));
                if (ImGui::Button("Reset inputs"))
                {
                    i1 = 0, i2 = 0, i4 = 0, i5 = 0;
                    i3 = 0.0f;
                    e = 0;
                    bool scannedYet = false;
                }

                ImGui::SetCursorPos(ImVec2(main_viewport->WorkPos.x + 985, main_viewport->WorkPos.y + 555));
                ImGui::RadioButton("Merge Sort", &e, 1);
                ImGui::SetCursorPos(ImVec2(main_viewport->WorkPos.x + 985, main_viewport->WorkPos.y + 645));
                ImGui::RadioButton("Radix Sort", &e, 2);
                //MERGE SORT = 1
                //RADIX SORT = 2

                ImGui::End();
            }

            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            //test loading window may not do this anymore
            if (show_loading)
            {
                const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 640, main_viewport->WorkPos.y + 300), ImGuiCond_Once);
                ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_Once);
                ImGui::Begin("Loading...", &show_loading);
                ImGui::Text("The user has selected...");
                ImGui::Text("population = %i", i1);
                ImGui::Text("size = %i", i2);
                ImGui::Text("density = %f", i3);
                ImGui::Text("income = %i", i4);
                ImGui::Text("housing = %i", i5);

                if (e == 1)
                    ImGui::Text("button selected = merge sort");
                else if (e == 2)
                    ImGui::Text("button selected = radix sort");

                ImGui::End();
            }

            //showing result window
            if (show_results)
            {
                const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 320, main_viewport->WorkPos.y + 140), ImGuiCond_Once);
                ImGui::SetNextWindowSize(ImVec2(1280, 800), ImGuiCond_Once);
                ImGui::SetNextWindowBgAlpha(0.80f);
                ImGui::Begin("Results", &show_results, resultWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                if (ImGui::BeginMainMenuBar())
                {
                    if (ImGui::Button("Exit"))
                        return EXIT_SUCCESS;

                    //ImGui::Checkbox("Demo Window", &show_demo_window);
                    ImGui::EndMainMenuBar();
                }
                //this closes the input window while the result window is open
                userInput = false;

                if (!scannedYet && e == 1)
                {
                    auto begin = std::chrono::high_resolution_clock::now();                    
                    mergeVector = CityRankers(e, i1, i2, i3, i4, i5, userInput, scannedYet);
                    auto end = std::chrono::high_resolution_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
                    mergeTime = elapsed.count();
                    scannedYet = true;
                    //cout << "Merge sort took " << elapsed.count() * 1e-9 << " seconds" << endl << endl << endl;
                }
                else if (!scannedYet && e == 2)
                {
                    auto begin = std::chrono::high_resolution_clock::now();
                    radixVector = CityRankers(e, i1, i2, i3, i4, i5, userInput, scannedYet);
                    auto end = std::chrono::high_resolution_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
                    radixTime = elapsed.count();
                    scannedYet = true;
                    //cout << "Merge sort took " << elapsed.count() * 1e-9 << " seconds" << endl << endl << endl;
                }
                if (e == 1)
                {
                    //creating my table to display results
                    //resizing the vector to be smaller for viewing purposes.
                    //mergeVector.resize(50);
                    ImGui::Text("How many cities would you like to view?");
                    ImGui::SliderInt(" ", &tableSize, 15, 100000);
                    ImGui::Text("");
                    if (ImGui::BeginTable("table1", 7, tableFlags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 100), 0.0f))
                    {
                        ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
                        ImGui::TableSetupColumn("City", ImGuiTableColumnFlags_WidthFixed, 200.0f);
                        ImGui::TableSetupColumn("Population", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                        ImGui::TableSetupColumn("Density", ImGuiTableColumnFlags_WidthFixed, 170.0f);
                        ImGui::TableSetupColumn("Avg. Housing Cost", ImGuiTableColumnFlags_WidthFixed, 250.0f);
                        ImGui::TableSetupColumn("Avg. Income", ImGuiTableColumnFlags_WidthFixed, 170.0f);
                        ImGui::TableSetupColumn("Score",  ImGuiTableColumnFlags_WidthFixed, 200.0f);
                        ImGui::TableHeadersRow();

                        ImGuiListClipper clipper;
                        clipper.Begin(tableSize);
                        while (clipper.Step())
                        {
                            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                            {
                                ImGui::TableNextRow();
                                for (int column = 0; column < 7; column++)
                                {
                                    ImGui::TableSetColumnIndex(column);
                                    //ImGui::Text("Hello %d,%d", column, row);
                                    if (column == 0)
                                    {
                                        ImGui::Text("%s", mergeVector[row]->name);
                                    }
                                    else if (column == 1)
                                    {
                                        ImGui::Text("%i", mergeVector[row]->population);
                                    }
                                    else if (column == 2)
                                    {
                                        ImGui::Text("%i", mergeVector[row]->size);
                                    }
                                    else if (column == 3)
                                    {
                                        ImGui::Text("%i", mergeVector[row]->getDensity());
                                    }
                                    else if (column == 4)
                                    {
                                        ImGui::Text("%i", mergeVector[row]->avgHousingCost);
                                    }
                                    else if (column == 5)
                                    {
                                        ImGui::Text("%i", mergeVector[row]->avgIncome);
                                    }
                                    else if (column == 6)
                                    {
                                        ImGui::Text("%i", mergeVector[row]->weightScore);
                                    }
                                }
                            }
                        }
                        ImGui::EndTable();
                    }
                    if (showwhatuinputted)
                    {
                        ImGui::Text("Population = %i", i1);
                        ImGui::SameLine(0.0f, 20.0f); ImGui::Text("Size = %i", i2);
                        ImGui::SameLine(0.0f, 20.0f); ImGui::Text("Density = %f", i3);
                        ImGui::SameLine(0.0f, 20.0f); ImGui::Text("Avg. housing = %i", i5);
                        ImGui::SameLine(0.0f, 20.0f); ImGui::Text("Avg. income = %i", i4);
                    }
                    else
                        ImGui::Text("");
                    ImGui::Text("");
                    ImGui::Text("Your merge sort took %i milliseconds to sort.", mergeTime);
                }
                else if (e == 2)
                {
                    //creating my table to display results
                    //resizing the vector to be smaller for viewing purposes.
                    //mergeVector.resize(50);
                    ImGui::Text("How many cities would you like to view?");
                    ImGui::SliderInt(" ", &tableSize, 15, 100000);
                    ImGui::Text("");
                    if (ImGui::BeginTable("table1", 7, tableFlags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 100), 0.0f))
                    {
                        ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
                        ImGui::TableSetupColumn("City", ImGuiTableColumnFlags_WidthFixed, 200.0f);
                        ImGui::TableSetupColumn("Population", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                        ImGui::TableSetupColumn("Density", ImGuiTableColumnFlags_WidthFixed, 170.0f);
                        ImGui::TableSetupColumn("Avg. Housing Cost", ImGuiTableColumnFlags_WidthFixed, 250.0f);
                        ImGui::TableSetupColumn("Avg. Income", ImGuiTableColumnFlags_WidthFixed, 170.0f);
                        ImGui::TableSetupColumn("Score", ImGuiTableColumnFlags_WidthFixed, 200.0f);
                        ImGui::TableHeadersRow();

                        ImGuiListClipper clipper;
                        clipper.Begin(tableSize);
                        while (clipper.Step())
                        {
                            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                            {
                                ImGui::TableNextRow();
                                for (int column = 0; column < 7; column++)
                                {
                                    ImGui::TableSetColumnIndex(column);
                                    //ImGui::Text("Hello %d,%d", column, row);
                                    if (column == 0)
                                    {
                                        ImGui::Text("%s", radixVector[row]->name);
                                    }
                                    else if (column == 1)
                                    {
                                        ImGui::Text("%i", radixVector[row]->population);
                                    }
                                    else if (column == 2)
                                    {
                                        ImGui::Text("%i", radixVector[row]->size);
                                    }
                                    else if (column == 3)
                                    {
                                        ImGui::Text("%i", radixVector[row]->getDensity());
                                    }
                                    else if (column == 4)
                                    {
                                        ImGui::Text("%i", radixVector[row]->avgHousingCost);
                                    }
                                    else if (column == 5)
                                    {
                                        ImGui::Text("%i", radixVector[row]->avgIncome);
                                    }
                                    else if (column == 6)
                                    {
                                        ImGui::Text("%i", radixVector[row]->weightScore);
                                    }
                                }
                            }
                        }
                        ImGui::EndTable();
                    }
                    if (showwhatuinputted)
                    {
                        ImGui::Text("Population = %i", i1);
                        ImGui::SameLine(0.0f, 20.0f); ImGui::Text("Size = %i", i2);
                        ImGui::SameLine(0.0f, 20.0f); ImGui::Text("Density = %f", i3);
                        ImGui::SameLine(0.0f, 20.0f); ImGui::Text("Avg. housing = %i", i5);
                        ImGui::SameLine(0.0f, 20.0f); ImGui::Text("Avg. income = %i", i4);
                    }
                    else
                        ImGui::Text("");
                    ImGui::Text("");
                    ImGui::Text("Radix sort took %i milliseconds to sort.", radixTime);
                }
                ImGui::Text("");
                ImGui::Checkbox("User Inputs", &showwhatuinputted);
                
                ImGui::Text("");
                if (ImGui::Button("Go back to inputs"))
                {
                    i1 = 0, i2 = 0, i4 = 0, i5 = 0;
                    i3 = 0.0f;
                    e = 0;
                    tableSize = 15;
                    show_results = false;
                    userInput = true;
                    scannedYet = false;
                    showwhatuinputted = false;
                }
                ImGui::End();
            }

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
