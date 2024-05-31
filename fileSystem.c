#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 100
#define DISK_SIZE 10485760

typedef struct {
    char filename[50]; 
    int start; 
    int size;
} FileEntry;

FileEntry file_table[MAX_FILES]; 
int file_count = 0; 
const char* disk_filename = "disk.img";

void save_file_table() {
    FILE* disk = fopen(disk_filename, "r+");
    if (disk == NULL) {
        printf("Erro: não foi possível abrir o disco virtual.\n");
        return;
    }
    fseek(disk, 0, SEEK_SET);
    fwrite(&file_count, sizeof(int), 1, disk);
    fwrite(file_table, sizeof(FileEntry), MAX_FILES, disk);
    fclose(disk);
}

void load_file_table() {
    FILE* disk = fopen(disk_filename, "rb");
    if (disk == NULL) {
        disk = fopen(disk_filename, "wb");
        if (disk == NULL) {
            printf("Erro: não foi possível criar o disco virtual.\n");
            exit(1);
        }

        fseek(disk, DISK_SIZE - 1, SEEK_SET);
        fputc('\0', disk);

        file_count = 0;
        memset(file_table, 0, sizeof(file_table));
        fseek(disk, 0, SEEK_SET);
        fwrite(&file_count, sizeof(int), 1, disk);
        fwrite(file_table, sizeof(FileEntry), MAX_FILES, disk);
        fclose(disk);
        return;
    }
    fread(&file_count, sizeof(int), 1, disk);
    fread(file_table, sizeof(FileEntry), MAX_FILES, disk);
    fclose(disk);
}

int file_exists(const char* filename) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].filename, filename) == 0) {
            return 1; 
        }
    }
    return 0;
}


void cat(const char* filename, const char* content) {
    if (file_count >= MAX_FILES) {
        printf("Erro: número máximo de arquivos atingido.\n");
        return;
    }

    if (file_exists(filename)) {
        printf("Erro: arquivo com esse nome já existe.\n");
        return;
    }

    int content_length = strlen(content);
    int start_position = (file_count == 0) ? sizeof(int) + sizeof(FileEntry) * MAX_FILES : file_table[file_count - 1].start + file_table[file_count - 1].size;

    if (start_position + content_length > DISK_SIZE) {
        printf("Erro: não há espaço suficiente no disco.\n");
        return;
    }

    strcpy(file_table[file_count].filename, filename);
    file_table[file_count].start = start_position;
    file_table[file_count].size = content_length;
    file_count++;

    FILE* disk = fopen(disk_filename, "r+");

    if (disk == NULL) {
        printf("Erro: não foi possível abrir o disco virtual.\n");
        return;
    }
    fseek(disk, start_position, SEEK_SET);
    fwrite(content, 1, content_length, disk);
    fclose(disk);

    save_file_table();
}


void ls() {
    for (int i = 0; i < file_count; i++) {
        printf("%s\n", file_table[i].filename);
    }
}


void more(const char* filename) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].filename, filename) == 0) {
            char* content = (char*) malloc(file_table[i].size + 1);
            FILE* disk = fopen(disk_filename, "r");
            if (disk == NULL) {
                printf("Erro: não foi possível abrir o disco virtual.\n");
                return;
            }
            fseek(disk, file_table[i].start, SEEK_SET);
            fread(content, 1, file_table[i].size, disk);
            content[file_table[i].size] = '\0';
            printf("%s\n", content);
            free(content);
            fclose(disk);
            return;
        }
    }
    printf("Erro: arquivo não encontrado.\n");
}

int main(int argc, char* argv[]) {
    load_file_table();
    if (argc < 2) {
        printf("Uso: %s comando [argumentos]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "cat") == 0 && argc == 4) {
        cat(argv[2], argv[3]);
    } else if (strcmp(argv[1], "ls") == 0) {
        ls();
    } else if (strcmp(argv[1], "more") == 0 && argc == 3) {
        more(argv[2]);
    } else {
        printf("Comando inválido ou argumentos insuficientes.\n");
    }

    return 0;
}
