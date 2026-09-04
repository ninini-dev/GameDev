#include <imgui.h>
#include <editor/editor.h>
#include <imgui_internal.h>
#include <own/enemy.h>
#include <own/bezier.h>
#include <string>

int pid = 0;
int len=1;

ImVec2 p0,p1,p2,p3; 

int setPoints() {
    p0 = ImVec2(enm_global_path_x[pid][0], enm_global_path_y[pid][0]);
    p1 = ImVec2(enm_global_path_x[pid][1], enm_global_path_y[pid][1]);
    p2 = ImVec2(enm_global_path_x[pid][2], enm_global_path_y[pid][2]);
    p3 = ImVec2(enm_global_path_x[pid][3], enm_global_path_y[pid][3]);
    return 1;
}
void screenPos(int j, ImVec2(&screen_pts)[4], ImVec2 ds,ImVec2 off=ImVec2(0,0)) {

    for (size_t i = 0; i < 4; i++)

        screen_pts[i] = ImVec2(enm_global_path_x[j][i]*ds.x+off.x, enm_global_path_y[j][i]*ds.y+off.y);

}
void ImGuiLoop() {
    // 1. Puntos de control en coordenadas normalizadas (0.0 a 1.0)
	static int initialized = setPoints();
    // 2. Dimensiones y entrada de ratón
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 ds = io.DisplaySize;
    ImVec2 mouse_pos = io.MousePos;
    bool mouse_down = ImGui::IsMouseDown(ImGuiMouseButton_Left);

    // Convertir de coordenadas normalizadas (0..1) a píxeles en pantalla
    ImVec2 screen_pts[4] = {};
    screenPos(pid, screen_pts, ds);

    // 3. Lógica para arrastrar los puntos con el ratón
    static int active_point = -1; // Guardar cuál punto se está arrastrando
    const float grab_radius = 12.0f; // Radio para detectar el clic en los nodos

    if (mouse_down) {
        if (active_point == -1) {
            // Si recién se hace clic, buscar el punto más cercano
            for (int i = 0; i < 4; i++) {
                float dx = mouse_pos.x - screen_pts[i].x;
                float dy = mouse_pos.y - screen_pts[i].y;
                if ((dx * dx + dy * dy) <= (grab_radius * grab_radius)) {
                    active_point = i;
                    break;
                }
            }
        }
        else {
            // Actualizar la posición del punto seleccionado según el movimiento del ratón
            float norm_x = ImClamp(mouse_pos.x / ds.x, 0.0f, 1.0f);
            float norm_y = ImClamp(mouse_pos.y / ds.y, 0.0f, 1.0f);

            switch (active_point) {
            case 0: p0 = ImVec2(norm_x, norm_y); break;
            case 1: p1 = ImVec2(norm_x, norm_y);
                if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
                    if (pid == 0)break;
                    enm_global_path_x[pid - 1][2] = enm_global_path_x[pid - 1][3] + p0.x - p1.x;
                    enm_global_path_y[pid - 1][2] = enm_global_path_y[pid - 1][3] + p0.y - p1.y;
                    //ImVec2(enm_global_path_x[pid][0], enm_global_path_y[pid][0])
                }

                break;
            case 2: p2 = ImVec2(norm_x, norm_y); 
                if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
                    if (pid == len -1 )break;
                    enm_global_path_x[pid + 1][1] = enm_global_path_x[pid + 1][0] + p3.x - p2.x;
                    enm_global_path_y[pid + 1][1] = enm_global_path_y[pid + 1][0] + p3.y - p2.y;
                }
                break;
            case 3: p3 = ImVec2(norm_x, norm_y); break;
            }
        }
    }
    else {
        active_point = -1; // Soltar el punto cuando no hay clic
    }

    // Ventana de control (sliders sincronizados)
    ImGui::Begin("Bezier Controls");
   
        if (ImGui::BeginTabBar("BarraDinamica#", ImGuiTabBarFlags_AutoSelectNewTabs)) {
           
            for (int i = 0; i < len; i++) {
                bool pestaña_abierta = true;
                std::string tab_name = "Pestaña Cerrable###Tab_" + std::to_string(i + 1);
                if (ImGui::BeginTabItem(tab_name.c_str(), &pestaña_abierta)) {
                    if (pid != i) {
                        pid = static_cast<int>(i);
                        setPoints();
                    }
                    ImGui::Text("Esta pestaña se puede cerrar.");
                    ImGui::EndTabItem();
                }
                if (!pestaña_abierta) {
                    memmove(&enm_global_path_x[i], &enm_global_path_x[i + 1], (len - i-1) * sizeof(enm_global_path_x[0]));
                    memmove(&enm_global_path_y[i], &enm_global_path_y[i + 1], (len - i-1) * sizeof(enm_global_path_y[0]));
                    len--;
                    setPoints();
                }
            }
        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing)) {
            // Código que se ejecuta al hacer clic en el '+'
            len++; // Aumenta la cantidad de pestañas
        }
        ImGui::EndTabBar();
    }

    ImGui::SliderFloat2("P0 (Inicio)", &p0.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("P1 (Ctrl 1)", &p1.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("P2 (Ctrl 2)", &p2.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("P3 (Fin)", &p3.x, 0.0f, 1.0f);
    ImGui::InputInt("Nombre de la Variable", &enm_global_path_length[pid]);
    ImGui::End();

    // 4. Renderizado en pantalla completa
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    // Líneas esqueleto
    draw_list->AddLine(screen_pts[0], screen_pts[1], IM_COL32(150, 150, 150, 180), 1.5f);
    draw_list->AddLine(screen_pts[2], screen_pts[3], IM_COL32(150, 150, 150, 180), 1.5f);

    // Curva Bézier
    int ps=0, pe= 0;

    ImVec2 sPos = ImVec2(0, 0);
    while (enm_global_path_length[pid - 1 - ps] > enm_global_path_length[pid - ps]) {
        ps--;
    }
    while (enm_global_path_length[pid + 1 + pe] < enm_global_path_length[pid + pe]) {
        pe++;
    }
    
    for (int i = ps; i <= pe; i++)
    {
        screenPos(pid+i, screen_pts, ds,sPos);
        draw_list->AddBezierCubic(screen_pts[0], screen_pts[1], screen_pts[2], screen_pts[3], 
            !i?IM_COL32(255, 215, 0, 255): IM_COL32(255, 0, 0, 255)
            , 4.0f);
		sPos = screen_pts[3];
    }
    // Nodos de control (cambian de tamaño al interactuar)

    screenPos(pid, screen_pts, ds);
    for (int i = 0; i < 4; i++) {
        ImU32 color = (i == 0 || i == 3) ? IM_COL32(255, 50, 50, 255) : IM_COL32(50, 255, 50, 255);
        float radius = (active_point == i) ? 8.0f : 5.0f;

        draw_list->AddCircleFilled(screen_pts[i], radius, color);
    }
	enm_global_path_x[pid][0] = p0.x;
    enm_global_path_y[pid][0] = p0.y;
    enm_global_path_x[pid][1] = p1.x;
    enm_global_path_y[pid][1] = p1.y;
    enm_global_path_x[pid][2] = p2.x;
    enm_global_path_y[pid][2] = p2.y;
    enm_global_path_x[pid][3] = p3.x;
    enm_global_path_y[pid][3] = p3.y;

}