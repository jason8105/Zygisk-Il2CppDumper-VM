//
// Created by Perfare on 2020/7/4.
// Forked by ZEFF
//
#include "xdl.h"
#include <stddef.h>
#include <pthread.h>
#include <jni.h>
#include "il2cpp-tabledefs.h"
#include "il2cpp-class.h"
#include "game.h"

static int enable_hack;
static void *handle;
int isGame(JNIEnv *env, jstring appDataDir);
static char *game_data_dir = NULL;
pthread_t gameThread;
uintptr_t absoluteStart = 0;



// initialization and runtime setup
int (*il2cpp_init)(const char* domain_name);
int (*il2cpp_init_utf16)(const Il2CppChar * domain_name);
void (*il2cpp_shutdown)();
void (*il2cpp_set_config_dir)(const char *config_path);
void (*il2cpp_set_data_dir)(const char *data_path);
void (*il2cpp_set_temp_dir)(const char *temp_path);
void (*il2cpp_set_commandline_arguments)(int argc, const char* const argv[], const char* basedir);
void (*il2cpp_set_commandline_arguments_utf16)(int argc, const Il2CppChar * const argv[], const char* basedir);
void (*il2cpp_set_config_utf16)(const Il2CppChar * executablePath);
void (*il2cpp_set_config)(const char* executablePath);

void (*il2cpp_set_memory_callbacks)(Il2CppMemoryCallbacks * callbacks);
const Il2CppImage* (*il2cpp_get_corlib)();
void (*il2cpp_add_internal_call)(const char* name, Il2CppMethodPointer method);
Il2CppMethodPointer (*il2cpp_resolve_icall)(const char* name);

// memory allocation
void* (*il2cpp_alloc)(size_t size);
void (*il2cpp_free)(void* ptr);

// array
Il2CppClass* (*il2cpp_array_class_get)(Il2CppClass * element_class, uint32_t rank);
uint32_t (*il2cpp_array_length)(Il2CppArray * array);
uint32_t (*il2cpp_array_get_byte_length)(Il2CppArray * array);
Il2CppArray* (*il2cpp_array_new)(Il2CppClass * elementTypeInfo, il2cpp_array_size_t length);
Il2CppArray* (*il2cpp_array_new_specific)(Il2CppClass * arrayTypeInfo, il2cpp_array_size_t length);
Il2CppArray* (*il2cpp_array_new_full)(Il2CppClass * array_class, il2cpp_array_size_t * lengths, il2cpp_array_size_t * lower_bounds);
Il2CppClass* (*il2cpp_bounded_array_class_get)(Il2CppClass * element_class, uint32_t rank, bool bounded);
int (*il2cpp_array_element_size)(const Il2CppClass * array_class);

// assembly
const Il2CppImage* (*il2cpp_assembly_get_image)(const Il2CppAssembly * assembly);

// class
void (*il2cpp_class_for_each)(void(*klassReportFunc)(Il2CppClass* klass, void* userData), void* userData);
const Il2CppType* (*il2cpp_class_enum_basetype)(Il2CppClass * klass);
bool (*il2cpp_class_is_generic)(const Il2CppClass * klass);
bool (*il2cpp_class_is_inflated)(const Il2CppClass * klass);
bool (*il2cpp_class_is_assignable_from)(Il2CppClass * klass, Il2CppClass * oklass);
bool (*il2cpp_class_is_subclass_of)(Il2CppClass * klass, Il2CppClass * klassc, bool check_interfaces);
bool (*il2cpp_class_has_parent)(Il2CppClass * klass, Il2CppClass * klassc);
Il2CppClass* (*il2cpp_class_from_il2cpp_type)(const Il2CppType * type);
Il2CppClass* (*il2cpp_class_from_name)(const Il2CppImage * image, const char* namespaze, const char *name);
Il2CppClass* (*il2cpp_class_from_system_type)(Il2CppReflectionType * type);
Il2CppClass* (*il2cpp_class_get_element_class)(Il2CppClass * klass);
const EventInfo* (*il2cpp_class_get_events)(Il2CppClass * klass, void* *iter);
FieldInfo* (*il2cpp_class_get_fields)(Il2CppClass * klass, void* *iter);
Il2CppClass* (*il2cpp_class_get_nested_types)(Il2CppClass * klass, void* *iter);
Il2CppClass* (*il2cpp_class_get_interfaces)(Il2CppClass * klass, void* *iter);
const PropertyInfo* (*il2cpp_class_get_properties)(Il2CppClass * klass, void* *iter);
const PropertyInfo* (*il2cpp_class_get_property_from_name)(Il2CppClass * klass, const char *name);
FieldInfo* (*il2cpp_class_get_field_from_name)(Il2CppClass * klass, const char *name);
const MethodInfo* (*il2cpp_class_get_methods)(Il2CppClass * klass, void* *iter);
const MethodInfo* (*il2cpp_class_get_method_from_name)(Il2CppClass * klass, const char* name, int argsCount);
const char* (*il2cpp_class_get_name)(Il2CppClass * klass);
void (*il2cpp_type_get_name_chunked)(const Il2CppType * type, void(*chunkReportFunc)(void* data, void* userData), void* userData);
const char* (*il2cpp_class_get_namespace)(Il2CppClass * klass);
Il2CppClass* (*il2cpp_class_get_parent)(Il2CppClass * klass);
Il2CppClass* (*il2cpp_class_get_declaring_type)(Il2CppClass * klass);
int32_t (*il2cpp_class_instance_size)(Il2CppClass * klass);
size_t (*il2cpp_class_num_fields)(const Il2CppClass * enumKlass);
bool (*il2cpp_class_is_valuetype)(const Il2CppClass * klass);
int32_t (*il2cpp_class_value_size)(Il2CppClass * klass, uint32_t * align);
bool (*il2cpp_class_is_blittable)(const Il2CppClass * klass);
int (*il2cpp_class_get_flags)(const Il2CppClass * klass);
bool (*il2cpp_class_is_abstract)(const Il2CppClass * klass);
bool (*il2cpp_class_is_interface)(const Il2CppClass * klass);
int (*il2cpp_class_array_element_size)(const Il2CppClass * klass);
Il2CppClass* (*il2cpp_class_from_type)(const Il2CppType * type);
const Il2CppType* (*il2cpp_class_get_type)(Il2CppClass * klass);
uint32_t (*il2cpp_class_get_type_token)(Il2CppClass * klass);
bool (*il2cpp_class_has_attribute)(Il2CppClass * klass, Il2CppClass * attr_class);
bool (*il2cpp_class_has_references)(Il2CppClass * klass);
bool (*il2cpp_class_is_enum)(const Il2CppClass * klass);
const Il2CppImage* (*il2cpp_class_get_image)(Il2CppClass * klass);
const char* (*il2cpp_class_get_assemblyname)(const Il2CppClass * klass);
int (*il2cpp_class_get_rank)(const Il2CppClass * klass);
uint32_t (*il2cpp_class_get_data_size)(const Il2CppClass * klass);
void* (*il2cpp_class_get_static_field_data)(const Il2CppClass * klass);

// testing only
size_t (*il2cpp_class_get_bitmap_size)(const Il2CppClass * klass);
void (*il2cpp_class_get_bitmap)(Il2CppClass * klass, size_t * bitmap);

// stats
bool (*il2cpp_stats_dump_to_file)(const char *path);
uint64_t (*il2cpp_stats_get_value)(Il2CppStat stat);

// domain
Il2CppDomain* (*il2cpp_domain_get)();
const Il2CppAssembly* (*il2cpp_domain_assembly_open)(Il2CppDomain * domain, const char* name);
const Il2CppAssembly** (*il2cpp_domain_get_assemblies)(const Il2CppDomain * domain, size_t * size);

// exception
void (*il2cpp_raise_exception)(Il2CppException*);
Il2CppException* (*il2cpp_exception_from_name_msg)(const Il2CppImage * image, const char *name_space, const char *name, const char *msg);
Il2CppException* (*il2cpp_get_exception_argument_null)(const char *arg);
void (*il2cpp_format_exception)(const Il2CppException * ex, char* message, int message_size);
void (*il2cpp_format_stack_trace)(const Il2CppException * ex, char* output, int output_size);
void (*il2cpp_unhandled_exception)(Il2CppException*);
void (*il2cpp_native_stack_trace)(const Il2CppException * ex, uintptr_t** addresses, int* numFrames, char** imageUUID, char** imageName);

// field
int (*il2cpp_field_get_flags)(FieldInfo * field);
const char* (*il2cpp_field_get_name)(FieldInfo * field);
Il2CppClass* (*il2cpp_field_get_parent)(FieldInfo * field);
size_t (*il2cpp_field_get_offset)(FieldInfo * field);
const Il2CppType* (*il2cpp_field_get_type)(FieldInfo * field);
void (*il2cpp_field_get_value)(Il2CppObject * obj, FieldInfo * field, void *value);
Il2CppObject* (*il2cpp_field_get_value_object)(FieldInfo * field, Il2CppObject * obj);
bool (*il2cpp_field_has_attribute)(FieldInfo * field, Il2CppClass * attr_class);
void (*il2cpp_field_set_value)(Il2CppObject * obj, FieldInfo * field, void *value);
void (*il2cpp_field_static_get_value)(FieldInfo * field, void *value);
void (*il2cpp_field_static_set_value)(FieldInfo * field, void *value);
void (*il2cpp_field_set_value_object)(Il2CppObject * instance, FieldInfo * field, Il2CppObject * value);
bool (*il2cpp_field_is_literal)(FieldInfo * field);

// gc
void (*il2cpp_gc_collect)(int maxGenerations);
int32_t (*il2cpp_gc_collect_a_little)();
void (*il2cpp_gc_start_incremental_collection)();
void (*il2cpp_gc_disable)();
void (*il2cpp_gc_enable)();
bool (*il2cpp_gc_is_disabled)();
void (*il2cpp_gc_set_mode)(Il2CppGCMode mode);
int64_t (*il2cpp_gc_get_max_time_slice_ns)();
void (*il2cpp_gc_set_max_time_slice_ns)(int64_t maxTimeSlice);
bool (*il2cpp_gc_is_incremental)();
int64_t (*il2cpp_gc_get_used_size)();
int64_t (*il2cpp_gc_get_heap_size)();
void (*il2cpp_gc_wbarrier_set_field)(Il2CppObject * obj, void **targetAddress, void *object);
bool (*il2cpp_gc_has_strict_wbarriers)();
void (*il2cpp_gc_set_external_allocation_tracker)(void(*func)(void*, size_t, int));
void (*il2cpp_gc_set_external_wbarrier_tracker)(void(*func)(void**));
void (*il2cpp_gc_foreach_heap)(void(*func)(void* data, void* userData), void* userData);
void (*il2cpp_stop_gc_world)();
void (*il2cpp_start_gc_world)();
void* (*il2cpp_gc_alloc_fixed)(size_t size);
void (*il2cpp_gc_free_fixed)(void* address);

