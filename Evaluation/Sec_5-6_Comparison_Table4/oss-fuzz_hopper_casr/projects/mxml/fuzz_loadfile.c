#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <mxml.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    char filename[256];
    sprintf(filename, "/tmp/libfuzzer.%d", getpid());
    char filename2[256];
    sprintf(filename, "/tmp/libfuzzer_s.%d", getpid());

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
	return 0;
    }
    fwrite(data, size, 1, fp);
    fclose(fp);

    mxml_node_t *tree = NULL;

    tree = mxmlLoadFilename(NULL, NULL, filename);


    if (tree != NULL) {
	mxmlSaveFilename(tree, NULL, filename2);
	free(tree);
    }

    unlink(filename);

    return 0;
}

