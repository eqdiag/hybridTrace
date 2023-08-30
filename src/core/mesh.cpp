#include "mesh.h"
#include <iostream>

core::Mesh::Mesh(const std::vector<VertexData>& vertexData,
    const std::vector<GLuint>& indices,
    bool buildFaceNormals
):
    mRadius{0.0},
    mVertexData{vertexData},
    mIndices{indices},
    mOppositesComputed{false},
    mPrintFrequency{10}
{

    if (buildFaceNormals) {
        computeFaceNormals(vertexData,indices);
    }

    computeFaceCentroids();

    /**std::cout << "Faces:\n";
    for (int f = 0; f < indices.size() / 3; f++) {
        int c = 3 * f;
        std::cout << "\tFace: " << mIndices.at(c) << ", " << mIndices.at(c + 1) << ", " << mIndices.at(c + 2) << std::endl;
    }

    std::cout << "Vertices: \n";
    for (auto& v : mVertexData) {
        std::cout << "\tVertex: " << v.x << ", " << v.y << ", " << v.z << std::endl;
        //std::cout << "\tNormal: " << v.nx << ", " << v.ny << ", " << v.nz << std::endl;

    }*/

    computeBoundingSphere();

    glGenVertexArrays(1,&mVAO);
    glGenBuffers(1,&mVBO);
    glGenBuffers(1,&mEBO);



    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(VertexData), mVertexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(3*sizeof(float)));

   


    glBindVertexArray(0);

}


core::Mesh::~Mesh(){
    glDeleteVertexArrays(1,&mVAO);
    glDeleteBuffers(1,&mVBO);
    glDeleteBuffers(1,&mEBO);   
    std::cout << "mesh deleted\n";
}

