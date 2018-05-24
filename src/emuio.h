//
// Created by Yoram Boccia on 24/05/2018.
//

//instruction from file to address
int read(char *fileName, int *address) {

    FILE *file;

    char *buffer;
    unsigned long lengthFile;

    char *buffer;
    unsigned long fileLen;

    file = fopen(fileName, "r");

    if (!file) {
        fprintf(stderr, "Unable to open file %s", fileName);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    fileLen = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = (char *) malloc(fileLen + 1);

    if (!buffer) {
        fprintf(stderr, "Memory error!");
        fclose(file);
        return 0;
    }

    fread(buffer, fileLen, 1, file);
    fclose(file);

    char *array;

    convert(buffer, address);

    free(buffer);

}

int convert(char *buffer, int *address) {
    int x = 0;
    int y = 0;

    int decimal = 1;

    while (buffer[y] != '\0') {
        int pow = 0;
        for (int i = 0; i < 32; ++i) {
            if (buffer[y] == '\001') {
                decimal = decimal << pow;
            }
            y++;
            pow++;
        }
        address[x] = decimal;
        x++;
    }
}

//write(storage)

