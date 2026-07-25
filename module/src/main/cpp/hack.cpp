#include "xdl.h"
#include "hack.h"
#include "il2cpp_dump.h"
#include "KittyMemory/KittyMemory.h"
#include "KittyMemory/MemoryPatch.h"
#include "KittyMemory/KittyScanner.h"
#include "KittyMemory/KittyUtils.h"
#include "Include/Utils.h"
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/system_properties.h>
#include <dlfcn.h>
#include <jni.h>
#include <pthread.h>
#include <sys/mman.h>
#include <linux/unistd.h>
#include <array>
#include <string>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cinttypes>
#include "zygisk.hpp"
#include "game.h"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

class MyModule : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        env_ = env;
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        if (!args || !args->nice_name) {
            return;
        }
        enable_hack = isGame(env_, args->app_data_dir);
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        if (enable_hack) {
            if (gameThread = pthread_self()) {
                pthread_create(&gameThread, nullptr, hack_thread, nullptr);
            }
        }
    }

private:
    JNIEnv *env_{};
};

REGISTER_ZYGISK_MODULE(MyModule)

int isGame(JNIEnv *env, jstring appDataDir)
{
    if (!appDataDir)
        return 0;
    const char *app_data_dir = env->GetStringUTFChars(appDataDir, nullptr);
    int user = 0;
    static char package_name[256];
    if (sscanf(app_data_dir, "/data/%*[^/]/%d/%s", &user, package_name) != 2) {
        if (sscanf(app_data_dir, "/data/%*[^/]/%s", package_name) != 1) {
            package_name[0] = '\0';
            return 0;
        }
    }
    if (strcmp(package_name, GamePackageName) == 0) {
        game_data_dir = new char[strlen(app_data_dir) + 1];
        strcpy(game_data_dir, app_data_dir);
        env->ReleaseStringUTFChars(appDataDir, app_data_dir);
        return 1;
    } else {
        env->ReleaseStringUTFChars(appDataDir, app_data_dir);
        return 0;
    }
}


bool isHackLoaded = false;
using KittyMemory::ProcMap;
void* hack_thread(void *) {
    if (!isHackLoaded) {
        ProcMap Il2cppMap;
        do {
            Il2cppMap = KittyMemory::getLibraryBaseMap(TargetLibName);
            handle = xdl_open(TargetLibName, RTLD_LAZY);
        } while (!Il2cppMap.isValid());
        do {
            absoluteStart = getAbsoluteAddress(TargetLibName, 0x0);
        } while (absoluteStart == 0);

        il2cpp_api_init(handle);
        il2cpp_dump(game_data_dir);
        isHackLoaded = true;
    }
    return nullptr;
}