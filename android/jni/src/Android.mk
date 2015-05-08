LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
	$(LOCAL_PATH)/../SDL2_ttf \
	$(LOCAL_PATH)/../SDL2_gfx \
	$(LOCAL_PATH)/../SDL2_image \
	$(LOCAL_PATH)/include

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	drawable/EDrawable.cpp \
	drawable/EFigure.cpp \
	drawable/ETexture.cpp \
	drawable/ETextureHandler.cpp \
	EScreenManager.cpp \
	EWindow.cpp \
	Ecore.cpp \
	main.cpp \

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_ttf SDL2_image SDL2_gfx

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
