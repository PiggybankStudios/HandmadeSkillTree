/*
File:   orca_aliases.h
Author: Taylor Robbins
Date:   09\21\2024
Description:
	** In order to make Orca feel more like home, we are going to make aliases of
	** all the types and functions we want to use, in the naming convention I
	** normally employ on my other projects. This also serves as a handy reference
	** file for all the types and functions so I don't have to reference the actual
	** Orca source code every time I want to remember a function signature.
	** We'll also give this file the normal treatment we would do for a file in
	** gylib, where we have an Auto-Complete dictionary at the bottom, and there
	** are nice large headers that can be looked up and jumped to easily
	** (using the custom Sublime syntax that makes them into jump points)
*/

#ifndef _ORCA_ALIASES_H
#define _ORCA_ALIASES_H

// +--------------------------------------------------------------+
// |                         Basic Macros                         |
// +--------------------------------------------------------------+
//NOTE: Clang treats basic "inline" a bit different than I expected, "static inline" does what I want
#define INLINE static inline
#define EXPORT ORCA_EXPORT

// +--------------------------------------------------------------+
// |                            Types                             |
// +--------------------------------------------------------------+
// +==============================+
// |       Number Typedefs        |
// +==============================+
// These types are already provided by Orca and match the names I give them normally: u8, u16, u32, u64, i8, i16, i32, i64
typedef f32 r32;
typedef f64 r64;

// +==============================+
// |   Opaque Orca Type Aliases   |
// +==============================+
typedef oc_mouse_button                 OC_MouseButton_t;
typedef oc_scan_code                    OC_ScanCode_t;
typedef oc_key_code                     OC_KeyCode_t;
typedef oc_surface                      OC_Surface_t;
typedef oc_color_space                  OC_ColorSpace_t;
typedef oc_canvas_renderer              OC_CanvasRenderer_t;
typedef oc_canvas_context               OC_CanvasContext_t;
typedef oc_file                         OC_File_t;
typedef oc_font                         OC_Font_t;
typedef oc_image                        OC_Image_t;
typedef oc_rect_atlas                   OC_RectAtlas_t;
typedef oc_image_region                 OC_ImageRegion_t;
typedef oc_gradient_blend_space         OC_GradientBlendSpace_t;
typedef oc_unicode_range                OC_UnicodeRange_t;
typedef oc_arena                        OC_Arena_t;
typedef oc_arena_options                OC_ArenaOptions_t;
typedef oc_arena_scope                  OC_ArenaScope_t;
typedef oc_font_metrics                 OC_FontMetrics_t;
typedef oc_text_metrics                 OC_TextMetrics_t;
typedef oc_joint_type                   OC_JointType_t;
typedef oc_cap_type                     OC_CapType_t;
typedef oc_io_cmp                       OC_IoCmp_t;
typedef oc_io_req                       OC_IoReq_t;
typedef oc_file_access                  OC_FileAccess_t;
typedef oc_file_open_flags              OC_FileOpenFlags_t;
typedef oc_file_whence                  OC_FileWhence_t;
typedef oc_file_dialog_desc             OC_FileDialogDesc_t;
typedef oc_io_error                     OC_IoError_t;
typedef oc_file_status                  OC_FileStatus_t;
typedef oc_file_open_with_dialog_result OC_FileOpenWithDialogResult_t;
typedef oc_ui_context                   OC_UiContext_t;
typedef oc_event                        OC_Event_t;
typedef oc_ui_style                     OC_UiStyle_t;
typedef oc_ui_style_mask                OC_UiStyleMask_t;
typedef oc_ui_sig                       OC_UiSig_t;
typedef oc_ui_box                       OC_UiBox_t;
typedef oc_ui_text_box_result           OC_UiTextBoxResult_t;
typedef oc_ui_select_popup_info         OC_UiSelectPopupInfo_t;
typedef oc_ui_radio_group_info          OC_UiRadioGroupInfo_t;
typedef oc_ui_flags                     OC_UiFlags_t;
typedef oc_ui_pattern                   OC_UiPattern_t;
typedef oc_ui_selector                  OC_UiSelector_t;
typedef oc_list                         OC_List_t;
typedef oc_list_elt                     OC_ListElement_t;

// +==============================+
// |           Strings            |
// +==============================+
union MyStr_t
{
	oc_str8 oc;
	struct
	{
		union { char* pntr; char* chars; u8* bytes; };
		u32 length; //NOTE: THis is actually size_t in oc_str8, but I'd rather deal with known 32-bit value until we actuall have WASM64 support
	};
};
typedef MyStr_t OC_Str8_t;
//TODO: Can we choose a shorter alias that makes sense and doesn't conflict with standard library types?

//TODO: Alias for oc_str8_list
//TODO: Alias for oc_str32
//TODO: Alias for oc_utf32

// +==============================+
// |           Vectors            |
// +==============================+
union Vec2_t
{
	oc_vec2 oc;
	r32 values[2];
	struct { r32 x, y; };
	struct { r32 width, height; };
};
typedef Vec2_t v2;

union Vec3_t
{
	oc_vec3 oc;
	r32 values[3];
	struct { r32 x, y, z; };
	struct { r32 width, height, depth; };
	struct { r32 r, g, b; };
	struct { r32 red, green, blue; };
};
typedef Vec3_t v3;

union Vec4_t
{
	oc_vec4 oc;
	r32 values[4];
	struct { r32 x, y, z, w; };
	struct { r32 width, height, depth, time; };
	struct { r32 r, g, b, a; };
	struct { r32 red, green, blue, alpha; };
};
typedef Vec4_t v4;

union Vec2i_t
{
	oc_vec2i oc;
	i32 values[2];
	struct { i32 x, y; };
	struct { i32 width, height; };
};
typedef Vec2i_t v2i;

//TODO: Should we add v3i and v4i?

// +==============================+
// |            Colors            |
// +==============================+
union Colorf_t
{
	oc_color oc;
	v4 values;
	struct { r32 r, g, b, a; };
	struct { r32 red, green, blue, alpha; };
};
typedef Colorf_t colf;

//TODO: Should we add Color_t?

// +==============================+
// |           Matrices           |
// +==============================+
union Matrix2x3_t
{
	oc_mat2x3 oc;
	//         R  C
	r32 values[3][2]; //TODO: Is this the right order?
	struct { v3 row0; v3 row1; };
	struct
	{
		r32 r0c0, r0c1, r0c2;
		r32 r1c0, r1c1, r1c2;
	};
};
typedef Matrix2x3_t mat23;

//TODO: Should we add mat4? Or even mat3?

// +==============================+
// |          Rectangles          |
// +==============================+
union Rectangle_t
{
	oc_rect oc;
	struct
	{
		union
		{
			v2 topLeft;
			struct { r32 x, y; };
			struct { r32 left, top; };
		};
		union
		{
			v2 size;
			struct { r32 width, height; };
		};
	};
};
typedef Rectangle_t rec;

//TODO: Should we add reci, box? obb2? etc?

//TODO: Should we add Color_t and Colorf_t?

// +--------------------------------------------------------------+
// |              Basic Vector / Rectangle Functions              |
// +--------------------------------------------------------------+
// +==============================+
// |     NewStruct Functions      |
// +==============================+
INLINE MyStr_t NewStr(const char* nullTermStr)
{
	MyStr_t result;
	// TODO: Use MyStrLength64 alias if we end up adding that!
	result.length = ((nullTermStr != nullptr) ? (u64)strlen(nullTermStr) : 0);
	result.pntr = (char*)nullTermStr;
	return result;
}
INLINE MyStr_t NewStr(u64 length, const char* pntr)
{
	MyStr_t result;
	result.length = length;
	result.pntr = (char*)pntr;
	return result;
}
INLINE v2 NewVec2(r32 x, r32 y)
{
	v2 result;
	result.x = x;
	result.y = y;
	return result;
}
INLINE v3 NewVec3(r32 x, r32 y, r32 z)
{
	v3 result;
	result.x = x;
	result.y = y;
	result.z = z;
	return result;
}
INLINE v4 NewVec4(r32 x, r32 y, r32 z, r32 w)
{
	v4 result;
	result.x = x;
	result.y = y;
	result.z = z;
	result.w = w;
	return result;
}
INLINE v2i NewVec2i(i32 x, i32 y)
{
	v2i result;
	result.x = x;
	result.y = y;
	return result;
}
INLINE mat23 NewMat23(
	r32 r0c0, r32 r0c1, r32 r0c2,
	r32 r1c0, r32 r1c1, r32 r1c2)
{
	mat23 result;
	result.r0c0 = r0c0; result.r0c1 = r0c1; result.r0c2 = r0c2;
	result.r1c0 = r1c0; result.r1c1 = r1c1; result.r1c2 = r1c2;
	return result;
}
INLINE rec NewRec(r32 x, r32 y, r32 width, r32 height)
{
	rec result;
	result.x = x;
	result.y = y;
	result.width = width;
	result.height = height;
	return result;
}
INLINE rec NewRec(v2 topLeft, r32 width, r32 height)
{
	rec result;
	result.x = topLeft.x;
	result.y = topLeft.y;
	result.width = width;
	result.height = height;
	return result;
}
INLINE rec NewRec(r32 x, r32 y, v2 size)
{
	rec result;
	result.x = x;
	result.y = y;
	result.width = size.width;
	result.height = size.height;
	return result;
}
INLINE rec NewRec(v2 topLeft, v2 size)
{
	rec result;
	result.x = topLeft.x;
	result.y = topLeft.y;
	result.width = size.width;
	result.height = size.height;
	return result;
}
INLINE colf NewColorf(r32 r, r32 g, r32 b, r32 a)
{
	colf result;
	result.r = r;
	result.g = g;
	result.b = b;
	result.a = a;
	return result;
}
INLINE colf NewColorfBytes(u8 r, u8 g, u8 b, u8 a)
{
	colf result;
	result.r = (r32)r / 255.0f;
	result.g = (r32)g / 255.0f;
	result.b = (r32)b / 255.0f;
	result.a = (r32)a / 255.0f;
	return result;
}
INLINE colf NewColorfHex(u32 packedColorBgra)
{
	colf result;
	result.b = (r32)((packedColorBgra >>  0) & 0xFF) / 255.0f;
	result.g = (r32)((packedColorBgra >>  8) & 0xFF) / 255.0f;
	result.r = (r32)((packedColorBgra >> 16) & 0xFF) / 255.0f;
	result.a = (r32)((packedColorBgra >> 24) & 0xFF) / 255.0f;
	return result;
}

