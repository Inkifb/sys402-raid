#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 1048576 // 1 MB

void store_get(const char *store_id, int unique_id) {
    char store_filename[256];
    snprintf(store_filename, sizeof(store_filename), "%s.store", store_id);

    // Open the store file
    FILE *store_file = fopen(store_filename, "rb");
    if (!store_file) {
        perror("Failed to open store file");
        exit(EXIT_FAILURE);
    }

    long offset = unique_id * BLOCK_SIZE;
    if (fseek(store_file, offset, SEEK_SET) != 0) {
        perror("Failed to seek to the correct position in store file");
        fclose(store_file);
        exit(EXIT_FAILURE);
    }

    // Read the block
    char buffer[BLOCK_SIZE];
    size_t bytes_read = fread(buffer, 1, BLOCK_SIZE, store_file);
    if (bytes_read == 0) {
        perror("Failed to read from store file");
        fclose(store_file);
        exit(EXIT_FAILURE);
    }

    fwrite(buffer, 1, bytes_read, stdout);

    fclose(store_file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s [store-id] [unique identifier]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int unique_id = atoi(argv[2]);
    store_get(argv[1], unique_id);
    return EXIT_SUCCESS;
}