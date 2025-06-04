#include "objmodel.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

namespace Models {

	ObjModel::ObjModel() {}

	ObjModel::ObjModel(const std::string& filename) {
		load(filename);
	}

	ObjModel::~ObjModel() {
		delete[] vertices;
		delete[] normals;
		delete[] texCoords;
	}

	void ObjModel::load(const std::string& filename) {
		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cerr << "Failed to open OBJ file: " << filename << std::endl;
			return;
		}

		std::vector<glm::vec3> temp_positions;
		std::vector<glm::vec3> temp_normals;
		std::vector<glm::vec2> temp_texcoords;

		std::vector<float> vertex_data;
		std::vector<float> normal_data;
		std::vector<float> texcoord_data;

		std::string line;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string prefix;
			iss >> prefix;

			if (prefix == "v") {
				glm::vec3 pos;
				iss >> pos.x >> pos.y >> pos.z;
				temp_positions.push_back(pos);
			}
			else if (prefix == "vt") {
				glm::vec2 tex;
				iss >> tex.x >> tex.y;
				temp_texcoords.push_back(tex);
			}
			else if (prefix == "vn") {
				glm::vec3 norm;
				iss >> norm.x >> norm.y >> norm.z;
				temp_normals.push_back(norm);
			}
			else if (prefix == "f") {
				std::string v1, v2, v3;
				iss >> v1 >> v2 >> v3;
				std::string face[3] = { v1, v2, v3 };

				for (int i = 0; i < 3; i++) {
					std::istringstream vss(face[i]);
					std::string vi, ti, ni;

					std::getline(vss, vi, '/');
					std::getline(vss, ti, '/');
					std::getline(vss, ni, '/');

					int vidx = std::stoi(vi) - 1;
					int tidx = ti.empty() ? -1 : std::stoi(ti) - 1;
					int nidx = ni.empty() ? -1 : std::stoi(ni) - 1;

					glm::vec3 v = temp_positions[vidx];
					vertex_data.push_back(v.x);
					vertex_data.push_back(v.y);
					vertex_data.push_back(v.z);
					vertex_data.push_back(1.0f); // W = 1 dla pozycji

					if (nidx >= 0) {
						glm::vec3 n = temp_normals[nidx];
						normal_data.push_back(n.x);
						normal_data.push_back(n.y);
						normal_data.push_back(n.z);
						normal_data.push_back(0.0f); // W = 0 dla wektora
					}
					else {
						normal_data.push_back(0.0f);
						normal_data.push_back(0.0f);
						normal_data.push_back(1.0f);
						normal_data.push_back(0.0f);
					}

					if (tidx >= 0) {
						glm::vec2 t = temp_texcoords[tidx];
						texcoord_data.push_back(t.x);
						texcoord_data.push_back(t.y);
					}
					else {
						texcoord_data.push_back(0.0f);
						texcoord_data.push_back(0.0f);
					}
				}
			}
		}

		vertexCount = vertex_data.size() / 4;

		vertices = new float[vertex_data.size()];
		normals = new float[normal_data.size()];
		texCoords = new float[texcoord_data.size()];
		colors = nullptr;

		std::copy(vertex_data.begin(), vertex_data.end(), vertices);
		std::copy(normal_data.begin(), normal_data.end(), normals);
		std::copy(texcoord_data.begin(), texcoord_data.end(), texCoords);
	}

	void ObjModel::drawSolid(bool smooth) {
		glEnableVertexAttribArray(0); // vertex
		glEnableVertexAttribArray(1); // normal

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vertices);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, normals);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

}
