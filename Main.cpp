#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::vector<std::pair<std::string, std::string>> kms_keys = {
    {"Windows 11 Pro", "W269N-WFGWX-YVC9B-4J6C9-T83GX"},
    {"Windows 11 Pro N", "MH37W-N47XK-V7XM9-C7227-GCQG9"},
    {"Windows 11 Pro for Workstations", "NRG8B-VKK3Q-CXVCJ-9G2XF-6Q84J"},
    {"Windows 11 Pro for Workstations N", "9FNHH-K3HBT-3W4TD-6383H-6XYWF"},
    {"Windows 11 Pro Education", "6TP4R-GNPTD-KYYHQ-7B7DP-J447Y"},
    {"Windows 11 Pro Education N", "YVWGF-BXNMC-HTQYQ-CPQ99-66QFC"},
    {"Windows 11 Education", "NW6C2-QMPVW-D7KKK-3GKT6-VCFB2"},
    {"Windows 11 Education N", "2WH4N-8QGBV-H22JP-CT43Q-MDWWJ"},
    {"Windows 11 Enterprise", "NPPR9-FWDCX-D2C8J-H872K-2YT43"},
    {"Windows 11 Enterprise N", "DPH2V-TTNVB-4X9Q3-TJR4H-KHJW4"},
    {"Windows 11 Enterprise G", "YYVX9-NTFWV-6MDM3-9PT4T-4M68B"},
    {"Windows 11 Enterprise G N", "44RPN-FTY23-9VTTB-MP9BX-T84FV"}
};

std::vector<std::string> kms_servers = {
    "kms.lotro.cc",
    "mhd.kmdns.net110",
    "noip.me",
    "45.78.3.223",
    "kms.didichuxing.com",
    "zh.us.to",
    "toxykz.f3322.org",
    "192.168.2.81",
    "kms.guowaifuli.com",
    "106.186.25.239",
    "rss.vicp.net:20439",
    "122.226.152.230",
    "222.76.251.188",
    "annychen.pw",
    "heu168.6655.la",
    "kms.aglc.cc",
    "kms.landiannews.com",
    "kms.xspace.in",
    "winkms.tk",
    "kms7.MSGuides.com",
    "kms8.MSGuides.com",
    "kms9.MSGuides.com"
};

bool activation_complete = false;
bool show_restart_buttons = false;

void run_activation_commands(const std::string& selected_key, const std::string& selected_server) {
    std::string command1 = "slmgr /ipk " + selected_key;
    std::string command2 = "slmgr /skms " + selected_server;
    std::string command3 = "slmgr /ato";

    system("timeout /t 2 >null | cls");
    system(command1.c_str());
    system(command2.c_str());
    system(command3.c_str());

    activation_complete = true;
    show_restart_buttons = true;
}

void apply_activation(const std::string& selected_version) {
    auto it = std::find_if(kms_keys.begin(), kms_keys.end(), [&](const std::pair<std::string, std::string>& pair) {
        return pair.first == selected_version;
        });

    if (it != kms_keys.end()) {
        std::string selected_key = it->second;
        std::string selected_server = kms_servers[rand() % kms_servers.size()];

        
        std::thread activation_thread(run_activation_commands, selected_key, selected_server);
        activation_thread.detach();
    }
    else {
        std::cout << "Version de Windows non prise en charge." << std::endl;
    }
}

int APIENTRY WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow) {
    
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); 

    GLFWwindow* window = glfwCreateWindow(500, 150, "ZitoActivator", NULL, NULL);

    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    srand(static_cast<unsigned int>(time(0)));

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        ImGui::SetNextWindowSize(ImVec2((float)windowWidth, (float)windowHeight), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::Begin("ZitoActivator", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

        if (!show_restart_buttons) {
            static int selected_version = 0;
            if (ImGui::BeginCombo("Select Windows Version", kms_keys[selected_version].first.c_str())) {
                for (int i = 0; i < kms_keys.size(); i++) {
                    bool is_selected = (selected_version == i);
                    if (ImGui::Selectable(kms_keys[i].first.c_str(), is_selected))
                        selected_version = i;
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (ImGui::Button("Activate")) {
                apply_activation(kms_keys[selected_version].first);
            }

            if (ImGui::Button("Join Discord")) {
                system("start https://discord.gg/2NujnKbZdY");
            }
        }
        else {
            ImGui::Text("Activation Complete. Please Restart Your Computer.");
            if (ImGui::Button("Restart Now")) {
                system("del null");
                system("shutdown -r -t 5 -c \"Thanks Using ZitoActivator\"");
            }
            if (ImGui::Button("Restart Later")) {
                show_restart_buttons = false;
                system("del null");
                system("timeout /t 2 > nul");
            }
        }

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
