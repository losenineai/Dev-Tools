//    Copyright 2017 jmpews
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#ifndef MachoMem_h
#define MachoMem_h

#include "MachoRuntime.h"

#define MACHO_LOAD_ADDRESS 0x100000000

class MachoMem : public MachoRuntime
{
public:
  MachoMem();

  MachoMem(input_t input);

  MachoMem(zaddr load_addr);

  //where is dyld load
  // zaddr m_dyld_load_addr;

  /* common */
  bool check_initialization();
  bool macho_read(zaddr addr, zpointer data, zsize len);
  char *macho_read_string(zaddr addr);

  bool searchBinLoadAddress();

  // bool search_dyld_load_address(zaddr dyld_vm_addr);

  // bool check_dyld_arch(zaddr addr);

  // bool parse_dyld();
};

#endif //MACHOPARSER_MACHOMEM_H
