#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "model.h"
//#include "tiny_obj_loader.h"

namespace Models {
    class ObjModel : public Model {
    private:

    public:
        ObjModel();
        ObjModel(const std::string& filename);
        virtual ~ObjModel();
        virtual void drawSolid(bool smooth = true);
        void load(const std::string& filename);

        int vertexCount = 0;
    };
}

#endif

