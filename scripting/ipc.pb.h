// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ipc.proto

#ifndef PROTOBUF_ipc_2eproto__INCLUDED
#define PROTOBUF_ipc_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace ipc {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_ipc_2eproto();
void protobuf_AssignDesc_ipc_2eproto();
void protobuf_ShutdownFile_ipc_2eproto();

class IPCMessage;
class IPCMessage_MessageItem;

enum IPCMessage_MessageType {
  IPCMessage_MessageType_GET_SCENEINFO = 0,
  IPCMessage_MessageType_SET_SCENEINFO = 1,
  IPCMessage_MessageType_LOG_DEBUG = 2,
  IPCMessage_MessageType_LOG_ERROR = 3,
  IPCMessage_MessageType_IPCMessage_MessageType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  IPCMessage_MessageType_IPCMessage_MessageType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool IPCMessage_MessageType_IsValid(int value);
const IPCMessage_MessageType IPCMessage_MessageType_MessageType_MIN = IPCMessage_MessageType_GET_SCENEINFO;
const IPCMessage_MessageType IPCMessage_MessageType_MessageType_MAX = IPCMessage_MessageType_LOG_ERROR;
const int IPCMessage_MessageType_MessageType_ARRAYSIZE = IPCMessage_MessageType_MessageType_MAX + 1;

const ::google::protobuf::EnumDescriptor* IPCMessage_MessageType_descriptor();
inline const ::std::string& IPCMessage_MessageType_Name(IPCMessage_MessageType value) {
  return ::google::protobuf::internal::NameOfEnum(
    IPCMessage_MessageType_descriptor(), value);
}
inline bool IPCMessage_MessageType_Parse(
    const ::std::string& name, IPCMessage_MessageType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<IPCMessage_MessageType>(
    IPCMessage_MessageType_descriptor(), name, value);
}
// ===================================================================

class IPCMessage_MessageItem : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:ipc.IPCMessage.MessageItem) */ {
 public:
  IPCMessage_MessageItem();
  virtual ~IPCMessage_MessageItem();

  IPCMessage_MessageItem(const IPCMessage_MessageItem& from);

  inline IPCMessage_MessageItem& operator=(const IPCMessage_MessageItem& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const IPCMessage_MessageItem& default_instance();

  void Swap(IPCMessage_MessageItem* other);

  // implements Message ----------------------------------------------

  inline IPCMessage_MessageItem* New() const { return New(NULL); }

  IPCMessage_MessageItem* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const IPCMessage_MessageItem& from);
  void MergeFrom(const IPCMessage_MessageItem& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(IPCMessage_MessageItem* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .ipc.IPCMessage.MessageType type = 1;
  void clear_type();
  static const int kTypeFieldNumber = 1;
  ::ipc::IPCMessage_MessageType type() const;
  void set_type(::ipc::IPCMessage_MessageType value);

  // optional string text = 2;
  void clear_text();
  static const int kTextFieldNumber = 2;
  const ::std::string& text() const;
  void set_text(const ::std::string& value);
  void set_text(const char* value);
  void set_text(const char* value, size_t size);
  ::std::string* mutable_text();
  ::std::string* release_text();
  void set_allocated_text(::std::string* text);

  // @@protoc_insertion_point(class_scope:ipc.IPCMessage.MessageItem)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr text_;
  int type_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_ipc_2eproto();
  friend void protobuf_AssignDesc_ipc_2eproto();
  friend void protobuf_ShutdownFile_ipc_2eproto();

  void InitAsDefaultInstance();
  static IPCMessage_MessageItem* default_instance_;
};
// -------------------------------------------------------------------

class IPCMessage : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:ipc.IPCMessage) */ {
 public:
  IPCMessage();
  virtual ~IPCMessage();

  IPCMessage(const IPCMessage& from);

