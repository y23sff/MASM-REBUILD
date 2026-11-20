#ifndef CORE_H
#define CORE_H

#include <cstdint>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "arr.h"

typedef enum {
	SECTION_NULL,
	SECTION_TEXT,
	SECTION_DATA,
	SECTION_RODATA,
	SECTION_BSS
} SECTION;

typedef enum {
	OPERAND_REGISTER,
	OPERAND_IMM,
	OPERAND_LABEL,
	OPERAND_STRING,
	OPERAND_NULL,
} OPERAND;

typedef struct {
  uint8_t *bin_local;

} patch_definition_t;

typedef struct {
  uint8_t *byte_code;
  uint8_t *bin_arr, *data_arr, *rodata_arr, *arch_output_bin, *format_output_bin;
  uint64_t bss_size;
} Context;

typedef void (*encode)(Context *ctx, char *mnemonic, OPERAND *op_arr, void *imm_arr);
typedef void (*link)  (Context *ctx);

typedef struct {
	char *name;
	encode encoder;
} arch_module_t;

typedef struct {
	const char *name;
	char *arch_support_arr;
  link linker;
} format_module_t;

arch_module_t *arch_registry_arr = NULL;
format_module_t *format_registry_arr = NULL;

arch_module_t* find_arch(char *name) {
	for (size_t i = 0; i < arr_len(arch_registry_arr); i++) {
		if (strcmp(arch_registry_arr[i].name, name) == 0) {
			return &arch_registry_arr[i]; 
		}
	}
}

format_module_t* find_format(char *name) {
	for (size_t i = 0; i < arr_len(format_registry_arr); i++) {
		if (strcmp(format_registry_arr[i].name, name) == 0) {
			return &format_registry_arr[i]; 
		}
	}
}

void push_arch_module(arch_module_t arch_module) {
  arr_push(arch_registry_arr, arch_module);
}

void push_format_module(format_module_t format_module) {
  arr_push(format_registry_arr, format_module);
}


#endif