INLINE MyStr_t ToStr(oc_str8 ocString)     { return NewStr(ocString.len, ocString.ptr); }
INLINE v2      ToVec2(oc_vec2 ocVector)    { return NewVec2(ocVector.x, ocVector.y); }
INLINE v3      ToVec3(oc_vec3 ocVector)    { return NewVec3(ocVector.x, ocVector.y, ocVector.z); }
INLINE v4      ToVec4(oc_vec4 ocVector)    { return NewVec4(ocVector.x, ocVector.y, ocVector.z, ocVector.w); }
INLINE v2i     ToVec2i(oc_vec2i ocVector)  { return NewVec2i(ocVector.x, ocVector.y); }
INLINE rec     ToRec(oc_rect ocRectangle)  { return NewRec(ocRectangle.x, ocRectangle.y, ocRectangle.w, ocRectangle.h); }
INLINE mat23   ToMat23(oc_mat2x3 ocMatrix) { return NewMat23(ocMatrix.m[0], ocMatrix.m[1], ocMatrix.m[2], ocMatrix.m[3], ocMatrix.m[4], ocMatrix.m[5]); }
INLINE colf    ToColorf(oc_color ocColor)  { return NewColorf(ocColor.r, ocColor.g, ocColor.b, ocColor.a); }

// +===============================+
// | Operator Overload Equivalents |
// +===============================+
INLINE v2 Vec2Add(v2 left, v2 right)
{
	v2 result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	return result;
}
INLINE v2 Vec2Subtract(v2 left, v2 right)
{
	v2 result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	return result;
}
INLINE v2 Vec2Scale(v2 vector, r32 scalar)
{
	v2 result;
	result.x = vector.x * scalar;
	result.y = vector.y * scalar;
	return result;
}
//TODO: Vec2Multiply?
INLINE v2 Vec2Shrink(v2 vector, r32 divisor)
{
	v2 result;
	result.x = vector.x / divisor;
	result.y = vector.y / divisor;
	return result;
}
//TODO: Vec2Divide?
//TODO: Vec2BasicallyEqual?

INLINE v3 Vec3Add(v3 left, v3 right)
{
	v3 result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	result.z = left.z + right.z;
	return result;
}
INLINE v3 Vec3Subtract(v3 left, v3 right)
{
	v3 result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	result.z = left.z - right.z;
	return result;
}
INLINE v3 Vec3Scale(v3 vector, r32 scalar)
{
	v3 result;
	result.x = vector.x * scalar;
	result.y = vector.y * scalar;
	result.z = vector.z * scalar;
	return result;
}
//TODO: Vec3Multiply?
INLINE v3 Vec3Shrink(v3 vector, r32 divisor)
{
	v3 result;
	result.x = vector.x / divisor;
	result.y = vector.y / divisor;
	result.z = vector.z / divisor;
	return result;
}
//TODO: Vec3Divide?
//TODO: Vec3BasicallyEqual?

INLINE v4 Vec4Add(v4 left, v4 right)
{
	v4 result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	result.z = left.z + right.z;
	result.w = left.w + right.w;
	return result;
}
INLINE v4 Vec4Subtract(v4 left, v4 right)
{
	v4 result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	result.z = left.z - right.z;
	result.w = left.w - right.w;
	return result;
}
INLINE v4 Vec4Scale(v4 vector, r32 scalar)
{
	v4 result;
	result.x = vector.x * scalar;
	result.y = vector.y * scalar;
	result.z = vector.z * scalar;
	result.w = vector.w * scalar;
	return result;
}
//TODO: Vec4Multiply?
INLINE v4 Vec4Shrink(v4 vector, r32 divisor)
{
	v4 result;
	result.x = vector.x / divisor;
	result.y = vector.y / divisor;
	result.z = vector.z / divisor;
	result.w = vector.w / divisor;
	return result;
}
//TODO: Vec4Divide?
//TODO: Vec4BasicallyEqual?

INLINE v2i Vec2iAdd(v2i left, v2i right)
{
	v2i result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	return result;
}
INLINE v2i Vec2iSubtract(v2i left, v2i right)
{
	v2i result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	return result;
}
INLINE v2i Vec2iScale(v2i vector, i32 scalar)
{
	v2i result;
	result.x = vector.x * scalar;
	result.y = vector.y * scalar;
	return result;
}
//TODO: Vec2iMultiply?
INLINE v2i Vec2iShrink(v2i vector, i32 divisor)
{
	v2i result;
	result.x = vector.x / divisor;
	result.y = vector.y / divisor;
	return result;
}
//TODO: Vec2iDivide?
//TODO: Vec2iBasicallyEqual?

//TODO: RecShift(x, y)?
INLINE rec RecShift(rec rectangle, v2 amount)
{
	rec result;
	result.x = rectangle.x + amount.x;
	result.y = rectangle.y + amount.y;
	result.size = rectangle.size;
	return result;
}
INLINE rec RecScale(rec rectangle, r32 scalar)
{
	rec result;
	result.x = rectangle.x * scalar;
	result.y = rectangle.y * scalar;
	result.width = rectangle.width * scalar;
	result.height = rectangle.height * scalar;
	return result;
}
//TODO: RecScale2/
//TODO: RecBasicallyEqual?

// +==============================+
// |      Operator Overloads      |
// +==============================+
INLINE v2 operator - (v2 vector)                            { return NewVec2(-vector.x, -vector.y); }
INLINE v2 operator + (v2 left, v2 right)                    { return Vec2Add(left, right); }
INLINE v2 operator - (v2 left, v2 right)                    { return Vec2Subtract(left, right); }
INLINE v2 operator * (v2 vector, r32 scalar)                { return Vec2Scale(vector, scalar); }
INLINE v2 operator * (r32 scalar, v2 vector)                { return Vec2Scale(vector, scalar); }
INLINE v2 operator / (v2 vector, r32 scalar)                { return Vec2Shrink(vector, scalar); }
INLINE bool operator == (v2 left, v2 right)                 { return (left.x == right.x && left.y == right.y); }
INLINE bool operator != (v2 left, v2 right)                 { return (left.x != right.x || left.y != right.y); }
INLINE void operator += (v2& leftSide, const v2& rightSide) { leftSide.x += rightSide.x; leftSide.y += rightSide.y; }
INLINE void operator -= (v2& leftSide, const v2& rightSide) { leftSide.x -= rightSide.x; leftSide.y -= rightSide.y; }
INLINE void operator *= (v2& leftSide, const r32 rightSide) { leftSide.x *= rightSide; leftSide.y *= rightSide; }
INLINE void operator /= (v2& leftSide, const r32 rightSide) { leftSide.x /= rightSide; leftSide.y /= rightSide; }

INLINE v3 operator - (v3 vector)                            { return NewVec3(-vector.x, -vector.y, -vector.z); }
INLINE v3 operator + (v3 left, v3 right)                    { return Vec3Add(left, right); }
INLINE v3 operator - (v3 left, v3 right)                    { return Vec3Subtract(left, right); }
INLINE v3 operator * (v3 vector, r32 scalar)                { return Vec3Scale(vector, scalar); }
INLINE v3 operator * (r32 scalar, v3 vector)                { return Vec3Scale(vector, scalar); }
INLINE v3 operator / (v3 vector, r32 scalar)                { return Vec3Shrink(vector, scalar); }
INLINE bool operator == (v3 left, v3 right)                 { return (left.x == right.x && left.y == right.y && left.z == right.z); }
INLINE bool operator != (v3 left, v3 right)                 { return (left.x != right.x || left.y != right.y || left.z != right.z); }
INLINE void operator += (v3& leftSide, const v3& rightSide) { leftSide.x += rightSide.x; leftSide.y += rightSide.y; leftSide.z += rightSide.z; }
INLINE void operator -= (v3& leftSide, const v3& rightSide) { leftSide.x -= rightSide.x; leftSide.y -= rightSide.y; leftSide.z -= rightSide.z; }
INLINE void operator *= (v3& leftSide, const r32 rightSide) { leftSide.x *= rightSide; leftSide.y *= rightSide; leftSide.z *= rightSide; }
INLINE void operator /= (v3& leftSide, const r32 rightSide) { leftSide.x /= rightSide; leftSide.y /= rightSide; leftSide.z /= rightSide; }

INLINE v4 operator - (v4 vector)                            { return NewVec4(-vector.x, -vector.y, -vector.z, -vector.w); }
INLINE v4 operator + (v4 left, v4 right)                    { return Vec4Add(left, right); }
INLINE v4 operator - (v4 left, v4 right)                    { return Vec4Subtract(left, right); }
INLINE v4 operator * (v4 vector, r32 scalar)                { return Vec4Scale(vector, scalar); }
INLINE v4 operator * (r32 scalar, v4 vector)                { return Vec4Scale(vector, scalar); }
INLINE v4 operator / (v4 vector, r32 scalar)                { return Vec4Shrink(vector, scalar); }
INLINE bool operator == (v4 left, v4 right)                 { return (left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w); }
INLINE bool operator != (v4 left, v4 right)                 { return (left.x != right.x || left.y != right.y || left.z != right.z || left.w != right.w); }
INLINE void operator += (v4& leftSide, const v4& rightSide) { leftSide.x += rightSide.x; leftSide.y += rightSide.y; leftSide.z += rightSide.z; leftSide.w += rightSide.w; }
INLINE void operator -= (v4& leftSide, const v4& rightSide) { leftSide.x -= rightSide.x; leftSide.y -= rightSide.y; leftSide.z -= rightSide.z; leftSide.w -= rightSide.w; }
INLINE void operator *= (v4& leftSide, const r32 rightSide) { leftSide.x *= rightSide; leftSide.y *= rightSide; leftSide.z *= rightSide; leftSide.w *= rightSide; }
INLINE void operator /= (v4& leftSide, const r32 rightSide) { leftSide.x /= rightSide; leftSide.y /= rightSide; leftSide.z /= rightSide; leftSide.w /= rightSide; }

