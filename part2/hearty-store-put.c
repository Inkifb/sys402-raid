#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 1048576 
#define TOTAL_BLOCKS 1024
#define METADATA_SIZE 256 

// Function to generate a unique identifier
int generate_unique_id() {
    static int id = 0;
    return id++;
}

void store_put(const char *store_id, const char *file_path) {
    char store_filename[256];
    snprintf(store_filename, sizeof(store_filename), "%s.store", store_id);

    // Open the store file
    FILE *store_file = fopen(store_filename, "r+b");
    if (!store_file) {
        perror("Failed to open store file");
        exit(EXIT_FAILURE);
    }

    FILE *original_file = fopen(file_path, "rb");
    if (!original_file) {
        perror("Failed to open original file");
        fclose(store_file);
        exit(EXIT_FAILURE);
    }

    // Check the size of the original file
    fseek(original_file, 0, SEEK_END);
    long file_size = ftell(original_file);
    fseek(original_file, 0, SEEK_SET);

    if (file_size > BLOCK_SIZE) {
        fprintf(stderr, "File size exceeds 1 MB block limit.\n");
        fclose(original_file);
        fclose(store_file);
        exit(EXIT_FAILURE);
    }

    int unique_id = generate_unique_id();

    long offset = unique_id * BLOCK_SIZE;

    fseek(store_file, offset, SEEK_SET);
    char *buffer = (char *)malloc(file_size);
    if (!buffer) {
        perror("Failed to allocate memory for buffer");
        fclose(original_file);
        fclose(store_file);
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, file_size, original_file);
    fwrite(buffer, 1, file_size, store_file);

    free(buffer);
    fclose(original_file);
    fclose(store_file);

    printf("File stored with unique identifier: %d\n", unique_id);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s [store-id] [original file path]\n", argv[0]);
        return EXIT_FAILURE;
    }

    store_put(argv[1], argv[2]);
    return EXIT_SUCCESS;
}