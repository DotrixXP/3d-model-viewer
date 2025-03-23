#pragma once
#include "../Graphics/Shader.hpp"

class OpenglData {
  private:
    static bool s_depthTest;
    static bool s_stenctilTest;
    static bool s_blending;
    static bool s_faceCulling;
    static bool s_colorBuffer;
    static float s_pointSize;
    static float s_lineWidth;

  public:
    static void SetBlending(bool value);
    static void SetDepthTesting(bool value);
    static void SetFaceCulling(bool value, bool front = false, int fronFace = GL_CCW);
    static void SetStencilTesting(bool value);
    static void SetColorBuffer(bool value);
    static void SetPolygonMode(int mode);
    

    static void SetPointSize(float size);
    static void SetLineWidth(float width);
    
    static float GetPointSize();
    static float GetLineWidth();
    static bool GetDepthTesting();
    static bool GetStencilTesting();
    static bool GetFaceCulling();
    static bool GetBlending();
    static bool GetColorBuffer();
};