INLINE v2i operator - (v2i vector)                            { return NewVec2i(-vector.x, -vector.y); }
INLINE v2i operator + (v2i left, v2i right)                   { return Vec2iAdd(left, right); }
INLINE v2i operator - (v2i left, v2i right)                   { return Vec2iSubtract(left, right); }
INLINE v2i operator * (v2i vector, i32 scalar)                { return Vec2iScale(vector, scalar); }
INLINE v2i operator * (i32 scalar, v2i vector)                { return Vec2iScale(vector, scalar); }
INLINE v2i operator / (v2i vector, i32 scalar)                { return Vec2iShrink(vector, scalar); }
INLINE bool operator == (v2i left, v2i right)                 { return (left.x == right.x && left.y == right.y); }
INLINE bool operator != (v2i left, v2i right)                 { return (left.x != right.x || left.y != right.y); }
INLINE void operator += (v2i& leftSide, const v2i& rightSide) { leftSide.x += rightSide.x; leftSide.y += rightSide.y; }
INLINE void operator -= (v2i& leftSide, const v2i& rightSide) { leftSide.x -= rightSide.x; leftSide.y -= rightSide.y; }
INLINE void operator *= (v2i& leftSide, const i32 rightSide)  { leftSide.x *= rightSide; leftSide.y *= rightSide; }
INLINE void operator /= (v2i& leftSide, const i32 rightSide)  { leftSide.x /= rightSide; leftSide.y /= rightSide; }

INLINE bool operator == (rec left, rec right)             { return  (left.x == right.x && left.y == right.y && left.width == right.width && left.height == right.height); }
INLINE bool operator != (rec left, rec right)             { return !(left.x == right.x && left.y == right.y && left.width == right.width && left.height == right.height); }
INLINE rec operator + (rec rectangle, v2 vector)          { return RecShift(rectangle, vector); }
INLINE rec operator - (rec rectangle, v2 vector)          { return RecShift(rectangle, -vector); }
INLINE rec operator * (rec rectangle, r32 scale)          { return RecScale(rectangle, scale); }
INLINE rec operator / (rec rectangle, r32 scale)          { return RecScale(rectangle, 1/scale); }
INLINE void operator += (rec& leftSide, const v2& vector) { leftSide.topLeft += vector; }
INLINE void operator -= (rec& leftSide, const v2& vector) { leftSide.topLeft -= vector; }

// +--------------------------------------------------------------+
// |                    Orca Function Aliases                     |
// +--------------------------------------------------------------+
// +==============================+
// |       Orca App Exports       |
// +==============================+
#define OC_OnInit          oc_on_init          // oc_on_init()
#define OC_OnMouseDown     oc_on_mouse_down    // oc_on_mouse_down(OC_MouseButton_t button)
#define OC_OnMouseUp       oc_on_mouse_up      // oc_on_mouse_up(OC_MouseButton_t button)
#define OC_OnMouseEnter    oc_on_mouse_enter   // oc_on_mouse_enter()
#define OC_OnMouseLeave    oc_on_mouse_leave   // oc_on_mouse_leave()
#define OC_OnMouseMove     oc_on_mouse_move    // oc_on_mouse_move(r32 x, r32 y, r32 deltaX, r32 deltaY)
#define OC_OnMouseWheel    oc_on_mouse_wheel   // oc_on_mouse_wheel(r32 deltaX, r32 deltaY)
#define OC_OnKeyDown       oc_on_key_down      // oc_on_key_down(OC_ScanCode_t scan, OC_KeyCode_t key)
#define OC_OnKeyUp         oc_on_key_up        // oc_on_key_up(OC_ScanCode_t scan, OC_KeyCode_t key)
#define OC_OnFrameRefresh  oc_on_frame_refresh // oc_on_frame_refresh()
#define OC_OnResize        oc_on_resize        // oc_on_resize(r32 width, r32 height)
#define OC_OnRawEvent      oc_on_raw_event     // oc_on_raw_event(OC_Event_t* event)
#define OC_OnTerminate     oc_on_terminate     // oc_on_terminate()

// +--------------------------------------------------------------+
// |                       Orca Window API                        |
// +--------------------------------------------------------------+
INLINE void OC_WindowSetTitle(MyStr_t title) { oc_window_set_title(title.oc); }
INLINE void OC_WindowSetSize(v2 size)        { oc_window_set_size(size.oc); }
INLINE void OC_RequestQuit()                 { oc_request_quit(); }

