/*
晚宀开源suf创建imgui适配安卓17
删除死妈
群号1105344707
*/



#ifndef A_NATIVE_WINDOW_CREATOR_H // !A_NATIVE_WINDOW_CREATOR_H
#define A_NATIVE_WINDOW_CREATOR_H

#include <android/native_window.h>
#include <android/log.h>
#include <dlfcn.h>
#include <sys/system_properties.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <limits>
#include <optional>
#include <unistd.h>
#include <unordered_map>
#include <string>
#include <string_view>
#include <vector>

#define ResolveMethod(ClassName, MethodName, Handle, MethodSignature)                                                                    \
    ClassName##__##MethodName = reinterpret_cast<decltype(ClassName##__##MethodName)>(symbolMethod.Find(Handle, MethodSignature));       \
    if (nullptr == ClassName##__##MethodName)                                                                                            \
    {                                                                                                                                    \
        __android_log_print(ANDROID_LOG_ERROR, "ImGui", "[-] Method not found: %s -> %s::%s", MethodSignature, #ClassName, #MethodName); \
    }

namespace android {
    namespace detail {
        namespace ui {
            // A LayerStack identifies a Z-ordered group of layers. A layer can only be associated to a single
            // LayerStack, but a LayerStack can be associated to multiple displays, mirroring the same content.
            struct LayerStack
            {
                uint32_t id = UINT32_MAX;
            };

            enum class Rotation
            {
                Rotation0 = 0,
                Rotation90 = 1,
                Rotation180 = 2,
                Rotation270 = 3
            };

            // A simple value type representing a two-dimensional size.
            struct Size
            {
                int32_t width = -1;
                int32_t height = -1;
            };

            // Transactional state of physical or virtual display. Note that libgui defines
            // android::DisplayState as a superset of android::ui::DisplayState.
            struct DisplayState
            {
                LayerStack layerStack;
                Rotation orientation = Rotation::Rotation0;
                Size layerStackSpaceRect;
            };

            typedef int64_t nsecs_t; // nano-seconds
            struct DisplayInfo
            {
                uint32_t w{0};
                uint32_t h{0};
                float xdpi{0};
                float ydpi{0};
                float fps{0};
                float density{0};
                uint8_t orientation{0};
                bool secure{false};
                nsecs_t appVsyncOffset{0};
                nsecs_t presentationDeadline{0};
                uint32_t viewportW{0};
                uint32_t viewportH{0};
            };

            enum class DisplayType
            {
                DisplayIdMain = 0,
                DisplayIdHdmi = 1
            };

            struct PhysicalDisplayId
            {
                uint64_t value;
            };
        }

        struct String8;

        struct LayerMetadata;

        struct Surface;

        struct SurfaceControl;

        struct SurfaceComposerClientTransaction;

        struct SurfaceComposerClient;

        template <typename any_t>
        struct StrongPointer
        {
            union
            {
                any_t *pointer;
                char padding[sizeof(std::max_align_t)];
            };

            inline any_t *operator->() const { return pointer; }
            inline any_t *get() const { return pointer; }
            inline explicit operator bool() const { return nullptr != pointer; }
        };

        struct Functionals
        {
            struct SymbolMethod
            {
                void *(*Open)(const char *filename, int flag) = nullptr;
                void *(*Find)(void *handle, const char *symbol) = nullptr;
                int (*Close)(void *handle) = nullptr;
            };

            size_t systemVersion = 13;

            void (*RefBase__IncStrong)(void *thiz, void *id) = nullptr;
            void (*RefBase__DecStrong)(void *thiz, void *id) = nullptr;

            void (*String8__Constructor)(void *thiz, const char *const data) = nullptr;
            void (*String8__Destructor)(void *thiz) = nullptr;

            void (*LayerMetadata__Constructor)(void *thiz) = nullptr;
            void (*LayerMetadata__setInt32)(void *thiz, uint32_t key, int32_t value) = nullptr;

            void (*SurfaceComposerClient__Constructor)(void *thiz) = nullptr;
            void (*SurfaceComposerClient__Destructor)(void *thiz) = nullptr;
            StrongPointer<void> (*SurfaceComposerClient__CreateSurface)(void *thiz, void *name, uint32_t w, uint32_t h, int32_t format, uint32_t flags, void *parentHandle, void *layerMetadata, uint32_t *outTransformHint) = nullptr;
            StrongPointer<void> (*SurfaceComposerClient__CreateSurface_and9)(void *thiz, void *name, uint32_t w, uint32_t h, int32_t format, uint32_t flags, void *parentHandle, int32_t windowType, int32_t ownerUid) = nullptr;
            StrongPointer<void> (*SurfaceComposerClient__MirrorSurface)(void *thiz, void *surface, void *mirrorRoot) = nullptr;
            StrongPointer<void> (*SurfaceComposerClient__GetInternalDisplayToken)() = nullptr;
            StrongPointer<void> (*SurfaceComposerClient__GetBuiltInDisplay)(ui::DisplayType type) = nullptr;
            int32_t (*SurfaceComposerClient__GetDisplayState)(StrongPointer<void> &display, ui::DisplayState *displayState) = nullptr;
            int32_t (*SurfaceComposerClient__GetDisplayInfo)(StrongPointer<void> &display, ui::DisplayInfo *displayInfo) = nullptr;
            std::vector<ui::PhysicalDisplayId> (*SurfaceComposerClient__GetPhysicalDisplayIds)() = nullptr;
            StrongPointer<void> (*SurfaceComposerClient__GetPhysicalDisplayToken)(ui::PhysicalDisplayId displayId) = nullptr;

            void (*SurfaceComposerClient__Transaction__Constructor)(void *thiz) = nullptr;
            void *(*SurfaceComposerClient__Transaction__SetLayer)(void *thiz, StrongPointer<void> &surfaceControl, int32_t z) = nullptr;
            void *(*SurfaceComposerClient__Transaction__SetTrustedOverlay)(void *thiz, StrongPointer<void> &surfaceControl, bool isTrustedOverlay) = nullptr;
            void (*WindowInfoHandle__Constructor)(void *thiz) = nullptr;
            void *(*SurfaceComposerClient__Transaction__SetInputWindowInfo)(void *thiz, StrongPointer<void> &surfaceControl, void *inputWindowInfo) = nullptr;
            void *(*SurfaceComposerClient__Transaction__SetLayerStack)(void *thiz, StrongPointer<void> &surfaceControl, uint32_t layerStack) = nullptr;
            void *(*SurfaceComposerClient__Transaction__Show)(void *thiz, StrongPointer<void> &surfaceControl) = nullptr;
            void *(*SurfaceComposerClient__Transaction__Reparent)(void *thiz, StrongPointer<void> &surfaceControl, StrongPointer<void> &newParent) = nullptr;
            void *(*SurfaceComposerClient__Transaction__SetMatrix)(void *thiz, StrongPointer<void> &surfaceControl, float dsdx, float dtdx, float dtdy, float dsdy) = nullptr;
            void *(*SurfaceComposerClient__Transaction__SetPosition)(void *thiz, StrongPointer<void> &surfaceControl, float x, float y) = nullptr;
            int32_t (*SurfaceComposerClient__Transaction__Apply)(void *thiz, bool synchronous, bool oneWay) = nullptr;

            int32_t (*SurfaceControl__Validate)(void *thiz) = nullptr;
            StrongPointer<Surface> (*SurfaceControl__GetSurface)(void *thiz) = nullptr;
            void (*SurfaceControl__DisConnect)(void *thiz) = nullptr;

            Functionals(const SymbolMethod &symbolMethod)
            {
                std::string systemVersionString(128, 0);

                systemVersionString.resize(__system_property_get("ro.build.version.release", systemVersionString.data()));
                if (!systemVersionString.empty())
                    systemVersion = std::stoi(systemVersionString);

                if (9 > systemVersion)
                {
                    __android_log_print(ANDROID_LOG_ERROR, "ImGui", "[-] Unsupported system version: %zu", systemVersion);
                    return;
                }

                static std::unordered_map<size_t, std::unordered_map<void **, const char *>> patchesTable = {
                    {
                        17,
                        {
                            {reinterpret_cast<void **>(&LayerMetadata__Constructor), "_ZN7android3gui13LayerMetadataC2Ev"},
                            {reinterpret_cast<void **>(&SurfaceComposerClient__CreateSurface), "_ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjiiRKNS_2spINS_7IBinderEEERKNS_3gui13LayerMetadataEPj"},
                        },
                    },
                    {
                        16,
                        {
                            {reinterpret_cast<void **>(&LayerMetadata__Constructor), "_ZN7android3gui13LayerMetadataC2Ev"},
                            {reinterpret_cast<void **>(&SurfaceComposerClient__CreateSurface), "_ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjiiRKNS_2spINS_7IBinderEEENS_3gui13LayerMetadataEPj"},
                         },
                    },
                    {
                        15,
                        {
                            {reinterpret_cast<void **>(&LayerMetadata__Constructor), "_ZN7android3gui13LayerMetadataC2Ev"},
                            {reinterpret_cast<void **>(&SurfaceComposerClient__CreateSurface), "_ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjiiRKNS_2spINS_7IBinderEEENS_3gui13LayerMetadataEPj"},
                         },
                    },
                    {
                        14,
                        {
                            {reinterpret_cast<void **>(&LayerMetadata__Constructor), "_ZN7android3gui13LayerMetadataC2Ev"},
                            {reinterpret_cast<void **>(&SurfaceComposerClient__CreateSurface), "_ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjiiRKNS_2spINS_7IBinderEEENS_3gui13LayerMetadataEPj"},
                        },
                    },
                    {
                        12,
                        {
                            {reinterpret_cast<void **>(&SurfaceComposerClient__Transaction__Apply), "_ZN7android21SurfaceComposerClient11Transaction5applyEb"},
                        },
                    },
                    {
                        11,
                        {
                            {reinterpret_cast<void **>(&SurfaceComposerClient__CreateSurface), "_ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjijPNS_14SurfaceControlENS_13LayerMetadataEPj"},
                            {reinterpret_cast<void **>(&SurfaceControl__GetSurface), "_ZNK7android14SurfaceControl10getSurfaceEv"},
                        },
                    },
                    {
                        10,
                        {
                            {reinterpret_cast<void **>(&SurfaceComposerClient__CreateSurface), "_ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjijPNS_14SurfaceControlENS_13LayerMetadataE"},
                            {reinterpret_cast<void **>(&SurfaceControl__GetSurface), "_ZNK7android14SurfaceControl10getSurfaceEv"},
                        },
                    },
                    {
                        9,
                        {
                            {reinterpret_cast<void **>(&SurfaceComposerClient__CreateSurface_and9), "_ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjijPNS_14SurfaceControlEii"},
                            {reinterpret_cast<void **>(&SurfaceComposerClient__GetBuiltInDisplay), "_ZN7android21SurfaceComposerClient17getBuiltInDisplayEi"},
                            {reinterpret_cast<void **>(&SurfaceControl__GetSurface), "_ZNK7android14SurfaceControl10getSurfaceEv"},
                        },
                    },
                };

#ifdef __LP64__
                auto libgui = symbolMethod.Open("/system/lib64/libgui.so", RTLD_LAZY);
                auto libutils = symbolMethod.Open("/system/lib64/libutils.so", RTLD_LAZY);
#else
                auto libgui = symbolMethod.Open("/system/lib/libgui.so", RTLD_LAZY);
                auto libutils = symbolMethod.Open("/system/lib/libutils.so", RTLD_LAZY);
#endif

                ResolveMethod(RefBase, IncStrong, libutils, "_ZNK7android7RefBase9incStrongEPKv");
                ResolveMethod(RefBase, DecStrong, libutils, "_ZNK7android7RefBase9decStrongEPKv");

                ResolveMethod(String8, Constructor, libutils, "_ZN7android7String8C2EPKc");
                ResolveMethod(String8, Destructor, libutils, "_ZN7android7String8D2Ev");

                ResolveMethod(LayerMetadata, Constructor, libgui, "_ZN7android3gui13LayerMetadataC2Ev");
                ResolveMethod(LayerMetadata, setInt32, libgui, "_ZN7android3gui13LayerMetadata8setInt32Eji");


                ResolveMethod(SurfaceComposerClient, Constructor, libgui, "_ZN7android21SurfaceComposerClientC2Ev");
                ResolveMethod(SurfaceComposerClient, CreateSurface, libgui, "_ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjiiRKNS_2spINS_7IBinderEEENS_3gui13LayerMetadataEPj");
                if (systemVersion < 14) {
                ResolveMethod(SurfaceComposerClient, GetInternalDisplayToken, libgui, "_ZN7android21SurfaceComposerClient23getInternalDisplayTokenEv");  //小于或者等于安卓13
                ResolveMethod(SurfaceComposerClient, GetDisplayState, libgui, "_ZN7android21SurfaceComposerClient15getDisplayStateERKNS_2spINS_7IBinderEEEPNS_2ui12DisplayStateE");
                }
                if (systemVersion < 11) {
                ResolveMethod(SurfaceComposerClient, GetDisplayInfo, libgui, "_ZN7android21SurfaceComposerClient14getDisplayInfoERKNS_2spINS_7IBinderEEEPNS_11DisplayInfoE"); //安卓10及以下
                ResolveMethod(SurfaceComposerClient, GetPhysicalDisplayIds, libgui, "_ZN7android21SurfaceComposerClient21getPhysicalDisplayIdsEv");
                }
                ResolveMethod(SurfaceComposerClient, GetPhysicalDisplayIds, libgui, "_ZN7android21SurfaceComposerClient21getPhysicalDisplayIdsEv");
                ResolveMethod(SurfaceComposerClient, GetPhysicalDisplayToken, libgui, "_ZN7android21SurfaceComposerClient23getPhysicalDisplayTokenENS_17PhysicalDisplayIdE");

                ResolveMethod(SurfaceComposerClient, GetDisplayState, libgui, "_ZN7android21SurfaceComposerClient15getDisplayStateERKNS_2spINS_7IBinderEEEPNS_2ui12DisplayStateE");
                ResolveMethod(SurfaceComposerClient__Transaction, Constructor, libgui, "_ZN7android21SurfaceComposerClient11TransactionC2Ev");
                ResolveMethod(SurfaceComposerClient__Transaction, SetLayer, libgui, "_ZN7android21SurfaceComposerClient11Transaction8setLayerERKNS_2spINS_14SurfaceControlEEEi");
                ResolveMethod(SurfaceComposerClient__Transaction, SetTrustedOverlay, libgui, "_ZN7android21SurfaceComposerClient11Transaction17setTrustedOverlayERKNS_2spINS_14SurfaceControlEEEb");
                if (systemVersion >= 16) {
                    ResolveMethod(WindowInfoHandle, Constructor, libgui, "_ZN7android3gui16WindowInfoHandleC1Ev");
                    ResolveMethod(SurfaceComposerClient__Transaction, SetInputWindowInfo, libgui, "_ZN7android21SurfaceComposerClient11Transaction18setInputWindowInfoERKNS_2spINS_14SurfaceControlEEENS2_INS_3gui16WindowInfoHandleEEE");
                }
                ResolveMethod(SurfaceComposerClient__Transaction, Apply, libgui, "_ZN7android21SurfaceComposerClient11Transaction5applyEbb");

                if (systemVersion >= 13) {
                    ResolveMethod(SurfaceComposerClient, MirrorSurface, libgui, "_ZN7android21SurfaceComposerClient13mirrorSurfaceEPNS_14SurfaceControlES2_");
                    ResolveMethod(SurfaceComposerClient__Transaction, SetLayerStack, libgui, "_ZN7android21SurfaceComposerClient11Transaction13setLayerStackERKNS_2spINS_14SurfaceControlEEENS_2ui10LayerStackE");
                    ResolveMethod(SurfaceComposerClient__Transaction, Show, libgui, "_ZN7android21SurfaceComposerClient11Transaction4showERKNS_2spINS_14SurfaceControlEEE");
                    ResolveMethod(SurfaceComposerClient__Transaction, Reparent, libgui, "_ZN7android21SurfaceComposerClient11Transaction8reparentERKNS_2spINS_14SurfaceControlEEES6_");
                    ResolveMethod(SurfaceComposerClient__Transaction, SetMatrix, libgui, "_ZN7android21SurfaceComposerClient11Transaction9setMatrixERKNS_2spINS_14SurfaceControlEEEffff");
                    ResolveMethod(SurfaceComposerClient__Transaction, SetPosition, libgui, "_ZN7android21SurfaceComposerClient11Transaction11setPositionERKNS_2spINS_14SurfaceControlEEEff");
                }

                ResolveMethod(SurfaceControl, Validate, libgui, "_ZNK7android14SurfaceControl8validateEv");
                ResolveMethod(SurfaceControl, GetSurface, libgui, "_ZN7android14SurfaceControl10getSurfaceEv");
                ResolveMethod(SurfaceControl, DisConnect, libgui, "_ZN7android14SurfaceControl10disconnectEv");
                
                auto it = patchesTable.find(systemVersion);
                if (it != patchesTable.end()) {
                    for (const auto &[patchTo, signature] : patchesTable.at(systemVersion))
                    {
                        *patchTo = symbolMethod.Find(libgui, signature);
                        if (nullptr != *patchTo)
                            continue;

                        __android_log_print(ANDROID_LOG_ERROR, "ImGui", "[-] Patch method not found: %s", signature);
                    }
                }

                symbolMethod.Close(libutils);
                symbolMethod.Close(libgui);
            }

            static const Functionals &GetInstance(const SymbolMethod &symbolMethod = {.Open = dlopen, .Find = dlsym, .Close = dlclose}) {
                static Functionals functionals(symbolMethod);
                return functionals;
            }
        };

        struct String8
        {
            char data[1024];

            String8(const char *const string)
            {
                Functionals::GetInstance().String8__Constructor(data, string);
            }

            ~String8()
            {
                Functionals::GetInstance().String8__Destructor(data);
            }

            operator void *()
            {
                return reinterpret_cast<void *>(data);
            }
        };

        struct LayerMetadata {
            char data[1024];

            LayerMetadata() {
                if (9 < Functionals::GetInstance().systemVersion) {
                    Functionals::GetInstance().LayerMetadata__Constructor(data);
                }
            }
            
            void setInt32(uint32_t key, int32_t value) {
                Functionals::GetInstance().LayerMetadata__setInt32(data, key, value);            
            }
            
            operator void *() {
                if (9 < Functionals::GetInstance().systemVersion)
                    return reinterpret_cast<void *>(data);
                else
                    return nullptr;
            }
        };

        struct Surface {
        };

        struct SurfaceControl {
            void *data;

            SurfaceControl() : data(nullptr) {}
            SurfaceControl(void *data) : data(data) {}

            int32_t Validate() {
                if (nullptr == data)
                    return 0;

                return Functionals::GetInstance().SurfaceControl__Validate(data);
            }

            Surface *GetSurface() {
                if (nullptr == data)
                    return nullptr;

                auto result = Functionals::GetInstance().SurfaceControl__GetSurface(data);

                return reinterpret_cast<Surface *>(reinterpret_cast<size_t>(result.pointer) + sizeof(std::max_align_t) / 2);
            }

            void DisConnect() {
                if (nullptr == data)
                    return;

                Functionals::GetInstance().SurfaceControl__DisConnect(data);
            }

            void DestroySurface(Surface *surface) {
                if (nullptr == data || nullptr == surface)
                    return;

                Functionals::GetInstance().RefBase__DecStrong(reinterpret_cast<Surface *>(reinterpret_cast<size_t>(surface) - sizeof(std::max_align_t) / 2), this);
                DisConnect();
                Functionals::GetInstance().RefBase__DecStrong(data, this);
            }
        };

        struct SurfaceComposerClientTransaction {
            char data[1024];

            SurfaceComposerClientTransaction() {
                Functionals::GetInstance().SurfaceComposerClient__Transaction__Constructor(data);
            }

            void *SetLayer(StrongPointer<void> &surfaceControl, int32_t z) {
                return Functionals::GetInstance().SurfaceComposerClient__Transaction__SetLayer(data, surfaceControl, z);
            }

            void *SetTrustedOverlay(StrongPointer<void> &surfaceControl, bool isTrustedOverlay) {
                return Functionals::GetInstance().SurfaceComposerClient__Transaction__SetTrustedOverlay(data, surfaceControl, isTrustedOverlay);
            }

            void SetInputWindowInfoAllow(StrongPointer<void> &surfaceControl) {
                auto &f = Functionals::GetInstance();
                if (f.systemVersion < 16 || f.WindowInfoHandle__Constructor == nullptr ||
                    f.SurfaceComposerClient__Transaction__SetInputWindowInfo == nullptr)
                    return;

                void *handleStorage = std::malloc(0x228);
                if (handleStorage == nullptr)
                    return;
                f.WindowInfoHandle__Constructor(handleStorage);

                constexpr size_t windowInfoOffset = 0x38;
                constexpr size_t touchOcclusionModeOffset = windowInfoOffset + 0x160;
                constexpr size_t inputConfigOffset = windowInfoOffset + 0x188;
                auto *handleBytes = reinterpret_cast<uint8_t *>(handleStorage);

                auto *windowName = reinterpret_cast<std::string *>(
                        handleBytes + windowInfoOffset + 0x20);
                *windowName = "ImGuiInputPassThrough";
                *reinterpret_cast<int32_t *>(handleBytes + touchOcclusionModeOffset) = 2; // ALLOW
                *reinterpret_cast<uint32_t *>(handleBytes + inputConfigOffset) = 1; // NO_INPUT_CHANNEL

                void *handle = handleStorage;
                f.RefBase__IncStrong(handle, this);
                f.SurfaceComposerClient__Transaction__SetInputWindowInfo(data, surfaceControl, &handle);
                __android_log_print(ANDROID_LOG_INFO, "ImGui",
                                    "[input-window] handle=%p name=%s touch=%d config=0x%x submitted=%d",
                                    handleStorage,
                                    windowName->c_str(),
                                    *reinterpret_cast<int32_t *>(handleBytes + touchOcclusionModeOffset),
                                    *reinterpret_cast<uint32_t *>(handleBytes + inputConfigOffset),
                                    handle == nullptr ? 1 : 0);
            }

            void *SetLayerStack(StrongPointer<void> &surfaceControl, uint32_t layerStack) {
                return Functionals::GetInstance().SurfaceComposerClient__Transaction__SetLayerStack(data, surfaceControl, layerStack);
            }

            void Show(StrongPointer<void> &surfaceControl) {
                Functionals::GetInstance().SurfaceComposerClient__Transaction__Show(data, surfaceControl);
            }

            void Reparent(StrongPointer<void> &surfaceControl, StrongPointer<void> &newParent) {
                Functionals::GetInstance().SurfaceComposerClient__Transaction__Reparent(data, surfaceControl, newParent);
            }

            void SetMatrix(StrongPointer<void> &surfaceControl, float dsdx, float dtdx, float dtdy, float dsdy) {
                Functionals::GetInstance().SurfaceComposerClient__Transaction__SetMatrix(data, surfaceControl, dsdx, dtdx, dtdy, dsdy);
            }

            void SetPosition(StrongPointer<void> &surfaceControl, float x, float y) {
                Functionals::GetInstance().SurfaceComposerClient__Transaction__SetPosition(data, surfaceControl, x, y);
            }

            int32_t Apply(bool synchronous, bool oneWay) {
                if (12 >= Functionals::GetInstance().systemVersion)
                    return reinterpret_cast<int32_t (*)(void *, bool)>(Functionals::GetInstance().SurfaceComposerClient__Transaction__Apply)(data, synchronous);
                else
                    return Functionals::GetInstance().SurfaceComposerClient__Transaction__Apply(data, synchronous, oneWay);
            }
        };

        struct SurfaceComposerClient {
            char data[1024];

            SurfaceComposerClient() {
                Functionals::GetInstance().SurfaceComposerClient__Constructor(data);
                Functionals::GetInstance().RefBase__IncStrong(data, this);
            }

            SurfaceControl CreateSurface(const char *name, int32_t width, int32_t height, bool skipScrenshot) {
                void *parentHandle = nullptr;
                String8 windowName(name);
                LayerMetadata layerMetadata;
                if (skipScrenshot && (Functionals::GetInstance().systemVersion == 10 || Functionals::GetInstance().systemVersion == 11)) {
                    layerMetadata.setInt32(2u, 441731);
                }
                uint32_t flags = 0;
                if (skipScrenshot && Functionals::GetInstance().systemVersion >= 12) {
                    flags |= 0x40;
                }
                
                if (12 <= Functionals::GetInstance().systemVersion) {
                    static void *fakeParentHandleForBinder = nullptr;
                    parentHandle = &fakeParentHandleForBinder;
                }
                                
                StrongPointer<void> result;
                if (Functionals::GetInstance().systemVersion == 9) {
                    int32_t windowType = -1;
                    int32_t ownerUid = -1;
                    if (skipScrenshot) {
                        windowType = 441731;                    
                    } 
                    result = Functionals::GetInstance().SurfaceComposerClient__CreateSurface_and9(data, windowName, width, height, 1, flags, parentHandle, windowType, ownerUid);                
                } else if (Functionals::GetInstance().systemVersion >= 10) {
                    result = Functionals::GetInstance().SurfaceComposerClient__CreateSurface(data, windowName, width, height, 1, flags, parentHandle, layerMetadata, nullptr);
                }
                
                if (12 <= Functionals::GetInstance().systemVersion) {
                    static SurfaceComposerClientTransaction transaction;
                    transaction.SetTrustedOverlay(result, skipScrenshot);
                    if (!skipScrenshot) {
                        transaction.SetInputWindowInfoAllow(result);
                    }
                    transaction.Apply(false, true);
                }
                return {result.get()};
            }

            bool GetDisplayInfo(ui::DisplayState *displayInfo) {
                static StrongPointer<void> defaultDisplayToken{};

                if (defaultDisplayToken.get() == nullptr) {
                    if (9 >= Functionals::GetInstance().systemVersion) { //小于或者等于安卓9
                        defaultDisplayToken = Functionals::GetInstance().SurfaceComposerClient__GetBuiltInDisplay(ui::DisplayType::DisplayIdMain);
                    } else {
                        if (14 > Functionals::GetInstance().systemVersion) {//小于或者等于安卓13
                            defaultDisplayToken = Functionals::GetInstance().SurfaceComposerClient__GetInternalDisplayToken();
                        } else { //安卓14及以上
                            auto displayIds = Functionals::GetInstance().SurfaceComposerClient__GetPhysicalDisplayIds();
                            if (displayIds.empty())
                                return false;

                            defaultDisplayToken = Functionals::GetInstance().SurfaceComposerClient__GetPhysicalDisplayToken(displayIds[0]);
                        }
                    }
                }

                if (nullptr == defaultDisplayToken.get())
                    return false;

                if (11 <= Functionals::GetInstance().systemVersion) { //大于或者等于安卓11
                    return 0 == Functionals::GetInstance().SurfaceComposerClient__GetDisplayState(defaultDisplayToken, displayInfo);
                } else { //安卓10及以下
                    ui::DisplayInfo realDisplayInfo{};
                    if (0 != Functionals::GetInstance().SurfaceComposerClient__GetDisplayInfo(defaultDisplayToken, &realDisplayInfo))
                        return false;

                    displayInfo->layerStackSpaceRect.width = realDisplayInfo.w;
                    displayInfo->layerStackSpaceRect.height = realDisplayInfo.h;
                    displayInfo->orientation = static_cast<ui::Rotation>(realDisplayInfo.orientation);

                    return true;
                }
            }

            bool MirrorToLayerStack(SurfaceControl &source, uint32_t layerStack,
                                    int32_t width, int32_t height,
                                    int32_t sourceWidth, int32_t sourceHeight,
                                    int32_t orientation,
                                    SurfaceControl *mirrorOut, SurfaceControl *rootOut) {
                auto &f = Functionals::GetInstance();
                if (f.systemVersion < 13 || !f.SurfaceComposerClient__MirrorSurface ||
                    !f.SurfaceComposerClient__Transaction__SetLayerStack ||
                    !f.SurfaceComposerClient__Transaction__Show ||
                    !f.SurfaceComposerClient__Transaction__Reparent ||
                    !f.SurfaceComposerClient__Transaction__SetLayer ||
                    !f.SurfaceComposerClient__Transaction__SetMatrix ||
                    !f.SurfaceComposerClient__Transaction__SetPosition ||
                    !f.SurfaceComposerClient__Transaction__Apply ||
                    !source.data || width <= 0 || height <= 0)
                    return false;

                if (sourceWidth <= 0 || sourceHeight <= 0) {
                    sourceWidth = width;
                    sourceHeight = height;
                }

                int32_t rootWidth = sourceWidth;
                int32_t rootHeight = sourceHeight;
                if (orientation == 1 || orientation == 3)
                    std::swap(rootWidth, rootHeight);
                auto root = CreateSurface((std::string("ImGuiMirrorRoot@") + std::to_string(layerStack)).c_str(),
                                          rootWidth, rootHeight, false);
                if (!root.data) {
                    __android_log_print(ANDROID_LOG_ERROR, "ImGui", "[mirror] root creation failed, layerStack=%u", layerStack);
                    return false;
                }

                StrongPointer<void> rootPtr{root.data};
                SurfaceComposerClientTransaction transaction;
                transaction.SetLayerStack(rootPtr, layerStack);
                transaction.SetLayer(rootPtr, std::numeric_limits<int32_t>::max());
                transaction.Show(rootPtr);
                transaction.Apply(false, true);

                auto mirror = f.SurfaceComposerClient__MirrorSurface(data, source.data, root.data);
                if (!mirror.get()) {
                    root.DisConnect();
                    f.RefBase__DecStrong(root.data, this);
                    return false;
                }

                StrongPointer<void> mirrorPtr{mirror.get()};
                transaction.SetLayerStack(mirrorPtr, layerStack);
                transaction.Show(mirrorPtr);
                transaction.Reparent(mirrorPtr, rootPtr);

                const float targetWidth = static_cast<float>(std::min(width, height));
                const float targetHeight = static_cast<float>(std::max(width, height));
                float scaleX = targetWidth / static_cast<float>(sourceWidth);
                float scaleY = targetHeight / static_cast<float>(sourceHeight);
                int scaleIndex = 0;
                if (scaleX <= scaleY) {
                    scaleY = scaleX;
                    scaleIndex = 1;
                } else {
                    scaleX = scaleY;
                    scaleIndex = 2;
                }

                const bool offset = (orientation == 1 || orientation == 3) &&
                                    targetHeight != static_cast<float>(width);
                float dsdx = scaleX;
                float dtdx = 0.0f;
                float dtdy = 0.0f;
                float dsdy = scaleY;
                float positionX = 0.0f;
                float positionY = 0.0f;
                switch ((orientation % 4 + 4) % 4) {
                    case 1:
                        dsdx = 0.0f;
                        dtdx = scaleY;
                        dtdy = -scaleX;
                        dsdy = 0.0f;
                        break;
                    case 2:
                        dsdx = -scaleX;
                        dsdy = -scaleY;
                        break;
                    case 3:
                        dsdx = 0.0f;
                        dtdx = -scaleY;
                        dtdy = scaleX;
                        dsdy = 0.0f;
                        break;
                    default:
                        break;
                }

                if (offset) {
                    switch ((orientation % 4 + 4) % 4) {
                        case 0:
                            if (scaleIndex == 1)
                                positionY = (height - sourceHeight * scaleY) / 2.0f;
                            else
                                positionX = (width - sourceWidth * scaleX) / 2.0f;
                            break;
                        case 1:
                            if (scaleIndex == 1) {
                                positionX = targetWidth - (targetWidth - sourceWidth * scaleY) / 2.0f;
                            } else {
                                positionX = targetWidth;
                                positionY = (targetHeight - sourceHeight * scaleY) / 2.0f;
                            }
                            break;
                        case 2:
                            if (scaleIndex == 1) {
                                positionX = targetWidth - (targetWidth - sourceWidth * scaleX) / 2.0f;
                                positionY = targetHeight;
                            } else {
                                positionX = targetWidth;
                                positionY = targetHeight - (targetHeight - sourceHeight * scaleY) / 2.0f;
                            }
                            break;
                        case 3:
                            if (scaleIndex == 1) {
                                positionX = (targetWidth - sourceWidth * scaleX) / 2.0f;
                                positionY = targetHeight;
                            } else {
                                positionY = sourceHeight - (targetHeight - sourceHeight * scaleX) / 2.0f;
                            }
                            break;
                    }
                } else {
                    if (scaleIndex == 1) {
                        if (orientation == 1 || orientation == 3)
                            positionX = (targetHeight - sourceHeight * scaleY) / 2.0f;
                        else
                            positionY = (targetHeight - sourceHeight * scaleY) / 2.0f;
                    } else {
                        if (orientation == 1 || orientation == 3)
                            positionY = (targetWidth - sourceWidth * scaleX) / 2.0f;
                        else
                            positionX = (targetWidth - sourceWidth * scaleX) / 2.0f;
                    }
                }

                transaction.SetMatrix(mirrorPtr, dsdx, dtdx, dtdy, dsdy);
                transaction.SetPosition(mirrorPtr, positionX, positionY);
                transaction.Apply(false, true);

                *mirrorOut = {mirror.get()};
                *rootOut = root;
                __android_log_print(ANDROID_LOG_INFO, "ImGui",
                                    "[mirror] created layerStack=%u target=%dx%d source=%dx%d orientation=%d matrix=[%.3f %.3f %.3f %.3f] position=(%.1f,%.1f)",
                                    layerStack, width, height, sourceWidth, sourceHeight, orientation,
                                    dsdx, dtdx, dtdy, dsdy, positionX, positionY);
                return true;
            }
        };

    }

    class ANativeWindowCreator {
    public:
        struct DisplayInfo {
            int32_t orientation;
            int32_t width;
            int32_t height;
        };

    public:
        static detail::SurfaceComposerClient &GetComposerInstance() {
            static detail::SurfaceComposerClient surfaceComposerClient;
            return surfaceComposerClient;
        }

        static DisplayInfo GetDisplayInfo() {
            auto &surfaceComposerClient = GetComposerInstance();
            detail::ui::DisplayState displayInfo{};

            if (!surfaceComposerClient.GetDisplayInfo(&displayInfo))
                return {};
            
            DisplayInfo local_displayInfo{0};   
            int32_t local_orientation = static_cast<int32_t>(displayInfo.orientation);  
            int32_t local_abs_x = (displayInfo.layerStackSpaceRect.width > displayInfo.layerStackSpaceRect.height ? displayInfo.layerStackSpaceRect.width : displayInfo.layerStackSpaceRect.height);
            int32_t local_abs_y = (displayInfo.layerStackSpaceRect.width < displayInfo.layerStackSpaceRect.height ? displayInfo.layerStackSpaceRect.width : displayInfo.layerStackSpaceRect.height);          
            if (local_orientation == 1 || local_orientation == 3) {
                local_displayInfo.width = local_abs_x;
                local_displayInfo.height = local_abs_y;
            } else {
                local_displayInfo.width = local_abs_y;
                local_displayInfo.height = local_abs_x;
            }
            local_displayInfo.orientation = local_orientation;
            return local_displayInfo;
        }

        static ANativeWindow *Create(const char *name, int32_t width = -1, int32_t height = -1, bool skipScrenshot_ = false) {
            auto &surfaceComposerClient = GetComposerInstance();
            while (-1 == width || -1 == height) {
                detail::ui::DisplayState displayInfo{};

                if (!surfaceComposerClient.GetDisplayInfo(&displayInfo))
                    break;

                width = displayInfo.layerStackSpaceRect.width;
                height = displayInfo.layerStackSpaceRect.height;

                break;
            }

            auto surfaceControl = surfaceComposerClient.CreateSurface(name, width, height, skipScrenshot_);
            auto nativeWindow = reinterpret_cast<ANativeWindow *>(surfaceControl.GetSurface());

            m_cachedSurfaceControl.emplace(nativeWindow, std::move(surfaceControl));
            return nativeWindow;
        }

        static void Destroy(ANativeWindow *nativeWindow) {
            auto it = m_cachedSurfaceControl.find(nativeWindow);
            if (it == m_cachedSurfaceControl.end())
                return;

            ClearMirrorSurfaces();
            m_cachedSurfaceControl[nativeWindow].DestroySurface(reinterpret_cast<detail::Surface *>(nativeWindow));
            m_cachedSurfaceControl.erase(nativeWindow);
        }


        static void ProcessMirrorDisplay() {
            auto &f = detail::Functionals::GetInstance();
            if (f.systemVersion < 13 || m_cachedSurfaceControl.empty())
                return;

            static auto lastProcess = std::chrono::steady_clock::time_point{};
            const auto now = std::chrono::steady_clock::now();
            if (lastProcess != std::chrono::steady_clock::time_point{} &&
                now - lastProcess < std::chrono::seconds(1))
                return;
            lastProcess = now;

            auto source = m_cachedSurfaceControl.begin();
            if (!source->second.data)
                return;

            const auto dump = ReadDisplayDump();
            const auto primaryDisplay = GetDisplayInfo();
            int32_t primaryOrientation = primaryDisplay.orientation;

            const int32_t mirrorSourceWidth = (primaryOrientation == 1 || primaryOrientation == 3)
                    ? primaryDisplay.height : primaryDisplay.width;
            const int32_t mirrorSourceHeight = (primaryOrientation == 1 || primaryOrientation == 3)
                    ? primaryDisplay.width : primaryDisplay.height;

            struct MirrorTarget {
                uint32_t layerStack;
                int32_t width;
                int32_t height;
            };
            std::vector<MirrorTarget> targets;
            size_t cursor = 0;
            while (true) {
                const auto device = dump.find("DisplayDeviceInfo", cursor);
                if (device == std::string::npos)
                    break;

                const auto next = dump.find("DisplayDeviceInfo", device + 1);
                const auto end = next == std::string::npos ? dump.size() : next;
                const std::string_view block(dump.data() + device, end - device);
                cursor = end;

                const auto layerStack = ParseUnsignedField(block, "mCurrentLayerStack=");
                const auto rect = ParseRectField(block, "mCurrentLayerStackRect=");
                if (!layerStack || !rect || *layerStack == 0 || rect->first <= 0 || rect->second <= 0)
                    continue;

                targets.push_back({*layerStack, rect->first, rect->second});
                continue;
            }

            cursor = 0;
            while (true) {
                const auto device = dump.find("DisplayDeviceInfo", cursor);
                if (device == std::string::npos)
                    break;

                const auto next = dump.find("DisplayDeviceInfo", device + 1);
                const auto end = next == std::string::npos ? dump.size() : next;
                const std::string_view block(dump.data() + device, end - device);
                cursor = end;

                const auto layerStack = ParseUnsignedField(block, "mCurrentLayerStack=");
                if (layerStack && *layerStack == 0) {
                    if (const auto orientation = ParseUnsignedField(block, "mCurrentOrientation="))
                        primaryOrientation = static_cast<int32_t>(*orientation);
                    break;
                }
            }

            std::unordered_map<uint32_t, bool> seen;
            for (const auto &target : targets) {
                const auto layerStack = target.layerStack;
                const auto width = target.width;
                const auto height = target.height;
                seen[layerStack] = true;
                auto mirror = m_mirrorSurfaces.find(layerStack);
                if (mirror != m_mirrorSurfaces.end() &&
                    (mirror->second.width != width || mirror->second.height != height ||
                     mirror->second.orientation != primaryOrientation)) {
                    ReleaseMirrorPair(mirror->second);
                    m_mirrorSurfaces.erase(mirror);
                    mirror = m_mirrorSurfaces.end();
                }
                if (mirror == m_mirrorSurfaces.end()) {
                    MirrorSurfacePair pair{};
                    if (GetComposerInstance().MirrorToLayerStack(source->second, layerStack,
                                                                  width, height,
                                                                  mirrorSourceWidth, mirrorSourceHeight,
                                                                  primaryOrientation,
                                                                  &pair.mirror, &pair.root)) {
                        pair.width = width;
                        pair.height = height;
                        pair.orientation = primaryOrientation;
                        m_mirrorSurfaces.emplace(layerStack, pair);
                    }
                }
            }

            for (auto it = m_mirrorSurfaces.begin(); it != m_mirrorSurfaces.end();) {
                if (seen.find(it->first) == seen.end()) {
                    ReleaseMirrorPair(it->second);
                    it = m_mirrorSurfaces.erase(it);
                } else {
                    ++it;
                }
            }
        }

        static size_t GetMirrorSurfaceCount() {
            return m_mirrorSurfaces.size();
        }

        static void ClearMirrorSurfaces() {
            for (auto &entry : m_mirrorSurfaces)
                ReleaseMirrorPair(entry.second);
            m_mirrorSurfaces.clear();
        }

    private:
            struct MirrorSurfacePair {
                detail::SurfaceControl mirror{};
                detail::SurfaceControl root{};
                int32_t width = 0;
                int32_t height = 0;
                int32_t orientation = -1;
            };

        static std::string ReadDisplayDump() {
            std::string result;
            FILE *pipe = popen("dumpsys display", "r");
            if (!pipe)
                return result;

            char buffer[512]{};
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
                result += buffer;
            pclose(pipe);
            return result;
        }

        static std::optional<uint32_t> ParseUnsignedField(std::string_view block, std::string_view key) {
            const auto start = block.find(key);
            if (start == std::string_view::npos)
                return std::nullopt;
            auto value = block.substr(start + key.size());
            const auto end = value.find_first_of(" \t\r\n");
            value = value.substr(0, end);
            std::string text(value);
            char *parseEnd = nullptr;
            const auto parsed = std::strtoul(text.c_str(), &parseEnd, 10);
            if (parseEnd == text.c_str() || *parseEnd != '\0')
                return std::nullopt;
            return static_cast<uint32_t>(parsed);
        }

        static std::optional<std::pair<int32_t, int32_t>> ParseRectField(std::string_view block, std::string_view key) {
            const auto start = block.find(key);
            if (start == std::string_view::npos)
                return std::nullopt;
            const auto value = block.substr(start + key.size());
            const auto left = value.find('(');
            const auto comma = value.find(',', left == std::string_view::npos ? 0 : left + 1);
            const auto dash = value.find(" - ", comma == std::string_view::npos ? 0 : comma + 1);
            const auto rightComma = value.find(',', dash == std::string_view::npos ? 0 : dash + 3);
            if (left == std::string_view::npos || comma == std::string_view::npos ||
                dash == std::string_view::npos || rightComma == std::string_view::npos)
                return std::nullopt;
            const auto heightEnd = value.find(')', rightComma + 1);
            if (heightEnd == std::string_view::npos)
                return std::nullopt;
            const std::string widthText(value.substr(dash + 3, rightComma - dash - 3));
            const std::string heightText(value.substr(rightComma + 1, heightEnd - rightComma - 1));
            char *widthEnd = nullptr;
            char *heightParseEnd = nullptr;
            const auto width = std::strtol(widthText.c_str(), &widthEnd, 10);
            const auto height = std::strtol(heightText.c_str(), &heightParseEnd, 10);
            if (widthEnd == widthText.c_str() || *widthEnd != '\0' ||
                heightParseEnd == heightText.c_str() || *heightParseEnd != '\0')
                return std::nullopt;
            return std::make_pair(static_cast<int32_t>(width), static_cast<int32_t>(height));
        }

        static void ReleaseMirrorPair(MirrorSurfacePair &pair) {
            auto &f = detail::Functionals::GetInstance();
            if (pair.mirror.data) {
                f.SurfaceControl__DisConnect(pair.mirror.data);
                f.RefBase__DecStrong(pair.mirror.data, &pair);
                pair.mirror.data = nullptr;
            }
            if (pair.root.data) {
                f.SurfaceControl__DisConnect(pair.root.data);
                f.RefBase__DecStrong(pair.root.data, &pair);
                pair.root.data = nullptr;
            }
        }

        inline static std::unordered_map<ANativeWindow *, detail::SurfaceControl> m_cachedSurfaceControl;
        inline static std::unordered_map<uint32_t, MirrorSurfacePair> m_mirrorSurfaces;
    };
}

#undef ResolveMethod

#endif // !A_NATIVE_WINDOW_CREATOR_H
