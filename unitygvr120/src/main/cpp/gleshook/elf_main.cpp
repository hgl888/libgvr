#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <dlfcn.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <sys/system_properties.h>
#include "gvrhook/gvrhookfn.h"
#include "elf_hooker.h"

static void *(*__old_impl_dlopen)(const char *filename, int flag);

static int (*__old_impl_connect)(int sockfd, struct sockaddr *serv_addr, int addrlen);

static void *(*__old_impl_android_dlopen_ext)(const char *filename, int flags, const void *extinfo);


extern "C" {



uint64_t getTicksNanos() {
    // Choreographer vsync timestamp is based on.
    struct timespec tp;
    const int status = clock_gettime(CLOCK_MONOTONIC, &tp);
    const uint64_t result = (uint64_t) tp.tv_sec * (uint64_t) (1000 * 1000 * 1000) + uint64_t(tp.tv_nsec);
    return result;
}

double getSeconds() {
    return double(getTicksNanos()) * 0.000000001;
}


//static unsigned int (*old_eglSwapBuffers)(void *dpy, void *draw) = NULL;
//static unsigned int my_eglSwapBuffers(void *dpy, void *draw) {
//    log_info("eglSwapBuffers called,arg display:%p,surface:%p", dpy, draw);
//    int result = old_eglSwapBuffers(dpy, draw);
//    return result;
//}


static unsigned int my_skipEglSwapBuffers(void *dpy, void *draw) {
    return EGL_TRUE;
}


static void logDeviceStatus(){
//    std::string batteryCapacity=Baofeng::Mojing::GetBatteryCapacity();
//    std::string electricCurrent=Baofeng::Mojing::GetElectricCurrent();
//    std::string cpuTemperature=Baofeng::Mojing::GetCPUTemperature();
//    std::string cpuFreq=Baofeng::Mojing::GetCPUFreq();
//    std::string gpuTemperature=Baofeng::Mojing::GetGPUTemperature();
//    std::string gpuFreq=Baofeng::Mojing::GetGPUFreq();
//    log_info("batteryCapacity:%s ,electricCurrent:%s,cpuTemperature:%s,cpuFreq:%s,gpuTemperature:%s,gpuFreq:%s",
//             batteryCapacity.c_str(),electricCurrent.c_str(),cpuTemperature.c_str(),cpuFreq.c_str(),gpuTemperature.c_str(),gpuFreq.c_str());

}


//static unsigned int count_eglSwapBuffers(void *dpy, void *draw) {
//    static int count = 0;
//    static double prev;
//    static double lastCount = 0;
//    static double maxInterval = 0;
//    static double minInterval;
//    double countInterval = 10.0;
//
//    double current = getSeconds();
//    double inteval = current - prev;
//    prev = current;
//    maxInterval = (maxInterval > inteval) ? maxInterval : inteval;
//    minInterval = (minInterval < inteval) ? minInterval : inteval;
//    count++;
//    if (current - lastCount >= countInterval) {
//        log_info("eglSwapBuffers:%d Times,average fps:%g,maxInterval:%g,minInterval:%g",count,count/(current-lastCount),maxInterval*1000,minInterval*1000);
//        maxInterval = 0;
//        minInterval = 10000;
//        count = 0;
//        lastCount = current;
////        logDeviceStatus();
//    }
//
//
//    return old_eglSwapBuffers(dpy, draw);
//}



static void *__nativehook_impl_dlopen(const char *filename, int flag) {
    log_info("__nativehook_impl_dlopen -> (%s)\n", filename);
    void *res = __old_impl_dlopen(filename, flag);
    return res;
}

static int __nativehook_impl_connect(int sockfd, struct sockaddr *serv_addr, int addrlen) {
    log_info("__nativehook_impl_connect ->\n");
    int res = __old_impl_connect(sockfd, serv_addr, addrlen);
    return res;
}

static void *__nativehook_impl_android_dlopen_ext(const char *filename, int flags, const void *extinfo) {
    log_info("__nativehook_impl_android_dlopen_ext -> (%s)\n", filename);
    void *res = __old_impl_android_dlopen_ext(filename, flags, extinfo);
    return res;
}

}

