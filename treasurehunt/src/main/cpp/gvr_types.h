/* Copyright 2016 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef VR_GVR_CAPI_INCLUDE_GVR_TYPES_H_
#define VR_GVR_CAPI_INCLUDE_GVR_TYPES_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup types Google VR Types
/// @brief Various types used in the Google VR NDK.
/// @{

/// Primary context for invoking Google VR APIs.
typedef struct gvr_context_ gvr_context;

/// An enum for the left and right eye.
typedef enum {
  GVR_LEFT_EYE = 0,
  GVR_RIGHT_EYE,
  GVR_NUM_EYES
} gvr_eye;

/// The type of VR viewer.
typedef enum {
  /// A Cardboard-compatible viewer. A typical Cardboard viewer supports a
  /// simple touchscreen-based trigger input mechanism. On most platforms, this
  // is the default viewer type if no viewer has been explicitly paired.
  GVR_VIEWER_TYPE_CARDBOARD = 0,
  /// A Daydream-compatible viewer. A typical Daydream viewer supports 3DOF
  /// controller input (as defined in gvr_controller.h), and is intended only
  /// for Daydream-ready platforms. It does *not* support touchscreen-based
  /// input unless Cardboard emulation is explicitly enabled.
  GVR_VIEWER_TYPE_DAYDREAM = 1,
} gvr_viewer_type;

// Types of VR-specific features which may or may not be supported on the
// underlying platform.
typedef enum {
  // Asynchronous reprojection warps the app's rendered frame using the most
  // recent head pose just before pushing the frame to the display.
  GVR_FEATURE_ASYNC_REPROJECTION = 0,
  // Support for framebuffers suitable for rendering with the GL_OVR_multiview2
  // and GL_OVR_multiview_multisampled_render_to_texture extensions.
  GVR_FEATURE_MULTIVIEW = 1,
} gvr_feature;

/// @}

/// Version information for the Google VR API.
typedef struct gvr_version_ {
  int32_t major;
  int32_t minor;
  int32_t patch;
} gvr_version;

/// An integral 2D size. Used for render target sizes.
typedef struct gvr_sizei {
  int32_t width;
  int32_t height;
} gvr_sizei;

/// An integral 2D rect. Used for window bounds in pixels.
typedef struct gvr_recti {
  int32_t left;
  int32_t right;
  int32_t bottom;
  int32_t top;
} gvr_recti;

/// A floating point 2D rect. Used for field of view, and also for ranges
/// in texture space. When used for a field of view, all angles are in positive
/// degrees from the optical axis.
typedef struct gvr_rectf {
  float left;
  float right;
  float bottom;
  float top;
} gvr_rectf;

/// A floating point 2D vector.
typedef struct gvr_vec2f {
  float x;
  float y;
} gvr_vec2f;

/// A floating point 3D vector.
typedef struct gvr_vec3f {
  float x;
  float y;
  float z;
} gvr_vec3f;

/// A floating point 4x4 matrix.
typedef struct gvr_mat4f { float m[4][4]; } gvr_mat4f;

/// A floating point quaternion, in JPL format.
/// We use this simple struct in order not to impose a dependency on a
/// particular math library. The user of this API is free to encapsulate this
/// into any math library they want.
typedef struct gvr_quatf {
  /// qx, qy, qz are the vector component.
  float qx;
  float qy;
  float qz;
  /// qw is the linelar component.
  float qw;
} gvr_quatf;


/// Valid color formats for swap chain buffers.
typedef enum {
    /// Equivalent to GL_RGBA8
            GVR_COLOR_FORMAT_RGBA_8888 = 0,
    /// Equivalent to GL_RGB565
            GVR_COLOR_FORMAT_RGB_565 = 1,
} gvr_color_format_type;

typedef enum {
    /// No depth or stencil buffer.
            GVR_DEPTH_STENCIL_FORMAT_NONE = 255,
    /// Equivalent to GL_DEPTH_COMPONENT16.
            GVR_DEPTH_STENCIL_FORMAT_DEPTH_16 = 0,
    /// Equivalent to GL_DEPTH_COMPONENT24.
            GVR_DEPTH_STENCIL_FORMAT_DEPTH_24 = 1,
    /// Equivlaent to GL_DEPTH24_STENCIL8.
            GVR_DEPTH_STENCIL_FORMAT_DEPTH_24_STENCIL_8 = 2,
    /// Equivalent to GL_DEPTH_COMPONENT32F.
            GVR_DEPTH_STENCIL_FORMAT_DEPTH_32_F = 3,
    /// Equivalent to GL_DEPTH_32F_STENCIL8.
            GVR_DEPTH_STENCIL_FORMAT_DEPTH_32_F_STENCIL_8 = 4,
    /// Equivalent to GL_STENCIL8.
            GVR_DEPTH_STENCIL_FORMAT_STENCIL_8 = 5,
} gvr_depth_stencil_format_type;

/// Types of asynchronous reprojection.
typedef enum {
    /// Do not reproject.
    GVR_REPROJECTION_NONE = 0,
    /// Reproject in all dimensions.
    GVR_REPROJECTION_FULL = 1,
} gvr_reprojection;


/// A *monotonic system time* representation. On Android, this is equivalent to
/// System.nanoTime(), or clock_gettime(CLOCK_MONOTONIC). If there is any doubt
/// about how to get the current time for the current platform, simply use
/// gvr_get_time_point_now().
typedef struct gvr_clock_time_point {
  int64_t monotonic_system_time_nanos;
} gvr_clock_time_point;

/// A structure that ties together a region of a buffer, the field of view
/// rendered into that region and a target eye index to define part of the
/// user's field of view. The SDK implementation uses this information to
/// transform the images generated by the app output into the final display that
/// is sent to the screen.
///
/// A set of these structures will most often be generated by the API, via
/// gvr_get_recommended_buffer_viewports() or
/// gvr_get_screen_buffer_viewports(). However, the client may also customize
/// these values via gvr_buffer_viewport_list_set(), constructing a custom
/// gvr_buffer_viewport_list for use in the distortion pass.
typedef struct gvr_buffer_viewport_mj
{
//   char data[112] ;
    gvr_rectf view;          //16
    gvr_mat4f transformMat;  //64
    gvr_eye eye;             //4
    int buffer_index; //4
    char data1[8];    // todo
    int layer_index;
    char data11[4];   //todo
    gvr_reprojection reprojection;
    char data2[4];    //todo
}gvr_buffer_viewport;

/// List of buffer viewports that completely specifies how to transform the
/// frame's buffers into the image displayed on the screen.
typedef struct gvr_buffer_viewport_list_mj
{
    char data[16];
}gvr_buffer_viewport_list;

/// Specification of a pixel buffer. A pixel buffer can have color, depth and
/// stencil attachments and mostly corresponds to the OpenGL concept of a
/// framebuffer object. However, since there can be multiple such objects for
/// each frame, we avoid calling them "framebuffers". Pixel buffers which are
/// part of the currently acquired frame are immutable, i.e., they cannot be
/// resized or otherwise reconfigured.
//typedef struct gvr_buffer_spec_ gvr_buffer_spec;
typedef struct gvr_buffer_spec_mj
{
    int32_t width;
    int32_t height;
    gvr_color_format_type color_format;
    gvr_depth_stencil_format_type depth_stencil_format;
    int32_t num_samples;
    int32_t num_layers;
}gvr_buffer_spec;



/// Swap chain that contains some number of frames. Frames in the swap chain
/// can be unused, in the process of being distorted and presented on the
/// screen, or acquired and being rendered to by the application. The swap chain
/// ensures that the most recent available frame is always shown and that the
/// application never has to wait to render the next frame.
typedef struct gvr_swap_chain_mj
{
    char data[8];
    gvr_context *context;
//    char data1[80];
}gvr_swap_chain;

/// A single frame acquired from the swap chain. Each frame is composed of one
/// or more buffers, which are then lens distorted and composited into the final
/// output. Buffers are identified by indices that correspond to the position
/// of their gvr_buffer_spec in the list passed when constructing the swap
/// chain.
typedef struct gvr_frame_ gvr_frame;

/// @addtogroup types
/// @{

/// Constants that represent GVR error codes.
typedef enum {
  GVR_ERROR_NONE = 0,
  GVR_ERROR_CONTROLLER_CREATE_FAILED = 2,
  GVR_ERROR_NO_FRAME_AVAILABLE = 3,
} gvr_error;

/// Controller API options (bit flags).
enum {
  /// Indicates that controller orientation data should be reported.
  GVR_CONTROLLER_ENABLE_ORIENTATION = 1 << 0,
  /// Indicates that controller touchpad data should be reported.
  GVR_CONTROLLER_ENABLE_TOUCH = 1 << 1,
  /// Indicates that controller gyroscope data should be reported.
  GVR_CONTROLLER_ENABLE_GYRO = 1 << 2,
  /// Indicates that controller accelerometer data should be reported.
  GVR_CONTROLLER_ENABLE_ACCEL = 1 << 3,
  /// Indicates that controller gestures should be reported.
  GVR_CONTROLLER_ENABLE_GESTURES = 1 << 4,
  /// Indicates that controller pose prediction should be enabled.
  GVR_CONTROLLER_ENABLE_POSE_PREDICTION = 1 << 5,
  /// Indicates that controller position data should be reported.
  GVR_CONTROLLER_ENABLE_POSITION = 1 << 6,
  /// Indicates that controller battery data should be reported.
  GVR_CONTROLLER_ENABLE_BATTERY = 1 << 7,
};

/// Constants that represent the status of the controller API.
typedef enum {
  /// API is happy and healthy. This doesn't mean the controller itself
  /// is connected, it just means that the underlying service is working
  /// properly.
  GVR_CONTROLLER_API_OK = 0,

  /// Any other status represents a permanent failure that requires
  /// external action to fix:

  /// API failed because this device does not support controllers (API is too
  /// low, or other required feature not present).
  GVR_CONTROLLER_API_UNSUPPORTED = 1,
  /// This app was not authorized to use the service (e.g., missing permissions,
  /// the app is blacklisted by the underlying service, etc).
  GVR_CONTROLLER_API_NOT_AUTHORIZED = 2,
  /// The underlying VR service is not present.
  GVR_CONTROLLER_API_UNAVAILABLE = 3,
  /// The underlying VR service is too old, needs upgrade.
  GVR_CONTROLLER_API_SERVICE_OBSOLETE = 4,
  /// The underlying VR service is too new, is incompatible with current client.
  GVR_CONTROLLER_API_CLIENT_OBSOLETE = 5,
  /// The underlying VR service is malfunctioning. Try again later.
  GVR_CONTROLLER_API_MALFUNCTION = 6,
} gvr_controller_api_status;

/// Constants that represent the state of the controller.
typedef enum {
  /// Controller is disconnected.
  GVR_CONTROLLER_DISCONNECTED = 0,
  /// Controller is scanning.
  GVR_CONTROLLER_SCANNING = 1,
  /// Controller is connecting.
  GVR_CONTROLLER_CONNECTING = 2,
  /// Controller is connected.
  GVR_CONTROLLER_CONNECTED = 3,
} gvr_controller_connection_state;

/// Controller buttons.
typedef enum {
  GVR_CONTROLLER_BUTTON_NONE = 0,
  GVR_CONTROLLER_BUTTON_CLICK = 1,  ///< Touchpad Click.
  GVR_CONTROLLER_BUTTON_HOME = 2,
  GVR_CONTROLLER_BUTTON_APP = 3,
  GVR_CONTROLLER_BUTTON_VOLUME_UP = 4,
  GVR_CONTROLLER_BUTTON_VOLUME_DOWN = 5,

  /// Note: there are 5 buttons on the controller, but the state arrays have
  /// this many elements due to the inclusion of a dummy "none" button.
  GVR_CONTROLLER_BUTTON_COUNT = 6,
} gvr_controller_button;

/// Controller battery states.
typedef enum {
  GVR_CONTROLLER_BATTERY_LEVEL_UNKNOWN = 0,
  GVR_CONTROLLER_BATTERY_LEVEL_CRITICAL_LOW = 1,
  GVR_CONTROLLER_BATTERY_LEVEL_LOW = 2,
  GVR_CONTROLLER_BATTERY_LEVEL_MEDIUM = 3,
  GVR_CONTROLLER_BATTERY_LEVEL_ALMOST_FULL = 4,
  GVR_CONTROLLER_BATTERY_LEVEL_FULL = 5,

  /// Note: there are 5 distinct levels, but there are 6 due to the inclusion
  /// of an UNKNOWN state before any battery information is collected, etc.
  GVR_CONTROLLER_BATTERY_LEVEL_COUNT = 6,
} gvr_controller_battery_level;


/// @}

/// Opaque handle to controller state.
typedef struct gvr_controller_state_ gvr_controller_state;

/// @addtogroup types
/// @{

/// Rendering modes define CPU load / rendering quality balances.
typedef enum {
  /// Stereo panning of all Sound Objects. This disables HRTF-based rendering.
  GVR_AUDIO_RENDERING_STEREO_PANNING = 0,
  /// HRTF-based rendering over a virtual array of 8 loudspeakers arranged in
  /// a cube configuration around the listener’s head.
  GVR_AUDIO_RENDERING_BINAURAL_LOW_QUALITY = 1,
  /// HRTF-based rendering over a virtual array of 16 loudspeakers arranged in
  /// an approximate equidistribution about the around the listener’s head.
  GVR_AUDIO_RENDERING_BINAURAL_HIGH_QUALITY = 2,
} gvr_audio_rendering_mode;

/// Room surface material names, used to set room properties.
typedef enum {
  /// Acoustically transparent material, reflects no sound.
  GVR_AUDIO_MATERIAL_TRANSPARENT = 0,
  /// Acoustic ceiling tiles, absorbs most frequencies.
  GVR_AUDIO_MATERIAL_ACOUSTIC_CEILING_TILES = 1,
  /// Bare brick, relatively reflective.
  GVR_AUDIO_MATERIAL_BRICK_BARE = 2,
  /// Painted brick
  GVR_AUDIO_MATERIAL_BRICK_PAINTED = 3,
  /// Coarse surface concrete block.
  GVR_AUDIO_MATERIAL_CONCRETE_BLOCK_COARSE = 4,
  /// Painted concrete block.
  GVR_AUDIO_MATERIAL_CONCRETE_BLOCK_PAINTED = 5,
  /// Heavy curtains.
  GVR_AUDIO_MATERIAL_CURTAIN_HEAVY = 6,
  /// Fiber glass insulation.
  GVR_AUDIO_MATERIAL_FIBER_GLASS_INSULATION = 7,
  /// Thin glass.
  GVR_AUDIO_MATERIAL_GLASS_THIN = 8,
  /// Thick glass.
  GVR_AUDIO_MATERIAL_GLASS_THICK = 9,
  /// Grass.
  GVR_AUDIO_MATERIAL_GRASS = 10,
  /// Linoleum on concrete.
  GVR_AUDIO_MATERIAL_LINOLEUM_ON_CONCRETE = 11,
  /// Marble.
  GVR_AUDIO_MATERIAL_MARBLE = 12,
  /// Galvanized sheet metal.
  GVR_AUDIO_MATERIAL_METAL = 13,
  /// Wooden parquet on concrete.
  GVR_AUDIO_MATERIAL_PARQUET_ON_CONCRETE = 14,
  /// Rough plaster surface.
  GVR_AUDIO_MATERIAL_PLASTER_ROUGH = 15,
  /// Smooth plaster surface.
  GVR_AUDIO_MATERIAL_PLASTER_SMOOTH = 16,
  /// Plywood panel.
  GVR_AUDIO_MATERIAL_PLYWOOD_PANEL = 17,
  /// Polished concrete OR tile surface.
  GVR_AUDIO_MATERIAL_POLISHED_CONCRETE_OR_TILE = 18,
  /// Sheet rock.
  GVR_AUDIO_MATERIAL_SHEET_ROCK = 19,
  /// Surface of water or ice.
  GVR_AUDIO_MATERIAL_WATER_OR_ICE_SURFACE = 20,
  /// Wooden ceiling.
  GVR_AUDIO_MATERIAL_WOOD_CEILING = 21,
  /// Wood paneling.
  GVR_AUDIO_MATERIAL_WOOD_PANEL = 22,
} gvr_audio_material_type;

/// Distance rolloff models used for distance attenuation.
typedef enum {
  /// Logarithmic distance rolloff model.
  GVR_AUDIO_ROLLOFF_LOGARITHMIC = 0,
  /// Linear distance rolloff model.
  GVR_AUDIO_ROLLOFF_LINEAR = 1,
  /// No distance attenuation will be applied.
  GVR_AUDIO_ROLLOFF_NONE = 2,
} gvr_audio_distance_rolloff_type;

/// Sound object and sound field identifier.
typedef int32_t gvr_audio_source_id;

/// Supported surround sound formats.
typedef enum {
  // Enables to initialize a yet undefined rendering mode.
  GVR_AUDIO_SURROUND_FORMAT_INVALID = 0,

  // Virtual stereo speakers at -30 degrees and +30 degrees.
  GVR_AUDIO_SURROUND_FORMAT_SURROUND_STEREO = 1,

  // 5.1 surround sound according to the ITU-R BS 775 speaker configuration
  // recommendation:
  //   - Front left (FL) at 30 degrees.
  //   - Front right (FR) at -30 degrees.
  //   - Front center (FC) at 0 degrees.
  //   - Low frequency effects (LFE) at front center at 0 degrees.
  //   - Left side (LS) at 110 degrees.
  //   - Right side (RS) at -110 degrees.
  //
  // The 5.1 channel input layout must matches AAC: FL, FR, FC, LFE, LS, RS.
  // Note that this differs from the Vorbis/Opus 5.1 channel layout, which
  // is: FL, FC, FR, LS, RS, LFE.
  GVR_AUDIO_SURROUND_FORMAT_SURROUND_FIVE_DOT_ONE = 2,

  // First-order ambisonics (AmbiX format: 4 channels, ACN channel ordering,
  // SN3D normalization).
  GVR_AUDIO_SURROUND_FORMAT_FIRST_ORDER_AMBISONICS = 3,

  // Second-order ambisonics (AmbiX format: 9 channels, ACN channel ordering,
  // SN3D normalization).
  GVR_AUDIO_SURROUND_FORMAT_SECOND_ORDER_AMBISONICS = 4,

  // Third-order ambisonics (AmbiX format: 16 channels, ACN channel ordering,
  // SN3D normalization).
  GVR_AUDIO_SURROUND_FORMAT_THIRD_ORDER_AMBISONICS = 5,
} gvr_audio_surround_format_type;

typedef enum {
  GVR_CONTROLLER_RIGHT_HANDED = 0,
  GVR_CONTROLLER_LEFT_HANDED = 1,
} gvr_controller_handedness;

typedef struct gvr_user_prefs_ gvr_user_prefs;

// Anonymous enum for miscellaneous integer constants.
enum {
  /// Constant that represents a nonexistent external surface. Pass to
  /// gvr_buffer_viewport_set_external_surface_id() to disable sampling from
  /// an external surface.
  GVR_EXTERNAL_SURFACE_ID_NONE = -1,
  /// Special index for a source buffer that has the same contents as the
  /// external surface attached to the given viewport. Pass this to
  /// gvr_buffer_viewport_set_source_buffer_index() to use the external surface
  /// as the buffer contents.
  GVR_BUFFER_INDEX_EXTERNAL_SURFACE = -1,
};

/// @}

#ifdef __cplusplus
}  // extern "C"
#endif

#if defined(__cplusplus) && !defined(GVR_NO_CPP_WRAPPER)
// These typedefs convert the C-style names to C++-style names.

namespace gvr {

const int32_t kControllerEnableOrientation =
    static_cast<int32_t>(GVR_CONTROLLER_ENABLE_ORIENTATION);
const int32_t kControllerEnableTouch =
    static_cast<int32_t>(GVR_CONTROLLER_ENABLE_TOUCH);
const int32_t kControllerEnableGyro =
    static_cast<int32_t>(GVR_CONTROLLER_ENABLE_GYRO);
const int32_t kControllerEnableAccel =
    static_cast<int32_t>(GVR_CONTROLLER_ENABLE_ACCEL);
const int32_t kControllerEnableGestures =
    static_cast<int32_t>(GVR_CONTROLLER_ENABLE_GESTURES);
const int32_t kControllerEnablePosePrediction =
    static_cast<int32_t>(GVR_CONTROLLER_ENABLE_POSE_PREDICTION);
const int32_t kControllerEnablePosition =
    static_cast<int32_t>(GVR_CONTROLLER_ENABLE_POSITION);
const int32_t kControllerEnableBattery =
    static_cast<int32_t>(GVR_CONTROLLER_ENABLE_BATTERY);

typedef gvr_controller_api_status ControllerApiStatus;
const ControllerApiStatus kControllerApiOk =
    static_cast<ControllerApiStatus>(GVR_CONTROLLER_API_OK);
const ControllerApiStatus kControllerApiUnsupported =
    static_cast<ControllerApiStatus>(GVR_CONTROLLER_API_UNSUPPORTED);
const ControllerApiStatus kControllerApiNotAuthorized =
    static_cast<ControllerApiStatus>(GVR_CONTROLLER_API_NOT_AUTHORIZED);
const ControllerApiStatus kControllerApiUnavailable =
    static_cast<ControllerApiStatus>(GVR_CONTROLLER_API_UNAVAILABLE);
const ControllerApiStatus kControllerApiServiceObsolete =
    static_cast<ControllerApiStatus>(GVR_CONTROLLER_API_SERVICE_OBSOLETE);
const ControllerApiStatus kControllerApiClientObsolete =
    static_cast<ControllerApiStatus>(GVR_CONTROLLER_API_CLIENT_OBSOLETE);
const ControllerApiStatus kControllerApiMalfunction =
    static_cast<ControllerApiStatus>(GVR_CONTROLLER_API_MALFUNCTION);

typedef gvr_controller_connection_state ControllerConnectionState;
const ControllerConnectionState kControllerDisconnected =
    static_cast<ControllerConnectionState>(GVR_CONTROLLER_DISCONNECTED);
const ControllerConnectionState kControllerScanning =
    static_cast<ControllerConnectionState>(GVR_CONTROLLER_SCANNING);
const ControllerConnectionState kControllerConnecting =
    static_cast<ControllerConnectionState>(GVR_CONTROLLER_CONNECTING);
const ControllerConnectionState kControllerConnected =
    static_cast<ControllerConnectionState>(GVR_CONTROLLER_CONNECTED);

typedef gvr_controller_button ControllerButton;
const ControllerButton kControllerButtonNone =
    static_cast<ControllerButton>(GVR_CONTROLLER_BUTTON_NONE);
const ControllerButton kControllerButtonClick =
    static_cast<ControllerButton>(GVR_CONTROLLER_BUTTON_CLICK);
const ControllerButton kControllerButtonHome =
    static_cast<ControllerButton>(GVR_CONTROLLER_BUTTON_HOME);
const ControllerButton kControllerButtonApp =
    static_cast<ControllerButton>(GVR_CONTROLLER_BUTTON_APP);
const ControllerButton kControllerButtonVolumeUp =
    static_cast<ControllerButton>(GVR_CONTROLLER_BUTTON_VOLUME_UP);
const ControllerButton kControllerButtonVolumeDown =
    static_cast<ControllerButton>(GVR_CONTROLLER_BUTTON_VOLUME_DOWN);
const ControllerButton kControllerButtonCount =
    static_cast<ControllerButton>(GVR_CONTROLLER_BUTTON_COUNT);

typedef gvr_controller_battery_level ControllerBatteryLevel;
const ControllerBatteryLevel kControllerBatteryLevelUnknown =
    static_cast<ControllerBatteryLevel>(
        GVR_CONTROLLER_BATTERY_LEVEL_UNKNOWN);
const ControllerBatteryLevel kControllerBatteryLevelCriticalLow =
    static_cast<ControllerBatteryLevel>(
        GVR_CONTROLLER_BATTERY_LEVEL_CRITICAL_LOW);
const ControllerBatteryLevel kControllerBatteryLevelLow =
    static_cast<ControllerBatteryLevel>(
        GVR_CONTROLLER_BATTERY_LEVEL_LOW);
const ControllerBatteryLevel kControllerBatteryLevelMedium =
    static_cast<ControllerBatteryLevel>(
        GVR_CONTROLLER_BATTERY_LEVEL_MEDIUM);
const ControllerBatteryLevel kControllerBatteryLevelAlmostFull =
    static_cast<ControllerBatteryLevel>(
        GVR_CONTROLLER_BATTERY_LEVEL_ALMOST_FULL);
const ControllerBatteryLevel kControllerBatteryLevelFull =
    static_cast<ControllerBatteryLevel>(
        GVR_CONTROLLER_BATTERY_LEVEL_FULL);

/// An uninitialized external surface ID.
const int32_t kUninitializedExternalSurface = GVR_BUFFER_INDEX_EXTERNAL_SURFACE;
/// The default source buffer index for viewports.
const int32_t kDefaultBufferIndex = 0;

typedef gvr_eye Eye;

typedef gvr_viewer_type ViewerType;
const ViewerType kViewerTypeCardboard =
    static_cast<ViewerType>(GVR_VIEWER_TYPE_CARDBOARD);
const ViewerType kViewerTypeDaydream =
    static_cast<ViewerType>(GVR_VIEWER_TYPE_DAYDREAM);

typedef gvr_version Version;
typedef gvr_sizei Sizei;
typedef gvr_recti Recti;
typedef gvr_rectf Rectf;
typedef gvr_vec2f Vec2f;
typedef gvr_vec3f Vec3f;
typedef gvr_mat4f Mat4f;
typedef gvr_quatf Quatf;
typedef gvr_clock_time_point ClockTimePoint;

typedef gvr_vec2f ControllerVec2;
typedef gvr_vec3f ControllerVec3;
typedef gvr_quatf ControllerQuat;

typedef gvr_audio_rendering_mode AudioRenderingMode;
typedef gvr_audio_material_type AudioMaterialName;
typedef gvr_audio_distance_rolloff_type AudioRolloffMethod;
typedef gvr_audio_source_id AudioSourceId;
typedef gvr_audio_surround_format_type AudioSurroundFormat;

typedef gvr_color_format_type ColorFormat;
const ColorFormat kColorFormatRgba8888 =
    static_cast<ColorFormat>(GVR_COLOR_FORMAT_RGBA_8888);
const ColorFormat kColorFormatRgb565 =
    static_cast<ColorFormat>(GVR_COLOR_FORMAT_RGB_565);

typedef gvr_depth_stencil_format_type DepthStencilFormat;
const DepthStencilFormat kDepthStencilFormatNone =
    static_cast<DepthStencilFormat>(GVR_DEPTH_STENCIL_FORMAT_NONE);
const DepthStencilFormat kDepthStencilFormatDepth16 =
    static_cast<DepthStencilFormat>(GVR_DEPTH_STENCIL_FORMAT_DEPTH_16);
const DepthStencilFormat kDepthStencilFormatDepth24 =
    static_cast<DepthStencilFormat>(GVR_DEPTH_STENCIL_FORMAT_DEPTH_24);
const DepthStencilFormat kDepthStencilFormatDepth24Stencil8 =
    static_cast<DepthStencilFormat>(
        GVR_DEPTH_STENCIL_FORMAT_DEPTH_24_STENCIL_8);
const DepthStencilFormat kDepthStencilFormatDepth32f =
    static_cast<DepthStencilFormat>(GVR_DEPTH_STENCIL_FORMAT_DEPTH_32_F);
const DepthStencilFormat kDepthStencilFormatDepth32fStencil8 =
    static_cast<DepthStencilFormat>(
        GVR_DEPTH_STENCIL_FORMAT_DEPTH_32_F_STENCIL_8);
const DepthStencilFormat kDepthStencilFormatStencil8 =
    static_cast<DepthStencilFormat>(GVR_DEPTH_STENCIL_FORMAT_STENCIL_8);

typedef gvr_controller_handedness ControllerHandedness;
const ControllerHandedness kControllerRightHanded =
    static_cast<ControllerHandedness>(GVR_CONTROLLER_RIGHT_HANDED);
const ControllerHandedness kControllerLeftHanded =
    static_cast<ControllerHandedness>(GVR_CONTROLLER_LEFT_HANDED);

typedef gvr_error Error;
const Error kErrorNone = static_cast<Error>(GVR_ERROR_NONE);
const Error kErrorControllerCreateFailed =
    static_cast<Error>(GVR_ERROR_CONTROLLER_CREATE_FAILED);
const Error kErrorNoFrameAvailable =
    static_cast<Error>(GVR_ERROR_NO_FRAME_AVAILABLE);

class AudioApi;
class BufferSpec;
class ControllerApi;
class ControllerState;
class Frame;
class GvrApi;
class BufferViewport;
class BufferViewportList;
class SwapChain;
class UserPrefs;

}  // namespace gvr

// Non-member equality operators for convenience. Since typedefs do not trigger
// argument-dependent lookup, these operators have to be defined for the
// underlying types.
inline bool operator==(const gvr_vec2f& lhs, const gvr_vec2f& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const gvr_vec2f& lhs, const gvr_vec2f& rhs) {
  return !(lhs == rhs);
}

inline bool operator==(const gvr_vec3f& lhs, const gvr_vec3f& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const gvr_vec3f& lhs, const gvr_vec3f& rhs) {
  return !(lhs == rhs);
}

inline bool operator==(const gvr_recti& lhs, const gvr_recti& rhs) {
  return lhs.left == rhs.left && lhs.right == rhs.right &&
         lhs.bottom == rhs.bottom && lhs.top == rhs.top;
}

inline bool operator!=(const gvr_recti& lhs, const gvr_recti& rhs) {
  return !(lhs == rhs);
}

inline bool operator==(const gvr_rectf& lhs, const gvr_rectf& rhs) {
  return lhs.left == rhs.left && lhs.right == rhs.right &&
         lhs.bottom == rhs.bottom && lhs.top == rhs.top;
}

inline bool operator!=(const gvr_rectf& lhs, const gvr_rectf& rhs) {
  return !(lhs == rhs);
}

inline bool operator==(const gvr_sizei& lhs, const gvr_sizei& rhs) {
  return lhs.width == rhs.width && lhs.height == rhs.height;
}

inline bool operator!=(const gvr_sizei& lhs, const gvr_sizei& rhs) {
  return !(lhs == rhs);
}

#endif  // #if defined(__cplusplus) && !defined(GVR_NO_CPP_WRAPPER)

#endif  // VR_GVR_CAPI_INCLUDE_GVR_TYPES_H_
