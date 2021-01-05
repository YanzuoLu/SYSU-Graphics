#include "objUtils.h"

using namespace std;
using namespace glm;

pair<vector<Vertex>,vector<unsigned int>> LoadOBJ(istream& in) {
    vector< Vertex > vertices;
    vector< unsigned int > indices;

    struct VertRef
    {
        VertRef(int v, int vt, int vn)
            : v(v), vt(vt), vn(vn)
        { }
        int v, vt, vn;
    };

    vector< vec4 > positions(1, vec4(0, 0, 0, 0));
    vector< vec3 > texcoords(1, vec3(0, 0, 0));
    vector< vec3 > normals(1, vec3(0, 0, 0));
    string lineStr;
    while (getline(in, lineStr))
    {
        istringstream lineSS(lineStr);
        string lineType;
        lineSS >> lineType;

        // vertex
        if (lineType == "v")
        {
            float x = 0, y = 0, z = 0, w = 1;
            lineSS >> x >> y >> z >> w;
            positions.push_back(vec4(x, y, z, w));
        }

        // texture
        if (lineType == "vt")
        {
            float u = 0, v = 0, w = 0;
            lineSS >> u >> v >> w;
            texcoords.push_back(vec3(u, v, w));
        }

        // normal
        if (lineType == "vn")
        {
            float i = 0, j = 0, k = 0;
            lineSS >> i >> j >> k;
            normals.push_back(normalize(vec3(i, j, k)));
        }

        // polygon
        if (lineType == "f")
        {
            vector< VertRef > refs;
            string refStr;
            while (lineSS >> refStr)
            {
                istringstream ref(refStr);
                string vStr, vtStr, vnStr;
                getline(ref, vStr, '/');
                getline(ref, vtStr, '/');
                getline(ref, vnStr, '/');
                int v = atoi(vStr.c_str());
                int vt = atoi(vtStr.c_str());
                int vn = atoi(vnStr.c_str());
                v = (v >= 0 ? v : positions.size() + v);
                vt = (vt >= 0 ? vt : texcoords.size() + vt);
                vn = (vn >= 0 ? vn : normals.size() + vn);
                refs.push_back(VertRef(v, vt, vn));
            }

            if (refs.size() < 3)
            {
                // error, skip
                continue;
            }

            // triangulate, assuming n>3-gons are convex and coplanar
            VertRef* p[3] = { &refs[0], NULL, NULL };
            for (size_t i = 1; i + 1 < refs.size(); ++i)
            {
                p[1] = &refs[i + 0];
                p[2] = &refs[i + 1];

                // http://www.opengl.org/wiki/Calculating_a_Surface_Normal
                vec3 U(positions[p[1]->v] - positions[p[0]->v]);
                vec3 V(positions[p[2]->v] - positions[p[0]->v]);
                vec3 faceNormal = normalize(cross(U, V));

                for (size_t j = 0; j < 3; ++j)
                {
                    Vertex vert;

                    vert.point[0] = positions[p[j]->v].x;
                    vert.point[1] = positions[p[j]->v].y;
                    vert.point[2] = positions[p[j]->v].z;
                    vert.point[3] = 1;;

                    vert.texcoord = vec2(texcoords[p[j]->vt]);

                    vert.normal[0] = p[j]->vn != 0 ? normals[p[j]->vn].x : faceNormal.x;
                    vert.normal[1] = p[j]->vn != 0 ? normals[p[j]->vn].y : faceNormal.y;
                    vert.normal[2] = p[j]->vn != 0 ? normals[p[j]->vn].z : faceNormal.z;
                    vert.normal[3] = 1;

                    auto iter = find(vertices.begin(), vertices.end(), vert);
                    //cout << vert.point[0] << " " << vert.point[1] << " " << vert.point[2] << " " << vert.point[3] << " " << iter - vertices.end() << endl;
                    if (iter == vertices.end()) {
                        vertices.push_back(vert);
                        indices.push_back(vertices.size() - 1);
                    }
                    else {
                        indices.push_back(iter - vertices.begin());
                    }
                }
            }
        }
    }
    return make_pair(vertices, indices);
}