// +==============================+
// |      Orca Graphics API       |
// +==============================+
INLINE OC_Surface_t OC_SurfaceNil()                                                                                                                 { return oc_surface_nil(); }
INLINE bool OC_SurfaceIsNil(OC_Surface_t surface)                                                                                                   { return oc_surface_is_nil(surface); }
INLINE void OC_SurfaceDestroy(OC_Surface_t surface)                                                                                                 { oc_surface_destroy(surface); }
INLINE v2 OC_SurfaceGetSize(OC_Surface_t surface)                                                                                                   { return ToVec2(oc_surface_get_size(surface)); }
INLINE v2 OC_SurfaceContentsScaling(OC_Surface_t surface)                                                                                           { return ToVec2(oc_surface_contents_scaling(surface)); }
INLINE void OC_SurfaceBringToFront(OC_Surface_t surface)                                                                                            { oc_surface_bring_to_front(surface); }
INLINE void OC_SurfaceSendToBack(OC_Surface_t surface)                                                                                              { oc_surface_send_to_back(surface); }
INLINE bool OC_SurfaceGetHidden(OC_Surface_t surface)                                                                                               { return oc_surface_get_hidden(surface); }
INLINE void OC_SurfaceSetHidden(OC_Surface_t surface, bool hidden)                                                                                  { oc_surface_set_hidden(surface, hidden); }
INLINE OC_Surface_t OC_GlesSurfaceCreate()                                                                                                          { return oc_gles_surface_create(); }
INLINE void OC_GlesSurfaceMakeCurrent(OC_Surface_t surface)                                                                                         { oc_gles_surface_make_current(surface); }
INLINE void OC_GlesSurfaceSwapInterval(OC_Surface_t surface, int interval)                                                                          { oc_gles_surface_swap_interval(surface, interval); }
INLINE void OC_GlesSurfaceSwapBuffers(OC_Surface_t surface)                                                                                         { oc_gles_surface_swap_buffers(surface); }
INLINE oc_color OC_ColorRgba(r32 r, r32 g, r32 b, r32 a)                                                                                            { return oc_color_rgba(r, g, b, a); }
INLINE oc_color OC_ColorSrgba(r32 r, r32 g, r32 b, r32 a)                                                                                           { return oc_color_srgba(r, g, b, a); }
INLINE oc_color OC_ColorConvert(oc_color color, OC_ColorSpace_t colorSpace)                                                                         { return oc_color_convert(color, colorSpace); }
INLINE OC_CanvasRenderer_t OC_CanvasRendererNil()                                                                                                   { return oc_canvas_renderer_nil(); }
INLINE bool OC_CanvasRendererIsNil(OC_CanvasRenderer_t renderer)                                                                                    { return oc_canvas_renderer_is_nil(renderer); }
INLINE OC_CanvasRenderer_t OC_CanvasRendererCreate()                                                                                                { return oc_canvas_renderer_create(); }
INLINE void OC_CanvasRendererDestroy(OC_CanvasRenderer_t renderer)                                                                                  { oc_canvas_renderer_destroy(renderer); }
INLINE void OC_CanvasRender(OC_CanvasRenderer_t renderer, OC_CanvasContext_t context, OC_Surface_t surface)                                         { oc_canvas_render(renderer, context, surface); }
INLINE void OC_CanvasPresent(OC_CanvasRenderer_t renderer, OC_Surface_t surface)                                                                    { oc_canvas_present(renderer, surface); }
INLINE OC_Surface_t OC_CanvasSurfaceCreate(OC_CanvasRenderer_t renderer)                                                                            { return oc_canvas_surface_create(renderer); }
INLINE void OC_CanvasSurfaceSwapInterval(OC_Surface_t surface, int swap)                                                                            { oc_canvas_surface_swap_interval(surface, swap); }
INLINE OC_CanvasContext_t OC_CanvasContextNil()                                                                                                     { return oc_canvas_context_nil(); }
INLINE bool OC_CanvasContextIsNil(OC_CanvasContext_t context)                                                                                       { return oc_canvas_context_is_nil(context); }
INLINE OC_CanvasContext_t OC_CanvasContextCreate()                                                                                                  { return oc_canvas_context_create(); }
INLINE void OC_CanvasContextDestroy(OC_CanvasContext_t context)                                                                                     { oc_canvas_context_destroy(context); }
INLINE OC_CanvasContext_t OC_CanvasContextSelect(OC_CanvasContext_t context)                                                                        { return oc_canvas_context_select(context); }
INLINE void OC_CanvasContextSetMsaaSampleCount(OC_CanvasContext_t context, u32 sampleCount)                                                         { oc_canvas_context_set_msaa_sample_count(context, sampleCount); }
INLINE OC_Font_t OC_FontNil()                                                                                                                       { return oc_font_nil(); }
INLINE bool OC_FontIsNil(OC_Font_t font)                                                                                                            { return oc_font_is_nil(font); }
INLINE OC_Font_t OC_FontCreateFromMemory(MyStr_t mem, u32 rangeCount, OC_UnicodeRange_t* ranges)                                                    { return oc_font_create_from_memory(mem.oc, rangeCount, ranges); }
INLINE OC_Font_t OC_FontCreateFromFile(OC_File_t file, u32 rangeCount, OC_UnicodeRange_t* ranges)                                                   { return oc_font_create_from_file(file, rangeCount, ranges); }
INLINE OC_Font_t OC_FontCreateFromPath(MyStr_t path, u32 rangeCount, OC_UnicodeRange_t* ranges)                                                     { return oc_font_create_from_path(path.oc, rangeCount, ranges); }
INLINE void OC_FontDestroy(OC_Font_t font)                                                                                                          { oc_font_destroy(font); }
INLINE oc_str32 OC_FontGetGlyphIndices(OC_Font_t font, oc_str32 codePoints, oc_str32 backing)                                                       { return oc_font_get_glyph_indices(font, codePoints, backing); }
INLINE oc_str32 OC_FontPushGlyphIndices(OC_Arena_t* arena, OC_Font_t font, oc_str32 codePoints)                                                     { return oc_font_push_glyph_indices(arena, font, codePoints); }
INLINE u32 OC_FontGetGlyphIndex(OC_Font_t font, oc_utf32 codePoint)                                                                                 { return oc_font_get_glyph_index(font, codePoint); }
INLINE OC_FontMetrics_t OC_FontGetMetrics(OC_Font_t font, r32 emSize)                                                                               { return oc_font_get_metrics(font, emSize); }
INLINE OC_FontMetrics_t OC_FontGetMetricsUnscaled(OC_Font_t font)                                                                                   { return oc_font_get_metrics_unscaled(font); }
INLINE r32 OC_FontGetScaleForEmPixels(OC_Font_t font, r32 emSize)                                                                                   { return oc_font_get_scale_for_em_pixels(font, emSize); }
INLINE OC_TextMetrics_t OC_FontTextMetricsUtr32(OC_Font_t font, r32 fontSize, oc_str32 codepoints)                                                  { return oc_font_text_metrics_utf32(font, fontSize, codepoints); }
INLINE OC_TextMetrics_t OC_FontTextMetrics(OC_Font_t font, r32 fontSize, MyStr_t text)                                                              { return oc_font_text_metrics(font, fontSize, text.oc); }
INLINE OC_Image_t OC_ImageNil()                                                                                                                     { return oc_image_nil(); }
INLINE bool OC_ImageIsNil(OC_Image_t a)                                                                                                             { return oc_image_is_nil(a); }
INLINE OC_Image_t OC_ImageCreate(OC_CanvasRenderer_t renderer, u32 width, u32 height)                                                               { return oc_image_create(renderer, width, height); }
INLINE OC_Image_t OC_ImageCreateFromRgba_8(OC_CanvasRenderer_t renderer, u32 width, u32 height, u8* pixels)                                         { return oc_image_create_from_rgba8(renderer, width, height, pixels); }
INLINE OC_Image_t OC_ImageCreateFromMemory(OC_CanvasRenderer_t renderer, MyStr_t mem, bool flip)                                                    { return oc_image_create_from_memory(renderer, mem.oc, flip); }
INLINE OC_Image_t OC_ImageCreateFromFile(OC_CanvasRenderer_t renderer, OC_File_t file, bool flip)                                                   { return oc_image_create_from_file(renderer, file, flip); }
INLINE OC_Image_t OC_ImageCreateFromPath(OC_CanvasRenderer_t renderer, MyStr_t path, bool flip)                                                     { return oc_image_create_from_path(renderer, path.oc, flip); }
INLINE void OC_ImageDestroy(OC_Image_t image)                                                                                                       { oc_image_destroy(image); }
INLINE void OC_ImageUploadRegionRgba_8(OC_Image_t image, rec region, u8* pixels)                                                                    { oc_image_upload_region_rgba8(image, region.oc, pixels); }
INLINE v2 OC_ImageSize(OC_Image_t image)                                                                                                            { return ToVec2(oc_image_size(image)); }
INLINE OC_RectAtlas_t* OC_RectAtlasCreate(OC_Arena_t* arena, i32 width, i32 height)                                                                 { return oc_rect_atlas_create(arena, width, height); }
INLINE rec OC_RectAtlasAlloc(OC_RectAtlas_t* atlas, i32 width, i32 height)                                                                          { return ToRec(oc_rect_atlas_alloc(atlas, width, height)); }
INLINE void OC_RectAtlasRecycle(OC_RectAtlas_t* atlas, rec rect)                                                                                    { oc_rect_atlas_recycle(atlas, rect.oc); }
INLINE OC_ImageRegion_t OC_ImageAtlasAllocFromRgba_8(OC_RectAtlas_t* atlas, OC_Image_t backingImage, u32 width, u32 height, u8* pixels)              { return oc_image_atlas_alloc_from_rgba8(atlas, backingImage, width, height, pixels); }
INLINE OC_ImageRegion_t OC_ImageAtlasAllocFromMemory(OC_RectAtlas_t* atlas, OC_Image_t backingImage, MyStr_t mem, bool flip)                         { return oc_image_atlas_alloc_from_memory(atlas, backingImage, mem.oc, flip); }
INLINE OC_ImageRegion_t OC_ImageAtlasAllocFromFile(OC_RectAtlas_t* atlas, OC_Image_t backingImage, OC_File_t file, bool flip)                        { return oc_image_atlas_alloc_from_file(atlas, backingImage, file, flip); }
INLINE OC_ImageRegion_t OC_ImageAtlasAllocFromPath(OC_RectAtlas_t* atlas, OC_Image_t backingImage, MyStr_t path, bool flip)                          { return oc_image_atlas_alloc_from_path(atlas, backingImage, path.oc, flip); }
INLINE void OC_ImageAtlasRecycle(OC_RectAtlas_t* atlas, OC_ImageRegion_t imageRgn)                                                                   { oc_image_atlas_recycle(atlas, imageRgn); }
INLINE void OC_MatrixPush(mat23 matrix)                                                                                                             { oc_matrix_push(matrix.oc); }
INLINE void OC_MatrixMultiplyPush(mat23 matrix)                                                                                                     { oc_matrix_multiply_push(matrix.oc); }
INLINE void OC_MatrixPop()                                                                                                                          { oc_matrix_pop(); }
INLINE mat23 OC_MatrixTop()                                                                                                                         { return ToMat23(oc_matrix_top()); }
INLINE void OC_ClipPush(r32 x, r32 y, r32 w, r32 h)                                                                                                 { oc_clip_push(x, y, w, h); }
INLINE void OC_ClipPop()                                                                                                                            { oc_clip_pop(); }
INLINE rec OC_ClipTop()                                                                                                                             { return ToRec(oc_clip_top()); }
INLINE void OC_SetColor(oc_color color)                                                                                                             { oc_set_color(color); }
INLINE void OC_SetColorRgba(r32 r, r32 g, r32 b, r32 a)                                                                                             { oc_set_color_rgba(r, g, b, a); }
INLINE void OC_SetColorRgba(colf color)                                                                                                             { oc_set_color_rgba(color.r, color.g, color.b, color.a); }
INLINE void OC_SetColorSrgba(r32 r, r32 g, r32 b, r32 a)                                                                                            { oc_set_color_srgba(r, g, b, a); }
INLINE void OC_SetColorSrgba(colf color)                                                                                                            { oc_set_color_srgba(color.r, color.g, color.b, color.a); }
INLINE void OC_SetGradient(OC_GradientBlendSpace_t blendSpace, oc_color bottomLeft, oc_color bottomRight, oc_color topRight, oc_color topLeft)      { oc_set_gradient(blendSpace, bottomLeft, bottomRight, topRight, topLeft); }
INLINE void OC_SetWidth(r32 width)                                                                                                                  { oc_set_width(width); }
INLINE void OC_SetTolerance(r32 tolerance)                                                                                                          { oc_set_tolerance(tolerance); }
INLINE void OC_SetJoint(OC_JointType_t joint)                                                                                                       { oc_set_joint(joint); }
INLINE void OC_SetMaxJointExcursion(r32 maxJointExcursion)                                                                                          { oc_set_max_joint_excursion(maxJointExcursion); }
INLINE void OC_SetCap(OC_CapType_t cap)                                                                                                             { oc_set_cap(cap); }
INLINE void OC_SetFont(OC_Font_t font)                                                                                                              { oc_set_font(font); }
INLINE void OC_SetFontSize(r32 size)                                                                                                                { oc_set_font_size(size); }
INLINE void OC_SetTextFlip(bool flip)                                                                                                               { oc_set_text_flip(flip); }
INLINE void OC_SetImage(OC_Image_t image)                                                                                                           { oc_set_image(image); }
INLINE void OC_SetImageSourceRegion(rec region)                                                                                                     { oc_set_image_source_region(region.oc); }
INLINE oc_color OC_GetColor()                                                                                                                       { return oc_get_color(); }
INLINE r32 OC_GetWidth()                                                                                                                            { return oc_get_width(); }
INLINE r32 OC_GetTolerance()                                                                                                                        { return oc_get_tolerance(); }
INLINE OC_JointType_t OC_GetJoint()                                                                                                                 { return oc_get_joint(); }
INLINE r32 OC_GetMaxJointExcursion()                                                                                                                { return oc_get_max_joint_excursion(); }
INLINE OC_CapType_t OC_GetCap()                                                                                                                     { return oc_get_cap(); }
INLINE OC_Font_t OC_GetFont()                                                                                                                       { return oc_get_font(); }
INLINE r32 OC_GetFontSize()                                                                                                                         { return oc_get_font_size(); }
INLINE bool OC_GetTextFlip()                                                                                                                        { return oc_get_text_flip(); }
INLINE OC_Image_t OC_GetImage()                                                                                                                     { return oc_get_image(); }
INLINE rec OC_GetImageSourceRegion()                                                                                                                { return ToRec(oc_get_image_source_region()); }
INLINE v2 OC_GetPosition()                                                                                                                          { return ToVec2(oc_get_position()); }
INLINE void OC_MoveTo(r32 x, r32 y)                                                                                                                 { oc_move_to(x, y); }
INLINE void OC_LineTo(r32 x, r32 y)                                                                                                                 { oc_line_to(x, y); }
INLINE void OC_QuadraticTo(r32 x1, r32 y1, r32 x2, r32 y2)                                                                                          { oc_quadratic_to(x1, y1, x2, y2); }
INLINE void OC_CubicTo(r32 x1, r32 y1, r32 x2, r32 y2, r32 x3, r32 y3)                                                                              { oc_cubic_to(x1, y1, x2, y2, x3, y3); }
INLINE void OC_ClosePath()                                                                                                                          { oc_close_path(); }
INLINE rec OC_GlyphOutlines(oc_str32 glyphIndices)                                                                                                  { return ToRec(oc_glyph_outlines(glyphIndices)); }
INLINE void OC_CodepointsOutlines(oc_str32 string)                                                                                                  { oc_codepoints_outlines(string); }
INLINE void OC_TextOutlines(MyStr_t string)                                                                                                         { oc_text_outlines(string.oc); }
INLINE void OC_Clear()                                                                                                                              { oc_clear(); }
INLINE void OC_Fill()                                                                                                                               { oc_fill(); }
INLINE void OC_Stroke()                                                                                                                             { oc_stroke(); }
INLINE void OC_RectangleFill(r32 x, r32 y, r32 w, r32 h)                                                                                            { oc_rectangle_fill(x, y, w, h); }
INLINE void OC_RectangleFill(rec rectangle)                                                                                                         { oc_rectangle_fill(rectangle.x, rectangle.y, rectangle.width, rectangle.height); }
INLINE void OC_RectangleStroke(r32 x, r32 y, r32 w, r32 h)                                                                                          { oc_rectangle_stroke(x, y, w, h); }
INLINE void OC_RectangleStroke(rec rectangle)                                                                                                       { oc_rectangle_stroke(rectangle.x, rectangle.y, rectangle.width, rectangle.height); }
INLINE void OC_RoundedRectangleFill(r32 x, r32 y, r32 w, r32 h, r32 r)                                                                              { oc_rounded_rectangle_fill(x, y, w, h, r); }
INLINE void OC_RoundedRectangleFill(rec rectangle, r32 r)                                                                                           { oc_rounded_rectangle_fill(rectangle.x, rectangle.y, rectangle.width, rectangle.height, r); }
INLINE void OC_RoundedRectangleStroke(r32 x, r32 y, r32 w, r32 h, r32 r)                                                                            { oc_rounded_rectangle_stroke(x, y, w, h, r); }
INLINE void OC_RoundedRectangleStroke(rec rectangle, r32 r)                                                                                         { oc_rounded_rectangle_stroke(rectangle.x, rectangle.y, rectangle.width, rectangle.height, r); }
INLINE void OC_EllipseFill(r32 x, r32 y, r32 rx, r32 ry)                                                                                            { oc_ellipse_fill(x, y, rx, ry); }
INLINE void OC_EllipseStroke(r32 x, r32 y, r32 rx, r32 ry)                                                                                          { oc_ellipse_stroke(x, y, rx, ry); }
INLINE void OC_CircleFill(r32 x, r32 y, r32 r)                                                                                                      { oc_circle_fill(x, y, r); }
INLINE void OC_CircleStroke(r32 x, r32 y, r32 r)                                                                                                    { oc_circle_stroke(x, y, r); }
INLINE void OC_Arc(r32 x, r32 y, r32 r, r32 arcAngle, r32 startAngle)                                                                               { oc_arc(x, y, r, arcAngle, startAngle); }
INLINE void OC_TextFill(r32 x, r32 y, MyStr_t text)                                                                                                 { oc_text_fill(x, y, text.oc); }
INLINE void OC_ImageDraw(OC_Image_t image, rec rect)                                                                                                { oc_image_draw(image, rect.oc); }
INLINE void OC_ImageDrawRegion(OC_Image_t image, rec srcRegion, rec dstRegion)                                                                      { oc_image_draw_region(image, srcRegion.oc, dstRegion.oc); }

