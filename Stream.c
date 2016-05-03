/**
 * @license GNU GENERAL PUBLIC LICENSE - Version 2, June 1991
 *          See LICENSE file for further information
 */


// ---------- Includes ------------
#include "Stream.h"


// ------ Structure declaration -------


// ------ Static declaration -------


// ------ Extern function implementation ------

extern inline void streamResetPosition(Stream *self);

Stream *streamNew(void *buffer, size_t size) {
    Stream *self;

    if ((self = malloc(sizeof(Stream))) == NULL) {
        error("Cannot allocate enough space.");
        return NULL;
    }

    streamInit(self, buffer, size);

    return self;
}

void streamInit(Stream *self, void *buffer, size_t size) {
    memset(self, 0, sizeof(*self));
    self->buffer = buffer;
    self->position = 0;
    self->size = size;
    self->sizeExcepted = 0;
    self->function = NULL;
}

void streamAppend(Stream *self, void *data, size_t dataSize) {

    if (self->position + dataSize > self->size) {
        error("Error, cannot get data from the stream in '%s' because it exceeds the stream size '%d' ('%d' requested).",
              self->function, self->size, self->position + dataSize);
        return;
    }

    memcpy(&self->buffer[self->position], data, dataSize);
    self->position += dataSize;
    self->sizeExcepted -= dataSize;
}

void *streamGetCurrentBuffer(Stream *self) {
    return &self->buffer[self->position];
}

void *streamGetBuffer(Stream *self) {
    return self->buffer;
}

void streamAddOffset(Stream *self, unsigned int offset) {
    self->position += offset;
}

void streamGet(Stream *self, void *data, size_t dataSize) {

    if (self->position + dataSize > self->size) {
        error("Error, cannot get data from the stream in '%s' because it exceeds the stream size '%d' ('%d' requested).",
              self->function, self->size, self->position + dataSize);
        return;
    }

    memcpy(data, &self->buffer[self->position], dataSize);
    self->position += dataSize;
    self->sizeExcepted -= dataSize;
}

void streamDestroy(Stream **_self) {
    Stream *self = *_self;

    free(self);
    *_self = NULL;
}

size_t streamGetPosition(Stream *self) {
    return self->position;
}

size_t streamGetSize(Stream *self) {
    return self->size - self->position;
}

void streamPrint(Stream *self) {
    buffer_print(self->buffer, self->size, "Stream Total : ");
    buffer_print(streamGetCurrentBuffer(self), streamGetSize(self), "Stream Current : ");
}

void _streamDebugStart(Stream *self, size_t sizeExcepted, const char *function) {
    self->sizeExcepted = sizeExcepted;
    self->function = function;
}

bool _streamDebugEnd(Stream *self, const char *function) {
    if (self->sizeExcepted != 0) {
        error("Stream didn't write the expected size in '%s'. Offset = %d", function, self->sizeExcepted);
        return false;
    }

    return true;
}

