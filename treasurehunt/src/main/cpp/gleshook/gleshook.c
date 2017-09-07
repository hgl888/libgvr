#include <stdint.h>
#include <GLES2/gl2.h>
#include "callstack.h"
#include <android/log.h>
#include <string.h>
#include <EGL/egl.h>
#include <pthread.h>
#include "hookutils.h"
#include "log.h"

//egl

/////////////////////////////
//gles
void (*old_glShaderSource) (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length) = NULL;
void mj_glShaderSource (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length)
{
    LOGITAG("mjgl","mj_glShaderSource");
//    for(int i = 0; i < count; ++i){
//        int len = strlen(*string);
//        FILE *pfile = fopen("/sdcard/shader.txt", "wb");
//        fwrite(*string, len, 1, pfile);
//        fflush(pfile);
//        fclose(pfile);
////        LOGITAG("mjgl","shader: %s", *string);
//    }
    return old_glShaderSource(shader, count, string, length);
}

void  (*old_glBindBuffer) (GLenum target, GLuint buffer) = NULL;
void mj_glBindBuffer (GLenum target, GLuint buffer)
{
    LOGITAG("mjgl","mj_glBindBuffer, bufferid=%d", buffer);
    return old_glBindBuffer(target, buffer);
}

void (*old_glGenFramebuffers)(GLsizei n, GLuint *framebuffers) = NULL;
void mj_glGenFramebuffers (GLsizei n, GLuint *framebuffers)
{
    LOGITAG("mjgl", "mj_glGenFramebuffers, tid=%d", gettid());
    return old_glGenFramebuffers(n, framebuffers);
}

void (*old_glGenTextures)(GLsizei n, GLuint *textures) = NULL;
void mj_glGenTextures (GLsizei n, GLuint *textures)
{
    LOGITAG("mjgl", "mj_glGenTextures, tid=%d", gettid());
    return old_glGenTextures(n, textures);
}

void (*old_glBindTexture)(GLenum target, GLuint texture) = NULL;
void mj_glBindTexture (GLenum target, GLuint texture)
{
    LOGITAG("mjgl", "mj_glBindTexture, texid=%d, tid=%d", texture, gettid());
    return old_glBindTexture(target, texture);
}

void (*old_glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) = NULL;
void mj_glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    LOGITAG("mjgl", "mj_glFramebufferTexture2D, texid=%d, tid=%d", texture, gettid());
    return old_glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void (*old_glGenRenderbuffers)(GLsizei n, GLuint *renderbuffers) = NULL;
void mj_glGenRenderbuffers (GLsizei n, GLuint *renderbuffers)
{
    LOGITAG("mjgl", "mj_glGenRenderbuffers");
    return old_glGenRenderbuffers(n, renderbuffers);
}

void (*old_glBindFramebuffer)(GLenum target, GLuint framebuffer) = NULL;
void mj_glBindFramebuffer (GLenum target, GLuint framebuffer)
{
    LOGITAG("mjgl","mj_glBindFramebuffer, framebuffer=%d, tid=%d", framebuffer, gettid());
    return old_glBindFramebuffer(target, framebuffer);
}



void (*old_glBindRenderbuffer)(GLenum target, GLuint renderbuffer) = NULL;
void mj_glBindRenderbuffer (GLenum target, GLuint renderbuffer)
{
    LOGITAG("mjgl","mj_glBindRenderbuffer");
    return old_glBindRenderbuffer(target, renderbuffer);
}

void (*old_glBindBufferRange) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) = NULL;
void mj_glBindBufferRange (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
    LOGITAG("mjgl","mj_glBindBufferRange");
    return old_glBindBufferRange(target, index, buffer, offset, size);
}

void (*old_glBindBufferBase) (GLenum target, GLuint index, GLuint buffer) = NULL;
void mj_glBindBufferBase (GLenum target, GLuint index, GLuint buffer)
{
    LOGITAG("mjgl","mj_glBindBufferBase");
    return old_glBindBufferBase(target, index, buffer);
}

void (*old_glBufferData)(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) = NULL;
void mj_glBufferData (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
{
    LOGITAG("mjgl","mj_glBufferData");
//    FILE *pfile = fopen("/sdcard/bufferdata.txt", "wb");
//    fwrite(data, size, 1, pfile);
//    fflush(pfile);
//    fclose(pfile);
    return old_glBufferData(target, size, data, usage);
}

void (*old_glDisableVertexAttribArray) (GLuint index) = NULL;
void mj_glDisableVertexAttribArray (GLuint index)
{
    LOGITAG("mjgl","mj_glDisableVertexAttribArray, index=%d", index);
    return old_glDisableVertexAttribArray(index);
}

void (*old_glEnableVertexAttribArray) (GLuint index) = NULL;
void mj_glEnableVertexAttribArray (GLuint index)
{
    LOGITAG("mjgl","mj_glEnableVertexAttribArray, index=%d", index);
    return old_glEnableVertexAttribArray(index);
}

void (*old_glVertexAttribPointer)(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr) = NULL;
void mj_glVertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr)
{
    LOGITAG("mjgl","mj_glVertexAttribPointer, indx=%d, size=%d, type=%d, stride=%d, ptr=%d", indx, size, type, stride, ptr);
    return old_glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
}