// +==============================+
// |        Orca File API         |
// +==============================+
INLINE OC_IoCmp_t OC_IoWaitSingleReq(OC_IoReq_t* req)                                                                                                      { return oc_io_wait_single_req(req); }
INLINE OC_File_t OC_FileNil()                                                                                                                              { return oc_file_nil(); }
INLINE bool OC_FileIsNil(OC_File_t handle)                                                                                                                 { return oc_file_is_nil(handle); }
INLINE OC_File_t OC_FileOpen(MyStr_t path, OC_FileAccess_t rights, OC_FileOpenFlags_t flags)                                                               { return oc_file_open(path.oc, rights, flags); }
INLINE OC_File_t OC_FileOpenAt(OC_File_t dir, MyStr_t path, OC_FileAccess_t rights, OC_FileOpenFlags_t flags)                                              { return oc_file_open_at(dir, path.oc, rights, flags); }
INLINE void OC_FileClose(OC_File_t file)                                                                                                                   { oc_file_close(file); }
INLINE OC_IoError_t OC_FileLastError(OC_File_t handle)                                                                                                     { return oc_file_last_error(handle); }
INLINE i64 OC_FilePos(OC_File_t file)                                                                                                                      { return oc_file_pos(file); }
INLINE i64 OC_FileSeek(OC_File_t file, i64 offset, OC_FileWhence_t whence)                                                                                 { return oc_file_seek(file, offset, whence); }
INLINE u64 OC_FileWrite(OC_File_t file, u64 size, char* buffer)                                                                                            { return oc_file_write(file, size, buffer); }
INLINE u64 OC_FileRead(OC_File_t file, u64 size, char* buffer)                                                                                             { return oc_file_read(file, size, buffer); }
INLINE OC_FileStatus_t OC_FileGetStatus(OC_File_t file)                                                                                                    { return oc_file_get_status(file); }
INLINE u64 OC_FileSize(OC_File_t file)                                                                                                                     { return oc_file_size(file); }
INLINE OC_File_t OC_FileOpenWithRequest(MyStr_t path, OC_FileAccess_t rights, OC_FileOpenFlags_t flags)                                                    { return oc_file_open_with_request(path.oc, rights, flags); }
INLINE OC_FileOpenWithDialogResult_t OC_FileOpenWithDialog(OC_Arena_t* arena, OC_FileAccess_t rights, OC_FileOpenFlags_t flags, OC_FileDialogDesc_t* desc) { return oc_file_open_with_dialog(arena, rights, flags, desc); }

// +==============================+
// |         Orca UI API          |
// +==============================+
INLINE void OC_UiInit(OC_UiContext_t* context)                                                        { oc_ui_init(context); }
INLINE OC_UiContext_t* OC_UiGetContext()                                                              { return oc_ui_get_context(); }
INLINE void OC_UiSetContext(OC_UiContext_t* context)                                                  { oc_ui_set_context(context); }
INLINE void OC_UiProcessEvent(OC_Event_t* event)                                                      { oc_ui_process_event(event); }
INLINE void OC_UiBeginFrame(v2 size, OC_UiStyle_t* defaultStyle, OC_UiStyleMask_t mask)               { oc_ui_begin_frame(size.oc, defaultStyle, mask); }
INLINE void OC_UiEndFrame()                                                                           { oc_ui_end_frame(); }
INLINE void OC_UiDraw()                                                                               { oc_ui_draw(); }
INLINE OC_UiSig_t OC_UiLabel(const char* label)                                                       { return oc_ui_label(label); }
INLINE OC_UiSig_t OC_UiLabelStr8(MyStr_t label)                                                       { return oc_ui_label_str8(label.oc); }
INLINE OC_UiSig_t OC_UiButton(const char* label)                                                      { return oc_ui_button(label); }
INLINE OC_UiSig_t OC_UiCheckbox(const char* name, bool* checked)                                      { return oc_ui_checkbox(name, checked); }
INLINE OC_UiBox_t* OC_UiSlider(const char* name, r32* value)                                          { return oc_ui_slider(name, value); }
INLINE OC_UiBox_t* OC_UiScrollbar(const char* name, r32 thumbRatio, r32* scrollValue)                 { return oc_ui_scrollbar(name, thumbRatio, scrollValue); }
INLINE OC_UiTextBoxResult_t OC_UiTextBox(const char* name, OC_Arena_t* arena, MyStr_t text)           { return oc_ui_text_box(name, arena, text.oc); }
INLINE OC_UiSelectPopupInfo_t OC_UiSelectPopup(const char* name, OC_UiSelectPopupInfo_t* info)        { return oc_ui_select_popup(name, info); }
INLINE OC_UiRadioGroupInfo_t OC_UiRadioGroup(const char* name, OC_UiRadioGroupInfo_t* info)           { return oc_ui_radio_group(name, info); }
INLINE void OC_UiPanelBegin(const char* name, OC_UiFlags_t flags)                                     { oc_ui_panel_begin(name, flags); }
INLINE void OC_UiPanelEnd()                                                                           { oc_ui_panel_end(); }
INLINE void OC_UiMenuBarBegin(const char* name)                                                       { oc_ui_menu_bar_begin(name); }
INLINE void OC_UiMenuBarEnd()                                                                         { oc_ui_menu_bar_end(); }
INLINE void OC_UiMenuBegin(const char* label)                                                         { oc_ui_menu_begin(label); }
INLINE void OC_UiMenuEnd()                                                                            { oc_ui_menu_end(); }
INLINE OC_UiSig_t OC_UiMenuButton(const char* label)                                                  { return oc_ui_menu_button(label); }
INLINE void OC_UiTooltip(const char* label)                                                           { oc_ui_tooltip(label); }
INLINE void OC_UiStyleNext(OC_UiStyle_t* style, OC_UiStyleMask_t mask)                                { oc_ui_style_next(style, mask); }
INLINE void OC_UiPatternPush(OC_Arena_t* arena, OC_UiPattern_t* pattern, OC_UiSelector_t selector)    { oc_ui_pattern_push(arena, pattern, selector); }
INLINE OC_UiPattern_t OC_UiPatternAll()                                                               { return oc_ui_pattern_all(); }
INLINE OC_UiPattern_t OC_UiPatternOwner()                                                             { return oc_ui_pattern_owner(); }
INLINE void OC_UiStyleMatchBefore(OC_UiPattern_t pattern, OC_UiStyle_t* style, OC_UiStyleMask_t mask) { oc_ui_style_match_before(pattern, style, mask); }
INLINE void OC_UiStyleMatchAfter(OC_UiPattern_t pattern, OC_UiStyle_t* style, OC_UiStyleMask_t mask)  { oc_ui_style_match_after(pattern, style, mask); }

#define OC_UiFrame(size, style, mask) oc_defer_loop(OC_UiBeginFrame((size), (style), (mask)), OC_UiEndFrame())
#define OC_UiPanel(s, f)              oc_defer_loop(OC_UiPanelBegin(s, f), OC_UiPanelEnd())
#define OC_UiMenuBar(name)            oc_defer_loop(OC_UiMenuBarBegin(name), OC_UiMenuBarEnd())
#define OC_UiMenu(name)               oc_defer_loop(OC_UiMenuBegin(label), OC_UiMenuEnd())