  inline IPCMessage& operator=(const IPCMessage& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const IPCMessage& default_instance();

  void Swap(IPCMessage* other);

  // implements Message ----------------------------------------------

  inline IPCMessage* New() const { return New(NULL); }

  IPCMessage* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const IPCMessage& from);
  void MergeFrom(const IPCMessage& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(IPCMessage* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef IPCMessage_MessageItem MessageItem;

  typedef IPCMessage_MessageType MessageType;
  static const MessageType GET_SCENEINFO =
    IPCMessage_MessageType_GET_SCENEINFO;
  static const MessageType SET_SCENEINFO =
    IPCMessage_MessageType_SET_SCENEINFO;
  static const MessageType LOG_DEBUG =
    IPCMessage_MessageType_LOG_DEBUG;
  static const MessageType LOG_ERROR =
    IPCMessage_MessageType_LOG_ERROR;
  static inline bool MessageType_IsValid(int value) {
    return IPCMessage_MessageType_IsValid(value);
  }
  static const MessageType MessageType_MIN =
    IPCMessage_MessageType_MessageType_MIN;
  static const MessageType MessageType_MAX =
    IPCMessage_MessageType_MessageType_MAX;
  static const int MessageType_ARRAYSIZE =
    IPCMessage_MessageType_MessageType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  MessageType_descriptor() {
    return IPCMessage_MessageType_descriptor();
  }
  static inline const ::std::string& MessageType_Name(MessageType value) {
    return IPCMessage_MessageType_Name(value);
  }
  static inline bool MessageType_Parse(const ::std::string& name,
      MessageType* value) {
    return IPCMessage_MessageType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // optional int32 pid = 1;
  void clear_pid();
  static const int kPidFieldNumber = 1;
  ::google::protobuf::int32 pid() const;
  void set_pid(::google::protobuf::int32 value);

  // repeated .ipc.IPCMessage.MessageItem item = 2;
  int item_size() const;
  void clear_item();
  static const int kItemFieldNumber = 2;
  const ::ipc::IPCMessage_MessageItem& item(int index) const;
  ::ipc::IPCMessage_MessageItem* mutable_item(int index);
  ::ipc::IPCMessage_MessageItem* add_item();
  ::google::protobuf::RepeatedPtrField< ::ipc::IPCMessage_MessageItem >*
      mutable_item();
  const ::google::protobuf::RepeatedPtrField< ::ipc::IPCMessage_MessageItem >&
      item() const;

  // @@protoc_insertion_point(class_scope:ipc.IPCMessage)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::RepeatedPtrField< ::ipc::IPCMessage_MessageItem > item_;
  ::google::protobuf::int32 pid_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_ipc_2eproto();
  friend void protobuf_AssignDesc_ipc_2eproto();
  friend void protobuf_ShutdownFile_ipc_2eproto();

  void InitAsDefaultInstance();
  static IPCMessage* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// IPCMessage_MessageItem

// optional .ipc.IPCMessage.MessageType type = 1;
inline void IPCMessage_MessageItem::clear_type() {
  type_ = 0;
}
inline ::ipc::IPCMessage_MessageType IPCMessage_MessageItem::type() const {
  // @@protoc_insertion_point(field_get:ipc.IPCMessage.MessageItem.type)
  return static_cast< ::ipc::IPCMessage_MessageType >(type_);
}
inline void IPCMessage_MessageItem::set_type(::ipc::IPCMessage_MessageType value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:ipc.IPCMessage.MessageItem.type)
}

// optional string text = 2;
inline void IPCMessage_MessageItem::clear_text() {
  text_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& IPCMessage_MessageItem::text() const {
  // @@protoc_insertion_point(field_get:ipc.IPCMessage.MessageItem.text)
  return text_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void IPCMessage_MessageItem::set_text(const ::std::string& value) {
  
  text_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:ipc.IPCMessage.MessageItem.text)
}
inline void IPCMessage_MessageItem::set_text(const char* value) {
  
  text_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:ipc.IPCMessage.MessageItem.text)
}
inline void IPCMessage_MessageItem::set_text(const char* value, size_t size) {
  
  text_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:ipc.IPCMessage.MessageItem.text)
}
inline ::std::string* IPCMessage_MessageItem::mutable_text() {
  
  // @@protoc_insertion_point(field_mutable:ipc.IPCMessage.MessageItem.text)
  return text_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* IPCMessage_MessageItem::release_text() {
  // @@protoc_insertion_point(field_release:ipc.IPCMessage.MessageItem.text)
  
  return text_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void IPCMessage_MessageItem::set_allocated_text(::std::string* text) {
  if (text != NULL) {
    
  } else {
    
  }
  text_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), text);
  // @@protoc_insertion_point(field_set_allocated:ipc.IPCMessage.MessageItem.text)
}

// -------------------------------------------------------------------

// IPCMessage

// optional int32 pid = 1;
inline void IPCMessage::clear_pid() {
  pid_ = 0;
}
inline ::google::protobuf::int32 IPCMessage::pid() const {
  // @@protoc_insertion_point(field_get:ipc.IPCMessage.pid)
  return pid_;
}
inline void IPCMessage::set_pid(::google::protobuf::int32 value) {
  
  pid_ = value;
  // @@protoc_insertion_point(field_set:ipc.IPCMessage.pid)
}

// repeated .ipc.IPCMessage.MessageItem item = 2;
inline int IPCMessage::item_size() const {
  return item_.size();
}
inline void IPCMessage::clear_item() {
  item_.Clear();
}
inline const ::ipc::IPCMessage_MessageItem& IPCMessage::item(int index) const {
  // @@protoc_insertion_point(field_get:ipc.IPCMessage.item)
  return item_.Get(index);
}
inline ::ipc::IPCMessage_MessageItem* IPCMessage::mutable_item(int index) {
  // @@protoc_insertion_point(field_mutable:ipc.IPCMessage.item)
  return item_.Mutable(index);
}
inline ::ipc::IPCMessage_MessageItem* IPCMessage::add_item() {
  // @@protoc_insertion_point(field_add:ipc.IPCMessage.item)
  return item_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::ipc::IPCMessage_MessageItem >*
IPCMessage::mutable_item() {
  // @@protoc_insertion_point(field_mutable_list:ipc.IPCMessage.item)
  return &item_;
}
inline const ::google::protobuf::RepeatedPtrField< ::ipc::IPCMessage_MessageItem >&
IPCMessage::item() const {
  // @@protoc_insertion_point(field_list:ipc.IPCMessage.item)
  return item_;
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace ipc

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::ipc::IPCMessage_MessageType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::ipc::IPCMessage_MessageType>() {
  return ::ipc::IPCMessage_MessageType_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ipc_2eproto__INCLUDED