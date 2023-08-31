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
	mEnvMapToggle{true},
	mNumSpheresDisplayed{ 1 },
	mNumRayBounces{0},
	mRasterizeMode{true},
	mRayTraceMode{false},
	mHybridMode{false},
	mSkybox{},
	mLight{}
{

}

Viewer::~Viewer()
{
	glDeleteVertexArrays(1, &mRayTraceVAO);
}

void Viewer::init()
{

	//Create graphics assets
	mSphereShader.init(SHADER_DIR, "sphere.vs", "sphere.fs");
	mRayTraceShader.init(SHADER_DIR, "trace.vs", "trace.fs");

	mLight = Light{
		math::Vec3{0,1000,0},
		math::Color3{1.0,1.0,1.0}
	};


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


	//Setup for ray trace mode
	
	glGenVertexArrays(1, &mRayTraceVAO);

	mRayTraceShader.use();

	for (int i = 0; i < MAX_NUM_SPHERES;i++) {
		std::string uniform{};
		uniform += "spheres[";
		uniform += std::to_string(i);
		uniform += "].";
		mRayTraceShader.setUniformFloat3((uniform+"center").c_str(), mSpheres[i].center);
		mRayTraceShader.setUniformFloat((uniform + "radius").c_str(), mSpheres[i].radius);
		mRayTraceShader.setUniformFloat3((uniform + "material.diffuse").c_str(), mSpheres[i].mat.diffuse);
		mRayTraceShader.setUniformFloat3((uniform + "material.specular").c_str(), mSpheres[i].mat.specular);
		mRayTraceShader.setUniformFloat((uniform + "material.a").c_str(), mSpheres[i].mat.specularExponent);
	}

	

	glClearColor(0.22f, 0.22f, 0.22f, 1.0f);
	glEnable(GL_DEPTH_TEST);

}

void Viewer::update()
{
	
}

void Viewer::render()
{
	if (mRasterizeMode) {
		rasterizerRender();
	}
	else if (mRayTraceMode) {
		rayTraceRender();
	}
	else if (mHybridMode) {
		hybridRender();
	}
}



void Viewer::rasterizerRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	mSphereShader.use();
	mSphereShader.setUniformMat4("view", mCamera->getViewMatrix().getRawData());
	mSphereShader.setUniformFloat3("eye", mCamera->getEye());
	mSphereShader.setUniformBool("skyboxToggle", mEnvMapToggle);

	for (int i = 0; i < mNumSpheresDisplayed; i++) {
		auto sphere = mSpheres.at(i);
		mModelMatrix = math::Mat4::fromUniformScale(sphere.radius);
		mModelMatrix = math::Mat4::fromTranslation(sphere.center) * mModelMatrix;
		mSphereShader.setUniformMat4("model", mModelMatrix.getRawData());
		mSphereShader.setUniformFloat3("material.diffuse", sphere.mat.diffuse);
		mSphereShader.setUniformFloat3("material.specular", sphere.mat.specular);
		mSphereShader.setUniformFloat("material.a", sphere.mat.specularExponent);

		mSphereShader.setUniformFloat3("light.position", mLight.position);
		mSphereShader.setUniformFloat3("light.intensity", mLight.intensity);

		mMesh->Render();

		mModelMatrix = math::Mat4::identity();
		mSphereShader.setUniformMat4("model", mModelMatrix.getRawData());
	}

	glDepthFunc(GL_LEQUAL);
	mSkybox.Render(*mCamera,mEnvMapToggle);
	glDepthFunc(GL_LESS);
}

void Viewer::rayTraceRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mRayTraceShader.use();
	mRayTraceShader.setUniformFloat3("light.position", mLight.position);
	mRayTraceShader.setUniformFloat3("light.intensity", mLight.intensity);
	mRayTraceShader.setUniformInt("skybox", 0);
	mRayTraceShader.setUniformBool("skyboxToggle", mEnvMapToggle);

	mViewMatrix = mCamera->getViewMatrix();
	mRayTraceShader.setUniformMat4("proj", mProjMatrix.getRawData());
	mRayTraceShader.setUniformMat4("view", mViewMatrix.getRawData());
	mRayTraceShader.setUniformFloat3("eye", mCamera->getEye());


	glBindVertexArray(mRayTraceVAO);

	glDrawArrays(GL_TRIANGLES, 0, 3);

}

void Viewer::hybridRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

bool Viewer::trySphereAdd(const math::Vec4& sphere)
{
	for (int i = 0; i < mSpheres.size(); i++) {
		auto dx = sphere.xyz() - mSpheres.at(i).center;
		float rtot = sphere.w() + mSpheres.at(i).radius;
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
	float floor_radius = 1600.0;

	mSpheres.reserve(Viewer::MAX_NUM_SPHERES);

	Sphere floorSphere{
		math::Vec3{0.0, -floor_radius, 0.0},
		floor_radius,
		Material{
			math::Color3{1.0,1.0,1.0}*.1,
			math::Color3{1.0,1.0,1.0}*.1,
			16.0f
		}
	};

	mSpheres.push_back(floorSphere);
	mSpheres.push_back(
		Sphere{
				math::Vec3{0.0, 1.0,0.0},
				1.0,
				Material{
					math::Color3{1.0f,1.0f,1.0f}*.1,
					math::Color3{1.0f,1.0f,1.0f}*.1,
					16.0f
				}
		}
	);

	for (int i = 2; i < MAX_NUM_SPHERES;i++) {
		Sphere sphere{};
		do {
			float radius = math::randFloatRange(0.1, 0.7);
			sphere = Sphere{
				math::Vec3{ math::randFloatRange(-extents,extents), radius, math::randFloatRange(-extents,extents)},
				radius,
				Material{
					(math::Color3::rand() + math::Color3{1.0f,1.0f,1.0f})*2.0f/5.0f,
					math::Color3{1.0f,1.0f,1.0f}*.1,
					16.0f
				}
			};
		} while (!trySphereAdd(math::Vec4{sphere.center.x(), sphere.center.y(), sphere.center.z(), sphere.radius}));
		mSpheres.push_back(sphere);
	}
}