void (*old_glDrawArrays)(GLenum mode, GLint first, GLsizei count) = NULL;
void mj_glDrawArrays (GLenum mode, GLint first, GLsizei count)
{
    LOGITAG("mjgl","mj_glDrawArrays");
    return old_glDrawArrays(mode, first, count);
}

void (*old_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) = NULL;
void mj_glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    LOGITAG("mjgl","mj_glDrawElements, tid=%d", gettid());
    return old_glDrawElements(mode, count, type, indices);
}

void (*old_glUseProgram) (GLuint program) = NULL;
void mj_glUseProgram (GLuint program)
{
    LOGITAG("mjgl","mj_glUseProgram, programid=%d, tid=%d", program, gettid());
    return old_glUseProgram(program);
}

void (*old_glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) = NULL;
void mj_glRenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    LOGITAG("mjgl","mj_glRenderbufferStorage");
    return old_glRenderbufferStorage(target, internalformat, width, height);
}


void (*old_glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) = NULL;
void mj_glFramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    LOGITAG("mjgl","mj_glFramebufferRenderbuffer, tid=%d", gettid());
    return old_glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

void (*old_glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) = NULL;
void mj_glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    LOGITAG("mjgl","mj_glCopyTexSubImage2D");
    return old_glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

void hookESFun()
{
    hook((uint32_t) glShaderSource, (uint32_t)mj_glShaderSource, (uint32_t **) &old_glShaderSource);
    hook((uint32_t) glBindFramebuffer, (uint32_t)mj_glBindFramebuffer, (uint32_t **) &old_glBindFramebuffer);
    hook((uint32_t) glBindRenderbuffer, (uint32_t)mj_glBindRenderbuffer, (uint32_t **) &old_glBindRenderbuffer);
    hook((uint32_t) glBindBuffer, (uint32_t)mj_glBindBuffer, (uint32_t **) &old_glBindBuffer);
    hook((uint32_t) glGenFramebuffers, (uint32_t)mj_glGenFramebuffers, (uint32_t**)&old_glGenFramebuffers);
    hook((uint32_t) glGenTextures, (uint32_t)mj_glGenTextures, (uint32_t**)&old_glGenTextures);
    hook((uint32_t) glBindTexture, (uint32_t)mj_glBindTexture, (uint32_t**)&old_glBindTexture);
    hook((uint32_t) glFramebufferTexture2D, (uint32_t)mj_glFramebufferTexture2D, (uint32_t**)&old_glFramebufferTexture2D);

    hook((uint32_t) glGenRenderbuffers, (uint32_t)mj_glGenRenderbuffers, (uint32_t**)&old_glGenRenderbuffers);
//    hook((uint32_t) glBindBufferRange, (uint32_t)mj_glBindBufferRange, (uint32_t **) &old_glBindBufferRange);
//    hook((uint32_t) glBindBufferBase, (uint32_t)mj_glBindBufferBase, (uint32_t **) &old_glBindBufferBase);
    hook((uint32_t) glBufferData, (uint32_t)mj_glBufferData, (uint32_t **) &old_glBufferData);
    hook((uint32_t) glDisableVertexAttribArray, (uint32_t)mj_glDisableVertexAttribArray, (uint32_t **) &old_glDisableVertexAttribArray);
    hook((uint32_t) glEnableVertexAttribArray, (uint32_t)mj_glEnableVertexAttribArray, (uint32_t **) &old_glEnableVertexAttribArray);
    hook((uint32_t) glVertexAttribPointer, (uint32_t)mj_glVertexAttribPointer, (uint32_t **) &old_glVertexAttribPointer);
    hook((uint32_t) glDrawArrays, (uint32_t)mj_glDrawArrays, (uint32_t **) &old_glDrawArrays);
    hook((uint32_t) glDrawElements, (uint32_t)mj_glDrawElements, (uint32_t **) &old_glDrawElements);
    hook((uint32_t) glUseProgram, (uint32_t)mj_glUseProgram, (uint32_t **) &old_glUseProgram);
    hook((uint32_t) glRenderbufferStorage, (uint32_t)mj_glRenderbufferStorage, (uint32_t **) &old_glRenderbufferStorage);
    hook((uint32_t) glFramebufferRenderbuffer, (uint32_t)mj_glFramebufferRenderbuffer, (uint32_t **) &old_glFramebufferRenderbuffer);
    hook((uint32_t) glCopyTexSubImage2D, (uint32_t)mj_glCopyTexSubImage2D, (uint32_t **) &old_glCopyTexSubImage2D);
}

void hookGLESFun()
{
    hookEGLFun();
    hookEglextFun();
    hookESFun();
    return;
}

void unhookAllFun()
{
    inlineUnHookAll();
//    unHook((uint32_t)glShaderSource);
//    unHook((uint32_t)glBindBuffer);
//    unHook((uint32_t)glBindBufferRange);
//    unHook((uint32_t)glBindBufferBase);
//    unHook((uint32_t)glBufferData);
//    unHook((uint32_t)glEnableVertexAttribArray);
//    unHook((uint32_t)glVertexAttribPointer);
}
