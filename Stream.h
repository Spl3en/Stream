/**
 * @file packet_stream.h
 * @brief Stream is a utility module for constructing packets as stream
 *
 * @license GNU GENERAL PUBLIC LICENSE - Version 2, June 1991
 *          See LICENSE file for further information
 */

#pragma once

#include "Pangu.h"

// Macro helper
#define streamIn(self, data) \
    streamAppend(self, (void *) (data), sizeof(*(data)))

#define streamOut(self, data) \
    streamGet(self, (void *) (data), sizeof(*(data)))

/**
 * @brief Stream is a buffer with a pointer to the last data written
 *
 * Stream is
 */
struct Stream {
    uint8_t *buffer;
    size_t size;
    uint64_t position;

    // Debug
    int sizeExcepted;
    const char *function;
};

typedef struct Stream Stream;

/**
 * @brief Allocate a new Stream structure.
 * @param buffer An allocated buffer for the stream. It must be big enough for the stream or it will overflow.
 * @return A pointer to an allocated Stream.
 */
Stream *streamNew(void *buffer, size_t size);

/**
 * @brief Initialize an allocated Stream structure.
 * @param self An allocated Stream to initialize.
 * @param buffer An allocated buffer for the stream. It must be big enough for the stream or it will overflow.
 * @return true on success, false otherwise.
 */
void streamInit(Stream *self, void *buffer, size_t size);

/**
 * @brief Move the position depending of the offset argument (position = position + offet)
 * @param self An allocated Stream
 * @param offset An offset
 */
void streamAddOffset(Stream *self, unsigned int offset);

/**
 * @brief Get the stream at the current position
 * @param self An allocated Stream
 */
void *streamGetCurrentBuffer(Stream *self);
void *streamGetBuffer(Stream *self);

/**
 * @brief Put data at the end of the stream
 * @param self An allocated Stream
 * @param data The data to append to the packet
 * @param dataSize the size of the data
 * @return
 */
void streamAppend(Stream *self, void *data, size_t dataSize);

/**
 * @brief Get the data from the current stream position and remove it
 * @param self An allocated Stream
 * @param data The data to append to the packet
 * @param dataSize the size of the data
 * @return
 */
void streamGet(Stream *self, void *data, size_t dataSize);

/**
 * @brief Free an allocated Stream structure and nullify the content of the pointer.
 *  It does *NOT* free the content of the stream, only the structure
 * @param self A pointer to an allocated Stream.
 */
void streamDestroy(Stream **self);

/**
 * Return the size of bytes written inside the stream
 */
size_t streamGetPosition(Stream *self);

/**
 * Return the size of bytes remining inside the stream
 */
size_t streamGetSize(Stream *self);

#define streamDebugStart(stream, size) \
    _streamDebugStart(stream, size, __FUNCTION__)
void _streamDebugStart(Stream *self, size_t sizeExcepted, const char *function);
#define streamDebugEnd(stream) \
    _streamDebugEnd(stream, __FUNCTION__)
bool _streamDebugEnd(Stream *self, const char *function);

/**
 * Debugging
 */
void streamPrint(Stream *self);

/**
 * Setters / Getters
 */
inline void streamResetPosition(Stream *self) { self->position = 0; }