static bool __prehook(const char *module_name, const char *func_name) {
    if (strstr(module_name, "libwebviewchromium.so") != NULL) {
        return true;
    }
    return false;
}

#if (ELFHOOK_STANDALONE)

int main(int argc, char* argv[])
{
    char ch = 0;
    elf_hooker hooker;

    void* h = dlopen("libart.so", RTLD_LAZY);
    void* f = dlsym(h,"artAllocObjectFromCodeResolvedRegion");
    log_info("artAllocObjectFromCodeResolvedRegion : %p\n", f);

    hooker.set_prehook_cb(__prehook);
    hooker.phrase_proc_maps();
    hooker.dump_module_list();
    hooker.hook_all_modules("dlopen", (void*)__nativehook_impl_dlopen, (void**)&__old_impl_dlopen);
    hooker.hook_all_modules("connect", (void*)__nativehook_impl_connect, (void**)&__old_impl_connect);

    do {
        ch = getc(stdin);
    } while(ch != 'q');
    return 0;
}

#else

#include <jni.h>
#include <gvrlog.h>

static char *__class_name = "com/wadahana/testhook/ElfHooker";
static elf_hooker __hooker;
static JavaVM *__java_vm = NULL;
static bool __is_attached = false;

static JNIEnv *__getEnv(bool *attached);

static void __releaseEnv(bool attached);

static int __set_hook(JNIEnv *env, jobject thiz);

static int __test(JNIEnv *env, jobject thiz);

static int __elfhooker_init(JavaVM *vm, JNIEnv *env);

static void __elfhooker_deinit(void);

static JNINativeMethod __methods[] =
        {
                {"setHook", "()I", (void *) __set_hook}
        };

static int __set_hook(JNIEnv *env, jobject thiz) {
    log_info("__set_hook() -->\r\n");
//    __hooker.set_prehook_cb(__prehook);
    __hooker.phrase_proc_maps();
    __hooker.dump_module_list();

    __hooker.hook_all_modules("dlopen", (void *) __nativehook_impl_dlopen, (void **) &__old_impl_dlopen);
    __hooker.hook_all_modules("connect", (void *) __nativehook_impl_connect, (void **) &__old_impl_connect);
    __hooker.hook_all_modules("android_dlopen_ext", (void *) __nativehook_impl_android_dlopen_ext,
                              (void **) &__old_impl_android_dlopen_ext);

#if 0
    void* h = dlopen("libart.so", RTLD_LAZY);
    if (h != NULL) {
        void* f = dlsym(h,"artAllocObjectFromCodeResolvedRegion");
        log_info("artAllocObjectFromCodeResolvedRegion : %p\n", f);
    } else {
        log_error("open libart.so fail\n");
    }
#endif
    return 0;
}

char*  (*old_strncpy)(char* __restrict, const char* __restrict, size_t) = NULL;
/*
 * daydream unity游戏，以cardboard模式运行，就替换一下string
 */
char*  my_strncpy(char* __restrict dest, const char* __restrict src, size_t s){
//    LOGE("my_strncpy, dest=%s, src=%s, size=%d", dest, src, s);
    if(strcmp("cardboard",src)==0){
        LOGI("my_strncpy,src:cardboard,new src daydream");
        return old_strncpy(dest,"daydream",s);
    }

    return old_strncpy(dest,src,s);
}

void hookUnityFun()
{
    LOGE("hookUnityFun");
    char sdkIntStr[PROP_VALUE_MAX];
    memset(sdkIntStr,'\0',PROP_VALUE_MAX);
    __system_property_get("ro.build.version.sdk", sdkIntStr);
    int sdkInt=atoi(sdkIntStr);
//    if(sdkInt < 20)
    {
        __hooker.phrase_proc_maps();
        __hooker.hook_module("libunity.so", "strncpy", (void *) my_strncpy, (void **) &old_strncpy);
    }
}


