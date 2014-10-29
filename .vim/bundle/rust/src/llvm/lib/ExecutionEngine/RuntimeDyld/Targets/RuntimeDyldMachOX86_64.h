//===-- RuntimeDyldMachOX86_64.h ---- MachO/X86_64 specific code. -*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_RUNTIMEDYLDMACHOX86_64_H
#define LLVM_RUNTIMEDYLDMACHOX86_64_H

#include "../RuntimeDyldMachO.h"

#define DEBUG_TYPE "dyld"

namespace llvm {

class RuntimeDyldMachOX86_64
    : public RuntimeDyldMachOCRTPBase<RuntimeDyldMachOX86_64> {
public:
  RuntimeDyldMachOX86_64(RTDyldMemoryManager *MM)
      : RuntimeDyldMachOCRTPBase(MM) {}

  unsigned getMaxStubSize() override { return 8; }

  unsigned getStubAlignment() override { return 1; }

  relocation_iterator
  processRelocationRef(unsigned SectionID, relocation_iterator RelI,
                       ObjectImage &ObjImg, ObjSectionToIDMap &ObjSectionToID,
                       const SymbolTableMap &Symbols, StubMap &Stubs) override {
    const MachOObjectFile &Obj =
        static_cast<const MachOObjectFile &>(*ObjImg.getObjectFile());
    MachO::any_relocation_info RelInfo =
        Obj.getRelocation(RelI->getRawDataRefImpl());

    assert(!Obj.isRelocationScattered(RelInfo) &&
           "Scattered relocations not supported on X86_64");

    RelocationEntry RE(getBasicRelocationEntry(SectionID, ObjImg, RelI));
    RelocationValueRef Value(
        getRelocationValueRef(ObjImg, RelI, RE, ObjSectionToID, Symbols));

    bool IsExtern = Obj.getPlainRelocationExternal(RelInfo);
    if (!IsExtern && RE.IsPCRel)
      makeValueAddendPCRel(Value, ObjImg, RelI, 1 << RE.Size);

    if (RE.RelType == MachO::X86_64_RELOC_GOT ||
        RE.RelType == MachO::X86_64_RELOC_GOT_LOAD)
      processGOTRelocation(RE, Value, Stubs);
    else {
      RE.Addend = Value.Addend;
      if (Value.SymbolName)
        addRelocationForSymbol(RE, Value.SymbolName);
      else
        addRelocationForSection(RE, Value.SectionID);
    }

    return ++RelI;
  }

  void resolveRelocation(const RelocationEntry &RE, uint64_t Value) {
    DEBUG(dumpRelocationToResolve(RE, Value));
    const SectionEntry &Section = Sections[RE.SectionID];
    uint8_t *LocalAddress = Section.Address + RE.Offset;

    // If the relocation is PC-relative, the value to be encoded is the
    // pointer difference.
    if (RE.IsPCRel) {
      // FIXME: It seems this value needs to be adjusted by 4 for an effective
      // PC address. Is that expected? Only for branches, perhaps?
      uint64_t FinalAddress = Section.LoadAddress + RE.Offset;
      Value -= FinalAddress + 4;
    }

    switch (RE.RelType) {
    default:
      llvm_unreachable("Invalid relocation type!");
    case MachO::X86_64_RELOC_SIGNED_1:
    case MachO::X86_64_RELOC_SIGNED_2:
    case MachO::X86_64_RELOC_SIGNED_4:
    case MachO::X86_64_RELOC_SIGNED:
    case MachO::X86_64_RELOC_UNSIGNED:
    case MachO::X86_64_RELOC_BRANCH:
      writeBytesUnaligned(LocalAddress, Value + RE.Addend, 1 << RE.Size);
      break;
    case MachO::X86_64_RELOC_GOT_LOAD:
    case MachO::X86_64_RELOC_GOT:
    case MachO::X86_64_RELOC_SUBTRACTOR:
    case MachO::X86_64_RELOC_TLV:
      Error("Relocation type not implemented yet!");
    }
  }

  void finalizeSection(ObjectImage &ObjImg, unsigned SectionID,
                       const SectionRef &Section) {}

private:
  void processGOTRelocation(const RelocationEntry &RE,
                            RelocationValueRef &Value, StubMap &Stubs) {
    SectionEntry &Section = Sections[RE.SectionID];
    assert(RE.IsPCRel);
    assert(RE.Size == 2);
    Value.Addend -= RE.Addend;
    RuntimeDyldMachO::StubMap::const_iterator i = Stubs.find(Value);
    uint8_t *Addr;
    if (i != Stubs.end()) {
      Addr = Section.Address + i->second;
    } else {
      Stubs[Value] = Section.StubOffset;
      uint8_t *GOTEntry = Section.Address + Section.StubOffset;
      RelocationEntry GOTRE(RE.SectionID, Section.StubOffset,
                            MachO::X86_64_RELOC_UNSIGNED, Value.Addend, false,
                            3);
      if (Value.SymbolName)
        addRelocationForSymbol(GOTRE, Value.SymbolName);
      else
        addRelocationForSection(GOTRE, Value.SectionID);
      Section.StubOffset += 8;
      Addr = GOTEntry;
    }
    RelocationEntry TargetRE(RE.SectionID, RE.Offset,
                             MachO::X86_64_RELOC_UNSIGNED, RE.Addend, true, 2);
    resolveRelocation(TargetRE, (uint64_t)Addr);
  }
};
}

#undef DEBUG_TYPE

#endif // LLVM_RUNTIMEDYLDMACHOX86_64_H
