#ifndef VIEWER_H
#define VIEWER_H

#include <memory>

#include "core/app.h"
#include "core/shader.h"
#include "core/mesh.h"
#include "core/asset_loader.h"
#include "core/camera.h"
#include "core/timer.h"
#include "math/matrix.h"

#include "skybox.h"
#include "trace.h"


class Viewer : public core::App {
public:
	Viewer();
	~Viewer() override;
	void init() override;
	void update() override;
	void render() override;


	void rebuildSpheres();


	//State variables
	bool mViewerOpen;
	bool mMousePressed;
	bool mMouseInit;
	float mMouseX;
	float mMouseY;
	bool mEnvMapToggle;
	int mNumSpheresDisplayed;
	int mNumRayBounces;
	bool mRasterizeMode;
	bool mRayTraceMode;
	bool mHybridMode;


	const int MAX_NUM_SPHERES = 100;

	core::Timer mTimer;


	//Shader uniforms
	core::Shader mRayTraceShader{};
	core::Shader mHybridShader{};

	math::Mat4 mModelMatrix{};
	math::Mat4 mViewMatrix{};
	math::Mat4 mProjMatrix{};

	//Model and camera
	std::unique_ptr<core::Mesh> mMesh;
	std::unique_ptr<core::ArcCamera> mCamera;


private:

	void rasterizerRender();
	void rayTraceRender();
	void hybridRender();

	bool trySphereAdd(const math::Vec4& sphere);
	void generateSpheres();


	core::Shader mSphereShader{};
	Skybox mSkybox{};

	GLuint mRayTraceVAO;

	//Random sphere locations (position,radius)
	Light mLight;
	std::vector<Sphere> mSpheres;

	core::AssetLoader mLoader{};


};


#endif