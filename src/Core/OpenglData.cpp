#include "OpenglData.hpp"

bool OpenglData::s_blending = true;
bool OpenglData::s_stenctilTest = false;
bool OpenglData::s_depthTest = true;
bool OpenglData::s_faceCulling = false;
bool OpenglData::s_colorBuffer = true;
float OpenglData::s_pointSize = 1.0f;
float OpenglData::s_lineWidth = 1.0f;

void OpenglData::SetBlending(bool value) {
    if (value) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        OpenglData::s_blending = true;
    }
    else {
        glDisable(GL_BLEND);
        OpenglData::s_blending = false;
    }
}

void OpenglData::SetDepthTesting(bool value) {
    if (value) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        OpenglData::s_depthTest = true;
    }
    else {
        glDisable(GL_DEPTH_TEST);
        OpenglData::s_depthTest = false;
    }
}

void OpenglData::SetPointSize(float size) {
    if (size < 1.0f)
        size = 1.0f;

    glPointSize(size);
    s_pointSize = size;
}

float OpenglData::GetPointSize() {
    return s_pointSize;
}

float OpenglData::GetLineWidth() {
    return s_lineWidth;
}

void OpenglData::SetLineWidth(float width) {
    if (width < 1.0f)
        width = 1.0f;

    glLineWidth(width);
    s_lineWidth = width;
}

void OpenglData::SetFaceCulling(bool value, bool front, int frontFace) {
    if (value) {
        glEnable(GL_CULL_FACE);

        if (front)
            glCullFace(GL_FRONT);
        else
            glCullFace(GL_BACK);

        switch (frontFace) {
        case GL_CCW:
            glFrontFace(GL_CCW);
            break;
        case GL_CW:
            glFrontFace(GL_CW);
            break;
        default:
            glFrontFace(GL_CCW);
            break;
        }

        OpenglData::s_faceCulling = true;
    }
    else {
        glDisable(GL_CULL_FACE);
        OpenglData::s_faceCulling = false;
    }
}

void OpenglData::SetStencilTesting(bool value) {
    if (value) {
        glEnable(GL_STENCIL_TEST);
    }
    else {
        glDisable(GL_STENCIL_TEST);
    }
}

void OpenglData::SetColorBuffer(bool value) {
    OpenglData::s_colorBuffer = value;
}

void OpenglData::SetPolygonMode(int mode) {
    glPolygonMode(GL_FRONT_AND_BACK, mode);
}

bool OpenglData::GetDepthTesting() {
    return OpenglData::s_depthTest;
}
bool OpenglData::GetStencilTesting() {
    return OpenglData::s_stenctilTest;
}
bool OpenglData::GetFaceCulling() {
    return OpenglData::s_faceCulling;
}
bool OpenglData::GetBlending() {
    return OpenglData::s_blending;
}
bool OpenglData::GetColorBuffer() {
    return OpenglData::s_colorBuffer;
}
