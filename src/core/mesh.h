#ifndef MESH_H
#define MESH_H

#include <vector>
#include <optional>

#include <glad/glad.h>

#include "core/shader.h"
#include "math/vec.h"

namespace core {

    struct VertexData {
        float x, y, z, nx, ny, nz;
    };

    class Mesh {
    public:

        Mesh(
            const std::vector<VertexData>& vertexData,
            const std::vector<GLuint>& indices,
            bool buildFaceNormals = false);
 
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh&&) = delete;
        Mesh& operator=(Mesh&&) = delete;
        virtual ~Mesh();

        virtual void Render();

        math::Vec3 getCentroid() const;
        float getBoundingRadius() const;
        const int getNumVertices() const;
        const int getNumIndices() const;
        const std::vector<VertexData>& getVertexData() const;
        const std::vector<GLuint>& getIndices() const;
        math::Vec3 getVertexPoint(int index) const;
        math::Vec3 getFaceCentroid(int faceIndex) const;
        math::Vec3 getNormalVector(int index) const;
        void computeOpposites();

        void computeFaceCentroids();
        std::pair<int, math::Vec3> getIntersection(const math::Vec3& p, const math::Vec3& v) const;

        //Uses corners representation of opposites table
        int face(int c) const;
        int next(int c) const;
        int prev(int c) const;
        int vertex(int c) const;
        int corner(int v) const;
        int opp(int c) const;
        int swing(int c) const;

    protected:

        GLuint mVAO, mVBO, mEBO;

        float mRadius;
        math::Vec3 mCentroid;

        std::vector<VertexData> mVertexData;
        std::vector<GLuint> mIndices;
        std::vector<int> mOpposites;

        std::vector<math::Vec3> mFaceCentroids;

        //Variables keeping track of cached computations
        bool mOppositesComputed;

        //For printing info about mesh processing
        int mPrintFrequency;


        void computeBoundingSphere();


        

        math::Vec3 getVertexData(int c) const;

        bool checkOpposites(int c0, int c1) const;
        void computeFaceNormals(const std::vector<VertexData>& vertexData, const std::vector<GLuint>& indices);

    private:
        std::optional<math::Vec3> computeTriangleInteresection(const math::Vec3& p,const math::Vec3& v,int faceId) const;
        

    };

}

#endif