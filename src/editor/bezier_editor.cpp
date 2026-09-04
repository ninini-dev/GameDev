#include <imgui.h>
#include <editor/editor.h>
#include <imgui_internal.h>
#include <own/enemy.h>
void ImGuiLoop() {
    // 1. Puntos de control en coordenadas normalizadas (0.0 a 1.0)
    static ImVec2 p0 = ImVec2(0.1f, 0.8f);
    static ImVec2 p1 = ImVec2(0.2f, 0.2f);
    static ImVec2 p2 = ImVec2(0.8f, 0.2f);
    static ImVec2 p3 = ImVec2(0.9f, 0.8f);

    // 2. Dimensiones y entrada de ratón
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 display_size = io.DisplaySize;
    ImVec2 mouse_pos = io.MousePos;
    bool mouse_down = ImGui::IsMouseDown(ImGuiMouseButton_Left);

    // Convertir de coordenadas normalizadas (0..1) a píxeles en pantalla
    ImVec2 screen_pts[4] = {
        ImVec2(p0.x * display_size.x, p0.y * display_size.y),
        ImVec2(p1.x * display_size.x, p1.y * display_size.y),
        ImVec2(p2.x * display_size.x, p2.y * display_size.y),
        ImVec2(p3.x * display_size.x, p3.y * display_size.y)
    };

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
            float norm_x = ImClamp(mouse_pos.x / display_size.x, 0.0f, 1.0f);
            float norm_y = ImClamp(mouse_pos.y / display_size.y, 0.0f, 1.0f);

            switch (active_point) {
            case 0: p0 = ImVec2(norm_x, norm_y); break;
            case 1: p1 = ImVec2(norm_x, norm_y); break;
            case 2: p2 = ImVec2(norm_x, norm_y); break;
            case 3: p3 = ImVec2(norm_x, norm_y); break;
            }
        }
    }
    else {
        active_point = -1; // Soltar el punto cuando no hay clic
    }

    // Ventana de control (sliders sincronizados)
    ImGui::Begin("Bezier Controls");
    ImGui::SliderFloat2("P0 (Inicio)", &p0.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("P1 (Ctrl 1)", &p1.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("P2 (Ctrl 2)", &p2.x, 0.0f, 1.0f);
    ImGui::SliderFloat2("P3 (Fin)", &p3.x, 0.0f, 1.0f);
    ImGui::End();

    // 4. Renderizado en pantalla completa
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    // Líneas esqueleto
    draw_list->AddLine(screen_pts[0], screen_pts[1], IM_COL32(150, 150, 150, 180), 1.5f);
    draw_list->AddLine(screen_pts[2], screen_pts[3], IM_COL32(150, 150, 150, 180), 1.5f);

    // Curva Bézier
    draw_list->AddBezierCubic(screen_pts[0], screen_pts[1], screen_pts[2], screen_pts[3], IM_COL32(255, 215, 0, 255), 4.0f);

    // Nodos de control (cambian de tamaño al interactuar)
    for (int i = 0; i < 4; i++) {
        ImU32 color = (i == 0 || i == 3) ? IM_COL32(255, 50, 50, 255) : IM_COL32(50, 255, 50, 255);
        float radius = (active_point == i) ? 8.0f : 5.0f;
        draw_list->AddCircleFilled(screen_pts[i], radius, color);
    }
	Enemy::b_x[0] = p0.x;
	Enemy::b_y[0] = p0.y;
	Enemy::b_x[1] = p1.x;
	Enemy::b_y[1] = p1.y;
	Enemy::b_x[2] = p2.x;
	Enemy::b_y[2] = p2.y;
	Enemy::b_x[3] = p3.x;
	Enemy::b_y[3] = p3.y;

}