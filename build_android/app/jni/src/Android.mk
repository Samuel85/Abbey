LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
SDL2_MIXER_PATH := ../SDL2_mixer


LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/$(SDL2_MIXER_PATH)
LOCAL_C_INCLUDES += $(wildcard $(LOCAL_PATH)*.h)

# Add your application source files here...
LOCAL_SRC_FILES :=  $(wildcard $(LOCAL_PATH)/*.cpp)


LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_mixer 

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -landroid

include $(BUILD_SHARED_LIBRARY)
