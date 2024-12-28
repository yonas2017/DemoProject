// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_SERVERMESSAGE_H_
#define FLATBUFFERS_GENERATED_SERVERMESSAGE_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 12 &&
              FLATBUFFERS_VERSION_REVISION == 23,
             "Non-compatible flatbuffers version included");

struct server_message_t;
struct server_message_tBuilder;

struct server_message_t FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef server_message_tBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_JOB_INFO = 4,
    VT_JOB_TEXT_LENGTH = 6,
    VT_JOB_TEKST = 8
  };
  uint8_t job_info() const {
    return GetField<uint8_t>(VT_JOB_INFO, 0);
  }
  uint32_t job_text_length() const {
    return GetField<uint32_t>(VT_JOB_TEXT_LENGTH, 0);
  }
  const ::flatbuffers::Vector<int8_t> *job_tekst() const {
    return GetPointer<const ::flatbuffers::Vector<int8_t> *>(VT_JOB_TEKST);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_JOB_INFO, 1) &&
           VerifyField<uint32_t>(verifier, VT_JOB_TEXT_LENGTH, 4) &&
           VerifyOffset(verifier, VT_JOB_TEKST) &&
           verifier.VerifyVector(job_tekst()) &&
           verifier.EndTable();
  }
};

struct server_message_tBuilder {
  typedef server_message_t Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_job_info(uint8_t job_info) {
    fbb_.AddElement<uint8_t>(server_message_t::VT_JOB_INFO, job_info, 0);
  }
  void add_job_text_length(uint32_t job_text_length) {
    fbb_.AddElement<uint32_t>(server_message_t::VT_JOB_TEXT_LENGTH, job_text_length, 0);
  }
  void add_job_tekst(::flatbuffers::Offset<::flatbuffers::Vector<int8_t>> job_tekst) {
    fbb_.AddOffset(server_message_t::VT_JOB_TEKST, job_tekst);
  }
  explicit server_message_tBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<server_message_t> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<server_message_t>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<server_message_t> Createserver_message_t(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t job_info = 0,
    uint32_t job_text_length = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<int8_t>> job_tekst = 0) {
  server_message_tBuilder builder_(_fbb);
  builder_.add_job_tekst(job_tekst);
  builder_.add_job_text_length(job_text_length);
  builder_.add_job_info(job_info);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<server_message_t> Createserver_message_tDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t job_info = 0,
    uint32_t job_text_length = 0,
    const std::vector<int8_t> *job_tekst = nullptr) {
  auto job_tekst__ = job_tekst ? _fbb.CreateVector<int8_t>(*job_tekst) : 0;
  return Createserver_message_t(
      _fbb,
      job_info,
      job_text_length,
      job_tekst__);
}

inline const server_message_t *Getserver_message_t(const void *buf) {
  return ::flatbuffers::GetRoot<server_message_t>(buf);
}

inline const server_message_t *GetSizePrefixedserver_message_t(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<server_message_t>(buf);
}

inline bool Verifyserver_message_tBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<server_message_t>(nullptr);
}

inline bool VerifySizePrefixedserver_message_tBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<server_message_t>(nullptr);
}

inline void Finishserver_message_tBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<server_message_t> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedserver_message_tBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<server_message_t> root) {
  fbb.FinishSizePrefixed(root);
}

#endif  // FLATBUFFERS_GENERATED_SERVERMESSAGE_H_
