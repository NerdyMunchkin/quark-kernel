#include "elf.hpp"
#include "util.hpp"

kernelns::ELF::ELF()
{
    this->m_fileLocation = (void*) NULL;
}

kernelns::ELF::ELF(void* location)
{
    this->m_fileLocation = location;
}

void* kernelns::ELF::entry()
{
    Header* fileHeader = (Header*) m_fileLocation;
    return fileHeader->entry;
}

int kernelns::ELF::validate()
{
    Header* fileHeader = (Header*) m_fileLocation;
    if(fileHeader->magic != 0x464c457f)
        return -1;
    else if((ISA) fileHeader->machine != HOST_ISA)
        return -1;
    else if((Endianness) fileHeader->endianness != Little)
        return -1;
    return 0;
}

int kernelns::ELF::load()
{
    Header* fileHeader = (Header*) m_fileLocation;
    ProgramHeader* programHeader = (ProgramHeader*) ((size_t) m_fileLocation + fileHeader->phoffset);
    int count = (int) fileHeader->phcount;
    for(int i = 0; i < count; i++)
    {
        if((SegmentType) programHeader->type != Load)
            continue;
        memcpy(programHeader->vaddr, m_fileLocation + programHeader->offset, programHeader->filesize);
    }
    return 0;
}