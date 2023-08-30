#include <cassert>

#include "core/settings.h"

#include "app/viewer.h"
#include "core/stb_image.h"


Viewer::Viewer() :
	App{},
	mViewerOpen{ true },
	mMousePressed{ false },
	mMouseInit{ false },
	mMouseX{},
	mMouseY{},
	mNumSpheresDisplayed{ 1 },
	mNumRayBounces{1},
	mRasterizeMode{true},
	mRayTraceMode{false},
	mHybridMode{false},
	mSkybox{}
{

}

void Viewer::init()
{

	//Create graphics assets
	mSphereShader.init(SHADER_DIR, "sphere.vs", "sphere.fs");

	generateSpheres();

	const std::vector<std::string> faces
	{
			"right.jpg",
			"left.jpg",
			"top.jpg",
			"bottom.jpg",
			"front.jpg",
			"back.jpg"
	};


	
	mMesh = mLoader.loadMesh(MODEL_DIR, "sphere.obj");
	mCamera = std::make_unique<core::ArcCamera>(mMesh->getCentroid(), 2.0 * mMesh->getBoundingRadius());


	mModelMatrix = math::Mat4::identity();
	mViewMatrix = mCamera->getViewMatrix();
	mProjMatrix = math::Mat4::perspectiveProjection(3.14*45.0/180.0, 800.0 / 600.0, 0.1, 1000.);

	mSphereShader.use();
	mSphereShader.setUniformMat4("model", mModelMatrix.getRawData());
	mSphereShader.setUniformMat4("view", mViewMatrix.getRawData());
	mSphereShader.setUniformMat4("proj", mProjMatrix.getRawData());
	mSphereShader.setUniformInt("skybox", 0);
	mSphereShader.setUniformFloat3("eye", mCamera->getEye());
	
	mSkybox.init(faces, *mCamera, mProjMatrix);
	

	glClearColor(0.22f, 0.22f, 0.22f, 1.0f);
	glEnable(GL_DEPTH_TEST);

}

void Viewer::update()
{
	
}

void Viewer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mSphereShader.use();
	mSphereShader.setUniformMat4("view", mCamera->getViewMatrix().getRawData());
	mSphereShader.setUniformFloat3("eye", mCamera->getEye());

	for (int i = 0; i < mNumSpheresDisplayed;i++) {
		auto sphere = mSpheres.at(i);
		mModelMatrix = math::Mat4::fromUniformScale(sphere.w());
		mModelMatrix = math::Mat4::fromTranslation(sphere.xyz()) * mModelMatrix;
		mSphereShader.setUniformMat4("model",mModelMatrix.getRawData());

		mMesh->Render();

		mModelMatrix = math::Mat4::identity();
		mSphereShader.setUniformMat4("model", mModelMatrix.getRawData());
	}

	glDepthFunc(GL_LEQUAL);
	mSkybox.Render(*mCamera);
	glDepthFunc(GL_LESS);

	

}



bool Viewer::trySphereAdd(const math::Vec4& sphere)
{
	for (int i = 0; i < mSpheres.size(); i++) {
		auto dx = sphere.xyz() - mSpheres.at(i).xyz();
		float rtot = sphere.w() + mSpheres.at(i).w();
		if (dx.dot(dx) <= rtot * rtot) {
			return false;
		}
	}
	return true;
}

void Viewer::generateSpheres()
{
	//Use the same seed each time
	std::srand(2);
	float extents = 4;
	float floor_radius = 200.0;

	mSpheres.reserve(Viewer::MAX_NUM_SPHERES);
	mSpheres.push_back(math::Vec4{0.0, -floor_radius, 0.0, floor_radius});
	for (int i = 1; i < MAX_NUM_SPHERES;i++) {
		auto sphere = math::Vec4{};
		do {
			float radius = math::randFloatRange(0.1, 0.7);
			sphere = math::Vec4{ math::randFloatRange(-extents,extents), radius, math::randFloatRange(-extents,extents),radius};
		} while (!trySphereAdd(sphere));
		mSpheres.push_back(sphere);
	}
}


