#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "app/viewer.h"
#include "app/viewer_ui.h"


ViewerUi::ViewerUi(core::Window& window, Viewer& viewer) :
    Ui{ window }, mViewer{ viewer}
{
}

void ViewerUi::update()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoBackground;



    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Rendering Options Menu")) {
                mViewer.mViewerOpen = !(mViewer.mViewerOpen);
            }
            ImGui::EndMenu();
        }

        /*if (mViewer.mNormalShadingMode) {
            ImGui::Text("SHADING MODE (M): Normal shading\n");
        }
        else {
            ImGui::Text("SHADING MODE (M): Phong shading\n");
        }*/

        ImGui::EndMainMenuBar();
    }

    if (mViewer.mViewerOpen) {

        ImGui::Begin("Rendering Options");

     

    
        if (ImGui::Checkbox("Rasterize Mode", &mViewer.mRasterizeMode)) {
            if (mViewer.mRasterizeMode) {
                mViewer.mRayTraceMode = false;
                mViewer.mHybridMode = false;
            }
        }
        if (ImGui::Checkbox("Ray Trace Mode", &mViewer.mRayTraceMode)) {
            if (mViewer.mRayTraceMode) {
                mViewer.mRasterizeMode = false;
                mViewer.mHybridMode = false;

            }
        }
        if (ImGui::Checkbox("Hybrid Mode: Rasterize + Raytrace ", &mViewer.mHybridMode)) {
            if (mViewer.mHybridMode) {
                mViewer.mRasterizeMode = false;
                mViewer.mRayTraceMode = false;
            }
        }

  


        if (ImGui::CollapsingHeader("Parameters", ImGuiTreeNodeFlags_DefaultOpen))
        {

            ImGui::Checkbox("Environment Map", &mViewer.mEnvMapToggle);

            if (mViewer.mRayTraceMode || mViewer.mHybridMode) {
                if (ImGui::SliderInt("Num Bounces", &mViewer.mNumRayBounces, 0, 10)) {
                    mViewer.mRayTraceShader.use();
                    mViewer.mRayTraceShader.setUniformInt("numBounces", mViewer.mNumRayBounces);
                }
            }
            if (ImGui::SliderInt("Num Spheres", &mViewer.mNumSpheresDisplayed, 1, mViewer.MAX_NUM_SPHERES)) {
                mViewer.mRayTraceShader.use();
                mViewer.mRayTraceShader.setUniformInt("numSpheresDisplayed", mViewer.mNumSpheresDisplayed);
            }

        }

       

        

        ImGui::End();
    }
}