// +--------------------------------------------------------------+
// |                        Orca Util API                         |
// +--------------------------------------------------------------+
INLINE void OC_ArenaInit(OC_Arena_t* arena)                                                                                 { oc_arena_init(arena); }
INLINE void OC_ArenaInitWithOptions(OC_Arena_t* arena, OC_ArenaOptions_t* options)                                          { oc_arena_init_with_options(arena, options); }
INLINE void OC_ArenaCleanup(OC_Arena_t* arena)                                                                              { oc_arena_cleanup(arena); }
INLINE void* OC_ArenaPush(OC_Arena_t* arena, u64 size)                                                                      { return oc_arena_push(arena, size); }
INLINE void* OC_ArenaPushAligned(OC_Arena_t* arena, u64 size, u32 alignment)                                                { return oc_arena_push_aligned(arena, size, alignment); }
INLINE void OC_ArenaClear(OC_Arena_t* arena)                                                                                { oc_arena_clear(arena); }
INLINE OC_ArenaScope_t OC_ArenaScopeBegin(OC_Arena_t* arena)                                                                { return oc_arena_scope_begin(arena); }
INLINE void OC_ArenaScopeEnd(OC_ArenaScope_t scope)                                                                         { oc_arena_scope_end(scope); }
INLINE OC_ArenaScope_t OC_ScratchBegin()                                                                                    { return oc_scratch_begin(); }
INLINE void OC_ScratchEnd(OC_ArenaScope_t scope)                                                                            { oc_scratch_end(scope); }
INLINE OC_ArenaScope_t OC_ScratchBeginNext(OC_Arena_t* used)                                                                { return oc_scratch_begin_next(used); }
INLINE void OC_ListInit(OC_List_t* list)                                                                                    { oc_list_init(list); }
INLINE bool OC_ListEmpty(OC_List_t list)                                                                                    { return oc_list_empty(list); }
INLINE OC_ListElement_t* OC_ListBegin(OC_List_t list)                                                                       { return oc_list_begin(list); }
INLINE OC_ListElement_t* OC_ListEnd(OC_List_t* list)                                                                        { return oc_list_end(list); }
INLINE OC_ListElement_t* OC_ListLast(OC_List_t list)                                                                        { return oc_list_last(list); }
INLINE void OC_ListInsert(OC_List_t* list, OC_ListElement_t* afterElt, OC_ListElement_t* elt)                               { oc_list_insert(list, afterElt, elt); }
INLINE void OC_ListInsertBefore(OC_List_t* list, OC_ListElement_t* beforeElt, OC_ListElement_t* elt)                        { oc_list_insert_before(list, beforeElt, elt); }
INLINE void OC_ListRemove(OC_List_t* list, OC_ListElement_t* elt)                                                           { oc_list_remove(list, elt); }
INLINE void OC_ListPushFront(OC_List_t* list, OC_ListElement_t* elt)                                                        { oc_list_push_front(list, elt); }
INLINE OC_ListElement_t* OC_ListPopFront(OC_List_t* list)                                                                   { return oc_list_pop_front(list); }
INLINE void OC_ListPushBack(OC_List_t* list, OC_ListElement_t* elt)                                                         { oc_list_push_back(list, elt); }
INLINE OC_ListElement_t* OC_ListPopBack(OC_List_t* list)                                                                    { return oc_list_pop_back(list); }
INLINE MyStr_t OC_Str8FromBuffer(u64 len, char* buffer)                                                                     { return ToStr(oc_str8_from_buffer(len, buffer)); }
INLINE MyStr_t OC_Str8Slice(MyStr_t s, u64 start, u64 end)                                                                  { return ToStr(oc_str8_slice(s.oc, start, end)); }
INLINE MyStr_t OC_Str8PushBuffer(OC_Arena_t* arena, u64 len, char* buffer)                                                  { return ToStr(oc_str8_push_buffer(arena, len, buffer)); }
INLINE MyStr_t OC_Str8PushCstring(OC_Arena_t* arena, const char* str)                                                       { return ToStr(oc_str8_push_cstring(arena, str)); }
INLINE MyStr_t OC_Str8PushCopy(OC_Arena_t* arena, MyStr_t s)                                                                { return ToStr(oc_str8_push_copy(arena, s.oc)); }
INLINE MyStr_t OC_Str8PushSlice(OC_Arena_t* arena, MyStr_t s, u64 start, u64 end)                                           { return ToStr(oc_str8_push_slice(arena, s.oc, start, end)); }
INLINE MyStr_t OC_Str8Pushfv(OC_Arena_t* arena, const char* format, va_list args)                                           { return ToStr(oc_str8_pushfv(arena, format, args)); }
INLINE int OC_Str8Cmp(MyStr_t s1, MyStr_t s2)                                                                               { return oc_str8_cmp(s1.oc, s2.oc); }
INLINE char* OC_Str8ToCstring(OC_Arena_t* arena, MyStr_t string)                                                            { return oc_str8_to_cstring(arena, string.oc); }
INLINE void OC_Str8ListPush(OC_Arena_t* arena, oc_str8_list* list, MyStr_t str)                                             { oc_str8_list_push(arena, list, str.oc); }
INLINE MyStr_t OC_Str8ListCollate(OC_Arena_t* arena, oc_str8_list list, MyStr_t prefix, MyStr_t separator, MyStr_t postfix) { return ToStr(oc_str8_list_collate(arena, list, prefix.oc, separator.oc, postfix.oc)); }
INLINE MyStr_t OC_Str8ListJoin(OC_Arena_t* arena, oc_str8_list list)                                                        { return ToStr(oc_str8_list_join(arena, list)); }
INLINE oc_str8_list OC_Str8Split(OC_Arena_t* arena, MyStr_t str, oc_str8_list separators)                                   { return oc_str8_split(arena, str.oc, separators); }
INLINE MyStr_t OC_PathSliceDirectory(MyStr_t path)                                                                          { return ToStr(oc_path_slice_directory(path.oc)); }
INLINE MyStr_t OC_PathSliceFilename(MyStr_t path)                                                                           { return ToStr(oc_path_slice_filename(path.oc)); }
INLINE oc_str8_list OC_PathSplit(OC_Arena_t* arena, MyStr_t path)                                                           { return oc_path_split(arena, path.oc); }
INLINE MyStr_t OC_PathJoin(OC_Arena_t* arena, oc_str8_list elements)                                                        { return ToStr(oc_path_join(arena, elements)); }
INLINE MyStr_t OC_PathAppend(OC_Arena_t* arena, MyStr_t parent, MyStr_t relPath)                                            { return ToStr(oc_path_append(arena, parent.oc, relPath.oc)); }
INLINE bool OC_PathIsAbsolute(MyStr_t path)                                                                                 { return oc_path_is_absolute(path.oc); }

#define OC_Str8Pushf(arena, format, ...)               oc_str8_pushf(arena, format, ##__VA_ARGS__)
#define OC_Str8ListPushf(arena, list, format, ...)     oc_str8_list_pushf(arena, list, format, ##__VA_ARGS__)
#define OC_ArenaPushType(arena, type)                  ((type*)OC_ArenaPushAligned(arena, sizeof(type), _Alignof(type)))
#define OC_ArenaPushArray(arena, type, count)          ((type*)OC_ArenaPushAligned(arena, sizeof(type) * (count), _Alignof(type)))
#define OC_ScratchEnd(scope)                           OC_ArenaScopeEnd(scope)
#define OC_ListNext(element)                           (element)->next
#define OC_ListPrev(element)                           (element)->prev
#define OC_ListEntry(element, type, member)            oc_container_of(element, type, member)
#define OC_ListNextEntry(list, element, type, member)  ((element->member.next != OC_ListEnd(list)) ? OC_ListEntry(element->member.next, type, member) : 0)
#define OC_ListPrevEntry(list, element, type, member)  ((element->member.prev != OC_ListEnd(list)) ? OC_ListEntry(element->member.prev, type, member) : 0)
#define OC_ListFirstEntry(list, type, member)          (oc_list_checked_entry(OC_ListBegin(list), type, member))
#define OC_ListLastEntry(list, type, member)           (oc_list_checked_entry(OC_ListLast(list), type, member))
#define OC_ListFor(list, element, type, member)        for (type* element = oc_list_checked_entry(OC_ListBegin(list), type, member); element != 0; element = oc_list_checked_entry(element->member.next, type, member))
#define OC_ListForReverse(list, element, type, member) for (type* element = oc_list_checked_entry(OC_ListLast(list), type, member); element != 0; element = oc_list_checked_entry(element->member.prev, type, member))
#define OC_ListForSafe(list, element, type, member)    for (type* element = oc_list_checked_entry(OC_ListBegin(list), type, member), *__tmp = element ? oc_list_checked_entry(element->member.next, type, member) : 0; element != 0; element = __tmp, __tmp = element ? oc_list_checked_entry(element->member.next, type, member) : 0)
#define OC_ListPopFrontEntry(list, type, member)       (OC_ListEmpty(*list) ? 0 : OC_ListEntry(OC_ListPopFront(list), type, member))
#define OC_ListPopBackEntry(list, type, member)        (OC_ListEmpty(*list) ? 0 : OC_ListEntry(OC_ListPopBack(list), type, member))
#define OC_Log_I(message, ...)                         oc_log_info(message, ##__VA_ARGS__) 
#define OC_Log_W(message, ...)                         oc_log_warning(message, ##__VA_ARGS__) 
#define OC_Log_E(message, ...)                         oc_log_error(message, ...) 
#define OC_Assert(test, message, ...)                  OC_ASSERT(test, message, ##__VA_ARGS__) 
#define OC_Abort(message, ...)                         OC_ABORT(message, ##__VA_ARGS__) 

#endif //  _ORCA_ALIASES_H

