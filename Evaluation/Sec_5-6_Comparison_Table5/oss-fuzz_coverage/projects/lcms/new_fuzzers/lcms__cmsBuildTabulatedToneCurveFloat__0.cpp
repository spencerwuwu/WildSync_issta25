// Copyright 2016 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#include "lcms2.h"

 int  LLVMFuzzerTestOneInput(  uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(cmsFloat64Number) + 1) {
	return 0;
}
cmsFloat64Number __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(cmsFloat64Number));
_BUF_POINTER += sizeof(cmsFloat64Number);

cmsFloat64Number g;
g = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

/**** End of fuzz variables initialization code ****/
if (size == 0) return 0;

  cmsHANDLE handle = cmsIT8LoadFromMem(0, (void *)data, size);
/**** Start of extracted external code ****/
// Extracted from: ghostscript: lcms2mt/testbed/testcms2.c_0
    cmsFloat32Number Values[1025];
    cmsToneCurve* Curve;
    cmsInt32Number i;
    cmsFloat32Number in, out;

    for (i=0; i <= 1024; i++) {

        in = (cmsFloat32Number) (i / 1024.0);
        Values[i] = powf(in, (float) g);
    }

    Curve = cmsBuildTabulatedToneCurveFloat(0, 1025, Values);
/**** End of extracted external code ****/
  if (handle) {
    char filename[256];
    sprintf(filename, "/tmp/fuzzer-it.%d.it8", getpid());
    cmsIT8SaveToFile(handle, filename);

    cmsIT8Free(handle);
  }

  return 0;
}
