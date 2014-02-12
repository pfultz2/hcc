// XFAIL: *
// Copyright (c) Microsoft
// All rights reserved
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
// THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
// INCLUDING WITHOUT LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
// See the Apache Version 2.0 License for specific language governing permissions and limitations under the License.
/// <tags>P1</tags>
/// <summary>Verify array associate_accelerator_view property - is valid (same as acclv2) when acclv1 and accl2 are compatible for staging</summary>
// RUN: %amp_device -D__GPU__ %s -m32 -emit-llvm -c -S -O2 -o %t.ll && mkdir -p %t
// RUN: %clamp-device %t.ll %t/kernel.cl
// RUN: pushd %t && %embed_kernel kernel.cl %t/kernel.o && popd
// RUN: %cxxamp %link %t/kernel.o %s -o %t.out && %t.out
#include "./../../../member.h"

int main()
{
	if (!is_gpu_hardware_available())
		return 2;

	const int _rank = 5;
    int edata[_rank];
    for (int i = 0; i < _rank; i++)
        edata[i] = 3;
    extent<_rank> e1(edata);

    int passed = 1;
    {
        accelerator_view gpu_av = _gpu_device.create_view(queuing_mode_immediate);
        accelerator_view cpu_av = _cpu_device.create_view(queuing_mode_immediate);

        array<int, _rank> src(e1, cpu_av, gpu_av);

        if (src.get_associated_accelerator_view() != src.get_accelerator_view())
            passed = 0;
    }

    return passed;
}

