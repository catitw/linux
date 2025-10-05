// SPDX-License-Identifier: GPL-2.0-only
/* ----------------------------------------------------------------------- *
 *
 *  Copyright (C) 2009 Intel Corporation. All rights reserved.
 *
 *  H. Peter Anvin <hpa@linux.intel.com>
 *
 * -----------------------------------------------------------------------
 *
 * Outputs a small assembly wrapper with the appropriate symbols defined.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <tools/le_byteshift.h>

int main(int argc, char *argv[])
{
	uint32_t olen;
	long ilen;
	FILE *f = NULL;
	int retval = 1;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s compressed_file\n", argv[0]);
		goto bail;
	}

	/* Get the information for the compressed kernel image first */

	f = fopen(argv[1], "r");
	if (!f) {
		perror(argv[1]);
		goto bail;
	}


	// 先读取最后四个字节，组成olen(其保存着解压缩后， 文件的大小)
	if (fseek(f, -4L, SEEK_END)) {
		perror(argv[1]);
	}

	if (fread(&olen, sizeof(olen), 1, f) != 1) {
		perror(argv[1]);
		goto bail;
	}

	ilen = ftell(f);	// 被压缩后的文件的总大小
	olen = get_unaligned_le32(&olen);

	printf(".section \".rodata..compressed\",\"a\",@progbits\n");
	printf(".globl z_input_len\n");
	printf("z_input_len = %lu\n", ilen);
	printf(".globl z_output_len\n");
	printf("z_output_len = %lu\n", (unsigned long)olen);

	printf(".globl input_data, input_data_end\n");
	printf("input_data:\n");
	// `.incbin`的作用为: 
	// 把压缩后的`$(obj)/vmlinux.bin.$(suffix-y)`的文件的内容include进来
	// > https://sourceware.org/binutils/docs/as/Incbin.html
	printf(".incbin \"%s\"\n", argv[1]);
	printf("input_data_end:\n");

	printf(".section \".rodata\",\"a\",@progbits\n");
	printf(".globl input_len\n");
	printf("input_len:\n\t.long %lu\n", ilen);
	printf(".globl output_len\n");
	printf("output_len:\n\t.long %lu\n", (unsigned long)olen);

	retval = 0;
bail:
	if (f)
		fclose(f);
	return retval;
}
