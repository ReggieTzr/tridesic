#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <Eigen/Dense>

#include "geometrycentral/surface/halfedge_mesh.h"
#include "geometrycentral/surface/halfedge_factories.h"
#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/heat_method_distance.h"
#include "geometrycentral/surface/fast_marching_method.h"
#include "geometrycentral/surface/vertex_position_geometry.h"
#include "exact_geodesic.h"

using namespace std;
using namespace geometrycentral;
using namespace geometrycentral::surface;
namespace py = pybind11;

// Geometry-central data
std::unique_ptr<HalfedgeMesh> mesh;
std::unique_ptr<VertexPositionGeometry> geometry;


// Loads a mesh from a NumPy array
// source: https://github.com/rubenwiersma/hsn/blob/master/vectorheat/src/main.cpp
std::tuple<std::unique_ptr<HalfedgeMesh>, std::unique_ptr<VertexPositionGeometry>>
loadMesh_np(Eigen::MatrixXd& pos, Eigen::Matrix<size_t, Eigen::Dynamic, Eigen::Dynamic>& faces) {

  // Set vertex positions
  std::vector<Vector3> vertexPositions(pos.rows());
  for (size_t i = 0; i < pos.rows(); i++) {
    vertexPositions[i][0] = pos(i, 0);
    vertexPositions[i][1] = pos(i, 1);
    vertexPositions[i][2] = pos(i, 2);
  }

  // Get face list
  std::vector<std::vector<size_t>> faceIndices(faces.rows());
  for (size_t i = 0; i < faces.rows(); i++) {
    faceIndices[i] = {faces(i, 0), faces(i, 1), faces(i, 2)};
  }

  return makeHalfedgeAndGeometry(faceIndices, vertexPositions);
}

// compute geodesic distance using the heat method
Eigen::MatrixXd get_heat_geodesics(Eigen::MatrixXd& pos, Eigen::Matrix<size_t, Eigen::Dynamic, Eigen::Dynamic>& faces){
    // Load mesh
    std::tie(mesh, geometry) = loadMesh_np(pos, faces);
    int nverts = mesh->nVertices();

    // Create the Heat Method solver
    HeatMethodDistanceSolver heatSolver(*geometry);

    // create output matrix
    Eigen::MatrixXd geodesic_matrix(nverts, nverts);

    // compute geodesic distances
    int i = 0;
    for (Vertex v : mesh->vertices()) {
        VertexData<double> distToSource = heatSolver.computeDistance(v);
        for (int j = 0; j < nverts; j++){
            geodesic_matrix(i, j) = distToSource[j];
        }
        i++;
    }

    return geodesic_matrix;
}


// compute geodesic distance using the fast marching method
Eigen::MatrixXd get_fmm_geodesics(Eigen::MatrixXd& pos, Eigen::Matrix<size_t, Eigen::Dynamic, Eigen::Dynamic>& faces){
    // Load mesh
    std::tie(mesh, geometry) = loadMesh_np(pos, faces);
    int nverts = mesh->nVertices();

    // create output matrix
    Eigen::MatrixXd geodesic_matrix(nverts, nverts);
    double initial_dist = 0.;

    // compute geodesic distances
    int i = 0;
    for (Vertex v : mesh->vertices()) {
        std::vector<std::pair<Vertex, double>> input{std::make_pair(v, initial_dist)};
        VertexData<double> distToSource = FMMDistance(*geometry, input);
        for (int j = 0; j < nverts; j++)
            geodesic_matrix(i, j) = distToSource[j];

        i++;
    }

    return geodesic_matrix;
}

// compute geodesic distance using the Exact geodesic algorithm by Mitchell, Mount and Papadimitriou in 1987
Eigen::MatrixXd get_exact_geodesics(Eigen::MatrixXd& pos, Eigen::Matrix<size_t, Eigen::Dynamic, Eigen::Dynamic>& faces){
    // utils vars
    int nverts = pos.rows();
    Eigen::Matrix<size_t, Eigen::Dynamic, Eigen::Dynamic> FS, FT;
    Eigen::MatrixXi target(nverts, 1);
    for (int i = 0; i < nverts; i++)
        target(i, 0) = i;

    // create output matrix
    Eigen::MatrixXd geodesic_matrix(nverts, nverts);

    // compute geodesic distances
    for (int i = 0; i < nverts; i++){
        Eigen::MatrixXi source(1, 1);
        source(0, 0) = i;
        Eigen::MatrixXd distToSource(nverts, 1);
        igl::exact_geodesic(pos, faces, source, FS, target, FT, distToSource);

        for (int j = 0; j < nverts; j++)
            geodesic_matrix(i, j) = distToSource(j);
    }

    return geodesic_matrix;
}


PYBIND11_MODULE(tridesic, m) {
    m.def("get_heat_geodesics", &get_heat_geodesics, py::return_value_policy::copy,
          "Compute the geodesic matrix using the heat method");
    m.def("get_fmm_geodesics", &get_fmm_geodesics, py::return_value_policy::copy,
          "Compute the geodesic matrix using the fast marching method");
    m.def("get_exact_geodesics", &get_exact_geodesics, py::return_value_policy::copy,
          "Compute the geodesic matrix using the Exact geodesic algorithm by Mitchell et al.");
}
