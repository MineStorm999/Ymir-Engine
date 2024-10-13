#include "TransformGizmo.h"
#include "Imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
#include "ECS/ECSManager.h"
#include "MathLib/ml.h"
/*
void TransformGizmo::Show(entt::entity e)
{
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
    TransformComponent* t = EntityManager::GetWorld().try_get<TransformComponent>(e);
    if (!t) {
        return;
    }
    if (!ImGui::Begin("Gizmo")) {
        return;
    }
    
    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_T))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_R))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_F)) // r Key
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    float matrixTranslation[3], matrixRotation[3], matrixScale[3];

    ImGuizmo::DecomposeMatrixToComponents(&t->localMat, matrixTranslation, matrixRotation, matrixScale);

    ImGui::InputFloat3("Tr", matrixTranslation, (ImGuiInputTextFlags)3);
    ImGui::InputFloat3("Rt", matrixRotation, (ImGuiInputTextFlags)3);
    ImGui::InputFloat3("Sc", matrixScale, (ImGuiInputTextFlags)3);
    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, &t->localMat);

    if (mCurrentGizmoOperation != ImGuizmo::SCALE)
    {
        if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
            mCurrentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
            mCurrentGizmoMode = ImGuizmo::WORLD;
    }
    static bool useSnap(false);
    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_G))
        useSnap = !useSnap;
    ImGui::Checkbox("", &useSnap);
    ImGui::SameLine();
    vec_t snap;
    switch (mCurrentGizmoOperation)
    {
    case ImGuizmo::TRANSLATE:
        snap = config.mSnapTranslation;
        ImGui::InputFloat3("Snap", &snap.x);
        break;
    case ImGuizmo::ROTATE:
        snap = config.mSnapRotation;
        ImGui::InputFloat("Angle Snap", &snap.x);
        break;
    case ImGuizmo::SCALE:
        snap = config.mSnapScale;
        ImGui::InputFloat("Scale Snap", &snap.x);
        break;
    }
    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    //ImGuizmo::Manipulate(camera.mView.m16, camera.mProjection.m16, mCurrentGizmoOperation, mCurrentGizmoMode, matrix.m16, NULL, useSnap ? &snap.x : NULL);
}*/



