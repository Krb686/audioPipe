#ifndef DATAFILE_H
#define DATAFILE_H

#include <QByteArray>

class DataFile{

public:
    DataFile();
    ~DataFile();


    QByteArray hexRiffChunkId, hexRiffChunkSize, hexRiffChunkFormat;
    QByteArray hexFormatChunkId, hexFormatChunkSize, hexFormatAudioFormat, hexFormatNumChannels, hexFormatSampleRate, hexFormatByteRate, hexFormatBlockAlign, hexFormatBitsPerSample;
    QByteArray hexDataChunkId, hexDataChunkSize, hexDataChunk;

    QByteArray hexRiffChunkIdText, hexRiffChunkFormatText, hexFormatChunkIdText, hexDataChunkIdText;

    int riffChunkSize;
    int formatChunkSize, formatAudioFormat, formatNumChannels, formatSampleRate, formatByteRate, formatBlockAlign, formatBitsPerSample;
    int dataChunkSize;
};

#endif