// gchandle
uint32_t (*il2cpp_gchandle_new)(Il2CppObject * obj, bool pinned);
uint32_t (*il2cpp_gchandle_new_weakref)(Il2CppObject * obj, bool track_resurrection);
Il2CppObject* (*il2cpp_gchandle_get_target)(uint32_t gchandle);
void (*il2cpp_gchandle_free)(uint32_t gchandle);
void (*il2cpp_gchandle_foreach_get_target)(void(*func)(void* data, void* userData), void* userData);

// vm runtime info
uint32_t (*il2cpp_object_header_size)();
uint32_t (*il2cpp_array_object_header_size)();
uint32_t (*il2cpp_offset_of_array_length_in_array_object_header)();
uint32_t (*il2cpp_offset_of_array_bounds_in_array_object_header)();
uint32_t (*il2cpp_allocation_granularity)();

// liveness
void* (*il2cpp_unity_liveness_allocate_struct)(Il2CppClass * filter, int max_object_count, il2cpp_register_object_callback callback, void* userdata, il2cpp_liveness_reallocate_callback reallocate);
void (*il2cpp_unity_liveness_calculation_from_root)(Il2CppObject * root, void* state);
void (*il2cpp_unity_liveness_calculation_from_statics)(void* state);
void (*il2cpp_unity_liveness_finalize)(void* state);
void (*il2cpp_unity_liveness_free_struct)(void* state);

// method
const Il2CppType* (*il2cpp_method_get_return_type)(const MethodInfo * method);
Il2CppClass* (*il2cpp_method_get_declaring_type)(const MethodInfo * method);
const char* (*il2cpp_method_get_name)(const MethodInfo * method);
const MethodInfo* (*il2cpp_method_get_from_reflection)(const Il2CppReflectionMethod * method);
Il2CppReflectionMethod* (*il2cpp_method_get_object)(const MethodInfo * method, Il2CppClass * refclass);
bool (*il2cpp_method_is_generic)(const MethodInfo * method);
bool (*il2cpp_method_is_inflated)(const MethodInfo * method);
bool (*il2cpp_method_is_instance)(const MethodInfo * method);
uint32_t (*il2cpp_method_get_param_count)(const MethodInfo * method);
const Il2CppType* (*il2cpp_method_get_param)(const MethodInfo * method, uint32_t index);
Il2CppClass* (*il2cpp_method_get_class)(const MethodInfo * method);
bool (*il2cpp_method_has_attribute)(const MethodInfo * method, Il2CppClass * attr_class);
uint32_t (*il2cpp_method_get_flags)(const MethodInfo * method, uint32_t * iflags);
uint32_t (*il2cpp_method_get_token)(const MethodInfo * method);
const char* (*il2cpp_method_get_param_name)(const MethodInfo * method, uint32_t index);

// profiler
#if IL2CPP_ENABLE_PROFILER
void (*il2cpp_profiler_install)(Il2CppProfiler * prof, Il2CppProfileFunc shutdown_callback);
void (*il2cpp_profiler_set_events)(Il2CppProfileFlags events);
void (*il2cpp_profiler_install_enter_leave)(Il2CppProfileMethodFunc enter, Il2CppProfileMethodFunc fleave);
void (*il2cpp_profiler_install_allocation)(Il2CppProfileAllocFunc callback);
void (*il2cpp_profiler_install_gc)(Il2CppProfileGCFunc callback, Il2CppProfileGCResizeFunc heap_resize_callback);
void (*il2cpp_profiler_install_fileio)(Il2CppProfileFileIOFunc callback);
void (*il2cpp_profiler_install_thread)(Il2CppProfileThreadFunc start, Il2CppProfileThreadFunc end);
#endif

// property
uint32_t (*il2cpp_property_get_flags)(PropertyInfo * prop);
const MethodInfo* (*il2cpp_property_get_get_method)(PropertyInfo * prop);
const MethodInfo* (*il2cpp_property_get_set_method)(PropertyInfo * prop);
const char* (*il2cpp_property_get_name)(PropertyInfo * prop);
Il2CppClass* (*il2cpp_property_get_parent)(PropertyInfo * prop);

// object
Il2CppClass* (*il2cpp_object_get_class)(Il2CppObject * obj);
uint32_t (*il2cpp_object_get_size)(Il2CppObject * obj);
const MethodInfo* (*il2cpp_object_get_virtual_method)(Il2CppObject * obj, const MethodInfo * method);
Il2CppObject* (*il2cpp_object_new)(const Il2CppClass * klass);
void* (*il2cpp_object_unbox)(Il2CppObject * obj);
Il2CppObject* (*il2cpp_value_box)(Il2CppClass * klass, void* data);

// monitor
void (*il2cpp_monitor_enter)(Il2CppObject * obj);
bool (*il2cpp_monitor_try_enter)(Il2CppObject * obj, uint32_t timeout);
void (*il2cpp_monitor_exit)(Il2CppObject * obj);
void (*il2cpp_monitor_pulse)(Il2CppObject * obj);
void (*il2cpp_monitor_pulse_all)(Il2CppObject * obj);
void (*il2cpp_monitor_wait)(Il2CppObject * obj);
bool (*il2cpp_monitor_try_wait)(Il2CppObject * obj, uint32_t timeout);

// runtime
Il2CppObject* (*il2cpp_runtime_invoke)(const MethodInfo * method, void *obj, void **params, Il2CppException **exc);
Il2CppObject* (*il2cpp_runtime_invoke_convert_args)(const MethodInfo * method, void *obj, Il2CppObject **params, int paramCount, Il2CppException **exc);
void (*il2cpp_runtime_class_init)(Il2CppClass * klass);
void (*il2cpp_runtime_object_init)(Il2CppObject * obj);
void (*il2cpp_runtime_object_init_exception)(Il2CppObject * obj, Il2CppException** exc);
void (*il2cpp_runtime_unhandled_exception_policy_set)(Il2CppRuntimeUnhandledExceptionPolicy value);

// string
int32_t (*il2cpp_string_length)(Il2CppString * str);
Il2CppChar* (*il2cpp_string_chars)(Il2CppString * str);
Il2CppString* (*il2cpp_string_new)(const char* str);
Il2CppString* (*il2cpp_string_new_len)(const char* str, uint32_t length);
Il2CppString* (*il2cpp_string_new_utf16)(const Il2CppChar * text, int32_t len);
Il2CppString* (*il2cpp_string_new_wrapper)(const char* str);
Il2CppString* (*il2cpp_string_intern)(Il2CppString * str);
Il2CppString* (*il2cpp_string_is_interned)(Il2CppString * str);

// thread
Il2CppThread* (*il2cpp_thread_current)();
Il2CppThread* (*il2cpp_thread_attach)(Il2CppDomain * domain);
void (*il2cpp_thread_detach)(Il2CppThread * thread);
Il2CppThread** (*il2cpp_thread_get_all_attached_threads)(size_t * size);
bool (*il2cpp_is_vm_thread)(Il2CppThread * thread);

// stacktrace
void (*il2cpp_current_thread_walk_frame_stack)(Il2CppFrameWalkFunc func, void* user_data);
void (*il2cpp_thread_walk_frame_stack)(Il2CppThread * thread, Il2CppFrameWalkFunc func, void* user_data);
bool (*il2cpp_current_thread_get_top_frame)(Il2CppStackFrameInfo * frame);
bool (*il2cpp_thread_get_top_frame)(Il2CppThread * thread, Il2CppStackFrameInfo * frame);
bool (*il2cpp_current_thread_get_frame_at)(int32_t offset, Il2CppStackFrameInfo * frame);
bool (*il2cpp_thread_get_frame_at)(Il2CppThread * thread, int32_t offset, Il2CppStackFrameInfo * frame);
int32_t (*il2cpp_current_thread_get_stack_depth)();
int32_t (*il2cpp_thread_get_stack_depth)(Il2CppThread * thread);
void (*il2cpp_override_stack_backtrace)(Il2CppBacktraceFunc stackBacktraceFunc);

// type
Il2CppObject* (*il2cpp_type_get_object)(const Il2CppType * type);
int (*il2cpp_type_get_type)(const Il2CppType * type);
Il2CppClass* (*il2cpp_type_get_class_or_element_class)(const Il2CppType * type);
char* (*il2cpp_type_get_name)(const Il2CppType * type);
bool (*il2cpp_type_is_byref)(const Il2CppType * type);
uint32_t (*il2cpp_type_get_attrs)(const Il2CppType * type);
bool (*il2cpp_type_equals)(const Il2CppType * type, const Il2CppType * otherType);
char* (*il2cpp_type_get_assembly_qualified_name)(const Il2CppType * type);
bool (*il2cpp_type_is_static)(const Il2CppType * type);
bool (*il2cpp_type_is_pointer_type)(const Il2CppType * type);

// image
const Il2CppAssembly* (*il2cpp_image_get_assembly)(const Il2CppImage * image);
const char* (*il2cpp_image_get_name)(const Il2CppImage * image);
const char* (*il2cpp_image_get_filename)(const Il2CppImage * image);
const MethodInfo* (*il2cpp_image_get_entry_point)(const Il2CppImage * image);
size_t (*il2cpp_image_get_class_count)(const Il2CppImage * image);
const Il2CppClass* (*il2cpp_image_get_class)(const Il2CppImage * image, size_t index);

// Memory information
Il2CppManagedMemorySnapshot* (*il2cpp_capture_memory_snapshot)();
void (*il2cpp_free_captured_memory_snapshot)(Il2CppManagedMemorySnapshot * snapshot);

// Plugin callback
void (*il2cpp_set_find_plugin_callback)(Il2CppSetFindPlugInCallback method);

// Logging
void (*il2cpp_register_log_callback)(Il2CppLogCallback method);

// Debugger
void (*il2cpp_debugger_set_agent_options)(const char* options);
bool (*il2cpp_is_debugger_attached)();
void (*il2cpp_register_debugger_agent_transport)(Il2CppDebuggerTransport * debuggerTransport);

// Debug metadata
bool (*il2cpp_debug_get_method_info)(const MethodInfo*, Il2CppMethodDebugInfo * methodDebugInfo);

// TLS module
void (*il2cpp_unity_install_unitytls_interface)(const void* unitytlsInterfaceStruct);

// custom attributes
Il2CppCustomAttrInfo* (*il2cpp_custom_attrs_from_class)(Il2CppClass * klass);
Il2CppCustomAttrInfo* (*il2cpp_custom_attrs_from_method)(const MethodInfo * method);
Il2CppObject* (*il2cpp_custom_attrs_get_attr)(Il2CppCustomAttrInfo * ainfo, Il2CppClass * attr_klass);
bool (*il2cpp_custom_attrs_has_attr)(Il2CppCustomAttrInfo * ainfo, Il2CppClass * attr_klass);
Il2CppArray* (*il2cpp_custom_attrs_construct)(Il2CppCustomAttrInfo * cinfo);
void (*il2cpp_custom_attrs_free)(Il2CppCustomAttrInfo * ainfo);

