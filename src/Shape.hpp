#ifndef SHAPE_H
#define SHAPE_H

class Shape {
public:
      std::vector<GLfloat> GetVertexs() = 0;
      std::vector<GLfloat> GetNormals() = 0;
      std::vector<GLfloat> GetIndexes() = 0;
};


#endif