void core::Mesh::Render()
{

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

math::Vec3 core::Mesh::getCentroid() const
{
    return mCentroid;
}

float core::Mesh::getBoundingRadius() const
{
    return mRadius;
}

const int core::Mesh::getNumVertices() const
{
    return mVertexData.size();
}

const int core::Mesh::getNumIndices() const
{
    return mIndices.size();
}

const std::vector<core::VertexData>& core::Mesh::getVertexData() const
{
    return mVertexData;
}


const std::vector<GLuint>& core::Mesh::getIndices() const
{
    return mIndices;
}

math::Vec3 core::Mesh::getVertexPoint(int index) const
{
    auto v = mVertexData.at(index);
    return math::Vec3(v.x, v.y, v.z);
}

math::Vec3 core::Mesh::getFaceCentroid(int faceIndex) const
{
    math::Vec3 centroid{};
    int c = 3 * faceIndex;
    centroid += getVertexPoint(mIndices.at(c));
    centroid += getVertexPoint(mIndices.at(c+1));
    centroid += getVertexPoint(mIndices.at(c+2));
    return centroid / 3.0f;
}

math::Vec3 core::Mesh::getNormalVector(int index) const
{
    auto v = mVertexData.at(index);
    return math::Vec3(v.nx, v.ny, v.nz);
}

void core::Mesh::computeOpposites()
{
    if (mOppositesComputed) return;

    mOpposites.resize(mIndices.size());

    /*std::cout << "Vertices\n";
    for (int i = 0; i < vertexData.size();i++) {
        auto v = vertexData.at(i);
        std::cout << i << "->" << "[" << v.x << ", " << v.y << ", " << v.z << "]\n";
    }*/

    int freq = (mIndices.size() / mPrintFrequency) + 1;
    std::cout << "Corners processed:\n";
    for (int c0 = 0; c0 < mIndices.size(); c0++) {

        for (int c1 = c0; c1 < mIndices.size(); c1++) {

            if (checkOpposites(c0, c1)) {
                mOpposites[c0] = c1;
                mOpposites[c1] = c0;
                //std::cout << c0 << "<->" << c1 << std::endl;
            }
        }
        if (c0 % freq == 0) {
            std::cout << "\t[" << c0 << "/" << mIndices.size() << "]\n";
        }
    }

    mOppositesComputed = true;
}

void core::Mesh::computeFaceCentroids()
{
    for (int f = 0; f < mIndices.size() / 3; f++) {
        int c = 3 * f;
        auto v0 = getVertexData(c);
        auto v1 = getVertexData(c + 1);
        auto v2 = getVertexData(c + 2);
        auto vc = (v0 + v1 + v2) / 3.0f;
        mFaceCentroids.emplace_back(vc);
    }
}

std::pair<int, math::Vec3> core::Mesh::getIntersection(const math::Vec3& p, const math::Vec3& v) const
{
    math::Vec3 closest{mVertexData[0].x, mVertexData[0].y, mVertexData[0].z};
    auto dx = p - closest;
    float dist2{ dx.dot(dx)};
    int hitFace = -1;

    for (int f = 0; f < mIndices.size() / 3; f++) {
        auto hit = computeTriangleInteresection(p, v, f);
        if (hit.has_value()) {
            dx = hit.value() - mFaceCentroids.at(f);
            auto newDist = dx.dot(dx);
            if (newDist < dist2) {
                dist2 = newDist;
                closest = hit.value();
                hitFace = f;
            }
        }
    }

    return std::make_pair(hitFace, closest);
}

int core::Mesh::face(int c) const
{
    return c / 3;
}

int core::Mesh::next(int c) const
{
    return 3 * face(c) + (c + 1) % 3;
}

int core::Mesh::prev(int c) const
{
    return 3 * face(c) + (c + 2) % 3;
}

int core::Mesh::vertex(int c) const
{
    return mIndices.at(c);
}

int core::Mesh::corner(int v) const
{
    for (int c = 0; c < mIndices.size(); c++) {
        if (mIndices.at(c) == v) return c;
    }
    return -1;
}

void core::Mesh::computeBoundingSphere()
{
    mCentroid = math::Vec3();
    mRadius = 0.0f;
    for (auto& v : mVertexData) {
        auto vec = math::Vec3(v.x, v.y, v.z);
        mCentroid += vec;
        mRadius = std::fmax(mRadius, vec.norm());
    }
    mCentroid /= mVertexData.size();
}

math::Vec3 core::Mesh::getVertexData(int c) const
{
    auto v = mVertexData.at(vertex(c));
    return math::Vec3(v.x, v.y, v.z);
}

bool core::Mesh::checkOpposites(int c0, int c1) const
{
    auto dx = getVertexData(next(c0)) - getVertexData(prev(c1));
    if (dx.dot(dx) > 0.001) return false;
    dx = getVertexData(prev(c0)) - getVertexData(next(c1));
    if (dx.dot(dx) > 0.001) return false;
    return true;
}



void core::Mesh::computeFaceNormals(const std::vector<VertexData>& vertexData, const std::vector<GLuint>& indices)
{

    mIndices.clear();
    mVertexData.clear();
   

    //Compute normal for each face
    for (int f = 0; f < indices.size()/3; f++) {
        int c = 3 * f;
        auto v0 = vertexData.at(indices.at(c));
        auto v1 = vertexData.at(indices.at(c+1));
        auto v2 = vertexData.at(indices.at(c+2));
        math::Vec3 v0Vec{v0.x, v0.y, v0.z};
        math::Vec3 v1Vec{v1.x, v1.y, v1.z};
        math::Vec3 v2Vec{v2.x, v2.y, v2.z};
        math::Vec3 normal{(v1Vec - v0Vec).cross(v2Vec - v0Vec).normalize()};
        //glm::vec3 normal{0, 0., 1};

        v0.nx = normal.x();
        v0.ny = normal.y();
        v0.nz = normal.z();
        mVertexData.push_back(v0);

        v1.nx = normal.x();
        v1.ny = normal.y();
        v1.nz = normal.z();
        mVertexData.push_back(v1);

        v2.nx = normal.x();
        v2.ny = normal.y();
        v2.nz = normal.z();
        mVertexData.push_back(v2);

        mIndices.push_back(c);
        mIndices.push_back(c+1);
        mIndices.push_back(c+2);

    }
}


int core::Mesh::opp(int c) const
{
    return mOpposites.at(c);
}

int core::Mesh::swing(int c) const
{
    return next(opp(next(c)));
}


std::optional<math::Vec3> core::Mesh::computeTriangleInteresection(const math::Vec3& p, const math::Vec3& v,int faceId) const
{
    int c = 3 * faceId;
    auto data = mVertexData.at(mIndices.at(c));
    auto p0 = math::Vec3(data.x, data.y, data.z);
    auto n = math::Vec3(data.nx, data.ny, data.nz);

    float denom = n.dot(v);
    //Parallel ray case
    if (fabs(denom) < 0.001f) return std::nullopt;

    float num = -n.dot(p - p0);
    float t = num / denom;

    return std::make_optional<math::Vec3>(p + v*t);
}


