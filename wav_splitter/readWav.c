#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void parse_header(struct wav_header*, FILE*);

int check_header(struct wav_header*);

void print_header(struct wav_header*);

struct wav_header{
    char container[5];
    uint64_t file_size;
    char type[5];
    char format[5];
    uint64_t format_length;
    uint16_t format_type;
    uint16_t channels;
    uint64_t sample_rate;
    uint64_t channel_bytes_per_sec; // sample_rate * bits/sample * channels / 8
    uint16_t channel_bytes_per_sample; // bits/sample * channels / 8
    uint16_t bits_per_sample;
    char data[5];
    uint64_t data_size;
};

int main(int argc, char** argv){
    // make sure an input is specified
    if(argc != 2){
        printf("Please specify one input .wav file\n");
        return 1;
    }

    // ensure file exists, open if it does
    FILE *fptr;
    fptr = fopen(argv[1], "rb");
    if(fptr == NULL){
        printf("Input file does not exists\n");
        return 1;
    }

    // parse the .wav header
    struct wav_header h1 = {0};
    parse_header(&h1, fptr);

    // ensure opened file is .wav
    if(check_header(&h1)){
        return 1;
    }

    // print header values
    // print_header(&h1);

    // create matrix for each channel
    int16_t *channel[h1.channels];
    uint64_t samples_per_channel = h1.data_size/h1.channel_bytes_per_sample;
    for(uint8_t i=0; i<h1.channels; i++){
        channel[i] = malloc(samples_per_channel * sizeof(int16_t));
    }

    // fill matrices with waveform data
    for(uint64_t i=0; i<samples_per_channel; i++){
        for(uint8_t j=0; j<h1.channels; j++){
            fread(channel[j]+i, 1, 2, fptr);
        }
    }

    // write waveforms to binary files as a test
    FILE *files[h1.channels];
    char outFile[18];
    for(uint8_t i=0; i<h1.channels; i++){
        sprintf(outFile, "./channel_%hhu.bin", i);
        files[i] = fopen(outFile, "wb");
    }
    
    for(uint8_t i=0; i<h1.channels; i++){
        fwrite(channel[i], sizeof(int16_t), samples_per_channel, files[i]);
        fclose(files[i]);
    }

    // free waveforms
    for(uint8_t i=0; i<h1.channels; i++){
        free(channel[i]);
    }

    fclose(fptr);

    return 0;
}

void parse_header(struct wav_header *header_ptr, FILE *wav_file){
    // read header data
    struct wav_header h1 = {0};
    fread(&header_ptr->container,                    1, 4, wav_file);
    fread(&header_ptr->file_size,                    1, 4, wav_file);
    fread(&header_ptr->type,                         1, 4, wav_file);
    fread(&header_ptr->format,                       1, 4, wav_file);
    fread(&header_ptr->format_length,                1, 4, wav_file);
    fread(&header_ptr->format_type,                  1, 2, wav_file);
    fread(&header_ptr->channels,                     1, 2, wav_file);
    fread(&header_ptr->sample_rate,                  1, 4, wav_file);
    fread(&header_ptr->channel_bytes_per_sec,        1, 4, wav_file);
    fread(&header_ptr->channel_bytes_per_sample,     1, 2, wav_file);
    fread(&header_ptr->bits_per_sample,              1, 2, wav_file);
    fread(&header_ptr->data,                         1, 4, wav_file);
    fread(&header_ptr->data_size,                    1, 4, wav_file);

    // add null terminators to strings
    header_ptr->container[4] = '\0';
    header_ptr->type[4] = '\0';
    header_ptr->format[4] = '\0';
    header_ptr->data[4] = '\0';
}

// return 0 if successful, 1 if unsuccessful
int check_header(struct wav_header *header_ptr){

    int8_t ret = 0;

    // container
    if(strcmp(header_ptr->container, "RIFF")){
        printf("Error: Not specified as RIFF\n");
        ret = 1;
    }
    // type
    if(strcmp(header_ptr->type, "WAVE")){
        printf("Error: Type is not set as WAVE\n");
        ret = 1;
    }

    return ret;
}

// print all header values, expects a .wav file
void print_header(struct wav_header *header_ptr){
    // container
    printf("container:\t\t\t%s\n", header_ptr->container);
    // file_size
    printf("file_size:\t\t\t%lu\n", header_ptr->file_size);
    // type
    printf("type:\t\t\t\t%s\n", header_ptr->type);
    // format
    printf("format:\t\t\t\t%s\n", header_ptr->format);
    // format_length
    printf("format_length:\t\t\t%lu\n", header_ptr->format_length);
    // format_type
    printf("format_type:\t\t\t%u\n", header_ptr->format_type);
    // channels
    printf("channels:\t\t\t%u\n", header_ptr->channels);
    // sample_rate
    printf("sample_rate:\t\t\t%lu\n", header_ptr->sample_rate);
    // channel_bytes_per_sec
    printf("channel_bytes_per_sec:\t\t%lu\n", header_ptr->channel_bytes_per_sec);
    // channel_bytes_per_sample
    printf("channel_bytes_per_sample:\t%u\n", header_ptr->channel_bytes_per_sample);
    // bits_per_sample
    printf("bits_per_sample:\t\t%u\n", header_ptr->bits_per_sample);
    // data
    printf("data:\t\t\t\t%s\n", header_ptr->data);
    // data_size
    printf("data_size:\t\t\t%lu\n", header_ptr->data_size);
}
