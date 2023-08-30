#ifndef VIEWER_H
#define VIEWER_H

#include <memory>

#include "core/app.h"
#include "core/shader.h"
#include "core/mesh.h"
#include "core/asset_loader.h"
#include "core/camera.h"
#include "math/matrix.h"

#include "skybox.h"


class Viewer : public core::App {
public:
	Viewer();
	void init() override;
	void update() override;
	void render() override;



	//State variables
	bool mViewerOpen;
	bool mMousePressed;
	bool mMouseInit;
	float mMouseX;
	float mMouseY;
	int mNumSpheresDisplayed;
	int mNumRayBounces;
	bool mRasterizeMode;
	bool mRayTraceMode;
	bool mHybridMode;


	const int MAX_NUM_SPHERES = 40;



	//Shader uniforms
	math::Mat4 mModelMatrix{};
	math::Mat4 mViewMatrix{};
	math::Mat4 mProjMatrix{};

	//Model and camera
	std::unique_ptr<core::Mesh> mMesh;
	std::unique_ptr<core::ArcCamera> mCamera;

private:

	bool trySphereAdd(const math::Vec4& sphere);
	void generateSpheres();

	core::Shader mSphereShader{};
	Skybox mSkybox{};
	GLuint mVAO, mVBO;

	//Random sphere locations (position,radius)
	std::vector<math::Vec4> mSpheres;


	core::AssetLoader mLoader{};


};


#endif