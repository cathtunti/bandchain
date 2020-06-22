/* (c) 2020 Band protocol. Licensed under Apache-2.0 */

/* Generated with cbindgen:0.14.2 */

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum Error {
  Error_NoError = 0,
  Error_CompliationError = 1,
  Error_RunError = 2,
  Error_ParseError = 3,
  Error_WriteBinaryError = 4,
  Error_ResolveNamesError = 5,
  Error_ValidateError = 6,
  Error_SpanExceededCapacityError = 7,
  Error_DeserializationError = 8,
  Error_GasCounterInjectionError = 9,
  Error_SerializationError = 10,
  Error_GasLimitExceedError = 11,
  Error_NoMemoryWasmError = 12,
  Error_MinimumMemoryExceedError = 13,
  Error_SetMaximumMemoryError = 14,
  Error_StackHeightInstrumentationError = 15,
  Error_CheckWasmImportsError = 16,
  Error_CheckWasmExportsError = 17,
  Error_InvalidSignatureFunctionError = 18,
  Error_SetReturnDataWrongPeriodError = 128,
  Error_AnsCountWrongPeriodError = 129,
  Error_AskExternalDataWrongPeriodError = 130,
  Error_AskExternalDataExceedError = 131,
  Error_GetExternalDataStatusWrongPeriodError = 132,
  Error_GetExternalDataWrongPeriodError = 133,
  Error_ValidatorOutOfRangeError = 134,
  Error_InvalidExternalIDError = 135,
  Error_GetUnreportedDataError = 136,
  Error_UnknownError = 255,
};
typedef int32_t Error;

enum GoResult {
  GoResult_Ok = 0,
  GoResult_SpanExceededCapacity = 1,
  GoResult_SetReturnDataWrongPeriod = 2,
  GoResult_AnsCountWrongPeriod = 3,
  GoResult_AskExternalDataWrongPeriod = 4,
  GoResult_AskExternalDataExceed = 5,
  GoResult_GetExternalDataStatusWrongPeriod = 6,
  GoResult_GetExternalDataWrongPeriod = 7,
  GoResult_ValidatorOutOfRange = 8,
  GoResult_InvalidExternalID = 9,
  GoResult_GetUnreportedData = 10,
  GoResult_Other = 11,
};
typedef int32_t GoResult;

typedef struct Span {
  uint8_t *ptr;
  uintptr_t len;
  uintptr_t cap;
} Span;

typedef struct env_t {
  uint8_t _private[0];
} env_t;

typedef struct EnvDispatcher {
  GoResult (*get_calldata)(env_t*, Span *calldata);
  GoResult (*set_return_data)(env_t*, Span data);
  int64_t (*get_ask_count)(env_t*);
  int64_t (*get_min_count)(env_t*);
  GoResult (*get_ans_count)(env_t*, int64_t*);
  GoResult (*ask_external_data)(env_t*, int64_t eid, int64_t did, Span data);
  GoResult (*get_external_data_status)(env_t*, int64_t eid, int64_t vid, int64_t *status);
  GoResult (*get_external_data)(env_t*, int64_t eid, int64_t vid, Span *data);
} EnvDispatcher;

typedef struct Env {
  env_t *env;
  EnvDispatcher dis;
} Env;

Error do_compile(Span input, Span *output);

Error do_run(Span code, uint32_t gas_limit, int64_t span_size, bool is_prepare, Env env);
