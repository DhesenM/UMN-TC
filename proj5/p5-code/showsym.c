// Template for parsing an ELF file to print its symbol table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>

// The below macros help to prevent errors when doing pointer
// arithmetic. Adding an offset to a pointer is the most common
// operation here while the other macros may not be needed.

#define PTR_PLUS_BYTES(ptr,off) ((void *) (((size_t) (ptr)) + ((size_t) (off))))
// macro to add a byte offset to a pointer, arguments are a pointer
// and a # of bytes (usually size_t)

#define PTR_MINUS_BYTES(ptr,off) ((void *) (((size_t) (ptr)) - ((size_t) (off))))
// macro to subtract a byte offset from a pointer, arguments are a pointer
// and a # of bytes (usually size_t)

#define PTR_MINUS_PTR(ptr,ptq) ((long) (((size_t) (ptr)) - ((size_t) (ptq))))
// macro to subtract one pointer from another

int DEBUG = 0;                  
// Controls whether to print debug messages
// Can be used in conditionals like if(DEBUG){ ... }
// and running 'showsym -d x.o' will set DEBUG=1

int main(int argc, char *argv[]){
  if(argc < 2){
    printf("usage: %s [-d] <file>\n",argv[0]);
    return 0;
  }

  char *objfile_name = argv[1];

  // check for debug mode
  if(argc >=3){
    if(strcmp("-d",argv[1])==0){
      DEBUG = 1;
      objfile_name = argv[2];
    }
    else{
      printf("incorrect usage\n");
      return 1;
    }
  }
                        
  // Open file descriptor and set up memory map for objfile_name
  int fd = open(objfile_name, O_RDONLY);
  struct stat stat_buf;
  fstat(fd, &stat_buf);                      // get stats on the open file such as size
  int size = stat_buf.st_size;               // size for mmap()'ed memory is size of file
  char *file_chars =                         // pointer to file contents
    mmap(NULL, size, PROT_READ, MAP_PRIVATE, // call mmap with given size and file descriptor
         fd, 0);

  // CREATE A POINTER to the intial bytes of the file which are an ELF64_Ehdr struct
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *) file_chars;


  // CHECK e_ident field's bytes 0 to for for the sequence {0x7f,'E','L','F'}.
  // Exit the program with code 1 if the bytes do not match
  if(ehdr -> e_ident[0] != 0x7f || 
     ehdr -> e_ident[1] != 'E'  || 
     ehdr -> e_ident[2] != 'L'  || 
     ehdr -> e_ident[3] != 'F'){
    printf("ERROR: Magic bytes wrong, this is not an ELF file");
    return 1;
  }

  // PROVIDED: check for a 64-bit file
  if(ehdr -> e_ident[EI_CLASS] != ELFCLASS64){
    printf("Not a 64-bit file ELF file\n");
    return 1;
  }

  // PROVIDED: check for x86-64 architecture
  if(ehdr -> e_machine != EM_X86_64){
    printf("Not an x86-64 file\n");
    return 1;
  }

  // DETERMINE THE OFFSET of the Section Header Array (e_shoff), the
  // number of sections (e_shnum), and the index of the Section Header
  // String table (e_shstrndx). These fields are from the ELF File
  // Header.
  int offset = ehdr -> e_shoff;
  int number = ehdr -> e_shnum;
  int index = ehdr -> e_shstrndx;

  // Set up a pointer to the array of section headers. Use the section
  // header string table index to find its byte position in the file
  // and set up a pointer to it.
  Elf64_Shdr *sec_hdrs = PTR_PLUS_BYTES(file_chars, offset);
  int str_offset = sec_hdrs[index].sh_offset;
  char *names = PTR_PLUS_BYTES(file_chars, str_offset);
  

  // Search the Section Header Array for the secion with name .symtab
  // (symbol table) and .strtab (string table).  Note their positions
  // in the file (sh_offset field).  Also note the size in bytes
  // (sh_size) and and the size of each entry (sh_entsize) for .symtab
  // so its number of entries can be computed.
  Elf64_Off symtaboff = 0;
  Elf64_Off strtaboff = 0;
  long total_size;
  long entries;
  long per_entry;

  for(int i = 0; i < number; i++){
      Elf64_Shdr header = sec_hdrs[i];
      char *headname = PTR_PLUS_BYTES(header.sh_name, names);

      if(strcmp(headname, ".symtab") == 0){       //if the name of the section = ".symtab"
      // Calculate total size, entries, and per entry
        symtaboff = header.sh_offset;
        total_size = header.sh_size;
        entries = header.sh_entsize;
        per_entry = total_size / entries;
      }
      else if(strcmp(headname, ".strtab") == 0){  //if the name of the section = ".strtab"
        strtaboff = header.sh_offset;
      }
  }

  if(!symtaboff){
    printf("ERROR: Couldn't find symbol table\n");
    return 1;
  }

  if(!strtaboff){
    printf("ERROR: Couldn't find string table\n");
    return 1;
  }

  // PRINT byte information about where the symbol table was found and
  // its sizes. The number of entries in the symbol table can be
  // determined by dividing its total size in bytes by the size of
  // each entry.
  printf("Symbol Table\n");
  printf("- %ld bytes offset from start of file\n", symtaboff);
  printf("- %ld bytes total size\n", total_size);
  printf("- %ld bytes per entry\n", entries);
  printf("- %ld entries\n", per_entry);


  // Set up pointers to the Symbol Table and associated String Table
  // using offsets found earlier.
  Elf64_Sym *symtab = PTR_PLUS_BYTES(file_chars, symtaboff);
  char *strtab = PTR_PLUS_BYTES(file_chars, strtaboff);

  // Print column IDs for info on each symbol
  printf("[%3s]  %8s %4s %s\n",
         "idx","TYPE","SIZE","NAME");

  // Iterate over the symbol table entries
  for(int i = 0; i < per_entry; i++){
    // Determine size of symbol and name. Use <NONE> name has zero
    // length.
    char *symname = PTR_PLUS_BYTES(strtab, symtab[i].st_name);
    if(strcmp(symname,"") == 0){
      symname = "<NONE>";
    }

    // Determine type of symbol. See assignment specification for
    // fields, macros, and definitions related to this.
    unsigned char typec = ELF64_ST_TYPE(symtab[i].st_info);
    char *symtype;

    // Determine which assigned variable to use 
    if(typec == STT_NOTYPE){
      symtype = "NOTYPE";
    }
    else if(typec == STT_OBJECT){
      symtype = "OBJECT";
    }
    else if(typec == STT_FUNC){
      symtype = "FUNC";
    }
    else if(typec == STT_FILE){
      symtype = "FILE";
    }
    else{
      symtype = "SECTION";
    }

    // Print symbol information
    printf("[%3d]: %8s %4lu %s\n", i, symtype, symtab[i].st_size, symname);
  }


  // Unmap file from memory and close associated file descriptor 
  munmap(file_chars, size);
  close(fd);
  return 0;
}