//void hsiu() {
//    log_info("hookSwapInUnity\r\n");
//    __hooker.phrase_proc_maps();
//    __hooker.dump_module_list();
//    __hooker.hook_module("unity", "eglSwapBuffers", (void *) my_skipEglSwapBuffers, (void **) &old_eglSwapBuffers);
//
//}

//void hookswapInImpl() {
//    log_info("hookswapInImpl\r\n");
//    static bool hasHook = false;
//    if (!hasHook) {
//        __hooker.hook_module("vrapiimpl", "eglSwapBuffers", (void *) my_skipEglSwapBuffers,
//                             (void **) &old_eglSwapBuffers);
//        hasHook = true;
//    }
//}
extern "C" {

void hookImportFunInit()
{
    __hooker.phrase_proc_maps();
    __hooker.dump_module_list();
}

void hookImportFun(const char *modulename, const char *funname, void *myEglGetProcAddress, void **oldEglSwapBuffers)
{
    __hooker.hook_module(modulename, funname, myEglGetProcAddress, oldEglSwapBuffers);
}
}

//void hookSwapForCount() {
//    log_info("enableCountFps");
//    __hooker.phrase_proc_maps();
//    __hooker.hook_module("vrapiimpl", "eglSwapBuffers", (void *) count_eglSwapBuffers,
//                         (void **) &old_eglSwapBuffers);
//
//    __hooker.hook_module("vrapi.so", "eglSwapBuffers", (void *) count_eglSwapBuffers, (void **) &old_eglSwapBuffers);
//    __hooker.hook_module("OculusPlugin", "eglSwapBuffers", (void *) count_eglSwapBuffers,
//                         (void **) &old_eglSwapBuffers);
//
//    __hooker.hook_module("OvrPlugin", "eglSwapBuffers", (void *) count_eglSwapBuffers, (void **) &old_eglSwapBuffers);
//}




static int __elfhooker_register_native_methods(JNIEnv *env, const char *class_name,
                                               JNINativeMethod *methods, int num_methods) {

    log_info("RegisterNatives start for \'%s\'", __class_name);

    jclass clazz = env->FindClass(class_name);
    if (clazz == NULL) {
        log_error("Native registration unable to find class \'%s\'", class_name);
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, methods, num_methods) < 0) {
        log_error("RegisterNatives failed for \'%s\'", class_name);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static int __elfhooker_init(JavaVM *vm, JNIEnv *env) {
    log_info("hookwrapper_init() -->\r\n");
    if (!__elfhooker_register_native_methods(env, __class_name,
                                             __methods, sizeof(__methods) / sizeof(__methods[0]))) {
        log_error("register hookJNIMethod fail, \r\n");
        __elfhooker_deinit();
        return -2;
    }

    return 0;
}

static void __elfhooker_deinit(void) {
    log_info("hookwrapper_deinit()->\r\n");
    return;
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    bool attached;
    __java_vm = vm;

    if ((env = __getEnv(&__is_attached)) == NULL) {
        log_error("getEnv fail\r\n");
        return -1;
    }
    assert(!__is_attached);

    return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
    bool attached;
    JNIEnv *env = __getEnv(&__is_attached);
    assert(!__is_attached);

    __elfhooker_deinit();
    return;
}


static JNIEnv *__getEnv(bool *attached) {
    JNIEnv *env = NULL;
    *attached = false;
    int ret = __java_vm->GetEnv((void **) &env, JNI_VERSION_1_4);
    if (ret == JNI_EDETACHED) {
        if (0 != __java_vm->AttachCurrentThread(&env, NULL)) {
            return NULL;
        }
        *attached = true;
        return env;
    }

    if (ret != JNI_OK) {
        return NULL;
    }

    return env;
}

static void __releaseEnv(bool attached) {
    if (attached)
        __java_vm->DetachCurrentThread();
}

#endif
