#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 1048576 // 1 MB
#define TOTAL_BLOCKS 1024
#define METADATA_SIZE 256 // Example size for metadata

void initialize_store(const char *store_id) {
    char filename[256];
    snprintf(filename, sizeof(filename), "%s.store", store_id);

    // Open the file for writing
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to create store file");
        exit(EXIT_FAILURE);
    }

    // Initialize the storage space
    char *block = (char *)calloc(BLOCK_SIZE, sizeof(char));
    if (!block) {
        perror("Failed to allocate memory for block");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TOTAL_BLOCKS; i++) {
        if (fwrite(block, sizeof(char), BLOCK_SIZE, file) != BLOCK_SIZE) {
            perror("Failed to write block to store file");
            free(block);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    free(block);

    char metadata[METADATA_SIZE];
    memset(metadata, 0, METADATA_SIZE);
    if (fwrite(metadata, sizeof(char), METADATA_SIZE, file) != METADATA_SIZE) {
        perror("Failed to write metadata to store file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    printf("Store %s initialized successfully.\n", store_id);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [store-id]\n", argv[0]);
        return EXIT_FAILURE;
    }

    initialize_store(argv[1]);
    return EXIT_SUCCESS;
}