// +--------------------------------------------------------------+
// |                   Autocomplete Dictionary                    |
// +--------------------------------------------------------------+
/*
@Defines
INLINE
EXPORT
@Types
Vec2_t
v2
Vec3_t
v3
Vec4_t
v4
Vec2i_t
v2i
Vec3i_t
v3i
Matrix2x3_t
mat23
Rectangle_t
rec
@Functions
INLINE v2 NewVec2(r32 x, r32 y)
INLINE v3 NewVec3(r32 x, r32 y, r32 z)
INLINE v4 NewVec4(r32 x, r32 y, r32 z, r32 w)
INLINE v2i NewVec2i(i32 x, i32 y)
INLINE mat23 NewMat23(r32 r0c0, r32 r0c1, r32 r0c2, r32 r1c0, r32 r1c1, r32 r1c2)
INLINE rec NewRec(r32 x, r32 y, r32 width, r32 height)
INLINE colf NewColorf(r32 r, r32 g, r32 b, r32 a)
INLINE colf NewColorfBytes(u8 r, u8 g, u8 b, u8 a)
INLINE colf NewColorfHex(u32 packedColorBgra)
INLINE MyStr_t ToStr(oc_str8 ocString)
INLINE v2 ToVec2(oc_vec2 ocVector)
INLINE v3 ToVec3(oc_vec3 ocVector)
INLINE v4 ToVec4(oc_vec4 ocVector)
INLINE v2i ToVec2i(oc_vec2i ocVector)
INLINE rec ToRec(oc_rect ocRectangle)
INLINE mat23 ToMat23(oc_mat2x3 ocMatrix)
INLINE colf ToColorf(oc_color ocColor)
INLINE v2 Vec2Add(v2 left, v2 right)
INLINE v2 Vec2Subtract(v2 left, v2 right)
INLINE v2 Vec2Scale(v2 vector, r32 scalar)
INLINE v2 Vec2Shrink(v2 vector, r32 divisor)
INLINE v3 Vec3Add(v3 left, v3 right)
INLINE v3 Vec3Subtract(v3 left, v3 right)
INLINE v3 Vec3Scale(v3 vector, r32 scalar)
INLINE v3 Vec3Shrink(v3 vector, r32 divisor)
INLINE v4 Vec4Add(v4 left, v4 right)
INLINE v4 Vec4Subtract(v4 left, v4 right)
INLINE v4 Vec4Scale(v4 vector, r32 scalar)
INLINE v4 Vec4Shrink(v4 vector, r32 divisor)
INLINE v2i Vec2iAdd(v2i left, v2i right)
INLINE v2i Vec2iSubtract(v2i left, v2i right)
INLINE v2i Vec2iScale(v2i vector, i32 scalar)
INLINE v2i Vec2iShrink(v2i vector, i32 divisor)
INLINE rec RecShift(rec rectangle, v2 amount)
INLINE rec RecScale(rec rectangle, r32 scalar)
void OC_OnInit()
void OC_OnMouseDown(OC_MouseButton_t button)
void OC_OnMouseUp(OC_MouseButton_t button)
void OC_OnMouseEnter()
void OC_OnMouseLeave()
void OC_OnMouseMove(r32 x, r32 y, r32 deltaX, r32 deltaY)
void OC_OnMouseWheel(r32 deltaX, r32 deltaY)
void OC_OnKeyDown(OC_ScanCode_t scan, OC_KeyCode_t key)
void OC_OnKeyUp(OC_ScanCode_t scan, OC_KeyCode_t key)
void OC_OnFrameRefresh()
void OC_OnResize(r32 width, r32 height)
void OC_OnRawEvent(OC_Event_t* event)
void OC_OnTerminate()
INLINE void OC_WindowSetTitle(MyStr_t title)
INLINE void OC_WindowSetSize(v2 size)
INLINE void OC_RequestQuit()
INLINE OC_Surface_t OC_SurfaceNil()
INLINE bool OC_SurfaceIsNil(OC_Surface_t surface)
INLINE void OC_SurfaceDestroy(OC_Surface_t surface)
INLINE v2 OC_SurfaceGetSize(OC_Surface_t surface)
INLINE v2 OC_SurfaceContentsScaling(OC_Surface_t surface)
INLINE void OC_SurfaceBringToFront(OC_Surface_t surface)
INLINE void OC_SurfaceSendToBack(OC_Surface_t surface)
INLINE bool OC_SurfaceGetHidden(OC_Surface_t surface)
INLINE void OC_SurfaceSetHidden(OC_Surface_t surface, bool hidden)
INLINE OC_Surface_t OC_GlesSurfaceCreate()
INLINE void OC_GlesSurfaceMakeCurrent(OC_Surface_t surface)
INLINE void OC_GlesSurfaceSwapInterval(OC_Surface_t surface, int interval)
INLINE void OC_GlesSurfaceSwapBuffers(OC_Surface_t surface)
INLINE oc_color OC_ColorRgba(r32 r, r32 g, r32 b, r32 a)
INLINE oc_color OC_ColorSrgba(r32 r, r32 g, r32 b, r32 a)
INLINE oc_color OC_ColorConvert(oc_color color, OC_ColorSpace_t colorSpace)
INLINE OC_CanvasRenderer_t OC_CanvasRendererNil()
INLINE bool OC_CanvasRendererIsNil(OC_CanvasRenderer_t renderer)
INLINE OC_CanvasRenderer_t OC_CanvasRendererCreate()
INLINE void OC_CanvasRendererDestroy(OC_CanvasRenderer_t renderer)
INLINE void OC_CanvasRender(OC_CanvasRenderer_t renderer, OC_CanvasContext_t context, OC_Surface_t surface)
INLINE void OC_CanvasPresent(OC_CanvasRenderer_t renderer, OC_Surface_t surface)
INLINE OC_Surface_t OC_CanvasSurfaceCreate(OC_CanvasRenderer_t renderer)
INLINE void OC_CanvasSurfaceSwapInterval(OC_Surface_t surface, int swap)
INLINE OC_CanvasContext_t OC_CanvasContextNil()
INLINE bool OC_CanvasContextIsNil(OC_CanvasContext_t context)
INLINE OC_CanvasContext_t OC_CanvasContextCreate()
INLINE void OC_CanvasContextDestroy(OC_CanvasContext_t context)
INLINE OC_CanvasContext_t OC_CanvasContextSelect(OC_CanvasContext_t context)
INLINE void OC_CanvasContextSetMsaaSampleCount(OC_CanvasContext_t context, u32 sampleCount)
INLINE OC_Font_t OC_FontNil()
INLINE bool OC_FontIsNil(OC_Font_t font)
INLINE OC_Font_t OC_FontCreateFromMemory(MyStr_t mem, u32 rangeCount, OC_UnicodeRange_t* ranges)
INLINE OC_Font_t OC_FontCreateFromFile(OC_File_t file, u32 rangeCount, OC_UnicodeRange_t* ranges)
INLINE OC_Font_t OC_FontCreateFromPath(MyStr_t path, u32 rangeCount, OC_UnicodeRange_t* ranges)
INLINE void OC_FontDestroy(OC_Font_t font)
INLINE oc_str32 OC_FontGetGlyphIndices(OC_Font_t font, oc_str32 codePoints, oc_str32 backing)
INLINE oc_str32 OC_FontPushGlyphIndices(OC_Arena_t* arena, OC_Font_t font, oc_str32 codePoints)
INLINE u32 OC_FontGetGlyphIndex(OC_Font_t font, oc_utf32 codePoint)
INLINE OC_FontMetrics_t OC_FontGetMetrics(OC_Font_t font, r32 emSize)
INLINE OC_FontMetrics_t OC_FontGetMetricsUnscaled(OC_Font_t font)
INLINE r32 OC_FontGetScaleForEmPixels(OC_Font_t font, r32 emSize)
INLINE OC_TextMetrics_t OC_FontTextMetricsUtr32(OC_Font_t font, r32 fontSize, oc_str32 codepoints)
INLINE OC_TextMetrics_t OC_FontTextMetrics(OC_Font_t font, r32 fontSize, MyStr_t text)
INLINE OC_Image_t OC_ImageNil()
INLINE bool OC_ImageIsNil(OC_Image_t a)
INLINE OC_Image_t OC_ImageCreate(OC_CanvasRenderer_t renderer, u32 width, u32 height)
INLINE OC_Image_t OC_ImageCreateFromRgba_8(OC_CanvasRenderer_t renderer, u32 width, u32 height, u8* pixels)
INLINE OC_Image_t OC_ImageCreateFromMemory(OC_CanvasRenderer_t renderer, MyStr_t mem, bool flip)
INLINE OC_Image_t OC_ImageCreateFromFile(OC_CanvasRenderer_t renderer, OC_File_t file, bool flip)
INLINE OC_Image_t OC_ImageCreateFromPath(OC_CanvasRenderer_t renderer, MyStr_t path, bool flip)
INLINE void OC_ImageDestroy(OC_Image_t image)
INLINE void OC_ImageUploadRegionRgba_8(OC_Image_t image, rec region, u8* pixels)
INLINE v2 OC_ImageSize(OC_Image_t image)
INLINE OC_RectAtlas_t* OC_RectAtlasCreate(OC_Arena_t* arena, i32 width, i32 height)
INLINE rec OC_RectAtlasAlloc(OC_RectAtlas_t* atlas, i32 width, i32 height)
INLINE void OC_RectAtlasRecycle(OC_RectAtlas_t* atlas, rec rect)
INLINE OC_ImageRegion_t OC_ImageAtlasAllocFromRgba_8(OC_RectAtlas_t* atlas, OC_Image_t backingImage, u32 width, u32 height, u8* pixels)
INLINE OC_ImageRegion_t OC_ImageAtlasAllocFromMemory(OC_RectAtlas_t* atlas, OC_Image_t backingImage, MyStr_t mem, bool flip)
INLINE OC_ImageRegion_t OC_ImageAtlasAllocFromFile(OC_RectAtlas_t* atlas, OC_Image_t backingImage, OC_File_t file, bool flip)
INLINE OC_ImageRegion_t OC_ImageAtlasAllocFromPath(OC_RectAtlas_t* atlas, OC_Image_t backingImage, MyStr_t path, bool flip)
INLINE void OC_ImageAtlasRecycle(OC_RectAtlas_t* atlas, OC_ImageRegion_t imageRgn)
INLINE void OC_MatrixPush(mat23 matrix)
INLINE void OC_MatrixMultiplyPush(mat23 matrix)
INLINE void OC_MatrixPop()
INLINE mat23 OC_MatrixTop()
INLINE void OC_ClipPush(r32 x, r32 y, r32 w, r32 h)
INLINE void OC_ClipPop()
INLINE rec OC_ClipTop()
INLINE void OC_SetColor(oc_color color)
INLINE void OC_SetColorRgba(colf color)
INLINE void OC_SetColorSrgba(colf color)
INLINE void OC_SetGradient(OC_GradientBlendSpace_t blendSpace, oc_color bottomLeft, oc_color bottomRight, oc_color topRight, oc_color topLeft)
INLINE void OC_SetWidth(r32 width)
INLINE void OC_SetTolerance(r32 tolerance)
INLINE void OC_SetJoint(OC_JointType_t joint)
INLINE void OC_SetMaxJointExcursion(r32 maxJointExcursion)
INLINE void OC_SetCap(OC_CapType_t cap)
INLINE void OC_SetFont(OC_Font_t font)
INLINE void OC_SetFontSize(r32 size)
INLINE void OC_SetTextFlip(bool flip)
INLINE void OC_SetImage(OC_Image_t image)
INLINE void OC_SetImageSourceRegion(rec region)
INLINE oc_color OC_GetColor()
INLINE r32 OC_GetWidth()
INLINE r32 OC_GetTolerance()
INLINE OC_JointType_t OC_GetJoint()
INLINE r32 OC_GetMaxJointExcursion()
INLINE OC_CapType_t OC_GetCap()
INLINE OC_Font_t OC_GetFont()
INLINE r32 OC_GetFontSize()
INLINE bool OC_GetTextFlip()
INLINE OC_Image_t OC_GetImage()
INLINE rec OC_GetImageSourceRegion()
INLINE v2 OC_GetPosition()
INLINE void OC_MoveTo(r32 x, r32 y)
INLINE void OC_LineTo(r32 x, r32 y)
INLINE void OC_QuadraticTo(r32 x1, r32 y1, r32 x2, r32 y2)
INLINE void OC_CubicTo(r32 x1, r32 y1, r32 x2, r32 y2, r32 x3, r32 y3)
INLINE void OC_ClosePath()
INLINE rec OC_GlyphOutlines(oc_str32 glyphIndices)
INLINE void OC_CodepointsOutlines(oc_str32 string)
INLINE void OC_TextOutlines(MyStr_t string)
INLINE void OC_Clear()
INLINE void OC_Fill()
INLINE void OC_Stroke()
INLINE void OC_RectangleFill(r32 x, r32 y, r32 w, r32 h)
INLINE void OC_RectangleStroke(r32 x, r32 y, r32 w, r32 h)
INLINE void OC_RoundedRectangleFill(r32 x, r32 y, r32 w, r32 h, r32 r)
INLINE void OC_RoundedRectangleStroke(r32 x, r32 y, r32 w, r32 h, r32 r)
INLINE void OC_EllipseFill(r32 x, r32 y, r32 rx, r32 ry)
INLINE void OC_EllipseStroke(r32 x, r32 y, r32 rx, r32 ry)
INLINE void OC_CircleFill(r32 x, r32 y, r32 r)
INLINE void OC_CircleStroke(r32 x, r32 y, r32 r)
INLINE void OC_Arc(r32 x, r32 y, r32 r, r32 arcAngle, r32 startAngle)
INLINE void OC_TextFill(r32 x, r32 y, MyStr_t text)
INLINE void OC_ImageDraw(OC_Image_t image, rec rect)
INLINE void OC_ImageDrawRegion(OC_Image_t image, rec srcRegion, rec dstRegion)
INLINE OC_IoCmp_t OC_IoWaitSingleReq(OC_IoReq_t* req)
INLINE OC_File_t OC_FileNil() 
INLINE bool OC_FileIsNil(OC_File_t handle)
INLINE OC_File_t OC_FileOpen(MyStr_t path, OC_FileAccess_t rights, OC_FileOpenFlags_t flags)
INLINE OC_File_t OC_FileOpenAt(OC_File_t dir, MyStr_t path, OC_FileAccess_t rights, OC_FileOpenFlags_t flags)
INLINE void OC_FileClose(OC_File_t file)
INLINE OC_IoError_t OC_FileLastError(OC_File_t handle)
INLINE i64 OC_FilePos(OC_File_t file)
INLINE i64 OC_FileSeek(OC_File_t file, i64 offset, OC_FileWhence_t whence)
INLINE u64 OC_FileWrite(OC_File_t file, u64 size, char* buffer)
INLINE u64 OC_FileRead(OC_File_t file, u64 size, char* buffer)
INLINE OC_FileStatus_t OC_FileGetStatus(OC_File_t file)
INLINE u64 OC_FileSize(OC_File_t file)
INLINE OC_File_t OC_FileOpenWithRequest(MyStr_t path, OC_FileAccess_t rights, OC_FileOpenFlags_t flags)
INLINE OC_FileOpenWithDialogResult_t OC_FileOpenWithDialog(OC_Arena_t* arena, OC_FileAccess_t rights, OC_FileOpenFlags_t flags, OC_FileDialogDesc_t* desc)
INLINE void OC_UiInit(OC_UiContext_t* context)
INLINE OC_UiContext_t* OC_UiGetContext() 
INLINE void OC_UiSetContext(OC_UiContext_t* context)
INLINE void OC_UiProcessEvent(OC_Event_t* event)
INLINE void OC_UiBeginFrame(v2 size, OC_UiStyle_t* defaultStyle, OC_UiStyleMask_t mask)
INLINE void OC_UiEndFrame() 
INLINE void OC_UiDraw() 
INLINE OC_UiSig_t OC_UiLabel(const char* label)
INLINE OC_UiSig_t OC_UiLabelStr8(MyStr_t label)
INLINE OC_UiSig_t OC_UiButton(const char* label)
INLINE OC_UiSig_t OC_UiCheckbox(const char* name, bool* checked)
INLINE OC_UiBox_t* OC_UiSlider(const char* name, r32* value)
INLINE OC_UiBox_t* OC_UiScrollbar(const char* name, r32 thumbRatio, r32* scrollValue)
INLINE OC_UiTextBoxResult_t OC_UiTextBox(const char* name, OC_Arena_t* arena, MyStr_t text)
INLINE OC_UiSelectPopupInfo_t OC_UiSelectPopup(const char* name, OC_UiSelectPopupInfo_t* info)
INLINE OC_UiRadioGroupInfo_t OC_UiRadioGroup(const char* name, OC_UiRadioGroupInfo_t* info)
INLINE void OC_UiPanelBegin(const char* name, OC_UiFlags_t flags)
INLINE void OC_UiPanelEnd() 
INLINE void OC_UiMenuBarBegin(const char* name)
INLINE void OC_UiMenuBarEnd() 
INLINE void OC_UiMenuBegin(const char* label)
INLINE void OC_UiMenuEnd() 
INLINE OC_UiSig_t OC_UiMenuButton(const char* label)
INLINE void OC_UiTooltip(const char* label)
INLINE void OC_UiStyleNext(OC_UiStyle_t* style, OC_UiStyleMask_t mask)
INLINE void OC_UiPatternPush(OC_Arena_t* arena, OC_UiPattern_t* pattern, OC_UiSelector_t selector)
INLINE OC_UiPattern_t OC_UiPatternAll() 
INLINE OC_UiPattern_t OC_UiPatternOwner() 
INLINE void OC_UiStyleMatchBefore(OC_UiPattern_t pattern, OC_UiStyle_t* style, OC_UiStyleMask_t mask)
INLINE void OC_UiStyleMatchAfter(OC_UiPattern_t pattern, OC_UiStyle_t* style, OC_UiStyleMask_t mask)
#define OC_UiFrame(size, style, mask)
#define OC_UiPanel(s, f)
#define OC_UiMenuBar(name)
#define OC_UiMenu(name)
INLINE void OC_ArenaInit(OC_Arena_t* arena)
INLINE void OC_ArenaInitWithOptions(OC_Arena_t* arena, OC_ArenaOptions_t* options)
INLINE void OC_ArenaCleanup(OC_Arena_t* arena)
INLINE void* OC_ArenaPush(OC_Arena_t* arena, u64 size)
INLINE void OC_ArenaClear(OC_Arena_t* arena)
INLINE OC_ArenaScope_t OC_ArenaScopeBegin(OC_Arena_t* arena)
INLINE void OC_ArenaScopeEnd(OC_ArenaScope_t scope)
INLINE OC_ArenaScope_t OC_ScratchBegin() 
INLINE OC_ArenaScope_t OC_ScratchBeginNext(OC_Arena_t* used)
INLINE void OC_ListInit(OC_List_t* list)
INLINE bool OC_ListEmpty(OC_List_t* list)
INLINE OC_ListElement_t* OC_ListBegin(OC_List_t* list)
INLINE OC_ListElement_t* OC_ListEnd(OC_List_t* list)
INLINE OC_ListElement_t* OC_ListLast(OC_List_t* list)
INLINE void OC_ListInsert(OC_List_t* list, OC_ListElement_t* afterElt, OC_ListElement_t* elt)
INLINE void OC_ListInsertBefore(OC_List_t* list, OC_ListElement_t* beforeElt, OC_ListElement_t* elt)
INLINE void OC_ListRemove(OC_List_t* list, OC_ListElement_t* elt)
INLINE void OC_ListPush(OC_List_t* list, OC_ListElement_t* elt)
INLINE OC_ListElement_t* OC_ListPop(OC_List_t* list)
INLINE void OC_ListPushBack(OC_List_t* list, OC_ListElement_t* elt)
INLINE OC_ListElement_t* OC_ListPopBack(OC_List_t* list)
INLINE MyStr_t OC_Str8FromBuffer(u64 len, char* buffer)
INLINE MyStr_t OC_Str8Slice(MyStr_t s, u64 start, u64 end)
INLINE MyStr_t OC_Str8PushBuffer(OC_Arena_t* arena, u64 len, char* buffer)
INLINE MyStr_t OC_Str8PushCstring(OC_Arena_t* arena, const char* str)
INLINE MyStr_t OC_Str8PushCopy(OC_Arena_t* arena, MyStr_t s)
INLINE MyStr_t OC_Str8PushSlice(OC_Arena_t* arena, MyStr_t s, u64 start, u64 end)
INLINE MyStr_t OC_Str8Pushfv(OC_Arena_t* arena, const char* format, va_list args)
INLINE MyStr_t OC_Str8Pushf(OC_Arena_t* arena, const char* format, ...)
INLINE int OC_Str8Cmp(MyStr_t s1, MyStr_t s2)
INLINE char* OC_Str8ToCstring(OC_Arena_t* arena, MyStr_t string)
INLINE void OC_Str8ListPush(OC_Arena_t* arena, oc_str8_list* list, MyStr_t str)
INLINE void OC_Str8ListPushf(OC_Arena_t* arena, oc_str8_list* list, const char* format, ...)
INLINE MyStr_t OC_Str8ListCollate(OC_Arena_t* arena, oc_str8_list list, MyStr_t prefix, MyStr_t separator, MyStr_t postfix)
INLINE MyStr_t OC_Str8ListJoin(OC_Arena_t* arena, oc_str8_list list)
INLINE oc_str8_list OC_Str8Split(OC_Arena_t* arena, MyStr_t str, oc_str8_list separators)
INLINE MyStr_t OC_PathSliceDirectory(MyStr_t path)
INLINE MyStr_t OC_PathSliceFilename(MyStr_t path)
INLINE oc_str8_list OC_PathSplit(OC_Arena_t* arena, MyStr_t path)
INLINE MyStr_t OC_PathJoin(OC_Arena_t* arena, oc_str8_list elements)
INLINE MyStr_t OC_PathAppend(OC_Arena_t* arena, MyStr_t parent, MyStr_t relPath)
INLINE bool OC_PathIsAbsolute(MyStr_t path)
#define OC_Str8Pushf(arena, format, ...)
#define OC_Str8ListPushf(arena, list, format, ...)
#define OC_ArenaPushType(arena, type)
#define OC_ArenaPushArray(arena, type, count)
#define OC_ScratchEnd(scope)
#define OC_ListNext(element)
#define OC_ListPrev(element)
#define OC_ListEntry(element, type, member)
#define OC_ListNextEntry(list, element, type, member)
#define OC_ListPrevEntry(list, element, type, member)
#define OC_ListFirstEntry(list, type, member)
#define OC_ListLastEntry(list, type, member)
#define OC_ListFor(list, element, type, member)
#define OC_ListForReverse(list, element, type, member)
#define OC_ListForSafe(list, element, type, member)
#define OC_ListPopFrontEntry(list, type, member)
#define OC_ListPopBackEntry(list, type, member)
#define OC_Log_I(message, ...)
#define OC_Log_W(message, ...)
#define OC_Log_E(message, ...)
#define OC_Assert(test, message, ...)
#define OC_Abort(message, ...)
*/