// Il2CppClass user data for GetComponent optimization
void (*il2cpp_class_set_userdata)(Il2CppClass * klass, void* userdata);
int (*il2cpp_class_get_userdata_offset)();

// Thread affinity
void (*il2cpp_set_default_thread_affinity)(int64_t affinity_mask);

void *hack_thread(void *arg);

void init_il2cpp_api(void* handle) {
    if (automaticDumpWithSymbols) {
        // initialization and runtime setup
        il2cpp_init = (int (*)(const char*))xdl_sym(handle, "il2cpp_init", nullptr);
        il2cpp_init_utf16 = (int (*)(const Il2CppChar*))xdl_sym(handle, "il2cpp_init_utf16", nullptr);
        il2cpp_shutdown = (void (*)())xdl_sym(handle, "il2cpp_shutdown", nullptr);
        il2cpp_set_config_dir = (void (*)(const char*))xdl_sym(handle, "il2cpp_set_config_dir", nullptr);
        il2cpp_set_data_dir = (void (*)(const char*))xdl_sym(handle, "il2cpp_set_data_dir", nullptr);
        il2cpp_set_temp_dir = (void (*)(const char*))xdl_sym(handle, "il2cpp_set_temp_dir", nullptr);
        il2cpp_set_commandline_arguments = (void (*)(int, const char* const[], const char*))xdl_sym(handle, "il2cpp_set_commandline_arguments", nullptr);
        il2cpp_set_commandline_arguments_utf16 = (void (*)(int, const Il2CppChar* const[], const char*))xdl_sym(handle, "il2cpp_set_commandline_arguments_utf16", nullptr);
        il2cpp_set_config_utf16 = (void (*)(const Il2CppChar*))xdl_sym(handle, "il2cpp_set_config_utf16", nullptr);
        il2cpp_set_config = (void (*)(const char*))xdl_sym(handle, "il2cpp_set_config", nullptr);

        il2cpp_set_memory_callbacks = (void (*)(Il2CppMemoryCallbacks*))xdl_sym(handle, "il2cpp_set_memory_callbacks", nullptr);
        il2cpp_get_corlib = (const Il2CppImage* (*)())xdl_sym(handle, "il2cpp_get_corlib", nullptr);
        il2cpp_add_internal_call = (void (*)(const char*, Il2CppMethodPointer))xdl_sym(handle, "il2cpp_add_internal_call", nullptr);
        il2cpp_resolve_icall = (Il2CppMethodPointer (*)(const char*))xdl_sym(handle, "il2cpp_resolve_icall", nullptr);

        // memory allocation
        il2cpp_alloc = (void* (*)(size_t))xdl_sym(handle, "il2cpp_alloc", nullptr);
        il2cpp_free = (void (*)(void*))xdl_sym(handle, "il2cpp_free", nullptr);

        // array
        il2cpp_array_class_get = (Il2CppClass* (*)(Il2CppClass*, uint32_t))xdl_sym(handle, "il2cpp_array_class_get", nullptr);
        il2cpp_array_length = (uint32_t (*)(Il2CppArray*))xdl_sym(handle, "il2cpp_array_length", nullptr);
        il2cpp_array_get_byte_length = (uint32_t (*)(Il2CppArray*))xdl_sym(handle, "il2cpp_array_get_byte_length", nullptr);
        il2cpp_array_new = (Il2CppArray* (*)(Il2CppClass*, il2cpp_array_size_t))xdl_sym(handle, "il2cpp_array_new", nullptr);
        il2cpp_array_new_specific = (Il2CppArray* (*)(Il2CppClass*, il2cpp_array_size_t))xdl_sym(handle, "il2cpp_array_new_specific", nullptr);
        il2cpp_array_new_full = (Il2CppArray* (*)(Il2CppClass*, il2cpp_array_size_t*, il2cpp_array_size_t*))xdl_sym(handle, "il2cpp_array_new_full", nullptr);
        il2cpp_bounded_array_class_get = (Il2CppClass* (*)(Il2CppClass*, uint32_t, bool))xdl_sym(handle, "il2cpp_bounded_array_class_get", nullptr);
        il2cpp_array_element_size = (int (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_array_element_size", nullptr);

        // assembly
        il2cpp_assembly_get_image = (const Il2CppImage* (*)(const Il2CppAssembly*))xdl_sym(handle, "il2cpp_assembly_get_image", nullptr);

        // class
        il2cpp_class_for_each = (void (*)(void (*)(Il2CppClass*, void*), void*))xdl_sym(handle, "il2cpp_class_for_each", nullptr);
        il2cpp_class_enum_basetype = (const Il2CppType* (*)(Il2CppClass*))xdl_sym(handle, "il2cpp_class_enum_basetype", nullptr);
        il2cpp_class_is_generic = (bool (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_is_generic", nullptr);
        il2cpp_class_is_inflated = (bool (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_is_inflated", nullptr);
        il2cpp_class_is_assignable_from = (bool (*)(Il2CppClass*, Il2CppClass*))xdl_sym(handle, "il2cpp_class_is_assignable_from", nullptr);
        il2cpp_class_is_subclass_of = (bool (*)(Il2CppClass*, Il2CppClass*, bool))xdl_sym(handle, "il2cpp_class_is_subclass_of", nullptr);
        il2cpp_class_has_parent = (bool (*)(Il2CppClass*, Il2CppClass*))xdl_sym(handle, "il2cpp_class_has_parent", nullptr);
        il2cpp_class_from_il2cpp_type = (Il2CppClass* (*)(const Il2CppType*))xdl_sym(handle, "il2cpp_class_from_il2cpp_type", nullptr);
        il2cpp_class_from_name = (Il2CppClass* (*)(const Il2CppImage*, const char*, const char*))xdl_sym(handle, "il2cpp_class_from_name", nullptr);
        il2cpp_class_from_system_type = (Il2CppClass* (*)(Il2CppReflectionType*))xdl_sym(handle, "il2cpp_class_from_system_type", nullptr);
        il2cpp_class_get_element_class = (Il2CppClass* (*)(Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_element_class", nullptr);
        il2cpp_class_get_events = (const EventInfo* (*)(Il2CppClass*, void**))xdl_sym(handle, "il2cpp_class_get_events", nullptr);
        il2cpp_class_get_fields = (FieldInfo* (*)(Il2CppClass*, void**))xdl_sym(handle, "il2cpp_class_get_fields", nullptr);
        il2cpp_class_get_nested_types = (Il2CppClass* (*)(Il2CppClass*, void**))xdl_sym(handle, "il2cpp_class_get_nested_types", nullptr);
        il2cpp_class_get_interfaces = (Il2CppClass* (*)(Il2CppClass*, void**))xdl_sym(handle, "il2cpp_class_get_interfaces", nullptr);
        il2cpp_class_get_properties = (const PropertyInfo* (*)(Il2CppClass*, void**))xdl_sym(handle, "il2cpp_class_get_properties", nullptr);
        il2cpp_class_get_property_from_name = (const PropertyInfo* (*)(Il2CppClass*, const char*))xdl_sym(handle, "il2cpp_class_get_property_from_name", nullptr);
        il2cpp_class_get_field_from_name = (FieldInfo* (*)(Il2CppClass*, const char*))xdl_sym(handle, "il2cpp_class_get_field_from_name", nullptr);
        il2cpp_class_get_methods = (const MethodInfo* (*)(Il2CppClass*, void**))xdl_sym(handle, "il2cpp_class_get_methods", nullptr);
        il2cpp_class_get_method_from_name = (const MethodInfo* (*)(Il2CppClass*, const char*, int))xdl_sym(handle, "il2cpp_class_get_method_from_name", nullptr);
        il2cpp_class_get_name = (const char* (*)(Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_name", nullptr);
        il2cpp_type_get_name_chunked = (void (*)(const Il2CppType*, void (*)(void*, void*), void*))xdl_sym(handle, "il2cpp_type_get_name_chunked", nullptr);
        il2cpp_class_get_namespace = (const char* (*)(Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_namespace", nullptr);
        il2cpp_class_get_parent = (Il2CppClass* (*)(Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_parent", nullptr);
        il2cpp_class_get_declaring_type = (Il2CppClass* (*)(Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_declaring_type", nullptr);
        il2cpp_class_instance_size = (int32_t (*)(Il2CppClass*))xdl_sym(handle, "il2cpp_class_instance_size", nullptr);
        il2cpp_class_num_fields = (size_t (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_num_fields", nullptr);
        il2cpp_class_is_valuetype = (bool (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_is_valuetype", nullptr);
        il2cpp_class_value_size = (int32_t (*)(Il2CppClass*, uint32_t*))xdl_sym(handle, "il2cpp_class_value_size", nullptr);
        il2cpp_class_is_blittable = (bool (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_is_blittable", nullptr);
        il2cpp_class_get_flags = (int (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_flags", nullptr);
        il2cpp_class_is_abstract = (bool (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_is_abstract", nullptr);
        il2cpp_class_is_interface = (bool (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_is_interface", nullptr);
        il2cpp_class_array_element_size = (int (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_array_element_size", nullptr);
        il2cpp_class_from_type = (Il2CppClass* (*)(const Il2CppType*))xdl_sym(handle, "il2cpp_class_from_type", nullptr);
        il2cpp_class_get_type = (const Il2CppType* (*)(Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_type", nullptr);
        il2cpp_class_get_type_token = (uint32_t (*)(Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_type_token", nullptr);
        il2cpp_class_has_attribute = (bool (*)(Il2CppClass*, Il2CppClass*))xdl_sym(handle, "il2cpp_class_has_attribute", nullptr);
        il2cpp_class_has_references = (bool (*)(Il2CppClass*))xdl_sym(handle, "il2cpp_class_has_references", nullptr);
        il2cpp_class_is_enum = (bool (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_is_enum", nullptr);
        il2cpp_class_get_image = (const Il2CppImage* (*)(Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_image", nullptr);
        il2cpp_class_get_assemblyname = (const char* (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_assemblyname", nullptr);
        il2cpp_class_get_rank = (int (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_rank", nullptr);
        il2cpp_class_get_data_size = (uint32_t (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_data_size", nullptr);
        il2cpp_class_get_static_field_data = (void* (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_static_field_data", nullptr);

        // testing only
        il2cpp_class_get_bitmap_size = (size_t (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_class_get_bitmap_size", nullptr);
        il2cpp_class_get_bitmap = (void (*)(Il2CppClass*, size_t*))xdl_sym(handle, "il2cpp_class_get_bitmap", nullptr);

        // stats
        il2cpp_stats_dump_to_file = (bool (*)(const char*))xdl_sym(handle, "il2cpp_stats_dump_to_file", nullptr);
        il2cpp_stats_get_value = (uint64_t (*)(Il2CppStat))xdl_sym(handle, "il2cpp_stats_get_value", nullptr);

        // domain
        il2cpp_domain_get = (Il2CppDomain* (*)())xdl_sym(handle, "il2cpp_domain_get", nullptr);
        il2cpp_domain_assembly_open = (const Il2CppAssembly* (*)(Il2CppDomain*, const char*))xdl_sym(handle, "il2cpp_domain_assembly_open", nullptr);
        il2cpp_domain_get_assemblies = (const Il2CppAssembly** (*)(const Il2CppDomain*, size_t*))xdl_sym(handle, "il2cpp_domain_get_assemblies", nullptr);

        // exception
        il2cpp_raise_exception = (void (*)(Il2CppException*))xdl_sym(handle, "il2cpp_raise_exception", nullptr);
        il2cpp_exception_from_name_msg = (Il2CppException* (*)(const Il2CppImage*, const char*, const char*, const char*))xdl_sym(handle, "il2cpp_exception_from_name_msg", nullptr);
        il2cpp_get_exception_argument_null = (Il2CppException* (*)(const char*))xdl_sym(handle, "il2cpp_get_exception_argument_null", nullptr);
        il2cpp_format_exception = (void (*)(const Il2CppException*, char*, int))xdl_sym(handle, "il2cpp_format_exception", nullptr);
        il2cpp_format_stack_trace = (void (*)(const Il2CppException*, char*, int))xdl_sym(handle, "il2cpp_format_stack_trace", nullptr);
        il2cpp_unhandled_exception = (void (*)(Il2CppException*))xdl_sym(handle, "il2cpp_unhandled_exception", nullptr);
        il2cpp_native_stack_trace = (void (*)(const Il2CppException*, uintptr_t**, int*, char**, char**))xdl_sym(handle, "il2cpp_native_stack_trace", nullptr);

        // field
        il2cpp_field_get_flags = (int (*)(FieldInfo*))xdl_sym(handle, "il2cpp_field_get_flags", nullptr);
        il2cpp_field_get_name = (const char* (*)(FieldInfo*))xdl_sym(handle, "il2cpp_field_get_name", nullptr);
        il2cpp_field_get_parent = (Il2CppClass* (*)(FieldInfo*))xdl_sym(handle, "il2cpp_field_get_parent", nullptr);
        il2cpp_field_get_offset = (size_t (*)(FieldInfo*))xdl_sym(handle, "il2cpp_field_get_offset", nullptr);
        il2cpp_field_get_type = (const Il2CppType* (*)(FieldInfo*))xdl_sym(handle, "il2cpp_field_get_type", nullptr);
        il2cpp_field_get_value = (void (*)(Il2CppObject*, FieldInfo*, void*))xdl_sym(handle, "il2cpp_field_get_value", nullptr);
        il2cpp_field_get_value_object = (Il2CppObject* (*)(FieldInfo*, Il2CppObject*))xdl_sym(handle, "il2cpp_field_get_value_object", nullptr);
        il2cpp_field_has_attribute = (bool (*)(FieldInfo*, Il2CppClass*))xdl_sym(handle, "il2cpp_field_has_attribute", nullptr);
        il2cpp_field_set_value = (void (*)(Il2CppObject*, FieldInfo*, void*))xdl_sym(handle, "il2cpp_field_set_value", nullptr);
        il2cpp_field_static_get_value = (void (*)(FieldInfo*, void*))xdl_sym(handle, "il2cpp_field_static_get_value", nullptr);
        il2cpp_field_static_set_value = (void (*)(FieldInfo*, void*))xdl_sym(handle, "il2cpp_field_static_set_value", nullptr);
        il2cpp_field_set_value_object = (void (*)(Il2CppObject*, FieldInfo*, Il2CppObject*))xdl_sym(handle, "il2cpp_field_set_value_object", nullptr);
        il2cpp_field_is_literal = (bool (*)(FieldInfo*))xdl_sym(handle, "il2cpp_field_is_literal", nullptr);

        // gc
        il2cpp_gc_collect = (void (*)(int))xdl_sym(handle, "il2cpp_gc_collect", nullptr);
        il2cpp_gc_collect_a_little = (int32_t (*)())xdl_sym(handle, "il2cpp_gc_collect_a_little", nullptr);
        il2cpp_gc_start_incremental_collection = (void (*)())xdl_sym(handle, "il2cpp_gc_start_incremental_collection", nullptr);
        il2cpp_gc_disable = (void (*)())xdl_sym(handle, "il2cpp_gc_disable", nullptr);
        il2cpp_gc_enable = (void (*)())xdl_sym(handle, "il2cpp_gc_enable", nullptr);
        il2cpp_gc_is_disabled = (bool (*)())xdl_sym(handle, "il2cpp_gc_is_disabled", nullptr);
        il2cpp_gc_set_mode = (void (*)(Il2CppGCMode))xdl_sym(handle, "il2cpp_gc_set_mode", nullptr);
        il2cpp_gc_get_max_time_slice_ns = (int64_t (*)())xdl_sym(handle, "il2cpp_gc_get_max_time_slice_ns", nullptr);
        il2cpp_gc_set_max_time_slice_ns = (void (*)(int64_t))xdl_sym(handle, "il2cpp_gc_set_max_time_slice_ns", nullptr);
        il2cpp_gc_is_incremental = (bool (*)())xdl_sym(handle, "il2cpp_gc_is_incremental", nullptr);
        il2cpp_gc_get_used_size = (int64_t (*)())xdl_sym(handle, "il2cpp_gc_get_used_size", nullptr);
        il2cpp_gc_get_heap_size = (int64_t (*)())xdl_sym(handle, "il2cpp_gc_get_heap_size", nullptr);
        il2cpp_gc_wbarrier_set_field = (void (*)(Il2CppObject*, void**, void*))xdl_sym(handle, "il2cpp_gc_wbarrier_set_field", nullptr);
        il2cpp_gc_has_strict_wbarriers = (bool (*)())xdl_sym(handle, "il2cpp_gc_has_strict_wbarriers", nullptr);
        il2cpp_gc_set_external_allocation_tracker = (void (*)(void (*)(void*, size_t, int)))xdl_sym(handle, "il2cpp_gc_set_external_allocation_tracker", nullptr);
        il2cpp_gc_set_external_wbarrier_tracker = (void (*)(void (*)(void**)))xdl_sym(handle, "il2cpp_gc_set_external_wbarrier_tracker", nullptr);
        il2cpp_gc_foreach_heap = (void (*)(void (*)(void*, void*), void*))xdl_sym(handle, "il2cpp_gc_foreach_heap", nullptr);
        il2cpp_stop_gc_world = (void (*)())xdl_sym(handle, "il2cpp_stop_gc_world", nullptr);
        il2cpp_start_gc_world = (void (*)())xdl_sym(handle, "il2cpp_start_gc_world", nullptr);
        il2cpp_gc_alloc_fixed = (void* (*)(size_t))xdl_sym(handle, "il2cpp_gc_alloc_fixed", nullptr);
        il2cpp_gc_free_fixed = (void (*)(void*))xdl_sym(handle, "il2cpp_gc_free_fixed", nullptr);

        // gchandle
        il2cpp_gchandle_new = (uint32_t (*)(Il2CppObject*, bool))xdl_sym(handle, "il2cpp_gchandle_new", nullptr);
        il2cpp_gchandle_new_weakref = (uint32_t (*)(Il2CppObject*, bool))xdl_sym(handle, "il2cpp_gchandle_new_weakref", nullptr);
        il2cpp_gchandle_get_target = (Il2CppObject* (*)(uint32_t))xdl_sym(handle, "il2cpp_gchandle_get_target", nullptr);
        il2cpp_gchandle_free = (void (*)(uint32_t))xdl_sym(handle, "il2cpp_gchandle_free", nullptr);
        il2cpp_gchandle_foreach_get_target = (void (*)(void (*)(void*, void*), void*))xdl_sym(handle, "il2cpp_gchandle_foreach_get_target", nullptr);

        // vm runtime info
        il2cpp_object_header_size = (uint32_t (*)())xdl_sym(handle, "il2cpp_object_header_size", nullptr);
        il2cpp_array_object_header_size = (uint32_t (*)())xdl_sym(handle, "il2cpp_array_object_header_size", nullptr);
        il2cpp_offset_of_array_length_in_array_object_header = (uint32_t (*)())xdl_sym(handle, "il2cpp_offset_of_array_length_in_array_object_header", nullptr);
        il2cpp_offset_of_array_bounds_in_array_object_header = (uint32_t (*)())xdl_sym(handle, "il2cpp_offset_of_array_bounds_in_array_object_header", nullptr);
        il2cpp_allocation_granularity = (uint32_t (*)())xdl_sym(handle, "il2cpp_allocation_granularity", nullptr);

        // liveness
        il2cpp_unity_liveness_allocate_struct = (void* (*)(Il2CppClass*, int, il2cpp_register_object_callback, void*, il2cpp_liveness_reallocate_callback))xdl_sym(handle, "il2cpp_unity_liveness_allocate_struct", nullptr);
        il2cpp_unity_liveness_calculation_from_root = (void (*)(Il2CppObject*, void*))xdl_sym(handle, "il2cpp_unity_liveness_calculation_from_root", nullptr);
        il2cpp_unity_liveness_calculation_from_statics = (void (*)(void*))xdl_sym(handle, "il2cpp_unity_liveness_calculation_from_statics", nullptr);
        il2cpp_unity_liveness_finalize = (void (*)(void*))xdl_sym(handle, "il2cpp_unity_liveness_finalize", nullptr);
        il2cpp_unity_liveness_free_struct = (void (*)(void*))xdl_sym(handle, "il2cpp_unity_liveness_free_struct", nullptr);

        // method
        il2cpp_method_get_return_type = (const Il2CppType* (*)(const MethodInfo*))xdl_sym(handle, "il2cpp_method_get_return_type", nullptr);
        il2cpp_method_get_declaring_type = (Il2CppClass* (*)(const MethodInfo*))xdl_sym(handle, "il2cpp_method_get_declaring_type", nullptr);
        il2cpp_method_get_name = (const char* (*)(const MethodInfo*))xdl_sym(handle, "il2cpp_method_get_name", nullptr);
        il2cpp_method_get_from_reflection = (const MethodInfo* (*)(const Il2CppReflectionMethod*))xdl_sym(handle, "il2cpp_method_get_from_reflection", nullptr);
        il2cpp_method_get_object = (Il2CppReflectionMethod* (*)(const MethodInfo*, Il2CppClass*))xdl_sym(handle, "il2cpp_method_get_object", nullptr);
        il2cpp_method_is_generic = (bool (*)(const MethodInfo*))xdl_sym(handle, "il2cpp_method_is_generic", nullptr);
        il2cpp_method_is_inflated = (bool (*)(const MethodInfo*))xdl_sym(handle, "il2cpp_method_is_inflated", nullptr);
        il2cpp_method_is_instance = (bool (*)(const MethodInfo*))xdl_sym(handle, "il2cpp_method_is_instance", nullptr);
        il2cpp_method_get_param_count = (uint32_t (*)(const MethodInfo*))xdl_sym(handle, "il2cpp_method_get_param_count", nullptr);
        il2cpp_method_get_param = (const Il2CppType* (*)(const MethodInfo*, uint32_t))xdl_sym(handle, "il2cpp_method_get_param", nullptr);
        il2cpp_method_get_class = (Il2CppClass* (*)(const MethodInfo*))xdl_sym(handle, "il2cpp_method_get_class", nullptr);
        il2cpp_method_has_attribute = (bool (*)(const MethodInfo*, Il2CppClass*))xdl_sym(handle, "il2cpp_method_has_attribute", nullptr);
        il2cpp_method_get_flags = (uint32_t (*)(const MethodInfo*, uint32_t*))xdl_sym(handle, "il2cpp_method_get_flags", nullptr);
        il2cpp_method_get_token = (uint32_t (*)(const MethodInfo*))xdl_sym(handle, "il2cpp_method_get_token", nullptr);
        il2cpp_method_get_param_name = (const char* (*)(const MethodInfo*, uint32_t))xdl_sym(handle, "il2cpp_method_get_param_name", nullptr);

        // profiler
#if IL2CPP_ENABLE_PROFILER
        il2cpp_profiler_install = (void (*)(Il2CppProfiler*, Il2CppProfileFunc))xdl_sym(handle, "il2cpp_profiler_install", nullptr);
        il2cpp_profiler_set_events = (void (*)(Il2CppProfileFlags))xdl_sym(handle, "il2cpp_profiler_set_events", nullptr);
        il2cpp_profiler_install_enter_leave = (void (*)(Il2CppProfileMethodFunc, Il2CppProfileMethodFunc))xdl_sym(handle, "il2cpp_profiler_install_enter_leave", nullptr);
        il2cpp_profiler_install_allocation = (void (*)(Il2CppProfileAllocFunc))xdl_sym(handle, "il2cpp_profiler_install_allocation", nullptr);
        il2cpp_profiler_install_gc = (void (*)(Il2CppProfileGCFunc, Il2CppProfileGCResizeFunc))xdl_sym(handle, "il2cpp_profiler_install_gc", nullptr);
        il2cpp_profiler_install_fileio = (void (*)(Il2CppProfileFileIOFunc))xdl_sym(handle, "il2cpp_profiler_install_fileio", nullptr);
        il2cpp_profiler_install_thread = (void (*)(Il2CppProfileThreadFunc, Il2CppProfileThreadFunc))xdl_sym(handle, "il2cpp_profiler_install_thread", nullptr);
#endif

        // property
        il2cpp_property_get_flags = (uint32_t (*)(PropertyInfo*))xdl_sym(handle, "il2cpp_property_get_flags", nullptr);
        il2cpp_property_get_get_method = (const MethodInfo* (*)(PropertyInfo*))xdl_sym(handle, "il2cpp_property_get_get_method", nullptr);
        il2cpp_property_get_set_method = (const MethodInfo* (*)(PropertyInfo*))xdl_sym(handle, "il2cpp_property_get_set_method", nullptr);
        il2cpp_property_get_name = (const char* (*)(PropertyInfo*))xdl_sym(handle, "il2cpp_property_get_name", nullptr);
        il2cpp_property_get_parent = (Il2CppClass* (*)(PropertyInfo*))xdl_sym(handle, "il2cpp_property_get_parent", nullptr);

        // object
        il2cpp_object_get_class = (Il2CppClass* (*)(Il2CppObject*))xdl_sym(handle, "il2cpp_object_get_class", nullptr);
        il2cpp_object_get_size = (uint32_t (*)(Il2CppObject*))xdl_sym(handle, "il2cpp_object_get_size", nullptr);
        il2cpp_object_get_virtual_method = (const MethodInfo* (*)(Il2CppObject*, const MethodInfo*))xdl_sym(handle, "il2cpp_object_get_virtual_method", nullptr);
        il2cpp_object_new = (Il2CppObject* (*)(const Il2CppClass*))xdl_sym(handle, "il2cpp_object_new", nullptr);
        il2cpp_object_unbox = (void* (*)(Il2CppObject*))xdl_sym(handle, "il2cpp_object_unbox", nullptr);
        il2cpp_value_box = (Il2CppObject* (*)(Il2CppClass*, void*))xdl_sym(handle, "il2cpp_value_box", nullptr);

        // monitor
        il2cpp_monitor_enter = (void (*)(Il2CppObject*))xdl_sym(handle, "il2cpp_monitor_enter", nullptr);
        il2cpp_monitor_try_enter = (bool (*)(Il2CppObject*, uint32_t))xdl_sym(handle, "il2cpp_monitor_try_enter", nullptr);
        il2cpp_monitor_exit = (void (*)(Il2CppObject*))xdl_sym(handle, "il2cpp_monitor_exit", nullptr);
        il2cpp_monitor_pulse = (void (*)(Il2CppObject*))xdl_sym(handle, "il2cpp_monitor_pulse", nullptr);
        il2cpp_monitor_pulse_all = (void (*)(Il2CppObject*))xdl_sym(handle, "il2cpp_monitor_pulse_all", nullptr);
        il2cpp_monitor_wait = (void (*)(Il2CppObject*))xdl_sym(handle, "il2cpp_monitor_wait", nullptr);
        il2cpp_monitor_try_wait = (bool (*)(Il2CppObject*, uint32_t))xdl_sym(handle, "il2cpp_monitor_try_wait", nullptr);

        // runtime
        il2cpp_runtime_invoke = (Il2CppObject* (*)(const MethodInfo*, void*, void**, Il2CppException**))xdl_sym(handle, "il2cpp_runtime_invoke", nullptr);
        il2cpp_runtime_invoke_convert_args = (Il2CppObject* (*)(const MethodInfo*, void*, Il2CppObject**, int, Il2CppException**))xdl_sym(handle, "il2cpp_runtime_invoke_convert_args", nullptr);
        il2cpp_runtime_class_init = (void (*)(Il2CppClass*))xdl_sym(handle, "il2cpp_runtime_class_init", nullptr);
        il2cpp_runtime_object_init = (void (*)(Il2CppObject*))xdl_sym(handle, "il2cpp_runtime_object_init", nullptr);
        il2cpp_runtime_object_init_exception = (void (*)(Il2CppObject*, Il2CppException**))xdl_sym(handle, "il2cpp_runtime_object_init_exception", nullptr);
        il2cpp_runtime_unhandled_exception_policy_set = (void (*)(Il2CppRuntimeUnhandledExceptionPolicy))xdl_sym(handle, "il2cpp_runtime_unhandled_exception_policy_set", nullptr);

        // string
        il2cpp_string_length = (int32_t (*)(Il2CppString*))xdl_sym(handle, "il2cpp_string_length", nullptr);
        il2cpp_string_chars = (Il2CppChar* (*)(Il2CppString*))xdl_sym(handle, "il2cpp_string_chars", nullptr);
        il2cpp_string_new = (Il2CppString* (*)(const char*))xdl_sym(handle, "il2cpp_string_new", nullptr);
        il2cpp_string_new_len = (Il2CppString* (*)(const char*, uint32_t))xdl_sym(handle, "il2cpp_string_new_len", nullptr);
        il2cpp_string_new_utf16 = (Il2CppString* (*)(const Il2CppChar*, int32_t))xdl_sym(handle, "il2cpp_string_new_utf16", nullptr);
        il2cpp_string_new_wrapper = (Il2CppString* (*)(const char*))xdl_sym(handle, "il2cpp_string_new_wrapper", nullptr);
        il2cpp_string_intern = (Il2CppString* (*)(Il2CppString*))xdl_sym(handle, "il2cpp_string_intern", nullptr);
        il2cpp_string_is_interned = (Il2CppString* (*)(Il2CppString*))xdl_sym(handle, "il2cpp_string_is_interned", nullptr);

        // thread
        il2cpp_thread_current = (Il2CppThread* (*)())xdl_sym(handle, "il2cpp_thread_current", nullptr);
        il2cpp_thread_attach = (Il2CppThread* (*)(Il2CppDomain*))xdl_sym(handle, "il2cpp_thread_attach", nullptr);
        il2cpp_thread_detach = (void (*)(Il2CppThread*))xdl_sym(handle, "il2cpp_thread_detach", nullptr);
        il2cpp_thread_get_all_attached_threads = (Il2CppThread** (*)(size_t*))xdl_sym(handle, "il2cpp_thread_get_all_attached_threads", nullptr);
        il2cpp_is_vm_thread = (bool (*)(Il2CppThread*))xdl_sym(handle, "il2cpp_is_vm_thread", nullptr);

        // stacktrace
        il2cpp_current_thread_walk_frame_stack = (void (*)(Il2CppFrameWalkFunc, void*))xdl_sym(handle, "il2cpp_current_thread_walk_frame_stack", nullptr);
        il2cpp_thread_walk_frame_stack = (void (*)(Il2CppThread*, Il2CppFrameWalkFunc, void*))xdl_sym(handle, "il2cpp_thread_walk_frame_stack", nullptr);
        il2cpp_current_thread_get_top_frame = (bool (*)(Il2CppStackFrameInfo*))xdl_sym(handle, "il2cpp_current_thread_get_top_frame", nullptr);
        il2cpp_thread_get_top_frame = (bool (*)(Il2CppThread*, Il2CppStackFrameInfo*))xdl_sym(handle, "il2cpp_thread_get_top_frame", nullptr);
        il2cpp_current_thread_get_frame_at = (bool (*)(int32_t, Il2CppStackFrameInfo*))xdl_sym(handle, "il2cpp_current_thread_get_frame_at", nullptr);
        il2cpp_thread_get_frame_at = (bool (*)(Il2CppThread*, int32_t, Il2CppStackFrameInfo*))xdl_sym(handle, "il2cpp_thread_get_frame_at", nullptr);
        il2cpp_current_thread_get_stack_depth = (int32_t (*)())xdl_sym(handle, "il2cpp_current_thread_get_stack_depth", nullptr);
        il2cpp_thread_get_stack_depth = (int32_t (*)(Il2CppThread*))xdl_sym(handle, "il2cpp_thread_get_stack_depth", nullptr);
        il2cpp_override_stack_backtrace = (void (*)(Il2CppBacktraceFunc))xdl_sym(handle, "il2cpp_override_stack_backtrace", nullptr);

        // type
        il2cpp_type_get_object = (Il2CppObject* (*)(const Il2CppType*))xdl_sym(handle, "il2cpp_type_get_object", nullptr);
        il2cpp_type_get_type = (int (*)(const Il2CppType*))xdl_sym(handle, "il2cpp_type_get_type", nullptr);
        il2cpp_type_get_class_or_element_class = (Il2CppClass* (*)(const Il2CppType*))xdl_sym(handle, "il2cpp_type_get_class_or_element_class", nullptr);
        il2cpp_type_get_name = (char* (*)(const Il2CppType*))xdl_sym(handle, "il2cpp_type_get_name", nullptr);
        il2cpp_type_is_byref = (bool (*)(const Il2CppType*))xdl_sym(handle, "il2cpp_type_is_byref", nullptr);
        il2cpp_type_get_attrs = (uint32_t (*)(const Il2CppType*))xdl_sym(handle, "il2cpp_type_get_attrs", nullptr);
        il2cpp_type_equals = (bool (*)(const Il2CppType*, const Il2CppType*))xdl_sym(handle, "il2cpp_type_equals", nullptr);
        il2cpp_type_get_assembly_qualified_name = (char* (*)(const Il2CppType*))xdl_sym(handle, "il2cpp_type_get_assembly_qualified_name", nullptr);
        il2cpp_type_is_static = (bool (*)(const Il2CppType*))xdl_sym(handle, "il2cpp_type_is_static", nullptr);
        il2cpp_type_is_pointer_type = (bool (*)(const Il2CppType*))xdl_sym(handle, "il2cpp_type_is_pointer_type", nullptr);

        // image
        il2cpp_image_get_assembly = (const Il2CppAssembly* (*)(const Il2CppImage*))xdl_sym(handle, "il2cpp_image_get_assembly", nullptr);
        il2cpp_image_get_name = (const char* (*)(const Il2CppImage*))xdl_sym(handle, "il2cpp_image_get_name", nullptr);
        il2cpp_image_get_filename = (const char* (*)(const Il2CppImage*))xdl_sym(handle, "il2cpp_image_get_filename", nullptr);
        il2cpp_image_get_entry_point = (const MethodInfo* (*)(const Il2CppImage*))xdl_sym(handle, "il2cpp_image_get_entry_point", nullptr);
        il2cpp_image_get_class_count = (size_t (*)(const Il2CppImage*))xdl_sym(handle, "il2cpp_image_get_class_count", nullptr);
        il2cpp_image_get_class = (const Il2CppClass* (*)(const Il2CppImage*, size_t))xdl_sym(handle, "il2cpp_image_get_class", nullptr);

        // Memory information
        il2cpp_capture_memory_snapshot = (Il2CppManagedMemorySnapshot* (*)())xdl_sym(handle, "il2cpp_capture_memory_snapshot", nullptr);
        il2cpp_free_captured_memory_snapshot = (void (*)(Il2CppManagedMemorySnapshot*))xdl_sym(handle, "il2cpp_free_captured_memory_snapshot", nullptr);

        // Plugin callback
        il2cpp_set_find_plugin_callback = (void (*)(Il2CppSetFindPlugInCallback))xdl_sym(handle, "il2cpp_set_find_plugin_callback", nullptr);

        // Logging
        il2cpp_register_log_callback = (void (*)(Il2CppLogCallback))xdl_sym(handle, "il2cpp_register_log_callback", nullptr);

        // Debugger
        il2cpp_debugger_set_agent_options = (void (*)(const char*))xdl_sym(handle, "il2cpp_debugger_set_agent_options", nullptr);
        il2cpp_is_debugger_attached = (bool (*)())xdl_sym(handle, "il2cpp_is_debugger_attached", nullptr);
        il2cpp_register_debugger_agent_transport = (void (*)(Il2CppDebuggerTransport*))xdl_sym(handle, "il2cpp_register_debugger_agent_transport", nullptr);

        // Debug metadata
        il2cpp_debug_get_method_info = (bool (*)(const MethodInfo*, Il2CppMethodDebugInfo*))xdl_sym(handle, "il2cpp_debug_get_method_info", nullptr);

        // TLS module
        il2cpp_unity_install_unitytls_interface = (void (*)(const void*))xdl_sym(handle, "il2cpp_unity_install_unitytls_interface", nullptr);

        // custom attributes
        il2cpp_custom_attrs_from_class = (Il2CppCustomAttrInfo* (*)(Il2CppClass*))xdl_sym(handle, "il2cpp_custom_attrs_from_class", nullptr);
        il2cpp_custom_attrs_from_method = (Il2CppCustomAttrInfo* (*)(const MethodInfo*))xdl_sym(handle, "il2cpp_custom_attrs_from_method", nullptr);
        il2cpp_custom_attrs_get_attr = (Il2CppObject* (*)(Il2CppCustomAttrInfo*, Il2CppClass*))xdl_sym(handle, "il2cpp_custom_attrs_get_attr", nullptr);
        il2cpp_custom_attrs_has_attr = (bool (*)(Il2CppCustomAttrInfo*, Il2CppClass*))xdl_sym(handle, "il2cpp_custom_attrs_has_attr", nullptr);
        il2cpp_custom_attrs_construct = (Il2CppArray* (*)(Il2CppCustomAttrInfo*))xdl_sym(handle, "il2cpp_custom_attrs_construct", nullptr);
        il2cpp_custom_attrs_free = (void (*)(Il2CppCustomAttrInfo*))xdl_sym(handle, "il2cpp_custom_attrs_free", nullptr);

        // Il2CppClass user data for GetComponent optimization
        il2cpp_class_set_userdata = (void (*)(Il2CppClass*, void*))xdl_sym(handle, "il2cpp_class_set_userdata", nullptr);
        il2cpp_class_get_userdata_offset = (int (*)())xdl_sym(handle, "il2cpp_class_get_userdata_offset", nullptr);

        // Thread affinity
        il2cpp_set_default_thread_affinity = (void (*)(int64_t))xdl_sym(handle, "il2cpp_set_default_thread_affinity", nullptr);
    } else {
        // initialization and runtime setup
        il2cpp_init = (int (*)(const char*))(absoluteStart + 0x0);
        il2cpp_init_utf16 = (int (*)(const Il2CppChar*))(absoluteStart + 0x0);
        il2cpp_shutdown = (void (*)())(absoluteStart + 0x0);
        il2cpp_set_config_dir = (void (*)(const char*))(absoluteStart + 0x0);
        il2cpp_set_data_dir = (void (*)(const char*))(absoluteStart + 0x0);
        il2cpp_set_temp_dir = (void (*)(const char*))(absoluteStart + 0x0);
        il2cpp_set_commandline_arguments = (void (*)(int, const char* const[], const char*))(absoluteStart + 0x0);
        il2cpp_set_commandline_arguments_utf16 = (void (*)(int, const Il2CppChar* const[], const char*))(absoluteStart + 0x0);
        il2cpp_set_config_utf16 = (void (*)(const Il2CppChar*))(absoluteStart + 0x0);
        il2cpp_set_config = (void (*)(const char*))(absoluteStart + 0x0);

        il2cpp_set_memory_callbacks = (void (*)(Il2CppMemoryCallbacks*))(absoluteStart + 0x0);
        il2cpp_get_corlib = (const Il2CppImage* (*)())(absoluteStart + 0x12345);
        il2cpp_add_internal_call = (void (*)(const char*, Il2CppMethodPointer))(absoluteStart + 0x0);
        il2cpp_resolve_icall = (Il2CppMethodPointer (*)(const char*))(absoluteStart + 0x0);

        // memory allocation
        il2cpp_alloc = (void* (*)(size_t))(absoluteStart + 0x0);
        il2cpp_free = (void (*)(void*))(absoluteStart + 0x0);

        // array
        il2cpp_array_class_get = (Il2CppClass* (*)(Il2CppClass*, uint32_t))(absoluteStart + 0x0);
        il2cpp_array_length = (uint32_t (*)(Il2CppArray*))(absoluteStart + 0x0);
        il2cpp_array_get_byte_length = (uint32_t (*)(Il2CppArray*))(absoluteStart + 0x0);
        il2cpp_array_new = (Il2CppArray* (*)(Il2CppClass*, il2cpp_array_size_t))(absoluteStart + 0x0);
        il2cpp_array_new_specific = (Il2CppArray* (*)(Il2CppClass*, il2cpp_array_size_t))(absoluteStart + 0x0);
        il2cpp_array_new_full = (Il2CppArray* (*)(Il2CppClass*, il2cpp_array_size_t*, il2cpp_array_size_t*))(absoluteStart + 0x0);
        il2cpp_bounded_array_class_get = (Il2CppClass* (*)(Il2CppClass*, uint32_t, bool))(absoluteStart + 0x0);
        il2cpp_array_element_size = (int (*)(const Il2CppClass*))(absoluteStart + 0x0);

        // assembly
        il2cpp_assembly_get_image = (const Il2CppImage* (*)(const Il2CppAssembly*))(absoluteStart + 0x12345);

        // class
        il2cpp_class_for_each = (void (*)(void (*)(Il2CppClass*, void*), void*))(absoluteStart + 0x0);
        il2cpp_class_enum_basetype = (const Il2CppType* (*)(Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_is_generic = (bool (*)(const Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_is_inflated = (bool (*)(const Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_is_assignable_from = (bool (*)(Il2CppClass*, Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_is_subclass_of = (bool (*)(Il2CppClass*, Il2CppClass*, bool))(absoluteStart + 0x0);
        il2cpp_class_has_parent = (bool (*)(Il2CppClass*, Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_from_il2cpp_type = (Il2CppClass* (*)(const Il2CppType*))(absoluteStart + 0x0);
        il2cpp_class_from_name = (Il2CppClass* (*)(const Il2CppImage*, const char*, const char*))(absoluteStart + 0x12345);
        il2cpp_class_from_system_type = (Il2CppClass* (*)(Il2CppReflectionType*))(absoluteStart + 0x12345);
        il2cpp_class_get_element_class = (Il2CppClass* (*)(Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_get_events = (const EventInfo* (*)(Il2CppClass*, void**))(absoluteStart + 0x0);
        il2cpp_class_get_fields = (FieldInfo* (*)(Il2CppClass*, void**))(absoluteStart + 0x12345);
        il2cpp_class_get_nested_types = (Il2CppClass* (*)(Il2CppClass*, void**))(absoluteStart + 0x0);
        il2cpp_class_get_interfaces = (Il2CppClass* (*)(Il2CppClass*, void**))(absoluteStart + 0x12345);
        il2cpp_class_get_properties = (const PropertyInfo* (*)(Il2CppClass*, void**))(absoluteStart + 0x12345);
        il2cpp_class_get_property_from_name = (const PropertyInfo* (*)(Il2CppClass*, const char*))(absoluteStart + 0x0);
        il2cpp_class_get_field_from_name = (FieldInfo* (*)(Il2CppClass*, const char*))(absoluteStart + 0x0);
        il2cpp_class_get_methods = (const MethodInfo* (*)(Il2CppClass*, void**))(absoluteStart + 0x12345);
        il2cpp_class_get_method_from_name = (const MethodInfo* (*)(Il2CppClass*, const char*, int))(absoluteStart + 0x12345);
        il2cpp_class_get_name = (const char* (*)(Il2CppClass*))(absoluteStart + 0x12345);
        il2cpp_type_get_name_chunked = (void (*)(const Il2CppType*, void (*)(void*, void*), void*))(absoluteStart + 0x0);
        il2cpp_class_get_namespace = (const char* (*)(Il2CppClass*))(absoluteStart + 0x12345);
        il2cpp_class_get_parent = (Il2CppClass* (*)(Il2CppClass*))(absoluteStart + 0x12345);
        il2cpp_class_get_declaring_type = (Il2CppClass* (*)(Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_instance_size = (int32_t (*)(Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_num_fields = (size_t (*)(const Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_is_valuetype = (bool (*)(const Il2CppClass*))(absoluteStart + 0x12345);
        il2cpp_class_value_size = (int32_t (*)(Il2CppClass*, uint32_t*))(absoluteStart + 0x0);
        il2cpp_class_is_blittable = (bool (*)(const Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_get_flags = (int (*)(const Il2CppClass*))(absoluteStart + 0x12345);
        il2cpp_class_is_abstract = (bool (*)(const Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_is_interface = (bool (*)(const Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_array_element_size = (int (*)(const Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_from_type = (Il2CppClass* (*)(const Il2CppType*))(absoluteStart + 0x12345);
        il2cpp_class_get_type = (const Il2CppType* (*)(Il2CppClass*))(absoluteStart + 0x12345);
        il2cpp_class_get_type_token = (uint32_t (*)(Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_has_attribute = (bool (*)(Il2CppClass*, Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_has_references = (bool (*)(Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_is_enum = (bool (*)(const Il2CppClass*))(absoluteStart + 0x12345);
        il2cpp_class_get_image = (const Il2CppImage* (*)(Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_get_assemblyname = (const char* (*)(const Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_get_rank = (int (*)(const Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_get_data_size = (uint32_t (*)(const Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_get_static_field_data = (void* (*)(const Il2CppClass*))(absoluteStart + 0x0);

        // testing only
        il2cpp_class_get_bitmap_size = (size_t (*)(const Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_class_get_bitmap = (void (*)(Il2CppClass*, size_t*))(absoluteStart + 0x0);

        // stats
        il2cpp_stats_dump_to_file = (bool (*)(const char*))(absoluteStart + 0x0);
        il2cpp_stats_get_value = (uint64_t (*)(Il2CppStat))(absoluteStart + 0x0);

        // domain
        il2cpp_domain_get = (Il2CppDomain* (*)())(absoluteStart + 0x12345);
        il2cpp_domain_assembly_open = (const Il2CppAssembly* (*)(Il2CppDomain*, const char*))(absoluteStart + 0x0);
        il2cpp_domain_get_assemblies = (const Il2CppAssembly** (*)(const Il2CppDomain*, size_t*))(absoluteStart + 0x12345);

        // exception
        il2cpp_raise_exception = (void (*)(Il2CppException*))(absoluteStart + 0x0);
        il2cpp_exception_from_name_msg = (Il2CppException* (*)(const Il2CppImage*, const char*, const char*, const char*))(absoluteStart + 0x0);
        il2cpp_get_exception_argument_null = (Il2CppException* (*)(const char*))(absoluteStart + 0x0);
        il2cpp_format_exception = (void (*)(const Il2CppException*, char*, int))(absoluteStart + 0x0);
        il2cpp_format_stack_trace = (void (*)(const Il2CppException*, char*, int))(absoluteStart + 0x0);
        il2cpp_unhandled_exception = (void (*)(Il2CppException*))(absoluteStart + 0x0);
        il2cpp_native_stack_trace = (void (*)(const Il2CppException*, uintptr_t**, int*, char**, char**))(absoluteStart + 0x0);

        // field
        il2cpp_field_get_flags = (int (*)(FieldInfo*))(absoluteStart + 0x12345);
        il2cpp_field_get_name = (const char* (*)(FieldInfo*))(absoluteStart + 0x12345);
        il2cpp_field_get_parent = (Il2CppClass* (*)(FieldInfo*))(absoluteStart + 0x0);
        il2cpp_field_get_offset = (size_t (*)(FieldInfo*))(absoluteStart + 0x12345);
        il2cpp_field_get_type = (const Il2CppType* (*)(FieldInfo*))(absoluteStart + 0x12345);
        il2cpp_field_get_value = (void (*)(Il2CppObject*, FieldInfo*, void*))(absoluteStart + 0x0);
        il2cpp_field_get_value_object = (Il2CppObject* (*)(FieldInfo*, Il2CppObject*))(absoluteStart + 0x0);
        il2cpp_field_has_attribute = (bool (*)(FieldInfo*, Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_field_set_value = (void (*)(Il2CppObject*, FieldInfo*, void*))(absoluteStart + 0x0);
        il2cpp_field_static_get_value = (void (*)(FieldInfo*, void*))(absoluteStart + 0x12345);
        il2cpp_field_static_set_value = (void (*)(FieldInfo*, void*))(absoluteStart + 0x0);
        il2cpp_field_set_value_object = (void (*)(Il2CppObject*, FieldInfo*, Il2CppObject*))(absoluteStart + 0x0);
        il2cpp_field_is_literal = (bool (*)(FieldInfo*))(absoluteStart + 0x0);

        // gc
        il2cpp_gc_collect = (void (*)(int))(absoluteStart + 0x0);
        il2cpp_gc_collect_a_little = (int32_t (*)())(absoluteStart + 0x0);
        il2cpp_gc_start_incremental_collection = (void (*)())(absoluteStart + 0x0);
        il2cpp_gc_disable = (void (*)())(absoluteStart + 0x0);
        il2cpp_gc_enable = (void (*)())(absoluteStart + 0x0);
        il2cpp_gc_is_disabled = (bool (*)())(absoluteStart + 0x0);
        il2cpp_gc_set_mode = (void (*)(Il2CppGCMode))(absoluteStart + 0x0);
        il2cpp_gc_get_max_time_slice_ns = (int64_t (*)())(absoluteStart + 0x0);
        il2cpp_gc_set_max_time_slice_ns = (void (*)(int64_t))(absoluteStart + 0x0);
        il2cpp_gc_is_incremental = (bool (*)())(absoluteStart + 0x0);
        il2cpp_gc_get_used_size = (int64_t (*)())(absoluteStart + 0x0);
        il2cpp_gc_get_heap_size = (int64_t (*)())(absoluteStart + 0x0);
        il2cpp_gc_wbarrier_set_field = (void (*)(Il2CppObject*, void**, void*))(absoluteStart + 0x0);
        il2cpp_gc_has_strict_wbarriers = (bool (*)())(absoluteStart + 0x0);
        il2cpp_gc_set_external_allocation_tracker = (void (*)(void (*)(void*, size_t, int)))(absoluteStart + 0x0);
        il2cpp_gc_set_external_wbarrier_tracker = (void (*)(void (*)(void**)))(absoluteStart + 0x0);
        il2cpp_gc_foreach_heap = (void (*)(void (*)(void*, void*), void*))(absoluteStart + 0x0);
        il2cpp_stop_gc_world = (void (*)())(absoluteStart + 0x0);
        il2cpp_start_gc_world = (void (*)())(absoluteStart + 0x0);
        il2cpp_gc_alloc_fixed = (void* (*)(size_t))(absoluteStart + 0x0);
        il2cpp_gc_free_fixed = (void (*)(void*))(absoluteStart + 0x0);

        // gchandle
        il2cpp_gchandle_new = (uint32_t (*)(Il2CppObject*, bool))(absoluteStart + 0x0);
        il2cpp_gchandle_new_weakref = (uint32_t (*)(Il2CppObject*, bool))(absoluteStart + 0x0);
        il2cpp_gchandle_get_target = (Il2CppObject* (*)(uint32_t))(absoluteStart + 0x0);
        il2cpp_gchandle_free = (void (*)(uint32_t))(absoluteStart + 0x0);
        il2cpp_gchandle_foreach_get_target = (void (*)(void (*)(void*, void*), void*))(absoluteStart + 0x0);

        // vm runtime info
        il2cpp_object_header_size = (uint32_t (*)())(absoluteStart + 0x0);
        il2cpp_array_object_header_size = (uint32_t (*)())(absoluteStart + 0x0);
        il2cpp_offset_of_array_length_in_array_object_header = (uint32_t (*)())(absoluteStart + 0x0);
        il2cpp_offset_of_array_bounds_in_array_object_header = (uint32_t (*)())(absoluteStart + 0x0);
        il2cpp_allocation_granularity = (uint32_t (*)())(absoluteStart + 0x0);

        // liveness
        il2cpp_unity_liveness_allocate_struct = (void* (*)(Il2CppClass*, int, il2cpp_register_object_callback, void*, il2cpp_liveness_reallocate_callback))(absoluteStart + 0x0);
        il2cpp_unity_liveness_calculation_from_root = (void (*)(Il2CppObject*, void*))(absoluteStart + 0x0);
        il2cpp_unity_liveness_calculation_from_statics = (void (*)(void*))(absoluteStart + 0x0);
        il2cpp_unity_liveness_finalize = (void (*)(void*))(absoluteStart + 0x0);
        il2cpp_unity_liveness_free_struct = (void (*)(void*))(absoluteStart + 0x0);

        // method
        il2cpp_method_get_return_type = (const Il2CppType* (*)(const MethodInfo*))(absoluteStart + 0x12345);
        il2cpp_method_get_declaring_type = (Il2CppClass* (*)(const MethodInfo*))(absoluteStart + 0x0);
        il2cpp_method_get_name = (const char* (*)(const MethodInfo*))(absoluteStart + 0x12345);
        il2cpp_method_get_from_reflection = (const MethodInfo* (*)(const Il2CppReflectionMethod*))(absoluteStart + 0x0);
        il2cpp_method_get_object = (Il2CppReflectionMethod* (*)(const MethodInfo*, Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_method_is_generic = (bool (*)(const MethodInfo*))(absoluteStart + 0x0);
        il2cpp_method_is_inflated = (bool (*)(const MethodInfo*))(absoluteStart + 0x0);
        il2cpp_method_is_instance = (bool (*)(const MethodInfo*))(absoluteStart + 0x0);
        il2cpp_method_get_param_count = (uint32_t (*)(const MethodInfo*))(absoluteStart + 0x12345);
        il2cpp_method_get_param = (const Il2CppType* (*)(const MethodInfo*, uint32_t))(absoluteStart + 0x12345);
        il2cpp_method_get_class = (Il2CppClass* (*)(const MethodInfo*))(absoluteStart + 0x0);
        il2cpp_method_has_attribute = (bool (*)(const MethodInfo*, Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_method_get_flags = (uint32_t (*)(const MethodInfo*, uint32_t*))(absoluteStart + 0x12345);
        il2cpp_method_get_token = (uint32_t (*)(const MethodInfo*))(absoluteStart + 0x0);
        il2cpp_method_get_param_name = (const char* (*)(const MethodInfo*, uint32_t))(absoluteStart + 0x12345);

        // profiler
#if IL2CPP_ENABLE_PROFILER
        il2cpp_profiler_install = (void (*)(Il2CppProfiler*, Il2CppProfileFunc))(absoluteStart + 0x0);
        il2cpp_profiler_set_events = (void (*)(Il2CppProfileFlags))(absoluteStart + 0x0);
        il2cpp_profiler_install_enter_leave = (void (*)(Il2CppProfileMethodFunc, Il2CppProfileMethodFunc))(absoluteStart + 0x0);
        il2cpp_profiler_install_allocation = (void (*)(Il2CppProfileAllocFunc))(absoluteStart + 0x0);
        il2cpp_profiler_install_gc = (void (*)(Il2CppProfileGCFunc, Il2CppProfileGCResizeFunc))(absoluteStart + 0x0);
        il2cpp_profiler_install_fileio = (void (*)(Il2CppProfileFileIOFunc))(absoluteStart + 0x0);
        il2cpp_profiler_install_thread = (void (*)(Il2CppProfileThreadFunc, Il2CppProfileThreadFunc))(absoluteStart + 0x0);
#endif

        // property
        il2cpp_property_get_flags = (uint32_t (*)(PropertyInfo*))(absoluteStart + 0x0);
        il2cpp_property_get_get_method = (const MethodInfo* (*)(PropertyInfo*))(absoluteStart + 0x12345);
        il2cpp_property_get_set_method = (const MethodInfo* (*)(PropertyInfo*))(absoluteStart + 0x12345);
        il2cpp_property_get_name = (const char* (*)(PropertyInfo*))(absoluteStart + 0x12345);
        il2cpp_property_get_parent = (Il2CppClass* (*)(PropertyInfo*))(absoluteStart + 0x0);

        // object
        il2cpp_object_get_class = (Il2CppClass* (*)(Il2CppObject*))(absoluteStart + 0x0);
        il2cpp_object_get_size = (uint32_t (*)(Il2CppObject*))(absoluteStart + 0x0);
        il2cpp_object_get_virtual_method = (const MethodInfo* (*)(Il2CppObject*, const MethodInfo*))(absoluteStart + 0x0);
        il2cpp_object_new = (Il2CppObject* (*)(const Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_object_unbox = (void* (*)(Il2CppObject*))(absoluteStart + 0x0);
        il2cpp_value_box = (Il2CppObject* (*)(Il2CppClass*, void*))(absoluteStart + 0x0);

        // monitor
        il2cpp_monitor_enter = (void (*)(Il2CppObject*))(absoluteStart + 0x0);
        il2cpp_monitor_try_enter = (bool (*)(Il2CppObject*, uint32_t))(absoluteStart + 0x0);
        il2cpp_monitor_exit = (void (*)(Il2CppObject*))(absoluteStart + 0x0);
        il2cpp_monitor_pulse = (void (*)(Il2CppObject*))(absoluteStart + 0x0);
        il2cpp_monitor_pulse_all = (void (*)(Il2CppObject*))(absoluteStart + 0x0);
        il2cpp_monitor_wait = (void (*)(Il2CppObject*))(absoluteStart + 0x0);
        il2cpp_monitor_try_wait = (bool (*)(Il2CppObject*, uint32_t))(absoluteStart + 0x0);

        // runtime
        il2cpp_runtime_invoke = (Il2CppObject* (*)(const MethodInfo*, void*, void**, Il2CppException**))(absoluteStart + 0x0);
        il2cpp_runtime_invoke_convert_args = (Il2CppObject* (*)(const MethodInfo*, void*, Il2CppObject**, int, Il2CppException**))(absoluteStart + 0x0);
        il2cpp_runtime_class_init = (void (*)(Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_runtime_object_init = (void (*)(Il2CppObject*))(absoluteStart + 0x0);
        il2cpp_runtime_object_init_exception = (void (*)(Il2CppObject*, Il2CppException**))(absoluteStart + 0x0);
        il2cpp_runtime_unhandled_exception_policy_set = (void (*)(Il2CppRuntimeUnhandledExceptionPolicy))(absoluteStart + 0x0);

        // string
        il2cpp_string_length = (int32_t (*)(Il2CppString*))(absoluteStart + 0x0);
        il2cpp_string_chars = (Il2CppChar* (*)(Il2CppString*))(absoluteStart + 0x0);
        il2cpp_string_new = (Il2CppString* (*)(const char*))(absoluteStart + 0x12345);
        il2cpp_string_new_len = (Il2CppString* (*)(const char*, uint32_t))(absoluteStart + 0x0);
        il2cpp_string_new_utf16 = (Il2CppString* (*)(const Il2CppChar*, int32_t))(absoluteStart + 0x0);
        il2cpp_string_new_wrapper = (Il2CppString* (*)(const char*))(absoluteStart + 0x0);
        il2cpp_string_intern = (Il2CppString* (*)(Il2CppString*))(absoluteStart + 0x0);
        il2cpp_string_is_interned = (Il2CppString* (*)(Il2CppString*))(absoluteStart + 0x0);

        // thread
        il2cpp_thread_current = (Il2CppThread* (*)())(absoluteStart + 0x0);
        il2cpp_thread_attach = (Il2CppThread* (*)(Il2CppDomain*))(absoluteStart + 0x12345);
        il2cpp_thread_detach = (void (*)(Il2CppThread*))(absoluteStart + 0x0);
        il2cpp_thread_get_all_attached_threads = (Il2CppThread** (*)(size_t*))(absoluteStart + 0x0);
        il2cpp_is_vm_thread = (bool (*)(Il2CppThread*))(absoluteStart + 0x12345);

        // stacktrace
        il2cpp_current_thread_walk_frame_stack = (void (*)(Il2CppFrameWalkFunc, void*))(absoluteStart + 0x0);
        il2cpp_thread_walk_frame_stack = (void (*)(Il2CppThread*, Il2CppFrameWalkFunc, void*))(absoluteStart + 0x0);
        il2cpp_current_thread_get_top_frame = (bool (*)(Il2CppStackFrameInfo*))(absoluteStart + 0x0);
        il2cpp_thread_get_top_frame = (bool (*)(Il2CppThread*, Il2CppStackFrameInfo*))(absoluteStart + 0x0);
        il2cpp_current_thread_get_frame_at = (bool (*)(int32_t, Il2CppStackFrameInfo*))(absoluteStart + 0x0);
        il2cpp_thread_get_frame_at = (bool (*)(Il2CppThread*, int32_t, Il2CppStackFrameInfo*))(absoluteStart + 0x0);
        il2cpp_current_thread_get_stack_depth = (int32_t (*)())(absoluteStart + 0x0);
        il2cpp_thread_get_stack_depth = (int32_t (*)(Il2CppThread*))(absoluteStart + 0x0);
        il2cpp_override_stack_backtrace = (void (*)(Il2CppBacktraceFunc))(absoluteStart + 0x0);

        // type
        il2cpp_type_get_object = (Il2CppObject* (*)(const Il2CppType*))(absoluteStart + 0x0);
        il2cpp_type_get_type = (int (*)(const Il2CppType*))(absoluteStart + 0x0);
        il2cpp_type_get_class_or_element_class = (Il2CppClass* (*)(const Il2CppType*))(absoluteStart + 0x0);
        il2cpp_type_get_name = (char* (*)(const Il2CppType*))(absoluteStart + 0x0);
        il2cpp_type_is_byref = (bool (*)(const Il2CppType*))(absoluteStart + 0x12345);
        il2cpp_type_get_attrs = (uint32_t (*)(const Il2CppType*))(absoluteStart + 0x0);
        il2cpp_type_equals = (bool (*)(const Il2CppType*, const Il2CppType*))(absoluteStart + 0x0);
        il2cpp_type_get_assembly_qualified_name = (char* (*)(const Il2CppType*))(absoluteStart + 0x0);
        il2cpp_type_is_static = (bool (*)(const Il2CppType*))(absoluteStart + 0x0);
        il2cpp_type_is_pointer_type = (bool (*)(const Il2CppType*))(absoluteStart + 0x0);

        // image
        il2cpp_image_get_assembly = (const Il2CppAssembly* (*)(const Il2CppImage*))(absoluteStart + 0x0);
        il2cpp_image_get_name = (const char* (*)(const Il2CppImage*))(absoluteStart + 0x12345);
        il2cpp_image_get_filename = (const char* (*)(const Il2CppImage*))(absoluteStart + 0x0);
        il2cpp_image_get_entry_point = (const MethodInfo* (*)(const Il2CppImage*))(absoluteStart + 0x0);
        il2cpp_image_get_class_count = (size_t (*)(const Il2CppImage*))(absoluteStart + 0x12345);
        il2cpp_image_get_class = (const Il2CppClass* (*)(const Il2CppImage*, size_t))(absoluteStart + 0x12345);

        // Memory information
        il2cpp_capture_memory_snapshot = (Il2CppManagedMemorySnapshot* (*)())(absoluteStart + 0x0);
        il2cpp_free_captured_memory_snapshot = (void (*)(Il2CppManagedMemorySnapshot*))(absoluteStart + 0x0);

        // Plugin callback
        il2cpp_set_find_plugin_callback = (void (*)(Il2CppSetFindPlugInCallback))(absoluteStart + 0x0);

        // Logging
        il2cpp_register_log_callback = (void (*)(Il2CppLogCallback))(absoluteStart + 0x0);

        // Debugger
        il2cpp_debugger_set_agent_options = (void (*)(const char*))(absoluteStart + 0x0);
        il2cpp_is_debugger_attached = (bool (*)())(absoluteStart + 0x0);
        il2cpp_register_debugger_agent_transport = (void (*)(Il2CppDebuggerTransport*))(absoluteStart + 0x0);

        // Debug metadata
        il2cpp_debug_get_method_info = (bool (*)(const MethodInfo*, Il2CppMethodDebugInfo*))(absoluteStart + 0x0);

        // TLS module
        il2cpp_unity_install_unitytls_interface = (void (*)(const void*))(absoluteStart + 0x0);

        // custom attributes
        il2cpp_custom_attrs_from_class = (Il2CppCustomAttrInfo* (*)(Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_custom_attrs_from_method = (Il2CppCustomAttrInfo* (*)(const MethodInfo*))(absoluteStart + 0x0);
        il2cpp_custom_attrs_get_attr = (Il2CppObject* (*)(Il2CppCustomAttrInfo*, Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_custom_attrs_has_attr = (bool (*)(Il2CppCustomAttrInfo*, Il2CppClass*))(absoluteStart + 0x0);
        il2cpp_custom_attrs_construct = (Il2CppArray* (*)(Il2CppCustomAttrInfo*))(absoluteStart + 0x0);
        il2cpp_custom_attrs_free = (void (*)(Il2CppCustomAttrInfo*))(absoluteStart + 0x0);

        // Il2CppClass user data for GetComponent optimization
        il2cpp_class_set_userdata = (void (*)(Il2CppClass*, void*))(absoluteStart + 0x0);
        il2cpp_class_get_userdata_offset = (int (*)())(absoluteStart + 0x0);

        // Thread affinity
        il2cpp_set_default_thread_affinity = (void (*)(int64_t))(absoluteStart + 0x0);
    }
}